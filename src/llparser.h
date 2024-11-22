/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           llparser.h                                                *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   16.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file llparser.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro LL syntaktický analyzátor.
 * @details Tento soubor obsahuje deklarace funkcí a datových struktur pro
 *          implementaci LL syntaktické analýzy. Funkce zde uvedené umožňují
 *          parsování jednotlivých neterminálů podle LL pravidel a podporují
 *          tvorbu abstraktního syntaktického stromu (AST) pro jazyk IFJ24.
 */

#ifndef LLPARSER_H_
/** @cond  */
#define LLPARSER_H_
/** @endcond  */

#include <stdbool.h>
#include "lltable.h"
#include "parser.h"
#include "symtable.h"
#include "frame_stack.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "precedence_parser.h"
#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define PARSING_SYNTAX_ERROR NULL    /**< Slouží k propagaci chyby syntaxe návratovými hodnotami při rekurzivním sestupu.  */


/*******************************************************************************
 *                                                                             *
 *           FUNKCE LL-PARSERU URČENÉ K PARSOVÁNÍ POMOCÍ LL-GRAMATKY           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Parsuje neterminál `<PROGRAM>`.
 *
 * @return Ukazatel na uzel programu (AST_ProgramNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
void LLparser_parseProgram();

/**
 * @brief Parsuje neterminál `<PROLOG>`.
 *
 * @return V případě úspěchu vrací cestu k importovaném souboru, jinak `NULL`
 */
AST_VarNode *LLparser_parsePrologue();

/**
 * @brief Parsuje neterminál `<FUN_DEF_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu definic funkcí (AST_FunDefNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_FunDefNode *LLparser_parseFunDefList();

/**
 * @brief Parsuje neterminál `<FUN_DEF>`.
 *
 * @return Ukazatel na uzel definice funkce (AST_FunDefNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_FunDefNode *LLparser_parseFunDef();

/**
 * @brief Parsuje neterminál `<PARAMETERS>`.
 *
 * @return Ukazatel na první uzel seznamu parametrů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *LLparser_parseParameters(size_t *paramCount);

/**
 * @brief Parsuje neterminál `<PARAM_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu parametrů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *LLparser_parseParamList(size_t *paramCount);

/**
 * @brief Parsuje neterminál `<PARAM_LIST_REST>`.
 *
 * @return Ukazatel na další uzel v seznamu parametrů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *LLparser_parseParamListRest(size_t *paramCount);

/**
 * @brief Parsuje neterminál `<PARAM>`.
 *
 * @return Ukazatel na uzel parametru (AST_ArgOrParamNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *LLparser_parseParam();

/**
 * @brief Parsuje neterminál `<RETURN_TYPE>`.
 *
 * @return Hodnota datového typu (AST_DataType) při úspěšném parsování,
 *         jinak `AST_DATA_TYPE_NOT_DEFINED`.
 */
AST_DataType LLparser_parseReturnType();

/**
 * @brief Parsuje neterminál `<DATA_TYPE>`.
 *
 * @return Hodnota datového typu (AST_DataType) při úspěšném parsování,
 *         jinak `AST_DATA_TYPE_NOT_DEFINED`.
 */
AST_DataType LLparser_parseDataType();

/**
 * @brief Parsuje neterminál `<STATEMENT_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu příkazů (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *LLparser_parseStatementList();

/**
 * @brief Parsuje neterminál `<STATEMENT>`.
 *
 * @return Ukazatel na uzel příkazu (AST_StatementNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_StatementNode *LLparser_parseStatement();

/**
 * @brief Parsuje neterminál `<VAR_DEF>`.
 *
 * @return Ukazatel na uzel definice proměnné (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *LLparser_parseVarDef();

/**
 * @brief Parsuje neterminál `<MODIFIABLE>`.
 *
 * @return `true` při úspěšném parsování, jinak `false`.
 */
void LLparser_parseModifiable(bool *isModifiable);

/**
 * @brief Parsuje neterminál `<POSSIBLE_TYPE>`.
 *
 * @return Hodnota datového typu (AST_DataType), nebo `AST_DATA_TYPE_NOT_DEFINED`
 *         pokud typ není uveden.
 */
AST_DataType LLparser_parsePossibleType();

/**
 * @brief Parsuje neterminál `<STATEMENT_REST>`.
 *
 * @return Ukazatel na uzel příkazu (AST_StatementNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_StatementNode *LLparser_parseStatementRest(DString **identifier);

/**
 * @brief Parsuje neterminál `<THROW_AWAY>`.
 *
 * @return Ukazatel na uzel příkazu (AST_StatementNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_ExprNode *LLparser_parseThrowAway();

/**
 * @brief Parsuje neterminál `<IF>`.
 *
 * @return Ukazatel na uzel podmíněného příkazu (AST_IfNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_IfNode *LLparser_parseIf();

/**
 * @brief Parsuje neterminál `<NULL_COND>`.
 *
 * @return `true` při úspěšném parsování, jinak `false`.
 */
AST_VarNode *LLparser_parseNullCond();

/**
 * @brief Parsuje neterminál `<SEQUENCE>`.
 *
 * @return Ukazatel na uzel sekvence příkazů (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *LLparser_parseSequence(bool createFrame);

/**
 * @brief Parsuje neterminál `<WHILE>`.
 *
 * @return Ukazatel na uzel smyčky while (AST_WhileNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_WhileNode *LLparser_parseWhile();

/**
 * @brief Parsuje neterminál `<ARGUMENTS>`.
 *
 * @return Ukazatel na první uzel seznamu argumentů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *LLparser_parseArguments();

#endif // LLPARSER_H_

/*** Konec souboru llparser.h ***/
