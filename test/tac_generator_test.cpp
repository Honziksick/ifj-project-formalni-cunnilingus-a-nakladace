/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable_test.cpp                                         *
 * Autor:            Farkašovský Lukáš  <xfarkal00> (hlavní testy)             *
 *                                                                             *
 * Datum:            16.11.2024                                                *
 * Poslední změna:   25.11.2024                                                *
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

TEST(TAC, generate_example1_statement){
    string path = exam_path + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    semantic_analyseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    AST_FunDefNode *nodeFun = ASTroot->functionList;

    AST_StatementNode *nodeStat = nodeFun->body;
    AST_FunCallNode *funCall = (AST_FunCallNode*)nodeStat->statement;
    AST_ExprNode *thisis = funCall->arguments->expression;
    cerr << thisis<< endl;

    PRINT_AST(AST_PROGRAM_NODE, ASTroot);

    TAC_generateStatementBlock(nodeStat);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(TAC, generate_example1_funDef){
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

    AST_FunDefNode *nodeFun = ASTroot->functionList;

    TAC_generateFunctionDefinition(nodeFun);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(TAC, generate_example1_binary){
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
    stdin = stdin_backup;
    fclose(f);
}

TEST(TAC, generate_example1_if){
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

    // Ukazatel na funkci main
    AST_FunDefNode *nodeFun = ASTroot->functionList;

    // Ukazatel na tělo funkce main --> volání funkce
    AST_StatementNode *nodeStat = nodeFun->body;
    EXPECT_EQ(nodeStat->statementType, AST_STATEMENT_FUN_CALL);

    // Ukazatel na další příkaz v těle funkce main --> definice proměnné
    AST_StatementNode *statement = (AST_StatementNode *)(nodeStat->next);
    EXPECT_EQ(statement->statementType, AST_STATEMENT_VAR_DEF);

    // Další statement v těle funkce main --> if
    statement = (AST_StatementNode *)(statement->next);
    EXPECT_EQ(statement->statementType, AST_STATEMENT_IF);

    AST_IfNode *ifNode = (AST_IfNode *)statement->statement;
    EXPECT_EQ(ifNode->type, AST_IF_NODE);
    EXPECT_NE(ifNode->condition, nullptr);

    TAC_generateIf(ifNode);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(TAC, generate_example1){
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

    TAC_generateProgram();

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(TAC, generate_example2){
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

    TAC_generateProgram();

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}