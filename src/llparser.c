/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           llparser.c                                                *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            01.11.2024                                                *
 * Poslední změna:   25.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file llparser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace LL syntaktického analyzátoru.
 * @details Tento soubor obsahuje implementaci funkcí pro parsování neterminálů
 *          podle LL gramatiky. Umožňuje tvorbu uzlů abstraktního syntaktického
 *          stromu (AST) a zajišťuje přechody mezi stavy, aby byla zachována
 *          správná struktura kódu během syntaktické analýzy.
 */

#include "llparser.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

// <PROGRAM> -> <PROLOGUE> <FUN_DEF_LIST> EOF
void LLparser_parseProgram() {
    // Alokujeme globální struktury a resetujeme statické proměnné
    LLparser_initParserStructures();

    // Inicializujeme "currentTerminal" a "lookaheadToken" prvním voláním scanneru
    Parser_getNextToken(POKE_SCANNER);

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_PROGRAM, &rule);

    // Volba dalšího postupu parsování na základě LL pravidla
    // <PROGRAM> -> <PROLOGUE> <FUN_DEF_LIST> EOF
    if(rule == PROGRAM) {
        // Parsujeme <PROLOGUE>
        ASTroot->importedFile = LLparser_parsePrologue();

        // Kontrolujeme, že při parsování <PROLOGUE> nedošlo k chybě
        if(Parser_errorWatcher(IS_PARSING_ERROR)) {
            goto parseProgram_handleError;  // skok na řešení chyb na konci funkce
        }

        // Žádáme o další token
        Parser_getNextToken(POKE_SCANNER);

        // Parsujeme <FUN_DEF_LIST>
        AST_FunDefNode *funDefNode = LLparser_parseFunDefList();

        // Kontrolujeme, že při parsování <FUN_DEF_LIST> nedošlo k chybě
        if(Parser_errorWatcher(IS_PARSING_ERROR)) {
            goto parseProgram_handleError;  // skok na řešení chyb na konci funkce
        }

        // Pokud k chybě nedošlo, připojíme seznam definic funkcí ke kořeni AST
        ASTroot->functionList = funDefNode;

        // Parsujeme "EOF"
        if(LLparser_isNotExpectedTerminal(T_EOF)) {
            goto parseProgram_handleError;  // skok na řešení chyb na konci funkce
        }
    }
    // Pokud došlo v rámci výběru pravidla k chybě, skáčeme na ošetření chyb
    else {
        goto parseProgram_handleError;  // skok na řešení chyb na konci funkce
    }

    // Program byl syntakticky správný a ukončujeme parsování
    Parser_freeCurrentTerminalValue();
    return;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseProgram_handleError:
        Parser_freeCurrentTerminalValue();
        Parser_errorWatcher(CALL_ERROR_HANDLE);
} // LLparser_parseProgram()


/*******************************************************************************
 *                                                                             *
 *           IMPLEMENTACE INTERNÍCH FUNKCÍ PRO PARSOVÁNÍ NETERMINÁLŮ           *
 *                                                                             *
 ******************************************************************************/

