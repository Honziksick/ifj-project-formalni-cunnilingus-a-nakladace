/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.h                                       *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            11.11.2024                                                *
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
 * @file semantic_analyser.h
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Hlavičkový soubor pro sémantický analyzátor.
 * @details Obsahuje definice funkcí pro sémantickou analýzu
 */

#ifndef SEMANTIC_ANALYSER_H_
/** @cond  */
#define SEMANTIC_ANALYSER_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "frame_stack.h"
#include "symtable.h"
#include "error.h"

/**
 * @brief Provede sémantickou analýzu celého programu.
 * 
 * @details Volá funkce pro dílčí sémantické analýzy programu:
 *          - semantic_checkProgramStructure()
 *          - semantic_checkFunctionDefinitions()
 *          - semantic_checkVariables()
 */
void semantic_analyseProgram();


/**
 * @brief Provede sémantickou analýzu základní struktury programu
 * 
 * @details Ověří, že existuje funkce main s návratovým typem void a bez parametrů
 *          Dále ověří sprvánost literálů v prologu programu
 * 
 * @return `true`, pokud je sémantická analýza struktury programu v pořádku, jinak `false`
 */
ErrorType semantic_analyseProgramStructure();


/**
 * @brief Provede sémantickou analýzu definic funkcí
 * 
 * @details Projde strom a pro každou definici funkce zavolá sémantickou sondu
 *          ()
 * 
 * @return `true`, pokud je sémantická analýza funkcí v pořádku, jinak `false`
 */
ErrorType semantic_analyseFunctionDefinitions();


/**
 * @brief Provede sémantickou analýzu všech proměnných v programu
 * 
 * @details Projde všechny rámce v globálním poli a zkontroluje,
 *          že každá proměnná je využita,
 *          že každá nekonstantní proměnná je změněna,
 *          že žádná konstantní proměnná není změněna
 * 
 * @return `true`, pokud je sémantická analýza proměnných v pořádku, jinak `false`
 */
ErrorType semantic_analyseVariables();


/**
 * @brief Provede sémantickou analýzu bloku funkce
 * 
 * @details Prochází blok příkaz po příkazu a podle typu volá další
 *          pomocné funkce podle typu příkazu.
 *          
 */
ErrorType semantic_probeFunction(AST_FunDefNode *node);

/**
 * @brief 
 */
ErrorType semantic_probeBlock(DString *FunId, AST_StatementNode *statement, bool* returned);

ErrorType semantic_analyseVarDef();

ErrorType semantic_analyseFunCallStatement();

ErrorType semantic_analyseIf();

ErrorType semantic_analyseWhile();

ErrorType semantic_analyseReturn();
#endif // SEMANTIC_ANALYSER_H_

/*** Konec souboru semantic_analyser.h ***/
