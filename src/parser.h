/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser.h                                                  *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
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
 * @file parser.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro sdílené funkce a proměnné parseru.
 * @details Tento soubor obsahuje deklarace globálních proměnných a funkcí,
 *          které využívají různé moduly parseru při syntaktické analýze
 *          jazyka IFJ24. Slouží jako centrální bod pro přístup ke sdíleným
 *          zdrojům, jako jsou aktuální token a kořen abstraktního syntaktického
 *          stromu (AST).
 */

#ifndef PARSER_H_
/** @cond  */
#define PARSER_H_
/** @endcond  */

#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "frame_stack.h"
#include "dynamic_string.h"
#include "lltable.h"
#include "ast_nodes.h"
#include "ast_interface.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define MAX_SIZE_T_DIGITS 20
#define FRAME_ID_SEPARATOR '$'


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální statická proměnná pro aktuální token, který je zpracováván.
 *
 * @details Tato proměnná obsahuje aktuální token, který je právě zpracováván
 *          parserem. Token je získáván pomocí funkce `scanner_getNextToken` a
 *          je aktulizován funkcí `parser_getNextToke`.
 *
 * @note Struktura bude na počátku inicializována na token typu @c EOF, který se
 *       jeví jako nejvíce neutrální volba. Obsažený ukazatel na @c DString bude
 *       inicializován na @c NULL.
 */
extern Token currentToken;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 *
 * @details Tato proměnná obsahuje kořenový uzel abstraktního syntaktického
 *          stromu (AST), který je vytvářen během syntaktické analýzy. AST je
 *          používán pro reprezentaci struktury programu a je generován parserem.
 *          Dále je využíván k sémantické analýze a generování 3AK.
 *
 * @note Ukazatel na kořen abstraktního syntaktického stromu bude před svou
 *       skutečnou inicializací inicializován na @c NULL.
 */
extern AST_ProgramNode *ASTroot;

/**
 * @brief Globální zásobník rámců používaný v celém překladači.
 *
 * @details Tato proměnná obsahuje zásobník rámců, který je používán během
 *          syntaktické a sémantické analýzy. Zásobník rámců umožňuje správu
 *          symbolických tabulek a dalších informací spojených s jednotlivými
 *          rámci. Každý rámec obsahuje tabulku symbolů, unikátní identifikátor
 *          a další informace potřebné pro analýzu a generování kódu.
 */
extern FrameStack stack;

/**
 * @brief Globální pole všech vytvořených rámců.
 *
 * @details Tato proměnná obsahuje pole všech rámců, které byly vytvořeny během
 *          syntaktické a sémantické analýzy. Pole rámců umožňuje rychlý přístup
 *          k jednotlivým rámcům a jejich správu. Každý rámec v poli obsahuje
 *          tabulku symbolů, unikátní identifikátor a další informace potřebné
 *          pro analýzu a generování kódu.
 */
extern FrameArray frameArray;


/*******************************************************************************
 *                                                                             *
 *                     DEKLARACE VEŘEJNÝCH FUNKCÍ PARSERU                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální současný token.
 *
 * @details Tato funkce volá @c scanner_getNextToken(), která vrací další token
 *          ze vstupního streamu. Globální proměnná @c currentToken je poté
 *          aktualizována na tento nový token.
 *
 * @note Funkce je deklarována jako @c inline.
 */
void Parser_pokeScanner();

/**
 * @brief Přidá suffix k jménu proměnné ve formátu @c $frameID$.
 *
 * @details Funkce připojí k původnímu jménu proměnné textový suffix @c $frameID$
 *          kde frameID je ID identifákoru příslušnému rámci. Tento formát
 *          suffixu zajišťuje, že jméno proměnné bude unikátní v rámci všech
 *          rámců. Pokud funkce obdrží ukazatel na neplatný DString či jeho
 *          obsah, volá funkce @c error_handle() s vnitřním @c ERROR_INTERNAL.
 *
 * @note Funkce využívá @c DString_appendChar() pro přidání suffixu.
 *
 * @param frameID Číselné ID rámce, které je připojeno jako suffix.
 * @param id Ukazatel na původní jméno proměnné typu @c DString, ke kterému
 *           bude suffix připojen.
 */
void Parser_addIdSuffix(size_t frameID, DString *id);


/*******************************************************************************
 *                                                                             *
 *                     DEKLARACE INTERNÍCH FUNKCÍ PARSERU                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Kontroluje úspěšnost připojení znaku do DString a případně hlásí chybu.
 *
 * @details Tato funkce kontroluje, zda operace připojení znaku do dynamického
 *          řetězce (DString) byla úspěšná. Pokud operace selhala, funkce zavolá
 *          @c error_handle() s chybovým kódem @c ERROR_INTERNAL.
 *
 * @param error Kód chyby vrácený operací připojení znaku do @c DString.
 *
 * @note Funkce je deklarována jako @c inline.
 */
void Parser_checkAppendSuccess(int error);

#endif  // PARSER_H_

/*** Konec souboru parser.h ***/
