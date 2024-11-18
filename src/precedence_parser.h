/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_parser.h                                       *
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
 * @file precedence_parser.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro precedenční syntaktický analyzátor.
 * @details Obsahuje deklarace funkcí a struktur pro práci s precedenčním
 *          syntaktickým analyzátorem, který zpracovává výrazy v jazyce IFJ24.
 *          Modul podporuje analýzu výrazů a operátorů podle precedenčních
 *          pravidel, včetně tvorby abstraktního syntaktického stromu (AST),
 *          který reprezentuje hierarchii operací.
 */

#ifndef PREC_PARSER_H_
/** @cond  */
#define PREC_PARSER_H_
/** @endcond  */

#include "parser.h"
#include "precedence_table.h"
#include "precedence_stack.h"
#include "frame_stack.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define NUM_OF_REDUCTION_RULES 20   /**< Počet různých redukčních pravidel. */
#define MAX_SYMBOLS_TO_REDUCE  6    /**< Maximální počet symbolů, nad kterými může být prováděna redukce. */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčet redukčních pravidel používaných při syntaktické analýze.
 *
 * @details Tento výčtový typ definuje všechna možná redukční pravidla, která
 *          jsou využívána během precedenční syntaktické analýzy. Každé
 *          pravidlo reprezentuje jednu možnou redukci v syntaktickém stromu.
 */
typedef enum ReductionRule {
    REDUCE_RULE_UNDEFINED           = -1,     /**<  Zatím nebylo zvoleno žádné redukční pravidlo */
    REDUCE_E_ID                     = 0,      /**<  E -> id                        */
    REDUCE_E_LITERAL                = 1,      /**<  E -> literal                   */
    REDUCE_E_PLUS_E                 = 2,      /**<  E -> E + E                     */
    REDUCE_E_MINUS_E                = 3,      /**<  E -> E - E                     */
    REDUCE_E_MULT_E                 = 4,      /**<  E -> E * E                     */
    REDUCE_E_DIV_E                  = 5,      /**<  E -> E / E                     */
    REDUCE_E_IDENTITY_E             = 6,      /**<  E -> E == E                    */
    REDUCE_E_NOT_EQUAL_E            = 7,      /**<  E -> E != E                    */
    REDUCE_E_LESS_THAN_E            = 8,      /**<  E -> E < E                     */
    REDUCE_E_GREATER_THAN_E         = 9,      /**<  E -> E > E                     */
    REDUCE_E_LESS_EQUAL_E           = 10,     /**<  E -> E <= E                    */
    REDUCE_E_GREATER_EQUAL_E        = 11,     /**<  E -> E >= E                    */
    REDUCE_E_INTO_BRACKETS          = 12,     /**<  E -> ( E )                     */
    REDUCE_E_FUN_CALL               = 13,     /**<  E -> id ( <ARGUMENTS> )        */
    REDUCE_E_IFJ_CALL               = 14,     /**<  E -> ifj . id ( <ARGUMENTS> )  */
    REDUCE_ARGUMENTS_TO_ARG_LIST    = 15,     /**<  <ARGUMENTS> -> <ARG_LIST>      */
    REDUCE_ARG_LIST_TO_E_ARG        = 16,     /**<  <ARG_LIST> -> E <ARG>          */
    REDUCE_ARG_TO_COMMA_E_ARG       = 17,     /**<  <ARG> -> , E <ARG>             */
} ReductionRule;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura pro uchování symbolů určených k redukci.
 *
 * @details Tato struktura slouží k uchování pole zásobníkových symbolů,
 *          které jsou určeny k redukci, a počtu těchto symbolů. Struktura
 *          obsahuje pole symbolů a proměnnou pro uchování aktuálního počtu
 *          symbolů k redukci.
 */
typedef struct ReduceSymbolArray {
    PrecStackSymbol symbolArr[MAX_SYMBOLS_TO_REDUCE];   /**< Pole zásobníkových symbolů určených k redukci*/
    size_t symbolCount;                                 /**< Aktuální počet symbolů k redukci */
} ReduceSymbolArray;

/**
 * @brief Struktura pro uchování redukčního pravidla a odpovídající sekvence
 *        symbolů určených k redukci.
 *
 * @details Tato struktura slouží k uchování jednoho redukčního pravidla a
 *          odpovídající sekvence symbolů, které jsou určeny k redukci.
 *          Struktura obsahuje redukční pravidlo a pole symbolů, které
 *          odpovídají tomuto pravidlu.
 */
