/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.c                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
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
 * @file tac_generator.c
 * @author Lukáš Farkašovský \<xfarkal00>
 *
 * @brief Implementace funkcí pro generátor vnitřního kódu (3AK + Cílový jazyk).
 * @details Knihovna pro generování cílového kódu z AST.
 */

#include "tac_generator.h"
#include "semantic_analyser.h"

/**
 * @brief Vytvoří a inicializuje novou tříadresnou instrukci na základní hodnoty.
 */
TAC_Instruction *TAC_initInstruction(){
    TAC_Instruction *instr = (TAC_Instruction *)malloc(sizeof(struct TAC_Instruction));
    if(instr == NULL){
        return NULL;
    }

    // Základní inicializace instrukce
    instr->op = TAC_OP_NONE;
    instr->dest.type = TAC_OPERAND_NONE;
    instr->src1.type = TAC_OPERAND_NONE;
    instr->src2.type = TAC_OPERAND_NONE;
    instr->next = NULL;

    return instr;
}

/**
 * @brief Vytvoří novou tříadresnou instrukci.
 */
void TAC_createInstruction(TAC_Operation op, TAC_Operand dest, TAC_Operand src1, TAC_Operand src2){
    // Alokace paměti a inicializace pro novou instrukci
    TAC_Instruction *instr = TAC_initInstruction();
    if(instr == NULL){
        error_handle(ERROR_INTERNAL);
    }

    instr->op = op;

    // Switch pro nastavení operandů instrukce pro jednotlivé operace
    switch (instr->op){
        case TAC_OP_JUMPIFEQ:
        case TAC_OP_JUMPIFNEQ:
        case TAC_OP_ADD:
        case TAC_OP_SUB:
        case TAC_OP_MUL:
        case TAC_OP_DIV:
        case TAC_OP_IF_EQ:
        case TAC_OP_IF_LT:
        case TAC_OP_IF_GT:
        case TAC_OP_CONCAT:
        case TAC_OP_OR:
            instr->src1 = src1;
            instr->src2 = src2;
            instr->dest = dest;
            TAC_printInstruction(instr);
            break;

        case TAC_OP_ASSIGN:
        case TAC_OP_READ:
        case TAC_OP_TYPE:
        case TAC_OP_NOT:
            instr->src1 = src1;
            instr->dest = dest;
            TAC_printInstruction(instr);
            break;

        case TAC_OP_PARAM:
        case TAC_OP_CALL:
        case TAC_OP_LABEL:
        case TAC_OP_JUMP:
        case TAC_OP_EXIT:
        case TAC_OP_WRITE:
        case TAC_OP_DEFVAR:
            instr->dest = dest;
            TAC_printInstruction(instr);
            break;

        default:
            TAC_printInstruction(instr);
            break;
    }
    free(instr);
} // TAC_createInstruction

/**
 * @brief Vytiskne operand tříadresného kódu.
 */
void TAC_printOperand(TAC_Operand *operand){

    if(operand->type != TAC_OPERAND_INT && operand->type != TAC_OPERAND_FLOAT && operand->type != TAC_OPERAND_NIL){
        // Pomocný řetězec pro výpis, kvůli alokaci ve funkci string_toConstChar
        char *stringHelpVar = string_toConstChar(operand->value.varName);
        char *stringHelpLabel = string_toConstChar(operand->value.labelName);

        switch (operand->type){
            case TAC_OPERAND_NONE: break;
            case TAC_OPERAND_VAR: printf("LF@$%s", stringHelpVar); break;
            case TAC_OPERAND_TEMP: printf("TF@$%s", stringHelpVar); break;
            case TAC_OPERAND_LABEL: printf("_%s", stringHelpLabel); break;
            case TAC_OPERAND_STRING: printf("string@%s", stringHelpVar); break;
            case TAC_OPERAND_BOOL: printf("bool@%s", stringHelpVar); break;
            case TAC_OPERAND_GLOBAL: printf("GF@$%s", stringHelpVar); break;
            default: break;
        }

        // Uvolnění alokovaného řetězce
        free(stringHelpVar);
        free(stringHelpLabel);
    }
    else{

        switch (operand->type){
            case TAC_OPERAND_INT: printf("int@%d", operand->value.intValue); break;
            case TAC_OPERAND_FLOAT: printf("float@%lf", operand->value.floatValue); break;
            case TAC_OPERAND_NIL: printf("nil@nil"); break;
            default: break;
        }
    }
}// TAC_printOperand

