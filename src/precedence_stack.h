
/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_stack.h                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            11.11.2024                                                *
 * Poslední změna:   23.11.2024                                                *
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
 * @details Tento soubor deklaruje funkce a datové struktury pro práci s
 *          precedenčním zásobníkem, který je používán při syntaktické analýze.
 *          Zásobník umožňuje manipulaci s terminály, neterminály a speciálními
 *          symboly (např. `<HANDLE>`). Obsahuje také funkce pro správu paměti,
 *          přidávání a odebírání uzlů a operace pro přechody mezi stavy parseru.
 */

#ifndef PRECEDENCE_STACK_H
/** @cond  */
#define PRECEDENCE_STACK_H
/** @endcond  */

// Import submodulů parseru
#include "parser_common.h"


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
 * @details Tento výčet definuje všechny terminály a speciální symboly, které
 *          mohou být uloženy v precedenčním zásobníku během syntaktické analýzy.
 */
typedef enum PrecStackSymbol {
    PREC_STACK_SYM_UNEDFINED             = -1,       /**<  Typ "PrecStack" symbolu zatím není známý         */
    PREC_STACK_SYM_ID                    = 0,        /**<  Identifikátor                                    */
    PREC_STACK_SYM_INT_LITERAL           = 1,        /**<  Literál typu i32                                 */
    PREC_STACK_SYM_FLOAT_LITERAL         = 2,        /**<  Literál typu i64                                 */
    PREC_STACK_SYM_STRING_LITERAL        = 3,        /**<  Literál typu []u8                                */
    PREC_STACK_SYM_NULL_LITERAL          = 4,        /**<  Literál typu NULL                                */
    PREC_STACK_SYM_IFJ                   = 5,        /**<  Klíčové slovo "ifj"                              */
    PREC_STACK_SYM_DOT                   = 6,        /**<  Symbol tečky "."                                 */
    PREC_STACK_SYM_LEFT_BRACKET          = 7,        /**<  Symbol levé závorky "("                          */
    PREC_STACK_SYM_RIGHT_BRACKET         = 8,        /**<  Symbol pravé závorky ")"                         */
    PREC_STACK_SYM_PLUS                  = 9,        /**<  Operátor součtu "+"                              */
    PREC_STACK_SYM_MINUS                 = 10,       /**<  Operátor rozdílu "-"                             */
    PREC_STACK_SYM_MULTIPLICATION        = 11,       /**<  Operátor součinu "*"                             */
    PREC_STACK_SYM_DIVISION              = 12,       /**<  Operátor podílu "/"                              */
    PREC_STACK_SYM_IDENTITY              = 13,       /**<  Operátor rovnosti "=="                           */
    PREC_STACK_SYM_NOT_EQUAL             = 14,       /**<  Operátor nerovnosti "!="                         */
    PREC_STACK_SYM_LESS_THAN             = 15,       /**<  Operátor menší než "<"                           */
    PREC_STACK_SYM_GREATER_THAN          = 16,       /**<  Operátor větší než ">"                           */
    PREC_STACK_SYM_LESS_THAN_OR_EQUAL    = 17,       /**<  Operátor menší rovno "<="                        */
    PREC_STACK_SYM_GREATER_THAN_OR_EQUAL = 18,       /**<  Operátor větší rovno ">="                        */
    PREC_STACK_SYM_COMMA                 = 19,       /**<  Symbol čárky "," uvnitř argumentů volání funkce  */
    PREC_STACK_SYM_DOLLAR                = 20,       /**<  Řízení parsování předáno LL parseru              */
    PREC_STACK_SYM_EXPRESSION            = 21,       /**<  NEterminál pro výraz                             */
    PREC_STACK_SYM_ARG_LIST              = 22,       /**<  NEterminál pro seznam argumentů                  */
    PREC_STACK_SYM_ARG                   = 23,       /**<  NEterminál pro argument                          */
    PREC_STACK_SYM_HANDLE                = 24,       /**<  Speciální symbol "<" pro handle                  */
} PrecStackSymbol;

/**
 * @brief Výčtový typ reprezentující symboly neterminálů v precedenčním zásobníku.
 *
 * @details Tento výčtový typ definuje různé neterminály, které mohou být použity
 *          v precedenčním zásobníku během syntaktické analýzy. Každý neterminál
 *          reprezentuje specifický typ neterminálu nebo speciální symbol pro handle.
 */
