/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ifj24_compiler_integration_test.cpp                       *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            26.11.2024                                                *
 * Poslední změna:   29.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ifj24_compiler_integration_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy sémantické analýzy
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `semantic_analyser.c`. Testy jsou vytvořené za využití 
 *          frameworku Google Test. Testy vychází z funkce lexkíláního
 *          analyzátoru a testují nejen sémantickou analýzu, ale i vytváření
 *          abstraktního syntaktického stromu.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Budeme tisknout strom i obsah zásobníku rámců
#define PRINT_AST_OUT 1
//#define PRINT_FRAME_STACK_OUT 1

// Definice názvu specifického testu, pro který se má provádět tisk
/* Pozn. Pokud není definováno, tisk se provádí pro všechny testy */
#define SPECIFIC_TEST_NAME "CorrectReturn"

// Kompletně vypneme výpis
//#define DISABLE_PRINT

#include "ifj24_compiler_test_utils.h"
extern "C" {
#include "ifj24_compiler.h"
}

/*******************************************************************************
 *                                                                             *
 *                    UŽIVATELSKÉ TESTOVACÍ FUNKCE A MAKRA                     *
 *                                                                             *
 ******************************************************************************/

extern "C" {
    void mock_main() {
        // Spustíme parsování zdrojového souboru a vygeneruje AST
        LLparser_parseProgram();

        // Přestože by toto nemělo nikdy nastat, raději zkontrolujeme, že kořen AST je platný
        if(ASTroot != NULL) {
            // Spustíme sémantickou analýzu nad AST pomocí Symtable
            semantic_analyseProgram();

            // Přesměrujeme STDOUT do černé díry
            freopen("/dev/null", "w", stdout);

            // Spustíme generování mezikódu IFJ24code
            TAC_generateProgram();

            // Obnovíme původní STDOUT
            freopen("/dev/tty", "w", stdout);
        }
        // Pokud tento neočekávaný případ nastal, hlásíme interní chybu
        else {
            error_handle(ERROR_INTERNAL);
        }

        // Před úspěšným dokončením překladu uvolníme všechnu alokovanou paměť
        IFJ24Compiler_freeAllAllocatedMemory();

        // Překlad skončil úspěchem
        exit(0);
    }
}

