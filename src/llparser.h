/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           llparser.h                                                *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   04.12.2024                                                *
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
 * @details Tento soubor obsahuje deklarace funkcí pro analýzu terminálů a
 *          neterminálů podle LL gramatiky. Umožňuje tvorbu uzlů abstraktního
 *          syntaktického stromu (AST) a zajišťuje přechody mezi stavy, aby byla
 *          zachována správná struktura kódu během syntaktické analýzy.
 */

#ifndef LLPARSER_H_
/** @cond  */
#define LLPARSER_H_
/** @endcond  */

// Import submodulů parseru
#include "parser_common.h"
#include "lltable.h"
#include "precedence_parser.h"


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define PARSING_SYNTAX_ERROR NULL    /**< Slouží k propagaci chyby syntaxe návratovými hodnotami při rekurzivním sestupu.  */


/*******************************************************************************
 *                                                                             *
 *                        DEKLARACE VEŘEJNÝCH FUNKCÍ                           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Spouští syntaktickou analýzu programu od neterminálu @c \<PROGRAM>.
 *
 * @details Tato funkce inicializuje všechny potřebné struktury, jako je AST,
 *          zásobník rámců a seznam precedenčních zásobníků. Poté začíná
 *          analyzovat program podle LL gramatiky, počínaje neterminálem
 *          @c \<PROGRAM>. Během analýzy jsou vytvářeny uzly AST, které
 *          reprezentují strukturu programu. Na konci kontroluje, zda byl
 *          program korektně zpracován a zda bylo dosaženo dkonce vstupu (EOF).
 */
void LLparser_parseProgram();

/*******************************************************************************
 *                                                                             *
 *            DEKLARACE INTERNÍCH FUNKCÍ PRO PARSOVÁNÍ NETERMINÁLŮ             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Analyzuje neterminál @c \<PROLOGUE>, který reprezentuje úvod programu.
 *
 * @details Funkce zpracovává začátek programu, kde očekává deklaraci konstanty
 *          s názvem @c ifj a importuje soubor @c "ifj24.zig". Během analýzy
 *          kontroluje správnost syntaxe a pořadí očekávaných tokenů. Vytváří
 *          AST uzel pro importovaný soubor a přidává ho do tabulky symbolů
 *          jako konstantu. Pokud dojde k chybě, funkce zajišťuje uvolnění
 *          alokovaných zdrojů a nastavuje příslušné chybové příznaky.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<PROLOGUE> -> const ifj = @import ( "ifj24.zig" ) ;`
 *
 * @return Ukazatel na AST uzel typu @c AST_VarNode, který reprezentuje
 *         importovaný soubor, nebo @c PARSING_ERROR v případě chyby.
 */
AST_VarNode *LLparser_parsePrologue();

/**
 * @brief Analyzuje neterminál @c \<FUN_DEF_LIST>, který reprezentuje seznam
 *        definic funkcí.
 *
 * @details Funkce rekurzivně parsuje jednotlivé definice funkcí pomocí volání
 *          @c LLparser_parseFunDef() a vytváří z nich lineární seznam uzlů typu
 *          @c `AST_FunDefNode. Každá definice funkce je připojena na začátek
 *          seznamu, což umožňuje efektivní sestavení kompletního seznamu definic.
 *          Pokud je seznam prázdný, funkce vrací @c NULL. V případě chyby zajišťuje
 *          uvolnění všech alokovaných zdrojů a nastavuje chybové příznaky.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST>`
 * - `<FUN_DEF_LIST> -> ε`
 *
 * @return Ukazatel na první uzel seznamu definic funkcí typu @c AST_FunDefNode,
 *         nebo @c PARSING_ERROR v případě chyby.
 */
AST_FunDefNode *LLparser_parseFunDefList();

