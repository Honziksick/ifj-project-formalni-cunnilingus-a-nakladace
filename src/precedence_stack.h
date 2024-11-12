
/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_stack.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            11.11.2024                                                *
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
 * @file precedence_stack.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro precedenční zásobník v syntaktické analýze.
 * @details Tento hlavičkový soubor obsahuje deklarace datových struktur a funkcí
 *          potřebných pro správu zásobníku používaného v precedenční syntaktické
 *          analýze zdrojového kódu jazyka IFJ24. Zásobník je klíčovou komponentou
 *          parseru, který umožňuje efektivně provádět operace posunu a redukce
 *          během analýzy. Zásobník pracuje s prvky typu `LLTerminals`, které
 *          reprezentují terminály a NEterminály v gramatice jazyka. Soubor
 *          zahrnuje funkce pro inicializaci, uvolňování paměti, kontrolu
 *          prázdnosti, a manipulaci s vrcholem zásobníku.
 */

#ifndef PRECEDENCE_STACK_H
/** @cond  */
#define PRECEDENCE_STACK_H
/** @endcond  */

#include "ast_nodes.h"
#include "ast_interface.h"
#include "precedence_table.h"
#include "lltable.h"
#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčet terminálů používaných v precedenční syntaktické analýze.
 *
 * @details Tento výčet obsahuje všechny terminály, které mohou být použity
 *          v precedenční syntaktické analýze.
 */
typedef enum PrecStackSymbol {
    PREC_STACK_SYM_ID                    = 0,        /**<  Identifikátor  */
    PREC_STACK_SYM_INT_LITERAL           = 1,        /**<  Literál typu i32  */
    PREC_STACK_SYM_FLOAT_LITERAL         = 2,        /**<  Literál typu i64  */
    PREC_STACK_SYM_STRING_LITERAL        = 3,        /**<  Literál typu []u8  */
    PREC_STACK_SYM_NULL_LITERAL          = 4,        /**<  Literál typu NULL  */
    PREC_STACK_SYM_IFJ                   = 5,        /**<  Klíčové slovo "ifj"  */
    PREC_STACK_SYM_DOT                   = 6,        /**<  Symbol tečky "."  */
    PREC_STACK_SYM_LEFT_BRACKET          = 7,        /**<  Symbol levé závorky "("  */
    PREC_STACK_SYM_RIGHT_BRACKET         = 8,        /**<  Symbol pravé závorky ")"  */
    PREC_STACK_SYM_PLUS                  = 9,        /**<  Operátor součtu "+"  */
    PREC_STACK_SYM_MINUS                 = 10,       /**<  Operátor rozdílu "-"  */
    PREC_STACK_SYM_MULTIPLICATION        = 11,       /**<  Operátor součinu "*"  */
    PREC_STACK_SYM_DIVISION              = 12,       /**<  Operátor podílu "/"  */
    PREC_STACK_SYM_IDENTITY              = 13,       /**<  Operátor rovnosti "=="  */
    PREC_STACK_SYM_NOT_EQUAL             = 14,       /**<  Operátor nerovnosti "!="  */
    PREC_STACK_SYM_LESS_THAN             = 15,       /**<  Operátor menší než "<"  */
    PREC_STACK_SYM_GREATER_THAN          = 16,       /**<  Operátor větší než ">"  */
    PREC_STACK_SYM_LESS_THAN_OR_EQUAL    = 17,       /**<  Operátor menší rovno "<="  */
    PREC_STACK_SYM_GREATER_THAN_OR_EQUAL = 18,       /**<  Operátor větší rovno ">="  */
    PREC_STACK_SYM_COMMA                 = 19,       /**<  Symbol čárky "," uvnitř argumentů volání funkce  */
    PREC_STACK_SYM_DOLLAR                = 20,       /**<  Řízení parsování předáno LL parseru */
    PREC_STACK_SYM_EXPRESSION            = 21,       /**<  NEterminál pro výraz  */
    PREC_STACK_SYM_ARGUMENTS             = 22,       /**<  NEterminál pro argumenty funkce */
    PREC_STACK_SYM_ARG_LIST              = 23,       /**<  NEterminál pro seznam argumentů  */
    PREC_STACK_SYM_ARG                   = 24,       /**<  NEterminál pro argument  */
    PREC_STACK_SYM_HANDLE                = 25,       /**<  Speciální symbol "<" pro handle  */
} PrecStackSymbol;

/**
 * @brief Výčtový typ reprezentující typy uzlů v precedenčním zásobníku.
 *
 * @details Tento výčtový typ definuje různé typy uzlů, které mohou být uloženy
 *          v precedenčním zásobníku během syntaktické analýzy. Každý typ uzlu
 *          reprezentuje buď terminál, neterminál, nebo speciální symbol pro handle.
 */
