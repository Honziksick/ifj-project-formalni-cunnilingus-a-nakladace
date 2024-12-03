/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_parser.h                                       *
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
 * @file precedence_parser.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor s deklaracemi funkcí pro precedenční syntaktickou
 *        analýzu výrazů v jazyce IFJ24.
 * @details Tento soubor obsahuje deklarace funkcí a datových struktur potřebných
 *          pro implementaci precedenční syntaktické analýzy výrazů a operátorů
 *          v jazyce IFJ24. Precedenční syntaktický analyzátor zpracovává výrazy
 *          a operátory, implementuje algoritmy pro posun a redukci a vytváří
 *          abstraktní syntaktický strom (AST) pro další zpracování během překladu.
 *          V rámci precedenčního syntaktického analyzátoru je také možné
 *          analyzovat volání funkcí ve výrazech včetně jejich parametrů.
 */

#ifndef PREC_PARSER_H_
/** @cond  */
#define PREC_PARSER_H_
/** @endcond  */

// Import submodulů parseru
#include "parser_common.h"
#include "llparser.h"
#include "precedence_table.h"
#include "precedence_stack.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define NUM_OF_REDUCTION_RULES 18     /**< Počet redukčních pravidel. */
#define MAX_SYMBOLS_TO_REDUCE  5      /**< Maximální počet symbolů, nad kterými může být prováděna redukce +1 pro handle.          */
#define IS_BUILT_IN_FUNCTION   true   /**< Konstanta předávána funkci `reduceFunCall`, značící, že se jedná o vestavěnou funkci.   */
#define IS_USER_FUNCTION       false  /**< Konstanta předávána funkci `reduceFunCall`, značící, že se jedná o uživatelskou funkci. */


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
 *          pravidlo reprezentuje jednu možnou redukci prováděnou na precedenčním
 *          zásobníku. V rámci aplikací redukcí jsou vytvářeny uzly abstraktního
 *          syntaktického stromu (AST) typu @c AST_ExprNode pro výraz nebo
 *          @c AST_ArgOrParamNode pro seznam argumentů volání funkcí.
 */
typedef enum ReductionRule {
    REDUCE_RULE_UNDEFINED           = -1,     /**<  Zatím nebylo zvoleno žádné redukční pravidlo  */
    REDUCE_E_ID                     = 0,      /**<  E -> id                    */
    REDUCE_E_INT_LITERAL            = 1,      /**<  E -> i32_literal           */
    REDUCE_E_FLOAT_LITERAL          = 2,      /**<  E -> f64_literal           */
    REDUCE_E_STRING_LITERAL         = 3,      /**<  E -> []u8_literal          */
    REDUCE_E_NULL_LITERAL           = 4,      /**<  E -> null_literal          */
    REDUCE_E_PLUS_E                 = 5,      /**<  E -> E + E                 */
    REDUCE_E_MINUS_E                = 6,      /**<  E -> E - E                 */
    REDUCE_E_MULT_E                 = 7,      /**<  E -> E * E                 */
    REDUCE_E_DIV_E                  = 8,      /**<  E -> E / E                 */
    REDUCE_E_IDENTITY_E             = 9,      /**<  E -> E == E                */
    REDUCE_E_NOT_EQUAL_E            = 10,     /**<  E -> E != E                */
    REDUCE_E_LESS_THAN_E            = 11,     /**<  E -> E < E                 */
    REDUCE_E_GREATER_THAN_E         = 12,     /**<  E -> E > E                 */
    REDUCE_E_LESS_EQUAL_E           = 13,     /**<  E -> E <= E                */
    REDUCE_E_GREATER_EQUAL_E        = 14,     /**<  E -> E >= E                */
    REDUCE_E_INTO_BRACKETS          = 15,     /**<  E -> ( E )                 */
    REDUCE_E_FUN_CALL               = 16,     /**<  E -> id \<ARG_LIST>        */
    REDUCE_E_IFJ_CALL               = 17,     /**<  E -> ifj . id \<ARG_LIST>  */
} ReductionRule;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

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
    ReductionRule   key;                                /**< Redukční pravidlo */
    PrecStackSymbol symbolArr[MAX_SYMBOLS_TO_REDUCE];   /**< Pole (sekvence) symbolů odpovídajících redukčnímu pravidlu */
} ReductionRuleSet;


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE VEŘEJNÝCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Spustí precedenční syntaktickou analýzu.
 *
 * @details Tato funkce spustí precedenční syntaktickou analýzu na základě
 *          aktuálního stavu zásobníku a vstupního terminálu. Implementuje
 *          logiku pro posun a redukci ve výrazech a konstrukci abstraktního
 *          syntaktického stromu (AST).
 *
 * @note Konkrétní stav případné chyby, ke které v rámci precedenční syntaktické
 *       analýzy došlo, je zaznamenáván funkcí @c parser_errorWatcher().
 *
 * @warning Pokud dojde během precedenční analýzy k chybě, je nutné uvolnit
 *          všechny v rámci precedenční analýzy alokované zdroje před předaním
 *          řízení zpět LL syntaktickému analyzátoru.
 *
 * @param [in] fromNonTerminal Neterminál předávající řízení precedenčnímu
 *                             syntaktickému analyzátoru.
 *
 * @return Ukazatel na kořen nového výrazu typu @c AST_ExprNode, @c NULL v případě
 *         prázdného výrazu nebo @c PARSING_ERROR (tj. NULL) v případě chyby při
 *         syntaktické analýze.
 */
