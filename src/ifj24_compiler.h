/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ifj24_compiler.h                                          *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                   David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            06.11.2024                                                *
 * Poslední změna:   25.11.2024                                                *
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
 * @author David Krejčí \<xkrejcd00>
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


/*******************************************************************************
 *                                                                             *
 *                   FUNKCE "MAIN()" PŘEKLADAČE JAZYKA IFJ24                   *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Hlavní funkce překladače 'ifj24compiler' týmu "xkalinj00".
 *
 * @details Funkce `main` zajišťuje nejzákladnější řízení překladu. Spouští
 *          příslušné moduly překladače (parser, sémantický analyzátor a generátor
 *          mezikódu IFJ24code). V případě úspěšného dokončení překladu zajišťuje
 *          uvolnění všech v modulech alokovaných zdrojů.
 *
 * @return @c SUCCESS při úspěchu, jinak @c ErrorType hodnota při chybě.
 */
int main();

#endif  // MAIN_H_
