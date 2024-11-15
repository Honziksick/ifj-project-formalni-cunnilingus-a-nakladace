/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_nodes.h                                               *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   01.11.2024                                                *
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
 * @details Tento soubor obsahuje deklarace datových struktur a funkcí pro práci
 *          s uzly v abstraktním syntaktickém stromu (AST), který je základem
 *          pro sémantickou analýzu a generování kódu v překladači IFJ24.
 *          Definuje typy uzlů, příkazů, výrazů, literálů a datových typů
 *          používaných v AST. Dále zahrnuje základní struktury jako jsou uzly
 *          pro program, funkce, parametry, příkazy a výrazy.
 */

#ifndef AST_NODES_H_
/** @cond  */
#define AST_NODES_H_
/** @endcond  */

#include <stdbool.h>
#include "dynamic_string.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define AST_FRAME_ID_NOT_ASSIGNED 0     /**< Uzlu AST_StatementNode nebo AST_VarNode ještě nebylo přiřazeno frameID. */
#define AST_VAL_UNDEFINED NULL          /**< Uzel `AST_VarNode` pro proměnnou neobsahuje literál */
#define AST_ID_UNDEFINED NULL           /**< Uzel `AST_VarNode` pro literál neobsahuje identifikátor */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Typy uzlů v AST.
 *
 * @details Reprezentuje různé typy uzlů v abstraktním syntaktickém stromu (AST).
 */
typedef enum AST_NodeType {
    // Nedefinovaný uzel
    AST_NODE_UNDEFINED          = -1,           /**< Typ uzlu zatím není definován */

    // Struktury programu
    AST_PROGRAM_NODE            = 1,            /**< Uzel pro celý program */
    AST_FUN_DEF_NODE            = 2,            /**< Uzel pro definici funkce */
    AST_ARG_OR_PARAM_NODE       = 3,            /**< Uzel pro argument/parametr funkce */
    AST_STATEMENT_NODE          = 4,            /**< Uzel pro příkaz */

    // Příkazy
    AST_FUN_CALL_NODE           = 5,            /**< Uzel pro volání funkce */
    AST_IF_NODE                 = 6,            /**< Uzel pro podmíněný příkaz "if" */
    AST_WHILE_NODE              = 7,            /**< Uzel pro cyklus "while" */

    // Výrazy
    AST_EXPR_NODE               = 8,            /**< Uzel pro výraz */
    AST_BIN_OP_NODE             = 9,            /**< Uzel pro binární operaci */
    AST_LITERAL_NODE            = 10,           /**< Uzel pro literál */
    AST_VAR_NODE                = 11,           /**< Uzel pro proměnnou */
} AST_NodeType;

/**
 * @brief Typy příkazů v AST.
 *
 * @details Reprezentuje různé typy příkazů, které mohou být použity v
 *          abstraktním syntaktickém stromu (AST).
 */
typedef enum AST_StatementType {
    AST_STATEMENT_NOT_DEFINED   = 0,            /**< Typ příkazu není definován */
    AST_STATEMENT_VAR_DEF       = 1,            /**< Definice proměnné */
    AST_STATEMENT_EXPR          = 2,            /**< Výraz */
    AST_STATEMENT_FUN_CALL      = 3,            /**< Volání funkce */
    AST_STATEMENT_IF            = 4,            /**< Podmíněný příkaz "if" */
    AST_STATEMENT_WHILE         = 5,            /**< Cyklus "while" */
    AST_STATEMENT_RETURN        = 6,            /**< Příkaz "return" */
} AST_StatementType;

/**
 * @brief Typy binárních operátorů v AST.
 *
 * @details Reprezentuje typy binárních operátorů používaných ve výrazech AST.
 */
typedef enum AST_BinOpType {
    AST_OP_NOT_DEFINED          = 0,            /**< Typ operátoru nebyl definován */
    AST_OP_EQUAL                = 1,            /**< Operátor rovnosti "==" */
    AST_OP_NOT_EQUAL            = 2,            /**< Operátor nerovnosti "!=" */
    AST_OP_LESS_THAN            = 3,            /**< Operátor menší než "<" */
    AST_OP_GREATER_THAN         = 4,            /**< Operátor větší než ">" */
    AST_OP_LESS_EQUAL           = 5,            /**< Operátor menší nebo rovno "<=" */
    AST_OP_GREATER_EQUAL        = 6,            /**< Operátor větší nebo rovno ">=" */
    AST_OP_ADD                  = 7,            /**< Operátor součtu "+" */
    AST_OP_SUBTRACT             = 8,            /**< Operátor rozdílu "-" */
    AST_OP_MULTIPLY             = 9,            /**< Operátor součinu "*" */
    AST_OP_DIVIDE               = 10,           /**< Operátor podílu "/" */
    AST_OP_ASSIGNEMENT          = 11,           /**< Operátor přiřazení "=" */
} AST_BinOpType;

