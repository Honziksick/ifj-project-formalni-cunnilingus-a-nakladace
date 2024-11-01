/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           frame_stack.c                                             *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                   XXXXX XXXXX  <xplagia00> (implementace)                   *
 *                                                                             *
 * Datum:            xx.xx.2024                                                *
 * Poslední změna:   xx.xx.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file frame_stack.c
 * @author Jan Kalina \<xkalinj00> (návrh, hlavičkový soubor)
 * @author David Krejčí \<xkrejcd00> (návrh)
 * @author XXXXX XXXXX \<xplagiat00> (implementace)
 *
 * @brief Implementační funkcí pro práci se zásobníkem rámců.
 * @details Tento implementační soubor obsahuje definice funkcí a datových struktur
 *          potřebných pro správu zásobníku rámců v překladači. Zásobník rámců
 *          je používán pro správu tabulek symbolů a jejich rámců během analýzy
 *          a generování kódu. Funkce v tomto souboru umožňují inicializaci,
 *          manipulaci a vyhledávání v zásobníku rámců.
 */

#include "frame_stack.h"
#include "symtable.h"
#include "dynamic_string.h"

// Globální zásobník rámců používaný v celém překladači.
FrameStack *frameStack;

/*** Konec souboru frame_stack.h ***/
