/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.h                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                   David Krejčí        <xkrejcd00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   01.12.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file tac_generator.h
 * @author Krejčí David \<xkrejcd00> (hlavní)
 * @author Farkašovský Lukáš \<xfarkal00> (edit)
 *
 * @brief Hlavičkový soubor pro generování tříadresného kódu (3AK) a cílového kódu.
 * @details Tento soubor obsahuje deklarace funkcí pro generování tříadresného
 *          kódu (3AK) a cílového kódu z abstraktního syntaktického stromu (AST).
 *          Deklarace zahrnují funkce pro generování kódu pro různé typy uzlů
 *          AST, jako jsou definice funkcí, výrazy, podmínky a smyčky. Obsažen
 *          je také výčtový typ pro režim generování kódu.
 */

#ifndef TAC_H_
/** @cond  */
#define TAC_H_
/** @endcond  */

// Import standardních knihoven jazyka C
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Import knihoven pro abstraktní syntaktický strom (AST)
#include "ast_nodes.h"
#include "ast_interface.h"

// Import knihoven pro práci s tabulkou symbolů
#include "frame_stack.h"

// Import sdílených knihoven překladače
#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define RESET_STATIC (AST_NodeType)123 /**< Typ pro reset statických proměnných */

#define MAX_BUFFER_SIZE 5           /**< Maximální velikost bufferu pro zpracování speciálních
                                         znaků ve funkci TAC_convertSpecialSymbols. */

#define OPTIMIZE_BUFFER_SIZE 2048   /**< Velikost bufferu pro optimalizaci */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Režim generování kódu
 *
 * @details Kvůli definicím proměnných v cyklech je nutné nejdříve všechny
 *          proměnné definovat a až poté generovat příkazy bez definic.
 *          Pokud nejsem v cyklu, tak generuji všechny příkazy.
 */
typedef enum TAC_mode {
    TAC_ALL,             /**< Generovat vše                       */
    TAC_VAR_DEF_ONLY,    /**< Generovat pouze definice proměnných */
    TAC_EXCEPT_VAR_DEF   /**< Generovat kromě definic proměnných  */
} TAC_mode;


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE VEŘEJNÝCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Generuje cílový kód programu ze stromu AST.
 *
 * @details Funkce prochází strom AST a rekurzivně volá funkci
 *          @c TAC_generateFunctionDefinition pro každou definici funkce.
 *          Pracuje s globálním kořenem stromu ASTroot.
 */
void TAC_generateProgram();


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE INTERNÍCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Generuje cílový kód definice funkce
 *
 * @details Přidá label a přidá rámec s parametry, následně zpracuje všechny
 *          instukce pomocí TAC_generateStatementBlock, odstraní rámec a volá return
 *
 * @param [in] funDefNode Ukazatel na uzel definice funkce
 */
void TAC_generateFunctionDefinition(AST_FunDefNode *funDefNode);

/**
 * @brief Generuje cílový kód pro blok příkazů
 *
 * @details Prochází všechny příkazy v bloku a volá další funkce pro generování
 *          podle typu příkazu:
 *          - @c TAC_generateVarDef
 *          - @c TAC_generateExpression
 *          - @c TAC_generateFunctionCall
 *          - @c TAC_generateIf
 *          - @c TAC_generateWhile
 *          - @c TAC_generateReturn
 *          Pokud narazí na return, tak přestává generovat mrtvý kód
 *
 * @param [in] statement Ukazatel na uzel bloku příkazů
 * @param [in] mode Režim generování kódu (nutné pro definice proměnných v cyklech)
 *                  - @c TAC_ALL - generuje všechny příkazy
 *                  - @c TAC_VAR_DEF_ONLY - generuje pouze definice proměnných
 *                  - @c TAC_EXCEPT_VAR_DEF - generuje všechny příkazy kromě definic proměnných
 */
void TAC_generateStatementBlock(AST_StatementNode* statement, TAC_mode mode);

/**
 * @brief Generuje cílový kód pro binární operace
 *
 * @details Pomocí TAC_generateExpression zpracuje oba operandy
 *          (jejich výsledky jsou na vrcholu datového zásobníku)
 *          dále podle operátoru provede operaci. Výsledek je opět
 *          na vrcholu datového zásobníku
 *
 * @param [in] binNode Ukazatel na uzel binární operace
 */
void TAC_generateBinaryOperator(AST_BinOpNode *binNode);

/**
 * @brief Generuje cílový kód pro definici proměnné
 *
 * @details Vyhodnotí výraz vpravo a její hodnotu uloží do proměnné
 *
 * @param [in] exprNode Ukazatel na výraz obsahující binarní operaci přiřazení
 * @param [in] mode Režim generování kódu (nutné pro definice proměnných v cyklech)
 *                  - @c TAC_ALL - definuje proměnnou a vloží hodnotu
 *                  - @c TAC_VAR_DEF_ONLY - pouze definuje proměnnou
 *                  - @c TAC_EXCEPT_VAR_DEF - pouze vloží hodnotu do proměnné
 */
void TAC_generateVarDef(AST_ExprNode *exprNode, TAC_mode mode);