AST_ExprNode *PrecParser_parse(LLNonTerminals fromNonTerminal);


/*******************************************************************************
 *                                                                             *
 *        DEKLARACE INTERNÍCH FUNKCÍ PRO ALGORITMY APLIKACÍ PRECEDENCÍ         *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Zpracuje terminál s precedencí @c P_EQUAL.
 *
 * @details Tato funkce zpracovává terminál s precedencí @c P_EQUAL podle
 *          algoritmu precedenční syntaktické analýzy. Pushne inicializovaný
 *          uzel na zásobník a čte další symbol ze vstupu. Aktualizuje terminály
 *          na vrcholu zásobníku a na vstupu pro další iteraci.
 *
 * @param [in] bracketDepth Úroveň zanoření závorek.
 * @param [in] dollarContext Kontextový terminál typu @c DollarTerminals.
 * @param [in,out] inTerminal Ukazatel na aktuální vstupní terminál.
 *                            Hodnota může být v rámci funkce aktualizována.
 * @param [in,out] topTerminal Ukazatel na terminál na vrcholu zásobníku.
 *                             Hodnota může být v rámci funkce aktualizována.
 *
 * @note Funkce modifikuje přes odkaz předané proměnné @c inTerminal a
 *       @c topTerminal pro další použití v hlavním cyklu precedenčního
 *       syntaktického analyzátoru uvnitř funkce @c PrecParser_parse().
 */
void PrecParser_applyEqualPrecedence(int bracketDepth, DollarTerminals dollarContext, \
                                     PrecTerminals *inTerminal, PrecTerminals *topTerminal);

/**
 * @brief Zpracuje terminál s precedencí @c P_LESS.
 *
 * @details Tato funkce zpracovává terminál s precedencí @c P_LESS podle
 *          algoritmu precedenční syntaktické analýzy. Pushne handle za první
 *          terminál na zásobníku a čte další symbol ze vstupu. Aktualizuje
 *          terminály na vrcholu zásobníku a na vstupu pro další iteraci.
 *
 * @note Funkce modifikuje přes odkaz předané proměnné @c inTerminal a
 *       @c topTerminal pro další použití v hlavním cyklu precedenčního
 *       syntaktického analyzátoru uvnitř funkce @c PrecParser_parse().
 *
 * @param [in] bracketDepth Úroveň zanoření závorek.
 * @param [in] dollarContext Kontextový terminál typu @c DollarTerminals.
 * @param [in,out] inTerminal Ukazatel na aktuální vstupní terminál.
 *                            Hodnota může být v rámci funkce aktualizována.
 * @param [in,out] topTerminal Ukazatel na terminál na vrcholu zásobníku.
 *                             Hodnota může být v rámci funkce aktualizována.
 */
void PrecParser_applyLessPrecedence(int bracketDepth, DollarTerminals dollarContext, \
                                    PrecTerminals *inTerminal, PrecTerminals *topTerminal);