typedef struct ReductionRuleSet {
    ReductionRule   rule;                               /**< Redukční pravidlo */
    PrecStackSymbol symbolArr[MAX_SYMBOLS_TO_REDUCE];   /**< Pole symbolů odpovídajících redukčnímu pravidlu */
} ReductionRuleSet;


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Pole struktur pro uchování všech redukčních pravidel a odpovídajících sekvencí symbolů.
 *
 * @details Tato globální proměnná obsahuje pole struktur typu `ReductionRuleSet`,
 *          kde každá struktura uchovává jedno redukční pravidlo a odpovídající
 *          sekvenci symbolů, které jsou určeny k redukci. Pole obsahuje všechny
 *          definované redukční pravidla používané při syntaktické analýze.
 */
extern const struct ReductionRuleSet reductionRuleSet[NUM_OF_REDUCTION_RULES];


/*******************************************************************************
 *                                                                             *
 *                    VEŘEJNÉ FUNKCE PRECEDENČNÍHO PARSERU                     *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Spustí precedenční syntaktickou analýzu.
 *
 * @details Tato funkce spustí precedenční syntaktickou analýzu na základě
 *          aktuálního stavu zásobníku a vstupního tokenu.
 *
 * @param fromNonTerminal Neterminál předávající řízení precedenčnímu syntaktickému analyzátoru.
 */
AST_ExprNode *PrecParser_parse(LLNonTerminals fromNonTerminal);


/*******************************************************************************
 *                                                                             *
 *                    INTERNÍ FUNKCE PRECEDENČNÍHO PARSERU                     *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vybere redukční pravidlo na základě aktuálního stavu zásobníku.
 *
 * @details Tato funkce prochází zásobník, dokud nenarazí na symbol `HANDLE`,
 *          a na základě aktuálního stavu a symbolů na zásobníku určí, které
 *          redukční pravidlo použít. Pokud pro danou sekvenci symbolů pravidlo
 *          neexistuje, dojde k chybě.
 *
 * @param[out] rule Ukazatel na proměnnou, do které bude uloženo nalezené redukční pravidlo.
 */
void precParser_chooseReductionRule(ReductionRule *rule);

/**
 * @brief Provede redukci na základě daného redukčního pravidla.
 *
 * @details Tato funkce provede redukci na základě daného redukčního pravidla
 *          a aktualizuje syntaktický strom.
 *
 * @param rule Redukční pravidlo, které se má použít.
 */
void PrecParser_reduce(ReductionRule rule);


/*******************************************************************************
 *                                                                             *
 *                   DEKLARACE FUNKCÍ PRO REDUKČNÍ PRAVIDLA                    *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Redukce podle pravidla E -> id nebo E -> literal.
 *
 * @details Tato funkce provádí redukci pro pravidla E -> id nebo E -> literal.
 *          Popne uzel ze zásobníku, vytvoří AST uzel pro proměnnou nebo literál
 *          a výraz, a poté pushne nový neterminál E na zásobník s vytvořeným
 *          AST uzlem.
 *
 * @param nodeType Typ uzlu, který má být vytvořen. Tento parametr určuje, zda
 *                 bude vytvořen uzel pro proměnnou (AST_VAR_NODE) nebo literál
 *                 (AST_LITERAL_NODE).
 */
void PrecParser_reduceVarOrLit(AST_NodeType nodeType);

/**
 * @brief Redukce pro binární operaci E -> E op E.
 *
 * @details Tato funkce provádí redukci pro binární operaci E -> E op E. Popne
 *          uzly pro pravý operand, operátor a levý operand ze zásobníku,
 *          vytvoří AST uzel pro binární operaci a výraz, a poté pushne nový
 *          neterminál (výraz) E na zásobník s vytvořeným AST uzlem.
 *
 * @param binOp Typ binární operace, která má být provedena. Tento parametr
 *                 určuje konkrétní operátor (např. +, -, *, /, ==, !=, <, >,
 *                 <=, >=), který bude použit při vytváření AST uzlu pro binární
 *                 operaci.
 */
void PrecParser_reduceBinOp(AST_BinOpType binOp);

