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
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PROGRAM, T_CONST), PROGRAM);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PROGRAM, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PROLOGUE>`.
 */
TEST(LLTable, FindRule_PROLOGUE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PROLOGUE, T_CONST), PROLOGUE);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PROLOGUE, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<FUN_DEF_LIST>`.
 */
TEST(LLTable, FindRule_FUN_DEF_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_FUN_DEF_LIST, T_PUB), FUN_DEF_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_FUN_DEF_LIST, T_EOF), FUN_DEF_LIST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_FUN_DEF_LIST, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<FUN_DEF>`.
 */
TEST(LLTable, FindRule_FUN_DEF) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_FUN_DEF, T_PUB), FUN_DEF);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_FUN_DEF, T_FN), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAMETERS>`.
 */

TEST(LLTable, FindRule_PARAMETERS) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAMETERS, T_ID), PARAMETERS_1);
    EXPECT_EQ(LLtable_findRule(NT_PARAMETERS, T_RIGHT_BRACKET), PARAMETERS_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAMETERS, T_LEFT_BRACKET), ExitedWithCode(2), "");
}


/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM_LIST>`.
 */
TEST(LLTable, FindRule_PARAM_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAM_LIST, T_ID), PARAM_LIST);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAM_LIST, T_CONST), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM_LIST_REST>`.
 */
TEST(LLTable, FindRule_PARAM_LIST_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAM_LIST_REST, T_COMMA), PARAM_LIST_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_PARAM_LIST_REST, T_RIGHT_BRACKET), PARAM_LIST_REST_2);


    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAM_LIST_REST, T_LEFT_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM>`.
 */
TEST(LLTable, FindRule_PARAM) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAM, T_ID), PARAM);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAM, T_LEFT_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN_TYPE>`.
 */
TEST(LLTable, FindRule_RETURN_TYPE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_INT), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_INT_OR_NULL), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_FLOAT), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_FLOAT_OR_NULL), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_STRING), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_STRING_OR_NULL), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, T_VOID), RETURN_TYPE_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_TYPE, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<DATA_TYPE>`.
 */
TEST(LLTable, FindRule_DATA_TYPE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, T_INT), DATA_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, T_INT_OR_NULL), DATA_TYPE_2);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, T_FLOAT), DATA_TYPE_3);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, T_FLOAT_OR_NULL), DATA_TYPE_4);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, T_STRING), DATA_TYPE_5);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, T_STRING_OR_NULL), DATA_TYPE_6);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_DATA_TYPE, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT_LIST>`.
 */
TEST(LLTable, FindRule_STATEMENT_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_ID), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_CONST), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_VAR), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_IF), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_WHILE), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_RETURN), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_DUMP), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, T_RIGHT_CURLY_BRACKET), STATEMENT_LIST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_LIST, T_LEFT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_LIST, T_RIGHT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_LIST, T_LEFT_CURLY_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT>`.
 */
TEST(LLTable, FindRule_STATEMENT) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_CONST), STATEMENT_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_VAR), STATEMENT_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_ID), STATEMENT_2);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_DUMP), STATEMENT_3);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_IF), STATEMENT_4);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_WHILE), STATEMENT_5);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, T_RETURN), STATEMENT_6);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT, T_DOT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT, T_COMMA), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT, T_IFJ), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<VAR_DEF>`.
 */
TEST(LLTable, FindRule_VAR_DEF) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_VAR_DEF, T_CONST), VAR_DEF);
    EXPECT_EQ(LLtable_findRule(NT_VAR_DEF, T_VAR), VAR_DEF);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_VAR_DEF, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<MODIFIABLE>`.
 */
TEST(LLTable, FindRule_MODIFIABLE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_MODIFIABLE, T_VAR), MODIFIABLE_1);
    EXPECT_EQ(LLtable_findRule(NT_MODIFIABLE, T_CONST), MODIFIABLE_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_MODIFIABLE, T_PUB), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<POSSIBLE_TYPE>`.
 */
