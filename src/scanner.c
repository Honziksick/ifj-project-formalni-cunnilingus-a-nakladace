/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   14.11.2024                                                *
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
 *
 * @brief Definice a implementace funkcí modulu scanner.
 * @details Tento soubor obsahuje deklaraci funkcí a datových typů modulu
 *          scannner, které jsou implementovány v souboru scanner.c.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"
//pro1 #include "symtable.h"


inline char scanner_getNextChar() {      // Čte jeden znak ze souboru
    return getchar();
}

inline void scanner_ungetChar(int c) {    // Vrátí char zpět do vstupního proudu
    ungetc(c, stdin);
}

CharType scanner_charIdentity(char c) {
    if(isalpha(c) || c == '_') {
        return LETTER;  //c je písmeno (LETTER)
    }
    else if(isdigit(c)) {
        return NUMBER;  //c je číslo (NUMBER)
    }
    else if(isspace(c)) { 
        return WHITESPACE;   //c je prázdný znak (WHITESPACE)
    }
    else if(c == '#' || c == '$' || c == '%' || 
            c == '&' || c == 39  || c == '^' ||
            c == '`' || c == '~' || c == 127) { //Znak, co není v jazyce povolen (39 = ' , 127 = DEL)
        return NOT_IN_LANGUAGE;     //c je znak, kerý nepatří do jazyka (NIL)
    }
    else if(c == '(' || c == ')' || c == '*' ||
            c == '+' || c == ',' || c == '-' ||
            c == ':' || c == ';' || c == '{' ||
            c == '|' || c == '}') { //c je speciální jednoduchý symbol (special SIMPLE)
        return SIMPLE;
    }
    else if(c == '!' || c == '"' || c == '.' ||
            c == '/' || c == '<' || c == '=' ||
            c == '>' || c == '?' || c == '@' ||
            c == '[' || c == 92  || c == ']') { //c je specialní složitý symbol (special COMPLEX)
        return COMPLEX;
    }
    else if(c == EOF) {  //c je konec souboru
        return CHAR_EOF;
    }
    else {  //c nespadá do žádné ze skupin znaků, jedná se o chybu, je nutné sem vložit návratovou hodnotu pro překlad kódu
        error_handle(ERROR_LEXICAL);
        return CHAR_EOF;    //V případě erroru dává smysl sem vložit návratovou hodnotu EOF
    }
}

Keywords scanner_isKeyword(DString *value) {
    if(strcmp(value->str, "const") == 0) {
        return KEY_const;
    } else if(strcmp(value->str, "var") == 0) {
        return KEY_var;
    } else if(strcmp(value->str, "i32") == 0) {
        return KEY_i32;
    } else if(strcmp(value->str, "f64") == 0) {
        return KEY_f64;
    } else if(strcmp(value->str, "pub") == 0) {
        return KEY_pub;
    } else if(strcmp(value->str, "fn") == 0) {
        return KEY_fn;
    } else if(strcmp(value->str, "void") == 0) {
        return KEY_void;
    } else if(strcmp(value->str, "return") == 0) {
        return KEY_return;
    } else if(strcmp(value->str, "null") == 0) {
        return KEY_null;
    } else if(strcmp(value->str, "if") == 0) {
        return KEY_if;
    } else if(strcmp(value->str, "else") == 0) {
        return KEY_else;
    } else if(strcmp(value->str, "while") == 0) {
        return KEY_while;
    } else if(strcmp(value->str, "_") == 0) {
        return KEY_underscore;
    } else {
        return KEY_IDENTIFIER;
    }
}

Token scanner_tokenCreate(TokenType type, DString *value) {
    Token token;
    token.type = type;
    token.value = value;
    return token;
}

Token scanner_stringlessTokenCreate(TokenType type) {
    Token token;
    token.type = type;
    token.value = NULL;
    return token;
}

