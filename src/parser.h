/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   10.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file precedence_table.h
 * @author Jan Kalina   \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro správu precedenční tabulky pro precedenční SA.
 * @details Tento hlavičkový soubor obsahuje deklarace funkcí a datových
 *          struktur potřebných pro správu precedenční tabulky pro precedenční SA.
 *          Precedenční tabulka obsahuje pravidla pro zpracování výrazů.
 */

#ifndef PARSER_H_
/** @cond  */
#define PARSER_H_
/** @endcond  */

#include "scanner.h"
#include "ast_nodes.h"
#include "ast_interface.h"

/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro aktuální token, který je zpracováván.
 *
 * @details Tato proměnná obsahuje aktuální token, který je právě zpracováván
 *          parserem. Token je získáván pomocí funkce `scanner_getNextToken` a
 *          je aktulizován funkcí `parser_getNextToke`.
 */
extern Token currentToken;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 *
 * @details Tato proměnná obsahuje kořenový uzel abstraktního syntaktického
 *          stromu (AST), který je vytvářen během syntaktické analýzy. AST je
 *          používán pro reprezentaci struktury programu a je generován parserem.
 *          Dále je využíván k sémantické analýze a generování 3AK.
 */
extern AST_ProgramNode *ASTroot;

#endif  // PARSER_H_

/*** Konec souboru precedence_table.h ***/
