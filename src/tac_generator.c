/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.c                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   15.11.2024                                                *
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
#include "error.h"

TAC_InstructionList *tacList = NULL; // Globální proměnná pro seznam instrukcí

/**
 * @brief Inicializuje nový prázdný seznam tříadresných instrukcí.
 */
inline void TAC_createInstructionList(){
    // Alokace paměti pro seznam instrukcí
    tacList = (TAC_InstructionList *)malloc(sizeof(struct TAC_InstructionList));
    if (tacList == NULL){
        error_handle(ERROR_INTERNAL);
    }

    // Inicializace seznamu
    tacList->head = NULL;
    tacList->tail = NULL;
}

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
TAC_Instruction *TAC_createInstruction(TAC_Operation op, TAC_Operand dest, TAC_Operand src1, TAC_Operand src2){
    // Alokace paměti a inicializace pro novou instrukci
    TAC_Instruction *instr = TAC_initInstruction();
    if(instr == NULL){
        return NULL;
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
            return instr;

        case TAC_OP_ASSIGN:
        case TAC_OP_READ:
        case TAC_OP_TYPE:
        case TAC_OP_NOT:
            instr->src1 = src1;
            instr->dest = dest;
            return instr;

        case TAC_OP_PARAM:
        case TAC_OP_CALL:
        case TAC_OP_LABEL:
        case TAC_OP_JUMP:
        case TAC_OP_EXIT:
        case TAC_OP_WRITE:
        case TAC_OP_DEFVAR:
            instr->dest = dest;
            return instr;

        default:
            return instr;
    }
} // TAC_createInstruction

/**
 * @brief Přidá instrukci na konec seznamu instrukcí.
 */
void TAC_appendInstruction(TAC_Instruction *instr){
    // Přidání první instrukce do seznamu
    if(tacList->head == NULL){
        tacList->head = instr;
        tacList->tail = instr;
    }
    // Přidání dalších instrukcí na konec seznamu
    else{
        tacList->tail->next = instr;
        tacList->tail = instr;
        // Konec ukazuje na NULL
        tacList->tail->next = NULL;
    }
} // TAC_appendInstruction

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
            case TAC_OPERAND_FLOAT: printf("float@%a", operand->value.floatValue); break;
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
            return;

        // dest
        case TAC_OP_DEFVAR:
        case TAC_OP_JUMP:
        case TAC_OP_EXIT:
        case TAC_OP_WRITE:
        case TAC_OP_CALL:
        case TAC_OP_LABEL:
            TAC_printOperand(&(instr->dest));
            return;

        // dest, src1
        case TAC_OP_ASSIGN:
        case TAC_OP_TYPE:
        case TAC_OP_READ:
            TAC_printOperand(&(instr->dest));
            printf(" ");
            TAC_printOperand(&(instr->src1));
            return;

        // Pokud instrukce nepotřebuje žádný operand
        default:
            return;
    }
}// TAC_printInstruction

/**
 * @brief Vytiskne všechny instrukce v seznamu tříadresného kódu.
 */
void TAC_printInstructionList(){
    // Pokud je seznam prázdný, nemáme co tisknout
    if(tacList->head == NULL){
        return;
    }

    // Postupné tisknutí instrukcí od hlavičky
    TAC_Instruction *current = tacList->head;

    // Dokud máme, co tisknout
    while(current != NULL){
        TAC_printInstruction(current);
        printf("\n");
        current = current->next;
    }
} // TAC_printInstructionList

/**
 * @brief Vytvoří označení pro začátek programu.
 */
bool TAC_generateProgramCodeBegin(AST_ProgramNode *programNode){
    // Vytvoření instrukce pro hlavní funkci
    TAC_Instruction *instr;

    // Globální proměnné pro práci s operacemi, které ukládají výsledek do proměnné
    // ?tempDEST, ?tempSRC1, ?tempSRC2 --> používá se ? pro zamezení kolize s názvy proměnných
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_GLOBAL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("?tempSRC1"), .type = TAC_OPERAND_GLOBAL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("?tempSRC2"), .type = TAC_OPERAND_GLOBAL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);

    // Vytvoření rámce pro tělo programu
    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = string_charToDString(programNode->importedFile->identifier->str), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_PUSHFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    return true;
} // TAC_generateProgramCode

