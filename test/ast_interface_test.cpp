/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_test.cpp                                              *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            08.11.2024                                                *
 * Poslední změna:   08.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ast_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy stavby abstraktního syntaktického stromu AST.
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `ast_interface.c`. Testy jsou vytvořené za využití 
 *          frameworku Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "ast_nodes.h"
#include "ast_interface.h"
}
#include "ast_test_utils.h"

using namespace std;
using namespace testing;
using namespace internal;

/** 
 * @brief Definice makra pro zapnutí/vypnutí výpisu stromu.
*/
#define PRINT_TREE_OUTPUT 1

#if PRINT_TREE_OUTPUT
#define PRINT_TREE(type, node) \
    do { \
        const ::testing::TestInfo* test_info = ::testing::UnitTest::GetInstance()->current_test_info(); \
        string output = ASTutils_printCapturedOutput(type, (void *)node, true); \
        cerr << endl << COLOR_PINK << "Output Tree for " << test_info->test_case_name() << "." << test_info->name() << ":" << COLOR_RESET << endl; \
        cerr << output << endl; \
    } while (0)
#else
#define PRINT_TREE(node) do {} while (0)
#endif


/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot;

/**
 * @brief Test kořene, kde je pouze neinicializovaný kořenový uzel.
 */
TEST(ASTtest_Root, UnInitRoot) {
    AST_ProgramNode *root = (AST_ProgramNode *)AST_createNode(AST_PROGRAM_NODE);

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->type, AST_PROGRAM_NODE);
    ASSERT_EQ(root->importedFile, nullptr);
    ASSERT_EQ(root->functionList, nullptr);

    PRINT_TREE(root->type, root);

    AST_destroyNode(AST_PROGRAM_NODE, root);
}

/**
 * @brief Test kořene, kde je pouze jednodušše inicializovaný kořenový uzel.
 */
TEST(ASTtest_Root, InitRoot) {
    AST_createTree();
    ASSERT_NE(ASTroot, nullptr);
    ASTroot->importedFile = string_init();
    ASSERT_NE(ASTroot->importedFile, nullptr);
    ASSERT_EQ(string_append_char(ASTroot->importedFile, 'i'), STRING_SUCCESS);
    ASSERT_EQ(ASTroot->type, AST_PROGRAM_NODE);
    ASSERT_EQ(ASTroot->functionList, nullptr);

    PRINT_TREE(ASTroot->type, ASTroot);

    AST_destroyTree();
}

/**
 * @brief Test kořene, který odkazuje na několik zlinkovaných definici funkcí.
 */
TEST(ASTtest_Root, ManyFuncLinkedRoot) {
    AST_ProgramNode *root = (AST_ProgramNode *)AST_createNode(AST_PROGRAM_NODE);
    ASSERT_NE(root, nullptr);
    root->importedFile = string_init();
    ASSERT_NE(root->importedFile, nullptr);
    ASSERT_EQ(string_append_char(root->importedFile, 'i'), STRING_SUCCESS);

    AST_FunDefNode *func1 = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(func1, nullptr);
    func1->identifier = string_init();
    ASSERT_NE(func1->identifier, nullptr);
    ASSERT_EQ(string_append_char(func1->identifier, 'f'), STRING_SUCCESS);
    func1->returnType = AST_DATA_TYPE_INT;

    AST_FunDefNode *func2 = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(func2, nullptr);
    func2->identifier = string_init();
    ASSERT_NE(func2->identifier, nullptr);
    ASSERT_EQ(string_append_char(func2->identifier, 'g'), STRING_SUCCESS);
    func2->returnType = AST_DATA_TYPE_FLOAT;

    func1->next = func2;
    root->functionList = func1;

    ASSERT_EQ(root->functionList, func1);
    ASSERT_EQ(root->functionList->next, func2);

    PRINT_TREE(root->type, root);

    AST_destroyNode(AST_PROGRAM_NODE, root);
}

/**
 * @brief Test AST_FunDefNode, který je NEinicializovaný.
 */
TEST(ASTtest_FunDefNode, UnInitFunDefNode) {
    AST_FunDefNode *node = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->type, AST_FUN_DEF_NODE);
    ASSERT_EQ(node->identifier, nullptr);
    ASSERT_EQ(node->parameters, nullptr);
    ASSERT_EQ(node->returnType, AST_DATA_TYPE_NOT_DEFINED);
    ASSERT_EQ(node->body, nullptr);
    ASSERT_EQ(node->next, nullptr);

    PRINT_TREE(node->type, node);
    
    AST_destroyNode(AST_FUN_DEF_NODE, node);
}

