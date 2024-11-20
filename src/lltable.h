/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable.h                                                 *
 * Autor:            Jan Kalina   <xkalinj00> (návrh LL-gramatiky,             *
 *                                             implementace LL-tabulky)        *
 *                   David Krejčí <xkrejcd00> (tvorba množin)                  *
 *                                                                             *
 * Datum:            24.10.2024                                                *
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
 * @file lltable.h
 * @author Jan Kalina   \<xkalinj00> (návrh LL-gramatiky, implementace LL-tabulky)
 * @author David Krejčí \<xkrejcd00> (tvorba množin)
 *
 * @brief Hlavičkový soubor pro LL tabulku.
 * @details Tento soubor deklaruje datové struktury, výčtové typy a funkce
 *          potřebné pro práci s LL tabulkou, která je klíčová pro řízení
 *          přechodů v LL syntaktické analýze. LL tabulka obsahuje pravidla
 *          pro přechody mezi stavy analyzátoru na základě aktuálního
 *          neterminálu a tokenu na vstupu.
 */

#ifndef LLTABLE_H_
/** @cond  */
#define LLTABLE_H_
/** @endcond  */

#include <stdbool.h>
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define LL_TERMINAL_COUNT 31       /**<  Celkový počet terminálů v LL gramatice. */
#define LL_NON_TERMINAL_COUNT 24   /**<  Celkový počet neterminálů v LL gramatice. */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčet terminálů používaných v LL syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v LL syntaktické analýze. Každý terminál určuje jeden řádek v
 *          LL tabulce.
 */
typedef enum LLTerminals {
    T_UNDEFINED             = -1,       /**<  Aktuální terminál je zatím neznámý */
    T_ID                    = 0,        /**<  Identifikátor  */
    T_IMPORT                = 1,        /**<  Klíčové slovo "@import"  */
    T_IFJ                   = 2,        /**<  Klíčové slovo "ifj"  */
    T_PUB                   = 3,        /**<  Klíčové slovo "pub"  */
    T_FN                    = 4,        /**<  Klíčové slovo "fn"  */
    T_CONST                 = 5,        /**<  Klíčové slovo "const"  */
    T_VAR                   = 6,        /**<  Klíčové slovo "var"  */
    T_IF                    = 7,        /**<  Klíčové slovo "if"  */
    T_ELSE                  = 8,        /**<  Klíčové slovo "else"  */
    T_WHILE                 = 9,        /**<  Klíčové slovo "while"  */
    T_RETURN                = 10,       /**<  Klíčové slovo "return"  */
    T_ASSIGNMENT            = 11,       /**<  Operátor přiřazení "="  */
    T_INT                   = 12,       /**<  Klíčové slovo "i32"  */
    T_INT_OR_NULL           = 13,       /**<  Klíčové slovo "?i32"  */
    T_FLOAT                 = 14,       /**<  Klíčové slovo "f64"  */
    T_FLOAT_OR_NULL         = 15,       /**<  Klíčové slovo "?f64"  */
    T_STRING                = 16,       /**<  Klíčové slovo "[]u8"  */
    T_STRING_OR_NULL        = 17,       /**<  Klíčové slovo "?[]u8"  */
    T_VOID                  = 18,       /**<  Klíčové slovo "void"  */
    T_DUMP                  = 19,       /**<  Speciální proměnná "_" pro zahození návratové hodnoty  */
    T_DOT                   = 20,       /**<  Symbol tečky "."  */
    T_COMMA                 = 21,       /**<  Symbol čárky ","  */
    T_COLON                 = 22,       /**<  Symbol dvojtečky ":"  */
    T_SEMICOLON             = 23,       /**<  Symbol středníku ";"  */
    T_PIPE                  = 24,       /**<  Symbol svislice "|"  */
    T_LEFT_BRACKET          = 25,       /**<  Symbol levé závorky "("  */
    T_RIGHT_BRACKET         = 26,       /**<  Symbol pravé závorky ")"  */
    T_LEFT_CURLY_BRACKET    = 27,       /**<  Symbol levé složené závorky "{"  */
    T_RIGHT_CURLY_BRACKET   = 28,       /**<  Symbol pravé složené závorky "}"  */
    T_EOF                   = 29,       /**<  Signalizace, že nastal konec souboru  */
    T_CALL_PRECEDENCE       = 30,       /**<  Řízení parsování předáno precedenčnímu parseru */
} LLTerminals;

