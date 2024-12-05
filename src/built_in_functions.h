/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           built_in_functions.h                                      *
 * Autor:            Pavel Hýža   <xhyzapa00>                                  *
 *                   David Krejčí <xkrejcd00>                                  *
 *                   Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 *                                                                             *
 * Datum:            22.11.2024                                                *
 * Poslední změna:   29.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file built_in_functions.h
 * @author Hýža Pavel \<xhyzapa00>
 * @author David Krejčí \<xkrejcd00>
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro generování některých vestavěných funkcí jazyka
 *        IFJ24 v cílovém kódu (IFJcode24).
 * @details Tento soubor obsahuje deklaraci funkce pro generování někteých
 *          vestavěných funkcí jazyka IFJ24 pomocí assembler instrukcí
 *          poskytovaných jazykem IFJcode24 pro generátor kódu tac_generator.
 */

#ifndef BUILT_IN_H_
/** @cond  */
#define BUILT_IN_H_
/** @endcond  */

// Import standardních knihoven jazyka C
#include <stdio.h>


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE VEŘEJNÝCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Generuje vestavěné funkce pro cílový kód IFJcode24.
 *
 * @details Tato funkce generuje kód pro vestavěné funkce, které jsou součástí
 *          jazyka IFJ24. Tyto funkce zahrnují:
 *          - @c ifj.substring()
 *          - @c ifj.strcmp()
 *          - @c ifj.ord()
 */
void TAC_builtInFunctions();

#endif // BUILT_IN_H_

/*** Konec souboru built_in_functions.h ***/
