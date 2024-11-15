/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.h                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   15.11.2024                                                *
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
 * @author Lukáš Farkašovský \<xfarkal00>
 *
 * @brief Hlavičkový soubor pro generátor vnitřního kódu (3AK).
 * @details Knihovna pro generování cílového kódu z AST.
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
typedef enum TAC_Operation {\
    TAC_OP_NONE,           /**< Žádná operace (pro základní hodnoty) */
    TAC_OP_DEFVAR,         /**< Definice proměnné */
    TAC_OP_JUMP,           /**< Nepodmíněný skok */
    TAC_OP_JUMPIFEQ,       /**< Podmíněný skok: rovnost */
    TAC_OP_JUMPIFNEQ,      /**< Podmíněný skok: nerovnost */
    TAC_OP_TYPE,           /**< Typ */
    TAC_OP_EXIT,           /**< Ukončení programu */
    TAC_OP_CREATEFRAME,    /**< Vytvoření nového rámce */
    TAC_OP_PUSHFRAME,      /**< Přidání prázdného rámce na zásobník rámců */
    TAC_OP_POPFRAME,       /**< Pop prvek zásobníku rámců */
    TAC_OP_ADD,            /**< Sčítání */
    TAC_OP_SUB,            /**< Odčítání */
    TAC_OP_MUL,            /**< Násobení */
    TAC_OP_DIV,            /**< Dělení */
    TAC_OP_ASSIGN,         /**< Přiřazení */
    TAC_OP_IF_EQ,          /**< Porovnání: rovnost */
    TAC_OP_IF_NEQ,         /**< Porovnání: nerovnost */
    TAC_OP_IF_LT,          /**< Porovnání: menší než */
    TAC_OP_IF_GT,          /**< Porovnání: větší než */
    TAC_OP_IF_LE,          /**< Porovnání: menší nebo rovno */
    TAC_OP_IF_GE,          /**< Porovnání: větší nebo rovno */
    TAC_OP_NOT,            /**< Negace */
    TAC_OP_WRITE,          /**< Výpis */
    TAC_OP_READ,           /**< Načtení */
    TAC_OP_GOTO,           /**< Nepodmíněný skok */
    TAC_OP_RETURN,         /**< Návratová hodnota */
    TAC_OP_CALL,           /**< Volání funkce */
    TAC_OP_PARAM,          /**< Parametr pro volání funkce */
    TAC_OP_CONCAT,         /**< Konkatenace */
    TAC_OP_LABEL           /**< Označení místa (label) ve kódu */
} TAC_Operation;

/**
 * @brief Typy operandů v tříadresném kódu.
 *
 * @details Reprezentuje možné typy operandů pro každou instrukci tříadresného kódu.
 */
typedef enum TAC_OperandType {
    TAC_OPERAND_INT,       /**< Celé číslo */
    TAC_OPERAND_FLOAT,     /**< Desetinné číslo */
    TAC_OPERAND_STRING,    /**< Řetězec */
    TAC_OPERAND_BOOL,      /**< Logická hodnota */
    TAC_OPERAND_NIL,       /**< Hodnota NULL */
    TAC_OPERAND_VAR,       /**< Proměnná */
    TAC_OPERAND_TEMP,      /**< Dočasná proměnná */
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
 * @details Alokuje paměť pro nový seznam instrukcí a inicializuje ho.
 *
 * @return @c list Ukazatel na prázdný seznam instrukcí, jinak NULL při chybě alokace.
 */
TAC_InstructionList *TAC_createInstructionList();

/**
 * @brief Vytvoří novou tříadresnou instrukci.
 *
 * @details Vytvoří novou instrukci s danou operací a s danými operandy.
 *
 * @param op Typ operace instrukce.
 * @param dest Cílový operand.
 * @param src1 První zdrojový operand.
 * @param src2 Druhý zdrojový operand.
 *
 * @return Ukazatel na nově vytvořenou instrukci.
 *         NULL, pokud selže alokace paměti.
 */
TAC_Instruction *TAC_createInstruction(TAC_Operation op, TAC_Operand dest, TAC_Operand src1, TAC_Operand src2);

/**
 * @brief Přidá instrukci na konec seznamu instrukcí.
 *
 * @details Přidá novou instrukci na konec seznamu instrukcí, jedná se o jednosměrně vázaný seznam,
 *          s ukazatelem na poslední prvek.
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
bool TAC_generateProgramCode(AST_ProgramNode *programNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro definici funkce.
 *
 * @param funDefNode Ukazatel na uzel definice funkce.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateFunctionDefinition(AST_FunDefNode *funDefNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro argument nebo parametr.
 *
 * @param argOrParamNode Ukazatel na uzel argumentu nebo parametru.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateArgOrParam(AST_ArgOrParamNode *argOrParamNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro příkaz.
 *
 * @param statementNode Ukazatel na uzel příkazu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateStatement(AST_StatementNode *statementNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro volání funkce.
 *
 * @param funCallNode Ukazatel na uzel volání funkce.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return Výstupní operand reprezentující výsledek volání funkce.
 */
bool TAC_generateFunctionCall(AST_FunCallNode *funCallNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro podmíněný příkaz "if".
 *
 * @param ifNode Ukazatel na uzel podmíněného příkazu "if".
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateIfStatement(AST_IfNode *ifNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro smyčku "while".
 *
 * @param whileNode Ukazatel na uzel smyčky "while".
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateWhileLoop(AST_WhileNode *whileNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro výraz.
 *
 * @param expressionNode Ukazatel na uzel výrazu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return TAC_Operand Výstupní operand reprezentující výsledek výrazu.
 */
TAC_Operand TAC_generateExpression(AST_ExprNode *expressionNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro binární operaci.
 *
 * @param binOpNode Ukazatel na uzel binární operace.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.
 */
bool TAC_generateBinOp(AST_BinOpNode *binOpNode, TAC_InstructionList *tacList);

/**
 * @brief Generuje tříadresný kód pro návratový příkaz.
 *
 * @param returnNode Ukazatel na uzel návratového příkazu.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return `true`, pokud generování proběhlo úspěšně, jinak `false`.

bool TAC_generateReturnStatement(AST_DataReturn *returnNode, TAC_InstructionList *tacList);*/

/**
 * @brief Generuje tříadresný kód pro proměnnou.
 *
 * @param varNode Ukazatel na uzel proměnné.
 * @param tacList Ukazatel na seznam instrukcí, do kterého bude kód přidán.
 *
 * @return Výstupní operand reprezentující hodnotu proměnné.
 */
bool TAC_generateVariable(AST_VarNode *varNode, TAC_InstructionList *tacList);

/**
 * @brief Uvolní všechny instrukce v seznamu tříadresného kódu.
 *
 * @details Uvolní všechny instrukce v seznamu tříadresných instrukcí.
 *          Pokud je seznam prázdný, nic se nestane.
 *
 * @param list Ukazatel na seznam tříadresných instrukcí.
 */
void TAC_freeInstructionList(TAC_InstructionList *list);

/**
 * @brief Vytiskne všechny instrukce v seznamu tříadresného kódu.
 *
 * @details Tiskne všechny instrukce v seznamu tříadresných instrukcí, dokud nenarazí na konec.
 *          Pokud je seznam prázdný, nic se nestane.
 *
 * @param list Ukazatel na seznam tříadresných instrukcí.
 */
void TAC_printInstructionList(TAC_InstructionList *list);

#endif // TAC_H_

/*** Konec souboru tac_generator.h ***/
