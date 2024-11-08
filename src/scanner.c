/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                   Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   8.11.2024                                                 *
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

#include "scanner.h"


char scanner_getNextChar() {      // Čte jeden znak ze souboru
    return (char)getchar();
}

void scanner_ungetChar(char c) {    // Vrátí char zpět do vstupního proudu
    ungetc(c, stdin);
}

CharType scanner_charIdentity(char c) {
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {  //65 - 90, 97 - 122
        return LETTER;  //c je písmeno (LETTER)
    }
    else if(c >= '0' && c <= '9') { //48 - 57
        return NUMBER;  //c je číslo (NUMBER)
    }
    else if(c == 9 || c == 32) { //TAB or SPACE
        return WHITE;   //c je prázdný znak (WHITE)
    }
    else if((c>= 0 && c <= 8) || (c>= 11 && c <= 31) || (c>= 35 && c <= 39) || c==96 || (c>= 126 && c <= 254)) { //Znak, co není v jazyce povolen
        return NIL;     //c je znak, kerý nepatří do jazyka (NIL)
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

Keywords scanner_isKeyword(DString *value) {
    if       (value->str == "const") {
        return KEY_const;
    } else if(value->str == "var") {
        return KEY_var;
    } else if(value->str == "i32") {
        return KEY_i32;
    } else if(value->str == "f64") {
        return KEY_f64;
    } else if(value->str == "pub") {
        return KEY_pub;
    } else if(value->str == "fn") {
        return KEY_fn;
    } else if(value->str == "void") {
        return KEY_void;
    } else if(value->str == "return") {
        return KEY_return;
    } else if(value->str == "null") {
        return KEY_null;
    } else if(value->str == "if") {
        return KEY_if;
    } else if(value->str == "else") {
        return KEY_else;
    } else if(value->str == "while") {
        return KEY_while;
    } else if(value->str == "_") {
        return KEY_UNDERSCORE;
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
    token.value->str = NULL;
    token.value->length = NULL;
    token.value->allocatedSize = NULL;
    return token;
}

Token scanner_FSM() {
    //Inicializace FSM po jednom volání Syntaktického analyzátoru
    char c;
    bool stopFSM = false;
    StateFSM state = START;
    DString *str = string_init();
    Token lexToken;

    //Běh FSM po jednom volání od Syntaktického analyzátoru
    while(stopFSM == false)
    {
        switch (state) {
            /*
            -------------
            Stav 1: START
            -------------
            */
            case START:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:                        //1/29
                        string_append_char(str, c);
                        state = LETTERS;
                        break;
                    case NUMBER:                        //2/29
                        string_append_char(str, c);
                        state = NUMBERS;
                        break;
                    case WHITE:                         //3/29
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case S_LEFT_PARENTHESIS:            //5/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LEFT_PARENTHESIS);
                        stopFSM = true;
                        break;
                    case S_RIGHT_PARENTHESIS:           //6/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_RIGHT_PARENTHESIS);
                        stopFSM = true;
                        break;
                    case S_ASTERISK:                    //7/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_ASTERISK);
                        stopFSM = true;
                        break;
                    case S_PLUS:                        //8/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_PLUS);
                        stopFSM = true;
                        break;
                    case S_COMMA:                       //9/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_COMMA);
                        stopFSM = true;
                        break;
                    case S_MINUS:                       //10/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_MINUS);
                        stopFSM = true;
                        break;
                    case S_COLON:                       //11/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_COLON);
                        stopFSM = true;
                        break;
                    case S_SEMICOLON:                   //12/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SEMICOLON);
                        stopFSM = true;
                        break;
                    case S_LEFT_CURLY_BRACKET:          //13/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LEFT_CURLY_BRACKET);
                        stopFSM = true;
                        break;
                    case S_VERTICAL_BAR:                //14/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_VERTICAL_BAR);
                        stopFSM = true;
                        break;
                    case S_RIGHT_CURLY_BRACKET:         //15/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_RIGHT_CURLY_BRACKET);
                        stopFSM = true;
                        break;
                    case C_EXCLAMATION_MARK:            //16/29
                        state = NOT_EQUAL;
                        break;
                    case C_DOUBLE_QUOTE:                //17/29
                        state = DOUBLE_QUOTE;
                        break;
                    case C_PERIOD:                      //18/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_PERIOD);
                        stopFSM = true;
                        break;
                    case C_SLASH:                       //19/29
                        state = SLASH;
                        break;
                    case C_LESS_THAN:                   //20/29
                        state = LESS;
                        break;
                    case C_EQUALITY_SIGN:               //21/29
                        state = EQUAL;
                        break;
                    case C_GREATER_THAN:                //22/29
                        state = GREATER;
                        break;
                    case C_QUESTION_MARK:               //23/29
                        state = NON;
                        break;
                    case C_AT_SIGN:                     //24/29
                        state = I1;
                        break;
                    case C_LEFT_SQUARE_BRACKET:         //25/29
                        state = U1;
                        break;
                    case C_BACKSLASH:                   //26/29
                        state = BACKSLASH;
                        break;
                    case C_RIGHT_SQUARE_BRACKET:        //27/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - byl načten znak ']', který před sebou neměl znak '[', což v jazyce není možné
                        break;
                    case C_EOL:                         //28/29
                        break;
                    case C_EOF:                         //29/29
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EOF);
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                    default:                            //KONEC
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří do základní ani rozšířené ASCII
                        break;
                }
                break;
            /*
            ---------------
            Stav 2: LETTERS
            ---------------
            */
            case LETTERS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:                        //1/29
                        string_append_char(str, c);
                        break;
                    case NUMBER:                        //2/29
                        string_append_char(str, c);
                        break;
                    case WHITE:                         //3/29
                        int keytest = scanner_isKeyword(str);
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                            stopFSM = true;
                        } else if (keytest == KEY_UNDERSCORE) {
                            stopFSM = true;
                            error_handle(ERROR_LEXICAL);    //ERROR - byl objeven IDENTIFIER '_'
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                            stopFSM;
                        }
                        break;
                    case NIL:                           //4/29
                        stopFSM = true;
                        error_handle(ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case C_EOL:                         //28/29
                        int keytest = scanner_isKeyword(str);
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                            stopFSM = true;
                        } else if (keytest == KEY_UNDERSCORE) {
                            stopFSM = true;
                            error_handle(ERROR_LEXICAL);    //ERROR - byl objeven IDENTIFIER '_'
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                            stopFSM;
                        }
                        break;
                    default:                            //5-27,29/29 (Special Simple, Special Complex(without 28-EOL))
                        int keytest = scanner_isKeyword(str);
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                            stopFSM = true;
                        } else if (keytest == KEY_UNDERSCORE) {
                            stopFSM = true;
                            error_handle(ERROR_LEXICAL);    //ERROR - byl objeven IDENTIFIER '_'
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                            stopFSM;
                        }
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ---------------
            Stav 3: NUMBERS
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
                int x = 0;

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





Token scanner_getNextToken() {  //Převaděč Tokenu pro Parser (vlastně nepotřebné, Token lze brát přímo z FSM)
    Token lexToken = scanner_FSM();
    return lexToken;
}