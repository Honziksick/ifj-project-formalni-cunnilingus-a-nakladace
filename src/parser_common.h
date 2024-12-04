/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser_common.h                                           *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   24.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file parser_common.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro sdílené funkce a proměnné parseru.
 * @details Tento soubor obsahuje deklarace globálních proměnných a funkcí,
 *          které využívají různé moduly parseru při syntaktické analýze
 *          jazyka IFJ24. Slouží jako centrální bod pro přístup ke sdíleným
 *          zdrojům, jako jsou aktuální token a kořen abstraktního syntaktického
 *          stromu (AST).
 */

#ifndef PARSER_COMMON_H_
/** @cond  */
#define PARSER_COMMON_H_
/** @endcond  */

// Import standardních knihoven jazyka C
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Import sdílených knihoven překladače
#include "error.h"
#include "dynamic_string.h"

// Import modulu: Scanner
#include "scanner.h"

// Import modulu: Abstraktní syntaktický strom (AST)
#include "ast_nodes.h"
#include "ast_interface.h"

// Import modulu: Symtable
#include "symtable.h"
#include "frame_stack.h"

// Import modulu: Tabulky LL a precedenčních pravidel
#include "lltable.h"
#include "precedence_table.h"


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

// Deklarace coverů pro true/false předávaných funkce 'parser_getNextToken()'
#define RESET_LOOKAHEAD false       /**<  Funkce Parser_getNextToken() vyresetuje statickou proměnnou pro lookahead token.     */
#define POKE_SCANNER    true        /**<  Funkce Parser_getNextToken() zavolá scanner a nastaví nový globální aktuální token.  */

// Cover konstanta pro návratovou hodnotu NULL ve významu chyby
#define PARSING_ERROR   NULL        /**<  Návratovvá hodnota NULL označující, že nastala chyba při parsování.  */


/*******************************************************************************
 *                                                                             *
 *                               DEFINICE MAKER                                *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Makro pro zjednodušení volání funkce @c Parser_watchSyntaxErrorInternal().
 *
 * @details Toto makro umožňuje volat funkci @c Parser_watchSyntaxErrorInternal()
 *          s automatickým předáním informací o souboru, řádku a funkci. Pokud
 *          je definováno makro @c LOG_VERBOSE, funkce vypíše tyto informace při
 *          nastavení chyby. Pokud není `LOG_PARSER` definováno, informace se
 *          nevypisují.
 *
 * @param [in] state Akce, kterou má funkce provést (viz `ParserErrorState`).
 *
 * @return Výsledek funkce @c Parser_watchSyntaxErrorInternal().
 */
#define parser_errorWatcher(state) \
        parser_errorWatcherInternal(state, __FILE__, __LINE__, __func__)


/**
 * @brief Makro pro logování chybových stavů při návratu rekurzivním sestupem.
 *
 * @details Toto makro slouží k logování chybových stavů na @c STDERR. Pokud je
 *          definována konstanta @c LOG_VERBOSE v hlavičkovém souboru @c error.h
 *          a její hodnota je @c true, makro vypíše název souboru, číslo řádku
 *          a název funkce, kde došlo k chybě. Pokud @c LOG_VERBOSE není definováno
 *          nebo jeho hodnota není @c true, makro neprovádí žádnou akci.
 *
 * @param file Název souboru, ve kterém došlo k chybě.
 * @param line Číslo řádku, na kterém došlo k chybě.
 * @param func Název funkce, ve které došlo k chybě.
 */
    #if LOG_VERBOSE
        #define LOG_ERROR(file, line, func) \
            do { \
                const char* fileName = error_getFileName(file); \
                fprintf(stderr, "\033[35mIn file: %s:%d (%s)\033[0m\n", fileName, line, func); \
            } while(false)
    #else
        #define LOG_ERROR(file, line, func) \
            do { \
                (void)(file); \
                (void)(line); \
                (void)(func); \
            } while(false)
    #endif

