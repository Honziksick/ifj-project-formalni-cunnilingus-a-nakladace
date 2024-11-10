/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_interface.h                                           *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   06.11.2024                                                *
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
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define AST_FRAME_ID_NOT_ASSIGNED 0   /**< Uzlu AST_StatementNode ještě nebylo přiřazeno frameID. */


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
 * @return @c void* Ukazatel na vytvořený uzel, nebo `NULL` v případě chyby alokace.
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

/**
 * @brief Alokuje paměť pro globální kořen abstraktního syntaktického stromu
 * @details Alokuje paměť pro kořen AST a inicializuje tímto ukazatelem globální
 *          ukazatel na kořen abstraktního syntaktického stromu.
 *
 * @note Funkce využívá jako vstupní parametr globální ukazatel na kořen stromu.
 *
 * @return Ukazatel na nový uzel AST typu `AST_ProgramNode`, nebo `NULL` při chybě alokace.
 */
void AST_createTree();

/**
 * @brief Uvolní z paměti celý abstraktní syntaktický strom
 * @details Pokud je kořen platný, zahájí postupnou destrukci stromu. Zajistí
 *          správné uvolnění všech členů uzlů, všech uzlů stromu a nakonec
 *          globálního kořenového uzlu.
 *
 * @note Funkce využívá jako vstupní parametr globální ukazatel na kořen stromu.
 */
void AST_destroyTree();


/*******************************************************************************
 *                                                                             *
 *                  INTERNÍ FUNKCE NA TVORBU A DESTRUKCI UZLŮ                  *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří kořenový uzel programu.
 *
 * @return Ukazatel na nový uzel AST typu `AST_ProgramNode`, nebo `NULL` při chybě alokace.
 */
AST_ProgramNode *AST_createProgramNode();

/**
 * @brief Uvolní paměť pro kořenový uzel programu.
 *
 * @param [in] node Ukazatel na `AST_ProgramNode` uzel, který má být uvolněn.
 */
void AST_destroyProgramNode(AST_ProgramNode *node);

/**
 * @brief Vytvoří uzel pro definici funkce.
 *
 * @return Ukazatel na nový uzel `AST_FunDefNode`, nebo `NULL` při chybě alokace.
 */
AST_FunDefNode *AST_createFunDefNode();

/**
 * @brief Uvolní paměť pro uzel definice funkce.
 *
 * @param [in] node Ukazatel na `AST_FunDefNode` uzel, který má být uvolněn.
 */
void AST_destroyFunDefNode(AST_FunDefNode *node);

/**
 * @brief Vytvoří uzel pro parametry nebo argumenty funkce.
 *
 * @return Ukazatel na nový uzel `AST_ArgOrParamNode`, nebo `NULL` při chybě alokace.
 */
AST_ArgOrParamNode *AST_createArgOrParamNode();

/**
 * @brief Uvolní paměť pro uzel definice funkce.
 *
 * @param [in] node Ukazatel na `AST_ArgOrParamNode` uzel, který má být uvolněn.
 */
void AST_destroyArgOrParamNode(AST_ArgOrParamNode *node);

/**
 * @brief Vytvoří uzel pro příkaz.
 *
 * @return Ukazatel na nový uzel `AST_StatementNode`, nebo `NULL` při chybě alokace.
 */
AST_StatementNode *AST_createStatementNode();

/**
 * @brief Uvolní paměť pro uzel příkazu.
 *
 * @param [in] node Ukazatel na `AST_StatementNode` uzel, který má být uvolněn.
 */
void AST_destroyStatementNode(AST_StatementNode *node);

/**
 * @brief Vytvoří uzel pro volání funkce.
 *
 * @return Ukazatel na nový uzel `AST_FunCallNode`, nebo `NULL` při chybě alokace.
 */
AST_FunCallNode *AST_createFunCallNode();