// <PROLOGUE> -> const ifj = @import ( "ifj24.zig" ) ;
AST_VarNode *LLparser_parsePrologue() {
    // Vytváříme flagy pro řízení uvolňování paměti při chybách
    bool errorLevel_1 = false;
    bool errorLevel_2 = false;

    // Definujeme název importované proměnné (scanner ji zpracovává jako keyword)
    DString *importVar = string_charToDString("ifj");

    // Definujeme název očekávané cesty k importovanému souboru
    DString *path = string_charToDString("ifj24.zig");

    // Kontrolujeme, že byla paměť stringům úspěšně přidělena
    if(importVar == NULL || path == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Parsujeme "const"
    if(LLparser_isNotExpectedTerminal(T_CONST)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "ifj"
    if(LLparser_isNotExpectedTerminal(T_IFJ)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme `=`
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme `@import`
    if(LLparser_isNotExpectedTerminal(T_IMPORT)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme `(`
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme `"ifj24.zig"`
    if(currentTerminal.PrecTerminal != T_PREC_STRING_LITERAL) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Kontrolujeme přijetí očekávaného souboru k importování
    if(string_compare_const_str(currentTerminal.value, path->str) != STRING_EQUAL) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Uvolníme obsah aktuálního tokenu
    Parser_freeCurrentTerminalValue();

    // Přidáme proměnnou prologu do tabulky symbolů a zkontrolujeme úspěšnost přidání
    frame_stack_result result = frameStack_addItemExpress(importVar, SYMTABLE_SYMBOL_VARIABLE_STRING, \
                                                          IS_CONST, path, NULL);

    // Kontrola úspěšného vložení do tabulky symbolů
    if(result != FRAME_STACK_SUCCESS) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Vytvoříme AST uzel pro proměnnou s cestou k importovanému souboru
    AST_VarNode *importedFile = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Inicializujeme AST uzel pro proměnnou s cestou k importovanému souboru
    AST_initNewVarNode(importedFile, AST_VAR_NODE, importVar, \
                       frameStack.top->frameID, AST_LITERAL_STRING, path);

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme `)`
    if(LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        errorLevel_2 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme `;`
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        errorLevel_2 = true;
        goto parsePrologue_handleError;  // skok na řešení chyb na konci funkce
    }

    // Vracíme uzel pro v prologu importovaný soubor
    return importedFile;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parsePrologue_handleError:
        if(errorLevel_1) {
            // Uvolnění paměti pro importVar, pokud není NULL
            if (importVar != NULL && importVar != currentTerminal.value) {
                string_free(importVar);
                importVar = NULL;
            }

            // Uvolnění paměti pro path, pokud není NULL
            if (path != NULL && path != currentTerminal.value) {
                string_free(path);
                path = NULL;
            }
        }
        else if(errorLevel_2) {
            // Uvolnění paměti pro importedFile, pokud došlo k chybě po jeho vytvoření
            AST_destroyNode(AST_VAR_NODE, importedFile);
        }

        // Uvolnění hodnoty currentTerminal
        Parser_freeCurrentTerminalValue();

        // Návrat s chybovým kódem
        return PARSING_ERROR;
} // LLparser_parsePrologue()

// <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST> | ε
AST_FunDefNode *LLparser_parseFunDefList() {
    // Inicializujeme proměnné pro zdroje funkce
    AST_FunDefNode *funDef = NULL;
    AST_FunDefNode *funDefList = NULL;

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_FUN_DEF_LIST, &rule);

    // Na <FUN_DEF_LIST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST>
        case FUN_DEF_LIST_1: {
            // Parsujeme <FUN_DEF>
            funDef = LLparser_parseFunDef();

            // Kontrola úspěchu parsování <FUN_DEF>
            if(Parser_errorWatcher(IS_PARSING_ERROR)) {
                goto parseFunDefList_errorLevel_0;  // skok na řešení chyb na konci funkce
            }

            // Parsujeme zbytek seznamu <FUN_DEF_LIST>
            funDefList = LLparser_parseFunDefList();

            // Kontrola úspěchu parsování <FUN_DEF_LIST>
            if(Parser_errorWatcher(IS_PARSING_ERROR)) {
                goto parseFunDefList_errorLevel_1;  // skok na řešení chyb na konci funkce
            }

            // Připojíme aktuální definici funkce na začátek seznamu
            funDef->next = funDefList;

            // Vrátíme ukazatel na seznam definic funkcí
            return funDef;
        }

        // <FUN_DEF_LIST> -> ε
        case FUN_DEF_LIST_2:
            return NULL;    // Vracíme NULL, jelikož rozvíjíme na prázdný řetězec

        // Defaultní stav: došlo k chybě
        default:
            goto parseFunDefList_errorLevel_0;  // skok na řešení chyb na konci funkce
    } // switch()


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseFunDefList_errorLevel_1:
        // Uvolníme uzel s definicemi funkcí
            AST_destroyFunDefNode(funDef);
    parseFunDefList_errorLevel_0:
        // Uvolnění hodnoty currentTerminal
        Parser_freeCurrentTerminalValue();

        // Návrat s chybovým kódem
        return PARSING_ERROR;
} // LLparser_parseFunDefList()

// <FUN_DEF> -> pub fn id ( <PARAMETERS> ) <RETURN_TYPE> <SEQUENCE>
AST_FunDefNode *LLparser_parseFunDef() {
    // Parsujeme "pub"
    if(LLparser_isNotExpectedTerminal(T_PUB)) {
        goto parseFunDef_errorLevel_0;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "fn"
    if(LLparser_isNotExpectedTerminal(T_FN)) {
        goto parseFunDef_errorLevel_0;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseFunDef_errorLevel_0;  // skok na řešení chyb na konci funkce
    }

    // Uchováme identifikátor funkce
    DString *functionName = currentTerminal.value;

    // Přidáme definici funkce do tabulky symbolů
    SymtableItem *functionItem = NULL;
    frame_stack_result result = frameStack_addItemExpress(functionName, SYMTABLE_SYMBOL_FUNCTION, IS_VAR, NULL, &functionItem);
    if(result != FRAME_STACK_SUCCESS) {
        if(result == FRAME_STACK_ITEM_ALREADY_EXISTS) {
            Parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }

        goto parseFunDef_errorLevel_0;  // skok na řešení chyb na konci funkce
    }


    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseFunDef_errorLevel_1;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Pushneme nový rámce příslušící parsované funkci
    frameStack_push(true);

    // Uložíme ID rámce těla funkce pro tabulku symbolů
    size_t function_frameID = frameStack.top->frameID;

    // Počítadlo parametrů funkce
    size_t paramCount = 0;

    // Parsujeme <PARAMETERS>
    AST_ArgOrParamNode *parameters = LLparser_parseParameters(&paramCount);

    // Kontrola úspěchu parsování <PARAMETERS>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_2;  // skok na řešení chyb na konci funkce
    }

    // Parsujeme ")"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        goto parseFunDef_errorLevel_2;  // skok na řešení chyb na konci funkce
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme <RETURN_TYPE>
    AST_DataType returnType = LLparser_parseReturnType();

    // Kontrola úspěchu parsování <RETURN_TYPE>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_2;  // skok na řešení chyb na konci funkce
    }

    // Parsujeme <SEQUENCE>
    AST_StatementNode *sequence = LLparser_parseSequence(false);

    // Kontrola úspěchu parsování <SEQUENCE>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_3;  // skok na řešení chyb na konci funkce
    }

    // Vytvoříme AST uzel pro definici funkce
    AST_FunDefNode *funDefNode = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);

    // Kontrola úspěchu vytvoření AST uzlu
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_3;  // skok na řešení chyb na konci funkce
    }

    // Inicializace uzlu pro definici funkce
    AST_initNewFunDefNode(funDefNode, functionName, parameters, returnType, sequence);

    // Vytvoříme strukturu SymtableFunctionData pro uložení do tabulky symbolů
    SymtableFunctionData *functionData = symtable_init_function_data(paramCount);

    // Kontrola úspěchu vytvoření dat pro funkci
    if(functionData == NULL) {
        goto parseFunDef_errorLevel_4;  // skok na řešení chyb na konci funkce
    }

    // Naplníme strukturu SymtableFunctionData parametry
    AST_ArgOrParamNode *paramNode = parameters;
    symtable_functionReturnType funType = SYMTABLE_TYPE_UNKNOWN;
    for(size_t i = 0; i < paramCount; i++) {
        if(paramNode->expression->exprType != AST_EXPR_VARIABLE) {
            goto parseFunDef_errorLevel_4;  // skok na řešení chyb na konci funkce
        }
        AST_VarNode *variable = (AST_VarNode *)(paramNode->expression->expression);
        Parser_mapASTDataTypeToFunReturnType(paramNode->dataType, &funType);
        functionData->params[i].id = variable->identifier;
        functionData->params[i].type = funType;
        paramNode = paramNode->next;
    }

    Parser_mapASTDataTypeToFunReturnType(returnType, &funType);
    functionData->body_frameID = function_frameID;
    functionData->return_type = funType;

    // Přidáme data funkce do položky v tabulce symbolů
    functionItem->data = functionData;

    // Popneme rámce parsované funkce po dokončení jejího parsování
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    return funDefNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseFunDef_errorLevel_4:
        AST_destroyNode(AST_FUN_DEF_NODE, funDefNode);
    parseFunDef_errorLevel_3:
        AST_destroyStatementList(sequence);
    parseFunDef_errorLevel_2:
        AST_destroyArgOrParamList(parameters);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
    parseFunDef_errorLevel_1:
        if(functionName != NULL && functionName != currentTerminal.value) {
            string_free(functionName);
            functionName = NULL;
        }
    parseFunDef_errorLevel_0:
        // Uvolnění hodnoty currentTerminal
        Parser_freeCurrentTerminalValue();

        // Návrat s chybovým kódem
        return PARSING_ERROR;
} // LLparser_parseFunDef()

// <PARAMETERS> -> <PARAM_LIST> | ε
AST_ArgOrParamNode *LLparser_parseParameters(size_t *paramCount) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_PARAMETERS, &rule);

    // Na <PARAMETERS> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <PARAMETERS> -> <PARAM_LIST>
        case PARAMETERS_1:
            return LLparser_parseParamList(paramCount);

        // <PARAMETERS> -> ε
        case PARAMETERS_2:
            *paramCount = 0;
            return NULL;    // NULL nesignalizuje SYTAX ERROR, jelikož ε se rozvíjí na NULL

        // Jinak došlo k syntaktické chybě
        default:
            Parser_freeCurrentTerminalValue();
            return PARSING_ERROR;
    }
} // LLparser_parseParameters()

