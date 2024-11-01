/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.h                                       *
 * Autor:            XXX XXXXXX   <xplagia00>                                  *
 *                                                                             *
 * Datum:            XX.XX.2024                                                *
 * Poslední změna:   XX.XX.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file semantic_analyser.h
 * @author XXX XXXXX \<xplagia00>
 *
 * @brief Hlavičkový soubor pro sémantický analyzátor.
 * @details XXX
 */

#ifndef SEMANTIC_ANALYSER_H_
/** @cond  */
#define SEMANTIC_ANALYSER_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "symtable.h"
#include "error.h"

/**
 * @brief Zpracuje kořenový uzel programu a zahájí sémantickou analýzu.
 * @details Tato funkce prochází všechny definice funkcí v kořenovém uzlu
 *          programu a provádí nad nimi sémantickou analýzu.
 *
 * @param programNode Ukazatel na kořenový uzel programu.
 *
 * @return `true`, pokud sémantická analýza programu proběhla úspěšně, jinak `false`.
 */
bool semantic_analyseProgramNode(AST_NodeProgram *programNode);

/**
 * @brief Provede sémantickou analýzu funkce.
 * @details Zpracuje parametry, tělo a návratový typ funkce, kontroluje
 *          jejich platnost a konzistenci s deklarovanými typy.
 *
 * @param funDefNode Ukazatel na uzel definice funkce.
 *
 * @return `true`, pokud analýza proběhla úspěšně, jinak `false`.
 */
bool semantic_analyseFunctionDefinition(AST_NodeFunDef *funDefNode);

/**
 * @brief Kontroluje parametry funkce v uzlu funkce.
 * @details Prochází seznam parametrů funkce a zajišťuje jejich platnost
 *          a unikátnost jmen v rámci funkce.
 *
 * @param parameterNode Ukazatel na uzel parametru.
 *
 * @return `true`, pokud jsou parametry funkce správně, jinak `false`.
 */
bool semantic_analyseFunctionParameters(AST_NodeParameter *parameterNode);

/**
 * @brief Zpracuje příkaz v rámci těla funkce.
 * @details Kontroluje platnost a typovou kompatibilitu všech příkazů ve funkci.
 *
 * @param statementNode Ukazatel na uzel příkazu.
 *
 * @return `true`, pokud je příkaz platný, jinak `false`.
 */
bool semantic_analyseStatement(AST_NodeStatement *statementNode);

/**
 * @brief Ověřuje definici proměnné.
 * @details Kontroluje, zda je proměnná správně deklarována s daným typem
 *          a případně inicializována.
 *
 * @param varDefNode Ukazatel na uzel definice proměnné.
 *
 * @return `true`, pokud je definice platná, jinak `false`.
 */
bool semantic_analyseVariableDefinition(AST_DataVarDef *varDefNode);

/**
 * @brief Ověřuje platnost příkazu přiřazení.
 * @details Kontroluje kompatibilitu datových typů mezi proměnnou a
 *          přiřazovaným výrazem.
 *
 * @param assignmentNode Ukazatel na uzel přiřazení.
 *
 * @return `true`, pokud je přiřazení platné, jinak `false`.
 */
bool semantic_analyseAssignment(AST_DataAssignment *assignmentNode);

/**
 * @brief Provede kontrolu nad voláním funkce.
 * @details Ověřuje, zda jsou argumenty funkce konzistentní s deklarací funkce
 *          v symbolové tabulce.
 *
 * @param funCallNode Ukazatel na uzel volání funkce.
 *
 * @return `true`, pokud je volání funkce správné, jinak `false`.
 */
bool semantic_analyseFunctionCall(AST_DataFunCall *funCallNode);

/**
 * @brief Zpracuje podmíněný příkaz "if".
 * @details Analyzuje podmínku a větve příkazu "if" z hlediska typové
 *          kompatibility.
 *
 * @param ifNode Ukazatel na uzel podmíněného příkazu "if".
 *
 * @return `true`, pokud je podmínka platná, jinak `false`.
 */
bool semantic_analyseIfStatement(AST_DataIf *ifNode);

/**
 * @brief Provede kontrolu nad smyčkou "while".
 * @details Kontroluje, zda je podmínka platná a kompatibilní s příkazy
 *          uvnitř cyklu.
 *
 * @param whileNode Ukazatel na uzel smyčky "while".
 *
 * @return `true`, pokud je smyčka platná, jinak `false`.
 */
bool semantic_analyseWhileLoop(AST_DataWhile *whileNode);

/**
 * @brief Provede kontrolu návratového příkazu.
 * @details Zajišťuje, že návratový typ je kompatibilní s návratovým
 *          typem funkce.
 *
 * @param returnNode Ukazatel na uzel návratového příkazu.
 *
 * @return `true`, pokud je návrat správný, jinak `false`.
 */
bool semantic_analyseReturnStatement(AST_DataReturn *returnNode);

/**
 * @brief Ověří typovou kompatibilitu ve výrazu.
 * @details Prochází binární operace a kontroluje, zda typy operandů odpovídají
 *          operátorům.
 *
 * @param binaryOpNode Ukazatel na uzel binární operace.
 *
 * @return `true`, pokud jsou typy kompatibilní, jinak `false`.
 */
bool semantic_analyseBinaryOperation(AST_DataBinaryOperator *binaryOpNode);

#endif // SEMANTIC_ANALYSER_H_

/*** Konec souboru semantic_anal.h ***/