/**
 * @brief Vytiskne instrukci tříadresného kódu.
 */
void TAC_printInstruction(TAC_Instruction *instr){
    // Výpis operace instrukce
    switch (instr->op){
        case TAC_OP_NONE: break;
        case TAC_OP_ADD: printf("ADD "); break; // ADD dest, src1, src2
        case TAC_OP_SUB: printf("SUB "); break; // SUB dest, src1, src2
        case TAC_OP_MUL: printf("MUL "); break; // MUL dest, src1, src2
        case TAC_OP_DIV: printf("DIV "); break; // DIV dest, src1, src2
        case TAC_OP_IF_EQ: printf("EQ "); break; // EQ dest, src1, src2
        case TAC_OP_IF_LT: printf("LT "); break; // LT dest, src1, src2
        case TAC_OP_IF_GT: printf("GT "); break; // GT dest, src1, src2
        case TAC_OP_CALL: printf("CALL "); break; // CALL dest
        case TAC_OP_RETURN: printf("RETURN "); break; // RETURN src1
        case TAC_OP_LABEL: printf("LABEL "); break; // LABEL dest
        case TAC_OP_JUMP: printf("JUMP "); break; // JUMP dest
        case TAC_OP_JUMPIFEQ: printf("JUMPIFEQ "); break; // JUMPIFEQ dest, src1, src2
        case TAC_OP_JUMPIFNEQ: printf("JUMPIFNEQ "); break; // JUMPIFNEQ dest, src1, src2
        case TAC_OP_TYPE: printf("TYPE "); break; // TYPE dest, src1
        case TAC_OP_EXIT: printf("EXIT "); break; // EXIT src1
        case TAC_OP_CREATEFRAME: printf("CREATEFRAME"); break; // CREATEFRAME
        case TAC_OP_PUSHFRAME: printf("PUSHFRAME"); break; // PUSHFRAME
        case TAC_OP_POPFRAME: printf("POPFRAME"); break; // POPFRAME
        case TAC_OP_DEFVAR: printf("DEFVAR "); break; // DEFVAR dest
        case TAC_OP_WRITE: printf("WRITE "); break; // WRITE src1
        case TAC_OP_READ: printf("READ "); break; // READ dest
        case TAC_OP_CONCAT: printf("CONCAT "); break; // CONCAT dest, src1, src2
        case TAC_OP_ASSIGN: printf("MOVE "); break; // MOVE dest, src1
        case TAC_OP_CLEARS: printf("CLEARS"); break; // CLEARS
        case TAC_OP_NOT: printf("NOT "); break; // NOT dest, src1
        default: break;
    }
    // Výpis operandů instrukce --> dest, src1, src2 (pokud využívá všechny adresy)
    switch (instr->op){
        // dest, src1, src2
        case TAC_OP_NONE: break;
        case TAC_OP_ADD:
        case TAC_OP_SUB:
        case TAC_OP_MUL:
        case TAC_OP_DIV:
        case TAC_OP_JUMPIFEQ:
        case TAC_OP_JUMPIFNEQ:
        case TAC_OP_IF_EQ:
        case TAC_OP_IF_LT:
        case TAC_OP_IF_GT:
        case TAC_OP_NOT:
            // Destinace --> kam se zapíš€e výsledek
            TAC_printOperand(&(instr->dest));
            // Mezery mezi operandy
            printf(" ");
            TAC_printOperand(&(instr->src1));
            printf(" ");
            TAC_printOperand(&(instr->src2));
            printf("\n");
            return;

        // dest
        case TAC_OP_DEFVAR:
        case TAC_OP_JUMP:
        case TAC_OP_EXIT:
        case TAC_OP_WRITE:
        case TAC_OP_CALL:
        case TAC_OP_LABEL:
            TAC_printOperand(&(instr->dest));
            printf("\n");
            return;

        // dest, src1
        case TAC_OP_ASSIGN:
        case TAC_OP_TYPE:
        case TAC_OP_READ:
            TAC_printOperand(&(instr->dest));
            printf(" ");
            TAC_printOperand(&(instr->src1));
            printf("\n");
            return;

        // Pokud instrukce nepotřebuje žádný operand
        default:
            printf("\n");
            return;
    }
}// TAC_printInstruction

