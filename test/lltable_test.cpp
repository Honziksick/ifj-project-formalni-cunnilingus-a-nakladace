/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable_test.cpp                                          *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            07.11.2024                                                *
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
    EXPECT_STREQ(terminalMap[T_ID], "ID");
    EXPECT_STREQ(terminalMap[T_IMPORT], "IMPORT");
    EXPECT_STREQ(terminalMap[T_IFJ], "IFJ");
    EXPECT_STREQ(terminalMap[T_PUB], "PUB");
    EXPECT_STREQ(terminalMap[T_FN], "FN");
    EXPECT_STREQ(terminalMap[T_CONST], "CONST");
    EXPECT_STREQ(terminalMap[T_VAR], "VAR");
    EXPECT_STREQ(terminalMap[T_IF], "IF");
    EXPECT_STREQ(terminalMap[T_ELSE], "ELSE");
    EXPECT_STREQ(terminalMap[T_WHILE], "WHILE");
    EXPECT_STREQ(terminalMap[T_RETURN], "RETURN");
    EXPECT_STREQ(terminalMap[T_ASSIGNMENT], "ASSIGNMENT");
    EXPECT_STREQ(terminalMap[T_PLUS], "PLUS");
    EXPECT_STREQ(terminalMap[T_MINUS], "MINUS");
    EXPECT_STREQ(terminalMap[T_MULTIPLICATION], "MULTIPLICATION");
    EXPECT_STREQ(terminalMap[T_DIVISION], "DIVISION");
    EXPECT_STREQ(terminalMap[T_IDENTITY], "IDENTITY");
    EXPECT_STREQ(terminalMap[T_NOT_EQUAL], "NOT_EQUAL");
    EXPECT_STREQ(terminalMap[T_LESS_THAN], "LESS_THAN");
    EXPECT_STREQ(terminalMap[T_GREATER_THAN], "GREATER_THAN");
    EXPECT_STREQ(terminalMap[T_LESS_THAN_OR_EQUAL], "LESS_THAN_OR_EQUAL");
    EXPECT_STREQ(terminalMap[T_GREATER_THAN_OR_EQUAL], "GREATER_THAN_OR_EQUAL");
    EXPECT_STREQ(terminalMap[T_INT], "INT");
    EXPECT_STREQ(terminalMap[T_INT_OR_NULL], "INT_OR_NULL");
    EXPECT_STREQ(terminalMap[T_FLOAT], "FLOAT");
    EXPECT_STREQ(terminalMap[T_FLOAT_OR_NULL], "FLOAT_OR_NULL");
    EXPECT_STREQ(terminalMap[T_STRING], "STRING");
    EXPECT_STREQ(terminalMap[T_STRING_OR_NULL], "STRING_OR_NULL");
    EXPECT_STREQ(terminalMap[T_VOID], "VOID");
    EXPECT_STREQ(terminalMap[T_INT_LITERAL], "INT_LITERAL");
    EXPECT_STREQ(terminalMap[T_FLOAT_LITERAL], "FLOAT_LITERAL");
    EXPECT_STREQ(terminalMap[T_STRING_LITERAL], "STRING_LITERAL");
    EXPECT_STREQ(terminalMap[T_NULL_LITERAL], "NULL_LITERAL");
    EXPECT_STREQ(terminalMap[T_DOT], "DOT");
    EXPECT_STREQ(terminalMap[T_COMMA], "COMMA");
    EXPECT_STREQ(terminalMap[T_COLON], "COLON");
    EXPECT_STREQ(terminalMap[T_SEMICOLON], "SEMICOLON");
    EXPECT_STREQ(terminalMap[T_PIPE], "PIPE");
    EXPECT_STREQ(terminalMap[T_LEFT_BRACKET], "LEFT_BRACKET");
    EXPECT_STREQ(terminalMap[T_RIGHT_BRACKET], "RIGHT_BRACKET");
    EXPECT_STREQ(terminalMap[T_LEFT_CURLY_BRACKET], "LEFT_CURLY_BRACKET");
    EXPECT_STREQ(terminalMap[T_RIGHT_CURLY_BRACKET], "RIGHT_CURLY_BRACKET");
    EXPECT_STREQ(terminalMap[T_EOF], "EOF");
    EXPECT_STREQ(terminalMap[T_EPSILON], "EPSILON");
}

/**
 * @brief Testovací případ pro korektnost hashovací funkce `LLtable_hashFunction()`.
 */