// <PARAM_LIST> -> <PARAM> <PARAM_LIST_REST>
AST_ArgOrParamNode *LLparser_parseParamList(size_t *paramCount) {
    // Parsujeme <PARAM>
    AST_ArgOrParamNode *paramList = LLparser_parseParam();

    // Kontrola úspěchu parsování <PARAM>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParamList_errorLevel_0;
    }

    // Zvýšíme počet parametrů
    (*paramCount)++;

    // Parsujeme <PARAM_LIST_REST>
    AST_ArgOrParamNode *paramListRest = LLparser_parseParamListRest(paramCount);

    // Kontrola úspěchu parsování <PARAM_LIST_REST>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParamList_errorLevel_1;
    }

    // Kontrola úspěchu parsování <PARAM_LIST_REST>
    if(paramListRest != NULL) {
        paramList->next = paramListRest;
    }

    // Vracíme <PARAM_LIST>
    return paramList;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseParamList_errorLevel_1:
        AST_destroyArgOrParamList(paramList);
    parseParamList_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseParamList()

// <PARAM_LIST_REST> -> , <PARAM_LIST> | ε
AST_ArgOrParamNode *LLparser_parseParamListRest(size_t *paramCount) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_PARAM_LIST_REST, &rule);

     // Na <PARAM_LIST_REST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <PARAM_LIST_REST> -> , <PARAM_LIST>
        case PARAM_LIST_REST_1: {
            if(LLparser_isNotExpectedTerminal(T_COMMA)) {
                return PARSING_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Parsujeme <PARAM_LIST>
            AST_ArgOrParamNode *paramList = LLparser_parseParamList(paramCount);

            // Kontrola úspěchu parsování <PARAM_LIST>
            if(Parser_errorWatcher(IS_PARSING_ERROR)) {
                return PARSING_ERROR;
            }

            // Vrátíme ukazatel na seznam parametrů
            return paramList;
        }

        // <PARAM_LIST_REST> -> ε
        case PARAM_LIST_REST_2:
            // Vracíme NULL, jelikož rozvíjíme na prázdný řetězec
            return NULL;

        // Defaultní stav: došlo k syntaktické chybě
        default:
            return PARSING_ERROR;
    } // switch()
} // LLparser_parseParamListRest()

// <PARAM> -> id : <DATA_TYPE>
AST_ArgOrParamNode *LLparser_parseParam() {
    // Parsujeme "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseParam_errorLevel_0;
    }

    // Identifikátorem parametru je hodnota token
    DString *paramId = currentTerminal.value;

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme ":"
    if(LLparser_isNotExpectedTerminal(T_COLON)) {
        goto parseParam_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "<DATA_TYPE>"
    AST_DataType dataType = LLparser_parseDataType();

    // Kontrola validity získaného návratového typu
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParam_errorLevel_1;
    }

    // Vytvoříme AST uzel pro parametr
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParam_errorLevel_1;
    }

    // Inicializujeme uzel pro proměnnou parametru
    AST_initNewVarNode(varNode, AST_VAR_NODE, paramId, frameStack.top->frameID, AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

    // Mapujeme AST datový typ na Symtable SymbolState
    symtable_symbolState state = SYMTABLE_SYMBOL_UNKNOWN;
    Parser_mapASTDataTypeToSymtableState(dataType, &state);

    // Přidáme parametr do tabulky symbolů jako lokální proměnnou
    frame_stack_result result = frameStack_addItemExpress(paramId, state, IS_CONST, NULL, NULL);
    if(result != FRAME_STACK_SUCCESS) {
        if(result == FRAME_STACK_ITEM_ALREADY_EXISTS) {
            Parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        AST_destroyNode(AST_VAR_NODE, varNode);
        goto parseParam_errorLevel_0;
    }

    // Vytvoříme uzel pro výraz parametru
    AST_ExprNode *paramExpr = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_VAR_NODE, varNode);
        goto parseParam_errorLevel_0;
    }

    // Inicializujeme výraz pro parametr
    AST_initNewExprNode(paramExpr, AST_EXPR_VARIABLE, varNode);

    AST_ArgOrParamNode *paramNode = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, paramExpr);
        goto parseParam_errorLevel_0;
    }
    AST_initNewArgOrParamNode(paramNode, dataType, paramExpr);

    // Vracíme <PARAM>
    return paramNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseParam_errorLevel_1:
        if(paramId != NULL && paramId != currentTerminal.value) {
            string_free(paramId);
            paramId = NULL;
        }
    parseParam_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseParam()

// <RETURN_TYPE> -> <DATA_TYPE> | void
AST_DataType LLparser_parseReturnType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_RETURN_TYPE, &rule);

    // Na <RETURN_TYPE> lze aplikovat dvě různá pravidla
    switch (rule) {
        // <RETURN_TYPE> -> <DATA_TYPE>
        case RETURN_TYPE_1:
            // Parsujeme <DATA_TYPE>
            return LLparser_parseDataType();

        // <RETURN_TYPE> -> void
        case RETURN_TYPE_2:
            if(LLparser_isNotExpectedTerminal(T_VOID)) {
                goto parseReturnType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "void"
            return AST_DATA_TYPE_VOID;

        // Defaultní stav: došlo k syntaktické chybě
        default:
            goto parseReturnType_errorHandle;
    } // switch()


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseReturnType_errorHandle:
        Parser_freeCurrentTerminalValue();
        return AST_DATA_TYPE_NOT_DEFINED;
} // LLparser_parseReturnType()

