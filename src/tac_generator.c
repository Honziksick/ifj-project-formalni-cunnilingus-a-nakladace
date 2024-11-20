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

/**
 * @brief Inicializuje nový prázdný seznam tříadresných instrukcí.
 */
TAC_InstructionList *TAC_createInstructionList(){
    // Alokace paměti pro seznam instrukcí
    TAC_InstructionList *list = malloc(sizeof(struct TAC_InstructionList));
    if (list == NULL){
        return NULL;
    }

    // Inicializace seznamu
    list->head = NULL;
    list->tail = NULL;

    return list;
}

/**
 * @brief Vytvoří a inicializuje novou tříadresnou instrukci na základní hodnoty.
 */
TAC_Instruction *TAC_initInstruction(){
    TAC_Instruction *instr = malloc(sizeof(struct TAC_Instruction));
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
        case TAC_OP_NOT:
        case TAC_OP_CONCAT:
            instr->src1 = src1;
            instr->src2 = src2;
            instr->dest = dest;
            return instr;

        case TAC_OP_ASSIGN:
        case TAC_OP_READ:
        case TAC_OP_TYPE:
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
void TAC_appendInstruction(TAC_InstructionList *tacList, TAC_Instruction *instr){
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
            default: break;
        }

        // Uvolnění alokovaného řetězce
        free(stringHelpVar);
        free(stringHelpLabel);
    }
    else{

        switch (operand->type){
            case TAC_OPERAND_INT: printf("int@%d", operand->value.intValue); break;
            case TAC_OPERAND_FLOAT: printf("float@%f", operand->value.floatValue); break;
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
void TAC_printInstructionList(TAC_InstructionList *tacList){
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
bool TAC_generateProgramCodeBegin(AST_ProgramNode *programNode, TAC_InstructionList *tacList){
    // Vytvoření instrukce pro hlavní funkci
    TAC_Instruction *instr;

    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = string_charToDString(programNode->importedFile->identifier->str), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_PUSHFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    return true;
} // TAC_generateProgramCode

bool TAC_generateProgramCodeEnd(TAC_InstructionList *tacList){
    TAC_Instruction *instr;
    // Přesun rámce ze zásobníku do globálního rámce
    instr = TAC_createInstruction(TAC_OP_POPFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    // Vyčistíme zásobník rámců
    instr = TAC_createInstruction(TAC_OP_CLEARS, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    return true;
} // TAC_generateProgramCode

/**
 * @brief Generuje kód pro začátek definice funkce.
 */
bool TAC_generateFunctionDefinitionBegin(AST_FunDefNode *funDefNode, TAC_InstructionList *tacList){
    TAC_Instruction *instr;
    // Label funkce
    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = string_charToDString(funDefNode->identifier->str), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    return true;
} // TAC_generateFunctionDefinitionBegin

/**
 * @brief Generuje kód pro instrukce uvnitř definované funkce.

bool TAC_generateFunctionDefinitionMiddle(AST_FunDefNode *funDefNode, TAC_InstructionList *tacList){
    TAC_Instruction *instr;
    // Vytvoření instrukce pro každý uzel v těle funkce
    switch (funDefNode->body->type){

        // ###################### TODO ######################
        // Zbylé typy nodes
        case AST_FUN_DEF_NODE: break;
        case AST_IF_NODE: break;
        case AST_WHILE_NODE: break;
        case AST_EXPR_NODE: break;
        case AST_BIN_OP_NODE: break;
        case AST_LITERAL_NODE: break;
        case AST_VAR_NODE: break;

        default: break;
    }
    return true;
} // TAC_generateFunctionDefinitionMiddle */

bool TAC_generateFunctionDefinitionEnd(TAC_InstructionList *tacList){
    TAC_Instruction *instr;

    instr = TAC_createInstruction(TAC_OP_POPFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    // Návrat do místa, kde bylo zavoláno call
    instr = TAC_createInstruction(TAC_OP_RETURN, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    return true;
} // TAC_generateFunctionDefinitionEnd

/**
 * @brief Generuje tříadresný kód pro přiřazení.
 */
bool TAC_generateBinOp(AST_BinOpNode *binOp, AST_VarNode *varNode, TAC_InstructionList *tacList){
    TAC_Instruction *instr;
    AST_ExprNode *leftOperand = binOp->left;
    AST_ExprNode *rightOperand = binOp->right;
    AST_VarNode *leftVar = varNode;
    AST_VarNode *rightVar = varNode;
    leftVar->identifier = ((AST_VarNode *)leftOperand->expression)->identifier;
    rightVar->identifier = ((AST_VarNode *)rightOperand->expression)->identifier;
    // Vytvoření instrukce pro každý binární operátor
    switch (binOp->op){
        case AST_OP_ADD:
            instr = TAC_createInstruction(TAC_OP_ADD, (TAC_Operand){.value.varName = string_charToDString("temp"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = leftVar->identifier, .type = TAC_OPERAND_VAR}, (TAC_Operand){.value.varName = rightVar->identifier, .type = TAC_OPERAND_VAR});
            TAC_appendInstruction(tacList, instr);
            break;
        case AST_OP_SUBTRACT:
            instr = TAC_createInstruction(TAC_OP_SUB, (TAC_Operand){.value.varName = string_charToDString("temp"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("temp1"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("temp2"), .type = TAC_OPERAND_TEMP});
            TAC_appendInstruction(tacList, instr);
            break;
        case AST_OP_MULTIPLY:
            instr = TAC_createInstruction(TAC_OP_MUL, (TAC_Operand){.value.varName = string_charToDString("temp"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("temp1"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("temp2"), .type = TAC_OPERAND_TEMP});
            TAC_appendInstruction(tacList, instr);
            break;
        case AST_OP_DIVIDE:
            instr = TAC_createInstruction(TAC_OP_DIV, (TAC_Operand){.value.varName = string_charToDString("temp"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("temp1"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.value.varName = string_charToDString("temp2"), .type = TAC_OPERAND_TEMP});

        default: break;
    }
    return true;
}

/**
 * @brief Generuje tříadresný kód pro výrazy.
 */
bool TAC_generateExpresion(AST_BinOpNode *binOp, AST_ExprNode *exprNode, AST_VarNode *varNode, TAC_InstructionList *tacList){
    //TAC_Instruction *instr;

    switch (exprNode->exprType){
        case AST_EXPR_BINARY_OP:
            TAC_generateBinOp(binOp, varNode, tacList);
        default: break;
    }
    return true;
} // TAC_generateStatement

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
void TAC_freeInstructionList(TAC_InstructionList *tacList){
    // Pokud je seznam prázdný, nemáme, co mazat
    if(tacList->head == NULL){
        return;
    }

    // Dokud máme, co mazat
    while(tacList->head != NULL){
        // Postupné mazání instrukcí od hlavičky
        TAC_Instruction *current = tacList->head;

        tacList->head = tacList->head->next;

        TAC_destroyInstruction(current);
        free(current);
    }
} // TAC_freeInstructionList

/**
 * @brief Uvolní všechny instrukce v seznamu tříadresného kódu a potom samotný seznam.
 */
void TAC_destroyInstructionList(TAC_InstructionList *tacList){
    TAC_freeInstructionList(tacList);
    free(tacList);
} // TAC_destroyInstructionList

/**
 * @brief Testovací funkce pro generování tříadresného kódu.
 */
bool TAC_generateTestCode(TAC_InstructionList *tacList){
    TAC_Instruction *instr;
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("konik"), .type = TAC_OPERAND_VAR}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = string_charToDString("pejsek"), .type = TAC_OPERAND_LABEL}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_EXIT, (TAC_Operand){.value.intValue = 42, .type = TAC_OPERAND_INT}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_EXIT, (TAC_Operand){.value.floatValue = 42.55556, .type = TAC_OPERAND_FLOAT}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("temp_konik"), .type = TAC_OPERAND_TEMP}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    return true;
}

/*** Konec souboru tac_generator.c ***/