TEST(LLTable, HashFunction) {
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_ID])), T_HASH_ID);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_IMPORT])), T_HASH_IMPORT);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_IFJ])), T_HASH_IFJ);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_PUB])), T_HASH_PUB);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_FN])), T_HASH_FN);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_CONST])), T_HASH_CONST);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_VAR])), T_HASH_VAR);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_IF])), T_HASH_IF);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_ELSE])), T_HASH_ELSE);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_WHILE])), T_HASH_WHILE);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_RETURN])), T_HASH_RETURN);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_ASSIGNMENT])), T_HASH_ASSIGNMENT);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_PLUS])), T_HASH_PLUS);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_MINUS])), T_HASH_MINUS);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_MULTIPLICATION])), T_HASH_MULTIPLICATION);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_DIVISION])), T_HASH_DIVISION);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_IDENTITY])), T_HASH_IDENTITY);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_NOT_EQUAL])), T_HASH_NOT_EQUAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_LESS_THAN])), T_HASH_LESS_THAN);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_GREATER_THAN])), T_HASH_GREATER_THAN);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_LESS_THAN_OR_EQUAL])), T_HASH_LESS_THAN_OR_EQUAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_GREATER_THAN_OR_EQUAL])), T_HASH_GREATER_THAN_OR_EQUAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_INT])), T_HASH_INT);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_INT_OR_NULL])), T_HASH_INT_OR_NULL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_FLOAT])), T_HASH_FLOAT);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_FLOAT_OR_NULL])), T_HASH_FLOAT_OR_NULL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_STRING])), T_HASH_STRING);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_STRING_OR_NULL])), T_HASH_STRING_OR_NULL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_VOID])), T_HASH_VOID);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_INT_LITERAL])), T_HASH_INT_LITERAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), T_HASH_FLOAT_LITERAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_STRING_LITERAL])), T_HASH_STRING_LITERAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_NULL_LITERAL])), T_HASH_NULL_LITERAL);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_DOT])), T_HASH_DOT);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_COMMA])), T_HASH_COMMA);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_COLON])), T_HASH_COLON);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_SEMICOLON])), T_HASH_SEMICOLON);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_PIPE])), T_HASH_PIPE);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_LEFT_BRACKET])), T_HASH_LEFT_BRACKET);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), T_HASH_RIGHT_BRACKET);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), T_HASH_LEFT_CURLY_BRACKET);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), T_HASH_RIGHT_CURLY_BRACKET);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_EOF])), T_HASH_EOF);
    EXPECT_EQ(LLtable_hashFunction(const_cast<char*>(terminalMap[T_EPSILON])), T_HASH_EPSILON);
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PROGRAM>`.
 */
TEST(LLTable, FindRule_PROGRAM) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PROGRAM, const_cast<char*>(terminalMap[T_CONST])), PROGRAM);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PROGRAM, const_cast<char*>(terminalMap[T_INT_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PROLOGUE>`.
 */
TEST(LLTable, FindRule_PROLOGUE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PROLOGUE, const_cast<char*>(terminalMap[T_CONST])), PROLOGUE);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PROLOGUE, const_cast<char*>(terminalMap[T_INT_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<FUN_DEF_LIST>`.
 */
TEST(LLTable, FindRule_FUN_DEF_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_FUN_DEF_LIST, const_cast<char*>(terminalMap[T_PUB])), FUN_DEF_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_FUN_DEF_LIST, const_cast<char*>(terminalMap[T_EOF])), FUN_DEF_LIST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_FUN_DEF_LIST, const_cast<char*>(terminalMap[T_INT_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<FUN_DEF>`.
 */
TEST(LLTable, FindRule_FUN_DEF) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_FUN_DEF, const_cast<char*>(terminalMap[T_PUB])), FUN_DEF);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_FUN_DEF, const_cast<char*>(terminalMap[T_FN])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAMETERS>`.
 */

TEST(LLTable, FindRule_PARAMETERS) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAMETERS, const_cast<char*>(terminalMap[T_ID])), PARAMETERS_1);
    EXPECT_EQ(LLtable_findRule(NT_PARAMETERS, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), PARAMETERS_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAMETERS, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
}


/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM_LIST>`.
 */
TEST(LLTable, FindRule_PARAM_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAM_LIST, const_cast<char*>(terminalMap[T_ID])), PARAM_LIST);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAM_LIST, const_cast<char*>(terminalMap[T_CONST])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM_LIST_REST>`.
 */
TEST(LLTable, FindRule_PARAM_LIST_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAM_LIST_REST, const_cast<char*>(terminalMap[T_ID])), PARAM_LIST_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_PARAM_LIST_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), PARAM_LIST_REST_2);


    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAM_LIST_REST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<PARAM>`.
 */
