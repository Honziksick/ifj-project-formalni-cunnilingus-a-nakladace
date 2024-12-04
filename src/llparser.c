/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           llparser.c                                                *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            01.11.2024                                                *
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
 * @file llparser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace LL syntaktického analyzátoru.
 * @details Tento soubor obsahuje implementaci funkcí pro analýzu neterminálů
 *          podle LL gramatiky. Umožňuje tvorbu uzlů abstraktního syntaktického
 *          stromu (AST) a zajišťuje správnou strukturu kódu během analýzy.
 *          LL syntaktický analyzátor čte vstupní řetězec zleva doprava a
 *          používá jednu "lookahead" (dopředný) symbol pro rozhodování.
 */


// Import submodulů parseru
#include "llparser.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Spouští syntaktickou analýzu programu od neterminálu @c \<PROGRAM>.
 */
void LLparser_parseProgram() {
    // Alokujeme globální struktury a resetujeme statické proměnné
    LLparser_initParserStructures();

    // Inicializujeme "lookahead" (dopředný) terminál prvním voláním lexikálního an.
    parser_getNextToken(POKE_SCANNER);

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_PROGRAM, &rule);

    // <PROGRAM> -> <PROLOGUE> <FUN_DEF_LIST> EOF
    if(rule == PROGRAM) {
        // Analyzujeme neterminál <PROLOGUE>
        ASTroot->importedFile = LLparser_parsePrologue();

        if(parser_errorWatcher(IS_PARSING_ERROR)) {
            goto parseProgram_handleError;  // ošetření chyb na konci funkce
        }

        // Žádáme o další token
        parser_getNextToken(POKE_SCANNER);

        // Analyzujeme neterminál <FUN_DEF_LIST>
        AST_FunDefNode *funDefNode = LLparser_parseFunDefList();

        if(parser_errorWatcher(IS_PARSING_ERROR)) {
            goto parseProgram_handleError;  // ošetření chyb na konci funkce
        }

        // Pokud k chybě nedošlo, připojíme seznam definic funkcí ke kořeni AST
        ASTroot->functionList = funDefNode;

        // Očekávaným terminálem je "EOF"
        if(LLparser_isNotExpectedTerminal(T_EOF)) {
            goto parseProgram_handleError;  // ošetření chyb na konci funkce
        }
    }
    // Pokud došlo v rámci výběru pravidla k chybě, skáčeme na ošetření chyb
    else {
        goto parseProgram_handleError;  // ošetření chyb na konci funkce
    }

    // Program byl syntakticky správný a ukončujeme parsování
    parser_freeCurrentTerminalValue();
    return;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu; volání funkce error_handle()
    parseProgram_handleError:
        parser_freeCurrentTerminalValue();
        parser_errorWatcher(CALL_ERROR_HANDLE);
} // LLparser_parseProgram()


/*******************************************************************************
 *                                                                             *
 *           IMPLEMENTACE INTERNÍCH FUNKCÍ PRO PARSOVÁNÍ NETERMINÁLŮ           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Analyzuje neterminál @c \<PROLOGUE>, který reprezentuje úvod programu.
 */
