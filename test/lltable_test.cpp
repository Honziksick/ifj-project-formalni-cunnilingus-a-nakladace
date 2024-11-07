/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable_test.cpp                                          *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            07.11.2024                                                *
 * Poslední změna:   07.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file lltable_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy funkcí knihovny pro řízení chybových stavů
 * @details Tento zdrojový soubor obsahuje unit testy pravidel obsažených v
 *          LL-tabulce v souboru `lltable.c`. Testy jsou vytvořené za využití 
 *          frameworku Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "lltable.h"
}

using namespace std;
using namespace testing;
using namespace internal;

/**
 * @brief Testovací případ na kontrolu správné indexace v mapovací tabulce terminálů.
 */
TEST(LLTable, TerminalMap) {
    EXPECT_STREQ(terminalMap[T_ID], "id");
    EXPECT_STREQ(terminalMap[T_IMPORT], "@import");
    EXPECT_STREQ(terminalMap[T_IFJ], "ifj");
    EXPECT_STREQ(terminalMap[T_PUB], "pub");
    EXPECT_STREQ(terminalMap[T_FN], "fn");
    EXPECT_STREQ(terminalMap[T_CONST], "const");
    EXPECT_STREQ(terminalMap[T_VAR], "var");
    EXPECT_STREQ(terminalMap[T_IF], "if");
    EXPECT_STREQ(terminalMap[T_ELSE], "else");
    EXPECT_STREQ(terminalMap[T_WHILE], "while");
    EXPECT_STREQ(terminalMap[T_RETURN], "return");
    EXPECT_STREQ(terminalMap[T_ASSIGNMENT], "=");
    EXPECT_STREQ(terminalMap[T_PLUS], "+");
    EXPECT_STREQ(terminalMap[T_MINUS], "-");
    EXPECT_STREQ(terminalMap[T_MULTIPLICATION], "*");
    EXPECT_STREQ(terminalMap[T_DIVISION], "/");
    EXPECT_STREQ(terminalMap[T_IDENTITY], "==");
    EXPECT_STREQ(terminalMap[T_NOT_EQUAL], "!=");
    EXPECT_STREQ(terminalMap[T_LESS_THAN], "<");
    EXPECT_STREQ(terminalMap[T_GREATER_THAN], ">");
    EXPECT_STREQ(terminalMap[T_LESS_THAN_OR_EQUAL], "<=");
    EXPECT_STREQ(terminalMap[T_GREATER_THAN_OR_EQUAL], ">=");
    EXPECT_STREQ(terminalMap[T_INT], "i32");
    EXPECT_STREQ(terminalMap[T_INT_OR_NULL], "?i32");
    EXPECT_STREQ(terminalMap[T_FLOAT], "f64");
    EXPECT_STREQ(terminalMap[T_FLOAT_OR_NULL], "?f64");
    EXPECT_STREQ(terminalMap[T_STRING], "[]u8");
    EXPECT_STREQ(terminalMap[T_STRING_OR_NULL], "?[]u8");
    EXPECT_STREQ(terminalMap[T_VOID], "void");
    EXPECT_STREQ(terminalMap[T_INT_LITERAL], "INT_LITERAL");
    EXPECT_STREQ(terminalMap[T_FLOAT_LITERAL], "FLOAT_LITERAL");
    EXPECT_STREQ(terminalMap[T_STRING_LITERAL], "STRING_LITERAL");
    EXPECT_STREQ(terminalMap[T_NULL_LITERAL], "NULL_LITERAL");
    EXPECT_STREQ(terminalMap[T_DOT], ".");
    EXPECT_STREQ(terminalMap[T_COMMA], ",");
    EXPECT_STREQ(terminalMap[T_COLON], ":");
    EXPECT_STREQ(terminalMap[T_SEMICOLON], ";");
    EXPECT_STREQ(terminalMap[T_PIPE], "|");
    EXPECT_STREQ(terminalMap[T_LEFT_BRACKET], "(");
    EXPECT_STREQ(terminalMap[T_RIGHT_BRACKET], ")");
    EXPECT_STREQ(terminalMap[T_LEFT_CURLY_BRACKET], "{");
    EXPECT_STREQ(terminalMap[T_RIGHT_CURLY_BRACKET], "}");
    EXPECT_STREQ(terminalMap[T_EOF], "EOF");
    EXPECT_STREQ(terminalMap[T_EPSILON], "EPSILON");
}


/**
 * @brief Testovací případ pro hashovací funkci `LLtable_hashFunction()`.
 */
TEST(LLtableTest, HashFunction) {
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_FN])), T_HASH_FN);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_ID])), T_HASH_ID);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_IF])), T_HASH_IF);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_CONST])), T_HASH_CONST);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_COMMA])), T_HASH_COMMA);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_MULTIPLICATION])), T_HASH_MULTIPLICATION);
}

/**
 * @brief Testovací případ pro funkci LLtable_findRule.
 */
/*
TEST(LLtableTest, FindRule) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(4, "id"), PARAMETERS_1);
    EXPECT_EQ(LLtable_findRule(10, "id"), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(11, "id"), STATEMENT_2);

    // Test pro neexistující pravidlo
    EXPECT_ANY_THROW(LLtable_findRule(0, "id"));
    EXPECT_ANY_THROW(LLtable_findRule(1, "id"));
    EXPECT_ANY_THROW(LLtable_findRule(2, "id"));

    // Test pro neexistující klíč
    EXPECT_ANY_THROW(LLtable_findRule(0, "nonexistent"));
}
*/



/*
// Test úspěšného nalezení pravidla pro 'var'
TEST(LLTableTest, FindRuleSuccess_Var) {
    int nt = NT_MODIFIABLE; 
    char str[] = "var";

    int rule = LLtable_findRule(nt, str);
    EXPECT_EQ(rule, MODIFIABLE_1);
}

// Test syntaktické chyby pro neznámý token
TEST(LLTableTest, FindRuleSyntaxError_InvalidToken) {
    int nt = NT_PROGRAM; 
    char str[] = "invalid_token";
    int rule = -42;
    EXPECT_EXIT(rule = LLtable_findRule(nt, str), ExitedWithCode(1), "");
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

// Test úspěšného nalezení pravidla pro 'if'
TEST(LLTableTest, FindRuleSuccess_If) {
    int nt = NT_IF; 
    char str[] = "if";

    int rule = LLtable_findRule(nt, str);
    EXPECT_EQ(rule, IF);
}

// Přidání dalších testů pro různé neterminály a vstupní řetězce
TEST(LLTableTest, FindRuleSuccess_Return) {
    int nt = NT_RETURN;
    char str[] = "return";

    int rule = LLtable_findRule(nt, str);
    EXPECT_EQ(rule, RETURN);
}

// Test pro neterminál 'LITERAL' s literálem 'i32_literal'
TEST(LLTableTest, FindRuleSuccess_Literal) {
    int nt = NT_TERM;
    char str[] = "i32_literal";

    int rule = LLtable_findRule(nt, str);
    EXPECT_EQ(rule, TERM_3);
}
*/
/*** Konec souboru lltable_test.cpp ***/