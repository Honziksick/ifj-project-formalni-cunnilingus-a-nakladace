/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable_test.cpp                                         *
 * Autor:            Farkašovský Lukáš  <xfarkal00> (hlavní testy)             *
 *                                                                             *
 * Datum:            16.11.2024                                                *
 * Poslední změna:   25.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file tac_generator_test.cpp
 * @author Farkašovský Lukáš  \<xfarkal00>
 *
 * @brief Testy funkcí knihovny s implementací generátoru tříadresného kódu.
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `tac_generator.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "tac_generator.h"
}
#include "ifj24_compiler_test_utils.h"

using namespace testing;
using namespace std;
using namespace testing;
using namespace internal;

TEST(TAC, Print_Test_Code) {
    TAC_generateTestCode();
}

TEST(TAC, Generate_Expression) {
    string path = synt_path + "correct_two_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    // Inicializace zásobníku rámců
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Má funkci
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Název funkce je main
    char *comp = string_toConstChar(fun->identifier);
    EXPECT_STREQ(comp, "main");
    free(comp);
    // Vrací void
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    // Nemá parametry
    EXPECT_EQ(fun->parameters, nullptr);
    // Má ukazatel na další funkci
    EXPECT_NE(fun->next, nullptr);

    fun = fun->next;
    // Druhá funkce
    // Funkce nemá tělo
    EXPECT_EQ(fun->body, nullptr);
    // Má identifikátor
    EXPECT_NE(fun->identifier, nullptr);
    // Vrací void
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    // Nemá parametry
    EXPECT_EQ(fun->parameters, nullptr);
    // Nemá na další funkci
    EXPECT_EQ(fun->next, nullptr);
    
    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectTwoFunctions);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}