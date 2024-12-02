/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_nodes.h                                               *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   01.12.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ast_nodes.h
 * @author Jan Kalina \<xkalinj00> (návrh, hlavičkový soubor)
 * @author David Krejčí \<xkrejcd00> (návrh)
 *
 * @brief Hlavičkový soubor pro uzly abstraktního syntaktického stromu (AST).
 * @details Tento soubor obsahuje deklarace výčtů a datových struktur uzlů
 *          abstraktního syntaktického stromu (AST), který je základem
 *          pro sémantickou analýzu a generování vnitřního kódu v překladači.
 *          Definuje typy uzlů příkazů, výrazů, literálů a datových typů
 *          používaných v AST. Dále zahrnuje základní struktury jako jsou uzly
 *          pro program, funkce, parametry, příkazy, výrazy apod.
 */

#ifndef AST_NODES_H_
/** @cond  */
#define AST_NODES_H_
/** @endcond  */

// Import standardních knihoven jazyka C
#include <stdbool.h>

// Import sdílených knihoven překladače
#include "dynamic_string.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define AST_FRAME_ID_NOT_ASSIGNED 0     /**< Uzlu `AST_StatementNode` nebo `AST_VarNode` ještě nebylo přiřazeno `frameID`. */
#define AST_VAL_UNDEFINED NULL          /**< Uzel `AST_VarNode` pro proměnnou neobsahuje hodnotu literálu. */
#define AST_ID_UNDEFINED NULL           /**< Uzel `AST_VarNode` pro literál neobsahuje identifikátor. */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Typy uzlů abstraktního sytaktického stromu (AST).
 *
 * @details Reprezentuje různé typy uzlů v abstraktním syntaktickém stromu (AST).
 */
typedef enum AST_NodeType {
    // Nedefinovaný uzel
    AST_NODE_UNDEFINED          = -1,           /**< Typ uzlu zatím není definován.    */

    // Struktury programu
    AST_PROGRAM_NODE            = 1,            /**< Uzel pro celý program.             */
    AST_FUN_DEF_NODE            = 2,            /**< Uzel pro definici funkce.          */
    AST_ARG_OR_PARAM_NODE       = 3,            /**< Uzel pro argument/parametr funkce. */
    AST_STATEMENT_NODE          = 4,            /**< Uzel pro příkaz.                   */

    // Příkazy
    AST_FUN_CALL_NODE           = 5,            /**< Uzel pro volání funkce.            */
    AST_IF_NODE                 = 6,            /**< Uzel pro podmíněný příkaz "if".    */
    AST_WHILE_NODE              = 7,            /**< Uzel pro cyklus "while".           */

    // Výrazy
    AST_EXPR_NODE               = 8,            /**< Uzel pro výraz.                    */
    AST_BIN_OP_NODE             = 9,            /**< Uzel pro binární operaci.          */
    AST_LITERAL_NODE            = 10,           /**< Uzel pro literál.                  */
    AST_VAR_NODE                = 11,           /**< Uzel pro proměnnou.                */
} AST_NodeType;

/**
 * @brief Typy příkazů uchovávané v abstraktním sytaktickém stromu (AST).
 *
 * @details Výčet reprezentuje různé typy příkazů, které mohou být použity v
 *          abstraktním syntaktickém stromu (AST). Přiřazením této hodnoty
 *          uzlu @c AST_StatementNode lze určit, jakým způsobem dereferencovat
 *          ukazetel na @c void, odkazující se na jeden z povolených typů
 *          příkazů.
 */
typedef enum AST_StatementType {
    AST_STATEMENT_NOT_DEFINED   = 0,            /**< Typ příkazu není definován */
    AST_STATEMENT_VAR_DEF       = 1,            /**< Definice proměnné          */
    AST_STATEMENT_EXPR          = 2,            /**< Výraz                      */
    AST_STATEMENT_FUN_CALL      = 3,            /**< Volání funkce              */
    AST_STATEMENT_IF            = 4,            /**< Podmíněný příkaz "if"      */
    AST_STATEMENT_WHILE         = 5,            /**< Cyklus "while"             */
    AST_STATEMENT_RETURN        = 6,            /**< Příkaz "return"            */
} AST_StatementType;