/**
 * @brief Test AST_FunDefNode, který je jednodušše inicializovaný.
 */
TEST(ASTtest_FunDefNode, InitFunDefNode) {
    AST_FunDefNode *node = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'f'), STRING_SUCCESS);
    node->returnType = AST_DATA_TYPE_INT;
    ASSERT_EQ(node->type, AST_FUN_DEF_NODE);
    ASSERT_EQ(node->parameters, nullptr);
    ASSERT_EQ(node->body, nullptr);
    ASSERT_EQ(node->next, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_FUN_DEF_NODE, node);
}

/**
 * @brief Test AST_FunDefNode, který odkazuje na několik dalších funkcí.
 */
TEST(ASTtest_FunDefNode, LinkedFunDefNode) {
    AST_FunDefNode *node1 = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(node1, nullptr);
    node1->identifier = string_init();
    ASSERT_NE(node1->identifier, nullptr);
    ASSERT_EQ(string_append_char(node1->identifier, 'f'), STRING_SUCCESS);
    node1->returnType = AST_DATA_TYPE_INT;

    AST_FunDefNode *node2 = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(node2, nullptr);
    node2->identifier = string_init();
    ASSERT_NE(node2->identifier, nullptr);
    ASSERT_EQ(string_append_char(node2->identifier, 'g'), STRING_SUCCESS);
    node2->returnType = AST_DATA_TYPE_FLOAT;

    node1->next = node2;

    ASSERT_EQ(node1->next, node2);

    PRINT_TREE(node1->type, node1);
    PRINT_TREE(node2->type, node2);

    AST_destroyNode(AST_FUN_DEF_NODE, node1);
    AST_destroyNode(AST_FUN_DEF_NODE, node2);
}

/**
 * @brief Test AST_FunDefNode, který má komplexní těle AST_StatementNode.
 */
TEST(ASTtest_FunDefNode, ComplexStatFunDefNode) {
    AST_FunDefNode *node = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'f'), STRING_SUCCESS);
    node->returnType = AST_DATA_TYPE_INT;

    AST_StatementNode *stmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmt, nullptr);
    stmt->statementType = AST_STATEMENT_VAR_DEF;
    node->body = stmt;

    ASSERT_EQ(node->body, stmt);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_FUN_DEF_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který je NEinicializovaný.
 */
TEST(ASTtest_ArgOrParamNode, UnInitArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->type, AST_ARG_OR_PARAM_NODE);
    ASSERT_EQ(node->identifier, nullptr);
    ASSERT_EQ(node->variable, nullptr);
    ASSERT_EQ(node->next, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který je jednodušše inicializovaný.
 */
TEST(ASTtest_ArgOrParamNode, InitArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);
    ASSERT_EQ(node->type, AST_ARG_OR_PARAM_NODE);
    ASSERT_EQ(node->variable, nullptr);
    ASSERT_EQ(node->next, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který odkazuje na několik dalších funkcí.
 */
TEST(ASTtest_ArgOrParamNode, LinkedArgOrParamNode) {
    AST_ArgOrParamNode *node1 = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node1, nullptr);
    node1->identifier = string_init();
    ASSERT_NE(node1->identifier, nullptr);
    ASSERT_EQ(string_append_char(node1->identifier, 'a'), STRING_SUCCESS);

    AST_ArgOrParamNode *node2 = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node2, nullptr);
    node2->identifier = string_init();
    ASSERT_NE(node2->identifier, nullptr);
    ASSERT_EQ(string_append_char(node2->identifier, 'b'), STRING_SUCCESS);

    node1->next = node2;

    ASSERT_EQ(node1->next, node2);

    PRINT_TREE(node1->type, node1);
    PRINT_TREE(node2->type, node2);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node1);
    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node2);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje proměnnou typu INT.
 */