AST_VarNode *LLparser_parsePrologue() {
    // Vytváříme vlajky pro řízení uvolňování paměti při chybách
    bool errorLevel_1 = false;
    bool errorLevel_2 = false;

    // Definujeme název importované proměnné (lexikální analyzátor ji
    // zpracovává jako keyword a předává bez řetězcové hodnoty)
    DString *importVar = DString_constCharToDString("ifj");

    // Definujeme název očekávané cesty k importovanému souboru (pevně daná)
    DString *path = DString_constCharToDString("ifj24.zig");

    // Kontrolujeme, že byla paměť řetězcům úspěšně přidělena
    if(importVar == NULL || path == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je "const"
    if(LLparser_isNotExpectedTerminal(T_CONST)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "ifj"
    if(LLparser_isNotExpectedTerminal(T_IFJ)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "="
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "@import"
    if(LLparser_isNotExpectedTerminal(T_IMPORT)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme `("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným obsah terminálu pro řetězcový literál "ifj24.zig"
    if(currentTerminal.PrecTerminal != T_PREC_STRING_LITERAL) {
        parser_errorWatcher(SET_ERROR_SYNTAX);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    if(DString_compareWithConstChar(currentTerminal.value, path->str) != STRING_EQUAL) {
        parser_errorWatcher(SET_ERROR_SYNTAX);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Uvolníme obsah aktuálního terminálu
    parser_freeCurrentTerminalValue();

    // Přidáme proměnnou prologu do tabulky symbolů a zkontrolujeme úspěšnost přidání
    FrameStack_result result = frameStack_addItemExpress(importVar, SYMTABLE_SYMBOL_VARIABLE_STRING, \
                                                          IS_CONST, path, NULL);

    if(result != FRAME_STACK_SUCCESS) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Vytvoříme AST uzel pro proměnnou s cestou k importovanému souboru
    AST_VarNode *importedFile = (AST_VarNode *)AST_createNode(AST_VAR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        errorLevel_1 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    AST_initNewVarNode(importedFile, AST_VAR_NODE, importVar, \
                       frameStack.top->frameID, AST_LITERAL_STRING, path);

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je ")"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        errorLevel_2 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je ";"
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        errorLevel_2 = true;
        goto parsePrologue_handleError;  // ošetření chyb na konci funkce
    }

    return importedFile;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parsePrologue_handleError:
        // Úroveň 1.1: řetězce identifátkoru proměnné "ifj" a cesty "ifj24.zig"
        if(errorLevel_1) {
            if(importVar != NULL && importVar != currentTerminal.value) {
                DString_free(importVar);
                importVar = NULL;
            }

            if(path != NULL && path != currentTerminal.value) {
                DString_free(path);
                path = NULL;
            }
        }
        // Úroveň 1.2: uzel proměnné (obsahuje výše uvedené řetězce)
        else if(errorLevel_2) {
            AST_destroyNode(AST_VAR_NODE, importedFile);
        }

        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parsePrologue()

/**
 * @brief Analyzuje neterminál @c \<FUN_DEF_LIST>, který reprezentuje seznam
 *        definic funkcí.
 */
AST_FunDefNode *LLparser_parseFunDefList() {
    // Inicializujeme proměnné pro zdroje funkce
    AST_FunDefNode *funDef = NULL;
    AST_FunDefNode *funDefList = NULL;

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_FUN_DEF_LIST, &rule);

    // Na neterminál <FUN_DEF_LIST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST>
        case FUN_DEF_LIST_1: {
            // Analyzujeme neterminál <FUN_DEF>
            funDef = LLparser_parseFunDef();

            if(parser_errorWatcher(IS_PARSING_ERROR)) {
                goto parseFunDefList_errorLevel_0;  // ošetření chyb na konci funkce
            }

            // Analyzujeme zbytek seznamu <FUN_DEF_LIST>
            funDefList = LLparser_parseFunDefList();

            if(parser_errorWatcher(IS_PARSING_ERROR)) {
                goto parseFunDefList_errorLevel_1;  // ošetření chyb na konci funkce
            }

            // Připojíme aktuální definici funkce do seznamu definic funkcí
            funDef->next = funDefList;

            return funDef;
        }

        // <FUN_DEF_LIST> -> ε
        case FUN_DEF_LIST_2:
            return NULL;  // ε reprezentujeme jako NULL

        // Výchozí stav: došlo k chybě
        default:
            goto parseFunDefList_errorLevel_0;  // ošetření chyb na konci funkce
    } // switch()


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: uzel pro definici funkce
    parseFunDefList_errorLevel_1:
        AST_destroyFunDefNode(funDef);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseFunDefList_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseFunDefList()

/**
 * @brief Analyzuje neterminál @c \<FUN_DEF>, který reprezentuje definici jedné
 *        funkce.
 */
AST_FunDefNode *LLparser_parseFunDef() {
    // Očekávaným terminálem je "pub"
    if(LLparser_isNotExpectedTerminal(T_PUB)) {
        goto parseFunDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "fn"
    if(LLparser_isNotExpectedTerminal(T_FN)) {
        goto parseFunDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseFunDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Uchováme identifikátor funkce
    DString *functionName = currentTerminal.value;

    // Přidáme definici funkce do tabulky symbolů
    SymtableItem *functionItem = NULL;
    FrameStack_result result = frameStack_addItemExpress(functionName, SYMTABLE_SYMBOL_FUNCTION, \
                                                          IS_VAR, NULL, &functionItem);

    if(result != FRAME_STACK_SUCCESS) {
        // Pokud funkce s daným identifikátorem již existuje => sémantická chyba 5
        if(result == FRAME_STACK_ITEM_ALREADY_EXISTS) {
            parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }

        goto parseFunDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseFunDef_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Pushneme nový rámce příslušící analyzované funkci na zásobník rámců
    frameStack_push(IS_FUNCTION);

    // Uložíme ID rámce těla funkce pro tabulku symbolů
    size_t function_frameID = frameStack.top->frameID;

    // Počítadlo parametrů funkce
    size_t paramCount = 0;

    // Analyzujeme neterminál <PARAMETERS>
    AST_ArgOrParamNode *parameters = LLparser_parseParameters(&paramCount);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je ")"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        goto parseFunDef_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <RETURN_TYPE>
    AST_DataType returnType = LLparser_parseReturnType();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Analyzujeme neterminál <SEQUENCE>
    AST_StatementNode *sequence = LLparser_parseSequence(false);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_3;  // ošetření chyb na konci funkce
    }

    // Vytvoříme AST uzel pro definici funkce
    AST_FunDefNode *funDefNode = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseFunDef_errorLevel_3;  // ošetření chyb na konci funkce
    }

    AST_initNewFunDefNode(funDefNode, functionName, parameters, returnType, sequence);

    // Vytvoříme strukturu SymtableFunctionData pro uložení parametrů do tabulky symbolů
    SymtableFunctionData *functionData = symtable_initFunctionData(paramCount);

    if(functionData == NULL) {
        goto parseFunDef_errorLevel_4;  // ošetření chyb na konci funkce
    }

    // Naplníme strukturu SymtableFunctionData parametry
    AST_ArgOrParamNode *paramNode = parameters;
    Symtable_functionReturnType funType = SYMTABLE_TYPE_UNKNOWN;

    for(size_t i = 0; i < paramCount; i++) {
        // Jediným typem povoleného výraz je výraz reprezentující proměnnou
        if(paramNode->expression->exprType != AST_EXPR_VARIABLE) {
            goto parseFunDef_errorLevel_4;  // ošetření chyb na konci funkce
        }

        AST_VarNode *variable = (AST_VarNode *)(paramNode->expression->expression);

        // Inicializace pole s parametry analyzované funkci
        parser_mapASTDataTypeToFunReturnType(paramNode->dataType, &funType);
        functionData->params[i].id = variable->identifier;
        functionData->params[i].type = funType;

        paramNode = paramNode->next;
    }

    // Zaznamenání obecných informací o funkci do tabulky symbolů
    parser_mapASTDataTypeToFunReturnType(returnType, &funType);
    functionData->bodyFrameID = function_frameID;
    functionData->returnType = funType;

    // Přidáme data funkce do položky v tabulce symbolů
    functionItem->data = functionData;

    // Popneme rámce analyzované funkce (dokončení její analýzy)
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    return funDefNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 4: celkový uzel s definicí funkce
    parseFunDef_errorLevel_4:
        AST_destroyNode(AST_FUN_DEF_NODE, funDefNode);

    // Úroveň 3: seznam uzlů s příkazy funkce
    parseFunDef_errorLevel_3:
        AST_destroyStatementList(sequence);

    // Úroveň 2: seznam uzlů pro paramaetry funkce; popnutí rámce funkce
    parseFunDef_errorLevel_2:
        AST_destroyArgOrParamList(parameters);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }

    // Úroveň 1: řetězec s identifikátorem funkce
    parseFunDef_errorLevel_1:
        if(functionName != NULL && functionName != currentTerminal.value) {
            DString_free(functionName);
            functionName = NULL;
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseFunDef_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseFunDef()

/**
 * @brief Analyzuje neterminál @c \<PARAMETERS>, který reprezentuje seznam
 *        parametrů funkce.
 */
AST_ArgOrParamNode *LLparser_parseParameters(size_t *paramCount) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_PARAMETERS, &rule);

    // Na neterminál <PARAMETERS> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <PARAMETERS> -> <PARAM_LIST>
        case PARAMETERS_1:
            return LLparser_parseParamList(paramCount);

        // <PARAMETERS> -> ε
        case PARAMETERS_2:
            *paramCount = 0;
            return NULL;  // ε reprezentujeme jako NULL

        // Výchozí stav: došlo k chybě
        default:
            parser_freeCurrentTerminalValue();
            return PARSING_ERROR;
    }
} // LLparser_parseParameters()

/**
 * @brief Analyzuje neterminál @c \<PARAM_LIST>, který reprezentuje neprázdný
 *        seznam parametrů.
 */
AST_ArgOrParamNode *LLparser_parseParamList(size_t *paramCount) {
    // Analyzujeme neterminál <PARAM>
    AST_ArgOrParamNode *paramList = LLparser_parseParam();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParamList_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Zvýšíme počet parametrů (byl analyzován nový parametr)
    (*paramCount)++;

    // Analyzujeme neterminál <PARAM_LIST_REST>
    AST_ArgOrParamNode *paramListRest = LLparser_parseParamListRest(paramCount);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParamList_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Připojíme do seznamu parametrů nový parametr
    if(paramListRest != NULL) {
        paramList->next = paramListRest;
    }

    return paramList;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: seznam uzlů pro parametry funkce
    parseParamList_errorLevel_1:
        AST_destroyArgOrParamList(paramList);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseParamList_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseParamList()

/**
 * @brief Analyzuje neterminál @c \<PARAM_LIST_REST>, který reprezentuje
 *        pokračování seznamu parametrů.
 */
AST_ArgOrParamNode *LLparser_parseParamListRest(size_t *paramCount) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_PARAM_LIST_REST, &rule);

    // Na neterminál <PARAM_LIST_REST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <PARAM_LIST_REST> -> , <PARAM_LIST>
        case PARAM_LIST_REST_1: {
            // Očekávaným terminálem je ","
            if(LLparser_isNotExpectedTerminal(T_COMMA)) {
                return PARSING_ERROR;
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            // Analyzujeme neterminál <PARAM_LIST>
            AST_ArgOrParamNode *paramList = LLparser_parseParamList(paramCount);

            if(parser_errorWatcher(IS_PARSING_ERROR)) {
                return PARSING_ERROR;
            }

            return paramList;
        }

        // <PARAM_LIST_REST> -> ε
        case PARAM_LIST_REST_2:
            return NULL;  // ε reprezentujeme jako NULL

        // Výchozí stav: došlo k syntaktické chybě
        default:
            return PARSING_ERROR;
    } // switch()
} // LLparser_parseParamListRest()

/**
 * @brief Analyzuje neterminál @c \<PARAM>, který reprezentuje jeden parametr
 *        funkce.
 */
AST_ArgOrParamNode *LLparser_parseParam() {
    // Očekávaným terminálem je "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseParam_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Identifikátorem parametru je hodnota aktuálního terminálu
    DString *paramId = currentTerminal.value;

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je ":"
    if(LLparser_isNotExpectedTerminal(T_COLON)) {
        goto parseParam_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál  "<DATA_TYPE>"
    AST_DataType dataType = LLparser_parseDataType();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParam_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Vytvoříme AST uzel pro parametr
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseParam_errorLevel_1;  // ošetření chyb na konci funkce
    }

    AST_initNewVarNode(varNode, AST_VAR_NODE, paramId, frameStack.top->frameID, \
                       AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

    // Mapujeme AST datový typ na Symtable SymbolState
    Symtable_symbolState state = SYMTABLE_SYMBOL_UNKNOWN;
    parser_mapASTDataTypeToSymtableState(dataType, &state);

    // Přidáme parametr do tabulky symbolů jako lokální proměnnou
    FrameStack_result result = frameStack_addItemExpress(paramId, state, \
                                                          IS_CONST, NULL, NULL);

    if(result != FRAME_STACK_SUCCESS) {
        if(result == FRAME_STACK_ITEM_ALREADY_EXISTS) {
            parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        AST_destroyNode(AST_VAR_NODE, varNode);
        goto parseParam_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel pro výraz (hodnotu) parametru
    AST_ExprNode *paramExpr = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_VAR_NODE, varNode);
        goto parseParam_errorLevel_0;  // ošetření chyb na konci funkce
    }

    AST_initNewExprNode(paramExpr, AST_EXPR_VARIABLE, varNode);

    // Vytvoříme a inicializujeme uzel pro parametr
    AST_ArgOrParamNode *paramNode = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, paramExpr);
        goto parseParam_errorLevel_0;  // ošetření chyb na konci funkce
    }

    AST_initNewArgOrParamNode(paramNode, dataType, paramExpr);

    return paramNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: řetězec s identifikátorem parametru funkce
    parseParam_errorLevel_1:
        if(paramId != NULL && paramId != currentTerminal.value) {
            DString_free(paramId);
            paramId = NULL;
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseParam_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseParam()

/**
 * @brief Analyzuje neterminál @c \<RETURN_TYPE>, který reprezentuje návratový
 *        typ funkce.
 */
AST_DataType LLparser_parseReturnType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_RETURN_TYPE, &rule);

    // Na neterminál <RETURN_TYPE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <RETURN_TYPE> -> <DATA_TYPE>
        case RETURN_TYPE_1:
            // Analyzujeme neterminál <DATA_TYPE>
            return LLparser_parseDataType();

        // <RETURN_TYPE> -> void
        case RETURN_TYPE_2:
            // Očekávaným terminálem je "void"
            if(LLparser_isNotExpectedTerminal(T_VOID)) {
                goto parseReturnType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_VOID;

        // Výchozí stav: došlo k chybě
        default:
            goto parseReturnType_errorHandle;  // ošetření chyb na konci funkce
    } // switch()


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseReturnType_errorHandle:
        parser_freeCurrentTerminalValue();
        return AST_DATA_TYPE_NOT_DEFINED;
} // LLparser_parseReturnType()

/**
 * @brief Analyzuje neterminál @c \<DATA_TYPE>, který reprezentuje podporované
 *        datové typy.
 */
AST_DataType LLparser_parseDataType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_DATA_TYPE, &rule);

    // Na neterminál <DATA_TYPE> lze aplikovat šest různých pravidel
    switch(rule) {
        // <DATA_TYPE> -> i32
        case DATA_TYPE_1:
            // Očekávaným terminálem je "i32"
            if(LLparser_isNotExpectedTerminal(T_INT)) {
                goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_INT;

        // <DATA_TYPE> -> ?i32
        case DATA_TYPE_2:
            // Očekávaným terminálem je "?i32"
            if(LLparser_isNotExpectedTerminal(T_INT_OR_NULL)) {
                goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_INT_OR_NULL;

        // <DATA_TYPE> -> f64
        case DATA_TYPE_3:
            // Očekávaným terminálem je "f64"
            if(LLparser_isNotExpectedTerminal(T_FLOAT)) {
                goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_FLOAT;

        // <DATA_TYPE> -> ?f64
        case DATA_TYPE_4:
            // Očekávaným terminálem je "?f64"
            if(LLparser_isNotExpectedTerminal(T_FLOAT_OR_NULL)) {
                goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_FLOAT_OR_NULL;

        // <DATA_TYPE> -> []u8
        case DATA_TYPE_5:
            // Očekávaným terminálem je "[]u8"
            if(LLparser_isNotExpectedTerminal(T_STRING)) {
                goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_STRING;

        // <DATA_TYPE> -> ?[]u8
        case DATA_TYPE_6:
            // Očekávaným terminálem je "?[]u8"
            if(LLparser_isNotExpectedTerminal(T_STRING_OR_NULL)) {
                goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return AST_DATA_TYPE_STRING_OR_NULL;

        // Výchozí stav: došlo k chybě
        default:
            goto parseDataType_errorHandle;  // ošetření chyb na konci funkce
    } // switch()


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseDataType_errorHandle:
        parser_freeCurrentTerminalValue();
        return AST_DATA_TYPE_NOT_DEFINED;
} // LLparser_parseDataType()

/**
 * @brief Analyzuje neterminál @c \<STATEMENT_LIST>, který reprezentuje
 *        seznam příkazů.
 */
AST_StatementNode *LLparser_parseStatementList() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_STATEMENT_LIST, &rule);

    // Na neterminál <STATEMENT_LIST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST>
        case STATEMENT_LIST_1: {
            // Analyzujeme neterminál <STATEMENT>
            AST_StatementNode *statement = LLparser_parseStatement();

            if(parser_errorWatcher(IS_PARSING_ERROR)) {
                AST_destroyStatementList(statement);
                goto parseStatementList_errorHandle;  // ošetření chyb na konci funkce
            }

            // Analyzujeme neterminál  <STATEMENT_LIST>
            AST_StatementNode *statementList = LLparser_parseStatementList();

            // Pokud není navrácený seznam příkazů prázdný, připojíme ho ke stávajícímmu
            if(statementList != NULL) {
                statement->next = statementList;
            }

            return statement;
        }

        // <STATEMENT_LIST> -> ε
        case STATEMENT_LIST_2:
            return NULL;  // ε reprezentujeme jako NULL

        // Výchozí stav: došlo k chybě
        default:
            goto parseStatementList_errorHandle;  // ošetření chyb na konci funkce
    }


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseStatementList_errorHandle:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLaprser_parseStatementList()

/**
 * @brief Analyzuje neterminál @c \<STATEMENT>, který reprezentuje jeden příkaz
 *        programu.
 */
AST_StatementNode *LLparser_parseStatement() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_STATEMENT, &rule);

    // Na neterminál <STATEMENT> lze aplikovat šest různých pravidel
    switch(rule) {
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

        // Výchozí stav: došlo k chybě
        default:
            parser_freeCurrentTerminalValue();
            return PARSING_ERROR;
    }
} // LLparser_parseStatement()

/**
 * @brief Analyzuje neterminál @c \<VAR_DEF>, který reprezentuje definici
 *        proměnné.
 */
AST_StatementNode *LLparser_parseVarDef() {
    // Analyzujeme neterminál <MODIFIABLE>
    bool isConstant = false;
    LLparser_parseModifiable(&isConstant);

    // Kontrolujeme úspěch parsování <MODIFIABLE>
    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseVarDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Zkontrolujeme, že se nejedná o zastínění (sémantická chyba 5)
    FrameStack_result shadowing = frameStack_findItem(currentTerminal.value, NULL);

    if(shadowing != FRAME_STACK_ITEM_DOESNT_EXIST) {
        if(shadowing == FRAME_STACK_SUCCESS) {
            parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseVarDef_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Uchovávme řetězcovou hodnotu identifikátoru
    DString *varName = currentTerminal.value;

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <POSSIBLE_TYPE>
    AST_DataType dataType = LLparser_parsePossibleType();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je "="
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        goto parseVarDef_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme výraz - předáváme řízení precedenčnímu synt. analyzátoru
    AST_ExprNode *rightOperand = precParser_parse(NT_VAR_DEF);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Přidáme proměnnou do tabulky symbolů
    SymtableItem *varItem = NULL;
    Symtable_symbolState state = SYMTABLE_SYMBOL_UNKNOWN;
    parser_mapASTDataTypeToSymtableState(dataType, &state);

    FrameStack_result result = frameStack_addItemExpress(varName, state, \
                                                          isConstant, NULL, &varItem);

    // Kontrolujeme, že nedošlo k redefinici proměnné (chyba 5)
    if(result != FRAME_STACK_SUCCESS) {
        if(result == FRAME_STACK_ITEM_ALREADY_EXISTS) {
            parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
        }
        else {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }

        goto parseVarDef_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Vytvoříme uzel pro proměnnou
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Vytvoříme uzel binární operace pro přiřazení "="
    AST_BinOpNode *assignOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_3;  // ošetření chyb na konci funkce
    }

    // Vytvoříme uzel výrazu pro cíl přiřazení
    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseVarDef_errorLevel_4;  // ošetření chyb na konci funkce
    }

    // Inicializujeme výše vytvořené uzly aST
    AST_initNewVarNode(varNode, AST_VAR_NODE, varName, frameStack.top->frameID, \
                       AST_LITERAL_NOT_DEFINED, NULL);

    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, varNode);

    AST_initNewBinOpNode(assignOpNode, AST_OP_ASSIGNMENT, leftOperand, rightOperand);

    // Vytvoříme a inicializujeme uzel výrazu přiřazení  "="
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_BIN_OP_NODE, assignOpNode);  // ošetření chyb na konci funkce
        goto parseVarDef_errorLevel_0;
    }

    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignOpNode);

    // Vytvoříme a inicializujeme uzel příkazu pro definici proměnné
    AST_StatementNode *varDefNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, assignExprNode);  // ošetření chyb na konci funkce
        goto parseVarDef_errorLevel_0;
    }

    AST_initNewStatementNode(varDefNode, frameStack.top->frameID, \
                             AST_STATEMENT_VAR_DEF, assignExprNode);


    return varDefNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 4: uzel pro levý operand výrazu přiřazení
    parseVarDef_errorLevel_4:
        AST_destroyNode(AST_EXPR_NODE, leftOperand);

    // Úroveň 3: uzel pro proměnnou
    parseVarDef_errorLevel_3:
        AST_destroyNode(AST_VAR_NODE, varNode);

    // Úroveň 2: uzel pro pravý operand výrazu přiřazení
    parseVarDef_errorLevel_2:
        AST_destroyNode(AST_EXPR_NODE, rightOperand);

    // Úroveň 1: řetězec s identifkátorem proměnné
    parseVarDef_errorLevel_1:
        if(varName != NULL && varName != currentTerminal.value) {
            DString_free(varName);
            varName = NULL;
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseVarDef_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseVarDef

/**
 * @brief Analyzuje neterminál @c \<MODIFIABLE>, který určuje modifikovatelnost
 *        proměnné.
 */
void LLparser_parseModifiable(bool *isConstant) {
    // Kontrolujeme platnost předaného ukazatele
    if(isConstant == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_MODIFIABLE, &rule);

    // Na neterminál <MODIFIABLE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <MODIFIABLE> -> var
        case MODIFIABLE_1:
            // Očekávaným terminálem je "var"
            if(LLparser_isNotExpectedTerminal(T_VAR)) {
                goto parseModifiable_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            // Vracíme přes ukazatel false, jelikož "var" je modifikovatelná
            *isConstant = false;
            return;

        // <MODIFIABLE> -> const
        case MODIFIABLE_2:
            // Očekávaným terminálem je "const"
            if(LLparser_isNotExpectedTerminal(T_CONST)) {
                goto parseModifiable_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            // Vracíme přes ukazatel true, jelikož "const" není modifikovatelná
            *isConstant = true;
            return;

        // Výchozí stav: došlo k chybě
        default:
            goto parseModifiable_errorHandle;  // ošetření chyb na konci funkce
    }


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseModifiable_errorHandle:
        parser_freeCurrentTerminalValue();
        return;
} // LLparser_parseModifiable()

/**
 * @brief Analyzuje neterminál @c \<POSSIBLE_TYPE>, který reprezentuje případný
 *        datový typ proměnné.
 */
AST_DataType LLparser_parsePossibleType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_POSSIBLE_TYPE, &rule);

    // Na neterminál <POSSIBLE_TYPE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <POSSIBLE_TYPE> -> : <DATA_TYPE>
        case POSSIBLE_TYPE_1:
            // Očekávaným terminálem je ":"
            if(LLparser_isNotExpectedTerminal(T_COLON)) {
                goto parsePossibleType_errorHandle;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            // Analyzujeme neterminál <DATA_TYPE>
            return LLparser_parseDataType();

        // <POSSIBLE_TYPE> -> ε
        case POSSIBLE_TYPE_2:
            // Datový typ není uveden a tedy vracíme nedefinovaný datový typ
            return AST_DATA_TYPE_NOT_DEFINED;

        // Výchozí stav: došlo k chybě
        default:
            goto parsePossibleType_errorHandle;  // ošetření chyb na konci funkce
    }


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parsePossibleType_errorHandle:
        parser_freeCurrentTerminalValue();
        return AST_DATA_TYPE_NOT_DEFINED;
} // LLparser_parsePossibleType()

/**
 * @brief Analyzuje neterminál @c \<STATEMENT_REST>, který reprezentuje zbytek
 *        příkazu po identifikátoru.
 */
AST_StatementNode *LLparser_parseStatementRest(DString **identifier) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_STATEMENT_REST, &rule);

    // Na neterminál <STATEMENT_REST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <STATEMENT_REST> -> = [precedence_expression]
        case STATEMENT_REST_1:
            return LLparser_parseRuleStatementRest1(identifier);

        // <STATEMENT_REST> -> ( <ARGUMENTS> )
        case STATEMENT_REST_2:
            return LLparser_parseRuleStatementRest2(identifier);

        // Výchozí stav: došlo k chybě
        default:
            // Uvolníme uchovaný indetifikátor
            if(*identifier != NULL && *identifier != currentTerminal.value) {
                DString_free(*identifier);
                *identifier = NULL;
            }

            // Uvolníme řetězcovou hodnotu aktuálního terminálu
            parser_freeCurrentTerminalValue();

            return PARSING_ERROR;
    }
} // LLparser_parseStatementRest

/**
 * @brief Analyzuje neterminál @c \<THROW_AWAY>, který reprezentuje výraz s
 *        výsledkem k zahození.
 */
AST_ExprNode *LLparser_parseThrowAway() {
    // Analyzujeme výraz - předáváme řízení precedenčnímu synt. analyzátoru
    return precParser_parse(NT_THROW_AWAY);
} // LLparser_parseThrowAway()

/**
 * @brief Analyzuje neterminál @c \<IF>, který reprezentuje podmíněný
 *        příkaz @c if.
 */
AST_IfNode *LLparser_parseIf() {
    // Očekávaným terminálem je "if"
    if(LLparser_isNotExpectedTerminal(T_IF)) {
        goto parseIf_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseIf_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme výraz (podmínku if) - předáváme řízení precedenčnímu synt. analyzátoru
    AST_ExprNode *condition = precParser_parse(NT_IF);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseIf_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je ")"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        goto parseIf_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <NULL_COND>
    AST_VarNode *nullCond = LLparser_parseNullCond();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseIf_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Analyzujeme neterminál <SEQUENCE>
    AST_StatementNode *thenBranch = LLparser_parseSequence(nullCond == NULL);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseIf_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Popneme rámec pro "if" blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        goto parseIf_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je "else"
    if(LLparser_isNotExpectedTerminal(T_ELSE)) {
        goto parseIf_errorLevel_3;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <SEQUENCE>
    AST_StatementNode *elseBranch = LLparser_parseSequence(true);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseIf_errorLevel_4;  // ošetření chyb na konci funkce
    }

    // Popneme rámec pro "else" blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        goto parseIf_errorLevel_4;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel pro "if" příkaz
    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseIf_errorLevel_4;  // ošetření chyb na konci funkce
    }

    AST_initNewIfNode(ifNode, condition, nullCond, thenBranch, elseBranch);

    // Vracíme uzel pro konstrukci "if"
    return ifNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 4: seznam uzlů příkazů větve "else"
    parseIf_errorLevel_4:
        AST_destroyStatementList(elseBranch);

    // Úroveň 3: seznam uzlů příkazů větve "then"
    parseIf_errorLevel_3:
        AST_destroyStatementList(thenBranch);

    // Úroveň 2: uzel volitelnou nullable podmínku
    parseIf_errorLevel_2:
        AST_destroyNode(AST_VAR_NODE, nullCond);

    // Úroveň 1: uzel pro výraz podmínky příkazu "if"
    parseIf_errorLevel_1:
        AST_destroyNode(AST_EXPR_NODE, condition);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseIf_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseIf()

/**
 * @brief Analyzuje neterminál @c \<NT_NULL_COND>, který reprezentuje případnou
 *        null podmínku.
 */
AST_VarNode *LLparser_parseNullCond() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    LLtable_findRule(currentTerminal.LLterminal, NT_NULL_COND, &rule);

    // Deklrace pomocné proměnné pro uchování identifikátoru "id_bez_null"
    DString *identifier = NULL;

    // Na neterminál <POSSIBLE_TYPE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <NT_NULL_COND> -> | id |
        case NULL_COND_1: {
            // Očekávaným terminálem je "|"
            if(LLparser_isNotExpectedTerminal(T_PIPE)) {
                goto parseNullCond_errorLevel_0;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            // Očekávaným terminálem je "id"
            if(LLparser_isNotExpectedTerminal(T_ID)) {
                goto parseNullCond_errorLevel_0;  // ošetření chyb na konci funkce
            }

            // Uchovávme hodnotu identifikátoru
            identifier = currentTerminal.value;

            // Vytvoříme rámce pro "if" větev
            frameStack_push(NOT_FUNCTION);

            // Vložíme do rámce pro "if" větev proměnou "id_bez_null"
            FrameStack_result addResult = \
            frameStack_addItemExpress(identifier, SYMTABLE_SYMBOL_UNKNOWN, \
                                      IS_CONST, NULL, NULL);

            if(addResult != FRAME_STACK_SUCCESS) {
                if(addResult == FRAME_STACK_ITEM_ALREADY_EXISTS) {
                    parser_errorWatcher(SET_ERROR_SEM_REDEF_OR_CONSTDEF);
                }
                else {
                    parser_errorWatcher(SET_ERROR_INTERNAL);
                }

                goto parseNullCond_errorLevel_1;  // ošetření chyb na konci funkce
            }

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            // Očekávaným terminálem je "|"
            if(LLparser_isNotExpectedTerminal(T_PIPE)) {
                goto parseNullCond_errorLevel_1;  // ošetření chyb na konci funkce
            }

            // Vytvoříme a inicializujeme uzel proměnnou "id_bez_null"
            AST_VarNode *nullCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
            if(nullCond == NULL) {
                goto parseNullCond_errorLevel_1;  // ošetření chyb na konci funkce
            }

            AST_initNewVarNode(nullCond, AST_VAR_NODE, identifier, frameStack.top->frameID, \
                               AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

            // Žádáme o další token
            parser_getNextToken(POKE_SCANNER);

            return nullCond;
        } // case NULL_COND_1

        // <NT_NULL_COND> -> ε
        case NULL_COND_2:
            return NULL;  // ε reprezentujeme jako NULL

        // Výchozí stav: došlo k chybě
        default:
            goto parseNullCond_errorLevel_0;  // ošetření chyb na konci funkce
    }

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: identifikátor proměnné "id_bez_null"
    parseNullCond_errorLevel_1:
        if(identifier != NULL && identifier != currentTerminal.value) {
            DString_free(identifier);
            identifier = NULL;
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseNullCond_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseNullCond()

/**
 * @brief Analyzuje neterminál @c \<SEQUENCE>, který reprezentuje blok kódu
 *        ohraničený složenými závorkami.
 */
AST_StatementNode *LLparser_parseSequence(bool createFrame) {
    // Očekávaným terminálem je "{"
    if(LLparser_isNotExpectedTerminal(T_LEFT_CURLY_BRACKET)) {
        goto parseSequence_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Pushneme nový rámec na zásobník rámců
    if(createFrame) {
        frameStack_push(NOT_FUNCTION);
    }

    // Analyzujeme neterminál <STATEMENT_LIST>
    AST_StatementNode *statementList = LLparser_parseStatementList();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseSequence_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je "}"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_CURLY_BRACKET)) {
        goto parseSequence_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    return statementList;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: seznam uzlů příkazů bloku příkazů
    parseSequence_errorLevel_1:
        AST_destroyStatementList(statementList);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseSequence_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseSequence()

/**
 * @brief Analyzuje neterminál @c \<WHILE>, který reprezentuje cyklus @c while.
 */
AST_WhileNode *LLparser_parseWhile() {
    // Očekávaným terminálem je "while"
    if(LLparser_isNotExpectedTerminal(T_WHILE)) {
        goto parseWhile_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseWhile_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme od další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme výraz (podmínku while) - předáváme řízení precedenčnímu synt. analyzátoru
    AST_ExprNode *condition = precParser_parse(NT_WHILE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseWhile_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je ")"
    if(LLparser_isNotExpectedTerminal(T_RIGHT_BRACKET)) {
        goto parseWhile_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <NULL_COND>
    AST_VarNode *nullCond = LLparser_parseNullCond();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseWhile_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Analyzujeme neterminál <SEQUENCE>
    AST_StatementNode *body = LLparser_parseSequence(nullCond == NULL);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }
        goto parseWhile_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Popneme rámec pro 'while' blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        goto parseWhile_errorLevel_3;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel pro příkaz "while"
    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);

    if(whileNode == NULL) {
        goto parseWhile_errorLevel_3;  // ošetření chyb na konci funkce
    }

    AST_initNewWhileNode(whileNode, condition, nullCond, body);

    return whileNode;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 3: seznam uzlů příkazů těla cyklu "while"
    parseWhile_errorLevel_3:
        AST_destroyStatementList(body);

    // Úroveň 2: uzel pro volitelnou nullable podmínku
    parseWhile_errorLevel_2:
        AST_destroyNode(AST_VAR_NODE, nullCond);

    // Úroveň 1: uzel pro výraz v podmínce cyklu "while"
    parseWhile_errorLevel_1:
        AST_destroyNode(AST_EXPR_NODE, condition);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseWhile_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseWhile()

/**
 * @brief Analyzuje neterminál @c \<ARGUMENTS>, který reprezentuje seznam
 *        argumentů funkce.
 */
AST_ArgOrParamNode *LLparser_parseArguments() {
    AST_ArgOrParamNode *argList = NULL;     // seznam argumentů
    AST_ArgOrParamNode *currentArg = NULL;  // ukazatel na poslední argument v seznamu
    AST_ExprNode *arg = NULL;               // pro návrstovou hodnotu prec. synt. anal.

    // Cyklus provádíme dokud v rámci této funkce nebude aktuálním terminálem pravá závorka
    do {
        // Analyzujeme výraz (jeden argument) - předáváme řízení precedenčnímu synt. analyzátoru
        arg = precParser_parse(NT_ARGUMENTS);

        if(parser_errorWatcher(IS_PARSING_ERROR)) {
            goto parseArguments_errorHandle;  // ošetření chyb na konci funkce
        }

        // Pokud argument existuje (nejednalo se o prázdný výraz)
        if(arg != NULL) {
            // Vytvoříme nový uzel pro argument a inicializujme ho
            AST_ArgOrParamNode *newArg = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);

            if(parser_errorWatcher(IS_PARSING_ERROR)) {
                goto parseArguments_errorHandle;  // ošetření chyb na konci funkce
            }

            AST_initNewArgOrParamNode(newArg, AST_DATA_TYPE_NOT_DEFINED, arg);

            // Připojení nového uzlu do seznamu
            if(argList == NULL) {
                argList = newArg;
            }
            else {
                currentArg->next = newArg;
            }

            currentArg = newArg;
        }

        // S oddělovačem argumentů nic needěláme (může být za posledním argumentem)
        if(currentTerminal.LLterminal == T_COMMA) {
            parser_getNextToken(POKE_SCANNER);
        }
    } while(currentTerminal.LLterminal != T_RIGHT_BRACKET);

    return argList;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu; seznam argumentů
    parseArguments_errorHandle:
        AST_destroyArgOrParamList(argList);
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseArguments()


/*******************************************************************************
 *                                                                             *
 *        IMPLEMENTACE INTERNÍCH FUNKCÍ PRO APLIKACI NĚKTERÝCH PRAVIDEL        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> <VAR_DEF> ;`.
 */
AST_StatementNode *LLparser_parseRuleStatement1() {
    // Analyzujeme neterminál <VAR_DEF>
    AST_StatementNode *varDef = LLparser_parseVarDef();

    if(varDef == NULL) {
        AST_destroyStatementList(varDef);
        goto parseRuleStatement1_handleError;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je ";"
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        AST_destroyNode(AST_STATEMENT_NODE, varDef);
        goto parseRuleStatement1_handleError;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    return varDef;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatement1_handleError:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
}  // LLparser_parseRuleStatement1()

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> id <STATEMENT_REST> ;`.
 */
AST_StatementNode *LLparser_parseRuleStatement2() {
    // Očekávaným terminálem je "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseRuleStatement2_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Uchováme identifikátor proměnné
    // (v případě chyby je uvolněn ve funkci LLparser_parseStatementRest() )
    DString *identifier = currentTerminal.value;

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Vytvoříme a inicializujeme uzel pro tento výraz a parsujeme <STATEMENT_REST>
    AST_StatementNode *statementRest = LLparser_parseStatementRest(&identifier);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement2_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je ";"
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement2_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    return statementRest;

    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: seznam uzlů příkazů
    parseRuleStatement2_errorLevel_1:
        AST_destroyStatementList(statementRest);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatement2_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement2()

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> _ = <THROW_AWAY> ;`.
 */
AST_StatementNode *LLparser_parseRuleStatement3() {
    // Očekávaným terminálem je "_"
    if(LLparser_isNotExpectedTerminal(T_DUMP)) {
        goto parseRuleStatement3_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Vytvoříme uzel pro proměnnou
    AST_VarNode *dumpVar = (AST_VarNode *)AST_createNode(AST_VAR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Vytvoříme identifikátor pro pseudoproměnnou "_"
    DString *underscore = DString_constCharToDString("_");

    // Inicializujeme uzel pro pseudo-proměnnou "_"
    AST_initNewVarNode(dumpVar, AST_VAR_NODE, underscore, frameStack.top->frameID, \
                       AST_LITERAL_NOT_DEFINED, NULL);

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "="
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        goto parseRuleStatement3_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel výrazu pro cíl přiřazení
    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_1;  // ošetření chyb na konci funkce
    }

    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, dumpVar);

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <THROW_AWAY>
    AST_ExprNode *rightOperand = LLparser_parseThrowAway();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel binární operace pro přiřazení "="
    AST_BinOpNode *assignBinOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_3;  // ošetření chyb na konci funkce
    }

    AST_initNewBinOpNode(assignBinOpNode, AST_OP_ASSIGNMENT, leftOperand, rightOperand);

    // Vložíme a inicializujeme uzel pro výraz přířazení
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_4;  // ošetření chyb na konci funkce
    }

    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignBinOpNode);

    // Vytvoříme a inicializujemeuzel příkazu pro definici proměnné
    AST_StatementNode *dumpStatement = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement3_errorLevel_4;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(dumpStatement, frameStack.top->frameID, \
                             AST_STATEMENT_EXPR, assignExprNode);

    // Očekávaným terminálem je ";"
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement3_errorLevel_5;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    return dumpStatement;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 5: uzel pro příkaz (resp. celý výraz přiřazení)
    parseRuleStatement3_errorLevel_5:
        AST_destroyStatementList(dumpStatement);
        goto parseRuleStatement3_errorLevel_0;  // skok na úroveň 0

    // Úroveň 4: uzel pro binární operaci (přiřazení)
    parseRuleStatement3_errorLevel_4:
        AST_destroyNode(AST_BIN_OP_NODE, assignBinOpNode);
        goto parseRuleStatement3_errorLevel_0;  // skok na úroveň 0

    // Úroveň 3: uzel pro výraz pravého operandu výrazu přiřazení
    parseRuleStatement3_errorLevel_3:
        AST_destroyNode(AST_EXPR_NODE, rightOperand);  // propad na úroveň 2

    // Úroveň 2: uzel pro výraz levého operandu výrazu přiřazení
    parseRuleStatement3_errorLevel_2:
        AST_destroyNode(AST_EXPR_NODE, leftOperand);
        goto parseRuleStatement3_errorLevel_0;  // skok na úroveň 0

    // Úroveň 1: uzel pro pseudo-proměnnou "_"
    parseRuleStatement3_errorLevel_1:
        AST_destroyNode(AST_VAR_NODE, dumpVar);  // propad na úroveň 0

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatement3_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement3()

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> <IF>`.
 */
AST_StatementNode *LLparser_parseRuleStatement4() {
    // Analyzujeme neterminál <IF>
    AST_IfNode *ifNode = LLparser_parseIf();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement4_errorHandle;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel pro příkaz
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_IF_NODE, ifNode);
        goto parseRuleStatement4_errorHandle;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_IF, ifNode);

    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatement4_errorHandle:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement4()

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> <WHILE>`.
 */
AST_StatementNode *LLparser_parseRuleStatement5() {
    // Analyzujeme neterminál <WHILE>
    AST_WhileNode *whileNode = LLparser_parseWhile();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement5_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Vytvoříme a inicializujeme uzel pro příkaz
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement5_errorLevel_2;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_WHILE, whileNode);

    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 2: seznam uzlů příkazů
    parseRuleStatement5_errorLevel_2:
        AST_destroyStatementList(statementNode);

    // Úroveň 1: uzel příkaz cyklu while
    parseRuleStatement5_errorLevel_1:
        AST_destroyNode(AST_WHILE_NODE, whileNode);

    // parseRuleStatement5_errorLevel_0
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement5()

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> return [precedence_expression] ;`.
 */
AST_StatementNode *LLparser_parseRuleStatement6() {
    // Očekávaným terminálem je "return"
    if(LLparser_isNotExpectedTerminal(T_RETURN)) {
        goto parseRuleStatement6_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme výraz - předáváme řízení precedenčnímu synt. analyzátoru
    AST_ExprNode *returnExpr = precParser_parse(NT_STATEMENT);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement6_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Očekávaným terminálem je ";"
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement6_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Vytvoříme a inicializujeme uzel pro příkaz
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement6_errorLevel_1;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_RETURN, returnExpr);

    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 1: uzel pro výraz návratového příkazu
    parseRuleStatement6_errorLevel_1:
        AST_destroyNode(AST_EXPR_NODE, returnExpr);

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatement6_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement6()

/**
 * @brief Analyzuje pravidlo `<STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;`.
 */
AST_StatementNode *LLparser_parseRuleStatement7() {
    // Očekávaným terminálem je "ifj"
    if(LLparser_isNotExpectedTerminal(T_IFJ)) {
        goto parseRuleStatement7_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "."
    if(LLparser_isNotExpectedTerminal(T_DOT)) {
        goto parseRuleStatement7_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "id"
    if(LLparser_isNotExpectedTerminal(T_ID)) {
        goto parseRuleStatement7_errorLevel_0;  // ošetření chyb na konci funkce
    }

    // Uchováme identifikátor
    DString *identifier = currentTerminal.value;

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je "("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseRuleStatement7_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme neterminál <ARGUMENTS>
    AST_ArgOrParamNode *arguments = LLparser_parseArguments();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement7_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Očekávaným terminálem je ";"
    if(LLparser_isNotExpectedTerminal(T_SEMICOLON)) {
        goto parseRuleStatement7_errorLevel_2;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Vytvoříme a inicializujeme uzal pro volání funkce
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatement7_errorLevel_2;  // ošetření chyb na konci funkce
    }

    AST_initNewFunCallNode(funCallNode, identifier, true, arguments);

    // Vytvoříme a inicializujeme uzel pro příkaz
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(statementNode == NULL) {
        AST_destroyStatementList(statementNode);
        goto parseRuleStatement7_errorLevel_0;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_FUN_CALL, funCallNode);

    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 2: seznam uzlů argumentů volání funkce
    parseRuleStatement7_errorLevel_2:
        AST_destroyArgOrParamList(arguments);

    // Úroveň 1: řetězec identifikátoru volané funkce
    parseRuleStatement7_errorLevel_1:
        if(identifier != NULL && identifier != currentTerminal.value) {
            DString_free(identifier);
            identifier = NULL;
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatement7_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatement7()

/**
 * @brief Analyzuje pravidlo `<STATEMENT_REST> -> = [precedence_expression]`.
 */
AST_StatementNode *LLparser_parseRuleStatementRest1(DString **identifier) {
    // Očekávaným terminálem je "="
    if(LLparser_isNotExpectedTerminal(T_ASSIGNMENT)) {
        goto parseRuleStatementRest1_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

    // Analyzujeme výraz - předáváme řízení precedenčnímu synt. analyzátoru
    AST_ExprNode *expr = precParser_parse(NT_STATEMENT_REST);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Vyhledáme proměnnou v tabulce symbolů (již by měla být obsažena)
    SymtableItem *varItem = NULL;
    FrameStack_result result = frameStack_findItem(*identifier, &varItem);

    if(result != FRAME_STACK_SUCCESS) {
        // Pokud obsažena není, dochází k sémantické chybě 3
        if(result == FRAME_STACK_ITEM_DOESNT_EXIST) {
            parser_errorWatcher(SET_ERROR_SEM_UNDEF);
        }
        else {
            parser_errorWatcher(SET_ERROR_INTERNAL);
        }

        goto parseRuleStatementRest1_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Pokud obsažena byla, nastavíme, že byla změněna a použita
    varItem->changed = true;
    varItem->used = true;

    // Vytvoříme uzel pro proměnnou a výraz přiřazení (levý i pravý operand)
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_2;
    }

    AST_BinOpNode *assignOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_3;
    }

    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_BIN_OP_NODE);


    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest1_errorLevel_3;  // ošetření chyb na konci funkce
    }

    // Inicializujeme uzly pro proměnnou a výraz přiřazení (levý i pravý operand)
    AST_initNewVarNode(varNode, AST_VAR_NODE, *identifier, frameStack_getId(*identifier), \
                       AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, varNode);

    AST_initNewBinOpNode(assignOpNode, AST_OP_ASSIGNMENT, leftOperand, expr);

    // Vytvoříme a inicializujeme uzel výrazu pro výraz přiřazení
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_BIN_OP_NODE, assignOpNode);
        goto parseRuleStatementRest1_errorLevel_0;  // ošetření chyb na konci funkce
    }

    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignOpNode);

    // Vytvoříme a inicializujeme uzel příkazu
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, assignExprNode);
        goto parseRuleStatementRest1_errorLevel_0;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_EXPR, assignExprNode);

    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 3: uzel pro proměnnou
    parseRuleStatementRest1_errorLevel_3:
        AST_destroyNode(AST_VAR_NODE, varNode);

    // Úroveň 2: uzel pro výraz
    parseRuleStatementRest1_errorLevel_2:
        AST_destroyNode(AST_EXPR_NODE, expr);

    // Úroveň 1: identifikátor cílové proměnné přiřazení
    parseRuleStatementRest1_errorLevel_1:
        if(*identifier != NULL && *identifier != currentTerminal.value) {
            DString_free(*identifier);
            *identifier = NULL;
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatementRest1_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatementRest1()

/**
 * @brief Analyzuje pravidlo `<STATEMENT_REST> -> ( <ARGUMENTS> )`.
 */
AST_StatementNode *LLparser_parseRuleStatementRest2(DString **identifier) {
    // Očekávaným terminálem je "("
    if(LLparser_isNotExpectedTerminal(T_LEFT_BRACKET)) {
        goto parseRuleStatementRest2_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);


    // Analyzujeme neterminál <ARGUMENTS>
    AST_ArgOrParamNode *arguments = LLparser_parseArguments();

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest2_errorLevel_1;  // ošetření chyb na konci funkce
    }

    // Žádáme o další token
    parser_getNextToken(POKE_SCANNER);

   // Vytvoříme a inicializujeme uzel pro volání funkce
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest2_errorLevel_1;  // ošetření chyb na konci funkce
    }

    AST_initNewFunCallNode(funCallNode, *identifier, false, arguments);

    // Vytvoříme a inicializujeme uzel příkazu
    AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);

    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        goto parseRuleStatementRest2_errorLevel_2;  // ošetření chyb na konci funkce
    }

    AST_initNewStatementNode(statementNode, frameStack.top->frameID, \
                             AST_STATEMENT_FUN_CALL, funCallNode);

    return statementNode;


    /***                                                                    ***/
    /*              ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ TÉTO FUNKCE             */
    /***                                                                    ***/

    // Úroveň 2: uzel pro volání funkce
    parseRuleStatementRest2_errorLevel_2:
        AST_destroyNode(AST_FUN_CALL_NODE, funCallNode);
        goto parseRuleStatementRest2_errorLevel_0;  // skok na úroveň 0

    // Úroveň 1: řetězec identifikátoru colané funkce
    parseRuleStatementRest2_errorLevel_1:
        if(*identifier != NULL && *identifier != currentTerminal.value) {
            DString_free(*identifier);
            *identifier = NULL;  // propad na úroveň 0
        }

    // Úroveň 0: hodnota řetězce aktuálního terminálu
    parseRuleStatementRest2_errorLevel_0:
        parser_freeCurrentTerminalValue();
        return PARSING_ERROR;
} // LLparser_parseRuleStatementRest2()


/*******************************************************************************
 *                                                                             *
 *             IMPLEMENTACE POMOCNÝCH INTERNÍCH FUNKCÍ LL PARSERU              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje potřebné struktury pro LL syntaktický analyzátor.
 */
void LLparser_initParserStructures() {
    // Pro jistotu vyresetujeme statické proměnné
    // (pozn. nevyresetování způsobuje problémy při opakovaném spouštění)
    parser_getNextToken(RESET_LOOKAHEAD);
    parser_errorWatcher(RESET_ERROR_FLAGS);

    // Alokujeme kořen AST a kontrolujeme úspěšnost alokace
    AST_initTree();
    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        return;
    }

    // Alokujeme zásobník rámců (resp. globální rámec) a kontrolujeme úspěšnost alokace
    frameStack_init();
    if(frameStack.bottom == NULL || frameStack.top == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Alokujeme seznam precedenčních zásobníků a kontrolujeme úspěšnost alokace
    precStackList_create();
    if(parser_errorWatcher(IS_PARSING_ERROR)) {
        return;
    }
}

/**
 * @brief Kontroluje, zda aktuální terminál odpovídá očekávanému terminálu.
 */
inline bool LLparser_isNotExpectedTerminal(LLTerminals expectedTerminal) {
    // Pokud se typ aktualního teminálu neshoduje s očekávaným => syntaktická chyba
    if(currentTerminal.LLterminal != expectedTerminal) {
        parser_errorWatcher(SET_ERROR_SYNTAX);
        return true;
    }
    // Pokud se shodují vracíme false (pozn. logika funkce je IS NOT)
    else {
        return false;
    }
}

/*** Konec souboru llparser.c ***/