/**
 * @brief Analyzuje neterminál @c \<FUN_DEF>, který reprezentuje definici jedné
 *        funkce.
 *
 * @details Funkce zpracovává kompletní definici funkce, včetně klíčových slov
 *          `pub fn`, identifikátoru funkce, parametrů, návratového typu a těla
 *          funkce. Během analýzy jsou parametry funkce přidávány do tabulky
 *          symbolů a je vytvářen nový rámec pro lokální proměnné. Po úspěšném
 *          analýzy je funkce přidána do globální tabulky symbolů jako definovaná.
 *          V případě chyby jsou všechny alokované zdroje uvolněny a jsou
 *          nastaveny chybové příznaky pro následné ošetření.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<FUN_DEF> -> pub fn id ( <PARAMETERS> ) <RETURN_TYPE> <SEQUENCE>`
 *
 * @return Ukazatel na AST uzel typu @c AST_FunDefNode, který reprezentuje
 *         definici funkce, nebo @c PARSING_ERROR v případě chyby.
 */
AST_FunDefNode *LLparser_parseFunDef();

/**
 * @brief Analyzuje neterminál @c \<PARAMETERS>, který reprezentuje seznam
 *        parametrů funkce.
 *
 * @details Funkce zpracovává seznam parametrů, který může být buď prázdný, nebo
 *          obsahovat jeden či více parametrů. Pokud jsou parametry přítomny,
 *          funkce volá @c LLparser_parseParamList() pro jejich zpracování.
 *          Počet parametrů je ukládán do proměnné @c paramCount, která je
 *          předávána jako ukazatel. V případě prázdného seznamu je
 *          @c paramCount nastaven na 0.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<PARAMETERS> -> <PARAM_LIST>`
 * - `<PARAMETERS> -> ε`
 *
 * @param [out] paramCount Ukazatel na proměnnou typu @c size_t, kam bude uložen
 *                         počet parametrů.
 * @return Ukazatel na první uzel seznamu parametrů typu @c AST_ArgOrParamNode,
 *         nebo @c PARSING_ERROR v případě chyby.
 */
AST_ArgOrParamNode *LLparser_parseParameters(size_t *paramCount);

/**
 * @brief Analyzuje neterminál @c \<PARAM_LIST>, který reprezentuje neprázdný
 *        seznam parametrů.
 *
 * @details Funkce zpracovává seznam parametrů oddělených čárkami. Rekurzivně
 *          volá sama sebe pro zpracování dalších parametrů v seznamu. Každý
 *          parametr je analyzován pomocí @c LLparser_parseParam() a připojen
 *          do seznamu. Počet parametrů je inkrementován pomocí
 *          ukazatele @c paramCount.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<PARAM_LIST> -> <PARAM> <PARAM_LIST_REST>`
 *
 * @param [out] paramCount Ukazatel na proměnnou typu @c size_t, která udržuje
 *                         počet parametrů.
 * @return Ukazatel na první uzel seznamu parametrů typu @c AST_ArgOrParamNode,
 *         nebo @c PARSING_ERROR v případě chyby.
 */
AST_ArgOrParamNode *LLparser_parseParamList(size_t *paramCount);

/**
 * @brief Analyzuje neterminál @c \<PARAM_LIST_REST>, který reprezentuje
 *        pokračování seznamu parametrů.
 *
 * @details Funkce zpracovává případné další parametry v seznamu po první čárce.
 *          Pokud je nalezena čárka, funkce volá @c LLparser_parseParamList()
 *          pro zpracování dalších parametrů. V opačném případě se jedná o konec
 *          seznamu parametrů.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<PARAM_LIST_REST> -> , <PARAM_LIST>`
 * - `<PARAM_LIST_REST> -> ε`
 *
 * @param [out] paramCount Ukazatel na proměnnou typu @c size_t, která udržuje
 *                         počet parametrů.
 * @return Ukazatel na první uzel další části seznamu parametrů typu
 *         @c AST_ArgOrParamNode, nebo @c PARSING_ERROR v případě chyby.
 */
AST_ArgOrParamNode *LLparser_parseParamListRest(size_t *paramCount);

/**
 * @brief Analyzuje neterminál @c \<PARAM>, který reprezentuje jeden parametr
 *        funkce.
 *
 * @details Funkce zpracovává jeden parametr funkce, který obsahuje identifikátor
 *          a datový typ oddělený dvojtečkou. Identifikátor parametru je přidán do
 *          tabulky symbolů jako lokální proměnná aktuálního rámce. Funkce vytváří
 *          odpovídající uzel AST typu @c AST_ArgOrParamNode.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<PARAM> -> id : <DATA_TYPE>`
 *
 * @return Ukazatel na AST uzel parametru typu @c AST_ArgOrParamNode,
 *         nebo @c PARSING_ERROR v případě chyby.
 */
