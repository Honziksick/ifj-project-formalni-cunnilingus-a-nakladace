 /*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_test_utils.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            08.11.2024                                                *
 * Poslední změna:   09.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ast_test_utils.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor s deklaracemi utility funkcí pro práci s AST v testovacím prostředí.
 *
 * @details Tento soubor deklaruje pomocné funkce pro tisk, porovnání a
 *          formátování různých typů uzlů AST. Obsahuje také escape sekvence
 *          pro zobrazení barevného výstupu, což usnadňuje vizualizaci struktury
 *          AST během testování.
 */

#ifndef AST_TEST_UTILS_H_
/** @cond  */
#define AST_TEST_UTILS_H_
/** @endcond  */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
extern "C" {
#include "ast_nodes.h"
#include "ast_interface.h"
}

using namespace std;  /**< Jmenný prostor standardní knihovny C++ */


/*******************************************************************************
 *                                                                             *
 *                          ESCAPE SEKVENCE PRO BARVY                          *
 *                                                                             *
 ******************************************************************************/

#define COLOR_RESET   "\033[0m"         /**< ANSI escape sekvence pro resetovvání barvy */
#define COLOR_GOLD    "\033[38;5;220m"  /**< ANSI escape sekvence pro zlatou barvu */
#define COLOR_BLUE    "\033[34m"        /**< ANSI escape sekvence pro modrou barvu */
#define COLOR_PINK    "\033[35m"        /**< ANSI escape sekvence pro růžovou barvu */

/*******************************************************************************
 *                                                                             *
 *                        VEŘEJNÉ FUNKCE PRO VÝPIS AST                         *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Zachytí výstup tisku uzlu AST_NODE_PROGRAM.
 * @details Tato funkce zachytí výstup tisku AST a vrátí ho jako řetězec.
 *
 * @param [in] node Uzel programu AST k vytištění.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @return Zachycený výstup tisku jako řetězec.
 */
string ASTutils_printCapturedOutput(AST_NodeType type, void *node, bool useColors = false);

/**
 * @brief Vytiskne výstup testu a referenční výstup.
 * @details Tato funkce vytiskne výstup testu a referenční výstup pro porovnání.
 *
 * @param[in] output Výstup testu.
 * @param[in] reference Referenční výstup.
 */
void ASTutils_printDiff(string output, string reference, ostream &out = cerr);

/*******************************************************************************
 *                                                                             *
 *                        INTERNÍ FUNKCE PRO VÝPIS AST                         *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytiskne uzel AST_NODE_PROGRAM.
 * @details Tato funkce vytiskne uzel programu AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel programu AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout. Výchozí je `cout`.
 * @param [in] indent Úroveň odsazení. Výchozí je 0.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 */
void ASTutils_printProgramNode(AST_ProgramNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_FUN_DEF.
 * @details Tato funkce vytiskne uzel definice funkce AST do specifikovaného
 *          výstupního streamu s daným odsazením.
 *
 * @param [in] node Uzel definice funkce AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printFunDefNode(AST_FunDefNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_ARG_OR_PARAM.
 * @details Tato funkce vytiskne uzel argumentu nebo parametru AST do
 *          specifikovaného výstupního streamu s daným odsazením.
 *
 * @param [in] node Uzel argumentu nebo parametru AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 */
void ASTutils_printArgOrParamNode(AST_ArgOrParamNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels);

/**
 * @brief Vytiskne uzel AST_NODE_STATEMENT.
 * @details Tato funkce vytiskne uzel příkazu AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel příkazu AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 */
void ASTutils_printStatementNode(AST_StatementNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels);

/**
 * @brief Vytiskne uzel AST_NODE_FUN_CALL.
 * @details Tato funkce vytiskne uzel volání funkce AST do specifikovaného
 *          výstupního streamu s daným odsazením.
 *
 * @param [in] node Uzel volání funkce AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printFunCallNode(AST_FunCallNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_IF.
 * @details Tato funkce vytiskne uzel if AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel if AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printIfNode(AST_IfNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_WHILE.
 * @details Tato funkce vytiskne uzel while AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel while AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printWhileNode(AST_WhileNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_EXPR.
 * @details Tato funkce vytiskne uzel výrazu AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel výrazu AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printExprNode(AST_ExprNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_BIN_OP.
 * @details Tato funkce vytiskne uzel binární operace AST do specifikovaného
 *          výstupního streamu s daným odsazením.
 *
 * @param [in] node Uzel binární operace AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printBinOpNode(AST_BinOpNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_LITERAL.
 * @details Tato funkce vytiskne uzel literálu AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel literálu AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printLiteralNode(AST_VarNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Vytiskne uzel AST_NODE_VAR_DEF.
 * @details Tato funkce vytiskne uzel proměnné AST do specifikovaného výstupního
 *          streamu s daným odsazením.
 *
 * @param [in] node Uzel proměnné AST k vytištění.
 * @param [in] out Výstupní proud, do kterého se bude tisknout.
 * @param [in] indent Úroveň odsazení.
 * @param [in] useColors Příznak pro použití barevného výstupu (výchozí hodnota je false)
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param [in] isLastChild Příznak pro určení poslední funkce v seznamu uzlů funkcí.
 */
void ASTutils_printVarNode(AST_VarNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild);

/**
 * @brief Pomocná funkce pro výpis odsazení.
 * @details Funkce generuje znaky pro vizuální propojení uzlů ve stylu `tree`.
 *          Pokud `isLastChild` je true, použije se koncová větev "└──",
 *          jinak mezilehlá větev "├──".
 *
 * @param[in] indent Počet odsazení (mezer), které se mají vytisknout.
 * @param[in] out Výstupní proud, do kterého se budou odsazení tisknout.
 * @param [in] levels Vektor sledující úroveň zanoření pro správné formátování.
 * @param[in] isLastChild Určuje typ propojení uzlů.
 */
void ASTutils_printIndent(int indent, std::ostream &out, const vector<bool> &levels, bool isLastChild);

#endif // AST_TEST_UTILS_H_

/*** Konec souboru ast_test_utils.h ***/
