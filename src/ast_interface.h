/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_interface.h                                           *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            30.10.2024                                                *
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
 * @file ast_interface.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor obsahující deklarace funkcí pro práci s abstraktním
 *        syntaktickým stromem (AST).
 * @details Tento hlavičkový soubor definuje rozhraní pro práci s uzly abstraktního
 *          syntaktického stromu (AST). Obsahuje deklarace funkcí pro vytváření
 *          a destrukci uzlů AST různých typů. Pro zjednodušení práce s uzly, je
 *          navrženo také veřejné rozhraní s obecnou funkcí pro tvorbu a obecnou
 *          funkcí pro destrukci uzlů. Obsaženy jsou také funkce pro inicializaci
 *          nových uzlů konkrétních typů a pro destrukci lineárních seznamů uzlů.
 */

#ifndef AST_INTERFACE_H_
/** @cond  */
#define AST_INTERFACE_H_
/** @endcond  */

// Import standardních knihoven jazyka C
#include <stdlib.h>
#include <limits.h>         // Konstanty pro meze datového typu integer
#include <math.h>           // Konstanty pro meze datového typu double

// Import knihoven pro abstraktní syntaktický strom (AST)
#include "ast_nodes.h"

// Import sdílených knihoven překladače
#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define INT_CONVERTION_BASE 10      /**< Základ pro číslo dekadické soustavy při převodu pomocí "strtol" */


/*******************************************************************************
 *                                                                             *
 *                       FUNKCE URČENÉ PRO VEŘEJNÉ UŽITÍ                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří nový uzel AST daného typu.
 *
 * @details Dynamicky alokuje paměť pro nový uzel AST. Funkce vrací ukazatel
 *          typu @c void*, který je nutné přetypovat na konkrétní typ uzlu.
 *          Pokud dojde k chybě při alokaci paměti, funkce vrací @c NULL.
 *
 * @param [in] type Typ uzlu, který má být vytvořen.
 *
 * @return @c void* Ukazatel na vytvořený uzel, nebo @c NULL v případě chyby
 *         alokace.
 *
 * @note Je vracen ukazatel na void, proto je nutné získaný ukazatel
 *       přetypovat na typ konkrétního uzlu.
 */
void *AST_createNode(AST_NodeType type);

/**
 * @brief Uvolní paměť uzlu AST daného typu.
 *
 * @details Funkce zajistí správné uvolnění všech členů uzlu a samotné struktury
 *          uzlu. Typ uzlu je specifikován parametrem @c type, což umožňuje
 *          správné uvolnění paměti pro různé typy uzlů. Pokud je předán
 *          neplatný ukazatel @c node, funkce neprovádí žádnou akci.
 *
 * @param [in] type Typ uzlu, který má být uvolněn.
 * @param [in] node Ukazatel na uzel, který má být uvolněn.
 */
void AST_destroyNode(AST_NodeType type, void *node);

/**
 * @brief Alokuje paměť pro globální kořen abstraktního syntaktického stromu (AST).
 *
 * @details Funkce alokuje paměť pro kořen AST a inicializuje globální ukazatel
 *          na kořen abstraktního syntaktického stromu. Pokud dojde k chybě při
 *          alokaci paměti, funkce nstaví ukatel na globální uzel na hodnotu
 *          @c NULL a globální ukazatel zůstává nadále neplatný.
 *
 * @note Funkce využívá jako vstupní parametr globální ukazatel na kořen stromu.
 */
void AST_initTree();

/**
 * @brief Uvolní z paměti celý abstraktní syntaktický strom.
 *
 * @details Funkce zahájí rekurzivní destrukci stromu, pokud je kořen platný.
 *          Zajistí správné uvolnění všech členů uzlů, všech uzlů stromu a
 *          nakonec globálního kořenového uzlu. Po uvolnění paměti je globální
 *          ukazatel na kořen stromu nastaven na @c NULL.
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
 * @brief Inicializuje uzel @c AST_FunDefNode pro definici funkce.
 *
 * @details Tato funkce inicializuje uzel pro definici funkce poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] identifier Ukazatel na dynamický řetězec s identifikátorem funkce.
 * @param [in] parameters Ukazatel na seznam parametrů funkce.
 * @param [in] returnType Návratový typ funkce.
 * @param [in] body Ukazatel na tělo funkce.
 */
void AST_initNewFunDefNode(AST_FunDefNode *node, DString *identifier,  \
                           AST_ArgOrParamNode *parameters, AST_DataType returnType,  \
                           AST_StatementNode *body);

