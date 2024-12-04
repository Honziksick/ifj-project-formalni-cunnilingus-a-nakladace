/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser_common.c                                           *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   24.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file parser_common.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementační soubor pro sdílené funkce a proměnné parseru.
 * @details Tento soubor obsahuje implementace funkcí a definice proměnných
 *          deklarovaných v `parser_common.h`. Tyto funkce a proměnné jsou
 *          využívány různými moduly parseru při syntaktické analýze jazyka
 *          IFJ24. Soubor zahrnuje správu tokenů, chybového stavu a mapování
 *          typů pro parser.
 */

#include "parser_common.h"


/*******************************************************************************
 *                                                                             *
 *                POČÁTEČNÍ INICIALIZACE GLOBÁLNÍCH PROMĚNNÝCH                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro aktuální token, který je zpracováván.
 */
Terminal currentTerminal = { T_UNDEFINED, T_PREC_UNDEFINED, NULL };  // Počíteční inicializace

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot = NULL;  // Počíteční inicializace


/*******************************************************************************
 *                                                                             *
 *           IMPLEMENTACE VEŘEJNÝCH FUNKCÍ OBECNÉHO ROZHRANÍ PARSERU           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální současný token.
 */
void parser_getNextToken(bool state) {
    // Statická proměnná pro uchování lookahead terminálu
    static Terminal lookaheadTerminal = { T_UNDEFINED, T_PREC_UNDEFINED, NULL };

    // Resetování statické proměnné, kdyby po předchozím běhu zůstala stále v paměti
    if(state == RESET_LOOKAHEAD) {
        lookaheadTerminal.LLterminal = T_UNDEFINED;
        lookaheadTerminal.PrecTerminal = T_PREC_UNDEFINED;
        lookaheadTerminal.value = NULL;
        return;
    }

    // Pokud lookahead terminál není inicializován, načti ho
    if(lookaheadTerminal.LLterminal == T_UNDEFINED && \
       lookaheadTerminal.PrecTerminal == T_PREC_UNDEFINED)
    {
        lookaheadTerminal = parser_pokeScanner(state);
    }

    // Aktualizuj currentTerminal na aktuální lookaheadTerminal
    currentTerminal = lookaheadTerminal;

    // Načti nový lookahead token voláním scanneru
    lookaheadTerminal = parser_pokeScanner(state);
} // parser_getNextToken()

/**
 * @brief Nastaví nebo zkontroluje stav syntax error.
 */
bool parser_errorWatcherInternal(ParserErrorState state, const char *file, \
                                 int line, const char *func)
{
    // Statické flagy pro uchování chybových stavů
    static bool lexicalError  = false;
    static bool syntaxError   = false;
    static bool semUndefError = false;
    static bool semRedefError = false;
    static bool internalError = false;

    // Statická proměnná pro zaznamení první chyby
    static ErrorType firstError = SUCCESS;

    // Většinou jdeo nastavení chybového flagu a první chyby, pokud se o ní jedná
    switch(state) {
        // Resetování statické proměnné, kdyby po předchozím běhu zůstala stále v paměti
        case RESET_ERROR_FLAGS:
            lexicalError  = false;
            syntaxError   = false;
            semUndefError = false;
            semRedefError = false;
            internalError = false;
            firstError = SUCCESS;
            break;

        // Nic speciálního neprovádíme, zajímá nás čistě návratová hodnota funkce
        case IS_PARSING_ERROR:
            break;

        // Nastavení stavu lexikální chyby
        case SET_ERROR_LEXICAL:
            lexicalError = true;
            parser_updateFirstError(state, &firstError);
            LOG_ERROR(file, line, func);
            break;

        // Nastavení stavu syntaktické chyby
        case SET_ERROR_SYNTAX:
            syntaxError = true;
            parser_updateFirstError(state, &firstError);
            LOG_ERROR(file, line, func);
            break;

        // Nastavení stavu sémantické chyby - nedefinovaná funkce či proměnná.
        case SET_ERROR_SEM_UNDEF:
            semUndefError = true;
            parser_updateFirstError(state, &firstError);
            LOG_ERROR(file, line, func);
            break;

        // Nastavení stavu sémantické chyby - redefinice nebo přiřazení do nemodifikovatelné proměnné
        case SET_ERROR_SEM_REDEF_OR_CONSTDEF:
            semRedefError = true;
            parser_updateFirstError(state, &firstError);
            LOG_ERROR(file, line, func);
            break;

        // Nastavení stavu interní chyby překladače
        case SET_ERROR_INTERNAL:
            internalError = true;
            parser_updateFirstError(state, &firstError);
            LOG_ERROR(file, line, func);
            break;

        // Zavolá funkci pro řízení chybových stavů
        case CALL_ERROR_HANDLE:
            error_handle(firstError);
            break;

        // Pro jistotu uvedeme výchozí stav, který nic neprovádí
        default:
            break;
    } // switch()

    // Vracíme `true`, pokud došlo alespoň k jakékoliv jedné chybě
    return (lexicalError || syntaxError || semUndefError || semRedefError || internalError);
} // Parser_watchSyntaxError()