Token scanner_FSM() {
    //Inicializace FSM po jednom volání Syntaktického analyzátoru
    char c;
    bool stopFSM = false;
    StateFSM state = STATE0_START;
    DString *str = string_init();
    Token lexToken;

    //Běh FSM po jednom volání od Syntaktického analyzátoru
    while(stopFSM == false)
    {
        int x = 0;
        int keytest = 0;
        switch (state) {
            /*
            -------------
            Stav 0: START
            -------------
            */
            case STATE0_START:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        string_append_char(str, c);
                        state = STATE1_LETTERS;
                        break;
                    case NUMBER:
                        string_append_char(str, c);
                        state = STATE2_NUMBERS;
                        break;
                    case WHITESPACE:
                        break;
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case SIMPLE:
                        state = STATE5_SIMPLE;
                        break;
                    case COMPLEX:
                        state = STATE6_COMPLEX;
                    case CHAR_EOF:
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EOF);
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                    default:    
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - charIdentity vrátil něco, co nedává smysl
                        break;
                }
                break;
            /*
            ---------------
            Stav 1: LETTERS
            ---------------
            */
            case STATE1_LETTERS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        string_append_char(str, c);
                        break;
                    case NUMBER:
                        string_append_char(str, c);
                        break;
                    case WHITESPACE:
                        keytest = scanner_isKeyword(str);
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                            stopFSM = true;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                            stopFSM = true;
                        }
                        break;
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    default:    //SIMPLE + COMPLEX + CHAR_EOF
                        keytest = scanner_isKeyword(str);
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                            stopFSM = true;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                            stopFSM = true;
                        }
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                }
                break;
            /*
            ---------------
            Stav 2: NUMBERS ZAČNI TADYYYYYYYYYYYYYYYYYYYYYYYY
            ---------------
            */
            case NUMBERS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:                        //1/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načteno písmeno do tokenu čísla
                        break;
                    case NUMBER:                        //2/29
                        string_append_char(str, c);
                        break;
                    case WHITE:                         //3/29
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_PERIOD:                      //18/29
                        string_append_char(str, c);
                        state = FLOAT_UNREADY;
                        break;
                    case C_EOL:                         //28/29
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        break;
                    default:                            //5-17,19-27,29/29 (Special Simple, Special Complex(without 18-PERIOD, 28-EOL))
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ---------------------
            Stav 4: FLOAT_UNREADY
            ---------------------
            */
            case FLOAT_UNREADY:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case NUMBER:                        //2/29
                        string_append_char(str, c);
                        state = FLOAT_READY;
                        break;
                    default:                            //1,3-29/29 (NOT 2-NUMBER)
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - nebylo načteno číslo do tokenu nehotového floatu
                        break;
                }
                break;
            /*
            -------------------
            Stav 5: FLOAT_READY
            -------------------
            */
            case FLOAT_READY:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:                        //1/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načteno písmeno do tokenu čísla
                        break;
                    case NUMBER:                        //2/29
                        string_append_char(str, c);
                        break;
                    case WHITE:                         //3/29
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_PERIOD:                      //18/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtena druhá tečka do tokenu floatu
                        break;
                    case C_EOL:                         //28/29
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        break;
                    default:                            //5-17,19-27,29/29 (Special Simple, Special Complex(without 18-PERIOD, 28-EOL))
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ---------------------------------------------------
            Stav 6: NON (Number or Null - Initialization ("?"))
            ---------------------------------------------------
            */
            case NON:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 105:   //i
                        state = NON1a;
                        break;
                    case 102:   //f
                        state = NON2a;
                        break;
                    case 91:    //[
                        state = NON3a;
                        break;
                    case 32:    //SPACE
                        break;
                    case 9:     //TAB
                        break;
                    default:    //NOT i,f,[,SPACE,TAB
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            --------------------
            Stav 7: NON1a ("?i")
            --------------------
            */
            case NON1a:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 51:    //3
                        state = NON1b;
                        break;
                    default:    //NOT 3
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            ---------------------
            Stav 8: NON1b ("?i3")
            ---------------------
            */
            case NON1b:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 50:    //2
                        lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qi32);
                        stopFSM = true;
                        break;
                    default:    //NOT 2
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            --------------------
            Stav 9: NON2a ("?f")
            --------------------
            */
            case NON2a:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 54:    //6
                        state = NON2b;
                        break;
                    default:    //NOT 6
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            ----------------------
            Stav 10: NON2b ("?f6")
            ----------------------
            */
            case NON2b:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 52:    //4
                        lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qf64);
                        stopFSM = true;
                        break;
                    default:    //NOT 4
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            ---------------------
            Stav 11: NON3a ("?[")
            ---------------------
            */
            case NON3a:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 93:    //]
                        state = NON3b;
                        break;
                    case 32:    //SPACE
                        break;
                    case 9:     //TAB
                        break;
                    default:    //NOT ],SPACE,TAB
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            ----------------------
            Stav 12: NON3b ("?[]")
            ----------------------
            */
            case NON3b:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 117:    //u
                        state = NON3c;
                        break;
                    case 32:    //SPACE
                        break;
                    case 9:     //TAB
                        break;
                    default:    //NOT u,SPACE,TAB
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            -----------------------
            Stav 13: NON3c ("?[]u")
            -----------------------
            */
            case NON3c:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 56:    //8
                        lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qu8);
                        stopFSM = true;
                        break;
                    default:    //NOT 8
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                        break;
                }
                break;
            /*
            -----------------
            Stav 14: U1 ("[")
            -----------------
            */
            case U1:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 93:    //]
                        state = U2;
                        break;
                    case 32:    //SPACE
                        break;
                    case 9:     //TAB
                        break;
                    default:    //NOT ],SPACE,TAB
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                        break;
                }
                break;
            /*
            ------------------
            Stav 15: U2 ("[]")
            ------------------
            */
            case U2:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 117:    //u
                        state = U3;
                        break;
                    case 32:    //SPACE
                        break;
                    case 9:     //TAB
                        break;
                    default:    //NOT u,SPACE,TAB
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                        break;
                }
                break;
            /*
            -------------------
            Stav 16: U3 ("[]u")
            -------------------
            */
            case U3:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 56:    //8
                        lexToken = scanner_stringlessTokenCreate(TOKEN_K_u8);
                        stopFSM = true;
                        break;
                    default:    //NOT 8
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                        break;
                }
                break;
            /*
            -----------------
            Stav 17: I1 ("@")
            -----------------
            */
            case I1:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 105:    //i
                        state = I2;
                        break;
                    default:    //NOT i
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu @import
                        break;
                }
                break;
            /*
            ------------------
            Stav 18: I2 ("@i")
            ------------------
            */
            case I2:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 109:    //m
                        state = I3;
                        break;
                    default:    //NOT m
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu @import
                        break;
                }
                break;
            /*
            -------------------
            Stav 19: I3 ("@im")
            -------------------
            */
            case I3:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 112:    //p
                        state = I4;
                        break;
                    default:    //NOT p
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu @import
                        break;
                }
                break;
            /*
            --------------------
            Stav 20: I4 ("@imp")
            --------------------
            */
            case I4:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 111:    //o
                        state = I5;
                        break;
                    default:    //NOT o
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu @import
                        break;
                }
                break;
            /*
            ---------------------
            Stav 21: I5 ("@impo")
            ---------------------
            */
            case I5:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 114:    //r
                        state = I6;
                        break;
                    default:    //NOT r
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu @import
                        break;
                }
                break;
            /*
            ----------------------
            Stav 22: I6 ("@impor")
            ----------------------
            */
            case I6:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 116:    //t
                        lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                        stopFSM = true;
                        break;
                    default:    //NOT t
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu @import
                        break;
                }
                break;
            /*
            --------------
            Stav 23: EQUAL
            --------------
            */
            case EQUAL:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case WHITE:                         //3/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
                        stopFSM = true;
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_EQUALITY_SIGN:               //21/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUAL_TO);
                        stopFSM = true;
                        break;
                    case C_EOL:                         //28/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
                        stopFSM = true;
                        break;
                    default:                            //1,2,5-20,22-27,29/29 (Letters, Numbers, Special Simple, Special Complex(without 21-EQUALITY_SIGN, 28-EOL))
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ------------------
            Stav 24: NOT_EQUAL
            ------------------
            */
            case NOT_EQUAL:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case C_EQUALITY_SIGN:               //21/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_NOT_EQUAL_TO);
                        stopFSM = true;
                        break;
                    default:                            //1-20,22-29 (NOT EQUALITY_SIGN)
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtení nekompatibilního znaku do tokenu !=
                        break;
                }
                break;
            /*
            -------------
            Stav 25: LESS
            -------------
            */
            case LESS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case WHITE:                         //3/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
                        stopFSM = true;
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_EQUALITY_SIGN:               //21/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_EQUAL_THAN);
                        stopFSM = true;
                        break;
                    case C_EOL:                         //28/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
                        stopFSM = true;
                        break;
                    default:                            //1,2,5-20,22-27,29/29 (Letters, Numbers, Special Simple, Special Complex(without 21-EQUALITY_SIGN, 28-EOL))
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ----------------
            Stav 26: GREATER
            ----------------
            */
            case GREATER:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case WHITE:                         //3/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
                        stopFSM = true;
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_EQUALITY_SIGN:               //21/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_EQUAL_THAN);
                        stopFSM = true;
                        break;
                    case C_EOL:                         //28/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
                        stopFSM = true;
                        break;
                    default:                            //1,2,5-20,22-27,29/29 (Letters, Numbers, Special Simple, Special Complex(without 21-EQUALITY_SIGN, 28-EOL))
                        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            --------------
            Stav 27: SLASH
            --------------
            */
            case SLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case WHITE:                         //3/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                        stopFSM = true;
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_SLASH:                       //19/29
                        state = DOUBLE_SLASH;
                        break;
                    case C_EOL:                         //28/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                        stopFSM = true;
                        break;
                    default:                            //1,2,5-18,20-27,29/29 (Letters, Numbers, Special Simple, Special Complex(without 19-SLASH, 28-EOL))
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ---------------------
            Stav 28: DOUBLE_SLASH
            ---------------------
            */
            case DOUBLE_SLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case C_EOL:                         //28/29
                        state = START;
                        break;
                    default:                            //1-27,29 (NOT EOL))
                        break;
                }
                break;
            /*
            ---------------------
            Stav 29: DOUBLE_QUOTE
            ---------------------
            */
            case DOUBLE_QUOTE:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case C_DOUBLE_QUOTE:                //17/29
                        lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                        stopFSM = true;
                        break;
                    case C_BACKSLASH:                   //26/29
                        state = ESCAPE;
                        break;
                    case C_EOF:                         //29/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - do tokenu stringu se načítá EOF
                        break;
                    default:                            //1-16,18-25,27,28 (NOT ",\,EOF))
                        string_append_char(str, c);
                        break;
                }
                break;
            /*
            ---------------
            Stav 30: ESCAPE
            ---------------
            */
            case ESCAPE:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (c) {  //Identifikace konkrétního znaku
                    case 92:    // backslash
                        string_append_char(str, 92);
                        state = DOUBLE_QUOTE;
                        break;
                    case 116:   //t
                        string_append_char(str, 9);
                        state = DOUBLE_QUOTE;
                        break;
                    case 114:   //r
                        string_append_char(str, 13);
                        state = DOUBLE_QUOTE;
                        break;
                    case 110:   //n
                        string_append_char(str, 10);
                        state = DOUBLE_QUOTE;
                        break;
                    case 34:    //"
                        string_append_char(str, 34);
                        state = DOUBLE_QUOTE;
                        break;
                    case 120:   //x
                        state = ESCAPE_X;
                        break;
                    default:    //NOT backslash,t,r,n,",x
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro escape sekvenci ve stringu
                        break;
                }
                break;
            /*
            -----------------
            Stav 31: ESCAPE_X
            -----------------
            */
            case ESCAPE_X:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102)) {
                    if(c >= 48 && c <= 57) {    //0-9
                        x = 16*(int)c;
                    }
                    if(c == 65 || c == 97) {    //A || a
                        x = 16*10;
                    }
                    if(c == 66 || c == 98) {    //B || b
                        x = 16*11;
                    }
                    if(c == 67 || c == 99) {    //C || c
                        x = 16*12;
                    }
                    if(c == 68 || c == 100) {   //D || d
                        x = 16*13;
                    }
                    if(c == 69 || c == 101) {   //E || e
                        x = 16*14;
                    }
                    if(c == 70 || c == 102) {   //F || f
                        x = 16*15;
                    }
                } else {
                    stopFSM = true;
                    error_handle(ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro X escape sekvenci ve stringu
                    break;
                }

                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102)) {
                    if(c >= 48 && c <= 57) {    //0-9
                        x = x + (int)c;
                    }
                    if(c == 65 || c == 97) {    //A || a
                        x = x + 10;
                    }
                    if(c == 66 || c == 98) {    //B || b
                        x = x + 11;
                    }
                    if(c == 67 || c == 99) {    //C || c
                        x = x + 12;
                    }
                    if(c == 68 || c == 100) {   //D || d
                        x = x + 13;
                    }
                    if(c == 69 || c == 101) {   //E || e
                        x = x + 14;
                    }
                    if(c == 70 || c == 102) {   //F || f
                        x = x + 15;
                    }
                } else {
                    stopFSM = true;
                    error_handle(ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro X escape sekvenci ve stringu
                    break;
                }

                string_append_char(str, (char)x);
                state = DOUBLE_QUOTE;
                break;
            /*
            ------------------
            Stav 32: BACKSLASH
            ------------------
            */
            case BACKSLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case C_BACKSLASH:                   //26/29
                        state = DOUBLE_BACKSLASH;
                        break;
                    default:                            //1-25,27-29 (NOT \))
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načten nekompatibilní znak do tokenu stringu s variantou backslash
                        break;
                }
                break;
            /*
            -------------------------
            Stav 33: DOUBLE_BACKSLASH
            -------------------------
            */
            case DOUBLE_BACKSLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case S_RIGHT_PARENTHESIS:           //6/29
                        lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                    case C_BACKSLASH:                   //26/29
                        c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                        if(c == 92) {                   //backslash
                            string_append_char(str, 10);    //EOL
                        } else {
                            string_append_char(str, 92);    //backslash
                            scanner_ungetChar(c);
                        }
                        break;
                    case C_EOL:                         //28/29
                        break;
                    case C_EOF:                         //29/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);    //ERROR - načten EOF do tokenu stringu
                        break;
                    default:                            //1-5,7-25,27 (NOT ),\,EOL,EOF))
                        string_append_char(str, c);
                        break;
                }
                break;
            /*
            --------------------
            Stav DEFAULT (ERROR)
            --------------------
            */
            default: //stateFSM ERROR
                stopFSM = true;
                error_handle(ERROR_LEXICAL);
                break;
        }
    }

    string_free(str);
    return lexToken;
}

inline Token scanner_getNextToken() {  //Převaděč Tokenu pro Parser (vlastně nepotřebné, Token lze brát přímo z FSM)
    return scanner_FSM();
}