/**
 * @brief Vytvoří označení pro začátek programu.
 */
bool TAC_generateProgramCodeBegin(AST_ProgramNode *programNode){
    // Globální proměnné pro práci s operacemi, které ukládají výsledek do proměnné
    // ?tempDEST, ?tempSRC1, ?tempSRC2 --> používá se ? pro zamezení kolize s názvy proměnných
    DString *tempDEST = string_charToDString("?tempDEST");
    DString *tempSRC1 = string_charToDString("?tempSRC1");
    DString *tempSRC2 = string_charToDString("?tempSRC2");
    DString *label = string_charToDString(programNode->importedFile->identifier->str);

    TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_GLOBAL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = tempSRC1, .type = TAC_OPERAND_GLOBAL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = tempSRC2, .type = TAC_OPERAND_GLOBAL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});

    // Vytvoření rámce pro tělo programu
    TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = label, .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_PUSHFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});

    // Uvolnění pomocných řetězců
    string_free(tempDEST);
    string_free(tempSRC1);
    string_free(tempSRC2);
    string_free(label);
    return true;
} // TAC_generateProgramCode

bool TAC_generateProgramCodeEnd(){
    // Přesun rámce ze zásobníku do globálního rámce
    TAC_createInstruction(TAC_OP_POPFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    // Vyčistíme zásobník rámců
    TAC_createInstruction(TAC_OP_CLEARS, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    return true;
} // TAC_generateProgramCode

/**
 * @brief Generuje kód pro začátek definice funkce.
 */
bool TAC_generateFunctionDefinitionBegin(AST_FunDefNode *funDefNode){
    // Pomocný řetězec pro výpis, kvůli alokaci ve funkci string_toConstChar
    DString *label = string_charToDString(funDefNode->identifier->str);

    TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = label, .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});

    // Uvolnění pomocného řetězce
    string_free(label);
    return true;
} // TAC_generateFunctionDefinitionBegin

/**
 * @brief Generuje kód pro konec definice funkce.
 */
