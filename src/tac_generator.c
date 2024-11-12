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
 * @brief Vytvoří novou tříadresnou instrukci.
 */
TAC_Instruction *TAC_createInstruction(TAC_Operation op, TAC_Operand dest, TAC_Operand src1, TAC_Operand src2){
    // Inicializace listu instrukcí
    TAC_InstructionList *list = TAC_createInstructionList();
    if(list == NULL){
        return NULL;
    }
    // Alokace paměti pro novou instrukci
    TAC_Instruction *instr = malloc(sizeof(struct TAC_Instruction));
    if(instr == NULL){
        return NULL;
    }

    // Nastavení hodnot instrukce
    instr->op = op;
    instr->dest = dest;
    instr->src1 = src1;
    instr->src2 = src2;
    instr->next = NULL;

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
}

/*** Konec souboru tac_generator.c ***/