TEST(ASTtest_ArgOrParamNode, IntegerVarArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_INT;
    int *value = (int *)malloc(sizeof(int));
    *value = 42;
    var->value = value;
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje proměnnou typu FLOAT.
 */
TEST(ASTtest_ArgOrParamNode, FloatVarArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_FLOAT;
    double *value = (double *)malloc(sizeof(double));
    *value = 3.14;
    var->value = value;
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje proměnnou typu STRING (Dstring).
 */
TEST(ASTtest_ArgOrParamNode, StringVarArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_STRING;
    var->value = string_init();
    ASSERT_NE(var->value, nullptr);
    ASSERT_EQ(string_append_char((DString *)var->value, 'H'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'e'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'o'), STRING_SUCCESS);
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje literál typu INT.
 */
TEST(ASTtest_ArgOrParamNode, IntegerLitArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_INT;
    int *value = (int *)malloc(sizeof(int));
    *value = 42;
    var->value = value;
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje literál typu FLOAT.
 */
TEST(ASTtest_ArgOrParamNode, FloatLitArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_FLOAT;
    double *value = (double *)malloc(sizeof(double));
    *value = 3.14;
    var->value = value;
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje literál typu STRING (Dstring).
 */
TEST(ASTtest_ArgOrParamNode, StringLitArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_STRING;
    var->value = string_init();
    ASSERT_NE(var->value, nullptr);
    ASSERT_EQ(string_append_char((DString *)var->value, 'H'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'e'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char((DString *)var->value, 'o'), STRING_SUCCESS);
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_ArgOrParamNode, který obsahuje literál typu NULL.
 */
TEST(ASTtest_ArgOrParamNode, NullLitArgOrParamNode) {
    AST_ArgOrParamNode *node = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(node, nullptr);
    node->identifier = string_init();
    ASSERT_NE(node->identifier, nullptr);
    ASSERT_EQ(string_append_char(node->identifier, 'a'), STRING_SUCCESS);

    AST_VarNode *var = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(var, nullptr);
    var->literalType = AST_LITERAL_NULL;
    var->value = nullptr;
    node->variable = var;

    ASSERT_EQ(node->variable, var);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_ARG_OR_PARAM_NODE, node);
}

/**
 * @brief Test AST_StatementNode, který je NEinicializovaný.
 */
TEST(ASTtest_StatementNode, UnInitStatementNode) {
    AST_StatementNode *node = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->type, AST_STATEMENT_NODE);
    ASSERT_EQ(node->statementType, AST_STATEMENT_NOT_DEFINED);
    ASSERT_EQ(node->statement, nullptr);
    ASSERT_EQ(node->next, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_STATEMENT_NODE, node);
}

/**
 * @brief Test AST_StatementNode, který je jednodušše inicializovaný.
 */
TEST(ASTtest_StatementNode, InitStatementNode) {
    AST_StatementNode *node = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node, nullptr);
    node->statementType = AST_STATEMENT_VAR_DEF;
    ASSERT_EQ(node->type, AST_STATEMENT_NODE);
    ASSERT_EQ(node->statement, nullptr);
    ASSERT_EQ(node->next, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_STATEMENT_NODE, node);
}

/**
 * @brief Test AST_StatementNode, který odkazuje na AST_ExprNode.
 */
TEST(ASTtest_StatementNode, ExprStatementNode) {
    AST_StatementNode *node = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node, nullptr);
    node->statementType = AST_STATEMENT_EXPR;

    AST_ExprNode *expr = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(expr, nullptr);
    expr->exprType = AST_EXPR_LITERAL;
    node->statement = expr;

    ASSERT_EQ(node->statement, expr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_STATEMENT_NODE, node);
}

/**
 * @brief Test AST_StatementNode, který odkazuje na AST_FunCallNode.
 */
TEST(ASTtest_StatementNode, FunCallStatementNode) {
    AST_StatementNode *node = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node, nullptr);
    node->statementType = AST_STATEMENT_FUN_CALL;

    AST_FunCallNode *funCall = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    ASSERT_NE(funCall, nullptr);
    funCall->identifier = string_init();
    ASSERT_NE(funCall->identifier, nullptr);
    ASSERT_EQ(string_append_char(funCall->identifier, 'f'), STRING_SUCCESS);
    node->statement = funCall;

    ASSERT_EQ(node->statement, funCall);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_STATEMENT_NODE, node);
}

/**
 * @brief Test AST_StatementNode, který odkazuje na AST_IfNode.
 */
TEST(ASTtest_StatementNode, IfStatementNode) {
    AST_StatementNode *node = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node, nullptr);
    node->statementType = AST_STATEMENT_IF;

    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(ifNode, nullptr);
    node->statement = ifNode;

    ASSERT_EQ(node->statement, ifNode);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_STATEMENT_NODE, node);
}

/**
 * @brief Test AST_StatementNode, který odkazuje na AST_WhileNode.
 */
TEST(ASTtest_StatementNode, WhileStatementNode) {
    AST_StatementNode *node = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node, nullptr);
    node->statementType = AST_STATEMENT_WHILE;

    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(whileNode, nullptr);
    node->statement = whileNode;

    ASSERT_EQ(node->statement, whileNode);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_STATEMENT_NODE, node);
}