/**
 * @brief Zpracuje terminál s precedencí @c P_GREATER.
 *
 * @details Tato funkce zpracovává terminál s precedencí @c P_GREATER podle
 *          algoritmu precedenční syntaktické analýzy. Vybere vhodné redukční
 *          pravidlo a aplikuje redukci na zásobníku. Aktualizuje terminály na
 *          vrcholu zásobníku a na vstupu pro další iteraci.
 *
 * @note Funkce modifikuje přes odkaz předané proměnné @c inTerminal a
 *       @c topTerminal pro další použití v hlavním cyklu precedenčního
 *       syntaktického analyzátoru uvnitř funkce @c PrecParser_parse().
 *
 * @param [in] bracketDepth Úroveň zanoření závorek.
 * @param [in] fromNonTerminal Neterminál předávající řízení precedenčnímu
 *                             syntaktickému analyzátoru.
 * @param [in] dollarContext Kontextový terminál typu @c DollarTerminals.
 * @param [in,out] inTerminal Ukazatel na aktuální vstupní terminál.
 *                            Hodnota může být v rámci funkce aktualizována.
 * @param [in,out] topTerminal Ukazatel na terminál na vrcholu zásobníku.
 *                             Hodnota může být v rámci funkce aktualizována.
 *
 * @return Vrací @c true, pokud došlo ke snížení hodnoty úrovně zanoření na 0 a
 *         aktuálním "dollar" terminálem je pravá závorka, což značí ukončení
 *         hlavní smyčky syntaktické analýzy. Jinak vrací @c false.
 */
bool PrecParser_applyGreaterPrecedence(int *bracketDepth, LLNonTerminals fromNonTerminal, \
                                       DollarTerminals dollarContext, PrecTerminals *inTerminal, \
                                       PrecTerminals *topTerminal);


/*******************************************************************************
 *                                                                             *
 *            DEKLARACE INTERNÍCH FUNKCÍ VOLBU REDUKČNÍHO PRAVIDLA             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vybere redukční pravidlo na základě aktuálního stavu zásobníku.
 *
 * @details Tato funkce prochází zásobník, dokud nenarazí na symbol @c HANDLE,
 *          a na základě aktuálního stavu a symbolů na zásobníku určí, které
 *          redukční pravidlo použít. Pokud pro danou sekvenci symbolů pravidlo
 *          neexistuje, nastaví syntaktickou chybu.
 *
 * @param [out] rule Ukazatel na proměnnou, do které bude uloženo nalezené
 *                   redukční pravidlo.
 */
void PrecParser_chooseReductionRule(ReductionRule *rule);

/**
 * @brief Vyhledá redukční pravidlo odpovídající dané sekvenci symbolů.
 *
 * @details Tato funkce porovnává předanou sekvenci symbolů s definovanými
 *          redukčními pravidly a v případě shody vrací odpovídající pravidlo.
 *          Pokud není nalezena shoda, nastaví syntaktickou chybu.
 *
 * @param [in] symbolsToReduce Pole symbolů určených k redukci.
 * @param [out] rule Ukazatel na proměnnou, do které bude uloženo nalezené
 *                   redukční pravidlo.
 */
void PrecParser_findReductionRule(PrecStackSymbol *symbolsToReduce, ReductionRule *rule);

/**
 * @brief Aplikuje redukci na zásobníku podle zvoleného redukčního pravidla.
 *
 * @details Tato funkce volá specializovanou redukční funkci na základě
 *          zvoleného redukčního pravidla. Modifikuje zásobník a případně
 *          vytváří nové uzly abstraktního syntaktického stromu (AST).
 *
 * @param [in] rule Redukční pravidlo, které se má aplikovat.
 */
void PrecParser_reduce(ReductionRule rule);

/**
 * @brief Aplikuje zbývající redukce, dokud nebude vrcholovým terminálem "dollar".
 *
 * @details Tato funkce vybere redukční pravidlo a provede redukci na základě
 *          zvoleného pravidla. Pokud dojde k chybě během výběru pravidla,
 *          redukce nebo mapování vrcholového terminálu, funkce vrátí @c false.
 *
 * @param topTerminal Ukazatel na terminál na vrcholu zásobníku.
 * @return Vrací @c true, pokud redukce proběhly úspěšně, jinak @c false.
 */
bool PrecParser_applyRemainingReductions(PrecTerminals *topTerminal);


/*******************************************************************************
 *                                                                             *
 *         DEKLARACE INTERNÍCH FUNKCÍ PRO APLIKACI REDUKČNÍCH PRAVIDEL         *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Aplikuje redukci pro pravidla `E -> id` nebo `E -> literal`.
 *
 * @details Tato funkce zpracovává redukci pro identifikátor nebo literál.
 *          Vytváří nový AST uzel pro výraz a vloží ho na vrchol zásobníku.
 *
 * @param [in] nodeType Typ AST uzlu (`AST_VAR_NODE` nebo `AST_LITERAL_NODE`).
 */
void PrecParser_reduceVarOrLit(AST_NodeType nodeType);