/**
 * @brief Výčet neterminálů používaných v LL syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny neterminály, které mohou být použity
 *          v syntaktické analýze. Každý neterminál je reprezentován unikátní
 *          hodnotou, která je využívaná v LL tabulce.
 */
typedef enum LLNonTerminals {
    NT_PROGRAM         = 0,     /**<  Neterminál pro program  */
    NT_PROLOGUE        = 1,     /**<  Neterminál pro prolog  */
    NT_FUN_DEF_LIST    = 2,     /**<  Neterminál pro seznam definic funkcí  */
    NT_FUN_DEF         = 3,     /**<  Neterminál pro definici funkce  */
    NT_PARAMETERS      = 4,     /**<  Neterminál pro parametry  */
    NT_PARAM_LIST      = 5,     /**<  Neterminál pro seznam parametrů  */
    NT_PARAM           = 6,     /**<  Neterminál pro parametr  */
    NT_PARAM_LIST_REST = 7,     /**<  Neterminál pro zbytek seznamu parametrů  */
    NT_RETURN_TYPE     = 8,     /**<  Neterminál pro návratový typ  */
    NT_DATA_TYPE       = 9,     /**<  Neterminál pro datový typ  */
    NT_STATEMENT_LIST  = 10,    /**<  Neterminál pro seznam příkazů  */
    NT_STATEMENT       = 11,    /**<  Neterminál pro příkaz  */
    NT_STATEMENT_REST  = 12,    /**<  Neterminál pro zbytek příkazu  */
    NT_THROW_AWAY      = 13,    /**<  Neterminál pro zahození návratové hodnoty  */
    NT_VAR_DEF         = 14,    /**<  Neterminál pro definici proměnné  */
    NT_MODIFIABLE      = 15,    /**<  Neterminál pro modifikovatelnou hodnotu  */
    NT_POSSIBLE_TYPE   = 16,    /**<  Neterminál pro možný typ  */
    NT_IF              = 17,    /**<  Neterminál pro podmínku if  */
    NT_NULL_COND       = 18,    /**<  Neterminál pro podmínku null  */
    NT_SEQUENCE        = 19,    /**<  Neterminál pro sekvenci příkazů (blok)  */
    NT_WHILE           = 20,    /**<  Neterminál pro cyklus while  */
    NT_RETURN          = 21,    /**<  Neterminál pro příkaz návratu  */
    NT_RETURN_REST     = 22,    /**<  Neterminál pro zbytek příkazu návratu  */
    NT_ARGUMENTS       = 23,    /**<  Neterminál pro argumenty  */
} LLNonTerminals;

/**
 * @brief Enum pro pravidla LL-gramatiky syntaktického analyzátoru.
 *
 * @details Tento `enum` obsahuje identifikátory pravidel LL-gramatiky parseru.
 *          Každá hodnota odpovídá konkrétnímu produkčnímu pravidlu, což umožňuje
 *          efektivní odkazování v parsovací tabulce.
 */