/**
 * @brief Test AST_IfNode, který je NEinicializovaný.
 */
TEST(ASTtest_IfNode, UnInitIfNode) {
    AST_IfNode *node = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->type, AST_IF_NODE);
    ASSERT_EQ(node->condition, nullptr);
    ASSERT_EQ(node->nullCondition, nullptr);
    ASSERT_EQ(node->thenBranch, nullptr);
    ASSERT_EQ(node->elseBranch, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_IF_NODE, node);
}

/**
 * @brief Test AST_IfNode, který je jednodušše inicializovaný.
 */
TEST(ASTtest_IfNode, InitIfNode) {
    AST_IfNode *node = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(node, nullptr);
    node->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->condition, nullptr);
    node->condition->exprType = AST_EXPR_LITERAL;
    ASSERT_EQ(node->type, AST_IF_NODE);
    ASSERT_EQ(node->nullCondition, nullptr);
    ASSERT_EQ(node->thenBranch, nullptr);
    ASSERT_EQ(node->elseBranch, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_IF_NODE, node);
}

/**
 * @brief Test AST_IfNode, který má plně inicializovanou NullCondition.
 */
TEST(ASTtest_IfNode, NullCondIfNode) {
    AST_IfNode *node = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(node, nullptr);
    node->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->condition, nullptr);
    node->condition->exprType = AST_EXPR_LITERAL;

    AST_VarNode *nullCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(nullCond, nullptr);
    nullCond->literalType = AST_LITERAL_NULL;
    node->nullCondition = nullCond;

    ASSERT_EQ(node->nullCondition, nullCond);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_IF_NODE, node);
}

/**
 * @brief Test AST_IfNode, který je velmi komplexní.
 * 
 * @details Plně inicializovanou podmínku, nemá NullCondition, má ThenBranch
 *          a ElseBranch s třemi příkazy.
 */
TEST(ASTtest_IfNode, ComplexIfNode) {
    AST_IfNode *node = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(node, nullptr);
    node->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->condition, nullptr);
    node->condition->exprType = AST_EXPR_BINARY_OP;
    node->condition->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(node->condition->expression, nullptr);
    ((AST_BinOpNode *)node->condition->expression)->op = AST_OP_LESS_THAN;

    node->thenBranch = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node->thenBranch, nullptr);
    node->thenBranch->statementType = AST_STATEMENT_EXPR;
    node->thenBranch->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->thenBranch->statement, nullptr);

    node->elseBranch = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node->elseBranch, nullptr);
    node->elseBranch->statementType = AST_STATEMENT_EXPR;
    node->elseBranch->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->elseBranch->statement, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_IF_NODE, node);
}

/**
 * @brief Test AST_WhileNode, který je NEinicializovaný.
 */
TEST(ASTtest_WhileNode, UnInitWhileNode) {
    AST_WhileNode *node = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->type, AST_WHILE_NODE);
    ASSERT_EQ(node->condition, nullptr);
    ASSERT_EQ(node->nullCondition, nullptr);
    ASSERT_EQ(node->body, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_WHILE_NODE, node);
}

/**
 * @brief Test AST_WhileNode, který je jednodušše inicializovaný.
 */
TEST(ASTtest_WhileNode, InitWhileNode) {
    AST_WhileNode *node = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(node, nullptr);
    node->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->condition, nullptr);
    node->condition->exprType = AST_EXPR_LITERAL;
    ASSERT_EQ(node->type, AST_WHILE_NODE);
    ASSERT_EQ(node->nullCondition, nullptr);
    ASSERT_EQ(node->body, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_WHILE_NODE, node);
}

/**
 * @brief Test AST_WhileNode, který má plně inicializovanou NullCondition.
 */
TEST(ASTtest_WhileNode, NullCondWhileNode) {
    AST_WhileNode *node = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(node, nullptr);
    node->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->condition, nullptr);
    node->condition->exprType = AST_EXPR_LITERAL;

    AST_VarNode *nullCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(nullCond, nullptr);
    nullCond->literalType = AST_LITERAL_NULL;
    node->nullCondition = nullCond;

    ASSERT_EQ(node->nullCondition, nullCond);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_WHILE_NODE, node);
}

/**
 * @brief Test AST_WhileNode, který je velmi komplexní.
 * 
 * @details Plně inicializovanou podmínku, nemá NullCondition, má ThenBranch
 *          a ElseBranch s třemi příkazy.
 */
