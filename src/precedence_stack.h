
/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_stack.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            11.11.2024                                                *
 * Poslední změna:   13.11.2024                                                *
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
 * @brief Hlavičkový soubor pro správu precedenčního zásobníku.
 * @details Deklaruje funkce a datové struktury potřebné pro správu zásobníku
 *          v precedenční syntaktické analýze. Zásobník umožňuje ukládání
 *          a manipulaci s prvky, které reprezentují terminály a neterminály
 *          gramatiky. Tento zásobník slouží k řízení operací posunu a redukce,
 *          což je klíčové pro úspěšné parsování výrazů.
 */

#ifndef PRECEDENCE_STACK_H
/** @cond  */
#define PRECEDENCE_STACK_H
/** @endcond  */

#include <stdbool.h>
#include "parser.h"
#include "precedence_table.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define SN_WITHOUT_AST_PTR  NULL                    /**< Uzel "PrecStackNode" neobsahuje ukazatel na AST uzel */
#define SN_WITHOUT_AST_TYPE AST_NODE_UNDEFINED      /**< Uzel "PrecStackNode" nemá definovaný typ AST uzlu */

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
    PREC_STACK_SYM_UNEDFINED             = -1,       /**<  Typ "PrecStack" symbolu zatím není známý */
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
    PREC_STACK_SYM_ARG_LIST              = 22,       /**<  NEterminál pro seznam argumentů  */
    PREC_STACK_SYM_ARG                   = 23,       /**<  NEterminál pro argument  */
    PREC_STACK_SYM_HANDLE                = 24,       /**<  Speciální symbol "<" pro handle  */
} PrecStackSymbol;

/**
 * @brief Výčtový typ reprezentující neterminály v precedenčním zásobníku.
 *
 * @details Tento výčtový typ definuje různé neterminály, které mohou být použity
 *          v precedenčním zásobníku během syntaktické analýzy. Každý neterminál
 *          reprezentuje specifický typ neterminálu nebo speciální symbol pro handle.
 */
typedef enum PrecStackNonTerminals {
    PREC_STACK_NT_UNEDFINED     = PREC_STACK_SYM_UNEDFINED,     /**<  Typ "stack" neterminálu zatím není známý */
    PREC_STACK_NT_EXPRESSION    = PREC_STACK_SYM_EXPRESSION,    /**<  NEterminál pro výraz  */
    PREC_STACK_NT_ARG_LIST      = PREC_STACK_SYM_ARG_LIST,      /**<  NEterminál pro seznam argumentů  */
    PREC_STACK_NT_HANDLE        = PREC_STACK_SYM_HANDLE,        /**<  Speciální symbol "<" pro handle  */
} PrecStackNonTerminals;

/**
 * @brief Výčtový typ reprezentující typy uzlů v precedenčním zásobníku.
 *
 * @details Tento výčtový typ definuje různé typy uzlů, které mohou být uloženy
 *          v precedenčním zásobníku během syntaktické analýzy. Každý typ uzlu
 *          reprezentuje buď terminál, neterminál, nebo speciální symbol pro handle.
 */
typedef enum PrecStackNodeType {
    STACK_NODE_TYPE_UNDEFINED    = -1,       /**< Typ "stack" uzlu zatím není známý */
    STACK_NODE_TYPE_TERMINAL     = 1,        /**< Uzel reprezentující terminál */
    STACK_NODE_TYPE_NONTERMINAL  = 2,        /**< Uzel reprezentující neterminál */
    STACK_NODE_TYPE_HANDLE       = 3,        /**< Speciální symbol pro handle */
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
    struct PrecStack *next;
} PrecStack;

typedef struct PrecStackList {
    PrecStack *stack;
} PrecStackList;

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
extern struct PrecStackList *precStackList;


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE VEŘEJNÝCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje globální seznam precedenčních zásobníků.
 */
void PrecStackList_create();

/**
 * @brief Přidá nový precedenční zásobník na vrchol seznamu a inicializuje jej.
 */
void PrecStackList_push();

