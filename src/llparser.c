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

AST_ProgramNode *LLparser_parseProgram() {
    // Alokujeme kořen AST a zkontrolujeme úspěšnost alokace
    AST_initTree();

    if(ASTroot == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Raději vyresetujeme statické proměnné
    Parser_getNextToken(RESET_LOOKAHEAD);
    Parser_watchSyntaxError(RESET_ERROR_FLAG);

    // Inicializujeme prvním voláním scanneru "currentToken" a "lookaheadToken"
    Parser_getNextToken(LL_PARSER);

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nebylo pravidlo nalazene, nastala syntaktická chyba
    if(!LLtable_findRule(currentToken.LLterminal, NT_PROGRAM, &rule)) {
        AST_destroyTree();
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Volba dalšího postupu parsování na základě LL pravidla
    // <PROGRAM> -> <PROLOGUE> <FUN_DEF_LIST> EOF
    if(rule == PROGRAM) {
        // Parsujeme <PROLOGUE>
        ASTroot->importedFile = LLparser_parsePrologue();
        if(ASTroot->importedFile == NULL) {
            AST_destroyTree();
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }

        // Žádáme o další token
        Parser_getNextToken(LL_PARSER);

        // Parsujeme <FUN_DEF_LIST>
        AST_FunDefNode *funDefNode = LLparser_parseFunDefList();

        // Jelikož <FUN_DEF_LIST> může být rozvinutou na ε, musíme kromě NULL zkontrolovat error flag
        if(funDefNode == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
            AST_destroyTree();
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }
        ASTroot->functionList = funDefNode;

        // Parsujeme `EOF`
        if(currentToken.LLterminal != T_EOF) {
            AST_destroyTree();
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }
    }
    // Pokud již dříve nebyla hlášena ERROR_SYNTAX, tak je tato větev ERROR_INTERNAL
    else {
        AST_destroyTree();
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Vracíme ukazatel na AST_ProgramNode
    return ASTroot;
}

// <PROLOGUE> -> const ifj = @import ( "ifj24.zig" ) ;
AST_VarNode *LLparser_parsePrologue() {
    // Parsujeme `const`
    if(currentToken.LLterminal != T_CONST) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "ifj"
    if(currentToken.LLterminal != T_IFJ) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    DString *importVar = string_charToDString("ifj");

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `=`
    if(currentToken.LLterminal != T_ASSIGNMENT) {
        string_free(importVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `@import`
    if(currentToken.LLterminal != T_IMPORT) {
        string_free(importVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `(`
    if(currentToken.LLterminal != T_LEFT_BRACKET) {
        string_free(importVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `"ifj24.zig"`
    if(currentToken.PrecTerminal != T_PREC_STRING_LITERAL) {
        string_free(importVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Kontrolujeme přijetí očekávaného souboru k importování
    if(string_compare_const_str(currentToken.value, "ifj24.zig") != STRING_EQUAL) {
        string_free(importVar);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme `"ifj24.zig"`
    DString *path = currentToken.value;

    // Přidáme proměnnou prologu do tabulky symbolů a zkontrolujeme úspěšnost přidání
    frame_stack_result result = frameStack_addItemExpress(importVar, SYMTABLE_SYMBOL_VARIABLE_STRING, true, path, NULL);

    // Kontrola úspěšného vložení do tabulky symbolů
    if(result != FRAME_STACK_SUCCESS) {
        string_free(importVar);
        string_free(path);
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme uzel pro proměnnou s cestou k importovanému souboru
    AST_VarNode *importedFile = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(importedFile == NULL) {
        string_free(importVar);
        string_free(path);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme tento uzel
    AST_initNewVarNode(importedFile, AST_VAR_NODE, importVar, frameStack.currentID, AST_LITERAL_STRING, path);

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `)`
    if(currentToken.LLterminal != T_RIGHT_BRACKET) {
        AST_destroyNode(AST_VAR_NODE, importedFile);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme `;`
    if(currentToken.LLterminal != T_SEMICOLON) {
        AST_destroyNode(AST_VAR_NODE, importedFile);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    return importedFile;
} // LLparser_parsePrologue()

// <FUN_DEF_LIST> -> <FUN_DEF> <FUN_DEF_LIST> | ε
AST_FunDefNode *LLparser_parseFunDefList() {
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
                return PARSING_SYNTAX_ERROR;
            }

            // Parsujeme zbytek seznamu <FUN_DEF_LIST>
            AST_FunDefNode *funDefList = LLparser_parseFunDefList();

            // Kontrola úspěchu parsování <FUN_DEF_LIST>
            if(funDefList == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
                AST_destroyFunDefNode(funDef);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Připojíme aktuální definici funkce na začátek seznamu
            funDef->next = funDefList;

            // Vrátíme ukazatel na seznam definic funkcí
            return funDef;
        }

        // <FUN_DEF_LIST> -> ε
        case FUN_DEF_LIST_2:
            return NULL;    // Vracíme NULL, jelikož rozvíjíme na prázdný řetězec

        // Defaultní stav: došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
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

    // Přidáme definici funkce do tabulky symbolů
    SymtableItem *functionItem = NULL;
    frame_stack_result result = frameStack_addItemExpress(functionName, SYMTABLE_SYMBOL_FUNCTION, false, NULL, &functionItem);
    if(result != FRAME_STACK_SUCCESS) {
        string_free(functionName);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }


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
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Naplníme strukturu SymtableFunctionData parametry
    AST_ArgOrParamNode *paramNode = parameters;
    symtable_functionReturnType funType = SYMTABLE_TYPE_UNKNOWN;
    for(size_t i = 0; i < paramCount; i++) {
        if(paramNode->expression->exprType != AST_EXPR_VARIABLE) {
            string_free(functionName);
            AST_destroyArgOrParamList(parameters);
            AST_destroyStatementList(sequence);
            AST_destroyNode(AST_FUN_DEF_NODE, funDefNode);
            if (frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
                error_handle(ERROR_INTERNAL);
            }
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
        }
        AST_VarNode *variable = (AST_VarNode *)(paramNode->expression->expression);
        Parser_mapASTDataTypeToFunReturnType(paramNode->dataType, &funType);
        functionData->params[i].id = variable->identifier;
        functionData->params[i].type = funType;
        paramNode = paramNode->next;
    }

    Parser_mapASTDataTypeToFunReturnType(returnType, &funType);
    functionData->return_type = funType;

    // Přidáme data funkce do položky v tabulce symbolů
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
    if(!LLtable_findRule(currentToken.LLterminal, NT_PARAMETERS, &rule)) {
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
    frame_stack_result result = frameStack_addItemExpress(paramId, state, false, NULL, NULL);
    if (result != FRAME_STACK_SUCCESS) {
        string_free(paramId);
        AST_destroyNode(AST_VAR_NODE, varNode);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Vytvoříme uzel pro výraz parametru
    AST_ExprNode *paramExpr = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(paramExpr == NULL) {
        AST_destroyNode(AST_VAR_NODE, varNode);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme výraz pro parametr
    AST_initNewExprNode(paramExpr, AST_EXPR_VARIABLE, varNode);

    AST_ArgOrParamNode *paramNode = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    if(paramNode == NULL) {
        AST_destroyNode(AST_EXPR_NODE, paramExpr);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        error_handle(ERROR_INTERNAL);
    }
    AST_initNewArgOrParamNode(paramNode, dataType, paramExpr);

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

    // Na <DATA_TYPE> lze aplikovat šest různých pravidel
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

// <STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST> | ε
AST_StatementNode *LLparser_parseStatementList() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, vracíme NULL
    if (!LLtable_findRule(currentToken.LLterminal, NT_STATEMENT_LIST, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Na <STATEMENT_LIST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <STATEMENT_LIST> -> <STATEMENT> <STATEMENT_LIST>
        case STATEMENT_LIST_1: {
            // Vytvoříme uzel pro příkaz a parsujeme <STATEMENT>
            AST_StatementNode *statement = LLparser_parseStatement();

            // Kontrola úspěšnosti parsování <STATEMENT>
            if(statement == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
                return PARSING_SYNTAX_ERROR;
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
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return AST_DATA_TYPE_NOT_DEFINED;
    }
} // LLaprser_parseStatementList()

// <STATEMENT> -> <VAR_DEF> ; | id <STATEMENT_REST> ; | _ = <THROW_AWAY> ; | <IF> | <WHILE> | return <RETURN_REST> ;
AST_StatementNode *LLparser_parseStatement() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud nastal syntax error, vracíme PARSING_SYNTAX_ERROR
    if (!LLtable_findRule(currentToken.LLterminal, NT_STATEMENT, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Na <STATEMENT> lze aplikovat šest různých pravidel
    switch (rule) {
        // <STATEMENT> -> <VAR_DEF> ;
        case STATEMENT_1: {
            // Vytvoříme uzel pro příkaz a parsujeme <VAR_DEF>
            AST_StatementNode *varDef = LLparser_parseVarDef();

            // Kontrola úspěšnosti parsování <VAR_DEF>
            if (varDef == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Parsujeme ";"
            if (currentToken.LLterminal != T_SEMICOLON) {
                AST_destroyNode(AST_STATEMENT_NODE, varDef);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme příkaz typu definice funkce
            return varDef;
        }

        // <STATEMENT> -> id <STATEMENT_REST> ;
        case STATEMENT_2: {
            // Parsujeme "id"
            if (currentToken.LLterminal != T_ID) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Zapamatujeme si identifikátor proměnné
            DString *identifier = currentToken.value;

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vytvoříme uzel pro tento výraz a parsujeme <STATEMENT_REST>
            AST_StatementNode *statementRest = LLparser_parseStatementRest(identifier);

            // Kontrola úspěcho parsování <STATEMENT_REST>
            // string je v případě chyby uvolněn v LLparser_parseStatementRest
            if(statementRest == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Parsujeme ";"
            if (currentToken.LLterminal != T_SEMICOLON) {
                AST_destroyNode(AST_STATEMENT_NODE, statementRest);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme kompletní příkaz (id + zbytek)
            return statementRest;
        }

        // <STATEMENT> -> _ = <THROW_AWAY> ;
        case STATEMENT_3: {
            // Parsujeme "_"
            if (currentToken.LLterminal != T_DUMP) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme "="
            if (currentToken.LLterminal != T_ASSIGNMENT) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vytváříme uzel pro výraz a parsujeme <THROW_AWAY>
            AST_ExprNode *expr = LLparser_parseThrowAway();
            if(expr == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Parsujeme ";"
            if (currentToken.LLterminal != T_SEMICOLON) {
                AST_destroyNode(AST_EXPR_NODE, expr);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vytvoříme uzel pro příkaz a zkontrolujeme úspěšnost alokace
            AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
            if(statementNode == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                error_handle(ERROR_INTERNAL);
            }

            // Inicializujeme uzel pro příkaz
            AST_initNewStatementNode(statementNode, frameStack.currentID, AST_STATEMENT_EXPR, expr);

            // Vracíme příkaz pro zahození výsledku
            return statementNode;
        }

        // <STATEMENT> -> <IF>
        case STATEMENT_4: {
            // Vytvoříme uzel pro podmíněný příkaz if() a parsujeme <IF>
            AST_IfNode *ifNode = LLparser_parseIf();

            // Kontrolujeme úspěšnost parsování <IF>
            if (ifNode == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Vytvoříme uzel pro příkaz a zkontrolujeme úspěšnost alokace
            AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
            if(statementNode == NULL) {
                AST_destroyNode(AST_IF_NODE, ifNode);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                error_handle(ERROR_INTERNAL);
            }

            // Inicializujeme uzel pro příkaz
            AST_initNewStatementNode(statementNode, frameStack.currentID, AST_STATEMENT_IF, ifNode);

            // Vracíme příkaz obsahující podmíněný příkaz if()
            return statementNode;
        }

        // <STATEMENT> -> <WHILE>
        case STATEMENT_5: {
            // Vytvoříme uzel pro cyklus while() a parsujeme <WHILE>
            AST_WhileNode *whileNode = LLparser_parseWhile();

            // Kontrolujeme úspěšnost parsování <WHILE>
            if (whileNode == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Vytvoříme uzel pro příkaz a zkontrolujeme úspěšnost alokace
            AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
            if (statementNode == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                AST_destroyNode(AST_WHILE_NODE, whileNode);
                error_handle(ERROR_INTERNAL);
            }

            // Inicializujeme uzel pro příkaz
            AST_initNewStatementNode(statementNode, frameStack.currentID, AST_STATEMENT_WHILE, whileNode);

            // Vracíme příkaz obsahující cyklus while()
            return statementNode;
        }

        // <STATEMENT> -> return <RETURN_REST> ;
        case STATEMENT_6: {
            // Parsujeme "return"
            if (currentToken.LLterminal != T_RETURN) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vytvoříme uzel pro příkaz a parsujeme <RETURN_REST>
            AST_StatementNode *returnNode = LLparser_parseReturn();
            if (returnNode == NULL) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Parsujeme ";"
            if (currentToken.LLterminal != T_SEMICOLON) {
                AST_destroyNode(AST_STATEMENT_NODE, returnNode);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme návratový příkaz
            return returnNode;
        }

        // <STATEMENT> -> ifj . id ( <ARGUMENTS> ) ;
        case STATEMENT_7: {
            // Parsujeme "ifj"
            if (currentToken.LLterminal != T_IFJ) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme "."
            if (currentToken.LLterminal != T_DOT) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme "id"
            if (currentToken.LLterminal != T_ID) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Uchováme identifikátor
            DString *identifier = currentToken.value;

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme "("
            if (currentToken.LLterminal != T_LEFT_BRACKET) {
                string_free(identifier);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme <ARGUMENTS>
            AST_ArgOrParamNode *arguments = LLparser_parseArguments();
            if (arguments == NULL) {
                string_free(identifier);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Parsujeme ")"
            if (currentToken.LLterminal != T_RIGHT_BRACKET) {
                AST_destroyNode(AST_ARG_OR_PARAM_NODE, arguments);
                string_free(identifier);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme ";"
            if (currentToken.LLterminal != T_SEMICOLON) {
                AST_destroyNode(AST_ARG_OR_PARAM_NODE, arguments);
                string_free(identifier);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vytvoříme uzal pro volání funkce a zkontrolujeme úspěch alokace paměti
            AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
            if (funCallNode == NULL) {
                AST_destroyNode(AST_ARG_OR_PARAM_NODE, arguments);
                string_free(identifier);
                error_handle(ERROR_INTERNAL);
                return PARSING_SYNTAX_ERROR;
            }

            // Inicializujeme uzel pro volání funkce
            AST_initNewFunCallNode(funCallNode, identifier, true, arguments);

            // Vytvoříme uzel pro příkaz a zkontrolujeme úspěch alokace paměti
            AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
            if (statementNode == NULL) {
                AST_destroyNode(AST_FUN_CALL_NODE, funCallNode);
                error_handle(ERROR_INTERNAL);
                return PARSING_SYNTAX_ERROR;
            }

            // Inicializujeme uzel pro příkaz
            AST_initNewStatementNode(statementNode, frameStack.currentID, AST_STATEMENT_FUN_CALL, funCallNode);

            // Vracíme příkaz pro volání vestavěné funkce
            return statementNode;
        }

        // Jinak došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    }
} // LLparser_parseStatement()


// <VAR_DEF> -> <MODIFIABLE> id <POSSIBLE_TYPE> = [precedence_expression]
AST_StatementNode *LLparser_parseVarDef() {
    // Parsujeme <MODIFIABLE>
    bool isModifiable;
    LLparser_parseModifiable(&isModifiable);

    // Kontrolujeme úspěch parsování <MODIFIABLE>
    if(Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme "id"
    if(currentToken.LLterminal != T_ID) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Zkontrolujeme, že se nejedná o zastínění (sémantická chyba 5)
    frame_stack_result shadowing = frameStack_findItem(currentToken.value, NULL);
    if(shadowing != FRAME_STACK_ITEM_DOESNT_EXIST) {
        error_handle(ERROR_SEM_REDEF_OR_CONSTDEF);
    }

    // Uchovávme hodnotu identifikátoru
    DString *varName = currentToken.value;

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme <POSSIBLE_TYPE>
    AST_DataType dataType = LLparser_parsePossibleType();

    // Kontrolujeme úspěch parsování <POSSIBLE_TYPE>
    if(dataType == AST_DATA_TYPE_NOT_DEFINED && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        string_free(varName);
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme '='
    if(currentToken.LLterminal != T_ASSIGNMENT) {
        string_free(varName);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme [precedence_expression]
    AST_ExprNode *rightOperand = PrecParser_parse(NT_VAR_DEF);

    // Kontrolujeme úspěch parsování
    if(Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        string_free(varName);
        return PARSING_SYNTAX_ERROR;
    }

    // Přidáme proměnnou do tabulky symbolů
    SymtableItem *varItem = NULL;
    symtable_symbolState state = SYMTABLE_SYMBOL_UNKNOWN;
    Parser_mapASTDataTypeToSymtableState(dataType, &state);
    frame_stack_result result = frameStack_addItemExpress(varName, state, isModifiable, NULL, &varItem);
    if(result != FRAME_STACK_SUCCESS) {
        string_free(varName);
        AST_destroyNode(AST_EXPR_NODE, rightOperand);
        error_handle(ERROR_INTERNAL);
    }

    if(varItem == NULL) {
        string_free(varName);
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme uzel pro proměnnou
    AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(varNode == NULL) {
        string_free(varName);
        AST_destroyNode(AST_EXPR_NODE, rightOperand);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme uzel pro proměnnou
    AST_initNewVarNode(varNode, AST_VAR_NODE, varName, frameStack.currentID, AST_LITERAL_NOT_DEFINED, NULL);

    // Vytvoříme uzel binární operace pro přiřazení "="
    AST_BinOpNode *assignOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    if(assignOpNode == NULL) {
        AST_destroyNode(AST_VAR_NODE, varNode);
        AST_destroyNode(AST_EXPR_NODE, rightOperand);
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme uzel výrazu pro cíl přiřazení
    AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_BIN_OP_NODE);
    if(leftOperand == NULL) {
        AST_destroyNode(AST_VAR_NODE, varNode);
        AST_destroyNode(AST_EXPR_NODE, rightOperand);
        error_handle(ERROR_INTERNAL);
    }

    //Inicializujeme uzel pro levý operand
    AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, varNode);

    // Inicializujeme uzel pro operaci přiřazení
    AST_initNewBinOpNode(assignOpNode, AST_OP_ASSIGNMENT, leftOperand, rightOperand);

    // Vytvoříme uzel výrazu pro přiřazení  "="
    AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    if(assignExprNode == NULL) {
        AST_destroyNode(AST_BIN_OP_NODE, assignOpNode);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme uzel pro výraz s přiřazením
    AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignOpNode);

    // Vytvoříme uzel příkazu pro definici proměnné
    AST_StatementNode *varDefNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if(varDefNode == NULL) {
        AST_destroyNode(AST_EXPR_NODE, assignExprNode);
        error_handle(ERROR_INTERNAL);
        return PARSING_SYNTAX_ERROR;
    }

    // Inicializujeme uzel pro příkaz výrazem s přiřazením
    AST_initNewStatementNode(varDefNode, frameStack.currentID, AST_STATEMENT_VAR_DEF, assignExprNode);

    // Vracíme uzel příkazu definice proměnné
    return varDefNode;
} // LLparser_parseVarDef

// <MODIFIABLE> -> var | const
void LLparser_parseModifiable(bool *isModifiable) {
    if(isModifiable == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud příslušné pravidlo nebylo nalezeno, vracíme NULL
    if(!LLtable_findRule(currentToken.LLterminal, NT_MODIFIABLE, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        *isModifiable = PARSING_SYNTAX_ERROR;
        return;
    }

    // Na <MODIFIABLE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <MODIFIABLE> -> var
        case MODIFIABLE_1:
            // Parsujeme "var"
            if(currentToken.LLterminal != T_VAR) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme true, jelikož "var" je modifikovatelná
            *isModifiable = true;
            return;

        // <MODIFIABLE> -> const
        case MODIFIABLE_2:
            // Parsujeme "const"
            if(currentToken.LLterminal != T_CONST) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme false, jelikož "const" není modifikovatelná
            *isModifiable = false;
            return;

        // Jinak došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return;
    }
} // LLparser_parseModifiable()

// <POSSIBLE_TYPE> -> : <DATA_TYPE> | ε
AST_DataType LLparser_parsePossibleType() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud příslušné pravidlo nebylo nalezeno, vracíme NULL
    if(!LLtable_findRule(currentToken.LLterminal, NT_POSSIBLE_TYPE, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return AST_DATA_TYPE_NOT_DEFINED;
    }

    // Na <POSSIBLE_TYPE> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <POSSIBLE_TYPE> -> : <DATA_TYPE>
        case POSSIBLE_TYPE_1:
            // Parsujeme ":"
            if(currentToken.LLterminal != T_COLON) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return AST_DATA_TYPE_NOT_DEFINED;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme a vracíme datový typ proměnné
            return LLparser_parseDataType();

        // <POSSIBLE_TYPE> -> ε
        case POSSIBLE_TYPE_2:
            // Datový typ není uveden a tedy vracíme nedefinovaný datový typ
            return AST_DATA_TYPE_NOT_DEFINED;

        // Jinak došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return AST_DATA_TYPE_NOT_DEFINED;
    }
} // LLparser_parsePossibleType()

// <STATEMENT_REST> -> = [precedence_expression] | ( <ARGUMENTS> )
AST_StatementNode *LLparser_parseStatementRest(DString *identifier) {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;

    // Pokud příslušné pravidlo nebylo nalezeno, vracíme NULL
    if(!LLtable_findRule(currentToken.LLterminal, NT_STATEMENT_REST, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Na <STATEMENT_REST> lze aplikovat dvě různá pravidla
    switch(rule) {
        // <STATEMENT_REST> -> = [precedence_expression]
        case STATEMENT_REST_1: {
            // Parsujeme "="
            if(currentToken.LLterminal != T_ASSIGNMENT) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }
            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme [precedence_expression]
            AST_ExprNode *expr = PrecParser_parse(NT_STATEMENT_REST);
            if(expr == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
                string_free(identifier);
                return PARSING_SYNTAX_ERROR;
            }

            // Vyhledáme proměnnou v tabulce symbolů (již by měla být obsažena)
            SymtableItem *varItem = NULL;
            frame_stack_result result = frameStack_findItem(identifier, &varItem);
            if(result != FRAME_STACK_SUCCESS) {
                string_free(identifier);
                AST_destroyNode(AST_EXPR_NODE, expr);
                error_handle(ERROR_SEM_UNDEF);
            }

            // Pokud obsažena byla nastavíme, že byla změněna
            varItem->changed = true;

            // Vytvoříme uzel pro proměnnou
            AST_VarNode *varNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
            if(varNode == NULL) {
                string_free(identifier);
                AST_destroyNode(AST_EXPR_NODE, expr);
                error_handle(ERROR_INTERNAL);
            }

            // Inicializujeme uzel pro proměnnou
            AST_initNewVarNode(varNode, AST_VAR_NODE, identifier, frameStack.currentID, AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

            // Vytvoříme uzel operaci přiřazení
            AST_BinOpNode *assignOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
            if(assignOpNode == NULL) {
                AST_destroyNode(AST_VAR_NODE, varNode);
                AST_destroyNode(AST_EXPR_NODE, expr);
                error_handle(ERROR_INTERNAL);
                return PARSING_SYNTAX_ERROR;
            }

            // Vytvoříme uzel výrazu pro cíl přiřazení
            AST_ExprNode *leftOperand = (AST_ExprNode *)AST_createNode(AST_BIN_OP_NODE);
            if(leftOperand == NULL) {
                AST_destroyNode(AST_VAR_NODE, varNode);
                AST_destroyNode(AST_EXPR_NODE, expr);
                error_handle(ERROR_INTERNAL);
            }

            //Inicializujeme uzel pro levý operand
            AST_initNewExprNode(leftOperand, AST_EXPR_VARIABLE, varNode);

            AST_initNewBinOpNode(assignOpNode, AST_OP_ASSIGNMENT, leftOperand, expr);

            // Vytvoříme uzel výrazu pro přiřazení
            AST_ExprNode *assignExprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
            if(assignExprNode == NULL) {
                AST_destroyNode(AST_BIN_OP_NODE, assignOpNode);
                error_handle(ERROR_INTERNAL);
                return PARSING_SYNTAX_ERROR;
            }

            // Inicializujeme uzel pro výraz přiřazení
            AST_initNewExprNode(assignExprNode, AST_EXPR_BINARY_OP, assignOpNode);

            // Vytvoříme uzel příkazu
            AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
            if(statementNode == NULL) {
                AST_destroyNode(AST_EXPR_NODE, assignExprNode);
                error_handle(ERROR_INTERNAL);
                return PARSING_SYNTAX_ERROR;
            }

            // Inicializujeme uzel pro příkaz
            AST_initNewStatementNode(statementNode, frameStack.currentID, AST_STATEMENT_EXPR, assignExprNode);

            // Vracíme uzel příkazu
            return statementNode;
        }

        // <STATEMENT_REST> -> ( <ARGUMENTS> )
        case STATEMENT_REST_2:
        {
            // Parsujeme "("
            if (currentToken.LLterminal != T_LEFT_BRACKET) {
                string_free(identifier);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return NULL;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Parsujeme <ARGUMENTS>
            AST_ArgOrParamNode *arguments = LLparser_parseArguments();
            if (arguments == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
                string_free(identifier);
                return NULL;
            }

            // Parsujeme ")"
            if (currentToken.LLterminal != T_RIGHT_BRACKET) {
                string_free(identifier);
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return NULL;
            }
            Parser_getNextToken(LL_PARSER);

            // Vytvoříme uzel pro volání funkce
            AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
            if (funCallNode == NULL) {
                string_free(identifier);
                error_handle(ERROR_INTERNAL);
            }

            // Inicializujeme uzel pro volání funkce
            AST_initNewFunCallNode(funCallNode, identifier, false, arguments);

            // Vytvoříme uzel příkazu
            AST_StatementNode *statementNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
            if(statementNode == NULL) {
                AST_destroyNode(AST_FUN_CALL_NODE, funCallNode);
                error_handle(ERROR_INTERNAL);
                return PARSING_SYNTAX_ERROR;
            }

            // Inicializujeme uzel pro příkaz voláním funkce
            AST_initNewStatementNode(statementNode, frameStack.currentID, AST_STATEMENT_FUN_CALL, funCallNode);

            // Vracíme uzel příkazu
            return statementNode;
        }

        // Jinak došlo k syntaktické chybě
        default:
            string_free(identifier);
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    }
} // LLparser_parseStatementRest

AST_ExprNode *LLparser_parseThrowAway() {
    return PrecParser_parse(NT_THROW_AWAY);
}

AST_IfNode *LLparser_parseIf() {
    // Parsujeme "if"
    if (currentToken.LLterminal != T_IF) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "("
    if (currentToken.LLterminal != T_LEFT_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Vytváříme uzel výrazu a parsujeme [precedence_expression] (čili podmínku if)
    AST_ExprNode *condition = PrecParser_parse(NT_IF);
    if(condition == NULL) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme ")"
    if (currentToken.LLterminal != T_RIGHT_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        AST_destroyNode(AST_EXPR_NODE, condition);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme <NULL_COND>
    AST_VarNode *nullCond = LLparser_parseNullCond();
    if (Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        return PARSING_SYNTAX_ERROR;
    }

    // Pushneme nový rámec pro "if" blok
    frameStack_push(false);

    // Vytváříme uzel pro sezna příkazů a parsujeme <SEQUENCE>
    AST_StatementNode *thenBranch = LLparser_parseSequence();
    if (thenBranch == NULL) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Popneme rámec pro "if" blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        error_handle(ERROR_INTERNAL);
    }

    // Parsujeme "else"
    if (currentToken.LLterminal != T_ELSE) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        AST_destroyNode(AST_STATEMENT_NODE, thenBranch);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }
    Parser_getNextToken(LL_PARSER);

    // Pushneme nový rámec pro "else" blok
    frameStack_push(false);

    // Vytváříme uzel pro seznam příkazů a parsujeme <SEQUENCE>
    AST_StatementNode *elseBranch = LLparser_parseSequence();
    if(elseBranch == NULL) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        AST_destroyNode(AST_STATEMENT_NODE, thenBranch);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Popneme rámec pro "else" blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme uzel pro "if" příkaz
    AST_IfNode *ifNode = (AST_IfNode *)AST_createNode(AST_IF_NODE);
    if (ifNode == NULL) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        AST_destroyNode(AST_STATEMENT_NODE, thenBranch);
        AST_destroyNode(AST_STATEMENT_NODE, elseBranch);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializace uzlu pro příkaz konstrukce "if"
    AST_initNewIfNode(ifNode, condition, nullCond, thenBranch, elseBranch);

    // Vracíme uzel pro konstrukci "if"
    return ifNode;
} // LLparser_parseIf()

AST_VarNode *LLparser_parseNullCond() {
    // Parsujeme "|"
    if (currentToken.LLterminal != T_PIPE) {
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

    // Zkontrolujeme, že se nejedná o zastínění (sémantická chyba 5)
    SymtableItem *item = NULL;
    frame_stack_result result = frameStack_findItem(currentToken.value, &item);
    if(result != FRAME_STACK_SUCCESS) {
        error_handle(ERROR_SEM_UNDEF);
    }
    item->used = true;

    // Uchovávme hodnotu identifikátoru
    DString *identifier = currentToken.value;

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Vytvoříme uzel příkazu
    AST_VarNode *nullCond = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
    if(nullCond == NULL) {
        string_free(identifier);
        error_handle(ERROR_INTERNAL);
        return PARSING_SYNTAX_ERROR;
    }

    // Inicializujeme uzel pro příkaz voláním funkce
    AST_initNewVarNode(nullCond, AST_VAR_NODE, identifier, frameStack.currentID, AST_LITERAL_NOT_DEFINED, AST_VAL_UNDEFINED);

    // Parsujeme "|"
    if (currentToken.LLterminal != T_PIPE) {
        AST_destroyNode(AST_VAR_NODE, nullCond);
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Vracíme uzel příkazu
    return nullCond;
} // LLparser_parseNullCond()

// <SEQUENCE> -> { <STATEMENT_LIST> }
AST_StatementNode *LLparser_parseSequence() {
    // Parsujeme "{"
    if(currentToken.LLterminal != T_LEFT_CURLY_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Pushneme nový rámec na zásobník rámců
    frameStack_push(false);

    // Vytvoříme si uzel reprezentující seznam příkazů a parsujeme <STATEMENT_LIST>
    AST_StatementNode *statementList = LLparser_parseStatementList();

    // Zkontrolujeme úspěšnost parsování <STATEMENT_LIST>
    if(statementList == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme "}"
    if(currentToken.LLterminal != T_RIGHT_CURLY_BRACKET) {
        AST_destroyNode(AST_STATEMENT_NODE, statementList);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Popneme rámec pro tuto sekvenci příkazů
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        error_handle(ERROR_INTERNAL);
    }

    // Vracíme sekvenci příkazů
    return statementList;
} // LLparser_parseSequence()

AST_WhileNode *LLparser_parseWhile() {
    // Parsujeme "while"
    if (currentToken.LLterminal != T_WHILE) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme "("
    if (currentToken.LLterminal != T_LEFT_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme od další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme [precedence_expression]
    AST_ExprNode *condition = PrecParser_parse(NT_WHILE);
    if (condition == NULL) {
        return PARSING_SYNTAX_ERROR;
    }

    // Parsujeme ")"
    if (currentToken.LLterminal != T_RIGHT_BRACKET) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        AST_destroyNode(AST_EXPR_NODE, condition);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme <NULL_COND>
    AST_VarNode *nullCond = LLparser_parseNullCond();
    if (nullCond == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        return PARSING_SYNTAX_ERROR;
    }

    // Pushneme nový rámec pro "while" blok
    frameStack_push(false);

    // Parsujeme <SEQUENCE>
    AST_StatementNode *body = LLparser_parseSequence();
    if (body == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
            error_handle(ERROR_INTERNAL);
        }
        return PARSING_SYNTAX_ERROR;
    }

    // Popneme rámec pro 'while' blok
    if(frameStack_pop() == FRAME_STACK_POP_GLOBAL) {
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme uzel pro 'while' příkaz
    AST_WhileNode *whileNode = (AST_WhileNode *)AST_createNode(AST_WHILE_NODE);
    if (whileNode == NULL) {
        AST_destroyNode(AST_EXPR_NODE, condition);
        AST_destroyNode(AST_STATEMENT_NODE, body);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme uzel pro cyklus while
    AST_initNewWhileNode(whileNode, condition, nullCond, body);

    // Vracíme uzel pro cyklus while
    return whileNode;
} // LLparser_parseWhile()

AST_StatementNode *LLparser_parseReturn() {
    // Parsujeme "return"
    if (currentToken.LLterminal != T_RETURN) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Žádáme o další token
    Parser_getNextToken(LL_PARSER);

    // Parsujeme <RETURN_REST>
    AST_ExprNode *expr = LLparser_parseReturnRest();
    if (expr == NULL && Parser_watchSyntaxError(IS_SYNTAX_ERROR)) {
        return PARSING_SYNTAX_ERROR;
    }

    // Vytvoříme uzel pro příkaz "return"
    AST_StatementNode *returnNode = (AST_StatementNode *)AST_createNode(AST_STATEMENT_NODE);
    if (returnNode == NULL) {
        if (expr != NULL) {
            AST_destroyNode(AST_EXPR_NODE, expr);
        }
        error_handle(ERROR_INTERNAL);
    }

    // Incializujeme uzel pro návratový příkaz
    AST_initNewStatementNode(returnNode, frameStack.currentID, AST_STATEMENT_RETURN, expr);

    return returnNode;
} // LLparser_parseReturn()


// <RETURN_REST> -> [precedence_expression] | ε
AST_ExprNode *LLparser_parseReturnRest() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    if (!LLtable_findRule(currentToken.LLterminal, NT_RETURN_REST, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Na <RETURN_REST> lze aplikovat dvě různá pravidla
    switch (rule) {
        // <RETURN_REST> -> [precedence_expression]
        case RETURN_REST_1:
            return PrecParser_parse(NT_RETURN_REST);

        // <RETURN_REST> -> ε
        case RETURN_REST_2:
            return NULL;    // Rozvíjíme na epsilon, ted NULL je korektní hodnotou

        // Jinak dochází k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    }
} // LLparser_parseReturnRest()

// <ARGUMENTS> -> <ARG_LIST> | ε
AST_ArgOrParamNode *LLparser_parseArguments() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    if (!LLtable_findRule(currentToken.LLterminal, NT_ARGUMENTS, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return NULL;
    }

    // Na <ARGUMENTS> lze aplikovat dvě různá pravidla
    switch (rule) {
        // <ARGUMENTS> -> <ARG_LIST>
        case ARGUMENTS_1:
            return LLparser_parseArgList();

        // <ARGUMENTS> -> ε
        case ARGUMENTS_2:
            return NULL;    // Rozvíjíme na epsilon, ted NULL je korektní hodnotou

        // Jinak došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    }
} // LLparser_parseArguments()

AST_ArgOrParamNode *LLparser_parseArgList() {
    // Parsujeme [precedence_expression]
    AST_ExprNode *expr = PrecParser_parse(NT_ARG_LIST);
    if (expr == NULL) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Vytvoříme uzel pro argument
    AST_ArgOrParamNode *argNode = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    if (argNode == NULL) {
        AST_destroyNode(AST_EXPR_NODE, expr);
        error_handle(ERROR_INTERNAL);
    }

    // Inicializujeme uzel pro argument funkce
    AST_initNewArgOrParamNode(argNode, AST_DATA_TYPE_NOT_DEFINED, expr);

    // Parsujeme zbytek argumentů
    AST_ArgOrParamNode *restArgs = LLparser_parseArg();
    if(Parser_watchSyntaxError(IS_SYNTAX_ERROR)){
        AST_destroyNode(AST_ARG_OR_PARAM_NODE, argNode);
        return PARSING_SYNTAX_ERROR;
    }

    if (restArgs != NULL) {
        argNode->next = restArgs;
    }

    return argNode;
} // LLparser_parseArgList()

// <ARG> -> , <ARG_LIST> | ε
AST_ArgOrParamNode *LLparser_parseArg() {
    // Vyhledáme pravidlo v LL tabulce
    LLRuleSet rule = RULE_UNDEFINED;
    if (!LLtable_findRule(currentToken.LLterminal, NT_ARG, &rule)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        return PARSING_SYNTAX_ERROR;
    }

    // Na <ARG> lze aplikovat dvě různá pravidla
    switch (rule) {
        // <ARG> -> , <ARG_LIST>
        case ARG_1:
            // Parsujeme ","
            if (currentToken.LLterminal != T_COMMA) {
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                return PARSING_SYNTAX_ERROR;
            }

            // Žádáme o další token
            Parser_getNextToken(LL_PARSER);

            // Vracíme aktualizovaný seznam argumentů
            return LLparser_parseArgList();

        // <ARG> -> ε
        case ARG_2:
            return NULL;    // Epsilon se rozvíjí na NULL, což je korektní

        // Jinak došlo k syntaktické chybě
        default:
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            return PARSING_SYNTAX_ERROR;
    }
} // LLparser_parseArg()

/*** Konec souboru llparser.c ***/