/**
 * @brief Typy binárních operátorů v abstraktním sytaktickém stromu (AST).
 *
 * @details Reprezentuje typy binárních operátorů používaných ve výrazech AST.
 *          Hodnota z tohoto výčtu je přiřazována uzlu typu @c AST_BinOpNode
 *          a určuje jaký operátor se vyskytuje mezi jeho levým a pravým synem.
 */
typedef enum AST_BinOpType {
    AST_OP_NOT_DEFINED          = 0,            /**< Typ operátoru nebyl definován  */
    AST_OP_EQUAL                = 1,            /**< Operátor rovnosti "=="         */
    AST_OP_NOT_EQUAL            = 2,            /**< Operátor nerovnosti "!="       */
    AST_OP_LESS_THAN            = 3,            /**< Operátor menší než "<"         */
    AST_OP_GREATER_THAN         = 4,            /**< Operátor větší než ">"         */
    AST_OP_LESS_EQUAL           = 5,            /**< Operátor menší nebo rovno "<=" */
    AST_OP_GREATER_EQUAL        = 6,            /**< Operátor větší nebo rovno ">=" */
    AST_OP_ADD                  = 7,            /**< Operátor součtu "+"            */
    AST_OP_SUBTRACT             = 8,            /**< Operátor rozdílu "-"           */
    AST_OP_MULTIPLY             = 9,            /**< Operátor součinu "*"           */
    AST_OP_DIVIDE               = 10,           /**< Operátor podílu "/"            */
    AST_OP_ASSIGNMENT           = 11,           /**< Operátor přiřazení "="         */
} AST_BinOpType;

/**
 * @brief Typy literálů v abstraktním sytaktickém stromu (AST).
 *
 * @details Reprezentuje základní typy literálů jazyka IFJ24, které budou
 *          reprezentovány uzlem @c AST_VarNode v AST.
 */
typedef enum AST_LiteralType {
    AST_LITERAL_NOT_DEFINED     = 0,            /**< Typ literálu nebyl definován */
    AST_LITERAL_INT             = 1,            /**< Literál typu i32  */
    AST_LITERAL_FLOAT           = 2,            /**< Literál typu f64  */
    AST_LITERAL_STRING          = 3,            /**< Literál typu []u8 */
    AST_LITERAL_NULL            = 4,            /**< Literál typu NULL */
    AST_LITERAL_BOOL            = 5,            /**< Literál typu boolean (interně využíváno při sémantické analýze) */
} AST_LiteralType;

/**
 * @brief Datové typy v abstraktním sytaktickém stromu (AST).
 *
 * @details Reprezentuje datové typy používané v jazyce IFJ24. Datový typ
 *          je přiřazován uzlu @c AST_ArgOrParamNode, pokud se jedná o jeho
 *          variantu obsahující parametr definice funkce. Dále je využíván
 *          u uzlu @c AST_FunDefNode, kde reprezentuje návratový datový typ
 *          funkce.
 *
 * @warning Typ @c void je povolen pouze jako pseudo návratový typ funkcí.
 * @warning Typ @c bool je povolen pouze pro výsledek relační binární operace.
 */
typedef enum AST_DataType {
    AST_DATA_TYPE_NOT_DEFINED   = 0,            /**< Datový typ není uveden    */
    AST_DATA_TYPE_INT           = 1,            /**< Datový typ i32            */
    AST_DATA_TYPE_INT_OR_NULL   = 2,            /**< Datový typ i32 nebo NULL  */
    AST_DATA_TYPE_FLOAT         = 3,            /**< Datový typ f64            */
    AST_DATA_TYPE_FLOAT_OR_NULL = 4,            /**< Datový typ f64 nebo NULL  */
    AST_DATA_TYPE_STRING        = 5,            /**< Datový typ []u8           */
    AST_DATA_TYPE_STRING_OR_NULL= 6,            /**< Datový typ []u8 nebo NULL */
    AST_DATA_TYPE_VOID          = 7,            /**< Datový typ void (pouze jako návratový typ funkce)    */
    AST_DATA_TYPE_BOOL          = 8,            /**< Datový typ boolean (využíván při sémantické analýze) */
} AST_DataType;