typedef enum PrecStackNodeType {
    STACK_NODE_TYPE_TERMINAL     = 1,        /**< Uzel reprezentující terminál */
    STACK_NODE_TYPE_NONTERMINAL  = 2,        /**< Uzel reprezentující neterminál */
    STACK_NODE_TYPE_HANDLE       = 3,        /**< Speciální marker pro handle */
} PrecStackNodeType;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura reprezentující uzel stacku.
 *
 * @details Tato struktura reprezentuje jednotlivý uzel v precedenčním zásobníku.
 *          Každý uzel obsahuje informace o typu symbolu (terminál nebo neterminál),
 *          samotný symbol, typ uzlu v AST, ukazatel na uzel AST spojený s tímto
 *          symbolem a ukazatel na další uzel ve stacku.
 */
typedef struct PrecStackNode {
    PrecStackNodeType    symbolType;       /**< Typ symbolu (terminál nebo neterminál nebo handle) */
    PrecStackSymbol      symbol;           /**< Terminál nebo neterminál uložený v uzlu */
    AST_NodeType         nodeType;         /**< Typ AST uzlu */
    void                 *node;            /**< Ukazatel na uzel AST spojený s terminálem nebo neterminálem */
    struct PrecStackNode *next;            /**< Ukazatel na další uzel ve stacku */
} PrecStackNode;

/**
 * @brief Struktura reprezentující stack.
 *
 * @details Tato struktura reprezentuje celý precedenční zásobník. Obsahuje
 *          ukazatel na vrchol zásobníku, což umožňuje efektivní provádění
 *          operací push, pop a top.
 */
typedef struct PrecStack {
    PrecStackNode *top;           /**< Ukazatel na vrchol stacku */
} PrecStack;

/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro precedenční zásobník.
 *
 * @details Tato proměnná uchovává ukazatel na aktuální precedenční zásobník,
 *          který je používán během syntaktické analýzy. Zásobník je inicializován
 *          na začátku analýzy a uvolněn po jejím dokončení.
 */
extern struct PrecStack *precStack;


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE VEŘEJNÝCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Alokuje paměť pro globální precedenční zásobník.
 *
 * @details Tato funkce alokuje paměť pro globální proměnnou `precStack`,
 *          která představuje precedenční zásobník. Pokud se alokace paměti
 *          nezdaří, funkce zavolá funkci `error_handle()` s chybovým kódem
 *          `ERROR_INTERNAL`.
 *
 * @note Alokovaný stack je globální, proto funkci voláme pouze jako
 *       `PrecStack_create();`
 */
void PrecStack_create();

/**
 * @brief Inicializuje globální precedenční zásobník.
 *
 * @details Tato funkce inicializuje prázdný precedenční zásobník.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack, který má být inicializován.
 */
void PrecStack_init();

/**
 * @brief Pushne NE/terminál na globální precedenční zásobník.
 *
 * @details Tato funkce přidá nový NE/terminál na vrchol precedenčního zásobníku.
 *
 * @param [in] symbol NE/terminál k pushnutí.
 * @param [in] type Typ AST uzlu spojeného s NE/terminálem.
 * @param [in] node Ukazatel na AST uzel spojený s NE/terminálem.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack.
 */
void PrecStack_push(PrecStackSymbol symbol, AST_NodeType type, void *node);

/**
 * @brief Popne uzel AST z globálního precedenčního zásobníku.
 *
 * @details Tato funkce odstraní vrcholový element ze stacku a vrátí
 *          ukazatel na tento uzel.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack.
 *
 * @return Ukazatel na popnutý uzel, nebo @c NULL pokud je stack prázdný.
 */
PrecStackNode* PrecStack_pop();

/**
 * @brief Získá ukazatel na uzel na vrcholu globálního precedenčního zásobníku
 *        bez jeho popnutí.
 *
 * @details Tato funkce vrátí ukazatel na vrcholový element stacku bez jeho odstranění.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack.
 *
 * @return Ukazatel na vrcholový uzel stacku, nebo @c NULL pokud je stack prázdný.
 */
PrecStackNode* PrecStack_top();

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku a
 *        inicializuje ho do počátečního stavu.
 *
 * @details Tato funkce uvolní veškerou paměť alokovanou pro uzly stacku a
 *          inicializuje stack jako prázdný.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack, který má být uvolněn.
 */
void PrecStack_dispose();


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE INTERNÍCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Zkontroluje, zda je precedenční zásobník prázdný.
 *
 * @details Tato funkce kontroluje, zda je zásobník prázdný tím, že ověří, zda
 *          ukazatel na vrchol zásobníku (`top`) je @c NULL.
 *
 * @note `param [in] stack`: Ukazatel na zásobník, který má být zkontrolován.
 *
 * @return @c true, pokud je zásobník prázdný, jinak @c false.
 */
bool PrecStack_isEmpty();

#endif // PRECEDENCE_STACK_H

/*** Konec souboru precedence_stack.h ***/