TEST(LLTable, FindRule_PARAM) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_PARAM, const_cast<char*>(terminalMap[T_ID])), PARAM);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_PARAM, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN_TYPE>`.
 */
TEST(LLTable, FindRule_RETURN_TYPE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_INT])), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_INT_OR_NULL])), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_FLOAT])), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_FLOAT_OR_NULL])), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_STRING])), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_STRING_OR_NULL])), RETURN_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_VOID])), RETURN_TYPE_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_INT_LITERAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_STRING_LITERAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_TYPE, const_cast<char*>(terminalMap[T_NULL_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<SEQUENCE>`.
 */
TEST(LLTable, FindRule_SEQUENCE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_SEQUENCE, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), SEQUENCE);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_SEQUENCE, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<DATA_TYPE>`.
 */
TEST(LLTable, FindRule_DATA_TYPE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_INT])), DATA_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_INT_OR_NULL])), DATA_TYPE_2);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_FLOAT])), DATA_TYPE_3);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_FLOAT_OR_NULL])), DATA_TYPE_4);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_STRING])), DATA_TYPE_5);
    EXPECT_EQ(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_STRING_OR_NULL])), DATA_TYPE_6);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_INT_LITERAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_STRING_LITERAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_DATA_TYPE, const_cast<char*>(terminalMap[T_NULL_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT_LIST>`.
 */
TEST(LLTable, FindRule_STATEMENT_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_ID])), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_IF])), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_VAR])), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_CONST])), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_WHILE])), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_RETURN])), STATEMENT_LIST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), STATEMENT_LIST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_LIST, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT>`.
 */
TEST(LLTable, FindRule_STATEMENT) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_VAR])), STATEMENT_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_CONST])), STATEMENT_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_ID])), STATEMENT_2);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_IF])), STATEMENT_3);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_WHILE])), STATEMENT_4);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_RETURN])), STATEMENT_5);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT, const_cast<char*>(terminalMap[T_INT_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<STATEMENT_REST>`.
 */
TEST(LLTable, FindRule_STATEMENT_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_REST, const_cast<char*>(terminalMap[T_ASSIGNMENT])), STATEMENT_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_REST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), STATEMENT_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_STATEMENT_REST, const_cast<char*>(terminalMap[T_IFJ])), STATEMENT_REST_3);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_STATEMENT_REST, const_cast<char*>(terminalMap[T_IMPORT])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<VAR_DEF>`.
 */
TEST(LLTable, FindRule_VAR_DEF) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_VAR_DEF, const_cast<char*>(terminalMap[T_VAR])), VAR_DEF);
    EXPECT_EQ(LLtable_findRule(NT_VAR_DEF, const_cast<char*>(terminalMap[T_CONST])), VAR_DEF);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_VAR_DEF, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<MODIFIABLE>`.
 */
TEST(LLTable, FindRule_MODIFIABLE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_MODIFIABLE, const_cast<char*>(terminalMap[T_VAR])), MODIFIABLE_1);
    EXPECT_EQ(LLtable_findRule(NT_MODIFIABLE, const_cast<char*>(terminalMap[T_CONST])), MODIFIABLE_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_MODIFIABLE, const_cast<char*>(terminalMap[T_PUB])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<POSSIBLE_TYPE>`.
 */
TEST(LLTable, FindRule_POSSIBLE_TYPE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_POSSIBLE_TYPE, const_cast<char*>(terminalMap[T_COLON])), POSSIBLE_TYPE_1);
    EXPECT_EQ(LLtable_findRule(NT_POSSIBLE_TYPE, const_cast<char*>(terminalMap[T_ASSIGNMENT])), POSSIBLE_TYPE_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_POSSIBLE_TYPE, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<IF>`.
 */
TEST(LLTable, FindRule_IF) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_IF, const_cast<char*>(terminalMap[T_IF])), IF);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_IF, const_cast<char*>(terminalMap[T_ELSE])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<WHILE>`.
 */
TEST(LLTable, FindRule_WHILE) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_WHILE, const_cast<char*>(terminalMap[T_WHILE])), WHILE);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_WHILE, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<NULL_COND>`.
 */