/**
 * @brief Odebere a uvolní vrcholový zásobník ze seznamu (bez uvolnění AST uzlů).
 */
void PrecStackList_popAndDispose();

/**
 * @brief Odebere a uvolní vrcholový zásobník ze seznamu (včetně uvolnění AST uzlů).
 */
void PrecStackList_popAndPurge();

/**
 * @brief Uvolní všechny zásobníky ze seznamu pomocí popAndDispose a následně uvolní samotný seznam.
 */
void PrecStackList_dispose();

/**
 * @brief Uvolní všechny zásobníky ze seznamu pomocí popAndPurge a následně uvolní samotný seznam.
 */
void PrecStackList_purge();

/**
 * @brief Uvolní globální strukturu seznamu zásobníků.
 */
void PrecStackList_destroy();

/**
 * @brief Pushne terminál na globální precedenční zásobník.
 *
 * @details Tato funkce přidá nový terminál na vrchol precedenčního zásobníku.
 *
 * @param [in] symbol Terminál k pushnutí.
 * @param [in] type Typ AST uzlu spojeného s terminálem.
 * @param [in] node Ukazatel na AST uzel spojený s terminálem.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack.
 */
void PrecStack_pushPrecTerminal(PrecTerminals symbol, AST_NodeType type, void *node);

/**
 * @brief Pushne NEterminál na globální precedenční zásobník.
 *
 * @details Tato funkce přidá nový NEterminál na vrchol precedenčního zásobníku.
 *
 * @param [in] symbol NEterminál k pushnutí.
 * @param [in] type Typ AST uzlu spojeného s NEterminál.
 * @param [in] node Ukazatel na AST uzel spojený s NEterminál.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack.
 */
void PrecStack_pushPrecNonTerminal(PrecStackNonTerminals symbol, AST_NodeType type, void *node);

/**
 * @brief Pushne inicializovaný Stack uzel na zásobník, popř. i s AST uzlem.
 *
 * @details Tato funkce vytváří a inicializuje uzly AST na základě typu terminálu
 *          a pushuje je na zásobník. Pokud je terminál typu identifikátor nebo
 *          literál, vytvoří se odpovídající uzel AST a inicializuje se. Poté se
 *          uzel pushne na zásobník. Pokud je terminál jiného typu, pushne se na
 *          zásobník bez vytvoření AST uzlu.
 *
 * @param inTerminal Typ terminálu, který má být pushnut na zásobník.
 * @param bracketDepth Úroveň zanoření závorek.
 */
void PrecStack_pushBothStackAndASTNode(PrecTerminals inTerminal);

void PrecStack_pushHandleAfterFirstTerminal();

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
 * @brief Uvolní všechny zdroje spojené s uzlem `PrecStackNode`.
 *
 * @details Tato funkce uvolní všechny zdroje spojené s uzlem `PrecStackNode`,
 *          včetně volání funkce `AST_destroyNode` pro uvolnění `void *node`.
 *
 * @param node Ukazatel na uzel `PrecStackNode`, který má být uvolněn.
 */
void PrecStack_freeNode(PrecStackNode *node);

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku pomocí
 *        funkce PrecStack_freeNode a nakonec zničí zásobník.
 */
void PrecStack_purgeStack(PrecStack *stack);

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku a
 *        inicializuje ho do počátečního stavu.
 *
 * @details Tato funkce uvolní veškerou paměť alokovanou pro uzly stacku a
 *          inicializuje stack jako prázdný.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack, který má být uvolněn.
 */
void PrecStack_dispose(PrecStack *stack);

/**
 * @brief Uvolní z paměti samotnou strukturu precedenčního zásobníku.
 *
 * @details Zavolá @c free() nad globálním ukazatel na precedenční zásobník.
 *
 * @note `param [in] stack`: Ukazatel na @c PrecStack, který má být uvolněn.
 *       Funkce je definována jako @c inline.
 */
void PrecStack_destroy();