typedef enum PrecStackNonTerminals {
    PREC_STACK_NT_UNEDFINED  = -1,      /**<  Typ "stack" neterminálu zatím není známý */
    PREC_STACK_NT_EXPRESSION = 0,       /**<  NEterminál pro výraz  */
    PREC_STACK_NT_ARG_LIST   = 1,       /**<  NEterminál pro seznam argumentů  */
    PREC_STACK_NT_HANDLE     = 2,       /**<  Speciální symbol "<" pro handle  */
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
 * @brief Struktura reprezentující uzel precedenčního zásobníku.
 *
 * @details Tato struktura reprezentuje jednotlivý uzel v precedenčním zásobníku.
 *          Každý uzel obsahuje informace o typu symbolu (terminál/neterminál/handle),
 *          samotný symbol, typ uzlu v AST, ukazatel na uzel AST spojený s tímto
 *          symbolem a ukazatel na další uzel ve stacku. AST uzly obsahují
 *          pouze zásobníkové uzly obsahující identifikátor, literál nebo
 *          neterminál pro výraz či seznam argumentů funkce
 */
typedef struct PrecStackNode {
    PrecStackNodeType    symbolType;       /**< Typ symbolu (terminál nebo neterminál nebo handle) */
    PrecStackSymbol      symbol;           /**< Terminál nebo neterminál uložený v uzlu */
    AST_NodeType         nodeType;         /**< Typ AST uzlu */
    void                 *node;            /**< Ukazatel na uzel AST spojený s terminálem nebo neterminálem */
    struct PrecStackNode *next;            /**< Ukazatel na další uzel ve stacku */
} PrecStackNode;

/**
 * @brief Struktura reprezentující jeden precedenční zásobník.
 *
 * @details Tato struktura reprezentuje celý precedenční zásobník. Obsahuje
 *          ukazatel na vrchol zásobníku, což umožňuje efektivní provádění
 *          operací push, pop a top.
 */
typedef struct PrecStack {
    PrecStackNode *top;             /**< Ukazatel na vrchol zásobníku */
    struct PrecStack *next;         /**< Ukazatel na další zásobník v seznamu */
} PrecStack;

/**
 * @brief Struktura reprezentující seznam precedenčních zásobníků.
 *
 * @details Obsahuje ukazatel na aktuální zásobník používaný během analýzy.
 *
 * @note Nejedná se ani tolik o seznam precedenčních zásobníků než spíše
 *       o zásobník všech aktuálně využívaných precedenčních zásobníků.
 *       Pojem "seznam" jsem zvolil s cílem lépe rozlišit jednotlivé struktury
 *       a příslušné funkce.
 */
typedef struct PrecStackList {
    PrecStack *stack;               /**< Ukazatel na aktuální zásobník */
} PrecStackList;

/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

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
 *                         DEKLARACE VEŘEJNÝCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje globální seznam precedenčních zásobníků.
 *
 * @details Tato funkce alokuje paměť pro globální seznam precedenčních zásobníků
 *          a inicializuje jej. Pokud se alokace nezdaří, hlásí interní chybu.
 */
void PrecStackList_create();

/**
 * @brief Uvolní globální strukturu seznamu zásobníků.
 *
 * @details Tato funkce uvolní paměť alokovanou pro globální seznam zásobníků.
 *          Pokud je seznam prázdný, funkce neprovádí žádnou akci.
 */
void PrecStackList_destroy();

/**
 * @brief Přidá nový precedenční zásobník na vrchol seznamu a inicializuje jej.
 *
 * @details Tato funkce vytvoří nový precedenční zásobník, inicializuje jej a
 *          přidá jej na vrchol globálního seznamu zásobníků. Pokud se alokace
 *          nezdaří, hlásí interní chybu.
 */
void PrecStackList_push();

/**
 * @brief Odebere a uvolní vrcholový zásobník ze seznamu (bez uvolnění AST uzlů).
 *
 * @details Tato funkce odebere vrcholový zásobník ze seznamu a uvolní jeho paměť,
 *          aniž by uvolnila paměť pro uzly AST. Pokud je seznam prázdný, funkce
 *          neprovádí žádnou akci.
 */
void PrecStackList_pop();

/**
 * @brief Uvolní všechny zásobníky a jejich obsah ze seznamu
 *        pomocí @c PrecStack_purge() (včetně uvolnění AST uzlů).
 *
 * @details Tato funkce uvolní všechny zásobníky ze seznamu pomocí funkce
 *          @c PrecStack_purge(). Pokud je seznam prázdný, funkce neprovádí
 *          žádnou akci.
 */
void PrecStackList_purge();

/**
 * @brief Pushne precedenční terminál na aktuální precedenční zásobník.
 *
 * @details Tato funkce přidá nový terminál na vrchol precedenčního zásobníku.
 *          Pokud není zásobník alokovaný, funkce neprovádí žádnou akci.
 *
 * @param [in] symbol Terminál k pushnutí.
 * @param [in] type Typ AST uzlu spojeného s terminálem.
 *                  (typ `AST_NodeType' nebo `SN_WITHOUT_AST_TYPE`)
 * @param [in] node Ukazatel na AST uzel spojený s terminálem.
 *                  (ukazatel na uzel AST nebo `SN_WITHOUT_AST_PTR`)
 */
void PrecStack_pushPrecTerminal(PrecTerminals symbol, AST_NodeType type, void *node);

/**
 * @brief Pushne NEterminál na globální precedenční zásobník.
 *
 * @details Tato funkce přidá nový NEterminál na vrchol precedenčního zásobníku.
 *          Pokud není zásobník alokovaný, funkce neprovádí žádnou akci.
 *
 * @param [in] symbol NEterminál k pushnutí.
 * @param [in] type Typ AST uzlu spojeného s NEterminálem
 *                  (typ `AST_NodeType' nebo `SN_WITHOUT_AST_TYPE`)
 * @param [in] node Ukazatel na AST uzel spojený s NEterminálem.
 *                  (ukazatel na uzel AST nebo `SN_WITHOUT_AST_PTR`)
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
 * @param [in] inTerminal Typ terminálu, který má být pushnut na zásobník.
 */
void PrecStack_pushBothStackAndASTNode(PrecTerminals inTerminal);

/**
 * @brief Pushne handle za první terminál na zásobníku.
 *
 * @details Tato funkce najde první terminál na zásobníku a vloží za něj handle.
 *          Pokud není zásobník alokovaný nebo neobsahuje žádný terminál, hlásí
 *          interní chybu.
 */
void PrecStack_pushHandleAfterFirstTerminal();

/**
 * @brief Popne uzel AST z globálního precedenčního zásobníku.
 *
 * @details Tato funkce odstraní vrcholový element ze stacku a vrátí
 *          ukazatel na tento uzel. Pokud je zásobník prázdný, vrátí @c NULL.
 *
 * @return Ukazatel na popnutý uzel, nebo @c NULL pokud je stack prázdný.
 */
PrecStackNode *PrecStack_pop();

/**
 * @brief Získá ukazatel na uzel na vrcholu globálního precedenčního zásobníku
 *        bez jeho popnutí.
 *
 * @details Tato funkce vrátí ukazatel na vrcholový element stacku bez jeho
 *          odstranění. Pokud je zásobník prázdný, hlásí interní chybu.
 *
 * @return Ukazatel na vrcholový uzel stacku, nebo @c NULL pokud je stack prázdný.
 */
PrecStackNode *PrecStack_top();

/**
 * @brief Uvolní všechny zdroje spojené s uzlem @c PrecStackNode.
 *
 * @details Tato funkce uvolní všechny zdroje spojené s uzlem @c PrecStackNode,
 *          včetně volání funkce @c AST_destroyNode() pro uvolnění `void *node`.
 *
 * @param [in] node Ukazatel na uzel @c PrecStackNode, který má být uvolněn.
 */
void PrecStack_freeNode(PrecStackNode *stackNode);

/**
 * @brief Získá terminál na vrcholu zásobníku, který je nejblíže vrcholu.
 *
 * @details Tato funkce prochází zásobník od vrcholu směrem dolů, dokud nenajde
 *          první terminál. Pokud na zásobníku není žádný terminál, hlásí
 *          interní chybu.
 *
 * @param [out] terminal Ukazatel na proměnnou, do které bude uložen nalezený
 *              terminál.
 */
void PrecStack_getTopPrecTerminal(PrecTerminals *topTerminal);

/**
 * @brief Zkontroluje, zda je na vrcholu zásobníku symbol ID.
 *
 * @details Tato funkce zkontroluje, zda je na vrcholu zásobníku symbol ID.
 *
 * @return @c true, pokud je na vrcholu zásobníku symbol ID, jinak @c false.
 */
bool PrecStack_isIdOnTop();

/**
 * @brief Získá výsledný AST uzel výrazu z precedenčního zásobníku.
 *
 * @details Tato funkce získá výsledný AST uzel výrazu z precedenčního zásobníku.
 *          Pokud je zásobník prázdný nebo vrcholový uzel neobsahuje výraz, hlásí
 *          interní chybu.
 *
 * @param [out] result Ukazatel na proměnnou, kam bude uložen výsledný `AST_ExprNode *`.
 */
void PrecStack_getResult(AST_ExprNode **result);


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE INTERNÍCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku.
 *
 * @details Tato funkce uvolní veškerou paměť alokovanou pro uzly stacku, ale
 *          obsažené uzly AST nechává alokované.
 *
 * @param stack Ukazatel na zásobník, který má být uvolněn.
 */
void PrecStack_dispose(PrecStack *stack);


/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku pomocí
 *        funkce @c PrecStack_freeNode().
 *
 * @details Tato funkce uvolní všechny uzly z globálního precedenčního zásobníku
 *          pomocí funkce @c PrecStack_freeNode(). Uvolňuje tedy  zásobníkové
 *          uzly včetně případných AST uzlů uvnitř nich.
 *
 * @param stack Ukazatel na zásobník, který má být uvolněn.
 */
void PrecStack_purge(PrecStack *stack);

/**
 * @brief Zkontroluje, zda je aktuální precedenční zásobník prázdný.
 *
 * @details Tato funkce kontroluje, zda je zásobník prázdný tím, že ověří, zda
 *          ukazatel na vrchol zásobníku (top) je @c NULL.
 *
 * @return @c true, pokud je zásobník prázdný, jinak @c false.
 */
bool PrecStack_isEmpty();

/**
 * @brief Vytvoří nový zásobníkový uzel typu typu @c PrecStackNode.
 *
 * @details Tato funkce alokuje paměť pro nový uzel typu @c PrecStackNode
 *          a inicializuje jeho členy na výchozí hodnoty. Pokud se alokace
 *          paměti nezdaří, funkce hlásí interní chybu a vrácí @c NULL.
 *
 * @return Ukazatel na nově vytvořený uzel typu @c PrecStackNode,
 *         nebo @c NULL v případě chyby alokace.
 */
PrecStackNode *PrecStack_createStackNode();

/**
 * @brief Namapuje typ precedenčního terminálu na typ zásobníkového symbolu.
 *
 * @details Tato funkce přijímá precedenční terminál a mapuje jej na odpovídající
 *          zásobníkový symbol. Pokud je předán neplatný ukazatel, funkce
 *          hlásí interní chybu. Pokud je předán neterminál nebo handle, funkce
 *          zavolá @c Parser_errorWatcher() a nastavuje chybu typu @c ERROR_INTERNAL
 *
 * @param [in] terminal Precedenční terminál, který se má namapovat.
 * @param [out] stackSymbol Ukazatel na zásobníkový symbol, který bude nastaven.
 */
void PrecStack_mapPrecTerminalToStackSymbol(PrecTerminals terminal, PrecStackSymbol *stackSymbol);

/**
 * @brief Namapuje typ zásobníkového symbolu na typ precedenčního terminálu.
 *
 * @details Tato funkce přijímá zásobníkový symbol a mapuje jej na odpovídající
 *          precedenční terminál. Pokud je předán neplatný ukazatel, funkce
 *          hlásí interní chybu. Pokud je předán neterminál nebo handle, funkce
 *          zavolá @c Parser_errorWatcher() a nastavuje chybu typu @c ERROR_INTERNAL.
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
 *          hlásí interní chybu. zavolá @c Parser_errorWatcher() a nastavuje
 *          chybu typu @c ERROR_INTERNAL
 *
 * @param [in] stackNonTerminal Zásobníkový neterminál, který má být namapován.
 * @param [out] symbol Ukazatel na zásobníkový symbol, do kterého bude uložena
 *                     namapovaná hodnota.
 */
void PrecStack_mapStackNonTerminalToStackSymbol(PrecStackNonTerminals stackNonTerminal, \
                                                PrecStackSymbol *symbol);

#endif // PRECEDENCE_STACK_H

/*** Konec souboru precedence_stack.h ***/
