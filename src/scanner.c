/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                   Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   6.10.2024                                                 *
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

char scanner_getNextChar(FILE *file) {
    return fgetc(file);
}

void scanner_ungetChar(FILE *file, char c) {
    ungetc(c, file);
}

CharType scanner_charIdentity(char c) {
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {  //65 - 90, 97 - 122
        return 1; //c je LETTER
    }
    else if(c >= '0' && c <= '9') { //48 - 57
        return 2; //c je NUMBER
    }
    else if((c >= '!' && c <= '-') || c == '/' || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~')) { //33 - 45, 47, 58 - 64, 91 - 96, 123 - 126
        return 3; //c je OPERATOR
    }
    else if(c == '.') { //47
        return 4; //c je DOT
    }
    else if(c == 32) { //space
        return 5; //c je EMPTY
    }
    else {
        return 6; //c není nic z výše uvedených
    }
}

Token scanner_FSM() {
    char c;
    bool stopFSM = false;
    stateFSM = 1;
    string str = *string_init();

    while(stopFSM == false)
    {
        switch (stateFSM) {
            case 1: //START
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case 1: //LETTER
                        string_append_char(*str, c);
                        stateFSM = 2;
                        break;
                    case 2: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 3;
                        break;
                    case 3: //OPERATOR
                        string_append_char(*str, c);
                        Token = scanner_tokenCreate(TOKEN_OPERATOR, *str);
                        stopFSM = true;
                        break;
                    case 4: //DOT
                        string_append_char(*str, c);
                        Token = scanner_tokenCreate(TOKEN_DOT, *str);
                        stopFSM = true;
                        break;
                    case 5: //EMPTY
                        stateFSM = 1;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;

            case 2: //CHARACTERS
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case 1: //LETTER
                        string_append_char(*str, c);
                        stateFSM = 2;
                        break;
                    case 2: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 2;
                        break;
                    case 3: //OPERATOR
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_IDENTIFIER, *str);
                        stopFSM = true;
                        break;
                    case 4: //DOT
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
                        error_handle(1);
                        break;
                }
                break;

            case 3: //DIGITS
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case 1: //LETTER
                        string_append_char(*str, c);
                        stateFSM = 2;
                        break;
                    case 2: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 3;
                        break;
                    case 3: //OPERATOR
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_INT, *str);
                        stopFSM = true;
                        break;
                    case 4: //DOT
                        string_append_char(*str, c);
                        stateFSM = 4;
                        break;
                    case 5: //EMPTY
                        Token = scanner_tokenCreate(TOKEN_INT, *str);
                        stopFSM = true;
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;
            case 4: //FLOAT_UNREADY
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case 1: //LETTER
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case 2: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 5;
                        break;
                    case 3: //OPERATOR
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case 4: //DOT
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case 5: //EMPTY
                        stopFSM = true;
                        error_handle(1);
                        break;
                    default: //ERROR (pravděpodobně scanner_charIdentity)
                        stopFSM = true;
                        error_handle(1);
                        break;
                }
                break;
            case 5: //FLOAT_READY
                c = scanner_getNextChar();
                switch (scanner_charIdentity(c)) {
                    case 1: //LETTER
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case 2: //NUMBER
                        string_append_char(*str, c);
                        stateFSM = 5;
                        break;
                    case 3: //OPERATOR
                        scanner_ungetChar(c);
                        Token = scanner_tokenCreate(TOKEN_FLOAT, *str);
                        stopFSM = true;
                        break;
                    case 4: //DOT
                        stopFSM = true;
                        error_handle(1);
                        break;
                    case 5: //EMPTY
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

Token scanner_getNextToken() {
    Token = scanner_FSM();
    return Token;
}