TEST(LLTable, FindRule_NULL_COND) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_NULL_COND, const_cast<char*>(terminalMap[T_PIPE])), NULL_COND_1);
    EXPECT_EQ(LLtable_findRule(NT_NULL_COND, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), NULL_COND_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_NULL_COND, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_NULL_COND, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_NULL_COND, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN>`.
 */
TEST(LLTable, FindRule_RETURN) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_RETURN, const_cast<char*>(terminalMap[T_RETURN])), RETURN);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_RETURN, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<RETURN_REST>`.
 */
TEST(LLTable, FindRule_RETURN_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_ID])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_IFJ])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_INT_LITERAL])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_STRING_LITERAL])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_NULL_LITERAL])), RETURN_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_SEMICOLON])), RETURN_REST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_RETURN_REST, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<EXPR>`.
 */
TEST(LLTable, FindRule_EXPR) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_ID])), EXPR);
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_IFJ])), EXPR);
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), EXPR);
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_INT_LITERAL])), EXPR);
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), EXPR);
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_STRING_LITERAL])), EXPR);
    EXPECT_EQ(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_NULL_LITERAL])), EXPR);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_INT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_EXPR, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<REL_EXPR>`.
 */
TEST(LLTable, FindRule_REL_EXPR) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_ID])), REL_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_IFJ])), REL_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), REL_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_INT_LITERAL])), REL_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), REL_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_STRING_LITERAL])), REL_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_NULL_LITERAL])), REL_EXPR);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_INT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_REL_EXPR, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<REL_EXPR_REST>`.
 */
TEST(LLTable, FindRule_REL_EXPR_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_IDENTITY])), REL_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_NOT_EQUAL])), REL_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_LESS_THAN])), REL_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_LESS_THAN_OR_EQUAL])), REL_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_GREATER_THAN])), REL_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_GREATER_THAN_OR_EQUAL])), REL_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_COMMA])), REL_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_SEMICOLON])), REL_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), REL_EXPR_REST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_REL_EXPR_REST, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<REL_OP>`.
 */
TEST(LLTable, FindRule_REL_OP) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_IDENTITY])), REL_OP_1);
    EXPECT_EQ(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_NOT_EQUAL])), REL_OP_2);
    EXPECT_EQ(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_LESS_THAN])), REL_OP_3);
    EXPECT_EQ(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_LESS_THAN_OR_EQUAL])), REL_OP_5);
    EXPECT_EQ(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_GREATER_THAN])), REL_OP_4);
    EXPECT_EQ(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_GREATER_THAN_OR_EQUAL])), REL_OP_6);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_PLUS])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_DIVISION])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_REL_OP, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ADD_EXPR>`.
 */
TEST(LLTable, FindRule_ADD_EXPR) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_ID])), ADD_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_IFJ])), ADD_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ADD_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_INT_LITERAL])), ADD_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), ADD_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_STRING_LITERAL])), ADD_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_NULL_LITERAL])), ADD_EXPR);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_FLOAT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_STRING])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_INT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ADD_EXPR_REST>`.
 */
TEST(LLTable, FindRule_ADD_EXPR_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_PLUS])), ADD_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_MINUS])), ADD_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_COMMA])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_SEMICOLON])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_IDENTITY])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_NOT_EQUAL])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_LESS_THAN])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_LESS_THAN_OR_EQUAL])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_GREATER_THAN])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_GREATER_THAN_OR_EQUAL])), ADD_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ADD_EXPR_REST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_COLON])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_EXPR_REST, const_cast<char*>(terminalMap[T_MULTIPLICATION])), ExitedWithCode(2), "");

}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ADD_OP>`.
 */
TEST(LLTable, FindRule_ADD_OP) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ADD_OP, const_cast<char*>(terminalMap[T_PLUS])), ADD_OP_1);
    EXPECT_EQ(LLtable_findRule(NT_ADD_OP, const_cast<char*>(terminalMap[T_MINUS])), ADD_OP_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ADD_OP, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_OP, const_cast<char*>(terminalMap[T_NOT_EQUAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_OP, const_cast<char*>(terminalMap[T_LESS_THAN])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ADD_OP, const_cast<char*>(terminalMap[T_GREATER_THAN])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<MULTI_EXPR>`.
 */
TEST(LLTable, FindRule_MULTI_EXPR) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_ID])), MULTI_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_IFJ])), MULTI_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), MULTI_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_INT_LITERAL])), MULTI_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), MULTI_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_STRING_LITERAL])), MULTI_EXPR);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_NULL_LITERAL])), MULTI_EXPR);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_FLOAT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_STRING])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_INT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<MULTI_EXPR_REST>`.
 */
TEST(LLTable, FindRule_MULTI_EXPR_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_DIVISION])), MULTI_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_MULTIPLICATION])), MULTI_EXPR_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_COMMA])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_SEMICOLON])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_PLUS])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_MINUS])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_IDENTITY])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_NOT_EQUAL])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_LESS_THAN])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_LESS_THAN_OR_EQUAL])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_GREATER_THAN])), MULTI_EXPR_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_GREATER_THAN_OR_EQUAL])), MULTI_EXPR_REST_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_EXPR_REST, const_cast<char*>(terminalMap[T_COLON])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<MULTI_OP>`.
 */
