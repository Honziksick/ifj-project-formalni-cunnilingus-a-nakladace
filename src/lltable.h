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
#include <string.h>
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define TERMINAL_COUNT 44       /**<  Celkový počet terminálů v LL tabulce.*/
#define NON_TERMINAL_COUNT 40   /**<  Celkový počet neterminálů v LL tabulce.*/
#define HASH_SEED 5381          /**<  Prvočíslo pro hashovací funkci */

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
extern LLtable table[TERMINAL_COUNT];

/**
 * @brief Externí deklarace mapy terminálů pro LL tabulku.
 *
 * @details Tato globální proměnná obsahuje seznam terminálů používaných
 *          v LL syntaktické analýze. Každý prvek pole je řetězec
 *          reprezentující jeden terminál. Mapa terminálů je použita
 *          pro účely hashování v tabulce symbolů na základě klíčů.
 */
extern const char *terminalMap[TERMINAL_COUNT];


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @enum Terminals
 * @brief Výčet terminálů používaných indexy do jejich mapovací tabulky
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v syntaktické analýze. Každý terminál vyjadřuje konrétní index
 *          do pole terminálů obsahujícího jejich klíčové slovo v podobě `*char`.
 */
typedef enum Terminals {
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
    T_PLUS                  = 12,       /**<  Operátor součtu "+"  */
    T_MINUS                 = 13,       /**<  Operátor rozdílu "-"  */
    T_MULTIPLICATION        = 14,       /**<  Operátor součinu "*"  */
    T_DIVISION              = 15,       /**<  Operátor podílu "/"  */
    T_IDENTITY              = 16,       /**<  Operátor rovnosti "=="  */
    T_NOT_EQUAL             = 17,       /**<  Operátor nerovnosti "!="  */
    T_LESS_THAN             = 18,       /**<  Operátor menší než "<"  */
    T_GREATER_THAN          = 19,       /**<  Operátor větší než ">"  */
    T_LESS_THAN_OR_EQUAL    = 20,       /**<  Operátor menší rovno "<="  */
    T_GREATER_THAN_OR_EQUAL = 21,       /**<  Operátor větší rovno ">=""  */
    T_INT                   = 22,       /**<  Klíčové slovo "i32"  */
    T_INT_OR_NULL           = 23,       /**<  Klíčové slovo "?i32"  */
    T_FLOAT                 = 24,       /**<  Klíčové slovo "f64"  */
    T_FLOAT_OR_NULL         = 25,       /**<  Klíčové slovo "?f64"  */
    T_STRING                = 26,       /**<  Klíčové slovo "[]u8"  */
    T_STRING_OR_NULL        = 27,       /**<  Klíčové slovo "?[]u8"  */
    T_VOID                  = 28,       /**<  Klíčové slovo "void"  */
    T_INT_LITERAL           = 29,       /**<  Literál typu i32  */
    T_FLOAT_LITERAL         = 30,       /**<  Literál typu i64  */
    T_STRING_LITERAL        = 31,       /**<  Literál typu []u8  */
    T_NULL_LITERAL          = 32,       /**<  Literál typu NULL  */
    T_DOT                   = 33,       /**<  Symbol tečky "."  */
    T_COMMA                 = 34,       /**<  Symbol čárky ","  */
    T_COLON                 = 35,       /**<  Symbol dvojtečky ":"  */
    T_SEMICOLON             = 36,       /**<  Symbol středníku ";"  */
    T_PIPE                  = 37,       /**<  Symbol svislice "|"  */
    T_LEFT_BRACKET          = 38,       /**<  Symbol levé závorky ")"  */
    T_RIGHT_BRACKET         = 39,       /**<  Symbol pravé závorky "("  */
    T_LEFT_CURLY_BRACKET    = 40,       /**<  Symbol levé složené závorky "{"  */
    T_RIGHT_CURLY_BRACKET   = 41,       /**<  Symbol levé složené závorky "}"  */
    T_EOF                   = 42,       /**<  Signalizace, že nastal konec souboru  */
    T_EPSILON               = 43,       /**<  Prázdný řetězec "epsilon"  */
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

/**
 * @defgroup HashedTerminals Hashované terminály
 * @brief Definice hashovaných terminálů sloužících jako klíče do tabulky symbolů.
 *
 * @details Každý hash představuje unikátní identifikátor terminálu, který se
 *          používá v syntaktické analýze a LL tabulce.
 * @{
 */

#define T_HASH_FN               5862297ULL              /**< Hash pro klíčové slovo "fn" */
#define T_HASH_ID               5862386ULL              /**< Hash pro identifikátor */
#define T_HASH_IF               5862388ULL              /**< Hash pro klíčové slovo "if" */
#define T_HASH_DOT              193453740ULL            /**< Hash pro symbol tečky "." */
#define T_HASH_EOF              193454815ULL            /**< Hash pro signalizaci konce souboru */
#define T_HASH_IFJ              193458878ULL            /**< Hash pro klíčové slovo "ifj" */
#define T_HASH_INT              193459152ULL            /**< Hash pro klíčové slovo "i32" */
#define T_HASH_PUB              193466988ULL            /**< Hash pro klíčové slovo "pub" */
#define T_HASH_VAR              193472878ULL            /**< Hash pro klíčové slovo "var" */
#define T_HASH_ELSE             6384006126ULL           /**< Hash pro klíčové slovo "else" */
#define T_HASH_PIPE             6384398067ULL           /**< Hash pro symbol svislice "|" */
#define T_HASH_PLUS             6384401513ULL           /**< Hash pro operátor součtu "+" */
#define T_HASH_VOID             6384619991ULL           /**< Hash pro klíčové slovo "void" */
#define T_HASH_COLON            210669930912ULL         /**< Hash pro symbol dvojtečky ":" */
#define T_HASH_COMMA            210669931922ULL         /**< Hash pro symbol čárky "," */
#define T_HASH_CONST            210669933228ULL         /**< Hash pro klíčové slovo "const" */
#define T_HASH_FLOAT            210673383675ULL         /**< Hash pro klíčové slovo "f64" */
#define T_HASH_MINUS            210681576881ULL         /**< Hash pro operátor rozdílu "-" */
#define T_HASH_WHILE            210693394398ULL         /**< Hash pro klíčové slovo "while" */
#define T_HASH_IMPORT           6952340304576ULL        /**< Hash pro klíčové slovo "@import" */
#define T_HASH_RETURN           6952683186021ULL        /**< Hash pro klíčové slovo "return" */
#define T_HASH_STRING           6952740025148ULL        /**< Hash pro klíčové slovo "[]u8" */
#define T_HASH_EPSILON          229422184920831ULL      /**< Hash pro prázdný řetězec "epsilon" */
#define T_HASH_DIVISION         7570880561319978ULL     /**< Hash pro operátor podílu "/" */
#define T_HASH_IDENTITY         7571086536859055ULL     /**< Hash pro operátor rovnosti "==" */
#define T_HASH_LESS             249850135850301702ULL   /**< Hash pro operátor menší než "<" */
#define T_HASH_NOT_EQUAL        249853376582133037ULL   /**< Hash pro operátor nerovnosti "!=" */
#define T_HASH_SEMICOLON        249859972537086990ULL   /**< Hash pro symbol středníku ";" */
#define T_HASH_FLOAT_LITERAL    1259126739841280167ULL  /**< Hash pro literál typu i64 */
#define T_HASH_FLOAT_OR_NULL    1259126744081274805ULL  /**< Hash pro klíčové slovo "?f64" */
#define T_HASH_LEFT_BRACKET     2663502761949992702ULL  /**< Hash pro symbol levé závorky ")" */
#define T_HASH_ASSIGNMENT       8244563632591329534ULL  /**< Hash pro operátor přiřazení "=" */
#define T_HASH_GREATER_OR_EQUAL 9421056868997489328ULL  /**< Hash pro operátor větší rovno ">=" */
#define T_HASH_GREATER          13513236659805563001ULL /**< Hash pro operátor větší než ">" */
#define T_HASH_RIGHT_BRACKET    13746110263706191467ULL /**< Hash pro symbol pravé závorky "(" */
#define T_HASH_INT_LITERAL      13828205768324589692ULL /**< Hash pro literál typu i32 */
#define T_HASH_INT_OR_NULL      13828205772564584330ULL /**< Hash pro klíčové slovo "?i32" */
#define T_HASH_NULL_LITERAL     13871966970720415084ULL /**< Hash pro literál typu NULL */
#define T_HASH_STRING_LITERAL   14265960969046774760ULL /**< Hash pro literál typu []u8 */
#define T_HASH_STRING_OR_NULL   14265960973286769398ULL /**< Hash pro klíčové slovo "?[]u8" */
#define T_HASH_LESS_OR_EQUAL       14898862961759019485ULL /**< Hash pro operátor menší rovno "<=" */
#define T_HASH_LEFT_CURLY_BRACKET  15940418408625531884ULL /**< Hash pro symbol levé složené závorky "{" */
#define T_HASH_RIGHT_CURLY_BRACKET 16850163782960150809ULL /**< Hash pro symbol pravé složené závorky "}" */
#define T_HASH_MULTIPLICATION      17446494366210133363ULL /**< Hash pro operátor součinu "*" */
/** @} */  // konec skupiny HashedTerminals


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
size_t LLtable_hashFunction(char key[]);

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
