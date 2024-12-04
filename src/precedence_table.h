/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   03.12.2024                                                *
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
 * @brief Hlavičkový soubor pro správu precedenční tabulky pro precedenční
 *        syntaktickou analýzu výrazů.
 * @details Tento soubor obsahuje deklarace funkcí a struktur potřebných pro
 *          práci s precedenční tabulkou v syntaktické analýze. Tabulka definuje
 *          precedence pro operace a rozhoduje o pořadí výpočtů a zpracování
 *          výrazů v precedenční syntaktické analýze. Poskytuje základní
 *          prostředky pro určení precedencí a asociativit mezi operátory
 *          jazyka IFJ24.
 */

#ifndef PREC_TABLE_H_
/** @cond  */
#define PREC_TABLE_H_
/** @endcond  */

// Import standardních knihoven jazyka C
#include <stdbool.h>

// Import sdílených knihoven překladače
#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define PREC_TERMINAL_COUNT 21              /**<  Celkový počet terminálů v precedenční syntaktické analýze.   */
#define FOLLOW_NON_TERMINALS_COUNT 7        /**<  Počet NEterminálů, ze kterých jde přejít do precedenční SA.  */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčet terminálů používaných v precedenční syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v precedenční syntaktické analýze. Každý terminál určuje nejen jeden
 *          řádek v precedenční tabulce (terminál na vrcholu zásobníku), ale
 *          také sloupec tabulky (vstupní terminál).
 */
typedef enum PrecTerminals {
    T_PREC_UNDEFINED             = -1,       /**<  Aktuální precedenční terminál je zatím neznámý   */
    T_PREC_ID                    = 0,        /**<  Identifikátor                                    */
    T_PREC_INT_LITERAL           = 1,        /**<  Literál typu i32                                 */
    T_PREC_FLOAT_LITERAL         = 2,        /**<  Literál typu i64                                 */
    T_PREC_STRING_LITERAL        = 3,        /**<  Literál typu []u8                                */
    T_PREC_NULL_LITERAL          = 4,        /**<  Literál typu NULL                                */
    T_PREC_IFJ                   = 5,        /**<  Klíčové slovo "ifj"                              */
    T_PREC_DOT                   = 6,        /**<  Symbol tečky "."                                 */
    T_PREC_LEFT_BRACKET          = 7,        /**<  Symbol levé závorky "("                          */
    T_PREC_RIGHT_BRACKET         = 8,        /**<  Symbol pravé závorky ")"                         */
    T_PREC_PLUS                  = 9,        /**<  Operátor součtu "+"                              */
    T_PREC_MINUS                 = 10,       /**<  Operátor rozdílu "-"                             */
    T_PREC_MULTIPLICATION        = 11,       /**<  Operátor součinu "*"                             */
    T_PREC_DIVISION              = 12,       /**<  Operátor podílu "/"                              */
    T_PREC_IDENTITY              = 13,       /**<  Operátor rovnosti "=="                           */
    T_PREC_NOT_EQUAL             = 14,       /**<  Operátor nerovnosti "!="                         */
    T_PREC_LESS_THAN             = 15,       /**<  Operátor menší než "<"                           */
    T_PREC_GREATER_THAN          = 16,       /**<  Operátor větší než ">"                           */
    T_PREC_LESS_THAN_OR_EQUAL    = 17,       /**<  Operátor menší rovno "<="                        */
    T_PREC_GREATER_THAN_OR_EQUAL = 18,       /**<  Operátor větší rovno ">="                        */
    T_PREC_COMMA                 = 19,       /**<  Symbol čárky "," uvnitř argumentů volání funkce  */
    T_PREC_DOLLAR                = 20,       /**<  Řízení parsování předáno LL parseru              */
} PrecTerminals;

/**
 * @brief Precedence operandů a operátorů používané při precedenční syntaktické
 *        analýze.
 *
 * @details Tento výčtový typ definuje různé hodnoty precedence, které se
 *          používají při precedenční syntaktické analýze pro určení vztahů
 *          mezi terminály. Dle zvolené precedence dochází buď k redukci nebo
 *          posuvu na základě algoritmu popisovaného na přednáškách.
 */
