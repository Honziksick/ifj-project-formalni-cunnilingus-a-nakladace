/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser.h                                                  *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
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
 * @file parser.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubor pro syntaktický analyzátor.
 * @details Tento hlavičkový soubor obsahuje deklarace funkcí a datových struktur
 *          potřebných pro syntaktickou analýzu zdrojového kódu jazyka IFJ24.
 *          Funkce deklarované v tomto souboru implementují parsování jednotlivých
 *          neterminálů gramatiky a podporují tvorbu abstraktního syntaktického
 *          stromu (AST).
 */

#ifndef PARSER_H_
/** @cond  */
#define PARSER_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"
#include "lltable.h"
#include "ast_nodes.h"
#include "ast_interface.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro aktuální token, který je zpracováván.
 *
 * @details Tato proměnná obsahuje aktuální token, který je právě zpracováván
 *          parserem. Token je získáván pomocí funkce `scanner_getNextToken` a
 *          je aktulizován funkcí `parser_getNextToke`.
 */
extern Token currentToken;

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 *
 * @details Tato proměnná obsahuje kořenový uzel abstraktního syntaktického
 *          stromu (AST), který je vytvářen během syntaktické analýzy. AST je
 *          používán pro reprezentaci struktury programu a je generován parserem.
 *          Dále je využíván k sémantické analýze a generování 3AK.
 */
extern AST_ProgramNode *ASTroot;

/*******************************************************************************
 *                                                                             *
 *                       POMOCENÉ VEŘEJNÉ FUNKCE PARSER                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální token.
 *
 * @details Tato funkce volá `scanner_getNextToken`, která vrací další token
 *          ze vstupního zdroje. Globální proměnná `currentToken` je poté
 *          aktualizována na tento nový token.
 */
void parser_getNextToken();

/**
 * @brief Přidá suffix k jménu proměnné ve formátu `$frameID$`.
 *
 * @details Funkce připojí k původnímu jménu proměnné textový suffix `$frameID$`,
 *          kde `frameID` je ID identifákoru příslušnému rámci. Tento formát
 *          suffixu zajišťuje, že jméno proměnné bude unikátní v rámci všech
 *          rámců.
 *
 * @note Funkce využívá `DString_appendChar()` pro přidání suffixu.
 *
 * @param frameID Číselné ID rámce, které je připojeno jako suffix.
 * @param identifier Ukazatel na původní jméno proměnné typu `DString`,
 *                   ke kterému bude suffix připojen.
 *
 * @return Ukazatel na `DString` s přidaným suffixem. Pokud připojení selže,
 *         vrací `NULL`.
 */
DString *parser_addIdentifierSuffix(size_t frameID, DString *identifier);


/*******************************************************************************
 *                                                                             *
 *                   VEŘEJNÉ FUNKCE PRO SYNTAKTICKOU ANALÝZU                   *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Parsuje neterminál `<PROGRAM>`.
 *
 * @return Ukazatel na uzel programu (AST_ProgramNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_ProgramNode *parser_parseProgram();

/**
 * @brief Parsuje neterminál `<PROLOG>`.
 *
 * @return `true` při úspěšném parsování, jinak `false`.
 */
bool parser_parsePrologue();

/**
 * @brief Parsuje neterminál `<FUN_DEF_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu definic funkcí (AST_FunDefNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_FunDefNode *parser_parseFunDefList();

/**
 * @brief Parsuje neterminál `<FUN_DEF>`.
 *
 * @return Ukazatel na uzel definice funkce (AST_FunDefNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_FunDefNode *parser_parseFunDef();

/**
 * @brief Parsuje neterminál `<PARAMETERS>`.
 *
 * @return Ukazatel na první uzel seznamu parametrů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseParameters();

/**
 * @brief Parsuje neterminál `<PARAM_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu parametrů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseParamList();

/**
 * @brief Parsuje neterminál `<PARAM_LIST_REST>`.
 *
 * @return Ukazatel na další uzel v seznamu parametrů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseParamListRest();

/**
 * @brief Parsuje neterminál `<PARAM>`.
 *
 * @return Ukazatel na uzel parametru (AST_ArgOrParamNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseParam();

/**
 * @brief Parsuje neterminál `<RETURN_TYPE>`.
 *
 * @return Hodnota datového typu (AST_DataType) při úspěšném parsování,
 *         jinak `AST_DATA_TYPE_NOT_DEFINED`.
 */
AST_DataType parser_parseReturnType();

/**
 * @brief Parsuje neterminál `<DATA_TYPE>`.
 *
 * @return Hodnota datového typu (AST_DataType) při úspěšném parsování,
 *         jinak `AST_DATA_TYPE_NOT_DEFINED`.
 */
AST_DataType parser_parseDataType();

/**
 * @brief Parsuje neterminál `<STATEMENT_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu příkazů (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *parser_parseStatementList();

/**
 * @brief Parsuje neterminál `<STATEMENT>`.
 *
 * @return Ukazatel na uzel příkazu (AST_StatementNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_StatementNode *parser_parseStatement();

/**
 * @brief Parsuje neterminál `<VAR_DEF>`.
 *
 * @return Ukazatel na uzel definice proměnné (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *parser_parseVarDef();

/**
 * @brief Parsuje neterminál `<MODIFIABLE>`.
 *
 * @return `true` při úspěšném parsování, jinak `false`.
 */
bool parser_parseModifiable();

/**
 * @brief Parsuje neterminál `<POSSIBLE_TYPE>`.
 *
 * @return Hodnota datového typu (AST_DataType), nebo `AST_DATA_TYPE_NOT_DEFINED`
 *         pokud typ není uveden.
 */
AST_DataType parser_parsePossibleType();