TEST(ASTtest_WhileNode, ComplexWhileNode) {
    AST_WhileNode *node = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(node, nullptr);
    node->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->condition, nullptr);
    node->condition->exprType = AST_EXPR_BINARY_OP;
    node->condition->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(node->condition->expression, nullptr);
    ((AST_BinOpNode *)node->condition->expression)->op = AST_OP_GREATER_THAN;

    node->body = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(node->body, nullptr);
    node->body->statementType = AST_STATEMENT_EXPR;
    node->body->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node->body->statement, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_WHILE_NODE, node);
}

/**
 * @brief Test AST_ExprNode, který je NEinicializovaný.
 */
TEST(ASTtest_ExprNode, UnInitExprNode) {
    AST_ExprNode *node = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->type, AST_EXPR_NODE);
    ASSERT_EQ(node->exprType, AST_EXPR_NOT_DEFINED);
    ASSERT_EQ(node->expression, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_EXPR_NODE, node);
}

/**
 * @brief Test AST_ExprNode, který je jednodušše inicializovaný.
 */
TEST(ASTtest_ExprNode, InitExprNode) {
    AST_ExprNode *node = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node, nullptr);
    node->exprType = AST_EXPR_LITERAL;
    ASSERT_EQ(node->type, AST_EXPR_NODE);
    ASSERT_EQ(node->expression, nullptr);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_EXPR_NODE, node);
}

/**
 * @brief Test AST_ExprNode s plně komplexně inicializovaným AST_BinOpNode.
 */
TEST(ASTtest_ExprNode, BinOpExprNode) {
    AST_ExprNode *node = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node, nullptr);
    node->exprType = AST_EXPR_BINARY_OP;
    node->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(node->expression, nullptr);
    ((AST_BinOpNode *)node->expression)->op = AST_OP_ADD;
    ASSERT_EQ(node->type, AST_EXPR_NODE);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_EXPR_NODE, node);
}

/**
 * @brief Test AST_ExprNode s plně komplexně inicializovaným AST_FunCallNode.
 */
TEST(ASTtest_ExprNode, FunCallExprNode) {
    AST_ExprNode *node = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node, nullptr);
    node->exprType = AST_EXPR_FUN_CALL;
    node->expression = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    ASSERT_NE(node->expression, nullptr);
    ASSERT_EQ(node->type, AST_EXPR_NODE);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_EXPR_NODE, node);
}

/**
 * @brief Test AST_ExprNode s plně komplexně inicializovaným AST_VarNodeNode.
 */
TEST(ASTtest_ExprNode, VarNodeExprNode) {
    AST_ExprNode *node = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(node, nullptr);
    node->exprType = AST_EXPR_VARIABLE;
    node->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(node->expression, nullptr);
    ASSERT_EQ(node->type, AST_EXPR_NODE);

    PRINT_TREE(node->type, node);

    AST_destroyNode(AST_EXPR_NODE, node);
}

/**
 * @brief Test jednoduchého stromu s jednou funkcí a jedním příkazem.
 */
TEST(ASTtest_Tree, SimpleTree) {
    // Vytvoření kořene stromu
    AST_ProgramNode *root = (AST_ProgramNode *)AST_createNode(AST_PROGRAM_NODE);
    ASSERT_NE(root, nullptr);
    root->importedFile = string_init();
    ASSERT_NE(root->importedFile, nullptr);
    ASSERT_EQ(string_append_char(root->importedFile, 'i'), STRING_SUCCESS);

    // Vytvoření funkce
    AST_FunDefNode *func = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(func, nullptr);
    func->identifier = string_init();
    ASSERT_NE(func->identifier, nullptr);
    ASSERT_EQ(string_append_char(func->identifier, 'f'), STRING_SUCCESS);
    func->returnType = AST_DATA_TYPE_INT;

    // Vytvoření příkazu
    AST_StatementNode *stmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmt, nullptr);
    stmt->statementType = AST_STATEMENT_VAR_DEF;
    func->body = stmt;

    // Přidání funkce do kořene
    root->functionList = func;

    // Ověření struktury stromu
    ASSERT_EQ(root->functionList, func);
    ASSERT_EQ(root->functionList->body, stmt);

    // Vypíš strom do terminálu
    PRINT_TREE(root->type, root);

    // Uvolnění stromu
    AST_destroyNode(AST_PROGRAM_NODE, root);
}

/**
 * @brief Test komplexnějšího stromu s více funkcemi, příkazy a výrazy.
 */
