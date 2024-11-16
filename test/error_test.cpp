/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           error_test.cpp                                            *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            09.10.2024                                                *
 * Poslední změna:   16.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file error_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy funkcí knihovny pro řízení chybových stavů
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `error.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "error.h"
}

using namespace std;
using namespace testing;
using namespace internal;

/**
 * @brief Testuje funkci `error_handle` pro lexikální chybu.
 */
TEST(ErrorHandle, E1_Lexical) {
    EXPECT_EXIT(error_handle(ERROR_LEXICAL), ExitedWithCode(1), "");
}

/**
 * @brief Testuje funkci `error_handle` pro syntaktickou chybu.
 */
TEST(ErrorHandle, E2_Syntactic) {
    EXPECT_EXIT(error_handle(ERROR_SYNTAX), ExitedWithCode(2), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - nedefinovaná funkce nebo proměnná.
 */
TEST(ErrorHandle, E3_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_UNDEF), ExitedWithCode(3), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - nesprávný počet/typ parametrů nebo návratová hodnota.
 */
TEST(ErrorHandle, E4_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_PARAMS_OR_RETVAL), ExitedWithCode(4), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - redefinice nebo přiřazení konstantě.
 */
TEST(ErrorHandle, E5_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_REDEF_OR_CONSTDEF), ExitedWithCode(5), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - chybějící nebo nadbytečný výraz v návratovém příkazu.
 */
TEST(ErrorHandle, E6_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_RETURN_EXP), ExitedWithCode(6), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - nekompatibilita typů.
 */
TEST(ErrorHandle, E7_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_TYPE_COMPATIBILITY), ExitedWithCode(7), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - nelze odvodit typ proměnné.
 */
TEST(ErrorHandle, E8_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_TYPE_INFERENCE), ExitedWithCode(8), "");
}

/**
 * @brief Testuje funkci `error_handle` pro sémantickou chybu - nepoužitá proměnná.
 */
TEST(ErrorHandle, E9_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_UNUSED_VAR), ExitedWithCode(9), "");
}

/**
 * @brief Testuje funkci `error_handle` pro jiné sémantické chyby.
 */
TEST(ErrorHandle, E10_Semantic) {
    EXPECT_EXIT(error_handle(ERROR_SEM_OTHER), ExitedWithCode(10), "");
}

/**
 * @brief Testuje funkci `error_handle` pro interní chybu.
 */
TEST(ErrorHandle, E99_Internal) {
    EXPECT_EXIT(error_handle(ERROR_INTERNAL), ExitedWithCode(99), "");
}

/**
 * @brief Testuje funkci `error_printMessage` pro lexikální chybu.
 */
TEST(PrintErrorMessage, E1_Lexical) {
    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 1) Lexical error: invalid " \
                            "structure of the current lexeme.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro syntaktickou chybu.
 */
TEST(PrintErrorMessage, E2_Syntactic) {
    CaptureStderr();
    error_printMessage(ERROR_SYNTAX);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 2) Syntax error: invalid program " \
                            "syntax, missing header, etc.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - nedefinovaná funkce nebo proměnná.
 */
TEST(PrintErrorMessage, E3_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_UNDEF);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 3) Semantic error: undefined " \
                            "function or variable.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - nesprávný počet/typ parametrů nebo návratová hodnota.
 */
TEST(PrintErrorMessage, E4_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_PARAMS_OR_RETVAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 4) Semantic error: incorrect " \
                            "number/type of parameters in function call; " \
                            "incorrect type or disallowed discard of function "\
                            "return value.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - redefinice nebo přiřazení konstantě.
 */
TEST(PrintErrorMessage, E5_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_REDEF_OR_CONSTDEF);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 5) Semantic error: redefinition " \
                            "of variable or function; assignment to a " \
                            "non-modifiable variable.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - chybějící nebo nadbytečný výraz v návratovém příkazu.
 */
TEST(PrintErrorMessage, E6_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_RETURN_EXP);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 6) Semantic error: missing/excessive " \
                            "expression in function return statement.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - nekompatibilita typů.
 */
TEST(PrintErrorMessage, E7_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_TYPE_COMPATIBILITY);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 7) Semantic error: type incompatibility " \
                            "in arithmetic, string, and relational expressions; " \
                            "incompatible expression type (e.g., in assignment).\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - nelze odvodit typ proměnné.
 */
TEST(PrintErrorMessage, E8_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_TYPE_INFERENCE);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 8) Semantic error: variable type not " \
                            "specified and cannot be inferred from the " \
                            "expression used.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro sémantickou chybu - nepoužitá proměnná.
 */
TEST(PrintErrorMessage, E9_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_UNUSED_VAR);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 9) Semantic error: unused variable " \
                            "within its scope; modifiable variable without " \
                            "the possibility of change after initialization.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro jiné sémantické chyby.
 */
TEST(PrintErrorMessage, E10_Semantic) {
    CaptureStderr();
    error_printMessage(ERROR_SEM_OTHER);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 10) Other semantic errors.\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/**
 * @brief Testuje funkci `error_printMessage` pro interní chybu.
 */
TEST(PrintErrorMessage, E99_Internal) {
    CaptureStderr();
    error_printMessage(ERROR_INTERNAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = RED_COLOR "(Error 99) Internal compiler error: " \
                            "not influenced by the input program (e.g., " \
                            "memory allocation error).\n" RESET_COLOR;
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);

    fprintf(stderr, "Ilustration of error message:\n%s", ExpectedErrMsg.c_str());
    fprintf(stderr, YELLOW_COLOR "In file: <fileName>:<line> (<inFunction>)\n" RESET_COLOR);
}

/*** Konec souboru error_test.cpp ***/