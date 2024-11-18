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

TEST(Correct, Simplest){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/semantic_examples/simplest.zig", "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Hello){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/hello.zig", "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
}

TEST(Correct, Example1){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/example1.zig", "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
}

TEST(Correct, Example2){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/example2.zig", "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
}

TEST(Correct, Example3){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/example3.zig", "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
}

TEST(Correct, Fun){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/fun.zig", "r");
    EXPECT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    ASTroot = LLparser_parseProgram();
    EXPECT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    frameStack_destroyAll();
    stdin = stdin_backup;
    fclose(f);
}



