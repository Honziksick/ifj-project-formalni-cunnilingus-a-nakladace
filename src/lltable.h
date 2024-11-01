/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable.h                                                 *
 * Autor:            Jan Kalina   <xkalinj00> (autor LL-gramatiky)             *
 *                   David Krejčí <xkrejcd00> (autor LL-tabulky)               *
 *                                                                             *
 * Datum:            24.10.2024                                                *
 * Poslední změna:   30.10.2024                                                *
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
 * @author Jan Kalina   \<xkalinj00> (autor LL-gramatiky)
 * @author David Krejčí \<xkrejcd00> (autor LL-tabulky)
 *
 * @brief Hlavičkový soubor pro správu LL tabulky.
 * @details Tento hlavičkový soubor obsahuje deklarace výčtových typů, funkcí
 *          a datových struktur potřebných pro správu LL tabulky v překladači.
 *          LL tabulka je používána pro LL syntaktickou analýzu a obsahuje
 *          pravidla pro přechody mezi stavy.
 */

#ifndef LLTABLE_H_
/** @cond  */
#define LLTABLE_H_
/** @endcond  */

#include <stdlib.h>
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define TERMINAL_COUNT 43       /**<  Celkový počet terminálů v LL tabulce.*/
#define NON_TERMINAL_COUNT 40   /**<  Celkový počet neterminálů v LL tabulce.*/


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Externí deklarace LL-tabulky.
 *
 * @details Tato tabulka je použita pro LL syntaktickou analýzu. Obsahuje
 *          pravidla pro přechody mezi stavy. Každý řádek tabulky obsahuje hash
 *          klíč a pole hodnot pro neterminály
 */
extern LLtable LLTable[TERMINAL_COUNT];


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
 */
typedef struct {
    size_t key;                     /**<  Klíč pro identifikaci neterminálu v tabulce  */
    int value[NON_TERMINAL_COUNT];  /**<  Pole hodnot reprezentující pravidla pro přechody mezi stavy  */
} LLtable;


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @enum Terminals
 * @brief Výčet terminálů používaných v LL syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v syntaktické analýze. Každý terminál je reprezentován unikátní
 *          hodnotou, která je využívaná v LL tabulce.
 */
typedef enum Terminals {
    T_CONST     = 0,                /**<  Klíčové slovo "const"  */
    T_IFJ       = 1,                /**<  Klíčové slovo "ifj"  */
    T_EQUAL     = 2,                /**<  Operátor přiřazení "="  */
    T_AT_IMPORT = 3,                /**<  Klíčové slovo "@import"  */
    T_LEFT_PARENTHESIS  = 4,        /**<  Symbol levé závorky ")"  */
    T_RIGHT_PARENTHESIS = 5,        /**<  Symbol pravé závorky "("  */
    T_SEMICOLON = 6,                /**<  Symbol středníku ";"  */
    T_PUB   = 7,                    /**<  Klíčové slovo "pub"  */
    T_FN    = 8,                    /**<  Klíčové slovo "fn"  */
    T_ID    = 9,                    /**<  Identifikátor  */
    T_COLON = 10,                   /**<  Symbol dvojtečky ":"  */
    T_COMMA = 11,                   /**<  Symbol čárky ","  */
    T_VOID  = 12,                   /**<  Klíčové slovo "void"  */
    T_INT   = 13,                   /**<  Klíčové slovo "i32"  */
    T_INT_OR_NULL    = 14,          /**<  Klíčové slovo "?i32"  */
    T_FLOAT          = 15,          /**<  Klíčové slovo "f64"  */
    T_FLOAT_OR_NULL  = 16,          /**<  Klíčové slovo "?f64"  */
    T_STRING         = 17,          /**<  Klíčové slovo "[]u8"  */
    T_STRING_OR_NULL = 18,          /**<  Klíčové slovo "?[]u8"  */
    T_VAR  = 19,                    /**<  Klíčové slovo "var"  */
    T_IF   = 20,                    /**<  Klíčové slovo "if"  */
    T_ELSE = 21,                    /**<  Klíčové slovo "else"  */
    T_PIPE = 22,                    /**<  Symbol svislice "|"  */
    T_LEFT_CURLY_BRACKET  = 23,     /**<  Symbol levé složené závorky "{"  */
    T_RIGHT_CURLY_BRACKET = 24,     /**<  Symbol levé složené závorky "}"  */
    T_WHILE  = 25,                  /**<  Klíčové slovo "while"  */
    T_RETURN = 26,                  /**<  Klíčové slovo "return"  */
    T_DOUBLE_EQUAL = 27,            /**<  Operátor rovnosti "=="  */
    T_NOT_EQUAL    = 28,            /**<  Operátor nerovnosti "!="  */
    T_LESS_THAN    = 29,            /**<  Operátor menší než "<"  */
    T_GREATER_THAN = 30,            /**<  Operátor větší než ">"  */
    T_LESS_THAN_OR_EQUAL    = 31,   /**<  Operátor menší rovno "<="  */
    T_GREATER_THAN_OR_EQUAL = 32,   /**<  Operátor větší rovno ">=""  */
    T_PLUS     = 33,                /**<  Operátor součtu "+"  */
    T_MINUS    = 34,                /**<  Operátor rozdílu "-"  */
    T_MULTIPLY = 35,                /**<  Operátor součinu "*"  */
    T_DIVIDE   = 36,                /**<  Operátor podílu "/"  */
    T_DOT      = 37,                /**<  Symbol tečky "."  */
    T_INT_LITERAL    = 38,          /**<  Literál typu i32  */
    T_FLOAT_LITERAL  = 39,          /**<  Literál typu i64  */
    T_STRING_LITERAL = 40,          /**<  Literál typu []u8  */
    T_NULL_LITERAL   = 41,          /**<  Literál typu NULL  */
    T_EOF            = 42,          /**<  Signalizace, že nastal konec souboru  */
} Terminals;