AST_ArgOrParamNode *LLparser_parseParam();

/**
 * @brief Analyzuje neterminál @c \<RETURN_TYPE>, který reprezentuje návratový
 *        typ funkce.
 *
 * @details Funkce zpracovává návratový typ funkce, který může být buď konkrétní
 *          datový typ (parsovaný pomocí `LLparser_parseDataType()`), nebo
 *          klíčové slovo @c void. Na základě nalezeného tokenu vrací
 *          odpovídající hodnotu typu @c AST_DataType.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<RETURN_TYPE> -> <DATA_TYPE>`
 * - `<RETURN_TYPE> -> void`
 *
 * @return Datový typ funkce jako hodnota výčtu @c AST_DataType,
 *         nebo @c AST_DATA_TYPE_NOT_DEFINED při chybě.
 */
AST_DataType LLparser_parseReturnType();

/**
 * @brief Analyzuje neterminál @c \<DATA_TYPE>, který reprezentuje podporované
 *        datové typy.
 *
 * @details Funkce zpracovává různé datové typy podporované jazykem, včetně
 *          základních typů jako @c i32, @c f64 a jejich nullable variant
 *          (`?i32`, `?f64`), stejně jako řetězce (`[]u8`) a nullable
 *          řetězce (`?[]u8`). Na základě aktuálního tokenu vrací odpovídající
 *          hodnotu typu @c AST_DataType.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<DATA_TYPE> -> i32`
 * - `<DATA_TYPE> -> ?i32`
 * - `<DATA_TYPE> -> f64`
 * - `<DATA_TYPE> -> ?f64`
 * - `<DATA_TYPE> -> []u8`
 * - `<DATA_TYPE> -> ?[]u8`
 *
 * @return Datový typ jako hodnota výčtu @c AST_DataType,
 *         nebo @c AST_DATA_TYPE_NOT_DEFINED při chybě.
 */
AST_DataType LLparser_parseDataType();

/**
 * @brief Analyzuje neterminál @c \<STATEMENT_LIST>, který reprezentuje
 *        seznam příkazů.
 *
 * @details Funkce rekurzivně parsuje jednotlivé příkazy pomocí volání
 *          @c LLparser_parseStatement() a vytváří z nich propojený seznam
 *          uzlů typu @c AST_StatementNode. Pokud je seznam prázdný, funkce
 *          vrací @c NULL.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST>`
 * - `<STATEMENT_LIST> -> ε`
 *
 * @return Ukazatel na první uzel seznamu příkazů typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseStatementList();

/**
 * @brief Analyzuje neterminál @c \<STATEMENT>, který reprezentuje jeden příkaz
 *        programu.
 *
 * @details Funkce zpracovává různé typy příkazů, jako je definice proměnné,
 *          přiřazení, volání funkce, podmíněný příkaz @c if, cyklus @c while,
 *          návratový příkaz @c return nebo volání vestavěných funkcí @c ifj.
 *          Na základě aktuálního tokenu a LL tabulky rozhoduje, které pravidlo
 *          aplikovat a volá příslušnou funkci pro analýzy.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<STATEMENT> -> <VAR_DEF> ;`
 * - `<STATEMENT> -> id <STATEMENT_REST> ;`
 * - `<STATEMENT> -> _ = <THROW_AWAY> ;`
 * - `<STATEMENT> -> <IF>`
 * - `<STATEMENT> -> <WHILE>`
 * - `<STATEMENT> -> return [precedence_expr] ;`
 * - `<STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;`
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseStatement();

/**
 * @brief Analyzuje neterminál @c \<VAR_DEF>, který reprezentuje definici
 *        proměnné.
 *
 * @details Funkce zpracovává definici proměnné, která může být deklarována
 *          jako modifikovatelná (`var`) nebo konstantní (`const`). Následuje
 *          identifikátor proměnné, případně její datový typ a inicializační
 *          výraz. Proměnná je přidána do tabulky symbolů aktuálního rámce a
 *          je vytvořen odpovídající uzel AST pro příkaz definice proměnné.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<VAR_DEF> -> <MODIFIABLE> id <POSSIBLE_TYPE> = [precedence_expr]`
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseVarDef();

/**
 * @brief Analyzuje neterminál @c \<MODIFIABLE>, který určuje modifikovatelnost
 *        proměnné.
 *
 * @details Funkce zpracovává klíčová slova @c var a @c const, která určují,
 *          zda je proměnná modifikovatelná nebo konstantní. Na základě
 *          nalezeného klíčového slova nastavuje hodnotu proměnné @c isConstant.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<MODIFIABLE> -> var`
 * - `<MODIFIABLE> -> const`
 *
 * @param [out] isConstant Ukazatel na logickou proměnnou, která bude nastavena
 *                         na @c true, pokud je proměnná konstantní (`const`),
 *                         nebo @c false, pokud je modifikovatelná (`var`).
 */
