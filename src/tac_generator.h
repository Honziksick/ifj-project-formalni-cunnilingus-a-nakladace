/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.h                                           *
 * Autor:            XXX XXXXXX   <xplagia00>                                  *
 *                                                                             *
 * Datum:            XX.XX.2024                                                *
 * Poslední změna:   XX.XX.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file tac_generator.h
 * @author XXX XXXXX \<xplagia00>
 *
 * @brief Hlavičkový soubor pro generátor vnitřního kódu (3AK).
 * @details XXX
 */

#ifndef TAC_H_
/** @cond  */
#define TAC_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "ast_nodes.h"
#include "ast_interface.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Typy operací v tříadresném kódu.
 *
 * @details Reprezentuje různé operace, které lze provádět v tříadresných instrukcích.
 */
typedef enum TAC_Operation {
    TAC_OP_ADD,            /**< Sčítání */
    TAC_OP_SUB,            /**< Odčítání */
    TAC_OP_MUL,            /**< Násobení */
    TAC_OP_DIV,            /**< Dělení */
    TAC_OP_ASSIGN,         /**< Přiřazení */
    TAC_OP_IF_EQ,          /**< Podmíněný skok: rovnost */
    TAC_OP_IF_NEQ,         /**< Podmíněný skok: nerovnost */
    TAC_OP_IF_LT,          /**< Podmíněný skok: menší než */
    TAC_OP_IF_GT,          /**< Podmíněný skok: větší než */
    TAC_OP_IF_LE,          /**< Podmíněný skok: menší nebo rovno */
    TAC_OP_IF_GE,          /**< Podmíněný skok: větší nebo rovno */
    TAC_OP_GOTO,           /**< Nepodmíněný skok */
    TAC_OP_RETURN,         /**< Návratová hodnota */
    TAC_OP_CALL,           /**< Volání funkce */
    TAC_OP_PARAM,          /**< Parametr pro volání funkce */
    TAC_OP_LABEL           /**< Označení místa (label) ve kódu */
} TAC_Operation;

/**
 * @brief Typy operandů v tříadresném kódu.
 *
 * @details Reprezentuje možné typy operandů pro každou instrukci tříadresného kódu.
 */
typedef enum TAC_OperandType {
    TAC_OPERAND_VAR,       /**< Proměnná */
    TAC_OPERAND_CONST,     /**< Konstanta */
    TAC_OPERAND_LABEL,     /**< Místo v kódu */
    TAC_OPERAND_NONE       /**< Žádný operand (např. pro instrukce jako GOTO) */
} TAC_OperandType;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura pro operand tříadresného kódu.
 *
 * @details Reprezentuje operand tříadresné instrukce, který může být proměnná, konstanta nebo label.
 */
typedef struct TAC_Operand {
    TAC_OperandType type;     /**< Typ operandu */
    union {
        DString *varName;     /**< Jméno proměnné */
        int intValue;         /**< Celá konstanta */
        double floatValue;    /**< Desetinná konstanta */
        DString *labelName;   /**< Název labelu */
    } value;                  /**< Hodnota operandu */
} TAC_Operand;

/**
 * @brief Struktura pro tříadresnou instrukci.
 *
 * @details Reprezentuje jednu instrukci v tříadresném kódu, včetně operace a operandů.
 */
typedef struct TAC_Instruction {
    TAC_Operation op;             /**< Typ operace */
    TAC_Operand dest;             /**< Cílový operand (např. výsledek operace) */
    TAC_Operand src1;             /**< První zdrojový operand */
    TAC_Operand src2;             /**< Druhý zdrojový operand */
    struct TAC_Instruction *next; /**< Ukazatel na další instrukci */
} TAC_Instruction;

/**
 * @brief Struktura pro seznam tříadresných instrukcí.
 *
 * @details Slouží jako spojový seznam pro ukládání sekvence tříadresných instrukcí.
 */
typedef struct TAC_InstructionList {
    TAC_Instruction *head;        /**< První instrukce v seznamu */
    TAC_Instruction *tail;        /**< Poslední instrukce v seznamu */
} TAC_InstructionList;


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje nový prázdný seznam tříadresných instrukcí.
 *
 * @return Ukazatel na prázdný seznam instrukcí.
 */
TAC_InstructionList *TAC_createInstructionList();

/**
 * @brief Vytvoří novou tříadresnou instrukci.
 *
 * @param op Typ operace instrukce.
 * @param dest Cílový operand.
 * @param src1 První zdrojový operand.
 * @param src2 Druhý zdrojový operand.
 *
 * @return Ukazatel na nově vytvořenou instrukci.
 */
TAC_Instruction *TAC_createInstruction(TAC_Operation op, TAC_Operand dest, TAC_Operand src1, TAC_Operand src2);

/**
 * @brief Přidá instrukci na konec seznamu instrukcí.
 *
 * @param list Ukazatel na seznam tříadresných instrukcí.
 * @param instr Ukazatel na instrukci, která má být přidána.
 */
void TAC_appendInstruction(TAC_InstructionList *list, TAC_Instruction *instr);

/**
 * @brief Generuje tříadresný kód pro daný uzel programu.
 *
 * @param programNode Ukazatel na kořenový uzel programu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování kódu proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateProgramCode(AST_NodeProgram *programNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro definici funkce.
 *
 * @param funDefNode Ukazatel na uzel definice funkce.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateFunctionDefinition(AST_NodeFunDef *funDefNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro příkaz.
 *
 * @param statementNode Ukazatel na uzel příkazu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateStatement(AST_NodeStatement *statementNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro výraz.
 *
 * @param expressionNode Ukazatel na uzel výrazu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return TAC_Operand Výstupní operand reprezentující výsledek výrazu.
 */
TAC_Operand TAC_generateExpression(AST_NodeExpression *expressionNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro přiřazení.
 *
 * @param assignmentNode Ukazatel na uzel přiřazení.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateAssignment(AST_DataAssignment *assignmentNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro podmíněný příkaz "if".
 *
 * @param ifNode Ukazatel na uzel podmíněného příkazu "if".
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateIfStatement(AST_DataIf *ifNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro smyčku "while".
 *
 * @param whileNode Ukazatel na uzel smyčky "while".
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateWhileLoop(AST_DataWhile *whileNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro návratový příkaz.
 *
 * @param returnNode Ukazatel na uzel návratového příkazu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateReturnStatement(AST_DataReturn *returnNode, TAC_InstructionList *tacList);

/**
 * @brief Uvolní všechny instrukce v seznamu tříadresného kódu.
 *
 * @param list Ukazatel na seznam tříadresných instrukcí.
 */
void TAC_freeInstructionList(TAC_InstructionList *list);

#endif // TAC_H_

/*** Konec souboru tac_generator.h ***/
