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
#include "semantic_analyser.h"
}

using namespace std;
using namespace testing;
using namespace internal;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot;