/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   13.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file precedence_table.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro správu precedenční tabulky.
 * @details Tento soubor deklaruje funkce a struktury potřebné pro práci
 *          s precedenční tabulkou v syntaktické analýze. Tabulka definuje
 *          pravidla pro operace a rozhoduje o pořadí výpočtů a zpracování
 *          výrazů v precedenční syntaktické analýze. Poskytuje základní
 *          prostředky pro určení precedencí a asociativit mezi operátory
 *          jazyka IFJ24.
 */

#ifndef PREC_TABLE_H_
/** @cond  */
#define PREC_TABLE_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "lltable.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define PREC_TERMINAL_COUNT 21              /**< Celkový počet terminálů v precedenční SA. */

#define FOLLOW_NON_TERMINALS_COUNT 9        /**< Počet NEterminálů, ze kterých jde přejít do precedenční SA. */
#define MAX_DOLLARS_IN_SET 2                /**< Maximální počet "dolar" terminálů v množině FOLLOW. */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčet terminálů používaných v precedenční syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v precedenční syntaktické analýze.
 */
typedef enum PrecTerminals {
    T_PREC_UNDEFINED             = -1,       /**<  Aktuální precedenční terminál je zatím neznámý */
    T_PREC_ID                    = 0,        /**<  Identifikátor  */
    T_PREC_INT_LITERAL           = 1,        /**<  Literál typu i32  */
    T_PREC_FLOAT_LITERAL         = 2,        /**<  Literál typu i64  */
    T_PREC_STRING_LITERAL        = 3,        /**<  Literál typu []u8  */
    T_PREC_NULL_LITERAL          = 4,        /**<  Literál typu NULL  */
    T_PREC_IFJ                   = 5,        /**<  Klíčové slovo "ifj"  */
    T_PREC_DOT                   = 6,        /**<  Symbol tečky "."  */
    T_PREC_LEFT_BRACKET          = 7,        /**<  Symbol levé závorky "("  */
    T_PREC_RIGHT_BRACKET         = 8,        /**<  Symbol pravé závorky ")"  */
    T_PREC_PLUS                  = 9,        /**<  Operátor součtu "+"  */
    T_PREC_MINUS                 = 10,       /**<  Operátor rozdílu "-"  */
    T_PREC_MULTIPLICATION        = 11,       /**<  Operátor součinu "*"  */
    T_PREC_DIVISION              = 12,       /**<  Operátor podílu "/"  */
    T_PREC_IDENTITY              = 13,       /**<  Operátor rovnosti "=="  */
    T_PREC_NOT_EQUAL             = 14,       /**<  Operátor nerovnosti "!="  */
    T_PREC_LESS_THAN             = 15,       /**<  Operátor menší než "<"  */
    T_PREC_GREATER_THAN          = 16,       /**<  Operátor větší než ">"  */
    T_PREC_LESS_THAN_OR_EQUAL    = 17,       /**<  Operátor menší rovno "<="  */
    T_PREC_GREATER_THAN_OR_EQUAL = 18,       /**<  Operátor větší rovno ">="  */
    T_PREC_COMMA                 = 19,       /**<  Symbol čárky "," uvnitř argumentů volání funkce  */
    T_PREC_DOLLAR                = 20,       /**<  Řízení parsování předáno LL parseru */
} PrecTerminals;

/**
 * @brief Precedence operandů a operátorů používané při precedenční syntaktické analýze.
 *
 * @details Tento výčtový typ definuje různé hodnoty precedence, které se používají
 *          při precedenční syntaktické analýze pro určení vztahů mezi terminály.
 */
typedef enum Precedence {
    P_SYNTAX_ERROR  = -1,          /**<  Chyba syntaxe (takovéto pravidlo se v tabulce nevyskytuje)  */
    P_EQUAL         = 0,           /**<  Operátory mají stejnou prioritu  */
    P_LESS          = 1,           /**<  Operátor vlevo má nižší prioritu než operátor vpravo  */
    P_GREATER       = 2,           /**<  Operátor vlevo má vyšší prioritu než operátor vpravo  */
} Precedence;