/**
 * @brief Typy literálů v AST.
 *
 * @details Reprezentuje základní datové typy literálů v AST.
 */
typedef enum AST_LiteralType {
    AST_LITERAL_NOT_DEFINED     = 0,            /**< Typ literálu nebyl definován */
    AST_LITERAL_INT             = 1,            /**< Literál typu i32 */
    AST_LITERAL_FLOAT           = 2,            /**< Literál typu f64 */
    AST_LITERAL_STRING          = 3,            /**< Literál typu []u8 */
    AST_LITERAL_NULL            = 4,            /**< Literál typu NULL */
} AST_LiteralType;

/**
 * @brief Datové typy v AST.
 *
 * @details Reprezentuje datové typy používané v AST.
 *
 * @warning Typ @c void je povolen pouze jako pseudo návratový typ funkcí.
 * @warning Typ @c bool je povolen pouze pro výsledek relační binární operace.
 */
typedef enum AST_DataType {
    AST_DATA_TYPE_NOT_DEFINED   = 0,            /**< Datový typ není uveden */
    AST_DATA_TYPE_INT           = 1,            /**< Datový typ i32 */
    AST_DATA_TYPE_INT_OR_NULL   = 2,            /**< Datový typ i32 nebo NULL */
    AST_DATA_TYPE_FLOAT         = 3,            /**< Datový typ f64 */
    AST_DATA_TYPE_FLOAT_OR_NULL = 4,            /**< Datový typ f64 nebo NULL */
    AST_DATA_TYPE_STRING        = 5,            /**< Datový typ []u8 */
    AST_DATA_TYPE_STRING_OR_NULL= 6,            /**< Datový typ []u8 nebo NULL */
    AST_DATA_TYPE_VOID          = 7,            /**< Datový typ void */
    AST_DATA_TYPE_BOOL          = 8,            /**< Datový typ boolean */
} AST_DataType;

/**
 * @brief Typy výrazů v AST.
 *
 * @details Reprezentuje různé typy výrazů, které mohou být použity v AST.
 */
typedef enum AST_ExprType {
    AST_EXPR_NOT_DEFINED        = 0,            /**< Typ výrazu nebyl definován */
    AST_EXPR_LITERAL            = 1,            /**< Literál */
    AST_EXPR_VARIABLE           = 2,            /**< Proměnná (identifikátor) */
    AST_EXPR_FUN_CALL           = 3,            /**< Volání funkce */
    AST_EXPR_BINARY_OP          = 4,            /**< Binární operace */
} AST_ExprType;


/*******************************************************************************
 *                                                                             *
 *                   STRUKTURY PRO JEDNOTLIVÉ TYPY AST UZLŮ                    *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Kořenový uzel programu.
 *
 * @details Tento uzel reprezentuje celý program v AST.
 */
typedef struct AST_ProgramNode {
    enum   AST_NodeType   type;                 /**< Typ uzlu (AST_PROGRAM_NODE) */
           DString        *importedFile;        /**< Prolog: cesta k importovanému modulu */
    struct AST_FunDefNode *functionList;        /**< Seznam definic funkcí */
} AST_ProgramNode;

/**
 * @brief Struktura pro definici funkce.
 */
typedef struct AST_FunDefNode {
    enum   AST_NodeType       type;             /**< Typ uzlu (AST_FUN_DEF_NODE) */
           DString            *identifier;      /**< Identifikátor funkce */
    struct AST_ArgOrParamNode *parameters;      /**< Parametry funkce (může být NULL) */
    enum   AST_DataType       returnType;       /**< Návratový typ funkce */
    struct AST_StatementNode  *body;            /**< Tělo funkce */
    struct AST_FunDefNode     *next;            /**< Ukazatel na další funkci v seznamu */
} AST_FunDefNode;

/**
 * @brief Struktura pro seznam argumentů.
 *
 * @details Pokud se jedná o parametr funkce či argument volání funkce,
 *
 */
typedef struct AST_ArgOrParamNode {
    enum   AST_NodeType        type;            /**< Typ uzlu (AST_ARG_NODE) */
    struct AST_VarNode         *variable;       /**< Argument/Parametr funkce */
    struct AST_ArgOrParamNode  *next;           /**< Ukazatel na další argument/parametr */
} AST_ArgOrParamNode;