typedef enum LLRuleSet {
    RULE_UNDEFINED    = -2,     /**<  LL pravidlo zatím nebylo zvoleno                                  */
    SYNTAX_ERROR      = -1,     /**<  Chyba syntaxe (takovéto pravidlo se v LL-tabulce nevyskytuje)     */
    PROGRAM           = 0,      /**<  <PROGRAM> -> <PROLOGUE> <FUN_DEF_LIST> EOF                        */
    PROLOGUE          = 1,      /**<  <PROLOGUE> -> const ifj = @import ( "ifj24.zig" ) ;               */
    FUN_DEF_LIST_1    = 2,      /**<  <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST>                        */
    FUN_DEF_LIST_2    = 3,      /**<  <FUN_DEF_LIST> -> ε                                               */
    FUN_DEF           = 4,      /**<  <FUN_DEF> -> pub fn id ( <PARAMETERS> ) <RETURN_TYPE> <SEQUENCE>  */
    PARAMETERS_1      = 5,      /**<  <PARAMETERS> -> <PARAM_LIST>                                      */
    PARAMETERS_2      = 6,      /**<  <PARAMETERS> -> ε                                                 */
    PARAM_LIST        = 7,      /**<  <PARAM_LIST> -> <PARAM> <PARAM_LIST_REST>                         */
    PARAM_LIST_REST_1 = 8,      /**<  <PARAM_LIST_REST> -> , <PARAM_LIST>                               */
    PARAM_LIST_REST_2 = 9,      /**<  <PARAM_LIST_REST> -> ε                                            */
    PARAM             = 10,     /**<  <PARAM> -> id : <DATA_TYPE>                                       */
    RETURN_TYPE_1     = 11,     /**<  <RETURN_TYPE> -> <DATA_TYPE>                                      */
    RETURN_TYPE_2     = 12,     /**<  <RETURN_TYPE> -> void                                             */
    DATA_TYPE_1       = 13,     /**<  <DATA_TYPE> -> i32                                                */
    DATA_TYPE_2       = 14,     /**<  <DATA_TYPE> -> ?i32                                               */
    DATA_TYPE_3       = 15,     /**<  <DATA_TYPE> -> f64                                                */
    DATA_TYPE_4       = 16,     /**<  <DATA_TYPE> -> ?f64                                               */
    DATA_TYPE_5       = 17,     /**<  <DATA_TYPE> -> []u8                                               */
    DATA_TYPE_6       = 18,     /**<  <DATA_TYPE> -> ?[]u8                                              */
    STATEMENT_LIST_1  = 19,     /**<  <STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST>                  */
    STATEMENT_LIST_2  = 20,     /**<  <STATEMENT_LIST> -> ε                                             */
    STATEMENT_1       = 21,     /**<  <STATEMENT> -> <VAR_DEF> ;                                        */
    STATEMENT_2       = 22,     /**<  <STATEMENT> -> id <STATEMENT_REST> ;                              */
    STATEMENT_3       = 23,     /**<  <STATEMENT> -> _ = <THROW_AWAY> ;                                 */
    STATEMENT_4       = 24,     /**<  <STATEMENT> -> <IF>                                               */
    STATEMENT_5       = 25,     /**<  <STATEMENT> -> <WHILE>                                            */
    STATEMENT_6       = 26,     /**<  <STATEMENT> -> <RETURN> ;                                         */
    STATEMENT_7       = 27,     /**<  <STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;                         */
    VAR_DEF           = 28,     /**<  <VAR_DEF> -> <MODIFIABLE> id <POSSIBLE_TYPE> = [precedence_expr]  */
    MODIFIABLE_1      = 29,     /**<  <MODIFIABLE> -> var                                               */
    MODIFIABLE_2      = 30,     /**<  <MODIFIABLE> -> const                                             */
    POSSIBLE_TYPE_1   = 31,     /**<  <POSSIBLE_TYPE> -> : <DATA_TYPE>                                  */
    POSSIBLE_TYPE_2   = 32,     /**<  <POSSIBLE_TYPE> -> ε                                              */
    STATEMENT_REST_1  = 33,     /**<  <STATEMENT_REST> -> = [precedence_expr]                           */
    STATEMENT_REST_2  = 34,     /**<  <STATEMENT_REST> -> ( <ARGUMENTS> )                               */
    THROW_AWAY        = 35,     /**<  <THROW_AWAY> -> [precedence_expr]                                 */
    IF                = 36,     /**<  <IF> -> if ( [precedence_expr] ) <NULL_COND> <SEQUENCE> else <SEQUENCE> */
    NULL_COND_1       = 37,     /**<  <NULL_COND> -> | id |                                             */
    NULL_COND_2       = 38,     /**<  <NULL_COND> -> ε                                                  */
    SEQUENCE          = 39,     /**<  <SEQUENCE> -> { <STATEMENT_LIST> }                                */
    WHILE             = 40,     /**<  <WHILE> -> while ( [precedence_expr] ) <NULL_COND> <SEQUENCE>     */
    RETURN            = 41,     /**<  <RETURN> -> return <RETURN_REST>                                  */
    RETURN_REST_1     = 42,     /**<  <RETURN_REST> -> [precedence_expr]                                */
    RETURN_REST_2     = 43,     /**<  <RETURN_REST> -> ε                                                */
    ARGUMENTS         = 44,     /**<  <ARGUMENTS> -> [precedence_expr]                                  */
} LLRuleSet;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura pro reprezentaci pravidel v LL tabulce.
 *
 * @details Tato struktura obsahuje klíč a pole hodnot, které reprezentují
 *          pravidla pro přechody mezi stavy na základě aktuálního ne/terminálu.
 *
 * @note V tabulce je příslušný řádek pro každý terminál, jejichž kódem je tabulka
 *       indexována. Sloupce tabulky odpovídají jednotlivým NEterminálům.
 *       Souřadnice [Terminál, NEterminál] určují aplikaci příslušného pravidla.
 */