typedef enum Precedence {
    P_PRECEDENCE_UNDEFINED  = -2,       /**<  Aktuální precedence není zatím definována                   */
    P_SYNTAX_ERROR          = -1,       /**<  Chyba syntaxe (takovéto pravidlo se v tabulce nevyskytuje)  */
    P_EQUAL                 = 0,        /**<  Operátory mají stejnou prioritu                             */
    P_LESS                  = 1,        /**<  Operátor vlevo má nižší prioritu než operátor vpravo        */
    P_GREATER               = 2,        /**<  Operátor vlevo má vyšší prioritu než operátor vpravo        */
} Precedence;

/**
 * @brief Výčet neterminálů, ze kterých lze předat řízení nad syntaktickou
 *        analýzou z LL syntaktického analyzátoru do precedenčního syntaktického
 *        analyzátoru.
 *
 * @details Tento výčet obsahuje neterminály, které mohou předat řízení
 *          precedenčnímu syntaktickému analyzátoru během LL syntaktické analýzy.
 *          LL syntaktický analyzátor předává při volání precedenčního
 *          syntaktického analyzátoru, informaci o tom, z funkce pro jaký
 *          neterminál je precedenční syntaktický analyzátor. Na základě této
 *          informace je v rámci precedenční syntaktické analýzy možné namapovat
 *          správný ukončující "dollar" terminál pro daný kontext.
 */
typedef enum CallPrecNonTerminals {
    CALL_PREC_NT_UNDEFINED      = -1,       /**<  Předaný neterminál je zatím neznámý        */
    CALL_PREC_NT_VAR_DEF        = 0,        /**<  Neterminál pro definici proměnné           */
    CALL_PREC_NT_STATEMENT      = 1,        /**<  Neterminál pro příkaz (resp. return)       */
    CALL_PREC_NT_STATEMENT_REST = 2,        /**<  Neterminál pro zbytek příkazu              */
    CALL_PREC_NT_THROW_AWAY     = 3,        /**<  Neterminál pro zahození návratové hodnoty  */
    CALL_PREC_NT_IF             = 4,        /**<  Neterminál pro podmínku if                 */
    CALL_PREC_NT_WHILE          = 5,        /**<  Neterminál pro cyklus while                */
    CALL_PREC_NT_ARGUMENTS      = 6,        /**<  Neterminál pro argumenty funkce            */
} CallPrecNonTerminals;

/**
 * @brief  Výčet ukončujících "dollar" terminálů, na základě kterých je za
 *         splnění určitých podmínek předáno řízení zpět do LL syntaktického
 *         analyzátoru.
 *
 * @details Tento výčet obsahuje možné hodnoty "dollar" terminálů, které se
 *          používají v precedenční analýze. "Dollar" terminály jsou speciální
 *          terminály, které označují konec určité části syntaktické analýzy a
 *          předávají řízení zpět LL parseru. Každý "dollar" terminál má
 *          specifický význam a je použit v různých kontextech syntaktické
 *          analýzy, kdy například základním kontextem je typ NEterminálu, ze
 *          kterého bylo precedenčnímu syntaktickému analyzátoru předáno řízení.
 */
typedef enum DollarTerminals {
    CURRENT_DOLLAR_UNDEFINED    = -1,       /**<  Aktuuální "dollar" terminál je zatím neznámý  */
    DOLLAR_T_SEMICOLON          = 0,        /**<  "Dollar" terminálem je středník ";"           */
    DOLLAR_T_RIGHT_BRACKET      = 1,        /**<  "Dollar" terminálem je pravá závorka ")"      */
    DOLLAR_T_COMMA              = 2,        /**<  "Dollar" terminálem je čárka ","              */
} DollarTerminals;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura pro reprezentaci precedenční tabulky.
 *
 * @details Tato struktura obsahuje klíč a hodnotu, které reprezentují
 *          vztah mezi terminálem na vrcholu precedenčního zásobníku a
 *          vstupním terminálem.
 *
 * @note Tabulka je tvořena lokálně v rámci funkce @c PrecTable_findRule().
 *       Řádky i sloupce tabulky odpovídakí jednotlivým terminálům, jejichž
 *       hodnotou z výčtu @c PrecTerminals je tabulka indexována. Souřadnice
 *       [Terminál_na_vrcholu_zásobníku, Vstupní_terminál] určují volbu
 *       precedence.
 */
