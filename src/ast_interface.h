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
#include <limits.h>         // Konstanty pro meze integeru
#include <math.h>           // Konstanty pro meze floatu
#include "ast_nodes.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define INT_CONVERTION_BASE 10      /**< Základ pro dekaické číslo při převodu pomocí "strtol" */


/*******************************************************************************
 *                                                                             *
 *                       FUNKCE URČENÉ PRO VEŘEJNÉ UŽITÍ                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří nový uzel AST daného typu.
 *
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
 *
 * @details Zajistí správné uvolnění všech členů uzlu a samotné struktury uzlu.
 *
 * @param [in] type Typ uzlu, který má být uvolněn.
 * @param [in] node Ukazatel na uzel, který má být zničen.
 */
void AST_destroyNode(AST_NodeType type, void *node);

/**
 * @brief Alokuje paměť pro globální kořen abstraktního syntaktického stromu
 *
 * @details Alokuje paměť pro kořen AST a inicializuje tímto ukazatelem globální
 *          ukazatel na kořen abstraktního syntaktického stromu.
 *
 * @note Funkce využívá jako vstupní parametr globální ukazatel na kořen stromu.
 *
 * @return Ukazatel na nový uzel AST typu `AST_ProgramNode`, nebo `NULL` při chybě alokace.
 */
AST_ProgramNode *AST_createTree();

/**
 * @brief Uvolní z paměti celý abstraktní syntaktický strom
 *
 * @details Pokud je kořen platný, zahájí postupnou destrukci stromu. Zajistí
 *          správné uvolnění všech členů uzlů, všech uzlů stromu a nakonec
 *          globálního kořenového uzlu.
 *
 * @note Funkce využívá jako vstupní parametr globální ukazatel na kořen stromu.
 */
void AST_destroyTree();


/*******************************************************************************
 *                                                                             *
 *        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ NA KONKRÉTNÍ INICIALIZACI UZLŮ         *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje kořenový uzel programu.
 *
 * @details Tato funkce inicializuje kořenový uzel programu s poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param importedFile Ukazatel na dynamický string s cestou k importovanému modulu.
 * @param functionList Ukazatel na seznam definic funkcí.
 */
void AST_initNewProgramNode(AST_ProgramNode *node, DString *importedFile, \
                            AST_FunDefNode *functionList);

/**
 * @brief Inicializuje uzel pro definici funkce.
 *
 * @details Tato funkce inicializuje uzel pro definici funkce s poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param identifier Ukazatel na dynamický string s identifikátorem funkce.
 * @param parameters Ukazatel na seznam parametrů funkce.
 * @param returnType Návratový typ funkce.
 * @param body Ukazatel na tělo funkce.
 */
void AST_initNewFunDefNode(AST_FunDefNode *node, DString *identifier,  \
                           AST_ArgOrParamNode *parameters, AST_DataType returnType,  \
                           AST_StatementNode *body);

/**
 * @brief Inicializuje uzel pro argument nebo parametr funkce.
 *
 * @details Tato funkce inicializuje uzel pro argument nebo parametr funkce s
 *          poskytnutými parametry. Pokud je uzel již inicializován nebo pokud
 *          je předán neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param dataType Datový typ proměnné parametru/argumentu.
 * @param variable Ukazatel na uzel proměnné.
 */
void AST_initNewArgOrParamNode(AST_ArgOrParamNode *node, AST_DataType dataType, \
                               AST_VarNode *variable);

/**
 * @brief Inicializuje uzel pro příkaz.
 *
 * @details Tato funkce inicializuje uzel pro příkaz s poskytnutými parametry.
 *          Pokud je uzel již inicializován nebo pokud je předán neplatný
 *          ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param frameID ID rámce, kde je příkaz definován.
 * @param statementType Typ příkazu.
 * @param statement Ukazatel na uzel specifický pro daný příkaz.
 */
void AST_initNewStatementNode(AST_StatementNode *node, size_t frameID, \
                              AST_StatementType statementType, void *statement);

/**
 * @brief Inicializuje uzel pro volání funkce.
 *
 * @details Tato funkce inicializuje uzel pro volání funkce s poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param identifier Ukazatel na dynamický string s názvem funkce.
 * @param isBuiltIn Příznak, zda je funkce vestavěná (`ifj.id(args)`).
 * @param arguments Ukazatel na uzly argumentů.
 */
void AST_initNewFunCallNode(AST_FunCallNode *node, DString *identifier, \
                            bool isBuiltIn, AST_ArgOrParamNode *arguments);