/**
 * @brief Uvolní hodnotu aktuálního terminálu.
 */
inline void parser_freeCurrentTerminalValue() {
    // Pokud hodnota aktuálního terminálu není NULL, uvolníme ji
    if(currentTerminal.value != NULL) {
        DString_free(currentTerminal.value);
        currentTerminal.value = NULL;        // inicializujeme ji na NULL
    }
} // LLparser_freeCurrentTerminalValue


/*******************************************************************************
 *                                                                             *
 *           IMPLEMENTACE INTERNÍCH FUNKCÍ OBECNÉHO ROZHRANÍ PARSERU           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Nastaví první zaznamenanou chybu.
 */
inline void parser_updateFirstError(ParserErrorState state, ErrorType *error) {
    // Pokud je proměnná error stále ve výchozím stavu
    if(*error == SUCCESS) {
        // Nastavíme do ní první vyskytnutý error
        switch(state) {
            // Mapujeme: SET_ERROR_LEXICAL -> ERROR_LEXICAL
            case SET_ERROR_LEXICAL:
                *error = ERROR_LEXICAL;
                break;

            // Mapujeme: SET_ERROR_SYNTAX -> ERROR_SYNTAX
            case SET_ERROR_SYNTAX:
                *error = ERROR_SYNTAX;
                break;

            // Mapujeme: SET_ERROR_SEM_UNDEF -> ERROR_SEM_UNDEF
            case SET_ERROR_SEM_UNDEF:
                *error = ERROR_SEM_UNDEF;
                break;

            // Mapujeme: SET_ERROR_SEM_REDEF_OR_CONSTDEF -> ERROR_SEM_REDEF_OR_CONSTDEF
            case SET_ERROR_SEM_REDEF_OR_CONSTDEF:
                *error = ERROR_SEM_REDEF_OR_CONSTDEF;
                break;

            // Mapujeme: SET_ERROR_INTERNAL -> ERROR_INTERNAL
            case SET_ERROR_INTERNAL:
                *error = ERROR_INTERNAL;
                break;

            // Jinak se nic nestane
            default:
                break;
        } // switch()
    } // if()
} // parser_updateFirstError()

/**
 * @brief Získá další token ze scanneru a namapuje ho na typ LL a precedenčního
 *        terminálu.
 */
Terminal parser_pokeScanner() {
    // Počáteční inicializace mapovaných hodnot a návratové struktury funkce
    LLTerminals llType = T_UNDEFINED;
    PrecTerminals precType = T_PREC_UNDEFINED;
    Terminal terminal = { T_UNDEFINED, T_PREC_UNDEFINED, NULL };

    // Požádáme scanner o další token
    Token receivedToken = scanner_getNextToken();

    // Namapujeme typ tokenu na příslušný tym LL a precedenčního terminálu
    parser_mapTokenToLLTerminal(receivedToken.type, &llType);
    parser_mapTokenToPrecTerminal(receivedToken.type, &precType);

    // Inicializujeme návratovou hodnotu obdrženými hodnotami
    terminal.LLterminal = llType;
    terminal.PrecTerminal = precType;
    terminal.value = receivedToken.value;  // jako value se využije přímo value Tokenu

    return terminal;
} // parser_pokeScanner()


/*******************************************************************************
 *                                                                             *
 *           DEKLARACE POMOCNÝCH INTERNÍCH MAPOVACÍCH FUNKCÍ PARSERU           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Namapuje typ tokenu na typ LL terminálu.
 */
void parser_mapTokenToLLTerminal(TokenType tokenType, LLTerminals *terminalType) {
    // Ověření platnosti předaného ukazatele
    if(terminalType == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
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

        // Pro jakýkoliv jiný typ tokenu není LL terminál definován
        default:
            *terminalType = T_UNDEFINED;  // Korektní, neznačí syntaktickou chybu
            break;
    } // switch()
} // parser_mapTokenToLLTerminal()

/**
 * @brief Namapuje typ tokenu na typ precedenčního terminálu.
 */
void parser_mapTokenToPrecTerminal(TokenType tokenType, PrecTerminals *terminal) {
    // Ověření platnosti předaného ukazatele
    if(terminal == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
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

        // Pro jakýkoliv jiný typ tokenu není LL terminál definován
        default:
            *terminal = T_PREC_UNDEFINED;  // Korektní, neznačí syntaktickou chybu
            break;
    } // switch()
} // parser_mapTokenToPrecTerminal()