/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčtový typ stavů chyb, které mohou v parseru nastat pro parser.
 *
 * @details Tento výčtový typ definuje různé stavy chyb, které mohou nastat během
 *          lexikální, syntaktické a sémantické analýzy programu. Slouží jako
 *          "error registr" pro zaznamenávání chyb při parsování.
 */
typedef enum ParserErrorState {
    RESET_ERROR_FLAGS               = 0,    /**< Resetování všech chybových příznaků. */
    IS_PARSING_ERROR                = 1,    /**< Kontrola, zda při prasování došlo k libovolné chybě. */
    SET_ERROR_LEXICAL               = 2,    /**< Nastavení stavu lexikální chyby. */
    SET_ERROR_SYNTAX                = 3,    /**< Nastavení stavu syntaktické chyby. */
    SET_ERROR_SEM_UNDEF             = 4,    /**< Nastavení stavu sémantické chyby - nedefinovaná funkce či proměnná. */
    SET_ERROR_SEM_REDEF_OR_CONSTDEF = 5,    /**< Nastavení stavu sémantické chyby - redefinice nebo přiřazení do nemodifikovatelné proměnné. */
    SET_ERROR_SEM_OTHER             = 6,    /**< Nastavení stavu sémantické chyby - ostatní sémantické chyby. */
    SET_ERROR_INTERNAL              = 7,    /**< Nastavení stavu interní chyby překladače. */
    CALL_ERROR_HANDLE               = 8,    /**< Zavolá funkci pro řízení chybových stavů `error_handle()` */
} ParserErrorState;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura reprezentující aktuální terminál (resp. token získaný ze scaneru).
 *
 * @details Tato struktura obsahuje informace o terminálu, který je zpracováván
 *          parserem. Zahrnuje typ LL terminálu pro LL parser, typ precedenčního
 *          terminálu pro precedenční parser a hodnotu terminálu jako dynamický
 *          řetězec.
 */
typedef struct Terminal {
    LLTerminals   LLterminal;       /**< Typ LL terminálu pro LL parser. */
    PrecTerminals PrecTerminal;     /**< Typ precedenčního terminálu pro precedenční parser. */
    DString       *value;           /**< Hodnota aktuálního tokenu jako dynamický řetězec `DString`. */
} Terminal;


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro aktuální terminál, který je zpracováván.
 *
 * @details Tato proměnná obsahuje aktuální terminál získaný ze scanneru a
 *          namapovaný na interní reprezentaci obou parserů. Je využívána během
 *          syntaktické analýzy k rozhodování o dalším postupu parsování.
 *
 * @note Inicializováno do nedefinovaného stavu před začátkem parsování.
 */
extern Terminal currentTerminal;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 *
 * @details Tato proměnná obsahuje ukazatel na kořenový uzel AST, který
 *          reprezentuje celý zdrojový kód programu ve stromové struktuře.
 *          AST je vytvářen během syntaktické analýzy a je dále využíván při
 *          sémantické analýze a generování kódu.
 *
 * @note Inicializováno na @c NULL před začátkem parsování.
 */
extern AST_ProgramNode *ASTroot;

/**
 * @brief Globální zásobník rámců používaný v celém překladači.
 *
 * @details Zásobník rámců slouží k udržování informací o proměnných, funkcích
 *          a jejich rozahu platnosti. Umožňuje správu symbolických tabulek
 *          a pomáhá při řešení viditelnosti identifikátorů v různých částech
 *          programu.
 */
extern FrameStack frameStack;

/**
 * @brief Globální pole všech vytvořených rámců, kam jsou odkládány po popnutí
 *        ze zásobníku rámců.
 *
 * @details Pole rámců obsahuje všechny rámce vytvořené během parsování
 *          a sémantické analýzy. Umožňuje přístup k jednotlivým rámcům
 *          podle indexu, což je užitečné při generování kódu a optimalizacích.
 */
extern FrameArray frameArray;

