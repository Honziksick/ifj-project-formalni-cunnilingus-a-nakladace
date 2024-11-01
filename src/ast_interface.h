/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_interface.h                                           *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   01.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ast_interface.h
 * @author Jan Kalina \<xkalinj00> (návrh, hlavičkový soubor)
 * @author David Krejčí \<xkrejcd00> (návrh)
 *
 * @brief Hlavičkový soubor pro tvorbu a destrukci uzlů AST.
 * @details Tento hlavičkový soubor definuje rozhraní pro práci s uzly abstraktního
 *          syntaktického stromu (AST). Obsahuje deklarace funkcí pro vytváření
 *          a rušení uzlů AST různých typů. Pro zjednodušení práce s uzly, je
 *          navrženo také veřejné rozhraní s obecnou funkcí pro tvorbu a obecnou
 *          funkcí pro destrukci uzlů.
 */

#ifndef AST_INTERFACE_H_
/** @cond  */
#define AST_INTERFACE_H_
/** @endcond  */

#include <stdlib.h>
#include "ast_nodes.h"

/*******************************************************************************
 *                                                                             *
 *                       FUNKCE URČENÉ PRO VEŘEJNÉ UŽITÍ                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří nový uzel AST daného typu.
 * @details Dynamicky alokuje paměť pro nový uzel AST a inicializuje jeho typ.
 *
 * @param [in] type Typ uzlu, který má být vytvořen.
 * @return @c void* Ukazatel na vytvořený uzel, nebo NULL v případě chyby alokace.
 *
 * @note Volající je zodpovědný za uvolnění paměti pomocí `AST_destroyNode`.
 *       Je vracen ukazatel na void, proto je nutné získaný ukazatel
 *       přetypovat na příslušný typ.
 */
void *AST_createNode(AST_NodeType type);

/**
 * @brief Uvolní paměť uzlu AST daného typu.
 * @details Zajistí správné uvolnění všech členů uzlu a samotné struktury uzlu.
 *
 * @param [in] type Typ uzlu, který má být uvolněn.
 * @param [in] node Ukazatel na uzel, který má být zničen.
 */
void AST_destroyNode(AST_NodeType type, void *node);


/*******************************************************************************
 *                                                                             *
 *                               INTERNÍ FUNKCE                                *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří kořenový uzel programu.
 *
 * @return Ukazatel na nový uzel AST typu `AST_ProgramNode`, nebo NULL při chybě alokace.
 */
AST_ProgramNode *AST_createProgramNode();

/**
 * @brief Uvolní paměť pro kořenový uzel programu.
 *
 * @param [in,out] node Ukazatel na `AST_ProgramNode` uzel, který má být uvolněn.
 */
void AST_destroyProgramNode(AST_ProgramNode *node);

/**
 * @brief Vytvoří uzel pro definici funkce.
 *
 * @return Ukazatel na nový uzel `AST_FunDefNode`, nebo NULL při chybě alokace.
 */
AST_FunDefNode *AST_createFunDefNode();

/**
 * @brief Uvolní paměť pro uzel definice funkce.
 * @param [in,out] node Ukazatel na `AST_FunDefNode` uzel, který má být uvolněn.
 */
void AST_destroyFunDefNode(AST_FunDefNode *node);

/**
 * @brief Vytvoří uzel pro příkaz.
 *
 * @return Ukazatel na nový uzel `AST_StatementNode`, nebo NULL při chybě alokace.
 */
AST_StatementNode *AST_createStatementNode();

/**
 * @brief Uvolní paměť pro uzel příkazu.
 *
 * @param [in,out] node Ukazatel na `AST_StatementNode` uzel, který má být uvolněn.
 */
void AST_destroyStatementNode(AST_StatementNode *node);

/**
 * @brief Vytvoří uzel pro volání funkce.
 *
 * @return Ukazatel na nový uzel `AST_FunCallNode`, nebo NULL při chybě alokace.
 */
AST_FunCallNode *AST_createFunCallNode();

/**
 * @brief Uvolní paměť pro uzel volání funkce.
 *
 * @param [in,out] node Ukazatel na `AST_FunCallNode` uzel, který má být uvolněn.
 */
void AST_destroyFunCallNode(AST_FunCallNode *node);

/**
 * @brief Vytvoří uzel pro podmíněný příkaz if.
 *
 * @return Ukazatel na nový uzel `AST_IfNode`, nebo NULL při chybě alokace.
 */
AST_IfNode *AST_createIfNode();

/**
 * @brief Uvolní paměť pro uzel podmíněného příkazu if.
 *
 * @param [in,out] node Ukazatel na `AST_IfNode` uzel, který má být uvolněn.
 */
void AST_destroyIfNode(AST_IfNode *node);

/**
 * @brief Vytvoří uzel pro cyklus while.
 *
 * @return Ukazatel na nový uzel `AST_WhileNode`, nebo NULL při chybě alokace.
 */
AST_WhileNode *AST_createWhileNode();

/**
 * @brief Uvolní paměť pro uzel cyklu while.
 *
 * @param [in,out] node Ukazatel na `AST_WhileNode` uzel, který má být uvolněn.
 */
void AST_destroyWhileNode(AST_WhileNode *node);

/**
 * @brief Vytvoří uzel pro výraz.
 *
 * @return Ukazatel na nový uzel `AST_ExprNode`, nebo NULL při chybě alokace.
 */
AST_ExprNode *AST_createExprNode();

/**
 * @brief Uvolní paměť pro uzel výrazu.
 *
 * @param [in,out] node Ukazatel na `AST_ExprNode` uzel, který má být uvolněn.
 */
void AST_destroyExprNode(AST_ExprNode *node);

/**
 * @brief Vytvoří uzel pro binární operátor.
 *
 * @return Ukazatel na nový uzel `AST_BinOpNode`, nebo NULL při chybě alokace.
 */
AST_BinOpNode *AST_createBinOpNode();

/**
 * @brief Uvolní paměť pro uzel binárního operátoru.
 *
 * @param [in,out] node Ukazatel na `AST_BinOpNode` uzel, který má být uvolněn.
 */
void AST_destroyBinOpNode(AST_BinOpNode *node);

/**
 * @brief Vytvoří uzel pro literál.
 *
 * @return Ukazatel na nový uzel `AST_LiteralNode`, nebo NULL při chybě alokace.
 */
AST_LiteralNode *AST_createLiteralNode();

/**
 * @brief Uvolní paměť pro uzel literálu.
 *
 * @param [in,out] node Ukazatel na `AST_LiteralNode` uzel, který má být uvolněn.
 */
void AST_destroyLiteralNode(AST_LiteralNode *node);

/**
 * @brief Vytvoří uzel pro proměnnou.
 *
 * @return Ukazatel na nový uzel `AST_VarNode`, nebo NULL při chybě alokace.
 */
AST_VarNode *AST_createVarNode();

/**
 * @brief Uvolní paměť pro uzel proměnné.
 *
 * @param [in,out] node Ukazatel na `AST_VarNode` uzel, který má být uvolněn.
 */
void AST_destroyVarNode(AST_VarNode *node);

#endif // AST_INTERFACE_H_

/*** Konec souboru ast_interface.h ***/
