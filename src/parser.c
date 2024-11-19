/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser.c                                                  *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
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
 * @file parser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace sdílených funkcí a proměnných pro parser.
 * @details Tento soubor implementuje sdílené funkce a globální proměnné, které
 *          ostatní moduly parseru využívají k syntaktické analýze. Umožňuje
 *          správu aktuálního tokenu a vytváření abstraktního syntaktického
 *          stromu (AST), což jsou základní komponenty potřebné pro práci parseru.
 */

#include "parser.h"

/*******************************************************************************
 *                                                                             *
 *                POČÁTEČNÍ INICIALIZACE GLOBÁLNÍCH PROMĚNNÝCH                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro aktuální token, který je zpracováván.
 */
Terminal currentToken = { T_UNDEFINED, T_PREC_UNDEFINED, NULL};   // počáteční inicializace

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot = NULL;                // počáteční inicializace

/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální současný token.
 */
Terminal Parser_getNextToken(GetNextTokenState state) {
    // Static proměnná pro uchování lookahead tokenu
    static Terminal lookaheadToken = { T_UNDEFINED, T_PREC_UNDEFINED, NULL };

    // Reset signál pro statickou proměnnou
    if(state == RESET_LOOKAHEAD) {
        lookaheadToken.LLterminal = T_UNDEFINED;
        lookaheadToken.PrecTerminal = T_PREC_UNDEFINED;
        lookaheadToken.value = NULL;
        return lookaheadToken;
    }

    // Pokud lookahead token není inicializován, načti ho
    if(lookaheadToken.LLterminal == T_UNDEFINED && lookaheadToken.PrecTerminal == T_PREC_UNDEFINED) {
        lookaheadToken = Parser_pokeScanner(state);
    }

    // Aktualizuj currentToken na aktuální lookaheadToken
    currentToken = lookaheadToken;

    // Načti další lookahead token voláním scanneru
    lookaheadToken = Parser_pokeScanner(state);

    return lookaheadToken;
} // Parser_getNextToken()

/**
 * @brief Nastaví nebo zkontroluje stav syntax error.
 */
bool Parser_watchSyntaxError(PropagateError state) {
    // Počáteční inicializace flagu pro sledování SYNTAX_ERROR
    static bool syntaxError = false;

    // Reset signál pro statickou proměnnou
    if(state == RESET_ERROR_FLAG) {
        syntaxError = false;
    }

    // Pokud byl předán true, signalizuj flagem SYNTAX_ERROR
    if(state == SET_SYNTAX_ERROR) {
        syntaxError = true;
    }

    // Vracíme aktuální stav flagu
    return syntaxError;
} // Parser_watchSyntaxError()

/**
 * @brief Přidá suffix k jménu proměnné ve formátu `$frameID$`.
 */
void Parser_addIdSuffix(size_t frameID, DString *id) {
    if(id == NULL || id->str == NULL) {
        error_handle(ERROR_INTERNAL);
        return;
    }

    // Dočasný C řetězec o velikost maximálního počtu číslic 'size_t'
    char suffix[MAX_SIZE_T_DIGITS] = "\0";

    // Převod hodnoty "frameID" na znaky
    snprintf(suffix, MAX_SIZE_T_DIGITS*sizeof(char), "%zu", frameID);

    // Přidání uvozovacího oddělovače frameID od indetifikátoru
    int error = string_append_char(id, FRAME_ID_SEPARATOR);

    // Přípojení hodnoty "frameID" jako suffix identifikátoru
    for(int i = 0; i < (int)strlen(suffix); i++) {
        error = string_append_char(id, suffix[i]);
        Parser_checkAppendSuccess(error);
    }

    // Přidání ukončovacího oddělovače frameID od indetifikátoru
    error = string_append_char(id, FRAME_ID_SEPARATOR);
    Parser_checkAppendSuccess(error);
} // Parser_addIdSuffix()


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Kontroluje úspěšnost připojení znaku do DString a případně hlásí chybu.
 */
inline void Parser_checkAppendSuccess(int error) {
    // Pokud připojení znaku selhalo, hlásíme interní chybu
    if(error == STRING_RESIZE_FAIL) {
        error_handle(ERROR_INTERNAL);
    }
} // parser_checkAppendSuccess()

/**
 * @brief Získá další token ze scanneru a namapuje ho na typ LL terminálu.
 */
