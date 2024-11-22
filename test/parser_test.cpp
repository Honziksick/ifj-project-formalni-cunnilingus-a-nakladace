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
#define SPECIFIC_TEST_NAME "CorrectReturn"
#define DISABLE_PRINT

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
string error_path = synt_path + "syntax_error/";

TEST(LLParserBasicsCorrect, PrologueAndEmptyMain) {
    // Načtení souboru s programem na STDIN
    string path = synt_path + "correct_prologue_and_empty_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, OneParam){
    string path = synt_path + "correct_one_param.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, TwoParams){
    string path = synt_path + "correct_two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, TwoFunctions){
    string path = synt_path + "correct_two_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, VarDef){
    string path = synt_path + "correct_var_def.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallNoArgs){
    string path = synt_path + "correct_fun_call_no_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallNoParam);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, NonVoidFun){
    string path = synt_path + "correct_nonvoid_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallIntLit){
    string path = synt_path + "correct_fun_call_int_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallIntLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserBasicsCorrect, FunCallFloatLit){
    string path = synt_path + "correct_fun_call_float_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallFloatLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStringLit){
    string path = synt_path + "correct_fun_call_string_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallStringLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallVar){
    string path = synt_path + "correct_fun_call_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallVar);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallTwoArgs){
    string path = synt_path + "correct_fun_call_two_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
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
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallTwoArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallManyArgs){
    string path = synt_path + "correct_fun_call_many_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallManyArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallCommaLast){
    string path = synt_path + "correct_fun_call_comma_last.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunCallCommaLast);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallNoArgs){
    string path = synt_path + "correct_ifj_fun_call_no_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallIntLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserBasicsCorrect, IFJFunCallIntLit){
    string path = synt_path + "correct_ifj_fun_call_int_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallIntLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserBasicsCorrect, IFJFunCallFloatLit){
    string path = synt_path + "correct_ifj_fun_call_float_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallFloatLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallStringLit){
    string path = synt_path + "correct_ifj_fun_call_string_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallStringLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallVar){
    string path = synt_path + "correct_ifj_fun_call_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallVar);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallTwoArgs){
    string path = synt_path + "correct_ifj_fun_call_two_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallTwoArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallManyArgs){
    string path = synt_path + "correct_ifj_fun_call_many_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallManyArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallCommaLast){
    string path = synt_path + "correct_ifj_fun_call_comma_last.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIFJFunCallCommaLast);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, ExcesiveCurlyBrackets){
    string path = synt_path + "correct_excesive_curly_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExcesiveCurlyBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfjDotEtc){
    string path = synt_path + "X_correct_ifj_dot_etc.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIfjDotEtc);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, AddOperatorsMinus){
    string path = synt_path + "X_correct_add_operators_minus.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectAddOperatorsMinus);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, AddOperatorsPlus){
    string path = synt_path + "X_correct_add_operators_plus.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectAddOperatorsPlus);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, MultiOperatorsAsterisk){
    string path = synt_path + "X_correct_multi_operators_asterisk.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectMultiOperatorsAsterisk);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, MultiOperatorsSlash){
    string path = synt_path + "X_correct_multi_operators_slash.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectMultiOperatorsSlash);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsEE){
    string path = synt_path + "X_correct_rel_operators_EE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectRelOperatorsEE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsG){
    string path = synt_path + "X_correct_rel_operators_G.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectRelOperatorsG);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsGE){
    string path = synt_path + "X_correct_rel_operators_GE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectRelOperatorsGE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsL){
    string path = synt_path + "X_correct_rel_operators_L.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectRelOperatorsL);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsLE){
    string path = synt_path + "X_correct_rel_operators_LE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectRelOperatorsLE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsNE){
    string path = synt_path + "X_correct_rel_operators_NE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectRelOperatorsNE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, DataTypes){
    string path = synt_path + "X_correct_data_types.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectDataTypes);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfNullCondition){
    string path = synt_path + "correct_if_null_condition.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIfNullCondition);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, ThrowAway){
    string path = synt_path + "X_correct_throw_away.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectThrowAway);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunReturn){
    string path = synt_path + "X_correct_fun_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectFunReturn);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfElse){
    string path = synt_path + "X_correct_if_else.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectIfElse);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, While){
    string path = synt_path + "X_correct_while.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectWhile);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprNoBrackets){
    string path = synt_path + "correct_complex_expr_no_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprNoBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithBrackets){
    string path = synt_path + "correct_complex_expr_with_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprWithBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallNoBrackets){
    string path = synt_path + "correct_complex_expr_with_fun_no_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprWithFunCallNoBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserComplexCorrect, ExprFunInBrackets){
    string path = synt_path + "correct_complex_expr_fun_in_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprFunInBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBrackets){
    string path = synt_path + "correct_complex_expr_with_fun_and_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprWithFunCallAndBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish1){
    string path = synt_path + "correct_complex_expr_with_fun_and_brackets_hellish_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprWithFunCallAndBracketsHellish1);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish2){
    string path = synt_path + "correct_complex_expr_with_fun_and_brackets_hellish_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprWithFunCallAndBracketsHellish2);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish3){
    string path = synt_path + "correct_complex_expr_with_fun_and_brackets_hellish_3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectExprWithFunCallAndBracketsHellish3);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, WhileNullCondition){
    string path = synt_path + "X_correct_while_null_condition.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectWhileNullCondition);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, Return){
    string path = synt_path + "correct_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(CorrectReturn);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Example1){
    string path = exam_path + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(Example1);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Example2){
    string path = exam_path + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(Example2);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Example3){
    string path = exam_path + "example3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(Example3);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Fun){
    string path = exam_path + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(Fun);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Hello){
    string path = exam_path + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(Hello);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Multiline){
    string path = exam_path + "multiline.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TREE_AND_FRAMESTACK(Multiline);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(ParserSyntaxError, Prologue) {
    for (int i = 1; i <= 12; i++) {
        string filename = "error_prologue_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = error_path + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << path;
        
        FILE* stdin_backup = stdin;
        stdin = f;
        
        // Syntaktická analýza programu
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
        
        // Očekáváme, že ASTroot bude nullptr, protože kód obsahuje syntaktickou chybu
        ASSERT_EQ(ASTroot, nullptr) << "Syntax error undetected in file: " << filename << endl;
        cerr << COLOR_PINK << "OK: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(ParserSyntaxError, FunctionDefinition) {
    for (int i = 1; i <= 21; i++) {
        string filename = "error_fun_def_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = error_path + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Syntaktická analýza programu
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
        
        // Očekáváme, že ASTroot bude nullptr, protože kód obsahuje syntaktickou chybu
        ASSERT_EQ(ASTroot, nullptr) << "Syntax error undetected in file: " << filename << endl;
        cerr << COLOR_PINK << "OK: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();
        
        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(ParserSyntaxError, Statements) {
    for (int i = 1; i <= 72; ++i) {
        string filename = "error_statement_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = error_path + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;
        
        // Syntaktická analýza programu
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
        
        // Očekáváme, že ASTroot bude nullptr, protože kód obsahuje syntaktickou chybu
        ASSERT_EQ(ASTroot, nullptr) << "Syntax error undetected in file: " << filename << endl;
        cerr << COLOR_PINK << "OK: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}


/*** Konec souboru parser_test.cpp ***/