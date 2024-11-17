/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           dynamic_string_test.cpp                                   *
 * Autor:            David Krejčí   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            15.10.2024                                                *
 * Poslední změna:   15.10.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file dynamic_string_test.cpp
 * @author David Krejčí \<xkrejcd00>
 *
 * @brief Testy funkcí knihovny pro dynamický řetězec
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `dynamic_string.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "dynamic_string.h"
}

using namespace std;
using namespace testing;
using namespace internal;

/**
 * @brief Testuje funkci `string_init` pro inicializaci řetězce
 */
TEST(StringInit, Init) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    ASSERT_EQ(str->length, (size_t)0);
    ASSERT_EQ(str->allocatedSize, (size_t)STRING_INIT_SIZE);

    free(str->str);
    free(str);
}

/**
 * @brief Testuje funkci `string_free` pro uvolnění řetězce
 */
TEST(StringFree, Free) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    string_free(str);
}

/**
 * @brief Testuje funkci `string_append_char` pro přidání jednoho znaku
 */
TEST(StringAppendChar, OneChar) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);

    ASSERT_EQ(str->length, (size_t)1);
    ASSERT_EQ(str->str[0], 'a');

    string_free(str);
}

/**
 * @brief Testuje funkci `string_append_char` pro přidání do NULL
 */
TEST(StringAppendChar, NullString) {
    ASSERT_EQ(string_append_char(nullptr, 'a'), STRING_RESIZE_FAIL);
}

/**
 * @brief Testuje funkci `string_append_char` pro přidání více znaků
 */
TEST(StringAppendChar, MoreChars) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);
    const char *strConst = "abc";

    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(str, strConst[i]), STRING_SUCCESS);
    }

    ASSERT_EQ(str->length, (size_t)strlen(strConst));
    
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(str->str[i], strConst[i]);
    }

    string_free(str);
}

/**
 * @brief Testuje funkci `string_append_char` pro přidání všech znaků ASCII
 */
TEST(StringAppendChar, AllChars) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    for (int i = 0; i < 128; i++) {
        ASSERT_EQ(string_append_char(str, (char)i), STRING_SUCCESS);
    }

    ASSERT_EQ(str->length, (size_t)128);

    for (int i = 0; i < 128; i++) {
        ASSERT_EQ(str->str[i], (char)i);
    }

    string_free(str);
}

/**
 * @brief Testuje funkci `string_append_char` pro přidání 4000 znaků
 */
TEST(StringAppendChar, ManyChars) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    for (int i = 0; i < 4000; i++) {
        ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);
    }

    ASSERT_EQ(str->length, (size_t)4000);
    ASSERT_EQ(str->str[3999], 'a');

    string_free(str);
}

/**
 * @brief Testuje funkci `string_copy` pro kopírování řetězce
 */
TEST(StringCopy, BasicCopy) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_copy(str1, str2), STRING_SUCCESS);

    ASSERT_EQ(str1->length, str2->length);
    for(size_t i = 0; i < str1->length; i++) {
        ASSERT_EQ(str1->str[i], str2->str[i]);
    }

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_copy` pro z NULL ukazatele
 */
TEST(StringCopy, NullCopiedString) {
    DString *strTo = string_init();
    ASSERT_NE(strTo, nullptr);

    ASSERT_EQ(string_copy(nullptr, strTo), STRING_COPY_FAIL);

    string_free(strTo);
}

/**
 * @brief Testuje funkci `string_copy` pro do NULL ukazatele
 */
TEST(StringCopy, NullToString) {
    DString *strCopied = string_init();
    ASSERT_NE(strCopied, nullptr);

    ASSERT_EQ(string_copy(strCopied, nullptr), STRING_COPY_FAIL);

    string_free(strCopied);
}

/**
 * @brief Testuje funkci `string_copy` pro kopírování prázdného řetězce
 */
TEST(StringCopy, EmptyString) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    ASSERT_EQ(string_copy(str1, str2), STRING_SUCCESS);

    ASSERT_EQ(str1->length, str2->length);

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_copy` pro kopírování řetězce 2000 znaků
 */
TEST(StringCopy, ManyChars) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    }
    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str1, 'b'), STRING_SUCCESS);
    }
    

    ASSERT_EQ(string_copy(str1, str2), STRING_SUCCESS);

    ASSERT_EQ(str1->length, str2->length);
    for(size_t i = 0; i < str1->length; i++) {
        ASSERT_EQ(str1->str[i], str2->str[i]);
    }

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_copy` pro kopírování do neprázdného řetězce
 */
TEST(StringCopy, NotEmptyString) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_append_char(str2, 'd'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'e'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'f'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'h'), STRING_SUCCESS);

    ASSERT_EQ(string_copy(str1, str2), STRING_SUCCESS);

    ASSERT_EQ(str1->length, str2->length);
    for(size_t i = 0; i < str1->length; i++) {
        ASSERT_EQ(str1->str[i], str2->str[i]);
    }

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání dvou stejných řetězců
 */
TEST(StringCompare, EqualStrings) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_append_char(str2, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_compare(str1, str2), STRING_EQUAL);

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání jednoho řetězce se sebou
 */
TEST(StringCompare, SameString) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);

    ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str1, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_compare(str1, str1), STRING_EQUAL);

    string_free(str1);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání dvou různých řetězců
 */
TEST(StringCompare, DifferentStrings) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);

    ASSERT_EQ(string_append_char(str2, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str2, 'd'), STRING_SUCCESS);

    ASSERT_EQ(string_compare(str1, str2), STRING_NOT_EQUAL);

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání prázdného řetězce
 */