/**
 * @brief Globální proměnná pro seznam precedenčních zásobníků.
 *
 * @details Tato proměnná uchovává ukazatel na aktuální precedenční zásobník,
 *          který je používán během syntaktické analýzy. Zásobník je inicializován
 *          na začátku analýzy a uvolněn po jejím dokončení.
 */
extern struct PrecStackList *precStackList;


/*******************************************************************************
 *                                                                             *
 *             DEKLARACE VEŘEJNÝCH FUNKCÍ OBECNÉHO ROZHRANÍ PARSERU            *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální současný token.
 *
 * @details Funkce @c parser_getNextToken() spravuje načítání tokenů ze scanneru
 *          pro parser. Chování funkce je řízeno parametrem @c state:
 *          - Pokud je @c state rovno @c POKE_SCANNER, funkce zavolá scanner a
 *            načte nový token, který aktualizuje globální proměnnou @c currentToken.
 *          - Pokud je @c state rovno @c RESET_LOOKAHEAD, funkce vyresetuje interní
 *            lookahead token, což je užitečné při zpětném zpracování tokenů.
 *
 * @note Funkce využívá statickou proměnnou pro uchování lookahead tokenu,
 *       což umožňuje předvídat další token, a to je potřebné pro LL syntaktickou
 *       analýzu.
 *
 * @param [in] state Určuje, zda funkce zavolá scanner nebo resetuje lookahead
 *                   token. Pro jednoduchost, lze pro první uvedený use-case
 *                   použít makro @c POKE_SCANNER nebo pro druhý
 *                   use-case @c RESET_LOOKAHEAD.
 */
void parser_getNextToken(bool state);

/**
 * @brief Nastaví nebo zkontroluje stav chyb během parsování.
 *
 * @details Funkce @c Parser_watchSyntaxErrorInternal() slouží k centrální správě
 *          chybových stavů během parsování. Umožňuje nastavit různé typy chyb
 *          nebo zjistit, zda k chybě již došlo, což je velmi užitečné při návratu
 *          v rámci rekurzivního sestupu. Parametr @c state určuje akci, kterou
 *          funkce provede.
 *
 * @note Funkce využívá statické lokální pro uchování typů chyb, které byly
 *       během parsování zaznamenány a také chybový kód první chyby, ke které
 *       došlo.
 * @note Pro jednodušší použití je k dispozici makro @c Parser_watchSyntaxError(state),
 *       které automaticky doplní informace o souboru, řádku a funkci.
 *
 * @param [in] state Akce, kterou má funkce provést (viz `ParserErrorState`).
 * @param [in] file Název souboru, ze kterého byla funkce volána (pro debuggovací účely).
 * @param [in] line Číslo řádku, ze kterého byla funkce volána (pro debuggovací účely).
 * @param [in] func Název funkce, ze které byla funkce volána (pro debuggovací účely).
 *
 * @return @c true, pokud je aktuálně nastavena chyba parsování, jinak @c false.
 */
bool parser_errorWatcherInternal(ParserErrorState state, const char *file, \
                                 int line, const char *func);

/**
 * @brief Uvolní hodnotu aktuálního terminálu.
 *
 * @details Tato funkce uvolní paměť pro hodnotu aktuálního terminálu, pokud není NULL,
 *          a nastaví ukazatel na NULL.
 */
void parser_freeCurrentTerminalValue();


/*******************************************************************************
 *                                                                             *
 *            DEKLARACE INTERNÍCH FUNKCÍ OBECNÝCH ROZHRANÍ PARSERU             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Nastaví první zaznamenanou chybu.
 *
 * @details Tato funkce aktualizuje hodnotu první zaznamenané chyby, pokud
 *          dosud nebyla nastavena žádná chyba. Pokud je hodnota chyby
 *          `SUCCESS`, nastaví se na hodnotu předanou v parametru `state`.
 *
 * @note Funkce je definována jako @c inline.
 *
 * @param [in] state Nový stav chyby, který se má nastavit, pokud dosud nebyla
 *                   zaznamenána žádná chyba.
 * @param [out] error Ukazatel na proměnnou, která uchovává první zaznamenanou chybu.
 */