/**
 * @brief Parsuje neterminál `<STATEMENT_REST>`.
 *
 * @return Ukazatel na uzel příkazu (AST_StatementNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_StatementNode *parser_parseStatementRest();

/**
 * @brief Parsuje neterminál `<IF>`.
 *
 * @return Ukazatel na uzel podmíněného příkazu (AST_IfNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_IfNode *parser_parseIf();

/**
 * @brief Parsuje neterminál `<NULL_COND>`.
 *
 * @return `true` při úspěšném parsování, jinak `false`.
 */
AST_VarNode *parser_parseNullCond();

/**
 * @brief Parsuje neterminál `<SEQUENCE>`.
 *
 * @return Ukazatel na uzel sekvence příkazů (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *parser_parseSequence();

/**
 * @brief Parsuje neterminál `<WHILE>`.
 *
 * @return Ukazatel na uzel smyčky while (AST_WhileNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_WhileNode *parser_parseWhile();

/**
 * @brief Parsuje neterminál `<RETURN>`.
 *
 * @return Ukazatel na uzel návratového příkazu (AST_StatementNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_StatementNode *parser_parseReturn();

/**
 * @brief Parsuje neterminál `<RETURN_REST>`.
 *
 * @return Ukazatel na výraz (AST_ExprNode), může být `NULL` pokud
 *         není uveden výraz.
 */
AST_ExprNode *parser_parseReturnRest();

/**
 * @brief Parsuje neterminál `<EXPR>`.
 *
 * @return Ukazatel na uzel výrazu (AST_ExprNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_ExprNode *parser_parseExpr();

/**
 * @brief Parsuje neterminál `<REL_EXPR>`.
 *
 * @return Ukazatel na uzel relačního výrazu (AST_BinOpNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_BinOpNode *parser_parseRelExpr();

/**
 * @brief Parsuje neterminál `<REL_EXPR_REST>`.
 *
 * @param[in] leftNode Levý operand (AST_ExprNode).
 * @return Ukazatel na uzel relačního výrazu (AST_BinOpNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_BinOpNode *parser_parseRelExprRest(AST_ExprNode *leftNode);

/**
 * @brief Parsuje neterminál `<REL_OP>`.
 *
 * @return Typ operátoru (AST_BinOpType) při úspěšném parsování, jinak
 *         `AST_OP_NOT_DEFINED`.
 */
AST_BinOpType parser_parseRelOp();

/**
 * @brief Parsuje neterminál `<ADD_EXPR>`.
 *
 * @return Ukazatel na uzel aditivního výrazu (AST_ExprNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ExprNode *parser_parseAddExpr();

/**
 * @brief Parsuje neterminál `<ADD_EXPR_REST>`.
 *
 * @param[in] leftNode Levý operand (AST_ExprNode).
 * @return Ukazatel na uzel aditivního výrazu (AST_ExprNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ExprNode *parser_parseAddExprRest(AST_ExprNode *leftNode);

/**
 * @brief Parsuje neterminál `<ADD_OP>`.
 *
 * @return Typ operátoru (AST_BinOpType) při úspěšném parsování, jinak
 *         `AST_OP_NOT_DEFINED`.
 */
AST_BinOpType parser_parseAddOp();

/**
 * @brief Parsuje neterminál `<MULTI_EXPR>`.
 *
 * @return Ukazatel na uzel multiplikativního výrazu (AST_ExprNode)
 *         při úspěšném parsování, jinak `NULL`.
 */
AST_ExprNode *parser_parseMultiExpr();

/**
 * @brief Parsuje neterminál `<MULTI_EXPR_REST>`.
 *
 * @param[in] leftNode Levý operand (AST_ExprNode).
 * @return Ukazatel na uzel multiplikativního výrazu (AST_ExprNode)
 *         při úspěšném parsování, jinak `NULL`.
 */
AST_ExprNode *parser_parseMultiExprRest(AST_ExprNode *leftNode);

/**
 * @brief Parsuje neterminál `<MULTI_OP>`.
 *
 * @return Typ operátoru (AST_BinOpType) při úspěšném parsování, jinak
 *         `AST_OP_NOT_DEFINED`.
 */
AST_BinOpType parser_parseMultiOp();

/**
 * @brief Parsuje neterminál `<TERM>`.
 *
 * @return Ukazatel na uzel termu (AST_ExprNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_ExprNode *parser_parseTerm();

/**
 * @brief Parsuje neterminál `<TERM_REST>`.
 *
 * @param[in] leftNode Levý operand (AST_ExprNode).
 * @return Ukazatel na uzel termu (AST_ExprNode) při úspěšném parsování,
 *         jinak `NULL`.
 */
AST_ExprNode *parser_parseTermRest(AST_ExprNode *leftNode);

/**
 * @brief Parsuje neterminál `<ARGUMENTS>`.
 *
 * @return Ukazatel na první uzel seznamu argumentů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseArguments();

/**
 * @brief Parsuje neterminál `<ARG_LIST>`.
 *
 * @return Ukazatel na první uzel seznamu argumentů (AST_ArgOrParamNode) při
 *         úspěšném parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseArgList();

/**
 * @brief Parsuje neterminál `<ARG>`.
 *
 * @return Ukazatel na uzel argumentu (AST_ArgOrParamNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_ArgOrParamNode *parser_parseArg();

/**
 * @brief Parsuje neterminál `<LITERAL>`.
 *
 * @return Ukazatel na uzel literálu (AST_LiteralNode) při úspěšném
 *         parsování, jinak `NULL`.
 */
AST_LiteralNode *parser_parseLiteral();

#endif // PARSER_H_

/*** Konec souboru parser.h ***/
