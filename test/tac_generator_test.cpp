/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable_test.cpp                                         *
 * Autor:            Farkašovský Lukáš  <xfarkal00> (hlavní testy)             *
 *                                                                             *
 * Datum:            16.11.2024                                                *
 * Poslední změna:   29.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file tac_generator_test.cpp
 * @author Farkašovský Lukáš  \<xfarkal00>
 *
 * @brief Testy funkcí knihovny s implementací generátoru tříadresného kódu.
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `tac_generator.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "tac_generator.h"
}
#define PRINT_AST_OUT true
#include "ifj24_compiler_test_utils.h"



using namespace testing;
using namespace std;
using namespace testing;
using namespace internal;


TEST(TAC, generate_example1_funDef) {
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    AST_FunDefNode *nodeFun = ASTroot->functionList;

    TAC_generateFunctionDefinition(nodeFun);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}

TEST(TAC, generate_example1_binary) {
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Ukazatel na funkci main
    AST_FunDefNode *nodeFun = ASTroot->functionList;

    // Ukazatel na tělo funkce main --> volání funkce
    AST_StatementNode *nodeStat = nodeFun->body;
    EXPECT_EQ(nodeStat->statementType, AST_STATEMENT_FUN_CALL);

    // Ukazatel na další příkaz v těle funkce main --> definice proměnné
    AST_StatementNode *statement = (AST_StatementNode *)(nodeStat->next);
    EXPECT_EQ(statement->statementType, AST_STATEMENT_VAR_DEF);

    // Ukazatel na výraz v definici proměnné
    AST_ExprNode *nodeExpr = (AST_ExprNode *)(statement->statement);
    EXPECT_EQ(nodeExpr->exprType, AST_EXPR_BINARY_OP);

    TAC_generateBinaryOperator((AST_BinOpNode *)nodeExpr->expression);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}

TEST(TAC, generate_example1) {
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    TAC_generateProgram();

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}


TEST(TAC, generate_example2) {
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    TAC_generateProgram();

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}


TEST(TAC, ray) {
    string path = semPath + "test_raytrace.ifj";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    EXPECT_EXIT(TAC_generateProgram(), ::testing::ExitedWithCode(0), "");

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}