/**
 * @brief Výčet neterminálů, ze kterých se dá předat řízení precedenčnímu
 *        syntaktickému analyzátoru.
 *
 * @details Tento výčet obsahuje neterminály, které mohou předat řízení
 *          precedenčnímu syntaktickému analyzátoru během LL syntaktické analýzy.
 */
typedef enum CallPrecNonTerminals {
    CALL_PREC_NT_UNDEFINED      = -1,       /**< Aktuální "follow" neterminál je zatím neznámý. */
    CALL_PREC_NT_VAR_DEF        = 0,        /**< Neterminál pro definici proměnné */
    CALL_PREC_NT_STATEMENT_REST = 1,        /**< Neterminál pro zbytek příkazu */
    CALL_PREC_NT_THROW_AWAY     = 2,        /**< Neterminál pro zahození návratové hodnoty */
    CALL_PREC_NT_IF             = 3,        /**< Neterminál pro podmínku if */
    CALL_PREC_NT_WHILE          = 4,        /**< Neterminál pro cyklus while */
    CALL_PREC_NT_RETURN_REST    = 5,        /**< Neterminál pro zbytek příkazu návratu */
    CALL_PREC_NT_ARGUMENTS      = 6,        /**< Neterminál pro argumenty funkce */
    CALL_PREC_NT_ARG_LIST       = 7,        /**< Neterminál pro seznam argumentů */
    CALL_PREC_NT_ARG            = 8,        /**< Neterminál pro argument */
} CallPrecNonTerminals;

typedef enum DollarTerminals {
    CURRENT_DOLLAR_UNDEFINED    = -1,       /**< Aktuální ukončovací terminál je zatím neznámý. */
    DOLLAR_T_SEMICOLON          = 0,        /**< Neterminál pro zahození návratové hodnoty */
    DOLLAR_T_RIGHT_BRACKET      = 1,        /**< Neterminál pro zbytek příkazu */
    DOLLAR_T_COMMA              = 2,        /**< Neterminál pro podmínku if */
} DollarTerminals;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura pro reprezentaci precedence v precedenční tabulce.
 *
 * @details Tato struktura obsahuje klíč a pole hodnot, které reprezentují
 *          precedence operátorů a operandů používané při precedenční syntaktické analýze.
 *
 * @note V tabulce je řádek značen terminálem na vrcholu zásobníku. Kódem tohoto
 *       terminálu je tabulka indexována. Sloupce tabulky odpovídají aktuálnímu
 *       terminálu na vstupu. Souřadnice [Terminál na vrcholu zásobníku, Terminál na vstupu]
 *       určují vztah precedence mezi dvěma terminály.
 */
struct PrecedenceTable{
    PrecTerminals key;                        /**<  Klíč pro identifikaci terminálu v tabulce  */
    Precedence value[PREC_TERMINAL_COUNT];    /**<  Pole hodnot reprezentující precedence mezi terminály  */
};

/**
 * @brief Struktura reprezentující FOLOW množinu pro NEterminál.
 *
 * @details Tato struktura obsahuje NEterminál, počet terminálů v konkrétní
 *          FOLOW množině a pole terminálů, které tvoří FOLOW množinu.
 */
typedef struct PrecDollars {
    CallPrecNonTerminals fromNonTerminal;           /**< NEterminál, ze kterého je FOLOW množina určena */
    DollarTerminals followSet[MAX_DOLLARS_IN_SET];  /**< Množinina FOLLOW obsahující "dollar" terminály předávající řízení zpět LL parseru */
} PrecDollars;


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Externí deklarace precedenční tabulky.
 *
 * @details V tabulce je příslušný řádek pro každý terminál, jejichž kódem je
 *          tabulka indexována. Sloupce tabulky odpovídají jednotlivým
 *          terminálům. Souřadnice [Terminál na vrcholu zásobníku, Terminál na vstupu]
 *          určují vztah precedence mezi dvěma terminály.
 */
