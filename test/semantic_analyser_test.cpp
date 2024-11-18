/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.cpp                                     *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            17.11.2024                                                *
 * Poslední změna:   17.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file semantic_analyser.cpp
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Testy sémantické analýzy
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `semantic_analyser.c`. Testy jsou vytvořené za využití 
 *          frameworku Google Test. Testy vychází z funkce lexkíláního
 *          analyzátoru a testují nejen sémantickou analýzu, ale i vytváření
 *          abstraktního syntaktického stromu.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "semantic_analyser.h"
#include "scanner.h"
#include "parser.h"
#include "llparser.h"
#include "precedence_parser.h"
#include "lltable.h"
#include "precedence_table.h"
#include "precedence_stack.h"
}

using namespace std;
using namespace testing;
using namespace internal;

void TestSemantic(){
    semantic_analyseProgram();
    exit(0);
}

void TestParser(){
    ASTroot = LLparser_parseProgram();
    exit(0);
}

std::string exam_path = "../ifj24_examples/ifj24_programs/";
std::string sem_path = "../test/test_examples/semantic_examples/";

TEST(Correct, Simplest){
    std::string path = sem_path + "simplest.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    Parser_getNextToken(true);
    LLparser_parseProgram();

    //EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    EXPECT_NE(ASTroot, nullptr);

    //Test pro AST
    EXPECT_EQ(ASTroot->type, AST_PROGRAM_NODE);
    EXPECT_STREQ(string_toConstChar(ASTroot->importedFile), "ifj24.zig");
    ASSERT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    EXPECT_EQ(fun->body, nullptr);
    EXPECT_STREQ(string_toConstChar(fun->identifier), "main");
    EXPECT_EQ(fun->next, nullptr);
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    EXPECT_EQ(fun->parameters, nullptr);



    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}
/*
TEST(Correct, Hello){
    std::string path = exam_path + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example1){
    std::string path = exam_path + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example2){
    std::string path = exam_path + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
ASTroot = NULL;
}

TEST(Correct, Example3){
    std::string path = exam_path + "example3.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
ASTroot = NULL;
}

TEST(Correct, Fun){
    std::string path = exam_path + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
ASTroot = NULL;
}

TEST(Incorrect, Undefined_Var){
    std::string path = sem_path + "semen_test_1_undefined_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(3), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
ASTroot = NULL;
}

TEST(Incorrect, Undefined_Fun){
    std::string path = sem_path + "semen_test_2_undefined_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, ParamCount){
    std::string path = sem_path + "semen_test_3_wrong_number_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(0), "");
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ParamType){
    std::string path = sem_path + "semen_test_4_wrong_type_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
        ASTroot = NULL;
}

TEST(Incorrect, Return){
    std::string path = sem_path + "semen_test_5_incompatible_return_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return2){
    std::string path = sem_path + "semen_test_6_missing_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return3){
    std::string path = sem_path + "semen_test_7_missing_return_completely.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Var){
    std::string path = sem_path + "semen_test_8_redefining_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(5), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Fun){
    std::string path = sem_path + "semen_test_9_redefining_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(TestParser(), ExitedWithCode(5), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ConstAssign){
    std::string path = sem_path + "semen_test_10_assigning_value_to_const.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(5), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType){
    std::string path = sem_path + "semen_test_11_uncompatible_assignment_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(7), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType2){
    std::string path = sem_path + "semen_test_12_uncompatible_assignment_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(7), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_Var){
    std::string path = sem_path + "semen_test_13_unused_variable.zig";
    FILE* f = fopen(path.c_str(), "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(9), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}
*/