/**
 * @brief Inicializuje uzel @c AST_ArgOrParamNode pro argument/parametr funkce.
 *
 * @details Tato funkce inicializuje uzel pro argument nebo parametr funkce s
 *          poskytnutými parametry. Pokud je uzel již inicializován nebo pokud
 *          je předán neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] dataType Datový typ proměnné parametru/argumentu.
 * @param [in] expression Ukazatel na uzel výraz argumentu / parametru.
 */
void AST_initNewArgOrParamNode(AST_ArgOrParamNode *node, AST_DataType dataType, \
                               AST_ExprNode *expression);

/**
 * @brief Inicializuje uzel @c AST_StatementNode pro příkaz.
 *
 * @details Tato funkce inicializuje uzel pro příkaz poskytnutými parametry.
 *          Pokud je uzel již inicializován nebo pokud je předán neplatný
 *          ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] frameID ID rámce, kde je příkaz definován.
 * @param [in] statementType Typ příkazu.
 * @param [in] statement Ukazatel na uzel specifický pro daný příkaz.
 */
void AST_initNewStatementNode(AST_StatementNode *node, size_t frameID, \
                              AST_StatementType statementType, void *statement);

/**
 * @brief Inicializuje uzel @c AST_FunCallNode pro volání funkce.
 *
 * @details Tato funkce inicializuje uzel pro volání funkce poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] identifier Ukazatel na dynamický řetězec s identifikátorem funkce.
 * @param [in] isBuiltIn Příznak, zda je funkce vestavěná (`ifj.*`).
 * @param [in] arguments Ukazatel na seznam argumentů volané funkce.
 */
void AST_initNewFunCallNode(AST_FunCallNode *node, DString *identifier, \
                            bool isBuiltIn, AST_ArgOrParamNode *arguments);

/**
 * @brief Inicializuje uzel @c AST_IfNode pro podmíněný příkaz if.
 *
 * @details Tato funkce inicializuje uzel pro podmíněný příkaz if poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] condition Ukazatel na výraz reprezentující podmínku.
 * @param [in] nullCondition Ukazatel na volitelnou nullable podmínku.
 * @param [in] thenBranch Ukazatel na seznam příkazů větve 'then'.
 * @param [in] elseBranch Ukazatel na seznam příkazů větve 'else'.
 */
void AST_initNewIfNode(AST_IfNode *node, AST_ExprNode *condition, AST_VarNode *nullCondition, \
                       AST_StatementNode *thenBranch, AST_StatementNode *elseBranch);

/**
 * @brief Inicializuje uzel @c AST_WhileNode pro cyklus while.
 *
 * @details Tato funkce inicializuje uzel pro cyklus while poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] condition Ukazatel na výraz reprezentující podmínku.
 * @param [in] nullCondition Ukazatel na volitelnou nullable podmínku.
 * @param [in] body Ukazatel na seznam příkazů těla cyklu.
 */
void AST_initNewWhileNode(AST_WhileNode *node, AST_ExprNode *condition, \
                          AST_VarNode *nullCondition, AST_StatementNode *body);

/**
 * @brief Inicializuje uzel @c AST_ExprNode pro výraz.
 *
 * @details Tato funkce inicializuje uzel pro výraz poskytnutými parametry.
 *          Pokud je uzel již inicializován nebo pokud je předán neplatný
 *          ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] exprType Typ výrazu.
 * @param [in] expression Ukazatel na konkrétní výraz.
 */
void AST_initNewExprNode(AST_ExprNode *node, AST_ExprType exprType, void *expression);

/**
 * @brief Inicializuje uzel @c AST_BinOpNode pro binární operaci.
 *
 * @details Tato funkce inicializuje uzel pro binární operaci poskytnutými
 *          parametry. Pokud je uzel již inicializován nebo pokud je předán
 *          neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] op Typ binární operace.
 * @param [in] left Ukazatel na levý operand (výraz).
 * @param [in] right Ukazatel na pravý operand (výraz).
 */
void AST_initNewBinOpNode(AST_BinOpNode *node, AST_BinOpType op, \
                          AST_ExprNode *left, AST_ExprNode *right);

