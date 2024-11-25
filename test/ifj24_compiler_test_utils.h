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
#include "error.h"
#include "scanner.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "parser_common.h"
#include "llparser.h"
#include "lltable.h"
#include "precedence_parser.h"
#include "precedence_table.h"
#include "precedence_stack.h"
#include "symtable.h"
#include "frame_stack.h"
#include "semantic_analyser.h"
#include "tac_generator.h"
#include "built_in_functions.h"
}

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using namespace testing;
using namespace internal;

extern string exam_path;
extern string lex_path;
extern string synt_path;
extern string synt_error_path;
extern string sem_path;


/*******************************************************************************
 *                                                                             *
 *                          ESCAPE SEKVENCE PRO BARVY                          *
 *                                                                             *
 ******************************************************************************/

#define COLOR_RESET   "\033[0m"         /**< ANSI escape sekvence pro resetovvání barvy */
#define COLOR_GOLD    "\033[38;5;220m"  /**< ANSI escape sekvence pro zlatou barvu */
#define COLOR_BLUE    "\033[34m"        /**< ANSI escape sekvence pro modrou barvu */
#define COLOR_PINK    "\033[35m"        /**< ANSI escape sekvence pro růžovou barvu */
#define COLOR_RED     "\033[91m"        /**< ANSI escape sekvence pro červenou barvu */


/*******************************************************************************
 *                                                                             *
 *                               SPECIÁLNÍ MAKRA                               *
 *                                                                             *
 ******************************************************************************/

#define MAKE_STRING(id, str) \
    DString* id = string_init(); \
    do { \
        ASSERT_NE(id, nullptr); \
        const char *tmp_str = str; \
        for(size_t i = 0; i < strlen(tmp_str); i++) { \
            ASSERT_EQ(string_append_char(id, tmp_str[i]), STRING_SUCCESS); \
        } \
    } while(false);

/**
 * @brief Definice makra pro zapnutí/vypnutí výpisu stromu.
*/
#if PRINT_AST_OUT
    #define PRINT_AST(type, node) \
        do { \
            if(node != nullptr) { \
                const TestInfo *test_info = UnitTest::GetInstance()->current_test_info(); \
                string output = ASTutils_printCapturedOutput(type, (void *)node, true); \
                cerr << endl << COLOR_PINK << "Output Tree for " << test_info->test_case_name() << "." << test_info->name() << ":" << COLOR_RESET << endl; \
                cerr << output << endl; \
            } \
        } while(false)
#else
    #define PRINT_AST(type, node) do {} while(false)
#endif

#if PRINT_FRAME_STACK_OUT
    #define PRINT_FRAME_STACK() \
        do { \
            frameStack_printArray(stderr, true, false); \
        } while(false)
#else
    #define PRINT_FRAME_STACK() do {} while (0)
#endif

#ifndef DISABLE_PRINT
    #ifdef SPECIFIC_TEST_NAME
        #define PRINT_TEST_LOG(test_name) \
            do { \
                if(strcmp(#test_name, SPECIFIC_TEST_NAME) == 0) { \
                    PRINT_AST(AST_PROGRAM_NODE, ASTroot); \
                    PRINT_FRAME_STACK(); \
                } \
            } while(false)
        #else
            #define PRINT_TEST_LOG(test_name) \
                do { \
                    PRINT_AST(AST_PROGRAM_NODE, ASTroot); \
                    PRINT_FRAME_STACK(); \
                }while(false)
            #endif
    #else
        #define PRINT_TEST_LOG(test_name) do {} while(false)
#endif

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
