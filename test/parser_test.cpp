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

// Definice názvu specifického testu, pro který se má provádět tisk
// Pokud není definováno, tisk se provádí pro všechny testy
#define SPECIFIC_TEST_NAME "CorrectFunCallStatCommaLast"
//#define DISABLE_PRINT

#ifndef DISABLE_PRINT
#ifdef SPECIFIC_TEST_NAME
#define PRINT_TREE_AND_FRAMESTACK(test_name) \
    do { \
        if (strcmp(#test_name, SPECIFIC_TEST_NAME) == 0) { \
            PRINT_TREE(AST_PROGRAM_NODE, ASTroot); \
            frameStack_printArray(stderr, true, false); \
        } \
    } while (0)
#else
#define PRINT_TREE_AND_FRAMESTACK(test_name) \
    do { \
        PRINT_TREE(AST_PROGRAM_NODE, ASTroot); \
        frameStack_printArray(stderr, true, false); \
    } while (0)
#endif
#else
#define PRINT_TREE_AND_FRAMESTACK(test_name) do {} while (0)
#endif

string exam_path = "../ifj24_examples/ifj24_programs/";
string synt_path = "../test/test_examples/syntactic_examples/";

/*
TEST(LLParserBasicsCorrect, PrologueAndEmptyMain) {
    // Načtení souboru s programem na STDIN
    string path = synt_path + "correct_prologue_and_empty_main.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectPrologueAndEmptyMain);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, OneParam){
    string path = synt_path + "correct_one_param.zig";
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

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Název funkce je main
    char *comp = string_toConstChar(fun->identifier);
    EXPECT_STREQ(comp, "main");
    free(comp);
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

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectOneParam);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, TwoParams){
    string path = synt_path + "correct_two_params.zig";
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
    AST_ExprNode* expr1 = param->expression;
    EXPECT_EQ(expr1->type, AST_EXPR_NODE);
    // Výraz je proměnná
    EXPECT_EQ(expr1->exprType, AST_EXPR_VARIABLE);
    AST_VarNode* var1 = (AST_VarNode*)expr1->expression;
    EXPECT_EQ(var1->type, AST_VAR_NODE);
    // Proměnná má identifikátor x
    EXPECT_NE(var1->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(var1->identifier, "x"), STRING_EQUAL);
    
    // Má další parametr
    EXPECT_NE(param->next, nullptr);
    param = param->next;

    // Parametr má typ string
    EXPECT_EQ(param->dataType, AST_DATA_TYPE_STRING);
    // Parametr má výraz
    EXPECT_NE(param->expression, nullptr);
    AST_ExprNode* expr2 = param->expression;
    EXPECT_EQ(expr2->type, AST_EXPR_NODE);
    // Výraz je proměnná
    EXPECT_EQ(expr2->exprType, AST_EXPR_VARIABLE);
    AST_VarNode* var2 = (AST_VarNode*)expr2->expression;
    EXPECT_EQ(var2->type, AST_VAR_NODE);
    // Proměnná má identifikátor y
    EXPECT_NE(var2->identifier, nullptr);
    EXPECT_EQ(string_compare_const_str(var2->identifier, "y"), STRING_EQUAL);

    // Nemá další parametry
    EXPECT_EQ(param->next, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectTwoParams);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, TwoFunctions){
    string path = synt_path + "correct_two_fun.zig";
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

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Název funkce je main
    char *comp = string_toConstChar(fun->identifier);
    EXPECT_STREQ(comp, "main");
    free(comp);
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
    
    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectTwoFunctions);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, VarDef){
    string path = synt_path + "correct_var_def.zig";
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

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectVarDef);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStatNoParam){
    string path = synt_path + "correct_fun_call_stat_no_param.zig";
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
    
    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatNoParam);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, NonVoidFun){
    string path = synt_path + "correct_nonvoid_fun.zig";
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

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce vrací int
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_INT);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectNonVoidFun);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStatIntLit){
    string path = synt_path + "correct_fun_call_stat_int_lit.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatIntLit);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserBasicsCorrect, FunCallStatFloatLit){
    string path = synt_path + "correct_fun_call_stat_float_lit.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatFloatLit);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStatStringLit){
    string path = synt_path + "correct_fun_call_stat_string_lit.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatStringLit);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStatVar){
    string path = synt_path + "correct_fun_call_stat_var.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatVar);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStatTwoArgs){
    string path = synt_path + "correct_fun_call_stat_two_args.zig";
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
    // Funkce má dva argumenty
    EXPECT_NE(fcall->arguments, nullptr);
    AST_ArgOrParamNode* arg1 = fcall->arguments;
    // První argument je literál typu float
    EXPECT_EQ(arg1->dataType, AST_DATA_TYPE_NOT_DEFINED);
    EXPECT_NE(arg1->expression, nullptr);
    AST_ExprNode* expr1 = arg1->expression;
    EXPECT_EQ(expr1->exprType, AST_EXPR_LITERAL);
    AST_VarNode* lit1 = (AST_VarNode*)expr1->expression;
    EXPECT_EQ(lit1->literalType, AST_LITERAL_FLOAT);
    EXPECT_EQ(*(double*)lit1->value, 3.14);
    // Druhý argument je literál typu string
    EXPECT_NE(arg1->next, nullptr);
    AST_ArgOrParamNode* arg2 = arg1->next;
    EXPECT_EQ(arg2->dataType, AST_DATA_TYPE_NOT_DEFINED);
    EXPECT_NE(arg2->expression, nullptr);
    AST_ExprNode* expr2 = arg2->expression;
    EXPECT_EQ(expr2->exprType, AST_EXPR_LITERAL);
    AST_VarNode* lit2 = (AST_VarNode*)expr2->expression;
    EXPECT_EQ(lit2->literalType, AST_LITERAL_STRING);
    EXPECT_EQ(string_compare_const_str((DString*)lit2->value, "Please work"), STRING_EQUAL);
    // Nemá další argumenty
    EXPECT_EQ(arg2->next, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatTwoArgs);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStatManyArgs){
    string path = synt_path + "correct_fun_call_stat_many_args.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatManyArgs);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}
*/

TEST(LLParserBasicsCorrect, FunCallStatCommaLast){
    string path = synt_path + "correct_fun_call_stat_comma_last.zig";
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStatCommaLast);

    // Uvolnění alokovaných zdrojů
    frameStack_destroyAll();
    AST_destroyTree();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

/*** Konec souboru parser_test.cpp ***/