/**
 * @brief Uvolní paměť pro uzel volání funkce.
 *
 * @param [in] node Ukazatel na `AST_FunCallNode` uzel, který má být uvolněn.
 */
void AST_destroyFunCallNode(AST_FunCallNode *node);

/**
 * @brief Vytvoří uzel pro podmíněný příkaz if.
 *
 * @return Ukazatel na nový uzel `AST_IfNode`, nebo `NULL` při chybě alokace.
 */
AST_IfNode *AST_createIfNode();

/**
 * @brief Uvolní paměť pro uzel podmíněného příkazu if.
 *
 * @param [in] node Ukazatel na `AST_IfNode` uzel, který má být uvolněn.
 */
void AST_destroyIfNode(AST_IfNode *node);

/**
 * @brief Vytvoří uzel pro cyklus while.
 *
 * @return Ukazatel na nový uzel `AST_WhileNode`, nebo `NULL` při chybě alokace.
 */
AST_WhileNode *AST_createWhileNode();

/**
 * @brief Uvolní paměť pro uzel cyklu while.
 *
 * @param [in] node Ukazatel na `AST_WhileNode` uzel, který má být uvolněn.
 */
void AST_destroyWhileNode(AST_WhileNode *node);

/**
 * @brief Vytvoří uzel pro výraz.
 *
 * @return Ukazatel na nový uzel `AST_ExprNode`, nebo `NULL` při chybě alokace.
 */
AST_ExprNode *AST_createExprNode();

/**
 * @brief Uvolní paměť pro uzel výrazu.
 *
 * @param [in] node Ukazatel na `AST_ExprNode` uzel, který má být uvolněn.
 */
void AST_destroyExprNode(AST_ExprNode *node);

/**
 * @brief Vytvoří uzel pro binární operátor.
 *
 * @return Ukazatel na nový uzel `AST_BinOpNode`, nebo `NULL` při chybě alokace.
 */
AST_BinOpNode *AST_createBinOpNode();

/**
 * @brief Uvolní paměť pro uzel binárního operátoru.
 *
 * @param [in] node Ukazatel na `AST_BinOpNode` uzel, který má být uvolněn.
 */
void AST_destroyBinOpNode(AST_BinOpNode *node);

/**
 * @brief Vytvoří uzel pro proměnnou.
 *
 * @param [in] type Nastaví typ uzlu buď na `AST_VAR_NODE` nebo `AST_LITERAL_NODE`.
 *
 * @return Ukazatel na nový uzel `AST_VarNode`, nebo `NULL` při chybě alokace.
 */
AST_VarNode *AST_createVarNode(AST_NodeType type);

/**
 * @brief Uvolní paměť pro uzel proměnné.
 *
 * @param [in] node Ukazatel na `AST_VarNode` uzel, který má být uvolněn.
 */
void AST_destroyVarNode(AST_VarNode *node);


/*******************************************************************************
 *                                                                             *
 *                     INTERNÍ FUNKCE NA DESTRUKCI SEZNAMŮ                     *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Uvolní paměť pro všechny uzly v seznamu parametrů/argumentů v uzlu pro funkci.
 *
 * @param [in] list Ukazatel na seznam `AST_ArgOrParamNode` uzlů, které mají být uvolněny.
 */
void AST_destroyArgOrParamList(AST_ArgOrParamNode *list);

/**
 * @brief Uvolní paměť pro všechny uzly v seznamu pro příkazy.
 *
 * @param [in] list Ukazatel na seznam `AST_StatementNode` uzlů, které mají být uvolněny.
 */
void AST_destroyStatementList(AST_StatementNode *list);

/**
 * @brief Uvolní paměť pro všechny uzly v seznamu definovaných funkcí.
 *
 * @param [in] list Ukazatel na seznam `AST_FunDefNode` uzlů, které mají být uvolněny.
 */
void AST_destroyFunDefList(AST_FunDefNode *list);

#endif // AST_INTERFACE_H_

/*** Konec souboru ast_interface.h ***/