TEST(StringCompare, EmptyString) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    ASSERT_EQ(string_compare(str1, str2), STRING_EQUAL);

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání NULL řetězce
 */
TEST(StringCompare, NullString) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);

    ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);

    ASSERT_EQ(string_compare(str1, nullptr), STRING_NOT_EQUAL);

    string_free(str1);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání dlouhých stejných řetězců
 */
TEST(StringCompare, LongEqualStrings) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    }
    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str2, 'a'), STRING_SUCCESS);
    }

    ASSERT_EQ(string_compare(str1, str2), STRING_EQUAL);

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_compare` pro porovnání dlouhých různých řetězců
 */
TEST(StringCompare, LongDifferentStrings) {
    DString *str1 = string_init();
    ASSERT_NE(str1, nullptr);
    DString *str2 = string_init();
    ASSERT_NE(str2, nullptr);

    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str1, 'a'), STRING_SUCCESS);
    }
    for (int i = 0; i < 999; i++) {
        ASSERT_EQ(string_append_char(str2, 'a'), STRING_SUCCESS);
    }
    ASSERT_EQ(string_append_char(str2, 'b'), STRING_SUCCESS);

    ASSERT_EQ(string_compare(str1, str2), STRING_NOT_EQUAL);

    string_free(str1);
    string_free(str2);
}

/**
 * @brief Testuje funkci `string_compare_const_str` pro porovnání řetězce s ekvivalentím konstantním řetězcem
 */
TEST(StringCompareConstStr, EqualStrings) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_compare_const_str(str, "abc"), STRING_EQUAL);

    string_free(str);
}

/**
 * @brief Testuje funkci `string_compare_const_str` pro porovnání řetězce s jiným konstantním řetězcem
 */
TEST(StringCompareConstStr, DifferentStrings) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'c'), STRING_SUCCESS);

    ASSERT_EQ(string_compare_const_str(str, "ab"), STRING_NOT_EQUAL);

    string_free(str);
}

/**
 * @brief Testuje funkci `string_compare_const_str` pro porovnání prázdného řetězce
 */
TEST(StringCompareConstStr, EmptyString) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    ASSERT_EQ(string_compare_const_str(str, ""), STRING_EQUAL);

    string_free(str);
}

/**
 * @brief Testuje funkci `string_compare_const_str` pro porovnání NULL řetězce
 */
TEST(StringCompareConstStr, NullString) {

    ASSERT_EQ(string_compare_const_str(nullptr, "abc"), STRING_NOT_EQUAL);
}

/**
 * @brief Testuje funkci `string_compare_const_str` pro porovnání řetězce s konstantním řetězcem o délce 2000 znaků
 */
TEST(StringCompareConstStr, LongEqualStrings) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);
    }
    for (int i = 0; i < 1000; i++) {
        ASSERT_EQ(string_append_char(str, 'b'), STRING_SUCCESS);
    }

    char *str2 = (char *)malloc(2001*sizeof(char));
    str2[2000] = '\0';
    for (int i = 0; i < 1000; i++) {
        str2[i] = 'a';
    }
    for (int i = 1000; i < 2000; i++) {
        str2[i] = 'b';
    }
    const char *strConst = (const char *)str2;

    ASSERT_EQ(string_compare_const_str(str, strConst), STRING_EQUAL);

    string_free(str);
    free(str2);
}

/**
 * @brief Testuje funkci `string_toConstStr` pro prázdný řetězec
 */
TEST(StringToConstStr, EmptyString) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    char *strConst = string_toConstChar(str);
    EXPECT_EQ(strConst, nullptr);

    string_free(str);
    free(strConst);
}

/**
 * @brief Testuje funkci `string_toConstStr` pro řetězec jednoho znaku
 */
TEST(StringToConstStr, OneCharString) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);
    ASSERT_EQ(string_append_char(str, 'x'), STRING_SUCCESS);

    char *strConst = string_toConstChar(str);
    ASSERT_NE(strConst, nullptr);
    ASSERT_STREQ("x", strConst);
    ASSERT_EQ(strlen(str->str), strlen(strConst));

    string_free(str);
    free(strConst);
}

/**
 * @brief Testuje funkci `string_toConstStr` pro dlouhý řetězec , který se rozšířil
 */
TEST(StringToConstStr, LongString) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);
    ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'c'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'd'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'e'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'f'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'g'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'h'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'j'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'k'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'm'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'n'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'o'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'p'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'q'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'r'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 's'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 't'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'u'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'v'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'w'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'x'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'y'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'z'), STRING_SUCCESS);

    char *strConst = string_toConstChar(str);
    ASSERT_NE(strConst, nullptr);
    ASSERT_STREQ("abcdefghijklmnopqrstuvwxyz", strConst);
    ASSERT_EQ(strlen(str->str), strlen(strConst));

    string_free(str);
    free(strConst);
}

TEST(ConstStrToDString, ShortString){
    DString *str = string_charToDString("abc");
    ASSERT_NE(str, nullptr);

    ASSERT_EQ(str->length, 3ULL);
    ASSERT_EQ(str->allocatedSize, 4ULL);
    ASSERT_EQ(string_append_char(str, 'd'), STRING_SUCCESS);

    ASSERT_EQ(str->length, 4ULL);
    ASSERT_GT(str->allocatedSize, 4ULL);

    string_free(str);
}

/*** Konec souboru dynamic_string_test.cpp ***/