/**
 * @brief Aplikuje redukci pro binární operace `E -> E op E`.
 *
 * @details Tato funkce zpracovává redukci pro binární operátory jako
 *          `+`, `-`, `*`, `/`, atd. Vytváří nový AST uzel pro binární operaci
 *          a vloží ho na vrchol zásobníku.
 *
 * @param [in] binOp Typ binární operace (např. `AST_OP_ADD`, `AST_OP_SUBTRACT`, ...).
 */
void PrecParser_reduceBinOp(AST_BinOpType binOp);

/**
 * @brief Aplikuje redukci pro výraz v závorkách `E -> ( E )`.
 *
 * @details Tato funkce zpracovává redukci závorek. Odstraní závorky a ponechá
 *          na zásobníku pouze výraz uvnitř závorek.
 */
void PrecParser_reduceIntoBrackets();

/**
 * @brief Aplikuje redukci pro volání funkce `E -> id \<ARG_LIST>` nebo
 *        `E -> ifj . id \<ARG_LIST>`.
 *
 * @details Tato funkce zpracovává redukci volání uživatelských nebo vestavěných
 *          funkcí. Vytváří nový AST uzel pro volání funkce a vloží ho na vrchol
 *          zásobníku.
 *
 * @param [in] isBuiltIn @c True, pokud se jedná o vestavěnou funkci,
 *                       jinak  @c false.
 *
 * @note Namísto vstupního parametru @c true lze využít exkvivalentní
 *       @c IS_BUILT_IN_FUNCTION a pro @c false ekvivalentní @c IS_USER_FUNCTION.
 */
void PrecParser_reduceFunCall(bool isBuiltIn);


/*******************************************************************************
 *                                                                             *
 *               DEKLARACE OSTATNÍCH INTERNÍCH POMOCNÝCH FUNKCÍ                *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Určuje, zda se v daném kontextu nechystáme analyzvat prázdný výraz.
 *
 * @details Tato funkce kontroluje, zda je výraz prázdný na základě aktuálního
 *          LL neterminálu a precedenčního terminálu.
 *
 * @note Používá se k detekci prázdných výrazů v kontextu argumentů funkcí
 *       nebo prázdných výrazů ve výrazech.
 *
 * @param [in] fromNonTerminal Neterminál, ze kterého je precedenční syntaktický
 *                             analyzátor volán.
 *
 * @return @c True, pokud se jedná o prázdný výraz, jinak @c false.
 */
bool PrecParser_parsingEmptyExpression(LLNonTerminals fromNonTerminal);

/**
 * @brief Určuje, zda by měla funkce pro výběr redukčního pravidla ukončit
 *        procházení zásobníku.
 *
 * @details Tato funkce kontroluje, zda bylo dosaženo konce při procházení
 *          zásobníku pro výběr redukčního pravidla, na základě vzdálenosti
 *          od vrcholu zásobníku a typu aktuálního zásobníkového uzlu.
 *
 * @param [in] distanceFromTop Vzdálenost od vrcholu zásobníku.
 * @param [in] stackNode Ukazatel na aktuální zásobníkový uzel.
 *
 * @return @c True, pokud by mělo procházení zásobníku skončit, jinak @c false.
 */
bool PrecParser_shouldEndRuleSelecetion(unsigned char distanceFromTop, PrecStackNode *stackNode);

/**
 * @brief Zkontroluje podmínky pro ukončení hlavní smyčky precedenční
 *        syntaktické analýzy.
 *
 * @details Tato funkce zkontroluje, zda došlo k chybě během syntaktické
 *          analýzy, zda je nastavena vlajka pro speciální ukončení po aplikaci
 *          precedence  @c P_GREATER, nebo zda jsou oba terminály rovny @c $.
 *          Pokud je splněna některá z těchto podmínek, funkce vrátí @c true,
 *          což znamená, že smyčka by měla být ukončena.
 *
 * @param [in] inTerminal Aktuální terminál.
 * @param [in] topTerminal Terminál na vrcholu zásobníku.
 * @param [in] greaterStopFlag Vlajka pro speciální ukončení po aplikaci @c P_GREATER.
 * @return Vrací @c true, pokud má být smyčka ukončena, jinak @c false.
 */
bool PrecParser_shouldStopParsingLoop(PrecTerminals inTerminal, PrecTerminals topTerminal, \
                                      bool greaterStopFlag);

#endif // PREC_PARSER_H_

/*** Konec souboru precedence_parser.h ***/
