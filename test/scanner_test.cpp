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

extern "C" {
#include "scanner.h"
}

using namespace testing;
using namespace std;
using namespace internal;

/**
 * @brief Testuje funkci `scanner_charIdentity` pro písmena.
 */
TEST(Identity, Letter) {
    EXPECT_EQ(scanner_charIdentity('A'), LETTER);
    EXPECT_EQ(scanner_charIdentity('Z'), LETTER);
    EXPECT_EQ(scanner_charIdentity('a'), LETTER);
    EXPECT_EQ(scanner_charIdentity('z'), LETTER);
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro čísla.
 */
TEST(Identity, Number) {
    for(size_t i = 0; i < 10; i++){

        // Převedení čísla i do char numC
        char numC = i + '0';
        EXPECT_EQ(scanner_charIdentity(numC), NUMBER);
    }
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro operátory.
 */
TEST(Identity, Operator) {
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
}