/**
 * @enum NonTerminals
 * @brief Výčet neterminálů používaných v LL syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny neterminály, které mohou být použity
 *          v syntaktické analýze. Každý neterminál je reprezentován unikátní
 *          hodnotou, která je využívaná v LL tabulce.
 */
typedef enum NonTerminals {
    NT_PROGRAM      = 0,            /**<  Neterminál pro program  */
    NT_PROLOGUE     = 1,            /**<  Neterminál pro prolog  */
    NT_FUN_DEF_LIST = 2,            /**<  Neterminál pro seznam definic funkcí  */
    NT_FUN_DEF      = 3,            /**<  Neterminál pro definici funkce  */
    NT_PARAMETERS   = 4,            /**<  Neterminál pro parametry  */
    NT_PARAM_LIST   = 5,            /**<  Neterminál pro seznam parametrů  */
    NT_PARAM        = 6,            /**<  Neterminál pro parametr  */
    NT_PARAM_LIST_REST = 7,         /**<  Neterminál pro zbytek seznamu parametrů  */
    NT_RETURN_TYPE     = 8,         /**<  Neterminál pro návratový typ  */
    NT_DATA_TYPE       = 9,         /**<  Neterminál pro datový typ  */
    NT_STATEMENT_LIST  = 10,        /**<  Neterminál pro seznam příkazů  */
    NT_STATEMENT       = 11,        /**<  Neterminál pro příkaz  */
    NT_STATEMENT_REST  = 12,        /**<  Neterminál pro zbytek příkazu  */
    NT_VAR_DEF       = 13,          /**<  Neterminál pro definici proměnné  */
    NT_MODIFIABLE    = 14,          /**<  Neterminál pro modifikovatelnou hodnotu  */
    NT_POSSIBLE_TYPE = 15,          /**<  Neterminál pro možný typ  */
    NT_ASSIGNMENT    = 16,          /**<  Neterminál pro přiřazení  */
    NT_FUN_CALL  = 17,              /**<  Neterminál pro volání funkce  */
    NT_IF        = 18,              /**<  Neterminál pro podmínku if  */
    NT_NULL_COND = 19,              /**<  Neterminál pro podmínku null  */
    NT_SEQUENCE  = 20,              /**<  Neterminál pro sekvenci příkazů (blok)  */
    NT_WHILE     = 21,              /**<  Neterminál pro cyklus while  */
    NT_RETURN    = 22,              /**<  Neterminál pro příkaz návratu  */
    NT_RETURN_REST     = 23,        /**<  Neterminál pro zbytek příkazu návratu  */
    NT_EXPR            = 24,        /**<  Neterminál pro výraz  */
    NT_REL_EXPR        = 25,        /**<  Neterminál pro relační výraz  */
    NT_REL_EXPR_REST   = 26,        /**<  Neterminál pro zbytek relačního výrazu  */
    NT_REL_OP          = 27,        /**<  Neterminál pro relační operátor  */
    NT_ADD_EXPR        = 28,        /**<  Neterminál pro aditivní výraz  */
    NT_ADD_EXPR_REST   = 29,        /**<  Neterminál pro zbytek aditivního výrazu  */
    NT_ADD_OP          = 30,        /**<  Neterminál pro aditivní operátor  */
    NT_MULTI_EXPR      = 31,        /**<  Neterminál pro multiplikativní výraz  */
    NT_MULTI_EXPR_REST = 32,        /**<  Neterminál pro zbytek multiplikativního výrazu  */
    NT_MULTI_OP  = 33,              /**<  Neterminál pro multiplikativní operátor  */
    NT_TERM      = 34,              /**<  Neterminál pro term  */
    NT_TERM_REST = 35,              /**<  Neterminál pro zbytek termu  */
    NT_ARGUMENTS = 36,              /**<  Neterminál pro argumenty  */
    NT_ARG_LIST  = 37,              /**<  Neterminál pro seznam argumentů  */
    NT_ARG       = 38,              /**<  Neterminál pro argument  */
    NT_LITERAL   = 39,              /**<  Neterminál pro literál  */
} NonTerminals;

