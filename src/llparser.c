/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           llparser.c                                                *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            01.11.2024                                                *
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
 *           FUNKCE LL-PARSERU URČENÉ K PARSOVÁNÍ POMOCÍ LL-GRAMATKY           *
 *                                                                             *
 ******************************************************************************/

AST_StatementNode *LLparser_parseStatementRest(DString *identifier);
AST_FunCallNode *LLparser_parseFunCall(DString *identifier, bool isBuiltIn);



/*******************************************************************************
 *                                                                             *
 *           FUNKCE LL-PARSERU URČENÉ K PARSOVÁNÍ POMOCÍ LL-GRAMATKY           *
 *                                                                             *
 ******************************************************************************/

AST_ProgramNode *LLparser_parseProgram() {
    // Alokujeme kořen AST a zkontrolujeme úspěšnost alokace
    AST_ProgramNode *programNode = AST_createTree();

    if(programNode == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme prvním voláním scanneru "currentToken" a "lookaheadToken"
    Parser_getNextToken(LL_PARSER);

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nebylo pravidlo nalazene, nastala syntaktická chyba
    if(!LLtable_findRule(currentToken.LLterminal, NT_FUN_DEF_LIST, &rule)) {
        AST_destroyNode(AST_PROGRAM_NODE, programNode);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Volba dalšího postupu parsování na základě LL pravidla
    // <PROGRAM> -> <PROLOGUE> <FUN_DEF_LIST> EOF
    if(rule == PROGRAM) {
        // Parsujeme <PROLOGUE>
        programNode->importedFile = LLparser_parsePrologue();
        if(programNode->importedFile == NULL) {
            AST_destroyNode(AST_PROGRAM_NODE, programNode);
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }

        // Pokud v Symtable proměnná prologu již je, značí to interní chybu překladače
        if(frameStack_findItem(programNode->importedFile, NULL) != FRAME_STACK_ITEM_DOESNT_EXIST) {
            AST_destroyNode(AST_PROGRAM_NODE, programNode);
            error_handle(ERROR_INTERNAL);
        }

        // Žádáme o další token
        Parser_getNextToken(LL_PARSER);

        // Parsujeme <FUN_DEF_LIST>
        AST_FunDefNode *funDefNode = LLparser_parseFunDefList();

        // Jelikož <FUN_DEF_LIST> může být rozvinutou na ε, musíme kromě NULL zkontrolovat error flag
        if(funDefNode == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
            AST_destroyNode(AST_PROGRAM_NODE, programNode);
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }
        ASTroot->functionList = funDefNode;

        // Parsujeme `EOF`
        if(currentToken.LLterminal != T_EOF) {
            AST_destroyNode(AST_PROGRAM_NODE, programNode);
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }
    }
    // Pokud již dříve nebyla hlášena ERROR_SYNTAX, tak je tato větev ERROR_INTERNAL
    else {
        AST_destroyNode(AST_PROGRAM_NODE, programNode);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Vracíme ukazatel na AST_ProgramNode
    return programNode;
}

// <PROLOGUE> -> const ifj = @import ( "ifj24.zig" ) ;
DString *LLparser_parsePrologue() {
    // Parsujeme `const`
    if(currentToken.LLterminal != T_CONST) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `ifj`
    DString *ifjVar = currentToken.value;

    // Kontrolujeme přijetí očekávaného identifikátoru
    if(currentToken.LLterminal != T_ID || string_compare_const_str(ifjVar, "ifj") != STRING_EQUAL) {
        string_free(ifjVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `=`
    if(currentToken.LLterminal != T_ASSIGNMENT) {
        string_free(ifjVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `@import`
    if(currentToken.LLterminal != T_IMPORT) {
        string_free(ifjVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `(`
    if(currentToken.LLterminal != T_LEFT_BRACKET) {
        string_free(ifjVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `"ifj24.zig"`
    DString *importedFile = currentToken.value;

    // Kontrolujeme přijetí očekávaného souboru k importování
    if(importedFile == NULL || string_compare_const_str(currentToken.value, "ifj24.zig") != STRING_EQUAL) {
        string_free(ifjVar);
        string_free(importedFile);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Přidáme proměnnou prologu do tabulky symbolů a zkontrolujeme úspěšnost přidání
    frame_stack_result result = frameStack_addItemExpress(ifjVar, SYMTABLE_SYMBOL_VARIABLE_STRING, true, importedFile);

    // Kontrola úspěšného vložení do tabulky symbolů
    if(result != FRAME_STACK_SUCCESS) {
        error_handle(ERROR_INTERNAL);
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `)`
    if(currentToken.LLterminal != T_RIGHT_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `;`
    if(currentToken.LLterminal != T_SEMICOLON) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    return importedFile;
} // LLparser_parsePrologue()

// <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST> | ε
AST_FunDefNode *LLparser_parseFunDefList() {
    // Statická proměnná pro uchování seznamu definic funkcí
    static AST_FunDefNode *funDefListHead = NULL;

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, uvvolníme uzel a vracíme NULL
    if(!LLtable_findRule(currentToken.LLterminal, NT_FUN_DEF_LIST, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Na <FUN_DEF_LIST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST>
        case FUN_DEF_LIST_1: {
            // Parsujeme <FUN_DEF>
            AST_FunDefNode *funDef = LLparser_parseFunDef();

            // Kontrola úspěchu parsování <FUN_DEF>
            if(funDef == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                AST_destroyFunDefList(funDefListHead);
                funDefListHead = NULL;
                return PARSING_SYNTAX_ERROR;
            }

            // Připojíme novou definici funkce do seznamu definic funkcí
            AST_insertFirstFunDefNode(&funDefListHead, funDef);

            // Parsujeme <FUN_DEF_LIST>
            AST_FunDefNode *funDefList = LLparser_parseFunDefList();

            // Kontrola úspěchu parsování <FUN_DEF_LIST>
            if(funDefList == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                AST_destroyFunDefList(funDefListHead);
                funDefListHead = NULL;
                return PARSING_SYNTAX_ERROR;
            }

            // Vrátíme ukazatel na seznam definic funkcí
            return funDefListHead;
        }

        // <FUN_DEF_LIST> -> ε
        case FUN_DEF_LIST_2:
            return NULL;    // Vracíme NULL, jelikož rozvíjíme na prázdný řetězec

        // Defaultní stav: došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            AST_destroyFunDefList(funDefListHead);
            funDefListHead = NULL;
            return PARSING_SYNTAX_ERROR;
    } // switch()
} // LLparser_parseFunDefList()

// <FUN_DEF> -> pub fn id ( <PARAMETERS> ) <RETURN_TYPE> <SEQUENCE>
AST_FunDefNode *LLparser_parseFunDef() {
    // Parsujeme "pub"
    if(currentToken.LLterminal != T_PUB) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "fn"
    if(currentToken.LLterminal != T_FN) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "id"
    if(currentToken.LLterminal != T_ID) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Uchováme identifikátor funkce
    DString *functionName = currentToken.value;

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "("
    if(currentToken.LLterminal != T_LEFT_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        string_free(functionName);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Pushneme nový rámce příslušící parsované funkci
    frameStack_push(true);

    // Počítadlo parametrů funkce
    size_t paramCount = 0;

    // Parsujeme <PARAMETERS>
    AST_ArgOrParamNode *parameters = LLparser_parseParameters(&paramCount);

    // Kontrola úspěchu parsování <PARAMETERS>
    if(parameters == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme ")"
    if(currentToken.LLterminal != T_RIGHT_BRACKET) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme <RETURN_TYPE>
    AST_DataType returnType = LLparser_parseReturnType();

    // Kontrola úspěchu parsování <RETURN_TYPE>
    if(returnType == AST_DATA_TYPE_NOT_DEFINED) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme <SEQUENCE>
    AST_StatementNode *sequence = LLparser_parseSequence();

    // Kontrola úspěchu parsování <SEQUENCE>
    if(sequence == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        AST_destroyStatementList(sequence);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Vytvoříme AST uzel pro definici funkce
    AST_FunDefNode *funDefNode = (AST_FunDefNode *)AST_createNode(AST_FUN_DEF_NODE);

    // Kontrola úspěchu vytvoření AST uzlu
    if(funDefNode == NULL) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        AST_destroyStatementList(sequence);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Inicializace uzlu pro definici funkce
    AST_initNewFunDefNode(funDefNode, functionName, parameters, returnType, sequence);

    // Vytvoříme strukturu SymtableFunctionData pro uložení do tabulky symbolů
    SymtableFunctionData *functionData = symtable_init_function_data(paramCount);

    // Kontrola úspěchu vytvoření dat pro funkci
    if(functionData == NULL) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        AST_destroyStatementList(sequence);
        AST_destroyNode(AST_FUN_DEF_NODE, funDefNode);
        if (frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Proměnná k přemapování AST návratového typu na Symtable návratový typ funkce
    symtable_functionReturnType funType = SYMTABLE_TYPE_UNKNOWN;

    // Naplníme strukturu SymtableFunctionData parametry
    AST_ArgOrParamNode *paramNode = parameters;
    for(size_t i = 0; i < paramCount; i++) {
        Parser_mapASTDataTypeToFunReturnType(paramNode->dataType, &funType);
        functionData->params[i].id = paramNode->variable->identifier;
        functionData->params[i].type = funType;
        paramNode = paramNode->next;
    }

    Parser_mapASTDataTypeToFunReturnType(returnType, &funType);
    functionData->return_type = funType;

    // Přidáme definici funkce do tabulky symbolů
    SymtableItem *functionItem = NULL;
    frame_stack_result result = frameStack_addItemExpress(functionName, SYMTABLE_SYMBOL_FUNCTION, false, &functionItem);
    if(result != FRAME_STACK_SUCCESS) {
        string_free(functionName);
        AST_destroyArgOrParamList(parameters);
        AST_destroyStatementList(sequence);
        AST_destroyNode(AST_FUN_DEF_NODE, funDefNode);
        free(functionData->params);
        free(functionData);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }
    functionItem->data = functionData;

    // Popneme rámce parsované funkce po dokončení jejího parsování
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        error_handle(ERROR_INTERNAL);
    }

    return funDefNode;
} // LLparser_parseFunDef()

// <PARAMETERS> -> <PARAM_LIST> | ε
AST_ArgOrParamNode *LLparser_parseParameters(size_t *paramCount) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, vracíme NULL
    if(!LLtable_findRule(currentToken.LLterminal, NT_FUN_DEF_LIST, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

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
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    }
} // LLparser_parseParameters()

// <PARAM_LIST> -> <PARAM> <PARAM_LIST_REST>
AST_ArgOrParamNode *LLparser_parseParamList(size_t *paramCount) {
    // Parsujeme <PARAM>
    AST_ArgOrParamNode *paramList = LLparser_parseParam();

    // Kontrola úspěchu parsování <PARAM>
    if(paramList == NULL) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Zvýšíme počet parametrů
    (*paramCount)++;

    // Parsujeme <PARAM_LIST_REST>
    AST_ArgOrParamNode *paramListRest = LLparser_parseParamListRest(paramCount);

    // Kontrola úspěchu parsování <PARAM_LIST_REST>
    if (paramListRest == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        AST_destroyArgOrParamList(paramList);
        return PARSING_SYNTAX_ERROR;
    }

    // Kontrola úspěchu parsování <PARAM_LIST_REST>
    if(paramListRest != NULL) {
        paramList->next = paramListRest;
    }

    // Vracíme <PARAM_LIST>
    return paramList;
} // LLparser_parseParamList()

// <PARAM_LIST_REST> -> , <PARAM_LIST> | ε
AST_ArgOrParamNode *LLparser_parseParamListRest(size_t *paramCount) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, vracíme NULL
    if (!LLtable_findRule(currentToken.LLterminal, NT_PARAM_LIST_REST, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

     // Na <PARAM_LIST_REST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <PARAM_LIST_REST> -> , <PARAM_LIST>
        case PARAM_LIST_REST_1: {
            if(currentToken.LLterminal != T_COMMA) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme <PARAM_LIST>
            AST_ArgOrParamNode *paramList = LLparser_parseParamList(paramCount);

            // Kontrola úspěchu parsování <PARAM_LIST>
            if (paramList == PARSING_SYNTAX_ERROR) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);

                return PARSING_SYNTAX_ERROR;
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
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    } // switch()
} // LLparser_parseParamListRest()



// <PARAM> -> id : <DATA_TYPE>
AST_ArgOrParamNode *LLparser_parseParam() {
    // Parsujeme "id"
    if(currentToken.LLterminal != T_ID) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Identifikátorem parametru je hodnota token
    DString *paramId = currentToken.value;

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme ":"
    if(currentToken.LLterminal != T_COLON) {
        string_free(paramId);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "<DATA_TYPE>"
    AST_DataType dataType = LLparser_parseDataType();

    // Kontrola validity získaného návratového typu
    if(dataType == AST_DATA_TYPE_NOT_DEFINED) {
        string_free(paramId);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Vytvoříme AST uzel pro parametr
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(varNode == NULL) {
        string_free(paramId);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }
    AST_initNewVarNode(varNode, AST_VAR_NODE, paramId, frameStack.currentID, AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

    // Mapujeme AST datový typ na Symtable SymbolState
    symtable_symbolState state = SYMTABLE_SYMBOL_UNKNOWN;
    Parser_mapASTDataTypeToSymtableState(dataType, &state);

    // Přidáme parametr do tabulky symbolů jako lokální proměnnou
    frame_stack_result result = frameStack_addItemExpress(paramId, state, false, NULL);
    if (result != FRAME_STACK_SUCCESS) {
        string_free(paramId);
        AST_destroyNode(AST_VAR_NODE, varNode);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    AST_ArgOrParamNode *paramNode = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    if(paramNode == NULL) {
        AST_destroyNode(AST_VAR_NODE, varNode);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }
    AST_initNewArgOrParamNode(paramNode, dataType, varNode);

    // Vracíme <PARAM>
    return paramNode;
} // LLparser_parseParam()

// <RETURN_TYPE> -> <DATA_TYPE> | void
AST_DataType LLparser_parseReturnType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, vracíme AST_DATA_TYPE_NOT_DEFINED
    if (!LLtable_findRule(currentToken.LLterminal, NT_RETURN_TYPE, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return AST_DATA_TYPE_NOT_DEFINED;
    }

    // Na <RETURN_TYPE> lze aplikovat dvě různá pravidla
    switch (rule) {
        // <RETURN_TYPE> -> <DATA_TYPE>
        case RETURN_TYPE_1:
            // Parsujeme <DATA_TYPE>
            return LLparser_parseDataType();

        // <RETURN_TYPE> -> void
        case RETURN_TYPE_2:
            if (currentToken.LLterminal != T_VOID) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "void"
            return AST_DATA_TYPE_VOID;

        // Defaultní stav: došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return AST_DATA_TYPE_NOT_DEFINED;
    } // switch()
} // LLparser_parseReturnType()

// <DATA_TYPE> -> i32 | ?i32 | f64 | ?f64 | []u8 | ?[]u8
AST_DataType LLparser_parseDataType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, vracíme AST_DATA_TYPE_NOT_DEFINED
    if (!LLtable_findRule(currentToken.LLterminal, NT_DATA_TYPE, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return AST_DATA_TYPE_NOT_DEFINED;
    }

    // Na <DATA_TYPE> lze aplikovat různá pravidla
    switch(rule) {
        // <DATA_TYPE> -> i32
        case DATA_TYPE_1:
            if (currentToken.LLterminal != T_INT) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "i32"
            return AST_DATA_TYPE_INT;

        // <DATA_TYPE> -> ?i32
        case DATA_TYPE_2:
            if (currentToken.LLterminal != T_INT_OR_NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "?i32"
            return AST_DATA_TYPE_INT_OR_NULL;

        // <DATA_TYPE> -> f64
        case DATA_TYPE_3:
            if (currentToken.LLterminal != T_FLOAT) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "f64"
            return AST_DATA_TYPE_FLOAT;

        // <DATA_TYPE> -> ?f64
        case DATA_TYPE_4:
            if (currentToken.LLterminal != T_FLOAT_OR_NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "?f64"
            return AST_DATA_TYPE_FLOAT_OR_NULL;

        // <DATA_TYPE> -> []u8
        case DATA_TYPE_5:
            if (currentToken.LLterminal != T_STRING) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "[]u8"
            return AST_DATA_TYPE_STRING;

        // <DATA_TYPE> -> ?[]u8
        case DATA_TYPE_6:
            if (currentToken.LLterminal != T_STRING_OR_NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme typ "?[]u8"
            return AST_DATA_TYPE_STRING_OR_NULL;

        // Defaultní stav: došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return AST_DATA_TYPE_NOT_DEFINED;
    } // switch()
} // LLparser_parseDataType()

/*** Konec souboru llparser.c ***/