// <DATA_TYPE> -> i32 | ?i32 | f64 | ?f64 | []u8 | ?[]u8
AST_DataType LLparser_parseDataType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_DATA_TYPE, &rule);

    // Na <DATA_TYPE> lze aplikovat šest různých pravidel
    switch(rule) {
        // <DATA_TYPE> -> i32
        case DATA_TYPE_1:
            if (LLparser_isNotExpectedTerminal(T_INT)) {
                goto parseDataType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "i32"
            return AST_DATA_TYPE_INT;

        // <DATA_TYPE> -> ?i32
        case DATA_TYPE_2:
            if (LLparser_isNotExpectedTerminal(T_INT_OR_NULL)) {
                goto parseDataType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "?i32"
            return AST_DATA_TYPE_INT_OR_NULL;

        // <DATA_TYPE> -> f64
        case DATA_TYPE_3:
            if (LLparser_isNotExpectedTerminal(T_FLOAT)) {
                goto parseDataType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "f64"
            return AST_DATA_TYPE_FLOAT;

        // <DATA_TYPE> -> ?f64
        case DATA_TYPE_4:
            if (LLparser_isNotExpectedTerminal(T_FLOAT_OR_NULL)) {
                goto parseDataType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "?f64"
            return AST_DATA_TYPE_FLOAT_OR_NULL;

        // <DATA_TYPE> -> []u8
        case DATA_TYPE_5:
            if (LLparser_isNotExpectedTerminal(T_STRING)) {
                goto parseDataType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "[]u8"
            return AST_DATA_TYPE_STRING;

        // <DATA_TYPE> -> ?[]u8
        case DATA_TYPE_6:
            if (LLparser_isNotExpectedTerminal(T_STRING_OR_NULL)) {
                goto parseDataType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme typ "?[]u8"
            return AST_DATA_TYPE_STRING_OR_NULL;

        // Defaultní stav: došlo k syntaktické chybě
        default:
            goto parseDataType_errorHandle;
    } // switch()

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseDataType_errorHandle:
        Parser_freeCurrentTerminalValue();
        return AST_DATA_TYPE_NOT_DEFINED;
} // LLparser_parseDataType()

// <STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST> | ε
AST_StatementNode *LLparser_parseStatementList() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_STATEMENT_LIST, &rule);

    // Na <STATEMENT_LIST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST>
        case STATEMENT_LIST_1: {
            // Vytvoříme uzel pro příkaz a parsujeme <STATEMENT>
            AST_StatementNode *statement = LLparser_parseStatement();

            // Kontrola úspěšnosti parsování <STATEMENT>
            if(Parser_errorWatcher(IS_PARSING_ERROR)) {
                AST_destroyStatementList(statement);
                goto parseStatementList_errorHandle;
            }

            // Vytvoříme uzel pro příkaz a parsujeme další příkazy <STATEMENT_LIST>
            AST_StatementNode *statementList = LLparser_parseStatementList();

            // Pokud není navrácený seznam příkazů prázdný, připojíme ho ke stávajícímmu
            if(statementList != NULL) {
                statement->next = statementList;
            }

            // Vracíme příkaz
            return statement;
        }

        // <STATEMENT_LIST> -> ε
        case STATEMENT_LIST_2:
            return NULL;

        // Jiné pravidlo značí syntaktickou chybu
        default:
            goto parseStatementList_errorHandle;
    }

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseStatementList_errorHandle:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLaprser_parseStatementList()

// <STATEMENT> -> <VAR_DEF> ; | id <STATEMENT_REST> ; | _ = <THROW_AWAY> ; | <IF> | <WHILE> | return [precedence_expression] ; | ifj . id ( <ARGUMENTS> ) ;
AST_StatementNode *LLparser_parseStatement() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_STATEMENT, &rule);

    // Na <STATEMENT> lze aplikovat šest různých pravidel
    switch (rule) {
        // <STATEMENT> -> <VAR_DEF> ;
        case STATEMENT_1:
            return LLparser_parseRuleStatement1();

        // <STATEMENT> -> id <STATEMENT_REST> ;
        case STATEMENT_2:
            return LLparser_parseRuleStatement2();

        // <STATEMENT> -> _ = <THROW_AWAY> ;
        case STATEMENT_3:
            return LLparser_parseRuleStatement3();

        // <STATEMENT> -> <IF>
        case STATEMENT_4:
            return LLparser_parseRuleStatement4();

        // <STATEMENT> -> <WHILE>
        case STATEMENT_5:
            return LLparser_parseRuleStatement5();

        // <STATEMENT> -> return [precedence_expression] ;
        case STATEMENT_6:
            return LLparser_parseRuleStatement6();

        // <STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;
        case STATEMENT_7:
            return LLparser_parseRuleStatement7();

        // Jinak došlo k syntaktické chybě
        default:
            Parser_freeCurrentTerminalValue();
            return PARSING_ERROR;
    }
} // LLparser_parseStatement()


// <VAR_DEF> -> <MODIFIABLE> id <POSSIBLE_TYPE> = [precedence_expression]
AST_StatementNode *LLparser_parseVarDef() {
    // Parsujeme <MODIFIABLE>
    bool isConstant = false;
    LLparser_parseModifiable(&isConstant);

    // Kontrolujeme úspěch parsování <MODIFIABLE>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_0;
    }

    // Parsujeme "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseVarDef_errorLevel_0;
    }

    // Zkontrolujeme, že se nejedná o zastínění (sémantická chyba 5)
    frame_stack_result shadowing = frameStack_findItem(currentTerminal.value, NULL);
    if(shadowing != FRAME_STACK_ITEM_DOESNT_EXIST) {
        if (shadowing == FRAME_STACK_SUCCESS) {
            Parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseVarDef_errorLevel_0;
    }

    // Uchovávme hodnotu identifikátoru
    DString *varName = currentTerminal.value;

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme <POSSIBLE_TYPE>
    AST_DataType dataType = LLparser_parsePossibleType();

    // Kontrolujeme úspěch parsování <POSSIBLE_TYPE>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_1;
    }

    // Parsujeme '='
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        goto parseVarDef_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme [precedence_expression]
    AST_ExprNode *rightOperand = PrecParser_parse(NT_VAR_DEF);

    // Kontrolujeme úspěch parsování
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_1;
    }

    // Přidáme proměnnou do tabulky symbolů
    SymtableItem *varItem = NULL;
    symtable_symbolState state = SYMTABLE_SYMBOL_UNKNOWN;
    Parser_mapASTDataTypeToSymtableState(dataType, &state);

    frame_stack_result result = frameStack_addItemExpress(varName, state, isConstant, NULL, &varItem);

    if(result != FRAME_STACK_SUCCESS) {
        if(result == FRAME_STACK_ITEM_ALREADY_EXISTS) {
            Parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseVarDef_errorLevel_2;
    }

    // Vytvoříme uzel pro proměnnou
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_2;
    }

    // Vytvoříme uzel binární operace pro přiřazení "="
    AST_BinOpNode *assignOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_3;
    }

    // Vytvoříme uzel výrazu pro cíl přiřazení
    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_4;
    }

    // Inicializujeme uzel pro proměnnou
    AST_initNewVarNode(varNode, AST_VAR_NODE, varName, frameStack.top->frameID, \
                       AST_LITERAL_NOT_DEFINED, NULL);

    //Inicializujeme uzel pro levý operand
    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, varNode);

    // Inicializujeme uzel pro operaci přiřazení
    AST_initNewBinOpNode(assignOpNode, AST_OP_ASSIGNMENT, leftOperand, rightOperand);

    // Vytvoříme uzel výrazu pro přiřazení  "="
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_BIN_OP_NODE, assignOpNode);
        goto parseVarDef_errorLevel_0;
    }

    // Inicializujeme uzel pro výraz s přiřazením
    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignOpNode);

    // Vytvoříme uzel příkazu pro definici proměnné
    AST_StatementNode *varDefNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, assignExprNode);
        goto parseVarDef_errorLevel_0;
    }

    // Inicializujeme uzel pro příkaz výrazem s přiřazením
    AST_initNewStatementNode(varDefNode, frameStack.top->frameID, \
                             AST_STATEMENT_VAR_DEF, assignExprNode);

    // Vracíme uzel příkazu definice proměnné
    return varDefNode;



    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseVarDef_errorLevel_4:
        AST_destroyNode(AST_EXPR_NODE, leftOperand);

    parseVarDef_errorLevel_3:
        AST_destroyNode(AST_VAR_NODE, varNode);

    parseVarDef_errorLevel_2:
        AST_destroyNode(AST_EXPR_NODE, rightOperand);

    parseVarDef_errorLevel_1:
        if(varName != NULL && varName != currentTerminal.value) {
            string_free(varName);
            varName = NULL;
        }
    parseVarDef_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseVarDef

// <MODIFIABLE> -> var | const
void LLparser_parseModifiable(bool *isConstant) {
    if(isConstant == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_MODIFIABLE, &rule);

    // Na <MODIFIABLE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <MODIFIABLE> -> var
        case MODIFIABLE_1:
            // Parsujeme "var"
            if(LLparser_isNotExpectedTerminal(T_VAR)) {
                goto parseModifiable_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme false, jelikož "var" je modifikovatelná
            *isConstant = false;
            return;

        // <MODIFIABLE> -> const
        case MODIFIABLE_2:
            // Parsujeme "const"
            if(LLparser_isNotExpectedTerminal(T_CONST)) {
                goto parseModifiable_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme true, jelikož "const" není modifikovatelná
            *isConstant = true;
            return;

        // Jinak došlo k syntaktické chybě
        default:
            goto parseModifiable_errorHandle;
    }


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseModifiable_errorHandle:
        Parser_freeCurrentTerminalValue();
        return;
} // LLparser_parseModifiable()

// <POSSIBLE_TYPE> -> : <DATA_TYPE> | ε
AST_DataType LLparser_parsePossibleType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_POSSIBLE_TYPE, &rule);

    // Na <POSSIBLE_TYPE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <POSSIBLE_TYPE> -> : <DATA_TYPE>
        case POSSIBLE_TYPE_1:
            // Parsujeme ":"
            if(LLparser_isNotExpectedTerminal(T_COLON)) {
                goto parsePossibleType_errorHandle;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Parsujeme a vracíme datový typ proměnné
            return LLparser_parseDataType();

        // <POSSIBLE_TYPE> -> ε
        case POSSIBLE_TYPE_2:
            // Datový typ není uveden a tedy vracíme nedefinovaný datový typ
            return AST_DATA_TYPE_NOT_DEFINED;

        // Jinak došlo k syntaktické chybě
        default:
            goto parsePossibleType_errorHandle;
    }

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parsePossibleType_errorHandle:
        Parser_freeCurrentTerminalValue();
        return AST_DATA_TYPE_NOT_DEFINED;
} // LLparser_parsePossibleType()

// <STATEMENT_REST> -> = [precedence_expression] | ( <ARGUMENTS> )
AST_StatementNode *LLparser_parseStatementRest(DString **identifier) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_STATEMENT_REST, &rule);

    // Na <STATEMENT_REST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <STATEMENT_REST> -> = [precedence_expression]
        case STATEMENT_REST_1:
            return LLparser_parseRuleStatementRest1(identifier);

        // <STATEMENT_REST> -> ( <ARGUMENTS> )
        case STATEMENT_REST_2:
            return LLparser_parseRuleStatementRest2(identifier);

        // Jinak došlo k syntaktické chybě
        default:
            if(*identifier != NULL && *identifier != currentTerminal.value) {
                string_free(*identifier);
                *identifier = NULL;
            }
            Parser_freeCurrentTerminalValue();
            return PARSING_ERROR;
    }
} // LLparser_parseStatementRest

AST_ExprNode *LLparser_parseThrowAway() {
    return PrecParser_parse(NT_THROW_AWAY);
}

AST_IfNode *LLparser_parseIf() {
    // Parsujeme "if"
    if (LLparser_isNotExpectedTerminal(T_IF)) {
        goto parseIf_errorLevel_0;
    }
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "("
    if (LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseIf_errorLevel_0;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytváříme uzel výrazu a parsujeme [precedence_expression] (čili podmínku if)
    AST_ExprNode *condition = PrecParser_parse(NT_IF);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseIf_errorLevel_0;
    }

    // Parsujeme ")"
    if (LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        goto parseIf_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme <NULL_COND>
    AST_VarNode *nullCond = LLparser_parseNullCond();
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseIf_errorLevel_1;
    }

    // Vytváříme uzel pro seznam příkazů a parsujeme <SEQUENCE>
    AST_StatementNode *thenBranch = LLparser_parseSequence(nullCond == NULL);
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseIf_errorLevel_2;
    }

    // Popneme rámec pro "if" blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        goto parseIf_errorLevel_2;
    }

    // Parsujeme "else"
    if (LLparser_isNotExpectedTerminal(T_ELSE)) {
        goto parseIf_errorLevel_3;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytváříme uzel pro seznam příkazů a parsujeme <SEQUENCE>
    AST_StatementNode *elseBranch = LLparser_parseSequence(true);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseIf_errorLevel_4;
    }

    // Popneme rámec pro "else" blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        goto parseIf_errorLevel_4;
    }

    // Vytvoříme uzel pro "if" příkaz
    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseIf_errorLevel_4;
    }

    // Inicializace uzlu pro příkaz konstrukce "if"
    AST_initNewIfNode(ifNode, condition, nullCond, thenBranch, elseBranch);

    // Vracíme uzel pro konstrukci "if"
    return ifNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseIf_errorLevel_4:
        AST_destroyStatementList(elseBranch);

    parseIf_errorLevel_3:
        AST_destroyStatementList(thenBranch);

    parseIf_errorLevel_2:
        AST_destroyNode(AST_VAR_NODE, nullCond);

    parseIf_errorLevel_1:
        AST_destroyNode(AST_EXPR_NODE, condition);

    parseIf_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseIf()