void LLparser_parseModifiable(bool *isConstant);

/**
 * @brief Analyzuje neterminál @c \<POSSIBLE_TYPE>, který reprezentuje případný
 *        datový typ proměnné.
 *
 * @details Funkce zpracovává možnost, že proměnná má explicitně uvedený datový
 *          typ po dvojtečce. Pokud je dvojtečka nalezena, funkce volá
 *          @c LLparser_parseDataType() pro získání konkrétního datového typu.
 *          Pokud dvojtečka chybí, funkce vrací @c AST_DATA_TYPE_NOT_DEFINED.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<POSSIBLE_TYPE> -> : <DATA_TYPE>`
 * - `<POSSIBLE_TYPE> -> ε`
 *
 * @return Datový typ proměnné jako hodnota výčtu @c AST_DataType,
 *         nebo @c AST_DATA_TYPE_NOT_DEFINED, pokud datový typ není uveden.
 */
AST_DataType LLparser_parsePossibleType();

/**
 * @brief Analyzuje neterminál @c \<STATEMENT_REST>, který reprezentuje zbytek
 *        příkazu po identifikátoru.
 *
 * @details Funkce zpracovává dvě možnosti pokračování příkazu po identifikátoru:
 *          buď přiřazení hodnoty do proměnné (volá `LLparser_parseRuleStatementRest1()`),
 *          nebo volání funkce (volá `LLparser_parseRuleStatementRest2()`).
 *          Na základě aktuálního tokenu a LL tabulky rozhoduje, kterou cestou
 *          se vydat.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<STATEMENT_REST> -> = [precedence_expr]`
 * - `<STATEMENT_REST> -> ( <ARGUMENTS> )`
 *
 * @param[in,out] identifier Ukazatel na identifikátor proměnné nebo funkce,
 *                           který byl analyzován před voláním této funkce.
 *                           V případě chyby je identifikátor uvolněn a ukazatel
 *                           je nastaven na @c NULL.
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseStatementRest(DString **identifier);

/**
 * @brief Analyzuje neterminál @c \<THROW_AWAY>, který reprezentuje výraz s
 *        výsledkem k zahození.
 *
 * @details Funkce zpracovává výraz, jehož výsledek není dále využíván, typicky
 *          v příkazu `_ = výraz;`. Volá precedenční parser pro zpracování
 *          výrazu a vrací AST uzel typu @c AST_ExprNode.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<THROW_AWAY> -> [precedence_expr]`
 *
 * @return Ukazatel na AST uzel výrazu typu @c AST_ExprNode, nebo @c PARSING_ERROR
 *         při chybě.
 */
AST_ExprNode *LLparser_parseThrowAway();