TEST(LLTable, FindRule_MULTI_OP) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_MULTIPLICATION])), MULTI_OP_1);
    EXPECT_EQ(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_DIVISION])), MULTI_OP_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_NOT_EQUAL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_LESS_THAN])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_GREATER_THAN])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_PLUS])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_MULTI_OP, const_cast<char*>(terminalMap[T_MINUS])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<TERM>`.
 */
TEST(LLTable, FindRule_TERM) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_ID])), TERM_1);
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_IFJ])), TERM_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_INT_LITERAL])), TERM_3);
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), TERM_3);
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_STRING_LITERAL])), TERM_3);
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_NULL_LITERAL])), TERM_3);
    EXPECT_EQ(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), TERM_4);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_EOF])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_TERM, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<TERM_REST>`.
 */
TEST(LLTable, FindRule_TERM_REST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), TERM_REST_1);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_PLUS])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_COMMA])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_MINUS])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_DIVISION])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_IDENTITY])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_LESS_THAN])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_NOT_EQUAL])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_SEMICOLON])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_GREATER_THAN_OR_EQUAL])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_GREATER_THAN])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_LESS_THAN_OR_EQUAL])), TERM_REST_2);
    EXPECT_EQ(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_MULTIPLICATION])), TERM_REST_2);


    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_TERM_REST, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARGUMENTS>`.
 */
TEST(LLTable, FindRule_ARGUMENTS) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_ID])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_IFJ])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_INT_LITERAL])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_STRING_LITERAL])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_NULL_LITERAL])), ARGUMENTS_1);
    EXPECT_EQ(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ARGUMENTS_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_FLOAT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_STRING])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_INT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARGUMENTS, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARG_LIST>`.
 */
TEST(LLTable, FindRule_ARG_LIST) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_ID])), ARG_LIST);
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_IFJ])), ARG_LIST);
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ARG_LIST);
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_INT_LITERAL])), ARG_LIST);
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), ARG_LIST);
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_STRING_LITERAL])), ARG_LIST);
    EXPECT_EQ(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_NULL_LITERAL])), ARG_LIST);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_FLOAT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_STRING])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_INT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_COMMA])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_SEMICOLON])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG_LIST, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");

}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<ARG>`.
 */
TEST(LLTable, FindRule_ARG) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_ARG, const_cast<char*>(terminalMap[T_COMMA])), ARG_1);
    EXPECT_EQ(LLtable_findRule(NT_ARG, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ARG_2);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_ARG, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` a neterminál `<LITERAL>`.
 */
TEST(LLTable, FindRule_LITERAL) {
    // Test pro existující pravidlo
    EXPECT_EQ(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_INT_LITERAL])), LITERAL_1);
    EXPECT_EQ(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_FLOAT_LITERAL])), LITERAL_2);
    EXPECT_EQ(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_STRING_LITERAL])), LITERAL_3);
    EXPECT_EQ(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_NULL_LITERAL])), LITERAL_4);

    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_ID])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_IFJ])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_INT])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_FLOAT_OR_NULL])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_LEFT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_RIGHT_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_LEFT_CURLY_BRACKET])), ExitedWithCode(2), "");
    EXPECT_EXIT(LLtable_findRule(NT_LITERAL, const_cast<char*>(terminalMap[T_RIGHT_CURLY_BRACKET])), ExitedWithCode(2), "");
}

/**
 * @brief Testovací případ pro funkci `LLtable_findRule` pro přijetí neplatného hashe.
 */
TEST(LLTable, FindRule_Hash_Failure) {
    // Test pro NExistující pravidlo
    EXPECT_EXIT(LLtable_findRule(NT_TERM, const_cast<char*>("SOMETHING_WENT_VERY_WRONG")), ExitedWithCode(99), "");
    EXPECT_EXIT(LLtable_findRule(NT_PARAM, const_cast<char*>("POINTS_LOST")), ExitedWithCode(99), "");
    EXPECT_EXIT(LLtable_findRule(NT_ARG, const_cast<char*>("PEW_PEW")), ExitedWithCode(99), "");
}

/*** Konec souboru lltable_test.cpp ***/