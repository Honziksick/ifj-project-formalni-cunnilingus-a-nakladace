/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                   Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   14.10.2024                                                 *
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
        return LETTER; //c je LETTER
    }
    else if(c >= '0' && c <= '9') { //48 - 57
        return NUMBER; //c je NUMBER
    }
    else if((c >= '!' && c <= '-') || c == '/' || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~')) { //33 - 45, 47, 58 - 64, 91 - 96, 123 - 126
        return OPERATOR; //c je OPERATOR
    }
    else if(c == '.') { //47
        return DOT; //c je DOT
    }
    else if(c == 32) { //space
        return EMPTY; //c je EMPTY
    }
    else {
        return CHAR_ERROR; //c není nic z výše uvedených
    }
}

Token scanner_FSM() {
    char c;
    bool stopFSM = false;
    stateFSM state = START;
    DString *str = string_init();
    Token tToken;

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
                        tToken = scanner_tokenCreate(TOKEN_OPERATOR, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        string_append_char(str, c);
                        tToken = scanner_tokenCreate(TOKEN_DOT, str);
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
                        tToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        scanner_ungetChar(c);
                        tToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                        stopFSM = true;
                        break;
                    case EMPTY: //EMPTY
                        tToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
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
                        tToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        string_append_char(str, c);
                        state = FLOAT_UNREADY;
                        break;
                    case EMPTY: //EMPTY
                        tToken = scanner_tokenCreate(TOKEN_INT, str);
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
                        tToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);
                        break;
                    case EMPTY: //EMPTY
                        tToken = scanner_tokenCreate(TOKEN_FLOAT, str);
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
    return tToken;
}


// Provizorní kód, pls dodělat popřípadě
Token scanner_tokenCreate(TokenType type, DString *value) {
    Token token;
    token.type = type;
    token.value = *value;
    return token;
}

Token scanner_getNextToken() {  //Převaděč Tokenu pro Parser (vlastně nepotřebné, Token lze brát přímo z FSM)
    Token tToken = scanner_FSM();
    return tToken;
}