/**
 * @enum RuleSet
 * @brief Enum pro pravidla LL-gramatiky syntaktického analyzátoru.
 *
 * @details Tento `enum` obsahuje identifikátory pravidel LL-gramatiky parseru.
 *          Každá hodnota odpovídá konkrétnímu produkčnímu pravidlu, což umožňuje
 *          efektivní odkazování v parsovací tabulce.
 */
typedef enum RuleSet {
    SYNTAX_ERROR      = -1,     /**<  Chyba syntaxe (takovéto pravidlo se v LL-tabulce nevyskytuje)    */
    PROGRAM           = 0,      /**<  <PROGRAM> -> <PROLOG> <FUN_DEF_LIST> EOF                          */
    PROLOGUE          = 1,      /**<  <PROLOG> -> const ifj = @import ( "ifj24.zig" ) ;                 */
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
    STATEMENT_1  = 21,          /**<  <STATEMENT> -> <VAR_DEF> ;                                        */
    STATEMENT_2  = 22,          /**<  <STATEMENT> -> id <STATEMENT_REST> ;                              */
    STATEMENT_3  = 23,          /**<  <STATEMENT> -> <IF>                                               */
    STATEMENT_4  = 24,          /**<  <STATEMENT> -> <WHILE>                                            */
    STATEMENT_5  = 25,          /**<  <STATEMENT> -> <RETURN> ;                                         */
    VAR_DEF      = 26,          /**<  <VAR_DEF> -> <MODIFIABLE> id <POSSIBLE_TYPE> = <EXPR>             */
    MODIFIABLE_1 = 27,          /**<  <MODIFIABLE> -> var                                               */
    MODIFIABLE_2 = 28,          /**<  <MODIFIABLE> -> const                                             */
    POSSIBLE_TYPE_1   = 29,     /**<  <POSSIBLE_TYPE> -> : <DATA_TYPE>                                  */
    POSSIBLE_TYPE_2   = 30,     /**<  <POSSIBLE_TYPE> -> ε                                              */
    STATEMENT_REST_1  = 31,     /**<  <STATEMENT_REST> -> = <EXPR>                                      */
    STATEMENT_REST_2  = 32,     /**<  <STATEMENT_REST> -> ( <ARGUMENTS> )                               */
    STATEMENT_REST_3  = 33,     /**<  <STATEMENT_REST> -> ifj . id ( <ARGUMENTS> )                      */
    IF                = 34,     /**<  <IF> -> if ( <EXPR> ) <NULL_COND> <SEQUENCE> else <SEQUENCE>      */
    NULL_COND_1       = 35,     /**<  <NULL_COND> -> | id |                                             */
    NULL_COND_2       = 36,     /**<  <NULL_COND> -> ε                                                  */
    SEQUENCE          = 37,     /**<  <SEQUENCE> -> { <STATEMENT_LIST> }                                */
    WHILE             = 38,     /**<  <WHILE> -> while ( <EXPR> ) <NULL_COND> <SEQUENCE>                */
    RETURN            = 39,     /**<  <RETURN> -> return <RETURN_REST>                                  */
    RETURN_REST_1     = 40,     /**<  <RETURN_REST> -> <EXPR>                                           */
    RETURN_REST_2     = 41,     /**<  <RETURN_REST> -> ε                                                */
    EXPR              = 42,     /**<  <EXPR> -> <REL_EXPR>                                              */
    REL_EXPR          = 43,     /**<  <REL_EXPR> -> <ADD_EXPR> <REL_EXPR_REST>                          */
    REL_EXPR_REST_1   = 44,     /**<  <REL_EXPR_REST> -> <REL_OP> <ADD_EXPR>                            */
    REL_EXPR_REST_2   = 45,     /**<  <REL_EXPR_REST> -> ε                                              */
    REL_OP_1          = 46,     /**<  <REL_OP> -> ==                                                    */
    REL_OP_2          = 47,     /**<  <REL_OP> -> !=                                                    */
    REL_OP_3          = 48,     /**<  <REL_OP> -> <                                                     */
    REL_OP_4          = 49,     /**<  <REL_OP> -> >                                                     */
    REL_OP_5          = 50,     /**<  <REL_OP> -> <=                                                    */
    REL_OP_6          = 51,     /**<  <REL_OP> -> >=                                                    */
    ADD_EXPR          = 52,     /**<  <ADD_EXPR> -> <MULTI_EXPR> <ADD_EXPR_REST>                        */
    ADD_EXPR_REST_1   = 53,     /**<  <ADD_EXPR_REST> -> <ADD_OP> <MULTI_EXPR> <ADD_EXPR_REST>          */
    ADD_EXPR_REST_2   = 54,     /**<  <ADD_EXPR_REST> -> ε                                              */
    ADD_OP_1          = 55,     /**<  <ADD_OP> -> +                                                     */
    ADD_OP_2          = 56,     /**<  <ADD_OP> -> –                                                     */
    MULTI_EXPR        = 57,     /**<  <MULTI_EXPR> -> <TERM> <MULTI_EXPR_REST>                          */
    MULTI_EXPR_REST_1 = 58,     /**<  <MULTI_EXPR_REST> -> <MULTI_OP> <TERM> <MULTI_EXPR_REST>          */
    MULTI_EXPR_REST_2 = 59,     /**<  <MULTI_EXPR_REST> -> ε                                            */
    MULTI_OP_1  = 60,           /**<  <MULTI_OP> -> *                                                   */
    MULTI_OP_2  = 61,           /**<  <MULTI_OP> -> /                                                   */
    TERM_1      = 62,           /**<  <TERM> -> id <TERM_REST>                                          */
    TERM_2      = 63,           /**<  <TERM> -> ifj . id <TERM_REST>                                    */
    TERM_3      = 64,           /**<  <TERM> -> <LITERAL>                                               */
    TERM_4      = 65,           /**<  <TERM> -> ( <EXPR> )                                              */
    TERM_REST_1 = 66,           /**<  <TERM_REST> -> ( <ARGUMENTS> )                                    */
    TERM_REST_2 = 67,           /**<  <TERM_REST> -> ε                                                  */
    ARGUMENTS_1 = 68,           /**<  <ARGUMENTS> -> <ARG_LIST>                                         */
    ARGUMENTS_2 = 69,           /**<  <ARGUMENTS> -> ε                                                  */
    ARG_LIST    = 70,           /**<  <ARG_LIST> -> <EXPR> <ARG>                                        */
    ARG_1       = 71,           /**<  <ARG> -> , <EXPR> <ARG>                                           */
    ARG_2       = 72,           /**<  <ARG> -> ε                                                        */
    LITERAL_1   = 73,           /**<  <LITERAL> -> i32_literal                                          */
    LITERAL_2   = 74,           /**<  <LITERAL> -> f64_literal                                          */
    LITERAL_3   = 75,           /**<  <LITERAL> -> u8_literal                                           */
    LITERAL_4   = 76            /**<  <LITERAL> -> null_literal                                         */
} RuleSet;


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vypočítá hash pro klíč do LL tabulky.
 *
 * @details Tato funkce používá algoritmus DJB2 pro výpočet hash hodnoty
 *          pro daný řetězec. Hash hodnota je použita pro rychlé vyhledávání
 *          v LL tabulce.
 *
 * @param [in] key Ukazatel na řetězec, pro který se má vypočítat hash.
 * @return @c size_t Hash hodnota pro daný klíč.
 */
size_t LLtable_hashFunction(char *key);

/**
 * @brief Najde pravidlo v LL tabulce na základě neterminálu a řetězce.
 *
 * @details Tato funkce používá hash hodnotu vypočítanou z řetězce pro
 *          vyhledání odpovídajícího pravidla v LL tabulce. Pokud je pravidlo
 *          nalezeno, vrací jeho index. Pokud není nalezeno, vrací -2.
 *
 * @param[in] nt Neterminál, pro který se má najít pravidlo.
 * @param[in] str Ukazatel na řetězec, pro který se má najít pravidlo.
 * @return Index nalezeného pravidla nebo hlásí, pokud pravidlo není nalezeno.
 *
 * @note Pokud získal funkcí find hash pro naprosto neočekávaný řádek tabulky,
 *       zahlásí `ERROR_INTERNAL`.
 */
int LLtable_findRule(int nt, char *str);

#endif  // LLTABLE_H_

/*** Konec souboru lltable.h ***/