bool TAC_generateProgramCodeEnd(){
    TAC_Instruction *instr;
    // Přesun rámce ze zásobníku do globálního rámce
    instr = TAC_createInstruction(TAC_OP_POPFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    // Vyčistíme zásobník rámců
    instr = TAC_createInstruction(TAC_OP_CLEARS, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    return true;
} // TAC_generateProgramCode

/**
 * @brief Generuje kód pro začátek definice funkce.
 */
bool TAC_generateFunctionDefinitionBegin(AST_FunDefNode *funDefNode){
    TAC_Instruction *instr;
    // Label funkce
    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = string_charToDString(funDefNode->identifier->str), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    return true;
} // TAC_generateFunctionDefinitionBegin

/**
 * @brief Generuje kód pro konec definice funkce.
 */
bool TAC_generateFunctionDefinitionEnd(){
    TAC_Instruction *instr;

    instr = TAC_createInstruction(TAC_OP_POPFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    // Návrat do místa, kde bylo zavoláno call
    instr = TAC_createInstruction(TAC_OP_RETURN, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    return true;
} // TAC_generateFunctionDefinitionEnd

/**
 * @brief Generuje tříadresný kód pro binární operace.
 */
bool TAC_generateBinOp(AST_BinOpNode *binOp){
    TAC_Instruction *instr;
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

    if(leftOperand->exprType == AST_EXPR_VARIABLE){
        operandUsedLeft.type = TAC_OPERAND_VAR;
        operandUsedLeft.value.varName = ((AST_VarNode *)leftOperand->expression)->identifier;
    }

    if(rightOperand->exprType == AST_EXPR_VARIABLE){
        operandUsedRight.type = TAC_OPERAND_VAR;
        operandUsedRight.value.varName = ((AST_VarNode *)rightOperand->expression)->identifier;
    }

    // Vytvoření instrukce pro každý binární operátor
    // ?tempDEST ==> globální proměnnám, do které ukládáme výsledek operací
    switch (binOp->op){
        case AST_OP_ADD:
            instr = TAC_createInstruction(TAC_OP_ADD, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_SUBTRACT:
            instr = TAC_createInstruction(TAC_OP_SUB, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_MULTIPLY:
            instr = TAC_createInstruction(TAC_OP_MUL, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_DIVIDE:
            instr = TAC_createInstruction(TAC_OP_DIV, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_ASSIGNMENT:
            instr = TAC_createInstruction(TAC_OP_ASSIGN, operandUsedLeft, operandUsedRight, (TAC_Operand){.type = TAC_OPERAND_NONE});
            TAC_appendInstruction(instr);
            break;

        case AST_OP_EQUAL:
            instr = TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_NOT_EQUAL:
            instr = TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            instr = TAC_createInstruction(TAC_OP_NOT, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.type = TAC_OPERAND_NONE});
            TAC_appendInstruction(instr);
            break;

        case AST_OP_GREATER_THAN:
            instr = TAC_createInstruction(TAC_OP_IF_GT, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_LESS_THAN:
            instr = TAC_createInstruction(TAC_OP_IF_LT, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            break;

        case AST_OP_GREATER_EQUAL:
            instr = TAC_createInstruction(TAC_OP_IF_GT, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            instr = TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = string_charToDString("?tempSRC1"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            instr = TAC_createInstruction(TAC_OP_OR, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("?tempSRC1"), .type = TAC_OPERAND_TEMP});
            TAC_appendInstruction(instr);
            break;

        case AST_OP_LESS_EQUAL:
            instr = TAC_createInstruction(TAC_OP_IF_LT, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            instr = TAC_createInstruction(TAC_OP_IF_EQ, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, operandUsedLeft, operandUsedRight);
            TAC_appendInstruction(instr);
            instr = TAC_createInstruction(TAC_OP_OR, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("?tempDEST"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("?tempSRC1"), .type = TAC_OPERAND_TEMP});
            TAC_appendInstruction(instr);
            break;

        default: break;
    }

    return true;
}

/**
 * @brief Generuje tříadresný kód pro výrazy.
 */
bool TAC_generateExpression(AST_ExprNode *exprNode){
    TAC_Instruction *instr;
    // Přidělení binárního operátoru z výrazu
    AST_BinOpNode *binOp = (AST_BinOpNode *)exprNode->expression;

    switch (exprNode->exprType){
        case AST_EXPR_BINARY_OP:
            TAC_generateBinOp(binOp);
            break;

        case AST_EXPR_FUN_CALL:
            instr = TAC_createInstruction(TAC_OP_CALL, (TAC_Operand){.value.labelName = string_charToDString(((AST_VarNode *)exprNode->expression)->identifier->str), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
            TAC_appendInstruction(instr);
            break;

        default: break;
    }
    return true;
} // TAC_generateStatement

/**
 * @brief Generuje tříadresný kód pro příkazy.
 */
bool TAC_generateStatement(AST_StatementNode *statement){
    TAC_Instruction *instr;

    switch (statement->statementType){
        case AST_STATEMENT_VAR_DEF:
            instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString(((AST_VarNode *)statement->statement)->identifier->str), .type = TAC_OPERAND_VAR}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
            TAC_appendInstruction(instr);
            break;

        default: break;
    }
    return true;
}

/**
 * @brief Uvolní operand.
*/
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

/*
 * @brief Uvolní instrukci.
 */
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
} // TAC_destroyInstruction


/**
 * @brief Uvolní všechny instrukce v seznamu tříadresného kódu.
 */
void TAC_freeInstructionList(){
    // Pokud je seznam prázdný, nemáme, co mazat
    if(tacList->head == NULL){
        return;
    }

    // Dokud máme, co mazat
    while(tacList->head != NULL){
        // Postupné mazání instrukcí od hlavičky
        TAC_Instruction *current = tacList->head;

        tacList->head = tacList->head->next;

        //TAC_destroyInstruction(current);
        free(current);
    }
} // TAC_freeInstructionList

/**
 * @brief Uvolní všechny instrukce v seznamu tříadresného kódu a potom samotný seznam.
 */
void TAC_destroyInstructionList(){
    if (tacList == NULL){
        return;
    }
    TAC_freeInstructionList(tacList);
    free(tacList);
    //tacList = NULL;
} // TAC_destroyInstructionList

/**
 * @brief Testovací funkce pro generování tříadresného kódu.
 */
bool TAC_generateTestCode(){
    TAC_Instruction *instr;
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("konik"), .type = TAC_OPERAND_VAR}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = string_charToDString("pejsek"), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_EXIT, (TAC_Operand){.value.intValue = 42, .type = TAC_OPERAND_INT}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_EXIT, (TAC_Operand){.value.floatValue = 42.55556, .type = TAC_OPERAND_FLOAT}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("temp_konik"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    return true;
}

bool TAC_generateFunCall(AST_FunCallNode *funCallNode){
    TAC_Instruction *instr;
    // Vytvoříme temporary frame pro parametry funkce
    instr = TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);

    // Najdeme definici funkce
    SymtableItemPtr function;
    if(symtable_findItem(frameStack.bottom->frame, funCallNode->identifier, &function) != SYMTABLE_SUCCESS){
        return false;
    }
    SymtableFunctionData *functionData = function->data;    /**< Definovaná data funkce */
    
    AST_ArgOrParamNode *arg = funCallNode->arguments;       /**< Argumenty volání funkce */
    // Pro všechny parametry
    for(size_t i = 0; i < functionData->param_count; i++){
        // Vytvoříme instrukci DEFVAR pro parametr
        instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = functionData->params[i].id}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
        TAC_appendInstruction(instr);
        AST_VarNode *var_node = (AST_VarNode*)arg->expression->expression;
        // Do nově definováné proměnné vložíme hodnotu
        // Vložíme hodnotu literálu
        if(var_node->identifier == NULL){
            TAC_Operand src;
            switch (var_node->literalType)
            {
            case AST_LITERAL_INT:
                src = (TAC_Operand){.type = TAC_OPERAND_INT, .value.intValue = *(int*)var_node->value};
                break;
            case AST_LITERAL_FLOAT:
                src = (TAC_Operand){.type = TAC_OPERAND_FLOAT, .value.floatValue = *(double*)var_node->value};
                break;
            case AST_LITERAL_STRING:
                src = (TAC_Operand){.type = TAC_OPERAND_STRING, .value.varName = var_node->value};
                break;
            case AST_LITERAL_NULL:
                src = (TAC_Operand){.type = TAC_OPERAND_NIL, .value.varName = NULL};
                break;
            
            default:
                return false;
            }
            instr = TAC_createInstruction(TAC_OP_ASSIGN, (TAC_Operand){.value.varName = functionData->params[i].id, .type = TAC_OPERAND_VAR}, src, (TAC_Operand){.type = TAC_OPERAND_NONE});
        }
        // Vložíme hodnotu proměnné ve volání funkce
        else {
            instr = TAC_createInstruction(TAC_OP_ASSIGN, (TAC_Operand){.value.varName = functionData->params[i].id, .type = TAC_OPERAND_VAR}, (TAC_Operand){.value.varName = var_node->identifier, .type = TAC_OPERAND_VAR}, (TAC_Operand){.type = TAC_OPERAND_NONE});

        }
        TAC_appendInstruction(instr);
    }
    // Přidáme skok na návěští funkce
    instr = TAC_createInstruction(TAC_OP_JUMP, (TAC_Operand){.value.labelName = funCallNode->identifier, .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(instr);
    return true;    //Vracíme true při úspěchu
}

/*** Konec souboru tac_generator.c ***/