/**
 * @brief Typy výrazů v abstraktním sytaktickém stromu (AST).
 *
 * @details Reprezentuje různé typy výrazů, které mohou být použity v AST.
 *          Na základě této hodnoty lze rozhodnout jakým způsobem dereferencovat
 *          ukazatel typu @c void u uzlů @c AST_ExprNode.
 */
typedef enum AST_ExprType {
    AST_EXPR_NOT_DEFINED        = 0,            /**< Typ výrazu nebyl definován      */
    AST_EXPR_LITERAL            = 1,            /**< Literál (i32, f64, []u8, null)  */
    AST_EXPR_VARIABLE           = 2,            /**< Proměnná (identifikátor)        */
    AST_EXPR_FUN_CALL           = 3,            /**< Volání funkce                   */
    AST_EXPR_BINARY_OP          = 4,            /**< Binární operace                 */
} AST_ExprType;


/*******************************************************************************
 *                                                                             *
 *                   STRUKTURY PRO JEDNOTLIVÉ TYPY AST UZLŮ                    *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Kořenový uzel pro program.
 *
 * @details Tento uzel reprezentuje celý program ve formě abstraktního
 *          syntaktického stromu (AST). Z tohoto uzlu se větví všechny ostatní
 *          části programu. Obsahuje seznam funkcí a cestu k importovanému
 *          modulu.
 */
typedef struct AST_ProgramNode {
    enum   AST_NodeType   type;                 /**< Typ uzlu: `AST_PROGRAM_NODE`                     */
    struct AST_VarNode    *importedFile;        /**< Prolog: proměnná s cestou k importovanému modulu */
    struct AST_FunDefNode *functionList;        /**< Seznam definic funkcí                            */
} AST_ProgramNode;

/**
 * @brief Uzlová struktura pro definici funkce (resp. seznam definic funkcí).
 *
 * @details Tento uzel představuje definici funkce, včetně jejího názvu,
 *          parametrů, návratového typu a těla. Funkce může mít libovolný
 *          počet parametrů a tělo funkce může být prázdné (NULL).
 */
typedef struct AST_FunDefNode {
    enum   AST_NodeType       type;             /**< Typ uzlu: `AST_FUN_DEF_NODE`            */
           DString            *identifier;      /**< Identifikátor funkce                    */
    struct AST_ArgOrParamNode *parameters;      /**< Seznam parametrů funkce (může být NULL) */
    enum   AST_DataType       returnType;       /**< Návratový typ funkce                    */
    struct AST_StatementNode  *body;            /**< Tělo funkce (může být NULL)             */
    struct AST_FunDefNode     *next;            /**< Ukazatel na další funkci v seznamu      */
} AST_FunDefNode;

/**
 * @brief Uzlová struktura pro argument/parametr funkce (resp. seznam argumentů/parametrů).
 *
 * @details Tento uzel reprezentuje jeden parametr při definici funkce nebo
 *          argument při volání funkce. Uzel je součástí lineární seznamu
 *          parametrů či argumentů. Každý uzel obsahuje datový typ a výraz
 *          reprezentující hodnotu argumentu nebo parametru. Datový typ je
 *          uveden pouze u parametrů při definici funkce, zatímco u argumentů
 *          uváděn není.
 */
