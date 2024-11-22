/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable_test.cpp                                          *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            07.11.2024                                                *
 * Poslední změna:   10.11.2024                                                *
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
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PROGRAM>`.
 */
TEST(LLTable, FindRule_PROGRAM) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_CONST, NT_PROGRAM, &rule));
    EXPECT_EQ(rule, PROGRAM);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_PROGRAM, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PROLOGUE>`.
 */
TEST(LLTable, FindRule_PROLOGUE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_CONST, NT_PROLOGUE, &rule));
    EXPECT_EQ(rule, PROLOGUE);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_PROLOGUE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<FUN_DEF_LIST>`.
 */
TEST(LLTable, FindRule_FUN_DEF_LIST) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_PUB, NT_FUN_DEF_LIST, &rule));
    EXPECT_EQ(rule, FUN_DEF_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_EOF, NT_FUN_DEF_LIST, &rule));
    EXPECT_EQ(rule, FUN_DEF_LIST_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_FUN_DEF_LIST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<FUN_DEF>`.
 */
TEST(LLTable, FindRule_FUN_DEF) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_PUB, NT_FUN_DEF, &rule));
    EXPECT_EQ(rule, FUN_DEF);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_FN, NT_FUN_DEF, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAMETERS>`.
 */

TEST(LLTable, FindRule_PARAMETERS) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_ID, NT_PARAMETERS, &rule));
    EXPECT_EQ(rule, PARAMETERS_1);
    EXPECT_TRUE(LLtable_findRule(T_RIGHT_BRACKET, NT_PARAMETERS, &rule));
    EXPECT_EQ(rule, PARAMETERS_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_LEFT_BRACKET, NT_PARAMETERS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}


/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM_LIST>`.
 */
TEST(LLTable, FindRule_PARAM_LIST) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_ID, NT_PARAM_LIST, &rule));
    EXPECT_EQ(rule, PARAM_LIST);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_CONST, NT_PARAM_LIST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}
/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM_LIST_REST>`.
 */
TEST(LLTable, FindRule_PARAM_LIST_REST) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_COMMA, NT_PARAM_LIST_REST, &rule));
    EXPECT_EQ(rule, PARAM_LIST_REST_1);
    EXPECT_TRUE(LLtable_findRule(T_RIGHT_BRACKET, NT_PARAM_LIST_REST, &rule));
    EXPECT_EQ(rule, PARAM_LIST_REST_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_LEFT_BRACKET, NT_PARAM_LIST_REST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM>`.
 */
TEST(LLTable, FindRule_PARAM) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_ID, NT_PARAM, &rule));
    EXPECT_EQ(rule, PARAM);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_LEFT_BRACKET, NT_PARAM, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN_TYPE>`.
 */
TEST(LLTable, FindRule_RETURN_TYPE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_INT, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_INT_OR_NULL, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_FLOAT, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_FLOAT_OR_NULL, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_STRING, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_STRING_OR_NULL, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_VOID, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, RETURN_TYPE_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_RETURN_TYPE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<DATA_TYPE>`.
 */