bool TAC_generateFunctionDefinitionEnd(){
    TAC_createInstruction(TAC_OP_POPFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    // Návrat do místa, kde bylo zavoláno call
    TAC_createInstruction(TAC_OP_RETURN, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    return true;
} // TAC_generateFunctionDefinitionEnd

/**
 * @brief Generuje tříadresný kód pro binární operace.
 */
bool TAC_generateBinOp(AST_BinOpNode *binOp){
    AST_ExprNode *leftOperand = binOp->left;
    AST_ExprNode *rightOperand = binOp->right;

    // Rekurzivní volání pro výrazy v levém a pravém operandu --> důležité pro složitější výrazy

    // Levý operand
    if (leftOperand->exprType == AST_EXPR_BINARY_OP){
        TAC_generateBinOp((AST_BinOpNode *)leftOperand->expression);
    }

    // Pravý operand
    if(rightOperand->exprType == AST_EXPR_BINARY_OP){
        TAC_generateBinOp((AST_BinOpNode *)rightOperand->expression);
    }

    TAC_Operand operandUsedLeft;
    TAC_Operand operandUsedRight;

    // Pokude je levý operand literál
    if(leftOperand->exprType == AST_EXPR_LITERAL){
        // Switch na určení typu literálu
        switch (((AST_VarNode*)leftOperand->expression)->literalType){
            case AST_LITERAL_INT:
                operandUsedLeft.type = TAC_OPERAND_INT;
                operandUsedLeft.value.intValue = *((int *)((AST_VarNode *)leftOperand->expression)->value);
                break;

            case AST_LITERAL_FLOAT:
                operandUsedLeft.type = TAC_OPERAND_FLOAT;
                operandUsedLeft.value.floatValue = *((double *)((AST_VarNode *)leftOperand->expression)->value);
                break;

            case AST_LITERAL_STRING:
                operandUsedLeft.type = TAC_OPERAND_STRING;
                operandUsedLeft.value.varName = (DString *)((AST_VarNode *)leftOperand->expression)->value;
                break;

            case AST_LITERAL_NULL:
                operandUsedLeft.type = TAC_OPERAND_NIL;
                break;

            default: break;
        }
    }

    // Pokud je pravý operand literál
    if(rightOperand->exprType == AST_EXPR_LITERAL){
        // Switch na určení typu literálu
        switch (((AST_VarNode*)rightOperand->expression)->literalType){
            case AST_LITERAL_INT:
                operandUsedRight.type = TAC_OPERAND_INT;
                operandUsedRight.value.intValue = *((int *)((AST_VarNode *)rightOperand->expression)->value);
                break;

            case AST_LITERAL_FLOAT:
                operandUsedRight.type = TAC_OPERAND_FLOAT;
                operandUsedRight.value.floatValue = *((double *)((AST_VarNode *)rightOperand->expression)->value);
                break;

            case AST_LITERAL_STRING:
                operandUsedRight.type = TAC_OPERAND_STRING;
                operandUsedRight.value.varName = (DString *)((AST_VarNode *)rightOperand->expression)->value;
                break;

            case AST_LITERAL_NULL:
                operandUsedRight.type = TAC_OPERAND_NIL;
                break;

            default: break;
        }
    }

    // Pokud je levý operand proměnná
    if(leftOperand->exprType == AST_EXPR_VARIABLE){
        operandUsedLeft.type = TAC_OPERAND_VAR;
        operandUsedLeft.value.varName = ((AST_VarNode *)leftOperand->expression)->identifier;
    }

    // Pokud je pravý operand proměnná
    if(rightOperand->exprType == AST_EXPR_VARIABLE){
        operandUsedRight.type = TAC_OPERAND_VAR;
        operandUsedRight.value.varName = ((AST_VarNode *)rightOperand->expression)->identifier;
    }

    // Pomocné proměnné pro ukládání výsledku operací
    DString *tempDEST = string_charToDString("?tempDEST");
    DString *tempSRC1 = string_charToDString("?tempSRC1");

    // Vytvoření instrukce pro každý binární operátor
    // ?tempDEST ==> globální proměnnám, do které ukládáme výsledek operací
    switch (binOp->op){
        case AST_OP_ADD:
            TAC_createInstruction(TAC_OP_ADD, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_SUBTRACT:
            TAC_createInstruction(TAC_OP_SUB, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_MULTIPLY:
            TAC_createInstruction(TAC_OP_MUL, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_DIVIDE:
            TAC_createInstruction(TAC_OP_DIV, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_ASSIGNMENT:
            TAC_createInstruction(TAC_OP_ASSIGN, operandUsedLeft, operandUsedRight, (TAC_Operand){.type = TAC_OPERAND_NONE});
            break;

        case AST_OP_EQUAL:
            TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_NOT_EQUAL:
            TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_createInstruction(TAC_OP_NOT, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.type = TAC_OPERAND_NONE});
            break;

        case AST_OP_GREATER_THAN:
            TAC_createInstruction(TAC_OP_IF_GT, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_LESS_THAN:
            TAC_createInstruction(TAC_OP_IF_LT, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            break;

        case AST_OP_GREATER_EQUAL:
            TAC_createInstruction(TAC_OP_IF_GT, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = tempSRC1, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_createInstruction(TAC_OP_OR, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = tempSRC1, .type = TAC_OPERAND_TEMP});
            break;

        case AST_OP_LESS_EQUAL:
            TAC_createInstruction(TAC_OP_IF_LT, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_createInstruction(TAC_OP_OR, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = tempDEST, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = tempSRC1, .type = TAC_OPERAND_TEMP});
            break;

        default: break;
    }

    // Uvolnění pomocných řetězců
    string_free(tempDEST);
    string_free(tempSRC1);

    return true;
} // TAC_generateBinOp

/**
 * @brief Generuje tříadresný kód pro výrazy.
 */
/*bool TAC_generateExpression(AST_ExprNode *exprNode){
    // Přidělení binárního operátoru z výrazu
    AST_BinOpNode *binOp = (AST_BinOpNode *)exprNode->expression;
    AST_FunCallNode *funCall = (AST_FunCallNode *)exprNode->expression;

    switch (exprNode->exprType){
        case AST_EXPR_BINARY_OP:
            TAC_generateBinOp(binOp);
            break;

        case AST_EXPR_FUN_CALL:
            TAC_generateFunctionCall(funCall);
            break;

        default: break;
    }

    return true;
} */// TAC_generateStatement

/**
 * @brief Generuje tříadresný kód pro příkazy.
 */
bool TAC_generateStatement(AST_StatementNode *statement){

    switch (statement->statementType){
        case AST_STATEMENT_VAR_DEF:
            TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString(((AST_VarNode *)statement->statement)->identifier->str), .type = TAC_OPERAND_VAR}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
            break;

        default: break;
    }
    return true;
} // TAC_generateStatement
/*

 * @brief Uvolní operand.

void TAC_destroyOperand(TAC_Operand *operand){
    switch (operand->type){
        // Uvolní dynamický řetězec varName
        case TAC_OPERAND_VAR:
        case TAC_OPERAND_TEMP:
        case TAC_OPERAND_STRING:
        case TAC_OPERAND_BOOL:
        case TAC_OPERAND_GLOBAL:
            string_free(operand->value.varName);
            break;

        // Uvolní dynamický řetězec labelName
        case TAC_OPERAND_LABEL:
            string_free(operand->value.labelName);
            break;

        // Pokud operand nemá dynamický řetězec, nemáme co uvolnit
        default:
            break;
    }
} // TAC_destroyOperand


 * @brief Uvolní instrukci.

void TAC_destroyInstruction(TAC_Instruction *instr){
    switch (instr->op){
        // dest, src1, src2
        case TAC_OP_NONE: break;
        case TAC_OP_ADD:
        case TAC_OP_SUB:
        case TAC_OP_MUL:
        case TAC_OP_DIV:
        case TAC_OP_JUMPIFEQ:
        case TAC_OP_JUMPIFNEQ:
        case TAC_OP_IF_EQ:
        case TAC_OP_IF_LT:
        case TAC_OP_IF_GT:
        case TAC_OP_NOT:

            TAC_destroyOperand(&(instr->dest));
            TAC_destroyOperand(&(instr->src1));
            TAC_destroyOperand(&(instr->src2));
            return;

        // dest
        case TAC_OP_DEFVAR:
        case TAC_OP_JUMP:
        case TAC_OP_EXIT:
        case TAC_OP_WRITE:
        case TAC_OP_CALL:
        case TAC_OP_LABEL:

            TAC_destroyOperand(&(instr->dest));
            return;

        // dest, src1
        case TAC_OP_ASSIGN:
        case TAC_OP_TYPE:
        case TAC_OP_READ:

            TAC_destroyOperand(&(instr->dest));
            TAC_destroyOperand(&(instr->src1));
            return;

        // Pokud instrukce nepotřebuje žádný operand
        default:
            return;
    }
} // TAC_destroyInstruction*/

/**
 * @brief Testovací funkce pro generování tříadresného kódu.
 */
bool TAC_generateTestCode(){
    DString* konik = string_charToDString("konik");
    TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = konik, .type = TAC_OPERAND_VAR}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = konik, .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_EXIT, (TAC_Operand){.value.intValue = 42, .type = TAC_OPERAND_INT}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_EXIT, (TAC_Operand){.value.floatValue = 42.55556, .type = TAC_OPERAND_FLOAT}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = konik, .type = TAC_OPERAND_TEMP}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});

    string_free(konik);
    return true;
}

void TAC_generateProgram(){
    printf(".IFJcode24\n");

    printf("DEFVAR GF@?tempDEST\n");
    printf("DEFVAR GF@?tempSRC1\n");
    printf("DEFVAR GF@?tempSRC2\n");

    printf("CALL $$main\n");
    printf("JUMP $$end$$\n");

    AST_FunDefNode *node = ASTroot->functionList;
    while(node != NULL) {
        putchar('\n');
        TAC_generateFunctionDefinition(node);
        node = node->next;
    }
    putchar('\n');
    printf("LABEL $$end$$\n");
    putchar('\n');

}

void TAC_generateFunctionDefinition(AST_FunDefNode *funDefNode){
    printf("LABEL $$%s\n", funDefNode->identifier->str);
    printf("PUSHFRAME\n");

    TAC_generateStatementBlock(funDefNode->body);

    // Tady by šlo přidat podmínku, že se toto přidá jen u void funkce
    // Ostatní v sobě budou 
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void TAC_generateStatementBlock(AST_StatementNode* statement){
    while(statement != NULL){
        switch(statement->statementType) {
            case AST_STATEMENT_VAR_DEF:
                TAC_generateVarDef(statement->statement);
                break;
            case AST_STATEMENT_EXPR:
                TAC_generateExpression(statement->statement);
                break;
            case AST_STATEMENT_FUN_CALL:
                TAC_generateFunctionCall(statement->statement);
                break;
            case AST_STATEMENT_IF:
                TAC_generateIf(statement->statement);
                break;
            case AST_STATEMENT_WHILE:
                TAC_generateWhile(statement->statement);
                break;
            case AST_STATEMENT_RETURN:
                TAC_generateReturn(statement->statement);
                // Vracíme se z funkce
                return;
            default:
                error_handle(ERROR_INTERNAL);
        }

        statement = statement->next;
    }
}

void TAC_generateBinaryOperator(AST_BinOpNode *bin_node){
    // Na vrcholu datového zásobníku dáme levý operand
    TAC_generateExpression(bin_node->left);
    // Na vrcholu datového zásobníku dáme pravý operand
    TAC_generateExpression(bin_node->right);

    // Pomocné proměnné je potřeba definovat mimo switch-case
    Semantic_Data type;
    AST_VarNode *var;

    switch (bin_node->op) {
    case AST_OP_ASSIGNMENT:
        var = (AST_VarNode *)bin_node->left->expression;
        printf("POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
        // Zahodíme hodnotu levé strany ze zásobníku
        printf("POPS GF@?tempDEST\n");
        break;
    case AST_OP_ADD:
        printf("ADDS\n");
        break;
    case AST_OP_SUBTRACT:
        printf("SUBS\n");
        break;
    case AST_OP_MULTIPLY:
        printf("MULS\n");
        break;
    case AST_OP_DIVIDE:
        // Musíme zjistit, zda se jedná o celočíselné nebo desetinné dělení
        if(semantic_analyseExpr(bin_node->right, &type, NULL) != 0) {
            error_handle(ERROR_INTERNAL);
        }

        if(type == SEM_DATA_INT) {
            printf("IDIVS\n");
        } else{
            printf("DIVS\n");
        }
        break;
    case AST_OP_EQUAL:
        printf("EQS\n");
        break;
    case AST_OP_NOT_EQUAL:
        printf("EQS\n");
        printf("NOTS\n");
        break;
    case AST_OP_LESS_THAN:
        printf("LTS\n");
        break;
    case AST_OP_GREATER_THAN:
        printf("GTS\n");
        break;
    case AST_OP_LESS_EQUAL:
        printf("POPS GF@?tempSRC2\n");
        printf("POPS GF@?tempSRC1\n");
        printf("LT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("EQ GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("ORS\n");
        break;
    case AST_OP_GREATER_EQUAL:
        printf("POPS GF@?tempSRC2\n");
        printf("POPS GF@?tempSRC1\n");
        printf("GT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("EQ GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("ORS\n");
        break;
    
    default:
        error_handle(ERROR_INTERNAL);
    }
}

void TAC_generateVarDef(AST_BinOpNode *bin_node){
    TAC_generateExpression(bin_node->right);
    AST_VarNode *var = (AST_VarNode *)bin_node->left->expression;
    printf("DEFVAR LF@%s$%lu$\n", var->identifier->str, var->frameID);
    // Nahrajeme hodnotu výrazu do proměnné
    printf("POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
}

void TAC_generateExpression(AST_ExprNode *expr){
    AST_VarNode *var = (AST_VarNode*)expr->expression;   /**< Pro literál a proměnnou */
    switch (expr->exprType) {
        case AST_EXPR_LITERAL:
            TAC_generateLiteral(var);
            break;
        case AST_EXPR_VARIABLE:
            printf("PUSHS LF@%s$%lu$\n", var->identifier->str, var->frameID);
            break;
        case AST_EXPR_BINARY_OP:
            TAC_generateBinaryOperator(expr->expression);
            break;
        case AST_EXPR_FUN_CALL:
            TAC_generateFunctionCall(expr->expression);
            break;
        default:
            error_handle(ERROR_INTERNAL);
    }
}

void TAC_generateLiteral(AST_VarNode *literal){
    DString *value = (DString*)literal->value;
    switch (literal->literalType) {
        case AST_LITERAL_INT:
            printf("PUSHS int@%d\n", *(int*)literal->value);
            break;
        case AST_LITERAL_FLOAT:
            printf("PUSHS float@%lf\n", *(double*)literal->value);
            break;
        case AST_LITERAL_STRING:
            printf("PUSHS string@%s\n", value->str);
            break;
        case AST_LITERAL_NULL:
            printf("PUSHS nil@nil\n");
            break;
        default:
            error_handle(ERROR_INTERNAL);
    }
}

void TAC_generateIf(AST_IfNode *if_node){
    static unsigned int count = 0;

    // Vyhodnotíme podmínku
    TAC_generateExpression(if_node->condition);
    // je normální nebo null condition?
    if(if_node->nullCondition == NULL) {
        printf("PUSHS bool@true\n");
        printf("JUMPIFNES if_else$%d\n", count);
    }else {
        // Výsledek podmínky dáme do proměnné
        printf("POPS GF@?tempSRC1\n");
        printf("JUMPIFEQ if_else$%d GF@?tempSRC1 nil@nil\n", count);
        // Definujeme id_bez_null
        DString *id_bez_null = if_node->nullCondition->identifier;
        printf("DEFVAR LF@%s$%lu$\n", id_bez_null->str, if_node->nullCondition->frameID);
        printf("MOVE LF@%s$%lu$ GF@?tempSRC1\n", id_bez_null->str, if_node->nullCondition->frameID);
    }
    
    // Generujeme tělo if
    TAC_generateStatementBlock(if_node->thenBranch);
    printf("JUMP if_end$%d\n", count);

    printf("LABEL if_else$%d\n", count);
    // Generujeme tělo else
    TAC_generateStatementBlock(if_node->elseBranch);
    printf("LABEL if_end$%d\n", count);
    count++;
}

void TAC_generateWhile(AST_WhileNode *while_node){
    static unsigned int count = 0;

    // Label začátku while
    printf("LABEL while_start$%d\n", count);
    // Vyhodnotíme podmínku
    TAC_generateExpression(while_node->condition);
    // je normální nebo null condition?
    if(while_node->nullCondition == NULL) {
        printf("PUSHS bool@true\n");
        printf("JUMPIFNES while_end$%d\n", count);
    }else {
        // Výsledek podmínky dáme do proměnné
        printf("POPS GF@?tempSRC1\n");
        printf("JUMPIFEQ while_end$%d GF@?tempSRC1 nil@nil\n", count);
        // Definujeme id_bez_null
        DString *id_bez_null = while_node->nullCondition->identifier;
        printf("DEFVAR LF@%s$%lu$\n", id_bez_null->str, while_node->nullCondition->frameID);
        printf("MOVE LF@%s$%lu$ GF@?tempSRC1\n", id_bez_null->str, while_node->nullCondition->frameID);
    }
    
    // Generujeme tělo while
    TAC_generateStatementBlock(while_node->body);
    printf("JUMP while_start$%d\n", count);
    printf("LABEL while_end$%d\n", count);

    count++;
}

void TAC_generateReturn(AST_ExprNode *expr){
    TAC_generateExpression(expr);
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void TAC_generateFunctionCall(AST_FunCallNode *funCallNode){
    // Vytvoříme temporary frame pro parametry funkce
    printf("CREATEFRAME\n");

    // Najdeme definici funkce
    SymtableItemPtr function;
    if(symtable_findItem(frameStack.bottom->frame, funCallNode->identifier, &function) != SYMTABLE_SUCCESS){
        error_handle(ERROR_INTERNAL);
    }
    SymtableFunctionData *functionData = function->data;    /**< Definovaná data funkce */
    
    AST_ArgOrParamNode *arg = funCallNode->arguments;       /**< Argumenty volání funkce */
    // Pro všechny parametry
    for(size_t i = 0; i < functionData->param_count; i++){
        // Na zásobník vyhodnotíme hodnotu parametru
        TAC_generateExpression(arg->expression->expression);

        // Vytvoříme instrukci DEFVAR pro parametr
        // Pokud je funkce built-in, tak se nepřidává frameID do názvu
        if(funCallNode->isBuiltIn) {
            printf("DEFVAR TF@%s\n", functionData->params[i].id->str);
            printf("POPS TF@%s\n", functionData->params[i].id->str);
        }else {
            printf("DEFVAR TF@%s$%lu$\n", functionData->params[i].id->str, functionData->body_frameID);
            printf("POPS TF@%s$%lu$\n", functionData->params[i].id->str, functionData->body_frameID);
        }
    }

    // Přidáme skok na návěští funkce
    printf("CALL $$%s", funCallNode->identifier->str);
}

/*** Konec souboru tac_generator.c ***/