typedef struct AST_ArgOrParamNode {
    enum   AST_NodeType        type;            /**< Typ uzlu: `AST_ARG_NODE`                      */
    enum   AST_DataType        dataType;        /**< Datový typ parametru funkce                   */
    struct AST_ExprNode        *expression;     /**< Výraz reprezentující argument/parametr funkce */
    struct AST_ArgOrParamNode  *next;           /**< Ukazatel na další argument/parametr funkce    */
} AST_ArgOrParamNode;

/**
 * @brief Uzlová struktura pro příkaz (resp. seznam příkazů).
 *
 * @details Tento uzel reprezentuje libovolný příkaz programu, jako je přiřazení,
 *          volání funkce, podmínka nebo cyklus. Obsahuje také ukazatel na
 *          další příkaz v sekvenci příkazů. Typ příkazu určuje, jaká data jsou
 *          uložena ve členu `statement`.
 *
 * @note Povolené typy pro `void *statement`:
 *       - @c AST_ExprNode* (pro přiřazení, volání funkce, návratový výraz)
 *       - @c AST_FunCallNode* (pro volání funkce)
 *       - @c AST_IfNode* (pro podmíněný příkaz)
 *       - @c AST_WhileNode* (pro cyklus typu `while()`)
 */
typedef struct AST_StatementNode {
    enum   AST_NodeType      type;              /**< Typ uzlu: `AST_STATEMENT_NODE`              */
           size_t            frameID;           /**< ID rámce, ve kterém se příkaz nachází       */
    enum   AST_StatementType statementType;     /**< Typ příkazu                                 */
           void              *statement;        /**< Ukazatel na uzel specifický pro daný příkaz */
    struct AST_StatementNode *next;             /**< Ukazatel na další příkaz v lineární seznamu */
} AST_StatementNode;

/**
 * @brief Uzlová struktura pro volání funkce.
 *
 * @details Tento uzel reprezentuje volání funkce v programu, včetně
 *          identifikátoru funkce a předaných argumentů. Funkce může být
 *          vestavěná (tj.`ifj.*`) nebo uživatelsky definovaná.
 */
typedef struct AST_FunCallNode {
    enum   AST_NodeType        type;            /**< Typ uzlu: `AST_FUN_CALL_NODE`              */
           DString             *identifier;     /**< Identifikátor funkce                       */
           bool                isBuiltIn;       /**< Příznak, zda je funkce vestavěná (`ifj.*`) */
    struct AST_ArgOrParamNode  *arguments;      /**< Seznam uzlů argumentů volané funkce        */
} AST_FunCallNode;

/**
 * @brief Uzlová struktura pro podmíněný příkaz `if`.
 *
 * @details Tento uzel reprezentuje podmíněný příkaz, který obsahuje podmínku,
 *          větev `then` a povinně také  větev `else`. Podmínka je vyhodnocena
 *          jako výraz a na základě její hodnoty se vykoná buď větev `then`,
 *          nebo větev `else`. Pomocí tohoto uzlu lze také reprezentovat
 *          variantu podmíněného příkazu `if` s nullable podmínkou.
 */
typedef struct AST_IfNode {
    enum   AST_NodeType      type;              /**< Typ uzlu: `AST_IF_NODE`                     */
    struct AST_ExprNode      *condition;        /**< Výraz reprezentující podmínku               */
    struct AST_VarNode       *nullCondition;    /**< Identifikátor: Volitelná nullable podmínka  */
    struct AST_StatementNode *thenBranch;       /**< Seznam příkazů větve 'then' (může být NULL) */
    struct AST_StatementNode *elseBranch;       /**< Seznam příkazů větve 'else' (může být NULL) */
} AST_IfNode;

/**
 * @brief Uzlová struktura pro smyčku `while`.
 *
 * @details Tento uzel reprezentuje cyklus s podmínkou na začátku, obsahuje
 *          podmínku a tělo smyčky. Podmínka je vyhodnocena před každou iterací
 *          smyčky a pokud je splněna, vykoná se tělo smyčky. Pomocí tohoto uzlu
 *          lze také reprezentovat variantu cyklu příkazu `while` s nullable
 *          podmínkou.
 */
