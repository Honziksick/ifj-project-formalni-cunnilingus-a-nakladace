/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.c                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   12.11.2024                                                *
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
 * @brief Implementace funkcí pro generátor vnitřního kódu (3AK).
 * @details XXX
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
    // Inicializace listu instrukcí
    TAC_InstructionList *list = TAC_createInstructionList();
    if(list == NULL){
        return NULL;
    }
    // Alokace paměti a inicializace pro novou instrukci
    TAC_Instruction *instr = TAC_initInstruction();
    if(instr == NULL){
        return NULL;
    }

    instr->op = op;

    // Switch pro nastavení operandů instrukce pro jednotlivé operace
    switch (instr->op){
        case TAC_OP_NONE: break;
        case TAC_OP_RETURN: break;
        case TAC_OP_ADD:
        case TAC_OP_SUB:
        case TAC_OP_MUL:
        case TAC_OP_DIV:
        case TAC_OP_IF_EQ:
        case TAC_OP_IF_NEQ:
        case TAC_OP_IF_LT:
        case TAC_OP_IF_LE:
        case TAC_OP_IF_GT:
        case TAC_OP_IF_GE:
            instr->src1 = src1;
            instr->src2 = src2;
            instr->dest = dest;
            break;
        case TAC_OP_ASSIGN:
            instr->src1 = src1;
            instr->dest = dest;
            break;
        case TAC_OP_PARAM:
        case TAC_OP_CALL:
        case TAC_OP_LABEL:
            instr->src1 = src1;
            break;
        case TAC_OP_GOTO:
            instr->dest = dest;
            break;

        default:
            return instr;
    }

    // Vracíme ukaatel na novou instrukci
    return instr;
} // TAC_createInstruction

/**
 * @brief Přidá instrukci na konec seznamu instrukcí.
 */
void TAC_appendInstruction(TAC_InstructionList *list, TAC_Instruction *instr){
    // Přidání první instrukce do seznamu
    if(list->head == NULL){
        list->head = instr;
        list->tail = instr;
    }
    // Přidání dalších instrukcí na konec seznamu
    else{
        list->tail->next = instr;
    }
} // TAC_appendInstruction

/**
 * @brief Uvolní všechny instrukce v seznamu tříadresného kódu.
 */
void TAC_freeInstructionList(TAC_InstructionList *list){
    // Pokud je seznam prázdný, nemáme, co mazat
    if(list->head == NULL){
        return;
    }

    // Postupné mazání instrukcí od hlavičky
    TAC_Instruction *current = list->head;

    // Dokud máme, co mazat
    while(current != NULL){
        TAC_Instruction *next = current->next;
        free(current);
        current = next;
    }
} // TAC_freeInstructionList

/**
 * @brief Generuje tříadresný kód pro definici funkce.

bool TAC_generateFunctionDefinition(AST_FunDefNode *funDefNode, TAC_InstructionList *tacList){
    // Vytvoření nového seznamu instrukcí pro tělo funkce
    funDefNode = AST_createFunDefNode();

    if(funDefNode == NULL){
        return false;
    }

    // Prázdný operand pro nevyužití
    TAC_Operand undef;
    // Instrukce
    TAC_Instruction *instr;

    instr = TAC_createInstruction(TAC_OP_PARAM, undef, funDefNode->, undef);
            TAC_appendInstruction(tacList, instr);
    return true;
} */

/**
 * @brief Generuje tříadresný kód pro proměnnou.
 */
/*bool TAC_generateVariable(AST_VarNode *varNode, TAC_InstructionList *tacList){
    // Prázdný operand pro nevyužití
    TAC_Operand undef, src1, src2;
    // Instrukce
    TAC_Instruction *instr;

    instr = TAC_createInstruction(TAC_OP_ASSIGN, undef, src1, src2);
    TAC_appendInstruction(tacList, instr);
    return true;
}*/

/*** Konec souboru tac_generator.c ***/