void parser_updateFirstError(ParserErrorState state, ErrorType *error);

/**
 * @brief Získá další token ze scanneru a namapuje ho na typ LL a precedenčního
 *        terminálu.
 *
 * @details Funkce @c parser_pokeScanner() získá další token ze scanneru pomocí
 *          @c scanner_getNextToken() a poté namapuje jeho typ na odpovídající
 *          typ LL terminálu a precedenčního terminálu. Výsledný terminál je
 *          uložen do struktury @c Terminal, která obsahuje také hodnotu tokenu.
 *
 * @return Struktura @c Terminal obsahující typy terminálů a hodnotu tokenu.
 */
Terminal parser_pokeScanner();

/*******************************************************************************
 *                                                                             *
 *          DEKLARACE POMOCONÝCH INTERNÍCH MAPOVACÍCH FUNKCÍ PARSERU           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Namapuje typ tokenu na typ LL terminálu.
 *
 * @details Funkce @c parser_mapTokenToLLTerminal() převede typ tokenu získaného
 *          ze scanneru na odpovídající typ LL terminálu, který je použit v
 *          LL parseru. Pokud je ukazatel @c terminalType @c NULL.
 *
 * @param [in] tokenType Typ tokenu získaný ze scanneru (`TokenType`).
 * @param [out] terminalType Ukazatel na proměnnou typu @c LLTerminals,
 *                           kam bude uložen výsledek.
 */
void parser_mapTokenToLLTerminal(TokenType tokenType, LLTerminals *terminalType);

/**
 * @brief Namapuje typ tokenu na typ precedenčního terminálu.
 *
 * @details Funkce @c parser_mapTokenToPrecTerminal() převede typ tokenu na typ
 *          precedenčního terminálu pro použití v precedenčním parseru.
 *          Pokud je ukazatel @c terminalType @c NULL.
 *
 * @param [in] tokenType Typ tokenu získaný ze scanneru (`TokenType`).
 * @param [out] terminalType Ukazatel na proměnnou typu @c PrecTerminals,
 *                           kam bude uložen výsledek.
 */
void parser_mapTokenToPrecTerminal(TokenType tokenType, PrecTerminals *terminalType);

/**
 * @brief Namapuje typ datového typu AST na typ návratového typu tabulky symbolů.
 *
 * @details Funkce @c parser_mapASTDataTypeToFunReturnType() převede datový typ
 *          z AST na odpovídající typ návratové hodnoty funkce v tabulce symbolů.
 *          To je nezbytné pro správnou sémantickou analýzu a kontrolu typů při
 *          volání funkcí.
 *
 * @param [in] astDataType Datový typ z AST uzlu @c AST_DataType.
 * @param [out] symtableType Ukazatel na proměnnou typu @c Symtable_functionReturnType,
 *                           kam bude uložen výsledek.
 */
void parser_mapASTDataTypeToFunReturnType(AST_DataType astDataType, \
                                          Symtable_functionReturnType *symtableType);

/**
 * @brief Namapuje typ datového typu AST na stav symbolu v tabulce symbolů.
 *
 * @details Funkce @c parser_mapASTDataTypeToSymtableState() převede datový typ
 *          z AST na odpovídající stav symbolu v tabulce symbolů. To je důležité
 *          při deklaraci proměnných a jejich správné registraci v tabulkách
 *          symbolů rámců.
 *
 * @param [in] astDataType Datový typ z AST (`AST_DataType`).
 * @param [out] symtableState Ukazatel na proměnnou typu @c Symtable_symbolState,
 *                           kam bude uložen výsledek.
 */
void parser_mapASTDataTypeToSymtableState(AST_DataType astDataType, \
                                          Symtable_symbolState *symtableState);

#endif  // PARSER_COMMON_H_

/*** Konec souboru parser_common.h ***/