TEST(LLParserBasicsCorrect, PrologueAndEmptyMain) {
    // Načtení souboru s programem na STDIN
    string path = syntPath + "correct_prologue_and_empty_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, OneParam){
    string path = syntPath + "correct_one_param.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, TwoParams){
    string path = syntPath + "correct_two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, TwoFunctions){
    string path = syntPath + "correct_two_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, VarDef){
    string path = syntPath + "correct_var_def.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallNoArgs){
    string path = syntPath + "correct_fun_call_no_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, NonVoidFun){
    string path = syntPath + "correct_nonvoid_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallIntLit){
    string path = syntPath + "correct_fun_call_int_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallFloatLit){
    string path = syntPath + "correct_fun_call_float_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallStringLit){
    string path = syntPath + "correct_fun_call_string_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallVar){
    string path = syntPath + "correct_fun_call_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallTwoArgs){
    string path = syntPath + "correct_fun_call_two_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallManyArgs){
    string path = syntPath + "correct_fun_call_many_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunCallCommaLast){
    string path = syntPath + "correct_fun_call_comma_last.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallNoArgs){
    string path = syntPath + "correct_ifj_fun_call_no_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserBasicsCorrect, IFJFunCallIntLit){
    string path = syntPath + "correct_ifj_fun_call_int_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserBasicsCorrect, IFJFunCallFloatLit){
    string path = syntPath + "correct_ifj_fun_call_float_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallStringLit){
    string path = syntPath + "correct_ifj_fun_call_string_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallVar){
    string path = syntPath + "correct_ifj_fun_call_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallTwoArgs){
    string path = syntPath + "correct_ifj_fun_call_two_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallManyArgs){
    string path = syntPath + "correct_ifj_fun_call_many_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IFJFunCallCommaLast){
    string path = syntPath + "correct_ifj_fun_call_comma_last.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfjDotEtc){
    string path = syntPath + "X_correct_ifj_dot_etc.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, AddOperatorsMinus){
    string path = syntPath + "X_correct_add_operators_minus.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, AddOperatorsPlus){
    string path = syntPath + "X_correct_add_operators_plus.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, MultiOperatorsAsterisk){
    string path = syntPath + "X_correct_multi_operators_asterisk.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, MultiOperatorsSlash){
    string path = syntPath + "X_correct_multi_operators_slash.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsEE){
    string path = syntPath + "X_correct_rel_operators_EE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsG){
    string path = syntPath + "X_correct_rel_operators_G.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsGE){
    string path = syntPath + "X_correct_rel_operators_GE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsL){
    string path = syntPath + "X_correct_rel_operators_L.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsLE){
    string path = syntPath + "X_correct_rel_operators_LE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, RelOperatorsNE){
    string path = syntPath + "X_correct_rel_operators_NE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, DataTypes){
    string path = syntPath + "X_correct_data_types.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfNullCondition){
    string path = syntPath + "correct_if_null_condition.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(7), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, ThrowAway){
    string path = syntPath + "X_correct_throw_away.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, FunReturn){
    string path = syntPath + "X_correct_fun_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfElse){
    string path = syntPath + "X_correct_if_else.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, While){
    string path = syntPath + "X_correct_while.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprNoBrackets){
    string path = syntPath + "correct_complex_expr_no_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithBrackets){
    string path = syntPath + "correct_complex_expr_with_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallNoBrackets){
    string path = syntPath + "correct_complex_expr_with_fun_no_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(LLParserComplexCorrect, ExprFunInBrackets){
    string path = syntPath + "correct_complex_expr_fun_in_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBrackets){
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish1){
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish2){
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish3){
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, WhileNullCondition){
    string path = syntPath + "X_correct_while_null_condition.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(7), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, Return){
    string path = syntPath + "correct_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserBasicsCorrect, IfCondition) {
    for (int i = 1; i <= 7; i++) {
        string filename = "correct_if_condition_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(mock_main(), ExitedWithCode(7), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(LLParserExamples, Example1){
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Example2){
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Example3){
    string path = examPath + "example3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Fun){
    string path = examPath + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Hello){
    string path = examPath + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(LLParserExamples, Multiline){
    string path = examPath + "multiline.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(ParserSyntaxError, Prologue) {
    for (int i = 1; i <= 12; i++) {
        string filename = "error_prologue_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntErrorPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << path;
        
        FILE* stdin_backup = stdin;
        stdin = f;
        
        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(mock_main(), ExitedWithCode(2), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(ParserSyntaxError, FunctionDefinition) {
    for (int i = 1; i <= 21; i++) {
        string filename = "error_fun_def_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntErrorPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(mock_main(), ExitedWithCode(2), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(ParserSyntaxError, Statements) {
    for (int i = 1; i <= 118; i++) {
        string filename = "error_statement_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntErrorPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(mock_main(), ExitedWithCode(2), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
        
        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(Correct, Simplest){
    string path = semPath + "simplest.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Hello){
    string path = examPath + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example1){
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");
    
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example2){
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example3){
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Fun){
    string path = examPath + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Implicit_to_float){
    string path = semPath + "implicit_to_float.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Implicit_to_int){
    string path = semPath + "implicit_to_int.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Pseudo){
    string path = semPath + "pseudo.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Pseudo2){
    string path = semPath + "pseudo2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Inference){
    string path = semPath + "type_inference_good.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Correct, Void_fun){
    string path = semPath + "void_fun_good.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Void_fun2){
    string path = semPath + "void_fun_good2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Undefined_Var){
    string path = semPath + "semen_test_1_undefined_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Undefined_Fun){
    string path = semPath + "semen_test_2_undefined_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ParamCount){
    string path = semPath + "semen_test_3_wrong_number_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ParamType){
    string path = semPath + "semen_test_4_wrong_type_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return){
    string path = semPath + "semen_test_5_incompatible_return_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return2){
    string path = semPath + "semen_test_6_missing_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(6), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return3){
    string path = semPath + "semen_test_7_missing_return_completely.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(6), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Var){
    string path = semPath + "semen_test_8_redefining_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    EXPECT_EXIT(mock_main(), ExitedWithCode(5), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Fun){
    string path = semPath + "semen_test_9_redefining_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(5), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ConstAssign){
    string path = semPath + "semen_test_10_assigning_value_to_const.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(5), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType){
    string path = semPath + "semen_test_11_uncompatible_assignment_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(7), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType2){
    string path = semPath + "semen_test_12_uncompatible_assignment_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(7), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_Var){
    string path = semPath + "semen_test_13_unused_variable.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(9), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, MainInt){
    string path = semPath + "main_type_int.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, NoMain){
    string path = semPath + "missing_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Prolog){
    string path = semPath + "prolog_issue.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
 
    EXPECT_EXIT(mock_main(), ExitedWithCode(2), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Prolog2){
    string path = semPath + "prolog_issue2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(2), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Inference){
    string path = semPath + "type_inference.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(8), "");
    
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Inference2){
    string path = semPath + "type_inference2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(8), "");
    
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unchanged_var){
    string path = semPath + "unchanged_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(9), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_value){
    string path = semPath + "unused_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(2), "");
    
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_value2){
    string path = semPath + "unused_value2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Void_fun){
    string path = semPath + "void_fun_bad.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    EXPECT_EXIT(mock_main(), ExitedWithCode(8), "");

    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, CantDetermineNullCondType){
    string filename = "cant_determine_nullcond_type.zig";
    string path = semPath + filename;

    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;

    FILE* stdin_backup = stdin;
    stdin = f;

    // Sémantická analýza by měl skončit chybou
    EXPECT_EXIT(mock_main(), ExitedWithCode(8), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(Correct, TypeCasting){
    string filename = "more_advanced_type_casting.zig";
    string path = semPath + filename;

    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;

    FILE* stdin_backup = stdin;
    stdin = f;

    // Sémantická analýza by měl skončit chybou
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}


TEST(SemanticError, Error3_UndefinedVariable){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_3_undef_var_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error3_UndefinedFunction){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_3_undef_fun_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error3_UndefinedIFJFunction){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_3_undef_ifj_fun_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(3), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_WrongNumberOfFunParams){
    for (int i = 1; i <= 10; i++) {
        string filename = "error_4_wrong_num_of_fun_params_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_WrongParamType){
    for (int i = 1; i <= 22; i++) {
        string filename = "error_4_wrong_param_type_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_WrongFunReturnType){
    for (int i = 1; i <= 3; i++) {
        string filename = "error_4_wrong_fun_return_type_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_ReturnValueDump){
    for (int i = 1; i <= 2; i++) {
        string filename = "error_4_return_value_dump_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error5_VariableRedefinition){
    for (int i = 1; i <= 10; i++) {
        string filename = "error_5_variable_redefinition_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(5), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(SemanticError, Error6_ReturnExp){
    for (int i = 1; i <= 21; i++) {
        string filename = "error_6_return_exp_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(6), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

TEST(Lex, EscapeAndHexadecimal) {
    string path = lexPath + "escape_and_hexa.zig";

    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Syntaktická analýza programu
    EXPECT_EXIT(mock_main(), ExitedWithCode(0), "");

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

TEST(Parser, Boundries) {
    for (int i = 1; i <= 2; i++) {
        string filename = "boundries_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(mock_main(), ExitedWithCode(10), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
       // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}
