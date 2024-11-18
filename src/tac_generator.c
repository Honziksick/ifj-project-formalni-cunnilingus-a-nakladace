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
    switch (operand->type){
        case TAC_OPERAND_NONE: return;
        case TAC_OPERAND_VAR: printf("LF@$%s", string_toConstChar(operand->value.varName)); return;
        case TAC_OPERAND_TEMP: printf("TF@$%s", string_toConstChar(operand->value.varName)); return;
        case TAC_OPERAND_LABEL: printf("_%s", string_toConstChar(operand->value.labelName)); return;
        case TAC_OPERAND_INT: printf("int@%d", operand->value.intValue); return;
        case TAC_OPERAND_FLOAT: printf("float@%a", operand->value.floatValue); return;
        case TAC_OPERAND_STRING: printf("string@%s", string_toConstChar(operand->value.varName)); return;
        case TAC_OPERAND_BOOL: printf("bool@%s", string_toConstChar(operand->value.varName)); return;
        case TAC_OPERAND_NIL: printf("nil@nil"); return;
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
        case TAC_OP_CREATEFRAME: printf("CREATEFRAME "); break; // CREATEFRAME
        case TAC_OP_PUSHFRAME: printf("PUSHFRAME "); break; // PUSHFRAME
        case TAC_OP_POPFRAME: printf("POPFRAME "); break; // POPFRAME
        case TAC_OP_DEFVAR: printf("DEFVAR "); break; // DEFVAR dest
        case TAC_OP_WRITE: printf("WRITE "); break; // WRITE src1
        case TAC_OP_READ: printf("READ "); break; // READ dest
        case TAC_OP_CONCAT: printf("CONCAT "); break; // CONCAT dest, src1, src2
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
 * @brief Vytvoří označení pro tělo programu.
 */
bool TAC_generateProgramCode(AST_ProgramNode *programNode, TAC_InstructionList *tacList){
    // Vytvoření instrukce pro hlavní funkci
    TAC_Instruction *instr;
    instr = TAC_createInstruction(TAC_OP_LABEL, (TAC_Operand){.value.labelName = programNode->importedFile}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_CREATEFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_PUSHFRAME, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("GF@$retval")}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("GF@$retval")}, (TAC_Operand){.type = TAC_OPERAND_NIL}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
    return true;
} // TAC_generateProgramCode

/**
 * @brief Testovací funkce pro generování tříadresného kódu.
 */
bool TAC_generateTestCode(TAC_InstructionList *tacList){
    TAC_Instruction *instr;
    instr = TAC_createInstruction(TAC_OP_DEFVAR, (TAC_Operand){.value.varName = string_charToDString("kokot"), .type = TAC_OPERAND_STRING}, (TAC_Operand){.type = TAC_OPERAND_NONE}, (TAC_Operand){.type = TAC_OPERAND_NONE});
    TAC_appendInstruction(tacList, instr);
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

/*** Konec souboru tac_generator.c ***/
