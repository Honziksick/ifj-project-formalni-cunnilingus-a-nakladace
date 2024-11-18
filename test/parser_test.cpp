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




/*** Konec souboru parser_test.cpp ***/