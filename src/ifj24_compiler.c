/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ifj24_compiler.c                                          *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            06.11.2024                                                *
 * Poslední změna:   22.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ifj24_compiler.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementační soubor pro funkci `main()` překladače jazyka IFJ24.
 * @details Tento soubor obsahuje implementaci funkce `main()`, která slouží
 *          jako vstupní bod programu a zajišťuje spuštění všech potřebných
 *          submodulů překladače pro jazyk IFJ24.
 */


#include "ifj24_compiler.h"

int main() {

    LLparser_parseProgram();
    if(ASTroot != NULL) {
        semantic_analyseProgram();
        TAC_createInstructionList();
        TAC_generateProgramCodeBegin(ASTroot);

        // Projdeme všechny funkce a vygenerujeme kód pro každou z nich
        AST_FunDefNode *current = ASTroot->functionList;
        while(current != NULL){

            TAC_generateFunctionDefinitionBegin(current);

            current = current->next;
        }

        TAC_generateFunctionDefinitionEnd();
        TAC_generateProgramCodeEnd();

        TAC_printInstructionList();
    }

    IFJ24Compiler_freeAllAllocatedMemory();

    return SUCCESS;
}
