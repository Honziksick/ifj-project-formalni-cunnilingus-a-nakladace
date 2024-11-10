/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   10.11.2024                                                *
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
 * @author Jan Kalina   \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro správu precedenční tabulky pro precedenční SA.
 * @details Tento hlavičkový soubor obsahuje deklarace funkcí a datových
 *          struktur potřebných pro správu precedenční tabulky pro precedenční SA.
 *          Precedenční tabulka obsahuje pravidla pro zpracování výrazů.
 */

#ifndef PREC_TABLE_H_
/** @cond  */
#define PREC_TABLE_H_
/** @endcond  */

#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define PREC_TERMINAL_COUNT 20  /**<  Celkový počet terminálů v precedenční SA. */


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
    int key;                           /**<  Klíč pro identifikaci terminálu v tabulce  */
    int value[PREC_TERMINAL_COUNT];    /**<  Pole hodnot reprezentující precedence mezi terminály  */
};


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
extern struct PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT];


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @enum PrecTerminals
 * @brief Výčet terminálů používaných v precedenční syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v precedenční syntaktické analýze.
 */
typedef enum PrecTerminals {
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
    T_PREC_GREATER_THAN_OR_EQUAL = 18,       /**<  Operátor větší rovno ">=""  */
    T_PREC_CALL_LL               = 19,       /**<  Řízení parsování předáno LL parseru */
} PrecTerminals;

/**
 * @enum Precedence
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
 *
 * @param[in] stackTopTerminal Terminál na vrcholu zásobníku sloužící jako index
 *                             do precedenční tabulky.
 * @param[in] inputTerminal Terminál na vstupu.
 *
 * @return Hodnota precedence pro dané terminály nebo hlásí chybu, pokud pravidlo
 *         není nalezeno.
 *
 * @note Pokud funkce najde naprosto neočekávaný řádek tabulky,
 *       zahlásí `ERROR_INTERNAL`.
 */
int PrecTable_findRule(int stackTopTerminal, int inputTerminal);

#endif  // PREC_TABLE_H_

/*** Konec souboru precedence_table.h ***/
