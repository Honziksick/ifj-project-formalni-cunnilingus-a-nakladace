/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                   Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   10.10.2024                                                 *
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
#include "dynamic_string.h"
#include "error.h"

char scanner_getNextChar() {      // Čte jeden znak ze souboru
    return getc();
}

void scanner_ungetChar(char c) {    // Vrátí char zpět do vstupního proudu
    ungetc(c);
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
        return ERROR; //c není nic z výše uvedených
    }
}

Token scanner_FSM() {
    char c;
    bool stopFSM = false;
    stateFSM = START;
    string str = *string_init();

    while(stopFSM == false)
    {
        switch (stateFSM) {
            case START: //START
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        string_append_char(*str, c);
                        stateFSM = CHARACTERS;
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = DIGITS;
                        break;
                    case OPERATOR: //OPERATOR
                        string_append_char(*str, c);
                        Token = scanner_tokenCreate(TOKEN_OPERATOR, *str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        string_append_char(*str, c);
                        Token = scanner_tokenCreate(TOKEN_DOT, *str);
                        stopFSM = true;
                        break;
                    case EMPTY: //EMPTY
                        stateFSM = START;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;

            case CHARACTERS: //CHARACTERS
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        string_append_char(*str, c);
                        stateFSM = CHARACTERS;
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = CHARACTERS;
                        break;
                    case OPERATOR: //OPERATOR
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_IDENTIFIER, *str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_IDENTIFIER, *str);
                        stopFSM = true;
                        break;
                    case 5: //EMPTY
                        Token = scanner_tokenCreate(TOKEN_IDENTIFIER, *str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(START);
                        break;
                }
                break;

            case DIGITS: //DIGITS
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case START: //LETTER
                        string_append_char(*str, c);
                        stateFSM = CHARACTERS;
                        break;
                    case CHARACTERS: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = DIGITS;
                        break;
                    case DIGITS: //OPERATOR
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_INT, *str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        string_append_char(*str, c);
                        stateFSM = FLOAT_UNREADY;
                        break;
                    case EMPTY: //EMPTY
                        Token = scanner_tokenCreate(TOKEN_INT, *str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;
            case FLOAT_UNREADY: //FLOAT_UNREADY
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 5;
                        break;
                    case OPERATOR: //OPERATOR
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case DOT: //DOT
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case EMPTY: //EMPTY
                        stopFSM = true;
                        error_handle(1);
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;
            case FLOAT_READY: //FLOAT_READY
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case LETTER: //LETTER
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case NUMBER: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 5;
                        break;
                    case OPERATOR: //OPERATOR
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_FLOAT, *str);
                        stopFSM = true;
                        break;
                    case DOT: //DOT
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case EMPTY: //EMPTY
                        Token = scanner_tokenCreate(TOKEN_FLOAT, *str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;
            default: //stateFSM ERROR
                stopFSM = true;
                error_handle(1);
                break;
        }
    }

    string_free(*str);
    return Token;
}

Token scanner_getNextToken() {  //Převaděč Tokenu pro Parser (vlastně nepotřebné, Token lze brát přímo z FSM)
    Token = scanner_FSM();
    return Token;
}