/**
 * @brief Redukce pro výraz v závorkách E -> ( E ).
 *
 * @details Tato funkce provádí redukci pro výraz v závorkách E -> ( E ). Popne
 *          uzly pro pravou závorku, výraz a levou závorku ze zásobníku, a poté
 *          pushne výraz zpět na zásobník bez změny.
 */
void PrecParser_reduceBrackets();

/**
 * @brief Redukce pro volání funkce E -> id ( <ARGUMENTS> ).
 *
 * @details Tato funkce provádí redukci pro volání funkce E -> id ( <ARGUMENTS> ).
 *          Popne uzly pro pravou závorku, argumenty, levou závorku a identifikátor
 *          ze zásobníku, vytvoří AST uzel pro volání funkce a výraz, a poté pushne
 *          nový neterminál (výraz) E na zásobník s vytvořeným AST uzlem.
 */
void PrecParser_reduceFunCall();

/**
 * @brief Redukce pro volání vestavěné funkce E -> ifj . id ( <ARGUMENTS> ).
 *
 * @details Tato funkce provádí redukci pro volání vestavěné funkce E -> ifj . id ( <ARGUMENTS> ).
 *          Popne uzly pro pravou závorku, argumenty, levou závorku, identifikátor,
 *          tečku a 'ifj' ze zásobníku, vytvoří AST uzel pro volání funkce a výraz,
 *          a poté pushne nový neterminál (výraz) E na zásobník s vytvořeným AST uzlem.
 */
void PrecParser_reduceIfjFunCall();

/**
 * @brief Redukce pro argumenty <ARGUMENTS> -> <ARG_LIST>.
 *
 * @details Tato funkce provádí redukci pro argumenty <ARGUMENTS> -> <ARG_LIST>.
 *          Popne uzel pro seznam argumentů ze zásobníku a poté pushne nový
 *          neterminál <ARGUMENTS> na zásobník s uzlem argumentů.
 */
void PrecParser_reduceArguments();

/**
 * @brief Redukce pro seznam argumentů <ARG_LIST> -> E <ARG>.
 *
 * @details Tato funkce provádí redukci pro seznam argumentů <ARG_LIST> -> E <ARG>.
 *          Popne uzly pro argument a výraz ze zásobníku, vytvoří nový uzel pro
 *          argument, a poté pushne nový neterminál <ARG_LIST> na zásobník
 *          s uzlem argumentů.
 */
void PrecParser_reduceArgList();

/**
 * @brief Redukce pro argument <ARG> -> , E <ARG>.
 *
 * @details Tato funkce provádí redukci pro argument <ARG> -> , E <ARG>.
 *          Popne uzly pro argument, výraz a čárku ze zásobníku, vytvoří nový
 *          uzel pro argument, a poté pushne nový neterminál <ARG> na zásobník
 *          s uzlem argumentů.
 */
void PrecParser_reduceArg();


/*******************************************************************************
 *                                                                             *
 *                POMOCENÉ INTERNÍ FUNKCE PRECEDENČNÍHO PARSERU                *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Mapuje typ aktuálního Tokenu na odpovídající typ Terminálu pro
 *        precedenční analýzu.
 *
 * @details Tato funkce převede typ globálního tokenu @c currentToken na
 *          odpovídající typ terminálu používaný v precedenční syntaktické
 *          analýze. Funkce využívá globální proměnnou @c currentToken, která
 *          obsahuje aktuální token získaný ze scanneru.
 *
 * @param [in] bracketDepth Hloubka zanoření závorek, která ovlivňuje mapování.
 * @param [out] terminal Ukazatel na proměnnou, do které bude uloženo namapované terminál.
 */
void PrecParser_mapToDollar(int bracketDepth, PrecTerminals *terminal);

/**
 * @brief Mapuje symbol na redukční pravidlo.
 *
 * @details Tato funkce mapuje symbol na odpovídající redukční pravidlo
 *          používané v precedenční syntaktické analýze.
 *
 * @param symbol Symbol, který se má mapovat.
 * @param rule Ukazatel na proměnnou, do které bude uloženo odpovídající redukční pravidlo.
 */
void PrecParser_mapNonTerminalToRule(PrecStackSymbol symbol, ReductionRule *rule);

#endif // PREC_PARSER_H_

/*** Konec souboru precedence_parser.h ***/