// <NT_NULL_COND> -> | id |   |   ε
AST_VarNode *LLparser_parseNullCond() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_NULL_COND, &rule);

    // Deklrace pomocné proměnné pro uchování identifikátoru id_bez_null
    DString *identifier = NULL;

    // Na <POSSIBLE_TYPE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <NT_NULL_COND> -> | id |
        case NULL_COND_1:
            // Parsujeme "|"
            if (LLparser_isNotExpectedTerminal(T_PIPE)) {
                goto parseNullCond_errorLevel_0;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Parsujeme "id"
            if(LLparser_isNotExpectedTerminal(T_ID)) {
                goto parseNullCond_errorLevel_0;
            }

            // Uchovávme hodnotu identifikátoru
            identifier = currentTerminal.value;

            // Vytvoříme rámce pro "if" větev
            frameStack_push(false);

            // Vložíme do rámce pro "if" větev "id_bez_null"
            frame_stack_result addResult = \
            frameStack_addItemExpress(identifier, SYMTABLE_SYMBOL_UNKNOWN, IS_CONST, NULL, NULL);

            // Kontrola úspěšného vložení do tabulky symbolů
            if(addResult != FRAME_STACK_SUCCESS) {
                if(addResult == FRAME_STACK_ITEM_ALREADY_EXISTS) {
                    Parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
                }
                else {
                    Parser_errorWatcher(SET_ERROR_INTERNAL);
                }
                goto parseNullCond_errorLevel_1;
            }

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Parsujeme "|"
            if (LLparser_isNotExpectedTerminal(T_PIPE)) {
                goto parseNullCond_errorLevel_1;
            }

            // Vytvoříme uzel příkazu
            AST_VarNode *nullCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
            if(nullCond == NULL) {
                goto parseNullCond_errorLevel_1;
            }

            // Inicializujeme uzel pro příkaz voláním funkce
            AST_initNewVarNode(nullCond, AST_VAR_NODE, identifier, frameStack.top->frameID, \
                               AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

            // Žádáme o další token
            Parser_getNextToken(POKE_SCANNER);

            // Vracíme uzel pro NULL podmínku
            return nullCond;

        // <NT_NULL_COND> -> ε
        case NULL_COND_2:
            // NULL podmínka není uvedena a tedy vracíme NULL (korektní)
            return NULL;

        // Jinak došlo k syntaktické chybě
        default:
            goto parseNullCond_errorLevel_0;
    }

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    parseNullCond_errorLevel_1:
        if(identifier != NULL && identifier != currentTerminal.value) {
            string_free(identifier);
            identifier = NULL;
        }
    parseNullCond_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseNullCond()

// <SEQUENCE> -> { <STATEMENT_LIST> }
AST_StatementNode *LLparser_parseSequence(bool createFrame) {
    // Parsujeme "{"
    if(LLparser_isNotExpectedTerminal(T_LEFT_CURLY_BRACKET)) {
        goto parseSequence_errorLevel_0;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Pushneme nový rámec na zásobník rámců
    if(createFrame) {
        frameStack_push(false);
    }

    // Vytvoříme si uzel reprezentující seznam příkazů a parsujeme <STATEMENT_LIST>
    AST_StatementNode *statementList = LLparser_parseStatementList();

    // Zkontrolujeme úspěšnost parsování <STATEMENT_LIST>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseSequence_errorLevel_1;
    }

    // Parsujeme "}"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_CURLY_BRACKET)) {
        goto parseSequence_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vracíme sekvenci příkazů
    return statementList;



    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseSequence_errorLevel_1:
        AST_destroyStatementList(statementList);

    parseSequence_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseSequence()

AST_WhileNode *LLparser_parseWhile() {
    // Parsujeme "while"
    if (LLparser_isNotExpectedTerminal(T_WHILE)) {
        goto parseWhile_errorLevel_0;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "("
    if (LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseWhile_errorLevel_0;
    }

    // Žádáme od další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme [precedence_expression]
    AST_ExprNode *condition = PrecParser_parse(NT_WHILE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseWhile_errorLevel_0;
    }

    // Parsujeme ")"
    if (LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        goto parseWhile_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme <NULL_COND>
    AST_VarNode *nullCond = LLparser_parseNullCond();
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseWhile_errorLevel_1;
    }

    // Parsujeme <SEQUENCE>
    AST_StatementNode *body = LLparser_parseSequence(nullCond == NULL);
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseWhile_errorLevel_2;
    }

    // Popneme rámec pro 'while' blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        goto parseWhile_errorLevel_3;
    }

    // Vytvoříme uzel pro 'while' příkaz
    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    if (whileNode == NULL) {
        goto parseWhile_errorLevel_3;
    }

    // Inicializujeme uzel pro cyklus while
    AST_initNewWhileNode(whileNode, condition, nullCond, body);

    // Vracíme uzel pro cyklus while
    return whileNode;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseWhile_errorLevel_3:
        AST_destroyStatementList(body);

    parseWhile_errorLevel_2:
        AST_destroyNode(AST_VAR_NODE, nullCond);

    parseWhile_errorLevel_1:
        AST_destroyNode(AST_EXPR_NODE, condition);

    parseWhile_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseWhile()

// <ARGUMENTS> -> [precedence_expression]
AST_ArgOrParamNode *LLparser_parseArguments() {
    AST_ArgOrParamNode *argList = NULL;
    AST_ArgOrParamNode *currentArg = NULL;

    AST_ExprNode *arg = NULL;
    do {
        arg = PrecParser_parse(NT_ARGUMENTS);
        if(Parser_errorWatcher(IS_PARSING_ERROR)) {
            goto parseArguments_errorHandle;
        }

        if (arg != NULL) {
            // Vytvoření nového uzlu pro argument/parametr
            AST_ArgOrParamNode *newArg = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
            if(Parser_errorWatcher(IS_PARSING_ERROR)) {
                goto parseArguments_errorHandle;
            }

            // Inicializace nového uzlu
            AST_initNewArgOrParamNode(newArg, AST_DATA_TYPE_NOT_DEFINED, arg);

            // Připojení nového uzlu do seznamu
            if (argList == NULL) {
                argList = newArg;
            } else {
                currentArg->next = newArg;
            }
            currentArg = newArg;
        }

        // Žádáme o další token
        if(currentTerminal.LLterminal == T_COMMA) {
            Parser_getNextToken(POKE_SCANNER);
        }

    } while(currentTerminal.LLterminal != T_RIGHT_BRACKET);

    return argList;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseArguments_errorHandle:
        // Uvolnění již alokovaných uzlů v případě chyby
        AST_destroyArgOrParamList(argList);

        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseArguments()


/*******************************************************************************
 *                                                                             *
 *        IMPLEMENTACE INTERNÍCH FUNKCÍ PRO APLIKACI NĚKTERÝCH PRAVIDEL        *
 *                                                                             *
 ******************************************************************************/

// <STATEMENT> -> <VAR_DEF> ;
AST_StatementNode *LLparser_parseRuleStatement1() {
    // Vytvoříme uzel pro příkaz a parsujeme <VAR_DEF>
    AST_StatementNode *varDef = LLparser_parseVarDef();

    // Kontrola úspěšnosti parsování <VAR_DEF>
    if (varDef == NULL) {
        AST_destroyStatementList(varDef);
        goto parseRuleStatement1_handleError;
    }

    // Parsujeme ";"
    if (LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        AST_destroyNode(AST_STATEMENT_NODE, varDef);
        goto parseRuleStatement1_handleError;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vracíme příkaz typu definice funkce
    return varDef;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement1_handleError:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
}  // LLparser_parseRuleStatement1()

// <STATEMENT> -> id <STATEMENT_REST> ;
AST_StatementNode *LLparser_parseRuleStatement2() {
    // Parsujeme "id"
    if (LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseRuleStatement2_errorLevel_0;
    }

    // Zapamatujeme si identifikátor proměnné
    DString *identifier = currentTerminal.value;

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytvoříme uzel pro tento výraz a parsujeme <STATEMENT_REST>
    AST_StatementNode *statementRest = LLparser_parseStatementRest(&identifier);

    // Kontrola úspěcho parsování <STATEMENT_REST>
    // string je v případě chyby uvolněn v LLparser_parseStatementRest
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement2_errorLevel_1;
    }

    // Parsujeme ";"
    if (LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement2_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vracíme kompletní příkaz (id + zbytek)
    return statementRest;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement2_errorLevel_1:
        AST_destroyStatementList(statementRest);
    parseRuleStatement2_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement2()

// <STATEMENT> -> _ = <THROW_AWAY> ;
AST_StatementNode *LLparser_parseRuleStatement3() {
    // Parsujeme "_"
    if (LLparser_isNotExpectedTerminal(T_DUMP)) {
        goto parseRuleStatement3_errorLevel_0;
    }

    // Vytvoříme uzel pro proměnnou
    AST_VarNode *dumpVar = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_0;
    }

    // Vytvoříme identifikátor pro pseudoproměnnou "_"
    DString *underscore = string_charToDString("_");

    // Inicializujeme uzel pro proměnnou
    AST_initNewVarNode(dumpVar, AST_VAR_NODE, underscore, frameStack.top->frameID, \
                       AST_LITERAL_NOT_DEFINED, NULL);

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "="
    if (LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        goto parseRuleStatement3_errorLevel_1;
    }

    // Vytvoříme uzel výrazu pro cíl přiřazení
    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_1;
    }

    // Inicializujeme uzel pro levý operand
    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, dumpVar);

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytváříme uzel pro výraz a parsujeme <THROW_AWAY>
    AST_ExprNode *rightOperand = LLparser_parseThrowAway();
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_2;
    }

    // Vytvoříme uzel binární operace pro přiřazení "="
    AST_BinOpNode *assignBinOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_3;
    }

    // Inicializujeme uzel pro operaci přiřazení
    AST_initNewBinOpNode(assignBinOpNode, AST_OP_ASSIGNMENT, leftOperand, rightOperand);

    // Vložíme uzel pro výraz přířazení
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_4;
    }

    // Inicializujeme uzel pro operaci přiřazení
    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignBinOpNode);

    // Vytvoříme uzel příkazu pro definici proměnné
    AST_StatementNode *dumpStatement = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_4;
    }

    // Inicializujeme uzel pro příkaz výrazem s přiřazením
    AST_initNewStatementNode(dumpStatement, frameStack.top->frameID, \
                             AST_STATEMENT_EXPR, assignExprNode);

    // Parsujeme ";"
    if (LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement3_errorLevel_5;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vracíme příkaz pro zahození výsledku
    return dumpStatement;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement3_errorLevel_5:
        AST_destroyStatementList(dumpStatement);
        goto parseRuleStatement3_errorLevel_0;

    parseRuleStatement3_errorLevel_4:
        AST_destroyNode(AST_BIN_OP_NODE, assignBinOpNode);
        goto parseRuleStatement3_errorLevel_0;

    parseRuleStatement3_errorLevel_3:
        AST_destroyNode(AST_EXPR_NODE, rightOperand);
    parseRuleStatement3_errorLevel_2:
        AST_destroyNode(AST_EXPR_NODE, leftOperand);
        goto parseRuleStatement3_errorLevel_0;

    parseRuleStatement3_errorLevel_1:
        AST_destroyNode(AST_VAR_NODE, dumpVar);
    parseRuleStatement3_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement3()

// <STATEMENT> -> <IF>
AST_StatementNode *LLparser_parseRuleStatement4() {
    // Vytvoříme uzel pro podmíněný příkaz if() a parsujeme <IF>
    AST_IfNode *ifNode = LLparser_parseIf();

    // Kontrolujeme úspěšnost parsování <IF>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement4_errorHandle;
    }

    // Vytvoříme uzel pro příkaz a zkontrolujeme úspěšnost alokace
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_IF_NODE, ifNode);
        goto parseRuleStatement4_errorHandle;
    }

    // Inicializujeme uzel pro příkaz
    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_IF, ifNode);

    // Vracíme příkaz obsahující podmíněný příkaz if()
    return statementNode;



    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement4_errorHandle:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement4()

