/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.h                                       *
 * Autor:            Krejčí David   <xkrejcd00>                                *
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
 * @file semantic_analyser.h
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Hlavičkový soubor pro sémantický analyzátor.
 * @details Obsahuje definice funkcí pro sémantickou analýzu
 */

#ifndef SEMANTIC_ANALYSER_H_
/** @cond  */
#define SEMANTIC_ANALYSER_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "ast_nodes.h"
#include "ast_interface.h"
#include "frame_stack.h"
#include "symtable.h"
#include "error.h"


typedef enum {
    SEM_DATA_UNKNOWN = 0,
    SEM_DATA_INT = 1,
    SEM_DATA_INT_OR_NULL = 2,
    SEM_DATA_FLOAT = 3,
    SEM_DATA_FLOAT_OR_NULL = 4,
    SEM_DATA_RAW_STRING = 5,
    SEM_DATA_STRING = 6,
    SEM_DATA_STRING_OR_NULL = 7,
    SEM_DATA_VOID = 8,
    SEM_DATA_BOOL = 9,
    SEM_DATA_NULL = 10
} Semantic_Data;


/**
 * @brief Provede sémantickou analýzu celého programu.
 *
 * @details Volá funkce pro dílčí sémantické analýzy programu:
 *          - semantic_checkProgramStructure()
 *          - semantic_checkFunctionDefinitions()
 *          - semantic_checkVariables()
 */
void semantic_analyseProgram();


/**
 * @brief Provede sémantickou analýzu základní struktury programu.
 *
 * @details Ověří, že existuje funkce main s návratovým typem void a bez parametrů
 *          Dále ověří sprvánost literálů v prologu programu
 *
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseProgramStructure();


/**
 * @brief Provede sémantickou analýzu definic funkcí
 *
 * @details Projde strom a pro každou definici funkce zavolá sémantickou sondu
 *          ()
 *
 * @return '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseFunctionDefinitions();


/**
 * @brief Provede sémantickou analýzu všech proměnných v programu
 *
 * @details Projde všechny rámce v globálním poli a zkontroluje,
 *          že každá proměnná je využita,
 *          že každá nekonstantní proměnná je změněna,
 *          že žádná konstantní proměnná není změněna
 *
 * @return '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseVariables();


/**
 * @brief Provede sémantickou analýzu bloku funkce
 *
 * @details Prochází blok příkaz po příkazu a podle typu volá další
 *          pomocné funkce podle typu příkazu.
 *
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 *
 */
ErrorType semantic_probeFunction(AST_FunDefNode *node);

/**
 * @brief
 */
ErrorType semantic_probeBlock(Semantic_Data fun_return,
                              AST_StatementNode *statement, bool* returned);

ErrorType semantic_analyseVarDef(AST_StatementNode *statement);

ErrorType semantic_analyseExpr(AST_ExprNode *expr_node, Semantic_Data *type, void **value);

ErrorType semantic_analyseFunCall(AST_FunCallNode *fun_node, Semantic_Data *return_type);

ErrorType semantic_analyseIf(Semantic_Data fun_return,       \
                             AST_IfNode *if_node, bool *returned);

ErrorType semantic_analyseWhile(Semantic_Data fun_return,       \
                                AST_WhileNode *while_node, bool *returned);

ErrorType semantic_analyseReturn(Semantic_Data fun_return, AST_ExprNode *node);

ErrorType semantic_compatibleAssign(Semantic_Data defined, Semantic_Data actual);

ErrorType semantic_compatibleRelation(Semantic_Data type_1, Semantic_Data type_2);

ErrorType semantic_compatibleEqual(Semantic_Data type_1, Semantic_Data type_2);

ErrorType semantic_compatibleArithmetic(Semantic_Data type_1, Semantic_Data type_2);

ErrorType semantic_toInt(AST_ExprNode *node);

ErrorType semantic_toFloat(AST_ExprNode *node);



Semantic_Data semantic_ASTToSemType(AST_DataType type);

Semantic_Data semantic_literalToSemType(AST_LiteralType type);

Semantic_Data semantic_stateToSemType(symtable_symbolState state);

Semantic_Data semantic_returnToSemType(symtable_functionReturnType type);

symtable_symbolState semantic_semTypeToState(Semantic_Data type);

AST_LiteralType semantic_semToLiteral(Semantic_Data type);

ErrorType semantic_checkIFJWrite(AST_FunCallNode *fun_node);

ErrorType semantic_checkIFJString(AST_FunCallNode *fun_node);

#endif // SEMANTIC_ANALYSER_H_

/*** Konec souboru semantic_analyser.h ***/
