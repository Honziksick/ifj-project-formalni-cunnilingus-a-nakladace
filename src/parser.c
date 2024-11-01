/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser.c                                                  *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            01.11.2024                                                *
 * Poslední změna:   01.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file parser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace funkcí pro syntaktický analyzátor.
 * @details Tento soubor obsahuje implementaci funkcí pro syntaktickou analýzu
 *          jednotlivých neterminálů gramatiky jazyka IFJ24. Funkce vytvářejí
 *          uzly abstraktního syntaktického stromu (AST) a zajišťují správný
 *          přechod mezi stavy podle LL gramatiky. Implementace vychází ze
 *          specifikace gramatiky a podporuje všechny pravidla a operace
 *          potřebné pro parsování zdrojového kódu.
 */

#include "parser.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "scanner.h"
#include "symtable.h"
#include "lltable.h"
#include "error.h"

/**
 * @brief Globální proměnná pro aktuální token, který je zpracováván.
 */
Token currentToken;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot;



/*** Konec souboru parser.c ***/