/**
 * @brief Namapuje typ datového typu AST na typ návratového typu tabulky symbolů.
 */
void parser_mapASTDataTypeToFunReturnType(AST_DataType astDataType, \
                                          Symtable_functionReturnType *symtableType)
{
    // Ověření platnosti předaného ukazatele
    if(symtableType == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    // Mapujeme typ datového typu AST na typ návratového typu tabulky symbolů
    switch(astDataType) {
        // Mapování: AST_DATA_TYPE_INT -> SYMTABLE_TYPE_INT
        case AST_DATA_TYPE_INT:
            *symtableType = SYMTABLE_TYPE_INT;
            break;

        // Mapování: AST_DATA_TYPE_INT_OR_NULL -> SYMTABLE_TYPE_INT_OR_NULL
        case AST_DATA_TYPE_INT_OR_NULL:
            *symtableType = SYMTABLE_TYPE_INT_OR_NULL;
            break;

        // Mapování: AST_DATA_TYPE_FLOAT -> SYMTABLE_TYPE_DOUBLE
        case AST_DATA_TYPE_FLOAT:
            *symtableType = SYMTABLE_TYPE_DOUBLE;
            break;

        // Mapování: AST_DATA_TYPE_FLOAT_OR_NULL -> SYMTABLE_TYPE_DOUBLE_OR_NULL
        case AST_DATA_TYPE_FLOAT_OR_NULL:
            *symtableType = SYMTABLE_TYPE_DOUBLE_OR_NULL;
            break;

        // Mapování: AST_DATA_TYPE_STRING -> SYMTABLE_TYPE_STRING
        case AST_DATA_TYPE_STRING:
            *symtableType = SYMTABLE_TYPE_STRING;
            break;

        // Mapování: AST_DATA_TYPE_STRING_OR_NULL -> SYMTABLE_TYPE_STRING_OR_NULL
        case AST_DATA_TYPE_STRING_OR_NULL:
            *symtableType = SYMTABLE_TYPE_STRING_OR_NULL;
            break;

        // Mapování: AST_DATA_TYPE_VOID -> SYMTABLE_TYPE_VOID
        case AST_DATA_TYPE_VOID:
            *symtableType = SYMTABLE_TYPE_VOID;
            break;

        // Pro jakýkoliv jiný typ datového typu AST není návratový typ definován
        default:
            *symtableType = SYMTABLE_TYPE_UNKNOWN;  // Značí interní chybu (nemělo by nikdy nastat)
            break;
    } // switch()
} // parser_mapASTDataTypeToFunReturnType()

/**
 * @brief Namapuje typ datového typu AST na stav symbolu v tabulce symbolů.
 */
void parser_mapASTDataTypeToSymtableState(AST_DataType astDataType, \
                                          Symtable_symbolState *symtableState)
{
    // Ověření platnosti předaného ukazatele
    if(symtableState == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    // Mapujeme typ datového typu AST na stav symbolu v tabulce symbolů
    switch(astDataType) {
        // Mapování: AST_DATA_TYPE_INT -> SYMTABLE_SYMBOL_VARIABLE_INT
        case AST_DATA_TYPE_INT:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_INT;
            break;

        // Mapování: AST_DATA_TYPE_INT_OR_NULL -> SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL
        case AST_DATA_TYPE_INT_OR_NULL:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL;
            break;

        // Mapování: AST_DATA_TYPE_FLOAT -> SYMTABLE_SYMBOL_VARIABLE_DOUBLE
        case AST_DATA_TYPE_FLOAT:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_DOUBLE;
            break;

        // Mapování: AST_DATA_TYPE_FLOAT_OR_NULL -> SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL
        case AST_DATA_TYPE_FLOAT_OR_NULL:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL;
            break;

        // Mapování: AST_DATA_TYPE_STRING -> SYMTABLE_SYMBOL_VARIABLE_STRING
        case AST_DATA_TYPE_STRING:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_STRING;
            break;

        // Mapování: AST_DATA_TYPE_STRING_OR_NULL -> SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL
        case AST_DATA_TYPE_STRING_OR_NULL:
            *symtableState = SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL;
            break;

        // Mapování: AST_DATA_TYPE_VOID -> SYMTABLE_SYMBOL_FUNCTION
        case AST_DATA_TYPE_VOID:
            *symtableState = SYMTABLE_SYMBOL_FUNCTION;
            break;

        // Pro jakýkoliv jiný typ datového typu AST není stav symbolu definován
        default:
            *symtableState = SYMTABLE_SYMBOL_UNKNOWN;  // Značí interní chybu (nemělo by nikdy nastat)
            break;
    } // switch()
} // parser_mapASTDataTypeToSymtableState()

/*** Konec souboru parser_common.c ***/
