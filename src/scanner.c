/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                   Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   29.10.2024                                                 *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file scanner.c
 * @author Hýža Pavel \<xhyzapa00>
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Definice a implementace funkcí modulu scanner.
 * @details Tento soubor obsahuje deklaraci funkcí a datových typů modulu
 *          scannner, které jsou implementovány v souboru scanner.c.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"        //Vlastní knihovny


char scanner_getNextChar() {      // Čte jeden znak ze souboru
    return (char)getchar();
}

void scanner_ungetChar(char c) {    // Vrátí char zpět do vstupního proudu
    ungetc(c, stdin);
}

CharType scanner_charIdentity(char c) {
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {  //65 - 90, 97 - 122
        return LETTER; //c je písmeno (LETTER)
    }
    else if(c >= '0' && c <= '9') { //48 - 57
        return NUMBER; //c je číslo (NUMBER)
    }
    else if(c == 9 || c == 32) { //TAB or SPACE
        return WHITE; //c je prázdný znak (WHITE)
    }
    else if((c>= 0 && c <= 8) || (c>= 11 && c <= 31) || (c>= 35 && c <= 39) || c==96 || (c>= 126 && c <= 254)) { //Znak, co není v jazyce povolen

    }
    else {
        switch (c) {
            case 10:    // New Line (EOL - End of Line)
                return C_EOL;

            case 33:    // !
                return C_EXCLAMATION_MARK;
            case 34:    // "
                return C_DOUBLE_QUOTE;

            case 40:    // (
                return S_LEFT_PARENTHESIS;
            case 41:    // )
                return S_RIGHT_PARENTHESIS;
            case 42:    // *
                return S_ASTERISK;
            case 43:    // +
                return S_PLUS;
            case 44:    // ,
                return S_COMMA;
            case 45:    // -
                return S_MINUS;
            case 46:    // .
                return C_PERIOD;
            case 47:    // /
                return C_SLASH;

            case 58:    // :
                return S_COLON;
            case 59:    // ;
                return S_SEMICOLON;
            case 60:    // <
                return C_LESS_THAN;
            case 61:    // =
                return C_EQUALITY_SIGN;
            case 62:    // >
                return C_GREATER_THAN;
            case 63:    // ?
                return C_QUESTION_MARK;
            case 64:    // @
                return C_AT_SIGN;

            case 91:    // [
                return C_LEFT_SQUARE_BRACKET;
            case 92:    // \ 
                return C_BACKSLASH;
            case 93:    // ]
                return C_RIGHT_SQUARE_BRACKET;

            case 123:   // {
                return S_LEFT_CURLY_BRACKET;
            case 124:   // |
                return S_VERTICAL_BAR;
            case 125:   // }
                return S_RIGHT_CURLY_BRACKET;

            case 255:   // EOF
                return C_EOF;
            
            default:    // ERROR při identifikaci CHARu (nepatří mezi 0 - 255)
                error_handle(ERROR_LEXICAL);
        }
    }
}

Token scanner_FSM() {
    char c;
    bool stopFSM = false;
    StateFSM state = START;
    DString *str = string_init();
    Token lexToken;

    while(stopFSM == false)
    {
        switch (state) {
            case START: //START
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        string_append_char(str, c);
                        state = CHARACTERS;
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(str, c);
                        state = DIGITS;
                        break;
                    case OPERATOR: //OPERATOR
                        string_append_char(str, c);
                        lexToken = scanner_tokenCreate(TOKEN_OPERATOR, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        string_append_char(str, c);
                        lexToken = scanner_tokenCreate(TOKEN_DOT, str);
                        stopFSM = true;
                        break;
                    case EMPTY: //EMPTY
                        state = START;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                }
                break;

            case CHARACTERS: //CHARACTERS
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        string_append_char(str, c);
                        state = CHARACTERS;
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(str, c);
                        state = CHARACTERS;
                        break;
                    case OPERATOR: //OPERATOR
                        scanner_ungetChar(c);
                        lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        scanner_ungetChar(c);
                        lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                        stopFSM = true;
                        break;
                    case EMPTY: //EMPTY
                        lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;

            case DIGITS: //DIGITS
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case START: //LETTER
                        string_append_char(str, c);
                        state = CHARACTERS;
                        break;
                    case CHARACTERS: //NUMBER
                        string_append_char(str, c);
                        state = DIGITS;
                        break;
                    case DIGITS: //OPERATOR
                        scanner_ungetChar(c);
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        string_append_char(str, c);
                        state = FLOAT_UNREADY;
                        break;
                    case EMPTY: //EMPTY
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                }
                break;
            case FLOAT_UNREADY: //FLOAT_UNREADY
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(str, c);
                        state = FLOAT_READY;
                        break;
                    case OPERATOR: //OPERATOR
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    case DOT: //DOT
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    case EMPTY: //EMPTY
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                }
                break;
            case FLOAT_READY: //FLOAT_READY
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(str, c);
                        state = FLOAT_READY;
                        break;
                    case OPERATOR: //OPERATOR
                        scanner_ungetChar(c);
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    case EMPTY: //EMPTY
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                }
                break;
            default: //stateFSM ERROR
                stopFSM = true;
                error_handle(ERROR_LEXICAL);
                break;
        }
    }

    string_free(str);
    return lexToken;
}


// Provizorní kód, pls dodělat popřípadě
Token scanner_tokenCreate(TokenType type, DString *value) {
    Token token;
    token.type = type;
    token.value = *value;
    return token;
}

Token scanner_getNexlexToken() {  //Převaděč Tokenu pro Parser (vlastně nepotřebné, Token lze brát přímo z FSM)
    Token lexToken = scanner_FSM();
    return lexToken;
}