/**
 * @brief Generuje cílový kód pro výraz
 *
 * @details Vyhodnotí výraz a výsledek uloží na vrchol datového zásobníku
 *          Může volat další funkce:
 *          - @c TAC_generateLiteral
 *          - @c TAC_generateBinaryOperator
 *          - @c TAC_generateFunctionCall
 *
 * @param [in] expr Ukazatel na uzel výrazu
 */
void TAC_generateExpression(AST_ExprNode *expr);

/**
 * @brief Generuje cílový kód pro literál
 *
 * @details Na vrchol datového zásobníku dá hodnotu literálu podle typu
 *
 * @param [in] literal Ukazatel na uzel literálu
 */
void TAC_generateLiteral(AST_VarNode *literal);

/**
 * @brief Generuje cílový kód pro podmíněný příkaz if
 *
 * @details Vyhodnotí typ podmínky, vygeneruje if a else větve pomocí
 *          @c TAC_generateStatementBlock a přidá skok na else větev v případě
 *          že podmínka není splněna. V případě null podmínky přidá definici
 *          @c id_bez_null. Každý if má svůj unikátní číselný identifikátor a
 *          návěští.
 *
 * @param [in] ifNode Ukazatel na uzel podmíněného příkazu if
 * @param [in] mode Režim generování kódu (nutné pro definice proměnných v cyklech)
 *                  - @c TAC_ALL - generuje všechny příkazy
 *                  - @c TAC_VAR_DEF_ONLY - generuje pouze definice proměnných
 *                  - @c TAC_EXCEPT_VAR_DEF - generuje všechny příkazy kromě definic proměnných
 */
void TAC_generateIf(AST_IfNode *if_node, TAC_mode mode);

/**
 * @brief Generuje cílový kód pro smyčku while
 *
 * @details Vyhodnotí podmínku a vygeneruje tělo cyklu pomocí
 *          @c TAC_generateStatementBlock. V případě null podmínky přidá definici
 *          @c id_bez_null. Po vykonání těla cyklu přidá skok na začátek cyklu,
 *          kde se opět vyhodnotí podmínka. Pokud podmínka není splněna, skočí
 *          na konec cyklu. Každý cyklus má svůj unikátní číselný identifikátor
 *          a návěští.
 *
 * @param [in] whileNode Ukazatel na uzel smyčky while
 * @param [in] mode Režim generování kódu (nutné pro definice proměnných v cyklech)
 *                  - @c TAC_ALL - Nejdříve definuje proměnné v podmínce a těle cyklu
 *                                 a následně generuje příkazy bez definic
 *                  - @c TAC_VAR_DEF_ONLY - Definuje proměnné v podmínce a těle cyklu
 *                  - @c TAC_EXCEPT_VAR_DEF - Generuje příkazy bez definic
 *
 */
void TAC_generateWhile(AST_WhileNode *whileNode, TAC_mode mode);

/**
 * @brief Generuje cílový kód pro návrat z funkce
 *
 * @details Vyhodnotí výraz a výsledek uloží na vrchol datového zásobníku.
 *          Odstraní rámec funkce a volá return.
 *
 * @param [in] expr Ukazatel na uzel výrazu
 */
void TAC_generateReturn(AST_ExprNode *expr);

/**
 * @brief Generuje cílový kód pro volání funkce
 *
 * @details Vytvoří dočasný rámec pro parametry funkce, najde definici funkce
 *          a pro všechny parametry vyhodnotí hodnotu a uloží ji do parametru.
 *          Parametry pro vestavěné funkce nemají rámcový suffix.
 *
 * @param [in] funCallNode Ukazatel na uzel volání funkce
 */
void TAC_generateFunctionCall(AST_FunCallNode *funCallNode);

/**
 * @brief Převede speciální znaky na escape sekvence
 *
 * @details Převede speciální znaky na escape sekvence pro výpis do cílového kódu
 *
 * @param [in] origin Ukazatel na řetězec, který chceme převést
 */
DString *TAC_convertSpecialSymbols(DString *origin);

/**
 * @brief Resetuje statické proměnné v generátoru
 *
 * @details Resetuje statické proměnné count v if a while funkcích v případě
 *          více testů v jednom spuštění.
 *
 */
void TAC_resetStatic();

/**
 * @brief Přidá instrukci do bufferu
 *
 * @details Buffer ukládá až jednu instrukci. Při přijetí druhé je analyzuje
 *          a v případě, že uložená instrukce je PUSHS a přijatá POPS,
 *          tak je nahradí jednou instrukcí @c MOVE. V případě, že by argument
 *          obou instrukcí byl stejný, tak je vymaže (jsou zbytečné).
 *          Pokud nelze optimalizaci provést, tak vypíše uloženou instrukci
 *          a nahradí ji přijatou.
 *
 * @param [in] newinstruction Nová instrukce pro zpracování
 * @param [in] NULL Pro vypsání uložené hodnoty bufferu
 */
void TAC_bufferPrint(char (*newInstruction)[OPTIMIZE_BUFFER_SIZE]);

#endif // TAC_H_

/*** Konec souboru tac_generator.h ***/