/**
 * @brief Analyzuje neterminál @c \<IF>, který reprezentuje podmíněný
 *        příkaz @c if.
 *
 * @details Funkce zpracovává konstrukci podmíněného příkazu @c if, včetně podmínky,
 *          případné null podmínky a těla příkazu. Podmínka je zpracována pomocí
 *          precedenčního parseru. Funkce také spravuje rámce pro bloky kódu
 *          v @c if a @c else větvích. Vytváří AST uzel typu @c AST_IfNode.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<IF> -> if ( [precedence_expr] ) <NULL_COND> <SEQUENCE> else <SEQUENCE>`
 *
 * @return Ukazatel na AST uzel konstrukce @c if typu @c AST_IfNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_IfNode *LLparser_parseIf();

/**
 * @brief Analyzuje neterminál @c \<NT_NULL_COND>, který reprezentuje případnou
 *        null podmínku.
 *
 * @details Funkce zpracovává možnost, že v konstrukci @c if nebo @c while je
 *          použita null podmínka ve tvaru `| id_bez_null |`. Pokud je null
 *          podmínka přítomna, identifikátor @c id_bez_null je přidán do tabulky
 *          symbolů aktuálního rámce. Funkce vrací AST uzel typu @c AST_VarNode
 *          pro tuto proměnnou.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<NULL_COND> -> | id |`
 * - `<NULL_COND> -> ε`
 *
 * @return Ukazatel na AST uzel proměnné typu @c AST_VarNode, pokud je null
 *         podmínka přítomna, nebo @c NULL, pokud není. V případě chyby
 *         vrací @c PARSING_ERROR.
 */
AST_VarNode *LLparser_parseNullCond();

/**
 * @brief Analyzuje neterminál @c \<SEQUENCE>, který reprezentuje blok kódu
 *        ohraničený složenými závorkami.
 *
 * @details Funkce zpracovává blok kódu, který může obsahovat seznam příkazů.
 *          Před analýzou příkazů může být vytvořen nový rámec pro proměnné,
 *          pokud je to vyžadováno parametrem @c createFrame. Po úspěšném
 *          analýzy je vrácen seznam příkazů jako propojený seznam uzlů
 *          typu @c AST_StatementNode.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<SEQUENCE> -> { <STATEMENT_LIST> }`
 *
 * @param [in] createFrame Boolean hodnota určující, zda má být před analýzou
 *                         vytvořen nový rámec.
 * @return Ukazatel na první uzel seznamu příkazů typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseSequence(bool createFrame);

/**
 * @brief Analyzuje neterminál @c \<WHILE>, který reprezentuje cyklus @c while.
 *
 * @details Funkce zpracovává konstrukci cyklu @c while, včetně podmínky,
 *          případné null podmínky a těla cyklu. Podmínka je zpracována pomocí
 *          precedenčního parseru. Spravuje rámce pro tělo cyklu a vytváří AST
 *          uzel typu @c AST_WhileNode.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<WHILE> -> while ( [precedence_expr] ) <NULL_COND> <SEQUENCE>`
 *
 * @return Ukazatel na AST uzel konstrukce @c while typu @c AST_WhileNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_WhileNode *LLparser_parseWhile();

/**
 * @brief Analyzuje neterminál @c \<ARGUMENTS>, který reprezentuje seznam
 *        argumentů funkce.
 *
 * @details Funkce zpracovává seznam argumentů předávaných funkci. Každý
 *          argument je analyzován pomocí precedenčního parseru a přidán do
 *          seznamu argumentů jako uzel typu @c AST_ArgOrParamNode. Pokud je
 *          seznam argumentů prázdný, funkce vrací @c NULL.
 * @note
 * Neterminál se může dále rozvíjet na:
 * - `<ARGUMENTS> -> [precedence_expr]`
 *
 * @return Ukazatel na první uzel seznamu argumentů typu @c AST_ArgOrParamNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_ArgOrParamNode *LLparser_parseArguments();


/*******************************************************************************
 *                                                                             *
 *           DEKLARACE INTERNÍCH FUNKCÍ PRO APLIKACI NĚKTERÝCH PRAVIDEL        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> <VAR_DEF> ;`.
 *
 * @details Funkce zpracovává definici proměnné následovanou středníkem.
 *          Volá @c LLparser_parseVarDef() pro analýzy definice proměnné
 *          a poté kontroluje přítomnost středníku. Vytváří AST uzel typu
 *          @c AST_StatementNode pro příkaz definice proměnné.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement1();

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> id <STATEMENT_REST> ;`.
 *
 * @details Funkce zpracovává příkaz začínající identifikátorem, který může být
 *          přiřazením hodnoty do proměnné nebo voláním funkce. Po analýzy
 *          identifikátoru volá @c LLparser_parseStatementRest() pro zpracování
 *          zbytku příkazu. Nakonec kontroluje přítomnost středníku.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement2();

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> _ = <THROW_AWAY> ;`.
 *
 * @details Funkce zpracovává příkaz, kde je výsledek výrazu zahozen, například
 *          při volání funkce, jejíž návratová hodnota není potřebná. Vytváří
 *          AST uzly pro proměnnou @c _ , přiřazení a výraz. Nakonec kontroluje
 *          přítomnost středníku.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement3();

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> <IF>`.
 *
 * @details Funkce zpracovává podmíněný příkaz @c if voláním @c LLparser_parseIf().
 *          Vytváří AST uzel typu @c AST_StatementNode pro příkaz obsahující
 *          konstrukci @c if.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement4();

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> <WHILE>`.
 *
 * @details Funkce zpracovává cyklus @c while voláním @c LLparser_parseWhile().
 *          Vytváří AST uzel typu @c AST_StatementNode pro příkaz obsahující
 *          konstrukci @c while.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement5();

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> return [precedence_expression] ;`.
 *
 * @details Funkce zpracovává návratový příkaz @c return, který může obsahovat
 *          návratový výraz. Výraz je zpracován pomocí precedenčního parseru.
 *          Nakonec funkce kontroluje přítomnost středníku a vytváří AST uzel
 *          typu @c AST_StatementNode pro návratový příkaz.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement6();

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;`.
 *
 * @details Funkce zpracovává volání vestavěné funkce z modulu @c ifj. Analyzuje
 *          identifikátor funkce, seznam argumentů a kontroluje přítomnost
 *          středníku. Vytváří AST uzel typu @c AST_StatementNode pro příkaz
 *          volání funkce.
 *
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatement7();

/**
 * @brief Analyzuje pravidlo `<STATEMENT_REST> -> = [precedence_expression]`.
 *
 * @details Funkce zpracovává přiřazení hodnoty do proměnné po identifikátoru.
 *          Volá precedenční parser pro zpracování výrazu na pravé straně
 *          přiřazení. Vytváří AST uzly pro proměnnou, výraz a přiřazení.
 *
 * @param [in,out] identifier Ukazatel na identifikátor proměnné.
 *                           V případě chyby je identifikátor uvolněn.
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
*/
AST_StatementNode *LLparser_parseRuleStatementRest1(DString **identifier);