// <STATEMENT> -> <WHILE>
AST_StatementNode *LLparser_parseRuleStatement5() {
    // Vytvoříme uzel pro cyklus while() a parsujeme <WHILE>
    AST_WhileNode *whileNode = LLparser_parseWhile();

    // Kontrolujeme úspěšnost parsování <WHILE>
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement5_errorLevel_1;
    }

    // Vytvoříme uzel pro příkaz a zkontrolujeme úspěšnost alokace
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement5_errorLevel_2;
    }

    // Inicializujeme uzel pro příkaz
    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_WHILE, whileNode);

    // Vracíme příkaz obsahující cyklus while()
    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement5_errorLevel_2:
        AST_destroyStatementList(statementNode);

    parseRuleStatement5_errorLevel_1:
        AST_destroyNode(AST_WHILE_NODE, whileNode);

    // parseRuleStatement5_errorLevel_0
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement5()

// <STATEMENT> -> return [precedence_expression] ;
AST_StatementNode *LLparser_parseRuleStatement6() {
    // Parsujeme "return"
    if (LLparser_isNotExpectedTerminal(T_RETURN)) {
        goto parseRuleStatement6_errorLevel_0;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytvoříme uzel pro příkaz a parsujeme [precedence_expression]
    AST_ExprNode *returnExpr = PrecParser_parse(NT_STATEMENT);
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement6_errorLevel_0;
    }

    // Parsujeme ";"
    if (LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement6_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytvoříme uzel pro příkaz a zkontrolujeme úspěšnost alokace
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement6_errorLevel_1;
    }

    // Inicializujeme uzel pro příkaz
    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_RETURN, returnExpr);

    // Vracíme návratový příkaz
    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement6_errorLevel_1:
        AST_destroyNode(AST_EXPR_NODE, returnExpr);
    parseRuleStatement6_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement6()

