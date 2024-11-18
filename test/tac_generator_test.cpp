/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable_test.cpp                                         *
 * Autor:            Farkašovský Lukáš  <xfarkal00> (hlavní testy)             *
 *                                                                             *
 * Datum:            16.11.2024                                                *
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
#include "ast_nodes.h"
#include "ast_interface.h"
}
#include "ast_test_utils.h"

using namespace testing;
using namespace std;
using namespace testing;
using namespace internal;


/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot;

/**
 * @brief Test kořene, kde je pouze neinicializovaný kořenový uzel.
 */
TEST(TAC_EMPTY, Create_InstrList){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    TAC_freeInstructionList(list);
    free(list);
}

/**
 * @brief Testuje funkci `TAC_createInstruction` pro vytvoření instrukce.
 */
TEST(TAC_EMPTY, Destroy_InstrList){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    TAC_freeInstructionList(list);
    free(list);
}

/**
 * @brief Testuje funkci `TAC_destroyInstruction` pro zničení instrukce.
 */
TEST(TAC_EMPTY, Destroy_Instruction){
    TAC_Instruction *instr = TAC_initInstruction();
    ASSERT_NE(instr, nullptr);

    TAC_destroyInstruction(instr);
    free(instr);
}

/**
 * @brief Testuje funkci `TAC_destroyOperand` pro zničení operandu.
 */
TEST(TAC_EMPTY, Destroy_Operand){
    TAC_Operand *operand = (TAC_Operand *)malloc(sizeof(TAC_Operand));
    ASSERT_NE(operand, nullptr);
    TAC_Operand *operand2 = (TAC_Operand *)malloc(sizeof(TAC_Operand));
    ASSERT_NE(operand, nullptr);

    // Operand má název proměnné kok a je typu string
    operand->value.varName = string_charToDString("kok");
    operand->type = TAC_OPERAND_STRING;

    operand2->value.intValue = 10;
    operand2->type = TAC_OPERAND_INT;

    TAC_destroyOperand(operand);
    TAC_destroyOperand(operand2);

    free(operand);
    free(operand2);
}

/**
 * @brief Testuje funkci `TAC_createInstruction` pro vytvoření instrukce.
 */
TEST(TAC_EMPTY, Create_Instr){
    TAC_Instruction *instr = TAC_initInstruction();
    ASSERT_NE(instr, nullptr);

    free(instr);
}

/**
 * @brief Testuje funkci `TAC_initInstruction` pro inicializaci instrukce.
 */
TEST(TAC_EMPTY, Init){
    TAC_Instruction *instr = TAC_initInstruction();
    ASSERT_NE(instr, nullptr);

    EXPECT_EQ(instr->op, TAC_OP_NONE);
    EXPECT_EQ(instr->dest.type, TAC_OPERAND_NONE);
    EXPECT_EQ(instr->src1.type, TAC_OPERAND_NONE);
    EXPECT_EQ(instr->src2.type, TAC_OPERAND_NONE);
    EXPECT_EQ(instr->next, nullptr);

    free(instr);
}

/**
 * @brief Testuje funkci `TAC_appendInstruction` pro přidání instrukce do seznamu.
 */
TEST(TAC_EMPTY, Append){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    TAC_Instruction *instr = TAC_initInstruction();
    ASSERT_NE(instr, nullptr);

    TAC_appendInstruction(list, instr);

    TAC_freeInstructionList(list);
    free(list);
}

/**
 * @brief Testuje funkci `TAC_appendInstruction` pro přidání více instrukcí.
 */
TEST(TAC_EMPTY, Append_Multiple){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    TAC_Instruction *instr = TAC_initInstruction();
    ASSERT_NE(instr, nullptr);
    TAC_Instruction *instr2 = TAC_initInstruction();
    ASSERT_NE(instr2, nullptr);
    TAC_Instruction *instr3 = TAC_initInstruction();
    ASSERT_NE(instr3, nullptr);
    TAC_Instruction *instr4 = TAC_initInstruction();
    ASSERT_NE(instr4, nullptr);
    TAC_Instruction *instr5 = TAC_initInstruction();
    ASSERT_NE(instr5, nullptr);

    TAC_appendInstruction(list, instr);
    TAC_appendInstruction(list, instr2);
    TAC_appendInstruction(list, instr3);
    TAC_appendInstruction(list, instr4);
    TAC_appendInstruction(list, instr5);

    TAC_destroyInstructionList(list);
}

/**
 * @brief Testuje funkci `TAC_printInstructionList` pro zničení instrukce.
 */
TEST(TAC_EMPTY, Print){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    TAC_printInstructionList(list);

    TAC_freeInstructionList(list);
    free(list);
}

/**
 * @brief Testuje funkci `TAC_generateTestCode` pro vytvoření testovacího kódu.
 */
TEST(TAC, Print){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    TAC_generateTestCode(list);

    TAC_printInstructionList(list);

    TAC_destroyInstructionList(list);
}

/**
 * @brief Testuje funkci `TAC_generateTestCode` pro vytvoření testovacího kódu.
 *

TEST(TAC, Generate_Program){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    AST_ProgramNode *programNode = AST_createProgramNode();
    ASSERT_NE(programNode, nullptr);

    TAC_generateProgramCode(programNode, list);

    TAC_printInstructionList(list);

    TAC_destroyInstructionList(list);
    AST_destroyProgramNode(programNode);
    } */