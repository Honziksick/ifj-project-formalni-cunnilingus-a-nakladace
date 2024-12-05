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


/**
 * @brief Hlavní funkce překladače 'ifj24compiler' týmu "xkalinj00".
 */
int main() {
    // Spustíme parsování zdrojového souboru a vygeneruje AST
    LLparser_parseProgram();

    // Přestože by toto nemělo nikdy nastat, raději zkontrolujeme, že kořen AST je platný
    if(ASTroot != NULL) {
        // Spustíme sémantickou analýzu nad AST pomocí Symtable
        semantic_analyseProgram();

        // Spustíme generování mezikódu IFJ24code
        TAC_generateProgram();
    }
    // Pokud tento neočekávaný případ nastal, hlásíme interní chybu
    else {
        error_handle(ERROR_INTERNAL);
    }

    // Před úspěšným dokončením překladu uvolníme všechnu alokovanou paměť
    IFJ24Compiler_freeAllAllocatedMemory();

    // Překlad skončil úspěchem
    return SUCCESS;
} // konec main()

/*** Konec souboru ifj24_compiler.h ***/