/**
 * @brief Inicializuje uzel @c AST_VarNode pro proměnnou nebo literál.
 *
 * @details Tato funkce inicializuje uzel pro proměnnou nebo literál
 *          poskytnutými parametry. Pokud je uzel již inicializován nebo pokud
 *          je předán neplatný ukazatel, funkce hlásí interní chybu.
 *
 * @param [in] node Ukazatel na uzel, který má být inicializován.
 * @param [in] type Typ uzlu (`AST_VAR_NODE` nebo `AST_LITERAL_NODE`).
 * @param [in] identifier Ukazatel na dynamický řetězec s identifikátorem proměnné.
 * @param [in] frameID ID rámce, kde je proměnná deklarována.
 * @param [in] literalType Typ literálu.
 * @param [in] value Ukazatel na hodnotu literálu.
 */
void AST_initNewVarNode(AST_VarNode *node, AST_NodeType type, DString *identifier, \
                        size_t frameID, AST_LiteralType literalType, DString *value);


/*******************************************************************************
 *                                                                             *
 *                  INTERNÍ FUNKCE NA TVORBU A DESTRUKCI UZLŮ                  *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří kořenový uzel programu @c AST_ProgramNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_ProgramNode. Funkce
 *          vrací ukazatel na tento uzel. Pokud dojde k chybě při alokaci
 *          paměti, funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_ProgramNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_ProgramNode *AST_createProgramNode();

/**
 * @brief Uvolní paměť pro kořenový uzel programu @c AST_ProgramNode.
 *
 * @details Uvolní veškerou paměť přidruženou ke kořenovému uzlu programu a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_ProgramNode, který má být uvolněn.
 */
void AST_destroyProgramNode(AST_ProgramNode *node);

/**
 * @brief Vytvoří uzel pro definici funkce @c AST_FunDefNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_FunDefNode. Funkce
 *          vrací ukazatel na tento uzel. Pokud dojde k chybě při alokaci
 *          paměti, funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_FunDefNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_FunDefNode *AST_createFunDefNode();

/**
 * @brief Uvolní paměť pro uzel definice funkce @c AST_FunDefNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu definice funkce a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_FunDefNode, který má být uvolněn.
 */
void AST_destroyFunDefNode(AST_FunDefNode *node);

/**
 * @brief Vytvoří uzel pro parametry nebo argumenty funkce @c AST_ArgOrParamNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_ArgOrParamNode. Funkce
 *          vrací ukazatel na tento uzel. Pokud dojde k chybě při alokaci
 *          paměti, funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_ArgOrParamNode, nebo @c NULL v
 *         případě chyby alokace.
 */
AST_ArgOrParamNode *AST_createArgOrParamNode();

/**
 * @brief Uvolní paměť pro uzel parametrů nebo argumentů funkce @c AST_ArgOrParamNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_ArgOrParamNode a
 *          jeho poduzlům.
 *
 * @param [in] node Ukazatel na @c AST_ArgOrParamNode uzel, který má být uvolněn.
 */
void AST_destroyArgOrParamNode(AST_ArgOrParamNode *node);

/**
 * @brief Vytvoří uzel pro příkaz @c AST_StatementNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_StatementNode. Funkce
 *          vrací ukazatel na tento uzel. Pokud dojde k chybě při alokaci
 *          paměti, funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_StatementNode, nebo @c NULL v
 *         případě chyby alokace.
 */
AST_StatementNode *AST_createStatementNode();

/**
 * @brief Uvolní paměť pro uzel příkazu @c AST_StatementNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_StatementNode a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_StatementNode, který má být uvolněn.
 */
void AST_destroyStatementNode(AST_StatementNode *node);

/**
 * @brief Vytvoří uzel pro volání funkce @c AST_FunCallNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_FunCallNode. Funkce
 *          vrací ukazatel na tento uzel. Pokud dojde k chybě při alokaci paměti,
 *          funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_FunCallNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_FunCallNode *AST_createFunCallNode();

/**
 * @brief Uvolní paměť pro uzel volání funkce @c AST_FunCallNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_FunCallNode a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_FunCallNode, který má být uvolněn.
 */
void AST_destroyFunCallNode(AST_FunCallNode *node);

/**
 * @brief Vytvoří uzel pro podmíněný příkaz if @c AST_IfNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_IfNode. Funkce vrací
 *          ukazatel na tento uzel. Pokud dojde k chybě při alokaci paměti,
 *          funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_IfNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_IfNode *AST_createIfNode();

/**
 * @brief Uvolní paměť pro uzel podmíněného příkazu if @c AST_IfNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_IfNode a
 *          jeho poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_IfNode, který má být uvolněn.
 */
void AST_destroyIfNode(AST_IfNode *node);