extern const struct PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT];

/**
 * @brief Pole FOLLOW množin pro všechny NEterminály, ze kterých se dá předat
 *        řízení precedenčnímu syntaktickému analyzátoru.
 *
 * @details Toto pole obsahuje FOLOW množiny pro všechny NEterminály, ze kterýCH
 *          se dá předat řízení precedenčnímu SA, s explicitním počtem symbolů
 *          v každé množině.
 */
extern const struct PrecDollars followSets[FOLLOW_NON_TERMINALS_COUNT];

/**
 * @brief Globální proměnná pro aktuální množinu dollar neterinálů.
 *
 * @details Tato proměnná uchovává aktuální množinu dollar terminálů, které
 *          jsou používán precedenčním parserem. Je inicializována na
 *          @c CURRENT_DOLLAR_UNDEFINED a aktualizována během zahájení
 *          precedenční syntaktické analýzy.
 */
extern DollarTerminals currentDollar[MAX_DOLLARS_IN_SET];


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v precedenční tabulce na základě terminálů.
 *
 * @details Tato funkce používá kód terminálu na vrcholu zásobníku a terminálu
 *          na vstupu k vyhledání odpovídajícího pravidla v precedenční tabulce.
 *          Pokud funkce najde naprosto neočekávaný řádek tabulky, zahlásí
 *          @c ERROR_INTERNAL.
 *
 * @param[in] stackTopTerminal Terminál na vrcholu zásobníku sloužící jako index
 *                             do precedenční tabulky.
 * @param [in] inputTerminal Terminál na vstupu.
 * @param [out] precedence Ukazatel na proměnnou, do které bude uložena nalezená precedence.
 */
void PrecTable_findPrecedence(PrecTerminals stackTopTerminal, PrecTerminals inputTerminal, Precedence *precedence);

/**
 * @brief Získá FOLOW množinu pro daný NEterminál pomocí binárního vyhledávání
 *        a aktualizuje globální proměnnou.
 *
 * @details Pokud nebyla množina nalezena, zavolá funkci @c error_handle()
 *          s chybovým kódem @c ERROR_INTERNAL.
 *
 * @param [in] fromNonTerminal NEterminál, pro který se získává množina FOLOW.
 *
 * @note Funkce aktualizuje globální proměnnou @c currentFollowSet.
 */
void PrecTable_getFollowSet(LLNonTerminals fromNonTerminal);

/**
 * @brief Kontroluje, zda je daný terminál v aktuální FOLLOW množině.
 *
 * @param [in] terminal Terminál, který se má zkontrolovat.
 * @return @c true, pokud je terminál v aktuální FOLLOW množině, jinak @c false.
 */
bool PrecTable_isInFollowSet(DollarTerminals terminal);


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO INTERNÍ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Mapuje neterminál z LL syntaktické analýzy na neterminál pro předání
 *        řízení precedenčnímu syntaktickému analyzátoru.
 *
 * @details Tato funkce převede neterminál z LL syntaktické analýzy
 *          (`LLNonTerminals`) na odpovídající neterminál pro předání řízení
 *          precedenčního syntaktického analyzátoru (`CallPrecNonTerminals`)
 *          zpět LL parseru.
 *
 * @note V případě úspěchu vrací výsledek parametrem. V případě obdržení
 *       nenamapovatelného neterminálu volá funkci @c error_handle() s chybovým
 *       kódem @c ERROR_INTERNAL.
 *
 * @param [in] fromNonTerminal Neterminál z LL syntaktické analýzy, který se
 *                             má mapovat.
 * @param [out] followNonTerminal Ukazatel na proměnnou, do které bude uložen
 *                                odpovídající neterminál pro předání řízení
 *                                precedenčnímu syntaktickému analyzátoru.
 */
void PrecTable_mapFollowNonTerminal(LLNonTerminals fromNonTerminal, CallPrecNonTerminals *followNonTerminal);

#endif  // PREC_TABLE_H_

/*** Konec souboru precedence_table.h ***/
