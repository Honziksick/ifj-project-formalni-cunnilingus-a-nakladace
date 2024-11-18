/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser_test.cpp                                           *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            08.11.2024                                                *
 * Poslední změna:   18.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file parser_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy syntaktického analyzátoru (parseru).
 * @details Tento zdrojový soubor obsahuje unit testy celkové funkčnosti
 *          parseru i jednotlivých submodulů parseru jako jsou LL a precedenční
 *          parsery, LL a precedenční tabulky a práce parseru se zásobníkem
 *          rámců. Testy jsou vytvořené za využití frameworku Google Test.
 * @note Přístup k testování inspirován členem týmu Davidem Krejčím.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "parser.h"
#include "llparser.h"
#include "lltable.h"
#include "precedence_parser.h"
#include "precedence_table.h"
#include "precedence_stack.h"
}
#include "ast_test_utils.h"

using namespace std;
using namespace testing;
using namespace internal;

string exam_path = "../ifj24_examples/ifj24_programs/";
string synt_path = "../test/test_examples/syntactic_examples/";

/**
 * @brief xxx.
 */
TEST(LLParserBasics, PrologueAndEmptyMain) {
    // Načtení souboru s programem na STDIN
    string path = synt_path + "prologue_and_empty_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE(AST_PROGRAM_NODE, ASTroot);
    frameStack_print(stderr, true, false);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

void TestParser(){
    ASTroot = LLparser_parseProgram();
    exit(0);
}

TEST(Correct, TwoFunctions){
    std::string path = synt_path + "two_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    ASTroot = LLparser_parseProgram();

    EXPECT_NE(ASTroot, nullptr);

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Název funkce je main
    EXPECT_STREQ(string_toConstChar(fun->identifier), "main");
    // Vrací void
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    // Nemá parametry
    EXPECT_EQ(fun->parameters, nullptr);
    // Má ukazatel na další funkci
    EXPECT_NE(fun->next, nullptr);

    fun = fun->next;
    // Druhá funkce
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Má identifikátor
    EXPECT_NE(fun->identifier, nullptr);
    // Vrací void
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    // Nemá parametry
    EXPECT_EQ(fun->parameters, nullptr);
    // Nemá na další funkci
    EXPECT_EQ(fun->next, nullptr);
    

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, OneParam){
    std::string path = synt_path + "one_param.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    ASTroot = LLparser_parseProgram();

    EXPECT_NE(ASTroot, nullptr);

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Název funkce je main
    EXPECT_STREQ(string_toConstChar(fun->identifier), "main");
    // Vrací void
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    // Má parametr
    EXPECT_NE(fun->parameters, nullptr);
    AST_ArgOrParamNode* param = fun->parameters;

    // Parametr má typ int
    EXPECT_EQ(param->dataType, AST_DATA_TYPE_INT);
    // Parametr má výraz
    EXPECT_NE(param->expression, nullptr);
    AST_ExprNode* expr = param->expression;
    EXPECT_EQ(expr->type, AST_EXPR_NODE);
    // Výraz je proměnná
    EXPECT_EQ(expr->exprType, AST_EXPR_VARIABLE);
    AST_VarNode* var = (AST_VarNode*)expr->expression;
    EXPECT_EQ(var->type, AST_VAR_NODE);
    // Proměnná má identifikátor x
    EXPECT_NE(var->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(var->identifier, "x"), STRING_EQUAL);
    
    // Nemá další parametry
    EXPECT_EQ(param->next, nullptr);

    // Nemá další funkci
    EXPECT_EQ(fun->next, nullptr);

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, TwoParams){
    std::string path = synt_path + "two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    ASTroot = LLparser_parseProgram();

    EXPECT_NE(ASTroot, nullptr);

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Má parametr
    EXPECT_NE(fun->parameters, nullptr);
    AST_ArgOrParamNode* param = fun->parameters;

    // Parametr má typ int
    EXPECT_EQ(param->dataType, AST_DATA_TYPE_INT);
    // Parametr má výraz
    EXPECT_NE(param->expression, nullptr);
    AST_ExprNode* expr = param->expression;
    EXPECT_EQ(expr->type, AST_EXPR_NODE);
    // Výraz je proměnná
    EXPECT_EQ(expr->exprType, AST_EXPR_VARIABLE);
    AST_VarNode* var = (AST_VarNode*)expr->expression;
    EXPECT_EQ(var->type, AST_VAR_NODE);
    // Proměnná má identifikátor x
    EXPECT_NE(var->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(var->identifier, "x"), STRING_EQUAL);
    
    // Má další parametr
    EXPECT_NE(param->next, nullptr);
    param = param->next;

    // Parametr má typ string
    EXPECT_EQ(param->dataType, AST_DATA_TYPE_STRING);
    // Parametr má výraz
    EXPECT_NE(param->expression, nullptr);
    expr = param->expression;
    EXPECT_EQ(expr->type, AST_EXPR_NODE);
    // Výraz je proměnná
    EXPECT_EQ(expr->exprType, AST_EXPR_VARIABLE);
    var = (AST_VarNode*)expr->expression;
    EXPECT_EQ(var->type, AST_VAR_NODE);
    // Proměnná má identifikátor y
    EXPECT_NE(var->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(var->identifier, "y"), STRING_EQUAL);

    // Nemá další parametry
    EXPECT_EQ(param->next, nullptr);


    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, VarDef){
    std::string path = synt_path + "two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    ASTroot = LLparser_parseProgram();


    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce má tělo
    EXPECT_NE(fun->body, nullptr);
    AST_StatementNode* stmt = fun->body;
    // Tělo funkce má jeden příkaz
    EXPECT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->next, nullptr);
    // Příkaz je deklarace proměnné
    EXPECT_EQ(stmt->statementType, AST_STATEMENT_VAR_DEF);
    EXPECT_NE(stmt->statement, nullptr);
    // Příkaz je tedy výraz
    AST_ExprNode* expr = (AST_ExprNode*)stmt->statement;
    // Výraz má binární operátor
    EXPECT_EQ(expr->exprType, AST_EXPR_BINARY_OP);
    AST_BinOpNode* binop = (AST_BinOpNode*)expr->expression;
    // Operátor je přiřazení
    EXPECT_EQ(binop->op, AST_OP_ASSIGNMENT);
    // Levý operand je proměnná
    EXPECT_EQ(binop->left->exprType, AST_EXPR_VARIABLE);
    AST_VarNode* var = (AST_VarNode*)binop->left->expression;
    // Proměnná má identifikátor x
    EXPECT_NE(var->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(var->identifier, "x"), STRING_EQUAL);
    // Pravý operand je literál
    EXPECT_EQ(binop->right->exprType, AST_EXPR_LITERAL);
    AST_VarNode* lit = (AST_VarNode*)binop->right->expression;
    EXPECT_EQ(lit->identifier, nullptr);
    // Literál je typu int
    EXPECT_EQ(lit->literalType, AST_LITERAL_INT);

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, FunCall){
    std::string path = synt_path + "two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    ASTroot = LLparser_parseProgram();

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce má tělo
    EXPECT_NE(fun->body, nullptr);
    AST_StatementNode* stmt = fun->body;
    // Tělo funkce má jeden příkaz
    EXPECT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->next, nullptr);
    // Příkaz je volání funkce
    EXPECT_EQ(stmt->statementType, AST_STATEMENT_FUN_CALL);
    EXPECT_NE(stmt->statement, nullptr);
    // Příkaz je tedy výraz
    AST_FunCallNode* fcall = (AST_FunCallNode*)stmt->statement;
    // Volaná funkce má identifikátor foo
    EXPECT_NE(fcall->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(fcall->identifier, "foo"), STRING_EQUAL);
    // Funkce nemá argumenty
    EXPECT_EQ(fcall->arguments, nullptr);
    // Funkce není vestavěná
    EXPECT_FALSE(fcall->isBuiltIn);
    
    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, NonVoidFun){
    std::string path = synt_path + "two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    ASTroot = LLparser_parseProgram();

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce vrací int
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_INT);

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

/*** Konec souboru parser_test.cpp ***/