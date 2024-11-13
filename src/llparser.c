/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           llparser.c                                                *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            01.11.2024                                                *
 * Poslední změna:   13.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file llparser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace LL syntaktického analyzátoru.
 * @details Tento soubor obsahuje implementaci funkcí pro parsování neterminálů
 *          podle LL gramatiky. Umožňuje tvorbu uzlů abstraktního syntaktického
 *          stromu (AST) a zajišťuje přechody mezi stavy, aby byla zachována
 *          správná struktura kódu během syntaktické analýzy.
 */

#include "llparser.h"



/*** Konec souboru llparser.c ***/