/**
 * @brief Struktura pro obecný příkaz.
 *
 * @details Tento uzel reprezentuje obecný příkaz v AST. Obsahuje typ uzlu,
 *          ukazatel na další příkaz a ukazatel na data specifická pro daný příkaz.
 *
 * @note Povolené typy pro `void *statementData`:
 *       - @c definice @c funkce (tj. ve skutečnosti AST_ExprNode*)
 *       - @c AST_ExprNode*
 *       - @c AST_FunCallNode*
 *       - @c AST_IfNode*
 *       - @c AST_WhileNode*
 *       - @c návratový @c výraz (tj. ve skutečnosti AST_ExprNode*)
 */
typedef struct AST_StatementNode {
    enum   AST_NodeType      type;              /**< Typ uzlu (AST_STATEMENT_NODE) */
           size_t            frameID;           /**< ID příslušného rámce */
    enum   AST_StatementType statementType;     /**< Typ příkazu */
           void              *statement;        /**< Ukazatel na uzel specifický pro daný příkaz */
    struct AST_StatementNode *next;             /**< Ukazatel na další příkaz */
} AST_StatementNode;

/**
 * @brief Struktura pro volání funkce.
 */
typedef struct AST_FunCallNode {
    enum   AST_NodeType        type;            /**< Typ uzlu (AST_FUN_CALL_NODE) */
           DString             *identifier;     /**< Název funkce */
           bool                isBuiltIn;       /**< Příznak, zda je funkce vestavěná (ifj.*) */
    struct AST_ArgOrParamNode  *arguments;      /**< Uzly argumentů */
} AST_FunCallNode;

/**
 * @brief Struktura pro podmíněný příkaz if.
 */
typedef struct AST_IfNode {
    enum   AST_NodeType      type;              /**< Typ uzlu (AST_IF_NODE) */
    struct AST_ExprNode      *condition;        /**< Podmínka */
    struct AST_VarNode       *nullCondition;    /**< Identifikátor: Volitelná podmínka NULL */
    struct AST_StatementNode *thenBranch;       /**< Větev 'then' */
    struct AST_StatementNode *elseBranch;       /**< Větev 'else' */
} AST_IfNode;

/**
 * @brief Struktura pro smyčku while.
 */
typedef struct AST_WhileNode {
    enum   AST_NodeType      type;              /**< Typ uzlu (AST_WHILE_NODE) */
    struct AST_ExprNode      *condition;        /**< Podmínka */
    struct AST_VarNode       *nullCondition;    /**< Identifikátor: Volitelná podmínka NULL */
    struct AST_StatementNode *body;             /**< Tělo cyklu */
} AST_WhileNode;

/**
 * @brief Struktura pro výraz.
 *
 * @note Povolené typy pro `void *expression`:
 *       - @c AST_VarNode*
 *       - @c AST_FunCallNode*
 *       - @c AST_BinaryOpNode*
 */
typedef struct AST_ExprNode {
    enum AST_NodeType type;                     /**< Typ uzlu (AST_EXPR_NODE) */
    enum AST_ExprType exprType;                 /**< Typ výrazu */
         void         *expression;
} AST_ExprNode;

/**
 * @brief Struktura pro binární operátor.
 */
typedef struct AST_BinOpNode {
    enum   AST_NodeType  type;                  /**< Typ uzlu (AST_BINARY_OP_NODE) */
    enum   AST_BinOpType op;                    /**< Operátor (+, -, *, /, atd.) */
    struct AST_ExprNode  *left;                 /**< Levý operand */
    struct AST_ExprNode  *right;                /**< Pravý operand */
} AST_BinOpNode;

/**
 * @brief Struktura pro proměnnou nebo literál.
 *
 * @details Pokud je typem literálu NULL literál, tak bude hodnota "*value" NULL.
 *
 * @note Povolené typy pro `void *value`:
 *       - Celé číslo: @c int*
 *       - Desetinné číslo: @c double*
 *       - Řetězec: @c DString*
 *       - Null: @c NULL
 */
typedef struct AST_VarNode {
    enum AST_NodeType    type;                  /**< Typ uzlu (AST_VAR_NODE) */
         DString         *identifier;           /**< Identifikátor proměnné (pro literál NULL)*/
         size_t          frameID;               /**< ID rámce, kde je proměnná definovaná */
    enum AST_LiteralType literalType;           /**< Typ hodnoty */
         void            *value;                /**< Ukazatel na hodnotu literálu */
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
 *          používán pro reprezentaci struktury programu a je generován parserem.
 *          Dále je využíván k sémantické analýze a generování 3AK.
 *
 * @note Ukazatel na kořen abstraktního syntaktického stromu bude před svou
 *       skutečnou inicializací inicializován na @c NULL.
 */
extern AST_ProgramNode *ASTroot;

#endif // AST_NODES_H_

/*** Konec souboru ast_nodes.h ***/