// <STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;
AST_StatementNode *LLparser_parseRuleStatement7() {
    // Parsujeme "ifj"
    if (LLparser_isNotExpectedTerminal(T_IFJ)) {
        goto parseRuleStatement7_errorLevel_0;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "."
    if (LLparser_isNotExpectedTerminal(T_DOT)) {
        goto parseRuleStatement7_errorLevel_0;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "id"
    if (LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseRuleStatement7_errorLevel_0;
    }

    // Uchováme identifikátor
    DString *identifier = currentTerminal.value;

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme "("
    if (LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseRuleStatement7_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme <ARGUMENTS>
    AST_ArgOrParamNode *arguments = LLparser_parseArguments();
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement7_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme ";"
    if (LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement7_errorLevel_2;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Vytvoříme uzal pro volání funkce a zkontrolujeme úspěch alokace paměti
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement7_errorLevel_2;
    }

    // Inicializujeme uzel pro volání funkce
    AST_initNewFunCallNode(funCallNode, identifier, true, arguments);

    // Vytvoříme uzel pro příkaz a zkontrolujeme úspěch alokace paměti
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if (statementNode == NULL) {
        AST_destroyStatementList(statementNode);
        goto parseRuleStatement7_errorLevel_0;
    }

    // Inicializujeme uzel pro příkaz
    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_FUN_CALL, funCallNode);

    // Vracíme příkaz pro volání vestavěné funkce
    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatement7_errorLevel_2:
        AST_destroyArgOrParamList(arguments);
    parseRuleStatement7_errorLevel_1:
        if(identifier != NULL && identifier != currentTerminal.value) {
            string_free(identifier);
            identifier = NULL;
        }
    parseRuleStatement7_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement7()

// <STATEMENT_REST> -> = [precedence_expression]
AST_StatementNode *LLparser_parseRuleStatementRest1(DString **identifier) {
    // Parsujeme "="
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        goto parseRuleStatementRest1_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

    // Parsujeme [precedence_expression]
    AST_ExprNode *expr = PrecParser_parse(NT_STATEMENT_REST);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_1;
    }

    // Vyhledáme proměnnou v tabulce symbolů (již by měla být obsažena)
    SymtableItem *varItem = NULL;
    frame_stack_result result = frameStack_findItem(*identifier, &varItem);

    if(result != FRAME_STACK_SUCCESS) {
        if(result == FRAME_STACK_ITEM_DOESNT_EXIST) {
            Parser_errorWatcher(SET_ERROR_SEM_UNDEF);
        }
        else {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseRuleStatementRest1_errorLevel_1;
    }

    // Pokud obsažena byla, nastavíme, že byla změněna a použita
    varItem->changed = true;
    varItem->used = true;

    // Vytvoříme uzel pro proměnnou
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_2;
    }

    // Vytvoříme uzel operaci přiřazení
    AST_BinOpNode *assignOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_3;
    }

    // Vytvoříme uzel výrazu pro cíl přiřazení
    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_BIN_OP_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_3;
    }

    // Inicializujeme uzel pro proměnnou
    AST_initNewVarNode(varNode, AST_VAR_NODE, *identifier, frameStack_getId(*identifier), \
                       AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

    //Inicializujeme uzel pro levý operand
    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, varNode);

    AST_initNewBinOpNode(assignOpNode, AST_OP_ASSIGNMENT, leftOperand, expr);

    // Vytvoříme uzel výrazu pro přiřazení
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_BIN_OP_NODE, assignOpNode);
        goto parseRuleStatementRest1_errorLevel_0;
    }

    // Inicializujeme uzel pro výraz přiřazení
    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignOpNode);

    // Vytvoříme uzel příkazu
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, assignExprNode);
        goto parseRuleStatementRest1_errorLevel_0;
    }

    // Inicializujeme uzel pro příkaz
    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_EXPR, assignExprNode);

    // Vracíme uzel příkazu
    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatementRest1_errorLevel_3:
        AST_destroyNode(AST_VAR_NODE, varNode);

    parseRuleStatementRest1_errorLevel_2:
        AST_destroyNode(AST_EXPR_NODE, expr);

    parseRuleStatementRest1_errorLevel_1:
        if(*identifier != NULL && *identifier != currentTerminal.value) {
            string_free(*identifier);
            *identifier = NULL;
        }
    parseRuleStatementRest1_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatementRest1()

// <STATEMENT_REST> -> ( <ARGUMENTS> )
AST_StatementNode *LLparser_parseRuleStatementRest2(DString **identifier) {
    // Parsujeme "("
    if (LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseRuleStatementRest2_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);


    // Parsujeme <ARGUMENTS>
    AST_ArgOrParamNode *arguments = LLparser_parseArguments();
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest2_errorLevel_1;
    }

    // Žádáme o další token
    Parser_getNextToken(POKE_SCANNER);

   // Vytvoříme uzel pro volání funkce
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest2_errorLevel_1;
    }

    // Inicializujeme uzel pro volání funkce
    AST_initNewFunCallNode(funCallNode, *identifier, false, arguments);

    // Vytvoříme uzel příkazu
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest2_errorLevel_2;
    }

    // Inicializujeme uzel pro příkaz voláním funkce
    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_FUN_CALL, funCallNode);

    // Vracíme uzel příkazu
    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/
    parseRuleStatementRest2_errorLevel_2:
        AST_destroyNode(AST_FUN_CALL_NODE, funCallNode);
        goto parseRuleStatementRest2_errorLevel_0;

    parseRuleStatementRest2_errorLevel_1:
        if(*identifier != NULL && *identifier != currentTerminal.value) {
            string_free(*identifier);
            *identifier = NULL;
        }
    parseRuleStatementRest2_errorLevel_0:
        Parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatementRest2()


/*******************************************************************************
 *                                                                             *
 *             IMPLEMENTACE POMOCNÝCH INTERNÍCH FUNKCÍ LL PARSERU              *
 *                                                                             *
 ******************************************************************************/

void LLparser_initParserStructures() {
    // Pro jistotu vyresetujeme statické proměnné
    // (pozn. nevyresetování způsobuje problémy při opakovaném spouštění)
    Parser_getNextToken(RESET_LOOKAHEAD);
    Parser_errorWatcher(RESET_ERROR_FLAGS);

    // Alokujeme kořen AST a kontrolujeme úspěšnost alokace
    AST_initTree();
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        return;
    }

    // Alokujeme zásobník rámců (resp. globální rámec) a kontrolujeme úspěšnost alokace
    frameStack_init();
    if(frameStack.bottom == NULL || frameStack.top == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Vytváříme zásobník precedenčním zásobníků a kontrolujeme úspěšnost alokace
    PrecStackList_create();
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        return;
    }
}

inline bool LLparser_isNotExpectedTerminal(LLTerminals expectedTerminal) {
    // Pokud se typ aktualního teminálu neshoduje s očekávaným => syntaktická chyba
    if(currentTerminal.LLterminal != expectedTerminal) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
        return true;
    }
    // Pokud se shodují vracíme false (pozn. logika funkce je IS NOT)
    else {
        return false;
    }
}

/*** Konec souboru llparser.c ***/