TEST(ASTtest_Tree, MoreComplexTree) {
    // Vytvoření kořene stromu
    AST_ProgramNode *root = (AST_ProgramNode *)AST_createNode(AST_PROGRAM_NODE);
    ASSERT_NE(root, nullptr);
    root->importedFile = string_init();
    ASSERT_NE(root->importedFile, nullptr);
    ASSERT_EQ(string_append_char(root->importedFile, 'i'), STRING_SUCCESS);

    // Vytvoření první funkce
    AST_FunDefNode *func1 = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(func1, nullptr);
    func1->identifier = string_init();
    ASSERT_NE(func1->identifier, nullptr);
    ASSERT_EQ(string_append_char(func1->identifier, 'f'), STRING_SUCCESS);
    func1->returnType = AST_DATA_TYPE_INT;

    // Vytvoření druhé funkce
    AST_FunDefNode *func2 = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(func2, nullptr);
    func2->identifier = string_init();
    ASSERT_NE(func2->identifier, nullptr);
    ASSERT_EQ(string_append_char(func2->identifier, 'g'), STRING_SUCCESS);
    func2->returnType = AST_DATA_TYPE_FLOAT;

    // Vytvoření příkazu pro první funkci
    AST_StatementNode *stmt1 = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmt1, nullptr);
    stmt1->statementType = AST_STATEMENT_VAR_DEF;
    func1->body = stmt1;

    // Vytvoření příkazu pro druhou funkci
    AST_StatementNode *stmt2 = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmt2, nullptr);
    stmt2->statementType = AST_STATEMENT_EXPR;
    func2->body = stmt2;

    // Vytvoření výrazu pro druhý příkaz
    AST_ExprNode *expr = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(expr, nullptr);
    expr->exprType = AST_EXPR_BINARY_OP;
    expr->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(expr->expression, nullptr);
    ((AST_BinOpNode *)expr->expression)->op = AST_OP_ADD;
    stmt2->statement = expr;

    // Přidání funkcí do kořene
    func1->next = func2;
    root->functionList = func1;

    // Ověření struktury stromu
    ASSERT_EQ(root->functionList, func1);
    ASSERT_EQ(root->functionList->next, func2);
    ASSERT_EQ(root->functionList->body, stmt1);
    ASSERT_EQ(root->functionList->next->body, stmt2);
    ASSERT_EQ(((AST_StatementNode *)root->functionList->next->body)->statement, expr);

    // Vypíš strom do terminálu
    PRINT_TREE(root->type, root);

    // Uvolnění stromu
    AST_destroyNode(AST_PROGRAM_NODE, root);
}

/**
 * @brief Test komplexního stromu podobajícího se funkci main s if, while a výrazem.
 */
