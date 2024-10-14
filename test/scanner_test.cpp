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

TEST(FSM, Letter){
    char c[] = "Hello";
    for(int i = 0; i < 5; i++){
        // Simulace stdin pomocí ungetc
        ungetc(c[i], stdin);
        Token t = scanner_FSM();
        EXPECT_EQ(t.type, TOKEN_IDENTIFIER);
    }
}

/*** Konec souboru scanner_test.cpp ***/