/**
 * @brief Analyzuje pravidlo `<STATEMENT_REST> -> ( <ARGUMENTS> )`.
 *
 * @details Funkce zpracovává volání uživatelské funkce po identifikátoru.
 *          Analyzuje seznam argumentů a vytváří AST uzel typu @c AST_StatementNode
 *          pro příkaz volání funkce.
 *
 * @param [in,out] identifier Ukazatel na identifikátor funkce. V případě chyby
 *                            je identifikátor uvolněn.
 * @return Ukazatel na AST uzel příkazu typu @c AST_StatementNode,
 *         nebo @c PARSING_ERROR při chybě.
 */
AST_StatementNode *LLparser_parseRuleStatementRest2(DString **identifier);

/*******************************************************************************
 *                                                                             *
 *              DEKLARACE POMOCNÝCH INTERNÍCH FUNKCÍ LL PARSERU                *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje potřebné struktury pro LL syntaktický analyzátor.
 *
 * @details Funkce resetuje statické proměnné, inicializuje AST strom,
 *          zásobník rámců a seznam precedenčních zásobníků. Připravuje parser
 *          pro syntaktickou analýzu programu. V případě chyby nastavuje
 *          příslušné chybové příznaky.
 */
void LLparser_initParserStructures();

/**
 * @brief Kontroluje, zda aktuální terminál odpovídá očekávanému terminálu.
 *
 * @details Funkce porovnává aktuální terminál uložený v globální proměnné
 *          @c currentTerminal s očekávaným terminálem předaným jako parametr.
 *          Pokud se terminály neshodují, funkce nastaví syntaktickou chybu a
 *          vrátí @c true. V opačném případě vrátí @c false.
 *
 * @param [in] expectedTerminal Očekávaný terminál typu @c LLTerminals.
 * @return @c True, pokud terminály nesouhlasí (došlo k chybě), jinak @c false.
*/
bool LLparser_isNotExpectedTerminal(LLTerminals expectedTerminal);

#endif // LLPARSER_H

/*** Konec souboru llparser.h ***/