/**
 * @brief Inicializuje uzel pro podmíněný příkaz if.
 *
 * @details Tato funkce inicializuje uzel pro podmíněný příkaz if s poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param condition Ukazatel na podmínku.
 * @param nullCondition Ukazatel na volitelnou podmínku NULL.
 * @param thenBranch Ukazatel na větev 'then'.
 * @param elseBranch Ukazatel na větev 'else'.
 */
void AST_initNewIfNode(AST_IfNode *node, AST_ExprNode *condition, AST_VarNode *nullCondition, \
                    AST_StatementNode *thenBranch, AST_StatementNode *elseBranch);

/**
 * @brief Inicializuje uzel pro cyklus while.
 *
 * @details Tato funkce inicializuje uzel pro cyklus while s poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param condition Ukazatel na podmínku.
 * @param nullCondition Ukazatel na volitelnou podmínku NULL.
 * @param body Ukazatel na tělo cyklu.
 */
void AST_initNewWhileNode(AST_WhileNode *node, AST_ExprNode *condition, \
                          AST_VarNode *nullCondition, AST_StatementNode *body);

/**
 * @brief Inicializuje uzel pro výraz.
 *
 * @details Tato funkce inicializuje uzel pro výraz s poskytnutými parametry.
 *          Pokud je uzel již inicializován nebo pokud je předán neplatný
 *          ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param exprType Typ výrazu.
 * @param expression Ukazatel na konkrétní výraz.
 */
void AST_initNewExprNode(AST_ExprNode *node, AST_ExprType exprType, void *expression);

/**
 * @brief Inicializuje uzel pro binární operátor.
 *
 * @details Tato funkce inicializuje uzel pro binární operátor s poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param op Typ binární operace.
 * @param left Ukazatel na levý operand.
 * @param right Ukazatel na pravý operand.
 */
void AST_initNewBinOpNode(AST_BinOpNode *node, AST_BinOpType op, \
                          AST_ExprNode *left, AST_ExprNode *right);

/**
 * @brief Inicializuje uzel pro proměnnou nebo literál.
 *
 * @details Tato funkce inicializuje uzel pro proměnnou nebo literál s
 *          poskytnutými parametry. Pokud je uzel již inicializován nebo pokud
 *          je předán neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param node Ukazatel na uzel, který má být inicializován.
 * @param type Typ uzlu (AST_VAR_NODE nebo AST_LITERAL_NODE).
 * @param identifier Ukazatel na dynamický string s identifikátorem proměnné.
 * @param frameID ID rámce, kde je proměnná definovaná.
 * @param literalType Typ literálu.
 * @param value Ukazatel na hodnotu literálu.
 */
void AST_initNewVarNode(AST_VarNode *node, AST_NodeType type, DString *identifier, \
                        size_t frameID, AST_LiteralType literalType, DString *value);


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
 * @brief Vloží uzel na začátek seznamu definic funkcí.
 *
 * @details Tato funkce přidá nový uzel na začátek seznamu definic funkcí. Pokud
 *          je seznam prázdný, nový uzel se stane prvním uzlem v seznamu.
 *
 * @param firstDef Ukazatel na ukazatel na první uzel seznamu definic funkcí.
 * @param newDef Ukazatel na nový uzel, který má být přidán na začátek seznamu.
 */
void AST_insertFirstFunDefNode(AST_FunDefNode **firstDef, AST_FunDefNode *newDef);

/**
 * @brief Vloží uzel na začátek seznamu argumentů nebo parametrů funkce.
 *
 * @details Tato funkce přidá nový uzel na začátek seznamu argumentů nebo
 *          parametrů funkce. Pokud je seznam prázdný, nový uzel se stane
 *          prvním uzlem v seznamu.
 *
 * @param firstArg Ukazatel na ukazatel na první uzel seznamu argumentů nebo parametrů.
 * @param newArg Ukazatel na nový uzel, který má být přidán na začátek seznamu.
 */
void AST_insertFirstArgOrParamNode(AST_ArgOrParamNode **firstArg, AST_ArgOrParamNode *newArg);

/**
 * @brief Vloží uzel na začátek seznamu příkazů.
 *
 * @details Tato funkce přidá nový uzel na začátek seznamu příkazů. Pokud je
 *          seznam prázdný, nový uzel se stane prvním uzlem v seznamu.
 *
 * @param firstStat Ukazatel na ukazatel na první uzel seznamu příkazů.
 * @param newStat Ukazatel na nový uzel, který má být přidán na začátek seznamu.
 */
void AST_insertFirstStatementNode(AST_StatementNode **firstStat, AST_StatementNode *newStat);

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
