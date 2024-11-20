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
#include <stdbool.h>
#include "scanner.h"
#include "lltable.h"
#include "precedence_table.h"
#include "frame_stack.h"
#include "dynamic_string.h"
#include "ast_nodes.h"
#include "ast_interface.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define MAX_SIZE_T_DIGITS 20        /**< Maximální počet číslic, které mohou být použity pro reprezentaci hodnoty typu `size_t`. */
#define FRAME_ID_SEPARATOR '$'      /**< Znak používaný jako odělovač ID rámce od názvu proměnné v jejím identifikátoru. */


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura reprezentující terminál.
 *
 * @details Tato struktura obsahuje informace o terminálu, který je zpracováván
 *          parserem. Struktura obsahuje typ terminálu a hodnotu terminálu.
 *          Typ terminálu je reprezentován výčtovým typem @c LLTerminals a
 *          hodnota terminálu je ukazatel na dynamický řetězec @c DString.
 */
typedef struct Terminal {
    LLTerminals   LLterminal;       /**< Typ LL terminálu pro LL parser. */
    PrecTerminals PrecTerminal;     /**< Typ LL terminálu pro precedenční parser. */
    DString       *value;           /**< Hodnota terminálu reprezentovaná ukazatelem na dynamický řetězec `DString`. */
} Terminal;

/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro aktuální token, který je zpracováván.
 *
 * @details Tato proměnná obsahuje aktuální token, který je právě zpracováván
 *          parserem. Token je získáván pomocí funkce @c scanner_getNextToken()
 *          a je aktualizován funkcí @c Parser_pokeScanner(), která implicitně
 *          přemapovává typ tokenu @c TokenType na typ LL terminálu @c LLTerminals.
 *          Struktura @c Terminal obsahuje typ terminálu a hodnotu terminálu,
 *          což umožňuje parseru efektivně zpracovávat různé typy tokenů.
 */
extern Terminal currentToken;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 *
 * @details Tato proměnná obsahuje kořenový uzel abstraktního syntaktického
 *          stromu (AST), který je vytvářen během syntaktické analýzy. AST je
 *          používán pro reprezentaci struktury programu a je generován parserem.
 *          Dále je využíván k sémantické analýze a generování kódu.
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
extern FrameStack frameStack;

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


typedef enum GetNextTokenState {
    RESET_LOOKAHEAD = 1,
    LL_PARSER = 2,
    PREC_PARSER = 3,
} GetNextTokenState;


typedef enum PropagateError{
    RESET_ERROR_FLAG = 1,
    IS_SYNTAX_ERROR = 2,       /**< Parametr pro funkci `Parser_watchSyntaxError`, aby zkontrolovala aktuální stav syntax error. */
    SET_SYNTAX_ERROR = 3,       /**< Parametr pro funkci `Parser_watchSyntaxError`, aby nastavila stav syntax error na `true`. */
} PropagateError;


/*******************************************************************************
 *                                                                             *
 *                     DEKLARACE VEŘEJNÝCH FUNKCÍ PARSERU                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální současný token.
 *
 * @details Tato funkce získá další token ze scanneru a aktualizuje globální
 *          proměnnou @c currentToken na aktuální lookahead token. Funkce používá
 *          statickou proměnnou @c lookaheadToken pro uchování lookahead tokenu,
 *          což umožňuje předvídání dalšího tokenu bez jeho okamžitého zpracování.
 *          Pokud lookahead token není inicializován, funkce ho načte pomocí
 *          @c scanner_getNextToken(). Poté aktualizuje @c currentToken na
 *          hodnotu lookahead tokenu a načte další lookahead token.
 *
 * @return Vrací @c lookaheadToken.
 */
Terminal Parser_getNextToken(GetNextTokenState state);

/**
 * @brief Nastaví nebo zkontroluje stav syntax error.
 *
 * @param setError Pokud je @c true, nastaví stav syntax error na @c true.
 *                 Pokud je @c false, vrátí aktuální stav syntax error.
 *
 * @return Aktuální stav syntax error.
 */
bool Parser_watchSyntaxErrorInternal(PropagateError state, const char *file, int line, const char *func);

#define Parser_watchSyntaxError(state) Parser_watchSyntaxErrorInternal(state, __FILE__, __LINE__, __func__)


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

/**
 * @brief Získá další token ze scanneru a namapuje ho na typ LL terminálu.
 *
 * @details Tato funkce získá další token ze scanneru, namapuje jeho typ na typ
 *          LL terminálu a vrátí strukturu `Terminal` obsahující typ a hodnotu
 *          tokenu.
 *
 * @return Struktura `Terminal` obsahující typ a hodnotu tokenu.
 */
Terminal Parser_pokeScanner(GetNextTokenState state);

/**
 * @brief Namapuje typ tokenu na typ LL terminálu.
 *
 * @details Tato funkce namapuje typ aktuálního tokenu na odpovídající typ
 *          LL terminálu. Pokud je předaný ukazatel @c terminalType NULL,
 *          funkce zavolá @c error_handle() s chybovým kódem @c ERROR_INTERNAL.
 *
 * @param [in] tokenType Typ tokenu, který má být přemapován na typ terminálu.
 * @param [in] terminalType Ukazatel na proměnnou typu @c LLTerminals, kam bude
 *                          uložen namapovaný typ LL terminálu.
 */
void Parser_mapTokenToLLTerminal(TokenType tokenType, LLTerminals *terminalType);

void Parser_mapTokenToPrecTerminal(TokenType tokenType, PrecTerminals *terminalType);

/**
 * @brief Namapuje typ datového typu AST na typ návratového typu tabulky symbolů.
 *
 * @param astDataType Typ datového typu AST.
 * @param symtableType Ukazatel na typ návratového typu tabulky symbolů.
 */
void Parser_mapASTDataTypeToFunReturnType(AST_DataType astDataType, symtable_functionReturnType *symtableType);

/**
 * @brief Namapuje typ datového typu AST na stav symbolu v tabulce symbolů.
 *
 * @param astDataType Typ datového typu AST.
 * @param symtableState Ukazatel na stav symbolu v tabulce symbolů.
 */
void Parser_mapASTDataTypeToSymtableState(AST_DataType astDataType, symtable_symbolState *symtableState);

#endif  // PARSER_H_

/*** Konec souboru parser.h ***/