typedef struct AST_WhileNode {
    enum   AST_NodeType      type;              /**< Typ uzlu: `AST_WHILE_NODE`                 */
    struct AST_ExprNode      *condition;        /**< Výraz reprezentující podmínku              */
    struct AST_VarNode       *nullCondition;    /**< Identifikátor: Volitelná nullable podmínka */
    struct AST_StatementNode *body;             /**< Seznam příkazů těla cyklu                  */
} AST_WhileNode;

/**
 * @brief Uzlová struktura pro výraz.
 *
 * @details Tento uzel reprezentuje obecný výraz v programu, který může být
 *          proměnnou, literálem, voláním funkce nebo binární operací. Typ
 *          výrazu určuje, jaká data jsou uložena v poli `expression`.
 *
 * @note Povolené typy pro `void *expression`:
 *       - @c AST_VarNode* (pro proměnnou nebo literál)
 *       - @c AST_FunCallNode* (pro volání funkce)
 *       - @c AST_BinOpNode* (pro binární operaci)
 */
typedef struct AST_ExprNode {
    enum AST_NodeType type;                     /**< Typ uzlu: `AST_EXPR_NODE`         */
    enum AST_ExprType exprType;                 /**< Specifický typ výrazu             */
         void         *expression;              /**< Ukazatel na konkrétní uzel výrazu */
} AST_ExprNode;

/**
 * @brief Uzlová struktura pro binární operaci.
 *
 * @details Tento uzel reprezentuje binární operaci s levým a pravým operandem.
 *          Operátorem může být aritmetická, relační nebo logická operace.
 *          Levý a pravý operand jsou výrazy.
 */
typedef struct AST_BinOpNode {
    enum   AST_NodeType  type;                  /**< Typ uzlu: `AST_BINARY_OP_NODE` */
    enum   AST_BinOpType op;                    /**< Operátor (+, -, *, /, atd.)    */
    struct AST_ExprNode  *left;                 /**< Levý operand (výraz)           */
    struct AST_ExprNode  *right;                /**< Pravý operand (výraz)          */
} AST_BinOpNode;

/**
 * @brief Uzlová struktura pro proměnnou nebo literál.
 *
 * @details Tento uzel reprezentuje buď proměnnou s identifikátorem, nebo
 *          literál s hodnotou. Umožňuje uchovávat informace o typu a hodnotě
 *          proměnné či literálu. Pokud je typem literálu `NULL`, hodnota bude
 *          `NULL`. Tento univerzálně uzel umožňuje jednoduché předvyhodnocení
 *          některých výrazů během sémantické analýzy.
 *
 * @note Povolené typy pro `void *value`:
 *       - Celé číslo: @c int*
 *       - Desetinné číslo: @c double*
 *       - Řetězec: @c DString*
 *       - Null: @c NULL
 */
typedef struct AST_VarNode {
    enum AST_NodeType    type;                  /**< Typ uzlu: `AST_VAR_NODE`                  */
         DString         *identifier;           /**< Identifikátor proměnné (pro literál NULL) */
         size_t          frameID;               /**< ID rámce, kde je proměnná deklarována     */
    enum AST_LiteralType literalType;           /**< Typ hodnoty                               */
         void            *value;                /**< Ukazatel na hodnotu literálu              */
} AST_VarNode;


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 *
 * @details Tato proměnná obsahuje kořenový uzel abstraktního syntaktického
 *          stromu (AST), který je vytvářen během syntaktické analýzy. AST je
 *          používán pro reprezentaci struktury programu a je generován během
 *          syntaktické analýzy vstupního programu. Dále je využíván k sémantické
 *          analýze a generování vnitřního kódu.
 *
 * @note Ukazatel na kořen abstraktního syntaktického stromu (AST) bude před
 *       svou skutečnou inicializací (alokací paměti) inicializován na @c NULL.
 */
extern AST_ProgramNode *ASTroot;

#endif // AST_NODES_H_

/*** Konec souboru ast_nodes.h ***/
