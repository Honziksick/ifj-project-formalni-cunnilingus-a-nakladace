/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ifj24_compiler.h                                          *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            06.11.2024                                                *
 * Poslední změna:   06.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ifj24_compiler.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro funkci `main()` překladače jazyka IFJ24.
 * @details Tento soubor obsahuje pouze deklaraci funkce main, která slouží jako
 *          vstupní bod programu a zajišťuje spuštění všech potřebných submodulů
 *          překladače pro jazyk IFJ24.
 */

#ifndef MAIN_H_
/** @cond  */
#define MAIN_H_
/** @endcond  */


#include "error.h"
#include "llparser.h"
#include "semantic_analyser.h"
#include "tac_generator.h"

/**
 * @brief Konstanta označující úspěšné dokončení překladu
 */
#define SUCCESS 0

/*******************************************************************************
 *                                                                             *
 *                   FUNKCE "MAIN()" PŘEKLADAČE JAZYKA IFJ24                   *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Hlavní funkce programu.
 * @details Funkce `main` zpracovává argumenty příkazové řádky, spouští
 *          inicializaci, překlad a uvolnění prostředků překladače pomocí
 *          volání příslušných funkcí jednotlivých submodulů překladače
 *
 * @param argc Počet argumentů příkazové řádky.
 * @param argv Pole řetězců s argumenty příkazové řádky.
 *
 * @return @c SUCCESS při úspěchu, jinak @c ErrorType hodnota při chybě.
 */
int main();

#endif  // MAIN_H_