Terminal Parser_pokeScanner(GetNextTokenState state) {
    Token receivedToken = scanner_getNextToken();
    LLTerminals llType = T_UNDEFINED;
    PrecTerminals precType = T_PREC_UNDEFINED;
    Terminal terminal;

    (void)state;
    Parser_mapTokenToLLTerminal(receivedToken.type, &llType);
    Parser_mapTokenToPrecTerminal(receivedToken.type, &precType);

    terminal.LLterminal = llType;
    terminal.PrecTerminal = precType;
    terminal.value = receivedToken.value;

    return terminal;
} // Parser_pokeScanner()

/**
 * @brief Namapuje typ tokenu na typ LL terminálu.
 */
void Parser_mapTokenToLLTerminal(TokenType tokenType, LLTerminals *terminalType) {
    // Ověření platnosti předaného ukazatele
    if(terminalType == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Mapujeme typ Tokenu na typ LL Terminálu
    switch(tokenType) {
        // Mapování: TOKEN_IDENTIFIER -> T_ID
        case TOKEN_IDENTIFIER:
            *terminalType = T_ID;
            break;

        // Mapování: TOKEN_K_import -> T_IMPORT
        case TOKEN_K_import:
            *terminalType = T_IMPORT;
            break;

        // Mapování: TOKEN_K_ifj -> T_IFJ
        case TOKEN_K_ifj:
            *terminalType = T_IFJ;
            break;

        // Mapování: TOKEN_K_pub -> T_PUB
        case TOKEN_K_pub:
            *terminalType = T_PUB;
            break;

        // Mapování: TOKEN_K_fn -> T_FN
        case TOKEN_K_fn:
            *terminalType = T_FN;
            break;

        // Mapování: TOKEN_K_const -> T_CONST
        case TOKEN_K_const:
            *terminalType = T_CONST;
            break;

        // Mapování: TOKEN_K_var -> T_VAR
        case TOKEN_K_var:
            *terminalType = T_VAR;
            break;

        // Mapování: TOKEN_K_if -> T_IF
        case TOKEN_K_if:
            *terminalType = T_IF;
            break;

        // Mapování: TOKEN_K_else -> T_ELSE
        case TOKEN_K_else:
            *terminalType = T_ELSE;
            break;

        // Mapování: TOKEN_K_while -> T_WHILE
        case TOKEN_K_while:
            *terminalType = T_WHILE;
            break;

        // Mapování: TOKEN_K_return -> T_RETURN
        case TOKEN_K_return:
            *terminalType = T_RETURN;
            break;

        // Mapování: TOKEN_EQUALITY_SIGN -> T_ASSIGNMENT
        case TOKEN_EQUALITY_SIGN:
            *terminalType = T_ASSIGNMENT;
            break;

        // Mapování: TOKEN_K_i32 -> T_INT
        case TOKEN_K_i32:
            *terminalType = T_INT;
            break;

        // Mapování: TOKEN_K_Qi32 -> T_INT_OR_NULL
        case TOKEN_K_Qi32:
            *terminalType = T_INT_OR_NULL;
            break;

        // Mapování: TOKEN_K_f64 -> T_FLOAT
        case TOKEN_K_f64:
            *terminalType = T_FLOAT;
            break;

        // Mapování: TOKEN_K_Qf64 -> T_FLOAT_OR_NULL
        case TOKEN_K_Qf64:
            *terminalType = T_FLOAT_OR_NULL;
            break;

        // Mapování: TOKEN_K_u8 -> T_STRING
        case TOKEN_K_u8:
            *terminalType = T_STRING;
            break;

        // Mapování: TOKEN_K_Qu8 -> T_STRING_OR_NULL
        case TOKEN_K_Qu8:
            *terminalType = T_STRING_OR_NULL;
            break;

        // Mapování: TOKEN_K_void -> T_VOID
        case TOKEN_K_void:
            *terminalType = T_VOID;
            break;

        // Mapování: TOKEN_K_underscore -> T_DUMP
        case TOKEN_K_underscore:
            *terminalType = T_DUMP;
            break;

        // Mapování: TOKEN_PERIOD -> T_DOT
        case TOKEN_PERIOD:
            *terminalType = T_DOT;
            break;

        // Mapování: TOKEN_COMMA -> T_COMMA
        case TOKEN_COMMA:
            *terminalType = T_COMMA;
            break;

        // Mapování: TOKEN_COLON -> T_COLON
        case TOKEN_COLON:
            *terminalType = T_COLON;
            break;

        // Mapování: TOKEN_SEMICOLON -> T_SEMICOLON
        case TOKEN_SEMICOLON:
            *terminalType = T_SEMICOLON;
            break;

        // Mapování: TOKEN_VERTICAL_BAR -> T_PIPE
        case TOKEN_VERTICAL_BAR:
            *terminalType = T_PIPE;
            break;

        // Mapování: TOKEN_LEFT_PARENTHESIS -> T_LEFT_BRACKET
        case TOKEN_LEFT_PARENTHESIS:
            *terminalType = T_LEFT_BRACKET;
            break;

        // Mapování: TOKEN_RIGHT_PARENTHESIS -> T_RIGHT_BRACKET
        case TOKEN_RIGHT_PARENTHESIS:
            *terminalType = T_RIGHT_BRACKET;
            break;

        // Mapování: TOKEN_LEFT_CURLY_BRACKET -> T_LEFT_CURLY_BRACKET
        case TOKEN_LEFT_CURLY_BRACKET:
            *terminalType = T_LEFT_CURLY_BRACKET;
            break;

        // Mapování: TOKEN_RIGHT_CURLY_BRACKET -> T_RIGHT_CURLY_BRACKET
        case TOKEN_RIGHT_CURLY_BRACKET:
            *terminalType = T_RIGHT_CURLY_BRACKET;
            break;

        // Mapování: TOKEN_EOF -> T_EOF
        case TOKEN_EOF:
            *terminalType = T_EOF;
            break;

        // Cokoliv jiného značí lexikální chybu
        default:
            *terminalType = T_UNDEFINED;
            break;
    } // switch()
} // Parser_mapTokenToLLTerminal()

/**
 * @brief Namapuje typ tokenu na typ precedenčního terminálu.
 */
void Parser_mapTokenToPrecTerminal(TokenType tokenType, PrecTerminals *terminal) {
    // Ověření platnosti předaného ukazatele
    if (terminal == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    switch(tokenType) {
        // Mapování: TOKEN_IDENTIFIER -> T_PREC_ID
        case TOKEN_IDENTIFIER:
            *terminal = T_PREC_ID;
            break;

        // Mapování: TOKEN_INT -> T_PREC_INT_LITERAL
        case TOKEN_INT:
            *terminal = T_PREC_INT_LITERAL;
            break;

        // Mapování: TOKEN_FLOAT -> T_PREC_FLOAT_LITERAL
        case TOKEN_FLOAT:
            *terminal = T_PREC_FLOAT_LITERAL;
            break;

        // Mapování: TOKEN_STRING -> T_PREC_STRING_LITERAL
        case TOKEN_STRING:
            *terminal = T_PREC_STRING_LITERAL;
            break;

        // Mapování: TOKEN_K_null -> T_PREC_NULL_LITERAL
        case TOKEN_K_null:
            *terminal = T_PREC_NULL_LITERAL;
            break;

        // Mapování: TOKEN_K_ifj -> T_PREC_IFJ
        case TOKEN_K_ifj:
            *terminal = T_PREC_IFJ;
            break;

        // Mapování: TOKEN_PERIOD -> T_PREC_DOT
        case TOKEN_PERIOD:
            *terminal = T_PREC_DOT;
            break;

        // Mapování: TOKEN_LEFT_PARENTHESIS -> T_PREC_LEFT_BRACKET
        case TOKEN_LEFT_PARENTHESIS:
            *terminal = T_PREC_LEFT_BRACKET;
            break;

        // Mapování: TOKEN_RIGHT_PARENTHESIS -> T_PREC_RIGHT_BRACKET nebo T_PREC_DOLLAR
        case TOKEN_RIGHT_PARENTHESIS:
            *terminal = T_PREC_RIGHT_BRACKET;
            break;

        // Mapování: TOKEN_PLUS -> T_PREC_PLUS
        case TOKEN_PLUS:
            *terminal = T_PREC_PLUS;
            break;

        // Mapování: TOKEN_MINUS -> T_PREC_MINUS
        case TOKEN_MINUS:
            *terminal = T_PREC_MINUS;
            break;

        // Mapování: TOKEN_ASTERISK -> T_PREC_MULTIPLICATION
        case TOKEN_ASTERISK:
            *terminal = T_PREC_MULTIPLICATION;
            break;

        // Mapování: TOKEN_SLASH -> T_PREC_DIVISION
        case TOKEN_SLASH:
            *terminal = T_PREC_DIVISION;
            break;

        // Mapování: TOKEN_EQUAL_TO -> T_PREC_IDENTITY
        case TOKEN_EQUAL_TO:
            *terminal = T_PREC_IDENTITY;
            break;

        // Mapování: TOKEN_NOT_EQUAL_TO -> T_PREC_NOT_EQUAL
        case TOKEN_NOT_EQUAL_TO:
            *terminal = T_PREC_NOT_EQUAL;
            break;

        // Mapování: TOKEN_LESS_THAN -> T_PREC_LESS_THAN
        case TOKEN_LESS_THAN:
            *terminal = T_PREC_LESS_THAN;
            break;

        // Mapování: TOKEN_GREATER_THAN -> T_PREC_GREATER_THAN
        case TOKEN_GREATER_THAN:
            *terminal = T_PREC_GREATER_THAN;
            break;

        // Mapování: TOKEN_LESS_EQUAL_THAN -> T_PREC_LESS_THAN_OR_EQUAL
        case TOKEN_LESS_EQUAL_THAN:
            *terminal = T_PREC_LESS_THAN_OR_EQUAL;
            break;

        // Mapování: TOKEN_GREATER_EQUAL_THAN -> T_PREC_GREATER_THAN_OR_EQUAL
        case TOKEN_GREATER_EQUAL_THAN:
            *terminal = T_PREC_GREATER_THAN_OR_EQUAL;
            break;

        // Mapování: TOKEN_COMMA -> T_PREC_COMMA nebo T_PREC_DOLLAR
        case TOKEN_COMMA:
            *terminal = T_PREC_COMMA;
            break;

        // Mapování: TOKEN_SEMICOLON -> T_PREC_DOLLAR
        case TOKEN_SEMICOLON:
            *terminal = T_PREC_DOLLAR;
            break;

        // Defaultní případ: syntaktická chyba
        default:
            *terminal = T_PREC_UNDEFINED;
            break;
    } // switch()
} // Parser_mapTokenToPrecTerminal()


/**
 * @brief Namapuje typ datového typu AST na typ návratového typu tabulky symbolů.
 *
 * @param astDataType Typ datového typu AST.
 * @param symtableType Ukazatel na typ návratového typu tabulky symbolů.
 */
void Parser_mapASTDataTypeToFunReturnType(AST_DataType astDataType, symtable_functionReturnType *symtableType) {
    // Ověření platnosti předaného ukazatele
    if (symtableType == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Mapujeme typ datového typu AST na typ návratového typu tabulky symbolů
    switch (astDataType) {
        case AST_DATA_TYPE_INT:
            *symtableType = SYMTABLE_TYPE_INT;
            break;

        case AST_DATA_TYPE_INT_OR_NULL:
            *symtableType = SYMTABLE_TYPE_INT_OR_NULL;
            break;

        case AST_DATA_TYPE_FLOAT:
            *symtableType = SYMTABLE_TYPE_DOUBLE;
            break;

        case AST_DATA_TYPE_FLOAT_OR_NULL:
            *symtableType = SYMTABLE_TYPE_DOUBLE_OR_NULL;
            break;

        case AST_DATA_TYPE_STRING:
            *symtableType = SYMTABLE_TYPE_STRING;
            break;

        case AST_DATA_TYPE_STRING_OR_NULL:
            *symtableType = SYMTABLE_TYPE_STRING_OR_NULL;
            break;

        case AST_DATA_TYPE_VOID:
            *symtableType = SYMTABLE_TYPE_VOID;
            break;

        default:
            *symtableType = SYMTABLE_TYPE_UNKNOWN;
            break;
    } // switch()
} // Parser_mapASTDataTypeToSymtableType()

/**
 * @brief Namapuje typ datového typu AST na stav symbolu v tabulce symbolů.
 *
 * @param astDataType Typ datového typu AST.
 * @param symtableState Ukazatel na stav symbolu v tabulce symbolů.
 */
void Parser_mapASTDataTypeToSymtableState(AST_DataType astDataType, symtable_symbolState *symtableState) {
    // Ověření platnosti předaného ukazatele
    if (symtableState == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Mapujeme typ datového typu AST na stav symbolu v tabulce symbolů
    switch (astDataType) {
        case AST_DATA_TYPE_INT:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_INT;
            break;

        case AST_DATA_TYPE_INT_OR_NULL:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL;
            break;

        case AST_DATA_TYPE_FLOAT:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_DOUBLE;
            break;

        case AST_DATA_TYPE_FLOAT_OR_NULL:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL;
            break;

        case AST_DATA_TYPE_STRING:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_STRING;
            break;

        case AST_DATA_TYPE_STRING_OR_NULL:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL;
            break;

        case AST_DATA_TYPE_VOID:
            *symtableState = SYMTABLE_SYMBOL_FUNCTION; // Funkce vracející void
            break;

        default:
            *symtableState = SYMTABLE_SYMBOL_UNKNOWN;
            break;
    } // switch()
} // Parser_mapASTDataTypeToSymtableState()


/*** Konec souboru parser.c ***/
