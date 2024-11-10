/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_test.cpp                                              *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            08.11.2024                                                *
 * Poslední změna:   08.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ast_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy stavby abstraktního syntaktického stromu AST.
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `ast_interface.c`. Testy jsou vytvořené za využití 
 *          frameworku Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "ast_nodes.h"
#include "ast_interface.h"
}
#include "ast_test_utils.h"

using namespace std;
using namespace testing;
using namespace internal;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot;

/**
 * @brief Test stromu, kde je pouze neinicializovaný kořenový uzel.
 */
TEST(ASTPrintTest, AST_OnlyRoot) {
    // Vytvoření kořenového uzlu stromu
    AST_createTree();

    // Získání vytvořeného stromu
    string output = ASTutils_printCapturedOutput(ASTroot);

    // Referenční strom
    string reference =
        "Program Node\n"
        "├── Imported File: (null)\n"
        "└── Function List: (null)\n";

    // Porovnání získaného stromu s referenčním a jejich výpis
    EXPECT_STREQ(output.c_str(), reference.c_str());
    ASTutils_printDiff(output, reference);

    // Uvolnění paměti celého stromu
    AST_destroyTree();
}

/**
 * @brief Test stromu, kde je pouze inicializovaný kořenový uzel.
 */
TEST(ASTPrintTest, AST_InicializedRoot) {
    // Vytvoření kořenového uzlu stromu
    AST_createTree();

    // Inicializace stromu
    DString *dynStr = string_init();
    ASSERT_NE(dynStr, nullptr);
    ASSERT_EQ(string_append_char(dynStr, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(dynStr, 'f'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(dynStr, 'j'), STRING_SUCCESS);
    ASTroot->importedFile = dynStr;

    // Získání vytvořeného stromu
    string output = ASTutils_printCapturedOutput(ASTroot);

    // Referenční strom
    string reference =
        "Program Node\n"
        "├── Imported File: ifj\n"
        "└── Function List: (null)\n";

    // Porovnání získaného stromu s referenčním a jejich výpis
    EXPECT_STREQ(output.c_str(), reference.c_str());
    ASTutils_printDiff(output, reference);

    // Uvolnění paměti celého stromu
    AST_destroyTree();
}

/**
 * @brief Test stromu, kde je pouze kořenový uzel.
 * @note Referenční program v jazyce C:
 * ```
 *    const ifj = @import("ifj24.zig");
 * 
 *    pub fn main() void {
 *        var a: i32 = 5;
 *        var b: i64 = 3.14;
 *        var s = ifj.string("Hello");
 *        var n: ?i32 = null;
 *
 *        if(a < 10) {
 *            a = a + 1;
 *        } else {
 *            a = a - 1;
 *        }
 *
 *        while(b > 0) {
 *            b = b - 0.1;
 *        }
 *
 *        if(n == null) {
 *            n = 0;
 *        }
 *
 *        var c: i32 = add(a, 10);
 *        return c;
 *    }
 *
 *    pub fn add(x: i32, y: i32) i32 {
 *        return x + y;
 *    }
 * ``` 
 */

TEST(ASTPrintTest, AST_Example2_FactorialRecursive) {
    /***************************************************************************/
    /*                            Vytvoření stromu                             */
    /***************************************************************************/

    // Alokace kořene stromu
    AST_createTree();


    /***************************************************************************/
    /*                            Vytvoření prologu                            */
    /***************************************************************************/

    // Importovaným souborem bude "ifj.zig"
    DString *prolog = string_init();
    ASSERT_NE(prolog, nullptr);
    ASSERT_EQ(string_append_char(prolog, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(prolog, 'f'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(prolog, 'j'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(prolog, '.'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(prolog, 'z'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(prolog, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(prolog, 'g'), STRING_SUCCESS);
    ASTroot->importedFile = prolog;
    EXPECT_STREQ(prolog->str, "ifj.zig");


    /***************************************************************************/
    /*                        Vytvoření funkce 'add()'                         */
    /***************************************************************************/
    
    // Vytvoření uzlu funkce 'add()'
    AST_FunDefNode *addFun = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(addFun, nullptr);
    
    // Nastavení identifikátoru
    DString *addFunID = string_init();
    ASSERT_NE(addFun, nullptr);
    ASSERT_EQ(string_append_char(addFunID, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(addFunID, 'd'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(addFunID, 'd'), STRING_SUCCESS);
    addFun->identifier = addFunID;
    
    // Nastavení návratového typu
    addFun->returnType = AST_DATA_TYPE_INT;
    
    // Vytvoření parametrů 'x' a 'y'
    AST_ArgOrParamNode *paramX = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(paramX, nullptr);
    DString *paramXID = string_init();
    ASSERT_NE(paramXID, nullptr);
    ASSERT_EQ(string_append_char(paramXID, 'x'), STRING_SUCCESS);
    paramX->identifier = paramXID;
    addFun->parameters = paramX;

    AST_ArgOrParamNode *paramY = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(paramY, nullptr);
    DString *paramYID = string_init();
    ASSERT_NE(paramYID, nullptr);
    ASSERT_EQ(string_append_char(paramYID, 'y'), STRING_SUCCESS);
    paramX->identifier = paramYID;
    paramX->next = paramY;
    

    /***************************************************************************/
    /*           Vytvoření funkce těla funkce 'add': 'return x + y;'           */
    /***************************************************************************/
    
    // Levý operand: proměnná 'x'
    AST_ExprNode *exprVarX = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarX->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarX, nullptr);
    AST_VarNode *varX = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varX, nullptr);
    DString *varXID = string_init();
    ASSERT_NE(varXID, nullptr);
    ASSERT_EQ(string_append_char(varXID, 'x'), STRING_SUCCESS);
    varX->identifier = varXID;
    exprVarX->expression = varX;
    
    // Pravý operand: proměnná 'y'
    AST_ExprNode *exprVarY = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarY->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarY, nullptr);
    AST_VarNode *varY = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varY, nullptr);
    DString *varYID = string_init();
    ASSERT_NE(varYID, nullptr);
    ASSERT_EQ(string_append_char(varYID, 'y'), STRING_SUCCESS);
    varY->identifier = varYID;
    exprVarY->expression = varY;
    
    // Binární operace '+'
    AST_BinOpNode *binOpAdd = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpAdd, nullptr);
    binOpAdd->op = AST_OP_ADD;
    binOpAdd->left = exprVarX;
    binOpAdd->right = exprVarY;
    
    AST_ExprNode *exprAdd = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprAdd, nullptr);
    exprAdd->exprType = AST_EXPR_BINARY_OP;
    exprAdd->expression = binOpAdd;
    
    // Vytvoření návratového příkazu
    AST_StatementNode *returnStmt = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(returnStmt, nullptr);
    returnStmt->statementType = AST_STATEMENT_RETURN;
    returnStmt->statement = exprAdd;
    
    // Přiřazení těla funkce 'add'
    addFun->body = returnStmt;
    
    // Přidání funkce 'add' do seznamu funkcí programu
    addFun->next = ASTroot->functionList;
    ASTroot->functionList = addFun;
    
    /***************************************************************************/
    /*                        Vytvoření funkce 'main'                          */
    /***************************************************************************/
    
    // Vytvoření uzlu funkce 'main'
    AST_FunDefNode *mainFun = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);
    ASSERT_NE(mainFun, nullptr);
    
    // Nastavení identifikátoru
    DString *mainFunID = string_init();
    ASSERT_NE(mainFunID, nullptr);
    ASSERT_EQ(string_append_char(mainFunID, 'm'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunID, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunID, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(mainFunID, 'n'), STRING_SUCCESS);
    mainFun->identifier = mainFunID;
    
    // Nastavení návratového typu
    mainFun->returnType = AST_DATA_TYPE_VOID;
    
    // Vytvoření těla funkce 'main'
    AST_StatementNode *mainBody = NULL;
    AST_StatementNode **currentStmt = &mainBody;
    
    // 1. var a: i32 = 5;
    AST_VarNode *varA = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varA, nullptr);
    DString *varAID = string_init();
    ASSERT_NE(varAID, nullptr);
    ASSERT_EQ(string_append_char(varAID, 'a'), STRING_SUCCESS);
    varA->identifier = varAID;
    varA->literalType = AST_LITERAL_INT;
    
    // Vytvoření literálu '5'
    AST_VarNode *literal5 = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal5, nullptr);
    literal5->literalType = AST_LITERAL_INT;
    int *value5 = (int *)malloc(sizeof(int));
    *value5 = 5;
    literal5->value = value5;
    literal5->identifier = NULL;
    varA->value = literal5;
    
    // Vytvoření příkazu 'var a: i32 = 5;'
    AST_StatementNode *stmtVarA = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtVarA, nullptr);
    stmtVarA->statementType = AST_STATEMENT_VAR_DEF;
    stmtVarA->statement = varA;
    
    // Přidání příkazu do těla funkce
    *currentStmt = stmtVarA;
    currentStmt = &stmtVarA->next;
    
    // 2. var b: i64 = 3.14;
    AST_VarNode *varB = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varB, nullptr);
    DString *varBID = string_init();
    ASSERT_NE(varBID, nullptr);
    ASSERT_EQ(string_append_char(varBID, 'b'), STRING_SUCCESS);
    varB->identifier = varBID;
    varB->literalType = AST_LITERAL_FLOAT;
    
    // Vytvoření literálu '3.14'
    AST_VarNode *literalPi = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literalPi, nullptr);
    literalPi->literalType = AST_LITERAL_FLOAT;
    double *valuePi = (double *)malloc(sizeof(double));
    *valuePi = 3.14;
    literalPi->identifier = NULL;
    literalPi->value = valuePi;
    varB->value = literalPi;
    
    // Vytvoření příkazu 'var b: i64 = 3.14;'
    AST_StatementNode *stmtVarB = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtVarB, nullptr);
    stmtVarB->statementType = AST_STATEMENT_VAR_DEF;
    stmtVarB->statement = varB;
    
    // Přidání příkazu do těla funkce
    *currentStmt = stmtVarB;
    currentStmt = &stmtVarB->next;
    
    // 3. var s = ifj.string("Hello");
    AST_VarNode *varS = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varS, nullptr);
    DString *varSID = string_init();
    ASSERT_NE(varSID, nullptr);
    ASSERT_EQ(string_append_char(varSID, 's'), STRING_SUCCESS);
    varS->identifier = varSID;
    DString *helloStr = string_init();
    ASSERT_NE(helloStr, nullptr);
    ASSERT_EQ(string_append_char(helloStr, 'H'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(helloStr, 'e'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(helloStr, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(helloStr, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(helloStr, 'o'), STRING_SUCCESS);
    varS->literalType = AST_LITERAL_STRING; 
    varS->value = helloStr;

    // Proměnná pro argument volání funkce
    AST_VarNode *literalHello = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(literalHello, nullptr);
    DString *literalHelloVal = string_init();
    ASSERT_NE(literalHelloVal, nullptr);
    ASSERT_EQ(string_append_char(literalHelloVal, 'H'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(literalHelloVal, 'e'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(literalHelloVal, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(literalHelloVal, 'l'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(literalHelloVal, 'o'), STRING_SUCCESS);
    literalHello->identifier = NULL;
    literalHello->literalType = AST_LITERAL_STRING;
    literalHello->value = literalHelloVal;
    
    // Vytvoření argumentu pro funkci
    AST_ArgOrParamNode *argHello = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(argHello, nullptr);
    DString *argHelloID = string_init();
    ASSERT_EQ(string_copy(helloStr, argHelloID), STRING_SUCCESS);
    argHello->identifier = argHelloID;
    argHello->next = NULL;
    argHello->variable = literalHello; // Přidáme výraz jako argument
    
    // Vytvoření volání funkce 'ifj.string("Hello")'
    AST_FunCallNode *funCallString = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    ASSERT_NE(funCallString, nullptr);
    DString *funID = string_init();
    ASSERT_NE(funID, nullptr);
    ASSERT_EQ(string_append_char(funID, 's'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funID, 't'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funID, 'r'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funID, 'i'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funID, 'n'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funID, 'g'), STRING_SUCCESS);
    funCallString->identifier = funID;
    funCallString->isBuiltIn = true;
    funCallString->arguments = argHello;
    
    // Vytvoření výrazu pro volání funkce
    AST_ExprNode *exprFunCallString = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprFunCallString, nullptr);
    exprFunCallString->exprType = AST_EXPR_FUN_CALL;
    exprFunCallString->expression = funCallString;
    
    // Nastavení value proměnné 's' na výsledek volání funkce
    varS->value = exprFunCallString;
    
    // Vytvoření příkazu 'var s = ifj.string("Hello");'
    AST_StatementNode *stmtVarS = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtVarS, nullptr);
    stmtVarS->statementType = AST_STATEMENT_VAR_DEF;
    stmtVarS->statement = varS;
    
    // Přidání příkazu do těla funkce
    *currentStmt = stmtVarS;
    currentStmt = &stmtVarS->next;

    // 4. var n: ?i32 = null;
    AST_VarNode *varN = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varN, nullptr);
    DString *varNID = string_init();
    ASSERT_NE(varNID, nullptr);
    ASSERT_EQ(string_append_char(varNID, 'n'), STRING_SUCCESS);
    varN->identifier = varNID;
    varN->literalType = AST_LITERAL_NULL;
    varN->value = nullptr;

    // Vytvoření příkazu 'var n: ?i32 = null;'
    AST_StatementNode *stmtVarN = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtVarN, nullptr);
    stmtVarN->statementType = AST_STATEMENT_VAR_DEF;
    stmtVarN->statement = varN;

    // Přidání příkazu do těla funkce
    *currentStmt = stmtVarN;
    currentStmt = &stmtVarN->next;

    // 5. if(a < 10) { a = a + 1; } else { a = a - 1; }
    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(ifNode, nullptr);

    // Podmínka: a < 10
    AST_ExprNode *exprVarA = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarA->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarA, nullptr);
    AST_VarNode *varACond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varACond, nullptr);
    DString *varACondID = string_init();
    ASSERT_NE(varACondID, nullptr);
    ASSERT_EQ(string_append_char(varACondID, 'a'), STRING_SUCCESS);
    varACond->identifier = varACondID;
    exprVarA->expression = varACond;

    AST_VarNode *literal10 = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal10, nullptr);
    literal10->literalType = AST_LITERAL_INT;
    int *value10 = (int *)malloc(sizeof(int));
    *value10 = 10;
    literal10->value = value10;
    literal10->identifier = NULL;

    AST_BinOpNode *binOpLessThan = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpLessThan, nullptr);
    binOpLessThan->op = AST_OP_LESS_THAN;
    binOpLessThan->left = exprVarA;
    binOpLessThan->right = (AST_ExprNode *)literal10;

    AST_ExprNode *exprLessThan = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprLessThan, nullptr);
    exprLessThan->exprType = AST_EXPR_BINARY_OP;
    exprLessThan->expression = binOpLessThan;
    ifNode->condition = exprLessThan;

    // Větev 'then': a = a + 1
    AST_ExprNode *exprVarAThen = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarAThen->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarAThen, nullptr);
    AST_VarNode *varAThen = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varAThen, nullptr);
    varAThen->identifier = varACondID;
    exprVarAThen->expression = varAThen;

    AST_VarNode *literal1 = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal1, nullptr);
    literal1->literalType = AST_LITERAL_INT;
    int *value1 = (int *)malloc(sizeof(int));
    *value1 = 1;
    literal1->value = value1;
    literal1->identifier = NULL;

    AST_BinOpNode *binOpAdd1 = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpAdd1, nullptr);
    binOpAdd1->op = AST_OP_ADD;
    binOpAdd1->left = exprVarAThen;
    binOpAdd1->right = (AST_ExprNode *)literal1;

    AST_ExprNode *exprAdd1 = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprAdd1, nullptr);
    exprAdd1->exprType = AST_EXPR_BINARY_OP;
    exprAdd1->expression = binOpAdd1;

    AST_StatementNode *stmtAssignAThen = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtAssignAThen, nullptr);
    stmtAssignAThen->statementType = AST_STATEMENT_EXPR;
    stmtAssignAThen->statement = exprAdd1;

    ifNode->thenBranch = stmtAssignAThen;

    // Větev 'else': a = a - 1
    AST_ExprNode *exprVarAElse = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarAElse->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarAElse, nullptr);
    AST_VarNode *varAElse = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varAElse, nullptr);
    varAElse->identifier = varACondID;
    exprVarAElse->expression = varAElse;

    AST_BinOpNode *binOpSub1 = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpSub1, nullptr);
    binOpSub1->op = AST_OP_SUBTRACT;
    binOpSub1->left = exprVarAElse;
    binOpSub1->right = (AST_ExprNode *)literal1;

    AST_ExprNode *exprSub1 = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprSub1, nullptr);
    exprSub1->exprType = AST_EXPR_BINARY_OP;
    exprSub1->expression = binOpSub1;

    AST_StatementNode *stmtAssignAElse = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtAssignAElse, nullptr);
    stmtAssignAElse->statementType = AST_STATEMENT_EXPR;
    stmtAssignAElse->statement = exprSub1;

    ifNode->elseBranch = stmtAssignAElse;

    // Vytvoření příkazu 'if(a < 10) { a = a + 1; } else { a = a - 1; }'
    AST_StatementNode *stmtIf = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtIf, nullptr);
    stmtIf->statementType = AST_STATEMENT_IF;
    stmtIf->statement = ifNode;

    // Přidání příkazu do těla funkce
    *currentStmt = stmtIf;
    currentStmt = &stmtIf->next;

    // 6. while(b > 0) { b = b - 0.1; }
    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    ASSERT_NE(whileNode, nullptr);

    // Podmínka: b > 0
    AST_ExprNode *exprVarB = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarB->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarB, nullptr);
    AST_VarNode *varBCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varBCond, nullptr);
    DString *varBCondID = string_init();
    ASSERT_NE(varBCondID, nullptr);
    ASSERT_EQ(string_append_char(varBCondID, 'b'), STRING_SUCCESS);
    varBCond->identifier = varBCondID;
    exprVarB->expression = varBCond;

    AST_VarNode *literal0 = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal0, nullptr);
    literal0->literalType = AST_LITERAL_FLOAT;
    double *value0 = (double *)malloc(sizeof(double));
    *value0 = 0.0;
    literal0->value = value0;
    literal0->identifier = NULL;

    AST_BinOpNode *binOpGreaterThan = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpGreaterThan, nullptr);
    binOpGreaterThan->op = AST_OP_GREATER_THAN;
    binOpGreaterThan->left = exprVarB;
    binOpGreaterThan->right = (AST_ExprNode *)literal0;

    AST_ExprNode *exprGreaterThan = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprGreaterThan, nullptr);
    exprGreaterThan->exprType = AST_EXPR_BINARY_OP;
    exprGreaterThan->expression = binOpGreaterThan;
    whileNode->condition = exprGreaterThan;

    // Tělo cyklu: b = b - 0.1
    AST_ExprNode *exprVarBBody = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarBBody->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarBBody, nullptr);
    AST_VarNode *varBBody = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varBBody, nullptr);
    varBBody->identifier = varBCondID;
    exprVarBBody->expression = varBBody;

    AST_VarNode *literal01 = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal01, nullptr);
    literal01->literalType = AST_LITERAL_FLOAT;
    double *value01 = (double *)malloc(sizeof(double));
    *value01 = 0.1;
    literal01->value = value01;
    literal01->identifier = NULL;

    AST_BinOpNode *binOpSub01 = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpSub01, nullptr);
    binOpSub01->op = AST_OP_SUBTRACT;
    binOpSub01->left = exprVarBBody;
    binOpSub01->right = (AST_ExprNode *)literal01;

    AST_ExprNode *exprSub01 = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprSub01, nullptr);
    exprSub01->exprType = AST_EXPR_BINARY_OP;
    exprSub01->expression = binOpSub01;

    AST_StatementNode *stmtAssignB = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtAssignB, nullptr);
    stmtAssignB->statementType = AST_STATEMENT_EXPR;
    stmtAssignB->statement = exprSub01;

    whileNode->body = stmtAssignB;

    // Vytvoření příkazu 'while(b > 0) { b = b - 0.1; }'
    AST_StatementNode *stmtWhile = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtWhile, nullptr);
    stmtWhile->statementType = AST_STATEMENT_WHILE;
    stmtWhile->statement = whileNode;

    // Přidání příkazu do těla funkce
    *currentStmt = stmtWhile;
    currentStmt = &stmtWhile->next;

    // 7. if(n == null) { n = 0; }
    AST_IfNode *ifNodeNull = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    ASSERT_NE(ifNodeNull, nullptr);

    // Podmínka: n == null
    AST_ExprNode *exprVarN = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarN->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarN, nullptr);
    AST_VarNode *varNCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varNCond, nullptr);
    DString *varNCondID = string_init();
    ASSERT_NE(varNCondID, nullptr);
    ASSERT_EQ(string_append_char(varNCondID, 'n'), STRING_SUCCESS);
    varNCond->identifier = varNCondID;
    exprVarN->expression = varNCond;

    AST_VarNode *literalNull = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literalNull, nullptr);
    literalNull->literalType = AST_LITERAL_NULL;
    literalNull->value = nullptr;
    literalNull->identifier = NULL;

    AST_BinOpNode *binOpEqualNull = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpEqualNull, nullptr);
    binOpEqualNull->op = AST_OP_EQUAL;
    binOpEqualNull->left = exprVarN;
    binOpEqualNull->right = (AST_ExprNode *)literalNull;

    AST_ExprNode *exprEqualNull = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprEqualNull, nullptr);
    exprEqualNull->exprType = AST_EXPR_BINARY_OP;
    exprEqualNull->expression = binOpEqualNull;
    ifNodeNull->condition = exprEqualNull;

    // Větev 'then': n = 0
    AST_ExprNode *exprVarNThen = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarNThen->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarNThen, nullptr);
    AST_VarNode *varNThen = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varNThen, nullptr);
    varNThen->identifier = varNCondID;
    exprVarNThen->expression = varNThen;

    AST_VarNode *literal0Int = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal0Int, nullptr);
    literal0Int->literalType = AST_LITERAL_INT;
    int *value0Int = (int *)malloc(sizeof(int));
    *value0Int = 0;
    literal0Int->value = value0Int;
    literal0Int->identifier = NULL;

    AST_BinOpNode *binOpAssign0 = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    ASSERT_NE(binOpAssign0, nullptr);
    binOpAssign0->op = AST_OP_ASSIGNEMENT;
    binOpAssign0->left = exprVarNThen;
    binOpAssign0->right = (AST_ExprNode *)literal0Int;

    AST_ExprNode *exprAssign0 = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprAssign0, nullptr);
    exprAssign0->exprType = AST_EXPR_BINARY_OP;
    exprAssign0->expression = binOpAssign0;

    AST_StatementNode *stmtAssignNThen = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtAssignNThen, nullptr);
    stmtAssignNThen->statementType = AST_STATEMENT_EXPR;
    stmtAssignNThen->statement = exprAssign0;

    ifNodeNull->thenBranch = stmtAssignNThen;

    // Vytvoření příkazu 'if(n == null) { n = 0; }'
    AST_StatementNode *stmtIfNull = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    ASSERT_NE(stmtIfNull, nullptr);
    stmtIfNull->statementType = AST_STATEMENT_IF;
    stmtIfNull->statement = ifNodeNull;

    // Přidání příkazu do těla funkce
    *currentStmt = stmtIfNull;
    currentStmt = &stmtIfNull->next;

    // 8. var c: i32 = add(a, 10);
    AST_VarNode *varC = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varC, nullptr);
    DString *varCID = string_init();
    ASSERT_NE(varCID, nullptr);
    ASSERT_EQ(string_append_char(varCID, 'c'), STRING_SUCCESS);
    varC->identifier = varCID;

    // Vytvoření argumentu 'a'
    AST_ExprNode *exprVarAArg = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    exprVarAArg->exprType = AST_EXPR_VARIABLE;
    ASSERT_NE(exprVarAArg, nullptr);
    AST_VarNode *varAArg = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    ASSERT_NE(varAArg, nullptr);
    varAArg->identifier = varACondID;
    exprVarAArg->expression = varAArg;

    // Vytvoření argumentu '10'
    AST_VarNode *literal10Arg = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
    ASSERT_NE(literal10Arg, nullptr);
    literal10Arg->literalType = AST_LITERAL_INT;
    int *value10Arg = (int *)malloc(sizeof(int));
    *value10Arg = 10;
    literal10Arg->value = value10Arg;
    literal10Arg->identifier = NULL;

    // Vytvoření volání funkce 'add(a, 10)'
    AST_FunCallNode *funCallAdd = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    ASSERT_NE(funCallAdd, nullptr);
    DString *funAddID = string_init();
    ASSERT_NE(funAddID, nullptr);
    ASSERT_EQ(string_append_char(funAddID, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funAddID, 'd'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(funAddID, 'd'), STRING_SUCCESS);
    funCallAdd->identifier = funAddID;
    funCallAdd->isBuiltIn = false;

    AST_ArgOrParamNode *argA = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(argA, nullptr);
    argA->identifier = varACondID;
    argA->variable = varAArg;

    AST_ArgOrParamNode *arg10 = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    ASSERT_NE(arg10, nullptr);
    arg10->identifier = NULL;
    arg10->variable = literal10Arg;
    argA->next = arg10;

    funCallAdd->arguments = argA;

    // Vytvoření výrazu pro volání funkce
    AST_ExprNode *exprFunCallAdd = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    ASSERT_NE(exprFunCallAdd, nullptr);
    exprFunCallAdd->exprType = AST_EXPR_FUN_CALL;
    exprFunCallAdd->expression = funCallAdd;

    // Nastavení value proměnné 'c' na výsledek volání funkce
   


    
    // Na závěr přiřadíme tělo funkce 'main'
    mainFun->body = mainBody;
    
    // Přidání funkce 'main' do seznamu funkcí programu
    mainFun->next = ASTroot->functionList;
    ASTroot->functionList = mainFun;
    
    /***************************************************************************/
    /*                              Výpis AST stromu                           */
    /***************************************************************************/
    
    // Získání vytvořeného stromu
    string output = ASTutils_printCapturedOutput(ASTroot);
    
    // Výpis AST stromu
    cout << output << std::endl;
    
    // Referenční strom (zde je potřeba vytvořit očekávaný výstup)
    string reference =
        "Program Node\n"
        "├── Imported File: ifj.zig\n"
        "├── Function Definition: main\n"
        "│   ├── Parameters: (null)\n"
        "│   ├── Return Type: 7\n"
        "│   └── Function Body:\n"
        "│       ├── Statement: Variable Definition\n"
        "│       │   └── Variable: a\n"
        "│       │       └── Value:\n"
        "│       │           └── Literal: Integer, Value: 5\n"
        "│       ├── Statement: Variable Definition\n"
        "│       │   └── Variable: b\n"
        "│       │       └── Value:\n"
        "│       │           └── Literal: Float, Value: 3.14\n"
        "│       ├── Statement: Variable Definition\n"
        "│       │   └── Variable: s\n"
        "│       │       └── Value:\n"
        "│       │           └── Expression: Function Call\n"
        "│       │               └── Function Call: string\n"
        "│       │                   └── Arguments:\n"
        "│       │                       └── Parameter: Hello\n"
        "│       │                           └── Variable: (null)\n"
        "│       │                               └── Value:\n"
        "│       │                                   └── Unknown Value Type\n"
        "│       ├── Statement: Variable Definition\n"
        "│       │   └── Variable: n\n"
        "│       │       └── Value: (null)\n"
        "│       ├── Statement: If Statement\n"
        "│       │   └── If Statement\n"
        "│       │       ├── Condition:\n"
        "│       │       │   └── Expression: Binary Operation\n"
        "│       │       │       └── Operator: <\n"
        "│       │       │           ├── Left Operand:\n"
        "│       │       │           │   └── Expression: Variable\n"
        "│       │       │           │       └── Variable: a\n"
        "│       │       │           │           └── Value: (null)\n"
        "│       │       │           └── Right Operand:\n"
        "│       │       │               └── Expression: Unknown Expression Type\n"
        "│       │       ├── Then Branch:\n"
        "│       │       │   └── Statement: Expression\n"
        "│       │       │       └── Expression: Binary Operation\n"
        "│       │       │           └── Operator: +\n"
        "│       │       │               ├── Left Operand:\n"
        "│       │       │               │   └── Expression: Variable\n"
        "│       │       │               │       └── Variable: a\n"
        "│       │       │               │           └── Value: (null)\n"
        "│       │       │               └── Right Operand:\n"
        "│       │       │                   └── Expression: Unknown Expression Type\n"
        "│       │       └── Else Branch:\n"
        "│       │           └── Statement: Expression\n"
        "│       │               └── Expression: Binary Operation\n"
        "│       │                   └── Operator: -\n"
        "│       │                       ├── Left Operand:\n"
        "│       │                       │   └── Expression: Variable\n"
        "│       │                       │       └── Variable: a\n"
        "│       │                       │           └── Value: (null)\n"
        "│       │                       └── Right Operand:\n"
        "│       │                           └── Expression: Unknown Expression Type\n"
        "│       ├── Statement: While Loop\n"
        "│       │   └── While Loop\n"
        "│       │       ├── Condition:\n"
        "│       │       │   └── Expression: Binary Operation\n"
        "│       │       │       └── Operator: >\n"
        "│       │       │           ├── Left Operand:\n"
        "│       │       │           │   └── Expression: Variable\n"
        "│       │       │           │       └── Variable: b\n"
        "│       │       │           │           └── Value: (null)\n"
        "│       │       │           └── Right Operand:\n"
        "│       │       │               └── Expression: Unknown Expression Type\n"
        "│       │       └── Body:\n"
        "│       │           └── Statement: Expression\n"
        "│       │               └── Expression: Binary Operation\n"
        "│       │                   └── Operator: -\n"
        "│       │                       ├── Left Operand:\n"
        "│       │                       │   └── Expression: Variable\n"
        "│       │                       │       └── Variable: b\n"
        "│       │                       │           └── Value: (null)\n"
        "│       │                       └── Right Operand:\n"
        "│       │                           └── Expression: Unknown Expression Type\n"
        "│       └── Statement: If Statement\n"
        "│           └── If Statement\n"
        "│               ├── Condition:\n"
        "│               │   └── Expression: Binary Operation\n"
        "│               │       └── Operator: ==\n"
        "│               │           ├── Left Operand:\n"
        "│               │           │   └── Expression: Variable\n"
        "│               │           │       └── Variable: n\n"
        "│               │           │           └── Value: (null)\n"
        "│               │           └── Right Operand:\n"
        "│               │               └── Expression: Unknown Expression Type\n"
        "│               ├── Then Branch:\n"
        "│               │   └── Statement: Expression\n"
        "│               │       └── Expression: Binary Operation\n"
        "│               │           └── Operator: =\n"
        "│               │               ├── Left Operand:\n"
        "│               │               │   └── Expression: Variable\n"
        "│               │               │       └── Variable: n\n"
        "│               │               │           └── Value: (null)\n"
        "│               │               └── Right Operand:\n"
        "│               │                   └── Expression: Unknown Expression Type\n"
        "│               └── Else Branch: (null)\n"
        "└── Function Definition: add\n"
        "    ├── Parameters:\n"
        "    │   ├── Parameter: y\n"
        "    │   │   └── Variable/Expression: (null)\n"
        "    │   └── Parameter: (null)\n"
        "    │       └── Variable/Expression: (null)\n"
        "    ├── Return Type: 1\n"
        "    └── Function Body:\n"
        "        └── Statement: Return Statement\n"
        "            └── Expression: Binary Operation\n"
        "                └── Operator: +\n"
        "                    ├── Left Operand:\n"
        "                    │   └── Expression: Variable\n"
        "                    │       └── Variable: x\n"
        "                    │           └── Value: (null)\n"
        "                    └── Right Operand:\n"
        "                        └── Expression: Variable\n"
        "                            └── Variable: y\n"
        "                                └── Value: (null)\n";
    
    // Porovnání získaného stromu s referenčním a jejich výpis
    EXPECT_STREQ(output.c_str(), reference.c_str());
    ASTutils_printDiff(output, reference);
    
    // Uvolnění paměti celého stromu
    AST_destroyTree();
}



/*** Konec souboru ast_test.cpp ***/