struct LLtable {
    LLTerminals key;                         /**<  Klíč pro identifikaci neterminálu v tabulce  */
    LLRuleSet value[LL_NON_TERMINAL_COUNT];  /**<  Pole hodnot reprezentující pravidla pro přechody mezi stavy  */
};


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Externí deklarace LL-tabulky.
 *
 * @details V tabulce je příslušný řádek pro každý terminál, jejichž kódem je
 *          tabulka indexována. Sloupce tabulky odpovídají jednotlivým
 *          NEterminálům. Souřadnice [Terminál, NEterminál] určují aplikaci
 *          příslušného pravidla.
 */
extern const struct LLtable LLtable[LL_TERMINAL_COUNT];


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v LL tabulce na základě aktuálního neterminálu a terminálu.
 *
 * @details Tato funkce používá kód neterminálu a terminálu k vyhledání
 *          odpovídajícího pravidla v LL tabulce. Funkce používá binární
 *          vyhledávání pro efektivní nalezení pravidla. Pokud pravidlo
 *          neexistuje nebo terminál není nalezen v tabulce, funkce vrací přes
 *          parametr syntaktickou chybu @c SYNTAX_ERROR typu @c LLRuleSet.
 *          Pokud funkce najde naprosto neočekávaný řádek tabulky, zahlásí
 *          @c ERROR_INTERNAL.
 *
 * @param [in] tokenType Typ aktuální tokenu.
 * @param [in] nonTerminal Neterminál, pro který se má najít pravidlo.
 * @param [out] rule Ukazatel na proměnnou typu @c LLRuleSet, kam bude uloženo
 *                   nalezené pravidlo.
 *
 * @return Vrací @c true, pokud bylo pravidlo v LL tabulce nalezeno,
 *         jinak @c false.
 *
 * @note Funkce typ aktuálního terminálu získává přímo z globální proměnné
 *       @c currentTerminal. Typ terminálu je následně implicitně přemapován
 *       na typ terminálu.
 */
bool LLtable_findRule(LLTerminals tokenType, LLNonTerminals nonTerminal, LLRuleSet *rule);

#endif  // LLTABLE_H_

/*** Konec souboru lltable.h ***/
