/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner_test.cpp                                          *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            9.10.2024                                                 *
 * Poslední změna:   9.10.2024                                                 *
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
#include <gmock/gmock.h>
#include "../src/scanner.h"
#include <string.h>
#include <iostream>

using namespace ::testing;
using namespace std;

TEST(Function, isDigit){
    // ASCII 65 == A, 90 == Z
    for(size_t i = 65; i <= 90; i++){
        // Čísla 65 - 90 reprezentují A - Z
        EXPECT_FALSE(scanner_isDigit(char(i)));
    }

    // ASCII 97 == a, 122 == z
    for(size_t i = 97; i <= 122; i++){
        // Čísla 97 - 122 reprezentují a - z
        EXPECT_FALSE(scanner_isDigit(char(i)));
    }

    // Čísla od 0 do 9 jsou skutečně čísly
    for(size_t i = 0; i < 10; i++){
        // Převedení čísla i do char numC
        char numC = i + '0';
        EXPECT_TRUE(scanner_isDigit(numC));
    }

    // ASCII od 0 - 31 se jedná o Control Characters, ne znaky/čísla
    // ASCII od 32 - 47 se jedná o symboly, ne znaky
    // Dále se tak jedná pro symboly mezi 58 - 64, 91 - 96, ...
    for(size_t i = 0; i <= 47; i++){
        EXPECT_FALSE(scanner_isDigit(i));
    }

    for(size_t i = 58; i <= 64; i++){
        EXPECT_FALSE(scanner_isDigit(i));
    }

    for(size_t i = 91; i <= 96; i++){
        EXPECT_FALSE(scanner_isDigit(i));
    }

    for(size_t i = 123; i <= 127; i++){
        EXPECT_FALSE(scanner_isDigit(i));
    }
}

TEST(Function, isAlpha){
    // ASCII 65 == A, 90 == Z
    for(size_t i = 65; i <= 90; i++){
        // Čísla 65 - 90 reprezentují A - Z
        EXPECT_TRUE(scanner_isAlphanumeric(char(i)));
    }

    // ASCII 97 == a, 122 == z
    for(size_t i = 97; i <= 122; i++){
        // Čísla 97 - 122 reprezentují a - z
        EXPECT_TRUE(scanner_isAlphanumeric(char(i)));
    }

    // Čísla od 0 do 9 nejsou znaky
    for(size_t i = 0; i < 10; i++){
        // Převedení čísla i do char numC
        char numC = i + '0';
        EXPECT_FALSE(scanner_isAlpha(numC));
    }

    // ASCII od 0 - 31 se jedná o Control Characters, ne znaky/čísla
    // ASCII od 32 - 47 se jedná o symboly, ne znaky
    // Dále se tak jedná pro symboly mezi 58 - 64, 91 - 96, ...
    for(size_t i = 0; i <= 47; i++){
        EXPECT_FALSE(scanner_isAlpha(i));
    }

    for(size_t i = 58; i <= 64; i++){
        EXPECT_FALSE(scanner_isAlpha(i));
    }

    for(size_t i = 91; i <= 96; i++){
        EXPECT_FALSE(scanner_isAlpha(i));
    }

    for(size_t i = 123; i <= 127; i++){
        EXPECT_FALSE(scanner_isAlpha(i));
    }
}

TEST(Function, isAlphanumeric){

    // ASCII 65 == A, 90 == Z
    for(size_t i = 65; i <= 90; i++){
        // Čísla 65 - 90 reprezentují A - Z
        EXPECT_TRUE(scanner_isAlphanumeric(char(i)));
    }

    // ASCII 97 == a, 122 == z
    for(size_t i = 97; i <= 122; i++){
        // Čísla 97 - 122 reprezentují a - z
        EXPECT_TRUE(scanner_isAlphanumeric(char(i)));
    }

    // Čísla od 0 do 9 jsou skutečně čísly
    for(size_t i = 0; i < 10; i++){
        // Převedení čísla i do char numC
        char numC = i + '0';
        EXPECT_TRUE(scanner_isAlphanumeric(numC));
    }

                    /*TESTING OF SYMBOLS*/

    // ASCII od 0 - 31 se jedná o Control Characters, ne znaky/čísla
    // ASCII od 32 - 47 se jedná o symboly, ne znaky
    // Dále se tak jedná pro symboly mezi 58 - 64, 91 - 96, ...
    for(size_t i = 0; i <= 47; i++){
        EXPECT_FALSE(scanner_isAlphanumeric(i));
    }

    for(size_t i = 58; i <= 64; i++){
        EXPECT_FALSE(scanner_isAlphanumeric(i));
    }

    for(size_t i = 91; i <= 96; i++){
        EXPECT_FALSE(scanner_isAlphanumeric(i));
    }

    for(size_t i = 123; i <= 127; i++){
        EXPECT_FALSE(scanner_isAlphanumeric(i));
    }
}