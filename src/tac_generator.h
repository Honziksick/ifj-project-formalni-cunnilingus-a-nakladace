/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.h                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   26.11.2024                                                *
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
 * @brief Hlavičkový soubor pro generátor vnitřního kódu (3AK).
 * @details Knihovna pro generování cílového kódu z AST.
 */

#ifndef TAC_H_
/** @cond  */
#define TAC_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "ast_nodes.h"
#include "ast_interface.h"
#include "frame_stack.h"
#include "error.h"

#define MAX_BUFFER_SIZE 5

/**
 * @brief Generuje cílový kód programu ze stromu AST.
 *
 * @details Funkce prochází strom AST a rekurzivně volá funkci
 *          TAC_generateFunctionDefinition pro každou definici funkce.
 *          Pracuje s globálním kořenem stromu ASTroot.
 */
void TAC_generateProgram();

/**
 * @brief Generuje cílový kód definice funkce
 *
 * @details Přidá label a pushne rámec s parametry, následně zpracuje všechny
 *          instukce pomocí TAC_generateStatementBlock, popne rámec a volá return
 *
 * @param [in] funDefNode Ukazatel na uzel definice funkce
 */
void TAC_generateFunctionDefinition(AST_FunDefNode *funDefNode);

/**
 * @brief Generuje cílový kód pro blok příkazů
 *
 * @details Prochází všechny příkazy v bloku a volá další funkce pro generování
 *          podle typu příkazu:
 *          - TAC_generateVarDef
 *          - TAC_generateExpression
 *          - TAC_generateFunctionCall
 *          - TAC_generateIf
 *          - TAC_generateWhile
 *          - TAC_generateReturn
 *          Pokud narazí na return, tak přestává generovat mrtvý kód
 *
 * @param [in] statement Ukazatel na uzel bloku příkazů
 */
void TAC_generateStatementBlock(AST_StatementNode* statement);

/**
 * @brief Generuje cílový kód pro binární operace
 *
 * @details Pomocí TAC_generateExpression zpracuje oba operandy
 *          (jejich výsledky jsou na vrcholu datového zásobníku)
 *          dále podle operátoru provede operaci. Výsledek je opět
 *          na vrcholu datového zásobníku
 *
 * @param [in] bin_node Ukazatel na uzel binární operace
 */
void TAC_generateBinaryOperator(AST_BinOpNode *bin_node);

/**
 * @brief Generuje cílový kód pro definici proměnné
 *
 * @details Vyhodnotí výraz vpravo a její hodnotu uloží do proměnné
 *
 * @param [in] expr_node Ukazatel na výraz obsahující binarní operaci přiřazení
 */
void TAC_generateVarDef(AST_ExprNode *expr_node);

/**
 * @brief Generuje cílový kód pro výraz
 *
 * @details Vyhodnotí výraz a výsledek uloží na vrchol datového zásobníku
 *          Může volat další funkce:
 *          - TAC_generateLiteral
 *          - TAC_generateBinaryOperator
 *          - TAC_generateFunctionCall
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
 *          TAC_generateStatementBlock a přidá skok na else větev v případě
 *          že podmínka není splněna. V případě null podmínky přidá definici
 *          id_bez_null.
 *          Každý if má svůj unikátní číselný identifikátor a návěští.
 *
 * @param [in] if_node Ukazatel na uzel podmíněného příkazu if
 */
void TAC_generateIf(AST_IfNode *if_node);

/**
 * @brief Generuje cílový kód pro smyčku while
 *
 * @details Vyhodnotí podmínku a vygeneruje tělo cyklu pomocí
 *          TAC_generateStatementBlock. V případě null podmínky přidá definici
 *          id_bez_null. Po vykonání těla cyklu přidá skok na začátek cyklu,
 *          kde se opět vyhodnotí podmínka. Pokud podmínka není splněna, skočí
 *          na konec cyklu.
 *          Každý cyklus má svůj unikátní číselný identifikátor a návěští.
 *
 * @param [in] while_node Ukazatel na uzel smyčky while
 */
void TAC_generateWhile(AST_WhileNode *while_node);

/**
 * @brief Generuje cílový kód pro návrat z funkce
 *
 * @details Vyhodnotí výraz a výsledek uloží na vrchol datového zásobníku.
 *          Popne rámec funkce a volá return.
 *
 * @param [in] expr Ukazatel na uzel výrazu
 */
void TAC_generateReturn(AST_ExprNode *expr);

/**
 * @brief Generuje cílový kód pro volání funkce
 *
 * @details Vytvoří temporary frame pro parametry funkce, najde definici funkce
 *          a pro všechny parametry vyhodnotí hodnotu a uloží ji do parametru.
 *          Parametry pro vestavěné funkce nemají rámcový suffix.
 *
 * @param [in] funCallNode Ukazatel na uzel volání funkce
 */
void TAC_generateFunctionCall(AST_FunCallNode *funCallNode);

DString *TAC_convertSpecialSymbols(DString *origin);

void TAC_resetStatic();

#endif // TAC_H_

/*** Konec souboru tac_generator.h ***/