/**
 * @brief Získá terminál na vrcholu zásobníku, který je nejblíže vrcholu.
 *
 * @details Tato funkce prochází zásobník od vrcholu směrem dolů, dokud nenajde
 *          první terminál. Pokud na zásobníku není žádný terminál, hlásí interní chybu.
 *
 * @param [out] terminal Ukazatel na proměnnou, do které bude uložen nalezený
 *              terminál.
 */
void PrecStack_getTopPrecTerminal(PrecTerminals *terminal);

/**
 * @brief Zkontroluje, zda je na vrcholu zásobníku symbol HANDLE.
 */
bool PrecStack_isIdOnTop();

/**
 * @brief Zkontroluje, zda je na vrcholu zásobníku symbol HANDLE.
 *
 * @note `param [in] stack`: Ukazatel na zásobník, který má být zkontrolován.
 *
 * @return @c true, pokud je na vrcholu zásobníku symbol HANDLE, jinak @c false.
 */
bool PrecStack_isHandleOnTop();

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

/**
 * @brief Získá výsledný AST uzel výrazu z precedenčního zásobníku.
 *
 * @param result Ukazatel na proměnnou, kam bude uložen výsledný AST_ExprNode *.
 * @return true při úspěchu, false pokud došlo k chybě.
 */
bool PrecStack_getResult(AST_ExprNode **result);


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE INTERNÍCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytvoří nový uzel typu `PrecStackNode`.
 *
 * @details Tato funkce alokuje paměť pro nový uzel typu `PrecStackNode`
 *          a inicializuje jeho členy na výchozí hodnoty. Pokud se alokace
 *          paměti nezdaří, funkce zavolá `error_handle` s chybovým kódem
 *          `ERROR_INTERNAL` a vrátí `NULL`.
 *
 * @return Ukazatel na nově vytvořený uzel typu `PrecStackNode`, nebo `NULL` v případě chyby alokace.
 */
PrecStackNode *PrecStack_createStackNode();

/**
 * @brief Namapuje typ precedenčního terminálu na typ zásobníkového symbolu.
 *
 * @details Tato funkce přijímá precedenční terminál a mapuje jej na odpovídající
 *          zásobníkový symbol. Pokud je předán neterminál nebo handle, funkce
 *          zavolá `error_handle` s chybovým kódem `ERROR_INTERNAL`.
 *
 * @param [in] terminal Precedenční terminál, který se má namapovat.
 * @param [out] stackSymbol Ukazatel na zásobníkový symbol, který bude nastaven.
 */
void PrecStack_mapPrecTerminalToStackSymbol(PrecTerminals terminal, PrecStackSymbol *stackSymbol);

/**
 * @brief Namapuje typ zásobníkového symbolu na typ precedenčního terminálu.
 *
 * @details Tato funkce přijímá zásobníkový symbol a mapuje jej na odpovídající
 *          precedenční terminál. Pokud je předán neterminál nebo handle, funkce
 *          zavolá `error_handle` s chybovým kódem `ERROR_INTERNAL`.
 *
 * @param [in] stackSymbol Zásobníkový symbol, který se má namapovat.
 * @param [out] terminal Ukazatel na precedenční terminál, který bude nastaven.
 */
void PrecStack_mapStackSymbolToPrecTerminal(PrecStackSymbol stackSymbol, PrecTerminals *terminal);

/**
 * @brief Namapuje typ zásobníkového neterminálu na typ zásobníkového symbolu.
 *
 * @details Tato funkce mapuje typ zásobníkového neterminálu na odpovídající typ
 *          zásobníkového symbolu. Pokud je předán neplatný ukazatel, funkce
 *          hlásí interní chybu.
 *
 * @param [in] stackNonTerminal Zásobníkový neterminál, který má být namapován.
 * @param [out] symbol Ukazatel na zásobníkový symbol, do kterého bude uložena
 *                     namapovaná hodnota.
 */
void PrecStack_mapStackNonTerminalToStackSymbol(PrecStackNonTerminals stackNonTerminal, PrecStackSymbol *symbol);

#endif // PRECEDENCE_STACK_H

/*** Konec souboru precedence_stack.h ***/
