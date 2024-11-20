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
 */
TEST(TAC, Generate_Program){
    TAC_InstructionList *list = TAC_createInstructionList();
    ASSERT_NE(list, nullptr);

    // Vytvoření kořene stromu
    AST_initTree();
    ASSERT_NE(ASTroot, nullptr);
    AST_VarNode *importedFile = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(importedFile, nullptr);
    ASTroot->importedFile = importedFile;
    ASTroot->importedFile->identifier = string_init();
    ASSERT_NE(ASTroot->importedFile->identifier, nullptr);
    ASSERT_EQ(string_append_char(ASTroot->importedFile->identifier, 'i'), STRING_SUCCESS);

    // Vytvoření funkce main
    AST_FunDefNode *mainFunc = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(mainFunc, nullptr);
    mainFunc->identifier = string_init();
    ASSERT_NE(mainFunc->identifier, nullptr);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'm'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunc->identifier, 'n'), STRING_SUCCESS);
    mainFunc->returnType = AST_DATA_TYPE_INT;

    // Vytvoření příkazu if
    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(ifNode, nullptr);
    ifNode->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(ifNode->condition, nullptr);
    ifNode->condition->exprType = AST_EXPR_BINARY_OP;
    ifNode->condition->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(ifNode->condition->expression, nullptr);
    ((AST_BinOpNode *)ifNode->condition->expression)->op = AST_OP_LESS_THAN;

    // Nastavení levého operandu podmínky if
    AST_ExprNode *leftOperandIf = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandIf, nullptr);
    leftOperandIf->exprType = AST_EXPR_VARIABLE;
    leftOperandIf->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandIf->expression, nullptr);
    ((AST_VarNode *)leftOperandIf->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandIf->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandIf->expression)->identifier, 'a'), STRING_SUCCESS);

    // Nastavení pravého operandu podmínky if
    AST_ExprNode *rightOperand = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperand, nullptr);
    rightOperand->exprType = AST_EXPR_LITERAL;
    rightOperand->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperand->expression, nullptr);
    ((AST_VarNode *)rightOperand->expression)->literalType = AST_LITERAL_INT;
    int *ifValue = (int *)malloc(sizeof(int));
    *ifValue = 10;
    ((AST_VarNode *)rightOperand->expression)->value = ifValue;

    ((AST_BinOpNode *)ifNode->condition->expression)->left = leftOperandIf;
    ((AST_BinOpNode *)ifNode->condition->expression)->right = rightOperand;

    // Vytvoření then větve if
    AST_StatementNode *thenStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(thenStmt, nullptr);
    thenStmt->statementType = AST_STATEMENT_EXPR;
    thenStmt->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(thenStmt->statement, nullptr);
    ((AST_ExprNode *)thenStmt->statement)->exprType = AST_EXPR_BINARY_OP;
    ((AST_ExprNode *)thenStmt->statement)->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(((AST_ExprNode *)thenStmt->statement)->expression, nullptr);
    ((AST_BinOpNode *)((AST_ExprNode *)thenStmt->statement)->expression)->op = AST_OP_ADD;

    // Nastavení operandů pro then větev if
    AST_ExprNode *leftOperandThen = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandThen, nullptr);
    leftOperandThen->exprType = AST_EXPR_VARIABLE;
    leftOperandThen->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandThen->expression, nullptr);
    ((AST_VarNode *)leftOperandThen->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandThen->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandThen->expression)->identifier, 'a'), STRING_SUCCESS);

    
    // Nastavení pravého operandu
    AST_ExprNode *rightOperandIf = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandIf, nullptr);
    rightOperandIf->exprType = AST_EXPR_VARIABLE;
    rightOperandIf->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(rightOperandIf->expression, nullptr);
    ((AST_VarNode *)rightOperandIf->expression)->literalType = AST_LITERAL_INT;
    int *ifValue = (int *)malloc(sizeof(int));
    *ifValue = 10;
    ((AST_VarNode *)rightOperandIf->expression)->value = ifValue;

    AST_ExprNode *rightOperandThen = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandThen, nullptr);
    rightOperandThen->exprType = AST_EXPR_LITERAL;
    rightOperandThen->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandThen->expression, nullptr);
    ((AST_VarNode *)rightOperandThen->expression)->literalType = AST_LITERAL_INT;
    int *thenValue = (int *)malloc(sizeof(int));
    *thenValue = 1;
    ((AST_VarNode *)rightOperandThen->expression)->value = thenValue;

    ((AST_BinOpNode *)((AST_ExprNode *)thenStmt->statement)->expression)->left = leftOperandThen;
    ((AST_BinOpNode *)((AST_ExprNode *)thenStmt->statement)->expression)->right = rightOperandThen;

    // Vytvoření else větve if
    AST_StatementNode *elseStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(elseStmt, nullptr);
    elseStmt->statementType = AST_STATEMENT_EXPR;
    elseStmt->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(elseStmt->statement, nullptr);
    ((AST_ExprNode *)elseStmt->statement)->exprType = AST_EXPR_BINARY_OP;
    ((AST_ExprNode *)elseStmt->statement)->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(((AST_ExprNode *)elseStmt->statement)->expression, nullptr);
    ((AST_BinOpNode *)((AST_ExprNode *)elseStmt->statement)->expression)->op = AST_OP_SUBTRACT;

    // Nastavení operandů pro else větev if
    AST_ExprNode *leftOperandElse = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandElse, nullptr);
    leftOperandElse->exprType = AST_EXPR_VARIABLE;
    leftOperandElse->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandElse->expression, nullptr);
    ((AST_VarNode *)leftOperandElse->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandElse->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandElse->expression)->identifier, 'a'), STRING_SUCCESS);

    AST_ExprNode *rightOperandElse = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandElse, nullptr);
    rightOperandElse->exprType = AST_EXPR_LITERAL;
    rightOperandElse->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandElse->expression, nullptr);
    ((AST_VarNode *)rightOperandElse->expression)->literalType = AST_LITERAL_INT;
    int *elseValue = (int *)malloc(sizeof(int));
    *elseValue = 1;
    ((AST_VarNode *)rightOperandElse->expression)->value = elseValue;

    ((AST_BinOpNode *)((AST_ExprNode *)elseStmt->statement)->expression)->left = leftOperandElse;
    ((AST_BinOpNode *)((AST_ExprNode *)elseStmt->statement)->expression)->right = rightOperandElse;

    ifNode->thenBranch = thenStmt;
    ifNode->elseBranch = elseStmt;

    // Vytvoření příkazu while
    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(whileNode, nullptr);
    whileNode->condition = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(whileNode->condition, nullptr);
    whileNode->condition->exprType = AST_EXPR_BINARY_OP;
    whileNode->condition->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(whileNode->condition->expression, nullptr);
    ((AST_BinOpNode *)whileNode->condition->expression)->op = AST_OP_GREATER_THAN;

    // Nastavení operandů pro podmínku while
    AST_ExprNode *leftOperandWhile = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandWhile, nullptr);
    leftOperandWhile->exprType = AST_EXPR_VARIABLE;
    leftOperandWhile->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandWhile->expression, nullptr);
    ((AST_VarNode *)leftOperandWhile->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandWhile->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandWhile->expression)->identifier, 'b'), STRING_SUCCESS);

    AST_ExprNode *rightOperandWhile = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandWhile, nullptr);
    rightOperandWhile->exprType = AST_EXPR_LITERAL;
    rightOperandWhile->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandWhile->expression, nullptr);
    ((AST_VarNode *)rightOperandWhile->expression)->literalType = AST_LITERAL_FLOAT;
    double *whileValue = (double *)malloc(sizeof(double));
    *whileValue = 0.0;
    ((AST_VarNode *)rightOperandWhile->expression)->value = whileValue;

    ((AST_BinOpNode *)whileNode->condition->expression)->left = leftOperandWhile;
    ((AST_BinOpNode *)whileNode->condition->expression)->right = rightOperandWhile;

    // Vytvoření těla while
    AST_StatementNode *whileBody = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(whileBody, nullptr);
    whileBody->statementType = AST_STATEMENT_EXPR;
    whileBody->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(whileBody->statement, nullptr);
    ((AST_ExprNode *)whileBody->statement)->exprType = AST_EXPR_BINARY_OP;
    ((AST_ExprNode *)whileBody->statement)->expression = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(((AST_ExprNode *)whileBody->statement)->expression, nullptr);
    ((AST_BinOpNode *)((AST_ExprNode *)whileBody->statement)->expression)->op = AST_OP_SUBTRACT;

    // Nastavení operandů pro tělo while
    AST_ExprNode *leftOperandWhileBody = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(leftOperandWhileBody, nullptr);
    leftOperandWhileBody->exprType = AST_EXPR_VARIABLE;
    leftOperandWhileBody->expression = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(leftOperandWhileBody->expression, nullptr);
    ((AST_VarNode *)leftOperandWhileBody->expression)->identifier = string_init();
    ASSERT_NE(((AST_VarNode *)leftOperandWhileBody->expression)->identifier, nullptr);
    ASSERT_EQ(string_append_char(((AST_VarNode *)leftOperandWhileBody->expression)->identifier, 'b'), STRING_SUCCESS);

    AST_ExprNode *rightOperandWhileBody = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(rightOperandWhileBody, nullptr);
    rightOperandWhileBody->exprType = AST_EXPR_LITERAL;
    rightOperandWhileBody->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(rightOperandWhileBody->expression, nullptr);
    ((AST_VarNode *)rightOperandWhileBody->expression)->literalType = AST_LITERAL_FLOAT;
    double *whileBodyValue = (double *)malloc(sizeof(double));
    *whileBodyValue = 0.1;
    ((AST_VarNode *)rightOperandWhileBody->expression)->value = whileBodyValue;

    ((AST_BinOpNode *)((AST_ExprNode *)whileBody->statement)->expression)->left = leftOperandWhileBody;
    ((AST_BinOpNode *)((AST_ExprNode *)whileBody->statement)->expression)->right = rightOperandWhileBody;

    whileNode->body = whileBody;

    // Vytvoření příkazu return
    AST_StatementNode *returnStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(returnStmt, nullptr);
    returnStmt->statementType = AST_STATEMENT_RETURN;
    returnStmt->statement = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(returnStmt->statement, nullptr);
    ((AST_ExprNode *)returnStmt->statement)->exprType = AST_EXPR_LITERAL;
    ((AST_ExprNode *)returnStmt->statement)->expression = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(((AST_ExprNode *)returnStmt->statement)->expression, nullptr);
    ((AST_VarNode *)((AST_ExprNode *)returnStmt->statement)->expression)->literalType = AST_LITERAL_INT;
    int *returnValue = (int *)malloc(sizeof(int));
    *returnValue = 0;
    ((AST_VarNode *)((AST_ExprNode *)returnStmt->statement)->expression)->value = returnValue;

    // Vytvoření těla funkce main
    AST_StatementNode *mainBody = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(mainBody, nullptr);
    mainBody->statementType = AST_STATEMENT_IF;
    mainBody->statement = ifNode;
    mainBody->next = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(mainBody->next, nullptr);
    mainBody->next->statementType = AST_STATEMENT_WHILE;
    mainBody->next->statement = whileNode;
    mainBody->next->next = returnStmt;

    mainFunc->body = mainBody;

    // Přidání funkce main do kořene
    ASTroot->functionList = mainFunc;

    // Ověření struktury stromu
    ASSERT_EQ(ASTroot->functionList, mainFunc);
    ASSERT_EQ(ASTroot->functionList->body, mainBody);
    ASSERT_EQ(((AST_StatementNode *)ASTroot->functionList->body)->statement, ifNode);
    ASSERT_EQ(((AST_StatementNode *)ASTroot->functionList->body)->next->statement, whileNode);
    ASSERT_EQ(((AST_StatementNode *)ASTroot->functionList->body)->next->next, returnStmt);


    TAC_generateProgramCodeBegin(ASTroot, list);

    // Projdeme všechny funkce a vygenerujeme kód pro každou z nich
    AST_FunDefNode *current = ASTroot->functionList; // Use a separate pointer for iteration
    while(current != nullptr){

        TAC_generateFunctionDefinitionBegin(current, list);
        TAC_generateFunctionDefinitionEnd(list);

        current = current->next;
    }

    TAC_generateProgramCodeEnd(list);

    TAC_printInstructionList(list);
    TAC_destroyInstructionList(list);

    // Uvolnění stromu
    AST_destroyTree();
} 