TEST(LLTable, FindRule_DATA_TYPE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_INT, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, DATA_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_INT_OR_NULL, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, DATA_TYPE_2);
    EXPECT_TRUE(LLtable_findRule(T_FLOAT, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, DATA_TYPE_3);
    EXPECT_TRUE(LLtable_findRule(T_FLOAT_OR_NULL, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, DATA_TYPE_4);
    EXPECT_TRUE(LLtable_findRule(T_STRING, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, DATA_TYPE_5);
    EXPECT_TRUE(LLtable_findRule(T_STRING_OR_NULL, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, DATA_TYPE_6);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_DATA_TYPE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT_LIST>`.
 */
TEST(LLTable, FindRule_STATEMENT_LIST) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_ID, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_CONST, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_VAR, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_IF, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_WHILE, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_RETURN, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_DUMP, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_1);
    EXPECT_TRUE(LLtable_findRule(T_RIGHT_CURLY_BRACKET, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, STATEMENT_LIST_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_LEFT_BRACKET, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_BRACKET, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_LEFT_CURLY_BRACKET, NT_STATEMENT_LIST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT>`.
 */
TEST(LLTable, FindRule_STATEMENT) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_CONST, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_1);
    EXPECT_TRUE(LLtable_findRule(T_VAR, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_1);
    EXPECT_TRUE(LLtable_findRule(T_ID, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_2);
    EXPECT_TRUE(LLtable_findRule(T_DUMP, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_3);
    EXPECT_TRUE(LLtable_findRule(T_IF, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_4);
    EXPECT_TRUE(LLtable_findRule(T_WHILE, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_5);
    EXPECT_TRUE(LLtable_findRule(T_RETURN, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_6);
    EXPECT_TRUE(LLtable_findRule(T_IFJ, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, STATEMENT_7);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_DOT, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_COMMA, NT_STATEMENT, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<VAR_DEF>`.
 */
TEST(LLTable, FindRule_VAR_DEF) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_CONST, NT_VAR_DEF, &rule));
    EXPECT_EQ(rule, VAR_DEF);
    EXPECT_TRUE(LLtable_findRule(T_VAR, NT_VAR_DEF, &rule));
    EXPECT_EQ(rule, VAR_DEF);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_VAR_DEF, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<MODIFIABLE>`.
 */
TEST(LLTable, FindRule_MODIFIABLE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_VAR, NT_MODIFIABLE, &rule));
    EXPECT_EQ(rule, MODIFIABLE_1);
    EXPECT_TRUE(LLtable_findRule(T_CONST, NT_MODIFIABLE, &rule));
    EXPECT_EQ(rule, MODIFIABLE_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_PUB, NT_MODIFIABLE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<POSSIBLE_TYPE>`.
 */
TEST(LLTable, FindRule_POSSIBLE_TYPE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_COLON, NT_POSSIBLE_TYPE, &rule));
    EXPECT_EQ(rule, POSSIBLE_TYPE_1);
    EXPECT_TRUE(LLtable_findRule(T_ASSIGNMENT, NT_POSSIBLE_TYPE, &rule));
    EXPECT_EQ(rule, POSSIBLE_TYPE_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_COMMA, NT_POSSIBLE_TYPE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT_REST>`.
 */
TEST(LLTable, FindRule_STATEMENT_REST) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_ASSIGNMENT, NT_STATEMENT_REST, &rule));
    EXPECT_EQ(rule, STATEMENT_REST_1);
    EXPECT_TRUE(LLtable_findRule(T_LEFT_BRACKET, NT_STATEMENT_REST, &rule));
    EXPECT_EQ(rule, STATEMENT_REST_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_IFJ, NT_STATEMENT_REST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_BRACKET, NT_STATEMENT_REST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_IMPORT, NT_STATEMENT_REST, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<THROW_AWAY>`.
 */
TEST(LLTable, FindRule_THROW_AWAY) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_CALL_PRECEDENCE, NT_THROW_AWAY, &rule));
    EXPECT_EQ(rule, THROW_AWAY);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_BRACKET, NT_THROW_AWAY, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_IMPORT, NT_THROW_AWAY, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<IF>`.
 */
TEST(LLTable, FindRule_IF) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_IF, NT_IF, &rule));
    EXPECT_EQ(rule, IF);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ELSE, NT_IF, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<NULL_COND>`.
 */
TEST(LLTable, FindRule_NULL_COND) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_PIPE, NT_NULL_COND, &rule));
    EXPECT_EQ(rule, NULL_COND_1);
    EXPECT_TRUE(LLtable_findRule(T_LEFT_CURLY_BRACKET, NT_NULL_COND, &rule));
    EXPECT_EQ(rule, NULL_COND_2);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_LEFT_BRACKET, NT_NULL_COND, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_BRACKET, NT_NULL_COND, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_CURLY_BRACKET, NT_NULL_COND, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<SEQUENCE>`.
 */
TEST(LLTable, FindRule_SEQUENCE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_LEFT_CURLY_BRACKET, NT_SEQUENCE, &rule));
    EXPECT_EQ(rule, SEQUENCE);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_CURLY_BRACKET, NT_SEQUENCE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<WHILE>`.
 */
TEST(LLTable, FindRule_WHILE) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_WHILE, NT_WHILE, &rule));
    EXPECT_EQ(rule, WHILE);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_LEFT_CURLY_BRACKET, NT_WHILE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARGUMENTS>`.
 */
TEST(LLTable, FindRule_ARGUMENTS) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro existující pravidlo
    EXPECT_TRUE(LLtable_findRule(T_CALL_PRECEDENCE, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, ARGUMENTS);

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule(T_ID, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_IFJ, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_INT, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_FLOAT, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_STRING, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_INT_OR_NULL, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_BRACKET, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_LEFT_BRACKET, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_LEFT_CURLY_BRACKET, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_RIGHT_CURLY_BRACKET, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_COMMA, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule(T_SEMICOLON, NT_ARGUMENTS, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}


/**
 * @brief Testovací případ pro funkci `LLtable_findRule` pro přijetí neplatného kódu terminálu.
 */
TEST(LLTable, FindRule_Index_Failure) {
    // Deklarace proměnné pro uložení získaného pravidla
    LLRuleSet rule = RULE_UNDEFINED;

    // Test pro neexistující pravidlo
    EXPECT_FALSE(LLtable_findRule((LLTerminals)-8, NT_PROGRAM, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
    EXPECT_FALSE(LLtable_findRule((LLTerminals)-1, NT_SEQUENCE, &rule));
    EXPECT_EQ(rule, SYNTAX_ERROR);
}
/*** Konec souboru lltable_test.cpp ***/