typedef struct PrecedenceTable {
    PrecTerminals key;                          /**< Klíč reprezentující hodnotu terminálu na vrcholu precedenčního zásobníku */
    Precedence value[PREC_TERMINAL_COUNT];      /**< Pole precedencní mezi terminálem na vrcholu precedenčního zásobníku a
                                                     vstupním terminálem. Indexem pole je hodnota vstupního terminálu.        */
} PrecedenceTable;


/*******************************************************************************
 *                                                                             *
 *                    DEKLARACE FUNKCÍ PRO VEŘEJNÉ VYUŽITÍ                     *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v precedenční tabulce na základě terminálu na vrcholu
 *        precedenčního zásobníku a vstupního terminálu.
 *
 * @details Tato funkce vytváří lokálně instanci precedenční tabulky. Používá
 *          hodnoty dvou terminálů k vyhledání odpovídající precedence mezi nimi.
 *          Tabulka je definována přímo v rámci funkce a indexována terminály.
 *          Pokud pravidlo neexistuje nebo kombinace terminálů není nalezena
 *          v tabulce, funkce vrací přes parametr nedefinované pravidlo
 *          @c RULE_UNDEFINED typu @c Precedence. V případě nalezení pravidla
 *          @c P_SYNTAX_ERROR zaznamenává syntaktickou chybu do funkce
 *          @c parser_errorWatcher() nebo v případě interní chyby zaznamenává
 *          výskyt interní chyby.
 *
 * @param [in] stackTopTerminal Terminál na vrcholu precedenčního zásobníku.
 * @param [in] inputTerminal Aktuální vstupní terminál.
 * @param [out] rule Ukazatel na proměnnou typu @c Precedence, kam bude uložena
 *                   nalezená precedence.
 */
void precTable_findPrecedence(PrecTerminals stackTopTerminal, PrecTerminals inputTerminal, \
                              Precedence *rule);

/**
 * @brief Namapuje NEterminál, ze kterého bylo předáno řízení precedenčnímu
 *        syntaktickému analyzátoru, na odpovídající "dollar" terminál.
 *
 * @details Funkce @c PrecTable_mapDollarTerminal() převede NEterminál na
 *          odpovídající "dollar" terminál, který se používá v precedenční
 *          analýze. To je důležité pro správné předání řízení zpět
 *          LL syntaktickému analyzátoru.
 *
 * @param [in] fromNonTerminal NEterminál typu @c LLNonTerminals, ze kterého
 *                             bylo předáno řízení z LL do precedenčnáho
 *                             syntaktického analyzátoru.
 * @param [out] currentDollar Ukazatel na proměnnou typu @c DollarTerminals,
 *                            kam bude uložen výsledek.
 */
void precTable_getDollarTerminalFromContext(LLNonTerminals fromNonTerminal, \
                                            DollarTerminals *currentDollar);

/**
 * @brief Mapuje typ vstupního precedenčního terminálu na "dollar" terminál.
 *
 * @details Funkce na základě kontextového "dollar" terminálu (určeného při
 *          zahájení precedenční syntaktické analýzy pomocí odvození od
 *          LL neterminál předávajícího řízení) a hloubky zanoření závorek (pro
 *          některé kontexty) namapuje vstupní terminál na "dollar" terminál.
 *          Tato funkce je v podstatě takovým filtrem, která při správném
 *          kontextu změní typ vstupního precedenčního terminálů na
 *          @c T_PREC_DOLLAR, nebo ho nechá nezměněný.
 *
 * @param [in] bracketDepth Hloubka zanoření závorek, která ovlivňuje mapování.
 * @param [in] dollarContext "Dollar" terminál pro aktuální kontext
 *                           precedenčního syntaktického analyzátoru.
 * @param [out] inTerminal Ukazatel na proměnnou, do které bude uložen
 *                         namapovaný typ vstupního precedenčního terminál.
 */
void precParser_mapInTerminalToDollar(int bracketDepth, DollarTerminals dollarContext, \
                                      PrecTerminals *inTerminal);

#endif  // PREC_TABLE_H_

/*** Konec souboru precedence_table.h ***/