/**
 * @brief Vytvoří uzel pro cyklus while @c AST_WhileNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_WhileNode. Funkce vrací
 *          ukazatel na tento uzel. Pokud dojde k chybě při alokaci paměti,
 *          funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_WhileNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_WhileNode *AST_createWhileNode();

/**
 * @brief Uvolní paměť pro uzel cyklu while @c AST_WhileNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_WhileNode a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_WhileNode, který má být uvolněn.
 */
void AST_destroyWhileNode(AST_WhileNode *node);

/**
 * @brief Vytvoří uzel pro výraz @c AST_ExprNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_ExprNode. Funkce vrací
 *          ukazatel na tento uzel. Pokud dojde k chybě při alokaci paměti,
 *          funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_ExprNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_ExprNode *AST_createExprNode();

/**
 * @brief Uvolní paměť pro uzel výrazu @c AST_ExprNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_ExprNode a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_ExprNode, který má být uvolněn.
 */
void AST_destroyExprNode(AST_ExprNode *node);

/**
 * @brief Vytvoří uzel pro binární operaci @c AST_BinOpNode.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_BinOpNode. Funkce vrací
 *          ukazatel na tento uzel. Pokud dojde k chybě při alokaci paměti,
 *          funkce vrací @c NULL.
 *
 * @return Ukazatel na vytvořený uzel @c AST_BinOpNode, nebo @c NULL v případě
 *         chyby alokace.
 */
AST_BinOpNode *AST_createBinOpNode();

/**
 * @brief Uvolní paměť pro uzel binární operace @c AST_BinOpNode.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_BinOpNode a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_BinOpNode, který má být uvolněn.
 */
void AST_destroyBinOpNode(AST_BinOpNode *node);

/**
 * @brief Vytvoří uzel pro proměnnou nebo literál @c AST_VarNode*.
 *
 * @details Dynamicky alokuje paměť pro nový uzel @c AST_VarNode*. Funkce
 *          nastaví typ uzlu na @c AST_VAR_NODE nebo @c AST_LITERAL_NODE podle
 *          parametru. Funkce vrací ukazatel na tento uzel. Pokud dojde k chybě
 *          při alokaci paměti, funkce vrací @c NULL.
 *
 * @param [in] type Typ uzlu, který má být vytvořen, buď @c  AST_VAR_NODE nebo
 *                  @c AST_LITERAL_NODE.
 *
 * @return Ukazatel na vytvořený uzel @c AST_VarNode*, nebo @c NULL v
 *         případě chyby alokace.
 */
AST_VarNode *AST_createVarNode(AST_NodeType type);

/**
 * @brief Uvolní paměť pro uzel proměnné nebo literál @c AST_VarNode*.
 *
 * @details Uvolní veškerou paměť přidruženou k uzlu @c AST_VarNode* a jeho
 *          poduzlům.
 *
 * @param [in] node Ukazatel na uzel @c AST_VarNode*, který má být uvolněn.
 */
void AST_destroyVarNode(AST_VarNode *node);

/*******************************************************************************
 *                                                                             *
 *                INTERNÍ FUNKCE NA DESTRUKCI SEZNAMŮ UZLŮ AST                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Uvolní paměť pro všechny uzly v seznamu parametrů/argumentů v uzlu pro
 *        funkci.
 *
 * @details Iteruje přes seznam @c AST_ArgOrParamNode uzlů a uvolní paměť
 *          každého z nich. Nakonec uvolní samotný seznam.
 *
 * @param [in] list Ukazatel na seznam @c AST_ArgOrParamNode uzlů, které mají
 *                  být uvolněny.
 */
void AST_destroyArgOrParamList(AST_ArgOrParamNode *list);

/**
 * @brief Uvolní paměť pro všechny uzly v seznamu pro příkazy.
 *
 * @details Iteruje přes seznam @c AST_StatementNode uzlů a uvolní paměť
 *          každého z nich. Nakonec uvolní samotný seznam.
 *
 * @param [in] list Ukazatel na seznam @c AST_StatementNode uzlů, které mají být
 *                  uvolněny.
 */
void AST_destroyStatementList(AST_StatementNode *list);

/**
 * @brief Uvolní paměť pro všechny uzly v seznamu definovaných funkcí.
 *
 * @details Iteruje přes seznam @c AST_FunDefNode uzlů a uvolní paměť každého z
 *          nich. Nakonec uvolní samotný seznam.
 *
 * @param [in] list Ukazatel na seznam @c AST_FunDefNode uzlů, které mají být
 *                  uvolněny.
 */
void AST_destroyFunDefList(AST_FunDefNode *list);

#endif // AST_INTERFACE_H_

/*** Konec souboru ast_interface.h ***/