TEST(ASTtest_Tree, ComplexTree) {
    // Vytvoření kořene stromu
    AST_createTree();
    ASSERT_NE(ASTroot, nullptr);
    ASTroot->importedFile = string_init();
    ASSERT_NE(ASTroot->importedFile, nullptr);
    ASSERT_EQ(string_append_char(ASTroot->importedFile, 'i'), STRING_SUCCESS);

    // Vytvoření funkce main
    AST_FunDefNode *mainFunc = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(mainFunc, nullptr);
    mainFunc->identifier = string_init();
    ASSERT_NE(mainFunc->identifier, nullptr);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'm'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'n'), STRING_SUCCESS);
    mainFunc->returnType = AST_DATA_TYPE_INT;

    // Vytvoření příkazu if
    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(ifNode, nullptr);
    ifNode->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(ifNode->condition, nullptr);
    ifNode->condition->exprType = AST_EXPR_BINARY_OP;
    ifNode->condition->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(ifNode->condition->expression, nullptr);
    ((AST_BinOpNode *)ifNode->condition->expression)->op = AST_OP_LESS_THAN;

    // Nastavení levého operandu podmínky if
    AST_ExprNode *leftOperandIf = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandIf, nullptr);
    leftOperandIf->exprType = AST_EXPR_VARIABLE;
    leftOperandIf->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandIf->expression, nullptr);
    ((AST_VarNode *)leftOperandIf->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandIf->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandIf->expression)->identifier, 'a'), STRING_SUCCESS);

    // Nastavení pravého operandu podmínky if
    AST_ExprNode *rightOperandIf = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandIf, nullptr);
    rightOperandIf->exprType = AST_EXPR_LITERAL;
    rightOperandIf->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandIf->expression, nullptr);
    ((AST_VarNode *)rightOperandIf->expression)->literalType = AST_LITERAL_INT;
    int *ifValue = (int *)malloc(sizeof(int));
    *ifValue = 10;
    ((AST_VarNode *)rightOperandIf->expression)->value = ifValue;

    ((AST_BinOpNode *)ifNode->condition->expression)->left = leftOperandIf;
    ((AST_BinOpNode *)ifNode->condition->expression)->right = rightOperandIf;

    // Vytvoření then větve if
    AST_StatementNode *thenStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(thenStmt, nullptr);
    thenStmt->statementType = AST_STATEMENT_EXPR;
    thenStmt->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(thenStmt->statement, nullptr);
    ((AST_ExprNode *)thenStmt->statement)->exprType = AST_EXPR_BINARY_OP;
    ((AST_ExprNode *)thenStmt->statement)->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(((AST_ExprNode *)thenStmt->statement)->expression, nullptr);
    ((AST_BinOpNode *)((AST_ExprNode *)thenStmt->statement)->expression)->op = AST_OP_ADD;

    // Nastavení operandů pro then větev if
    AST_ExprNode *leftOperandThen = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandThen, nullptr);
    leftOperandThen->exprType = AST_EXPR_VARIABLE;
    leftOperandThen->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandThen->expression, nullptr);
    ((AST_VarNode *)leftOperandThen->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandThen->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandThen->expression)->identifier, 'a'), STRING_SUCCESS);

    AST_ExprNode *rightOperandThen = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandThen, nullptr);
    rightOperandThen->exprType = AST_EXPR_LITERAL;
    rightOperandThen->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandThen->expression, nullptr);
    ((AST_VarNode *)rightOperandThen->expression)->literalType = AST_LITERAL_INT;
    int *thenValue = (int *)malloc(sizeof(int));
    *thenValue = 1;
    ((AST_VarNode *)rightOperandThen->expression)->value = thenValue;

    ((AST_BinOpNode *)((AST_ExprNode *)thenStmt->statement)->expression)->left = leftOperandThen;
    ((AST_BinOpNode *)((AST_ExprNode *)thenStmt->statement)->expression)->right = rightOperandThen;

    // Vytvoření else větve if
    AST_StatementNode *elseStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(elseStmt, nullptr);
    elseStmt->statementType = AST_STATEMENT_EXPR;
    elseStmt->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(elseStmt->statement, nullptr);
    ((AST_ExprNode *)elseStmt->statement)->exprType = AST_EXPR_BINARY_OP;
    ((AST_ExprNode *)elseStmt->statement)->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(((AST_ExprNode *)elseStmt->statement)->expression, nullptr);
    ((AST_BinOpNode *)((AST_ExprNode *)elseStmt->statement)->expression)->op = AST_OP_SUBTRACT;

    // Nastavení operandů pro else větev if
    AST_ExprNode *leftOperandElse = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandElse, nullptr);
    leftOperandElse->exprType = AST_EXPR_VARIABLE;
    leftOperandElse->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandElse->expression, nullptr);
    ((AST_VarNode *)leftOperandElse->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandElse->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandElse->expression)->identifier, 'a'), STRING_SUCCESS);

    AST_ExprNode *rightOperandElse = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandElse, nullptr);
    rightOperandElse->exprType = AST_EXPR_LITERAL;
    rightOperandElse->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandElse->expression, nullptr);
    ((AST_VarNode *)rightOperandElse->expression)->literalType = AST_LITERAL_INT;
    int *elseValue = (int *)malloc(sizeof(int));
    *elseValue = 1;
    ((AST_VarNode *)rightOperandElse->expression)->value = elseValue;

    ((AST_BinOpNode *)((AST_ExprNode *)elseStmt->statement)->expression)->left = leftOperandElse;
    ((AST_BinOpNode *)((AST_ExprNode *)elseStmt->statement)->expression)->right = rightOperandElse;

    ifNode->thenBranch = thenStmt;
    ifNode->elseBranch = elseStmt;

    // Vytvoření příkazu while
    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(whileNode, nullptr);
    whileNode->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(whileNode->condition, nullptr);
    whileNode->condition->exprType = AST_EXPR_BINARY_OP;
    whileNode->condition->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(whileNode->condition->expression, nullptr);
    ((AST_BinOpNode *)whileNode->condition->expression)->op = AST_OP_GREATER_THAN;

    // Nastavení operandů pro podmínku while
    AST_ExprNode *leftOperandWhile = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandWhile, nullptr);
    leftOperandWhile->exprType = AST_EXPR_VARIABLE;
    leftOperandWhile->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandWhile->expression, nullptr);
    ((AST_VarNode *)leftOperandWhile->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandWhile->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandWhile->expression)->identifier, 'b'), STRING_SUCCESS);

    AST_ExprNode *rightOperandWhile = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandWhile, nullptr);
    rightOperandWhile->exprType = AST_EXPR_LITERAL;
    rightOperandWhile->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandWhile->expression, nullptr);
    ((AST_VarNode *)rightOperandWhile->expression)->literalType = AST_LITERAL_FLOAT;
    double *whileValue = (double *)malloc(sizeof(double));
    *whileValue = 0.0;
    ((AST_VarNode *)rightOperandWhile->expression)->value = whileValue;

    ((AST_BinOpNode *)whileNode->condition->expression)->left = leftOperandWhile;
    ((AST_BinOpNode *)whileNode->condition->expression)->right = rightOperandWhile;

    // Vytvoření těla while
    AST_StatementNode *whileBody = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(whileBody, nullptr);
    whileBody->statementType = AST_STATEMENT_EXPR;
    whileBody->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(whileBody->statement, nullptr);
    ((AST_ExprNode *)whileBody->statement)->exprType = AST_EXPR_BINARY_OP;
    ((AST_ExprNode *)whileBody->statement)->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(((AST_ExprNode *)whileBody->statement)->expression, nullptr);
    ((AST_BinOpNode *)((AST_ExprNode *)whileBody->statement)->expression)->op = AST_OP_SUBTRACT;

    // Nastavení operandů pro tělo while
    AST_ExprNode *leftOperandWhileBody = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandWhileBody, nullptr);
    leftOperandWhileBody->exprType = AST_EXPR_VARIABLE;
    leftOperandWhileBody->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandWhileBody->expression, nullptr);
    ((AST_VarNode *)leftOperandWhileBody->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandWhileBody->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandWhileBody->expression)->identifier, 'b'), STRING_SUCCESS);

    AST_ExprNode *rightOperandWhileBody = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandWhileBody, nullptr);
    rightOperandWhileBody->exprType = AST_EXPR_LITERAL;
    rightOperandWhileBody->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandWhileBody->expression, nullptr);
    ((AST_VarNode *)rightOperandWhileBody->expression)->literalType = AST_LITERAL_FLOAT;
    double *whileBodyValue = (double *)malloc(sizeof(double));
    *whileBodyValue = 0.1;
    ((AST_VarNode *)rightOperandWhileBody->expression)->value = whileBodyValue;

    ((AST_BinOpNode *)((AST_ExprNode *)whileBody->statement)->expression)->left = leftOperandWhileBody;
    ((AST_BinOpNode *)((AST_ExprNode *)whileBody->statement)->expression)->right = rightOperandWhileBody;

    whileNode->body = whileBody;

    // Vytvoření příkazu return
    AST_StatementNode *returnStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(returnStmt, nullptr);
    returnStmt->statementType = AST_STATEMENT_RETURN;
    returnStmt->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(returnStmt->statement, nullptr);
    ((AST_ExprNode *)returnStmt->statement)->exprType = AST_EXPR_LITERAL;
    ((AST_ExprNode *)returnStmt->statement)->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(((AST_ExprNode *)returnStmt->statement)->expression, nullptr);
    ((AST_VarNode *)((AST_ExprNode *)returnStmt->statement)->expression)->literalType = AST_LITERAL_INT;
    int *returnValue = (int *)malloc(sizeof(int));
    *returnValue = 0;
    ((AST_VarNode *)((AST_ExprNode *)returnStmt->statement)->expression)->value = returnValue;

    // Vytvoření těla funkce main
    AST_StatementNode *mainBody = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(mainBody, nullptr);
    mainBody->statementType = AST_STATEMENT_IF;
    mainBody->statement = ifNode;
    mainBody->next = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(mainBody->next, nullptr);
    mainBody->next->statementType = AST_STATEMENT_WHILE;
    mainBody->next->statement = whileNode;
    mainBody->next->next = returnStmt;

    mainFunc->body = mainBody;

    // Přidání funkce main do kořene
    ASTroot->functionList = mainFunc;

    // Ověření struktury stromu
    ASSERT_EQ(ASTroot->functionList, mainFunc);
    ASSERT_EQ(ASTroot->functionList->body, mainBody);
    ASSERT_EQ(((AST_StatementNode *)ASTroot->functionList->body)->statement, ifNode);
    ASSERT_EQ(((AST_StatementNode *)ASTroot->functionList->body)->next->statement, whileNode);
    ASSERT_EQ(((AST_StatementNode *)ASTroot->functionList->body)->next->next, returnStmt);

    // Vypíš strom do terminálu
    PRINT_TREE(ASTroot->type, ASTroot);

    // Uvolnění stromu
    AST_destroyTree();
}

/*** Konec souboru ast_interface_test.cpp ***/