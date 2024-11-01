/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_interface.c                                           *
 * Autor:            XXXX XXXXX   <xplagia00>                                  *
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
 * @file ast_interface.c
 * @author XXXX XXXXX \<xplagia00>
 *
 * @brief Implementace funkcí pro tvorbu a destrukci uzlů AST.
 *
 * @details Tento soubor obsahuje definice funkcí pro vytváření a rušení uzlů
 *          abstraktního syntaktického stromu (AST) pro překladač IFJ24. Každá
 *          funkce zajišťuje správnou inicializaci nebo uvolnění paměti pro
 *          příslušný typ uzlu AST, což usnadňuje práci s datovými strukturami
 *          v rámci sémantické analýzy a generování kódu.
 */

#include "ast_nodes.h"
#include "ast_interface.h"

/*** Konec souboru ast_interface.c ***/
