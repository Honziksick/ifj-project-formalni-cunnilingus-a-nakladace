/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner_test.cpp                                          *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            9.10.2024                                                 *
 * Poslední změna:   14.10.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
*******************************************************************************/
/**
 * @file scanner_test.cpp
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Testování podpůrné knihovny scanner.c.
 * @details Tento soubor obsahuje testy k ověření funkčnosti dané knihovny.
*/

// Potřebné knihovny k testování
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdio.h>
#include <cstdio>
#include <cstring>

extern "C" {
#include "scanner.h"
}

using namespace testing;
using namespace std;
using namespace internal;

/**
 * @brief Testuje funkci `scanner_charIdentity` pro písmena.
 */
TEST(Identity_Letter_Test, Letter) {
    // Krajní hodnoty
    EXPECT_EQ(scanner_charIdentity('A'), LETTER);
    EXPECT_EQ(scanner_charIdentity('Z'), LETTER);
    EXPECT_EQ(scanner_charIdentity('a'), LETTER);
    EXPECT_EQ(scanner_charIdentity('z'), LETTER);
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro čísla.
 */
TEST(Identity_Number_Test, Number) {
    for(int i = 0; i < 10; i++){

        // Převedení čísla i do char numC
        char numC =(char) i + '0';
        EXPECT_EQ(scanner_charIdentity(numC), NUMBER);
    }
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro operátory.
 */
TEST(Identity_Operator_Test, Operator) {
    // Testování znaků, které jsou operátory
    EXPECT_EQ(scanner_charIdentity('+'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('-'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('*'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('/'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('%'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('<'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('>'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('='), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('!'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('&'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('|'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('^'), OPERATOR);
    EXPECT_EQ(scanner_charIdentity('~'), OPERATOR);
    // Tečka je samostatný znak
    EXPECT_NE(scanner_charIdentity('.'), OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro speciální operátor tečku.
 */
TEST(Identity_Operator_Test, Operator_Dot) {
    // Testování znaků, které jsou operátory
    EXPECT_EQ(scanner_charIdentity('.'), DOT);
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro mezeru.
 */
TEST(Identity_Operator_Test, Operator_Space) {
    // Testování znaků, které jsou operátory
    EXPECT_EQ(scanner_charIdentity(' '), EMPTY);
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro zbylé znaky ASCII.
 */
TEST(Identity_Operator_Test, Err) {
    char err = 127;
    for(int i = 0; i < 32; i++) {

        // Převedení čísla i do char numC
        char numC =(char) i;
        EXPECT_EQ(scanner_charIdentity(numC), CHAR_ERROR);
    }
    EXPECT_EQ(scanner_charIdentity(err), CHAR_ERROR);
}

/**
 * @brief Testuje funkci `scanner_getNextChar` pro náhodné pole.
 */
TEST(Get, Get_Next_Char){
    char c[] = "Hello";
    
    for(int i = 0; i < 5; i++){
        // Simulace stdin pomocí ungetc
        ungetc(c[i], stdin);
        char c2 = scanner_getNextChar();
        EXPECT_EQ(c2, c[i]);
    }
}

/**
 * @brief Testuje funkci `scanner_FSM` pro identifikátor hello.
 */
TEST(FSM, FSM_Identifier){
    char c[] = "hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    //for(size_t i = 0; i < strlen(c); i++){printf("%c\n", fgetc(stdin));} //kontrola, co se vypíše
    
    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_IDENTIFIER);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor =hello.
 */
TEST(FSM, FSM_Identifier_ERROR_1){
    char c[] = "=hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor .hello.
 */
TEST(FSM, FSM_Identifier_ERROR_2){
    char c[] = ".hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor 1hello.
 */
TEST(FSM, FSM_Identifier_ERROR_3){
    char c[] = "1hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor 'hello.'.
 */
TEST(FSM, FSM_Identifier_ERROR_4){
    char c[] = "hello.";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro int číslo.
 */
TEST(FSM, FSM_Number_INT){
    char c[] = "42";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_INT);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné int číslo.
 */
TEST(FSM, FSM_Number_INT_ERROR_1){
    char c[] = "42c";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro float číslo.
 */
TEST(FSM, FSM_Number_FLOAT){
    char c[] = "42.42";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_FLOAT);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné float číslo.
 */
TEST(FSM, FSM_Number_FLOAT_ERROR_1){
    char c[] = ".42";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné float číslo.
 */
TEST(FSM, FSM_Number_FLOAT_ERROR_2){
    char c[] = "42.";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné float číslo.
 */
TEST(FSM, FSM_Number_FLOAT_ERROR_3){
    char c[] = "42c.0";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor =.
 */
TEST(FSM, FSM_OPERATOR_1){
    char c[] = "=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor *.
 */
TEST(FSM, FSM_OPERATOR_2){
    char c[] = "*";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor /.
 */
TEST(FSM, FSM_OPERATOR_3){
    char c[] = "/";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor +.
 */
TEST(FSM, FSM_OPERATOR_4){
    char c[] = "+";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor -.
 */
TEST(FSM, FSM_OPERATOR_5){
    char c[] = "-";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor ==.
 */
TEST(FSM, FSM_OPERATOR_6){
    char c[] = "==";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor !=.
 */
TEST(FSM, FSM_OPERATOR_7){
    char c[] = "!=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor <=.
 */
TEST(FSM, FSM_OPERATOR_8){
    char c[] = "<=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor >=.
 */
TEST(FSM, FSM_OPERATOR_9){
    char c[] = ">=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor >.
 */
TEST(FSM, FSM_PERATOR_10){
    char c[] = ">";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor <.
 */
TEST(FSM, FSM_OPERATOR_11){
    char c[] = "<";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_OPERATOR);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný operátor !.
 */
TEST(FSM, FSM_OPERATOR_ERROR_1){
    char c[] = "!";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný operátor ==!.
 */
TEST(FSM, FSM_OPERATOR_ERROR_2){
    char c[] = "==!";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný operátor =>.
 */
TEST(FSM, FSM_OPERATOR_ERROR_3){
    char c[] = "=>";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword if.
 */
TEST(FSM, FSM_KEYWORD_1){
    char c[] = "if";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword else.
 */
TEST(FSM, FSM_KEYWORD_2){
    char c[] = "else";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword const.
 */
TEST(FSM, FSM_KEYWORD_3){
    char c[] = "const";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword fn.
 */
TEST(FSM, FSM_KEYWORD_4){
    char c[] = "fn";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword i32.
 */
TEST(FSM, FSM_KEYWORD_5){
    char c[] = "i32";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword f64.
 */
TEST(FSM, FSM_KEYWORD_6){
    char c[] = "f64";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword null.
 */
TEST(FSM, FSM_KEYWORD_7){
    char c[] = "null";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword pub.
 */
TEST(FSM, FSM_KEYWORD_8){
    char c[] = "pub";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword return.
 */
TEST(FSM, FSM_KEYWORD_9){
    char c[] = "return";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword u8.
 */
TEST(FSM, FSM_KEYWORD_10){
    char c[] = "u8";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword var.
 */
TEST(FSM, FSM_KEYWORD_11){
    char c[] = "var";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword void.
 */
TEST(FSM, FSM_KEYWORD_11){
    char c[] = "void";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro keyword while.
 */
TEST(FSM, FSM_KEYWORD_12){
    char c[] = "while";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_KEYWORD);
}

/*** Konec souboru scanner_test.cpp ***/