TEST(LLTable, FindRule_POSSIBLE_TYPE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_POSSIBLE_TYPE, T_COLON), POSSIBLE_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_POSSIBLE_TYPE, T_ASSIGNMENT), POSSIBLE_TYPE_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_POSSIBLE_TYPE, T_COMMA), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT_REST>`.
 */
TEST(LLTable, FindRule_STATEMENT_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_REST, T_ASSIGNMENT), STATEMENT_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_REST, T_LEFT_BRACKET), STATEMENT_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_REST, T_IFJ), STATEMENT_REST_3);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_REST, T_RIGHT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_REST, T_IMPORT), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<THROW_AWAY>`.
 */
TEST(LLTable, FindRule_THROW_AWAY) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_THROW_AWAY, T_CALL_PRECEDENCE), THROW_AWAY_1);
    EXPECT_EQ(LLtable_findRule(NT_THROW_AWAY, T_LEFT_BRACKET), THROW_AWAY_2);
    EXPECT_EQ(LLtable_findRule(NT_THROW_AWAY, T_IFJ), THROW_AWAY_3);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_THROW_AWAY, T_RIGHT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_THROW_AWAY, T_IMPORT), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<IF>`.
 */
TEST(LLTable, FindRule_IF) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_IF, T_IF), IF);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_IF, T_ELSE), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<NULL_COND>`.
 */
TEST(LLTable, FindRule_NULL_COND) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_NULL_COND, T_PIPE), NULL_COND_1);
    EXPECT_EQ(LLtable_findRule(NT_NULL_COND, T_LEFT_CURLY_BRACKET), NULL_COND_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_NULL_COND, T_LEFT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_NULL_COND, T_RIGHT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_NULL_COND, T_RIGHT_CURLY_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<SEQUENCE>`.
 */
TEST(LLTable, FindRule_SEQUENCE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_SEQUENCE, T_LEFT_CURLY_BRACKET), SEQUENCE);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_SEQUENCE, T_RIGHT_CURLY_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<WHILE>`.
 */
TEST(LLTable, FindRule_WHILE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_WHILE, T_WHILE), WHILE);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_WHILE, T_LEFT_CURLY_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN>`.
 */
TEST(LLTable, FindRule_RETURN) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_RETURN, T_RETURN), RETURN);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_RETURN, T_SEMICOLON), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN_REST>`.
 */
TEST(LLTable, FindRule_RETURN_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, T_CALL_PRECEDENCE), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, T_SEMICOLON), RETURN_REST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, T_LEFT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, T_RIGHT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, T_RIGHT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, T_COMMA), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, T_IFJ), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, T_ID), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARGUMENTS>`.
 */
TEST(LLTable, FindRule_ARGUMENTS) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, T_CALL_PRECEDENCE), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, T_RIGHT_BRACKET), ARGUMENTS_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_ID), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_IFJ), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_INT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_FLOAT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_STRING), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_INT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_INT_OR_NULL), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_LEFT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_LEFT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_RIGHT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_COMMA), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, T_SEMICOLON), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARG_LIST>`.
 */
TEST(LLTable, FindRule_ARG_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, T_CALL_PRECEDENCE), ARG_LIST);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_ID), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_IFJ), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_LEFT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_INT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_FLOAT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_STRING), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_INT), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_INT_OR_NULL), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_LEFT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_RIGHT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_COMMA), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_SEMICOLON), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, T_RIGHT_BRACKET), ExitedWithCode(2), "");

}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARG>`.
 */
TEST(LLTable, FindRule_ARG) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ARG, T_COMMA), ARG_1);
    EXPECT_EQ(LLtable_findRule(NT_ARG, T_RIGHT_BRACKET), ARG_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ARG, T_LEFT_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, T_LEFT_CURLY_BRACKET), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, T_RIGHT_CURLY_BRACKET), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` pro přijetí neplatného kódu terminálu.
 */
TEST(LLTable, FindRule_Index_Failure) {
    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_SEQUENCE, 68), ExitedWithCode(99), "");
    EXPECT_EXIT(LLtable_findRule(NT_PARAM, 77), ExitedWithCode(99), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, -8), ExitedWithCode(99), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, (LL_TERMINAL_COUNT + 1)), ExitedWithCode(99), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, -1), ExitedWithCode(99), "");
}

/*** Konec souboru lltable_test.cpp ***/