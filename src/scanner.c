/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   22.11.2024                                                *
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


#include "scanner.h"
#include "parser_common.h"

inline int scanner_getNextChar() {      // Čte jeden znak ze souboru
    return getchar();
}

inline void scanner_ungetChar(int c) {    // Vrátí char zpět do vstupního proudu
    ungetc(c, stdin);
}

CharType scanner_charIdentity(int c) {
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
            c == '`' || c == '~' || c >= 127) { //Znak, co není v jazyce povolen (39 = ' , 127 = DEL)
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
            c == '>' || c == '?' || c == '@' || //(92 = \)
            c == '[' || c == 92  || c == ']') { //c je specialní složitý symbol (special COMPLEX)
        return COMPLEX;
    }
    else if(c == EOF) {  //c je konec souboru
        return CHAR_EOF;
    }
    else {  //c nespadá do žádné ze skupin znaků, jedná se o chybu, je nutné sem vložit návratovou hodnotu pro překlad kódu
        Parser_errorWatcher(SET_ERROR_LEXICAL);
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
    } else if(strcmp(value->str, "ifj") == 0) {
        return KEY_ifj;
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
    int c;
    bool stopFSM = false;
    StateFSM state = STATE0_START;
    DString *str = string_init();
    Token lexToken;
    int keytest = 0;

    //Běh FSM po jednom volání od Syntaktického analyzátoru
    while(stopFSM == false)
    {
        int x = 0;
        //printf("State num: %d\n", state);

        switch (state) {
            /*
            -------------
            Stav 0: START
            -------------
            */
            case STATE0_START:
                //printf("S0 Start\n");
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        //printf("S0 Let\n");
                        string_append_char(str, (char)c);
                        state = STATE1_LETTERS;
                        break;
                    case NUMBER:
                        string_append_char(str, (char)c);
                        //printf("S0 Num\n");
                        state = STATE2_NUMBERS;
                        break;
                    case WHITESPACE:
                        //printf("S0 White\n");
                        break;
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case SIMPLE:
                        //printf("S0 Sim\n");
                        state = STATE5_SIMPLE;
                        break;
                    case COMPLEX:
                        //printf("S0 Com\n");
                        state = STATE6_COMPLEX;
                        break;
                    case CHAR_EOF:
                        //printf("S0 EOF\n");
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EOF);
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                    default:
                        //printf("S0 DEF\n");
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - charIdentity vrátil něco, co nedává smysl
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
                //printf("S1 Start\n");
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        string_append_char(str, (char)c);
                        //printf("S1 Let\n");
                        break;
                    case NUMBER:
                        string_append_char(str, (char)c);
                        //printf("S1 Num\n");
                        break;
                    case WHITESPACE:
                        keytest = scanner_isKeyword(str);
                        //printf("S1 White\n");
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                            stopFSM = true;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                            stopFSM = true;
                        }
                        break;
                    case NOT_IN_LANGUAGE:
                        //printf("S1 NIL\n");
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    default:    //SIMPLE + COMPLEX + CHAR_EOF
                        //printf("S1 DEF\n");
                        keytest = scanner_isKeyword(str);
                        if(keytest == KEY_IDENTIFIER) {
                            lexToken = scanner_tokenCreate(TOKEN_IDENTIFIER, str);
                        } else {
                            lexToken = scanner_stringlessTokenCreate(keytest);
                        }
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                }
                break;
            /*
            ---------------
            Stav 2: NUMBERS
            ---------------
            */
            case STATE2_NUMBERS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                //printf("S2 Start\n");
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        if(c == 'e') {
                            string_append_char(str, (char)c);
                            state = STATE33_FLOAT_UNREADY_E1;
                        } else {
                            stopFSM = true;
                            //printf("S2 Let\n");
                            Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načítá se písmeno do Tokenu Int
                        }
                        break;
                    case NUMBER:
                        string_append_char(str, (char)c);
                        //printf("S2 Num\n");
                        break;
                    case WHITESPACE:
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        stopFSM = true;
                        //printf("S2 White\n");
                        break;
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        //printf("S2 NIL\n");
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case COMPLEX:
                        //printf("S2 COM\n");
                        if(c == '.') {
                            string_append_char(str, (char)c);
                            state = STATE3_FLOAT_UNREADY;
                        } else {
                            lexToken = scanner_tokenCreate(TOKEN_INT, str);
                            scanner_ungetChar(c);
                            stopFSM = true;
                        }
                        break;
                    default:    //SIMPLE + CHAR_EOF
                        //printf("S2 DEF\n");
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                }
                break;
            /*
            ---------------------
            Stav 3: FLOAT_UNREADY
            ---------------------
            */
            case STATE3_FLOAT_UNREADY:
                //printf("S3 Start\n");
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case NUMBER:
                        //printf("S3 Num\n");
                        string_append_char(str, (char)c);
                        state = STATE4_FLOAT_READY;
                        break;
                    default:    //LETTER + NOT_IN_LANGUAGE + SIMPLE + COMPLEX + CHAR_EOF
                        //printf("S3 DEF\n");
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - nebylo načteno číslo do Tokenu nehotového floatu
                        break;
                }
                break;
            /*
            -------------------------
            Stav 33: FLOAT_UNREADY_E1
            -------------------------
            */
            case STATE33_FLOAT_UNREADY_E1:
                //printf("S33 Start\n");
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case SIMPLE:
                        if(c == '+') {
                            string_append_char(str, (char)c);
                            state = STATE4_FLOAT_READY;
                        } else if (c == '-') {
                            string_append_char(str, (char)c);
                            state = STATE4_FLOAT_READY;
                        } else {
                            stopFSM = true;
                            Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - nebylo načteno +/- do Tokenu nehotového floatu varianty exponent
                        }
                        break;
                    case NUMBER:
                        //printf("S33 Num\n");
                        string_append_char(str, (char)c);
                        state = STATE4_FLOAT_READY;
                        break;
                    default:    //LETTER + NOT_IN_LANGUAGE + COMPLEX + CHAR_EOF
                        //printf("S33 DEF\n");
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - nebylo načteno číslo do Tokenu nehotového floatu
                        break;
                }
                break;
            /*
            -------------------
            Stav 4: FLOAT_READY
            -------------------
            */
            case STATE4_FLOAT_READY:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                //printf("S4 Start\n");
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        //printf("S4 Let\n");
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načítá se písmeno do Tokenu Float
                        break;
                    case NUMBER:
                        //printf("S4 Num\n");
                        string_append_char(str, (char)c);
                        break;
                    case WHITESPACE:
                        //printf("S4 White\n");
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        stopFSM = true;
                        break;
                    case NOT_IN_LANGUAGE:
                        //printf("S4 Nil\n");
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    default:    //SIMPLE + COMPLEX + CHAR_EOF
                        //printf("S4 DEF\n");
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        scanner_ungetChar(c);
                        stopFSM = true;
                        break;
                }
                break;
            /*
            --------------
            Stav 5: SIMPLE
            --------------
            */
            case STATE5_SIMPLE:
                switch(c) {
                    case '(':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LEFT_PARENTHESIS);
                        break;
                    case ')':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_RIGHT_PARENTHESIS);
                        break;
                    case '*':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_ASTERISK);
                        break;
                    case '+':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_PLUS);
                        break;
                    case ',':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_COMMA);
                        break;
                    case '-':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_MINUS);
                        break;
                    case ':':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_COLON);
                        break;
                    case ';':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SEMICOLON);
                        break;
                    case '{':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LEFT_CURLY_BRACKET);
                        break;
                    case '|':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_VERTICAL_BAR);
                        break;
                    case '}':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_RIGHT_CURLY_BRACKET);
                        break;
                }
                stopFSM = true;
                break;
            /*
            ---------------
            Stav 6: COMPLEX
            ---------------
            */
            case STATE6_COMPLEX:
                switch(c) {
                    case '.':
                        lexToken = scanner_stringlessTokenCreate(TOKEN_PERIOD);
                        stopFSM = true;
                        break;
                    case '!':
                        state = STATE7_EXCLAMATION_MARK;
                        break;
                    case '<':
                        state = STATE8_LESS;
                        break;
                    case '>':
                        state = STATE9_GREATER;
                        break;
                    case '=':
                        state = STATE10_EQUALITY_MARK;
                        break;
                    case '/':
                        state = STATE11_SLASH;
                        break;
                    case '@':
                        state = STATE13_AT_SIGN;
                        break;
                    case '[':
                        state = STATE14_LEFT_SQUARE_BRACKET_A;
                        break;
                    case ']':
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtený znak ] bez [ před ním
                        break;
                    case '?':
                        state = STATE17_QUESTION_MARK;
                        break;
                    case 92:    // 92 = backslash
                        state = STATE28_BACKSLASH;
                        break;
                    case 34:    // 34 - "
                        state = STATE25_DOUBLE_QUOTATION_MARKS;
                        break;
                    default:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtený COMPLEX není validní
                }
                break;
            /*
            ------------------------
            Stav 7: EXCLAMATION_MARK
            ------------------------
            */
            case STATE7_EXCLAMATION_MARK:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case COMPLEX:
                        if(c == '=') {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_NOT_EQUAL_TO);
                            stopFSM = true;
                        } else {
                            stopFSM = true;
                            Parser_errorWatcher(SET_ERROR_LEXICAL);
                        }
                        break;
                    default:    //LETTER + NUMBER + WHITESPACE + NOT_IN_LANGUAGE + SIMPLE + CHAR_EOF
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);
                        break;
                }
                break;
            /*
            ------------
            Stav 8: LESS
            ------------
            */
            case STATE8_LESS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
                        stopFSM = true;
                        break;
                    case COMPLEX:
                        if(c == '=') {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_EQUAL_THAN);
                            stopFSM = true;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
                            stopFSM = true;
                            scanner_ungetChar(c);
                        }
                        break;
                    default:    //LETTER + NUMBER + SIMPLE + CHAR_EOF
                        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ---------------
            Stav 9: GREATER
            ---------------
            */
            case STATE9_GREATER:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
                        stopFSM = true;
                        break;
                    case COMPLEX:
                        if(c == '=') {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_EQUAL_THAN);
                            stopFSM = true;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
                            stopFSM = true;
                            scanner_ungetChar(c);
                        }
                        break;
                    default:    //LETTER + NUMBER + SIMPLE + CHAR_EOF
                        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ----------------------
            Stav 10: EQUALITY_MARK
            ----------------------
            */
            case STATE10_EQUALITY_MARK:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
                        stopFSM = true;
                        break;
                    case COMPLEX:
                        if(c == '=') {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_EQUAL_TO);
                            stopFSM = true;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
                            stopFSM = true;
                            scanner_ungetChar(c);
                        }
                        break;
                    default:    //LETTER + NUMBER + SIMPLE + CHAR_EOF
                        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            --------------
            Stav 11: SLASH
            --------------
            */
            case STATE11_SLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                        stopFSM = true;
                        break;
                    case COMPLEX:
                        if(c == '/') {
                            state = STATE12_DOUBLE_SLASH;
                        } else {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                            stopFSM = true;
                            scanner_ungetChar(c);
                        }
                        break;
                    default:    //LETTER + NUMBER + SIMPLE + CHAR_EOF
                        lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                        stopFSM = true;
                        scanner_ungetChar(c);
                        break;
                }
                break;
            /*
            ---------------------
            Stav 12: DOUBLE_SLASH
            ---------------------
            */
            case STATE12_DOUBLE_SLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case WHITESPACE:
                        if(c == 10) {   //10 = LF
                            state = STATE0_START;
                        }
                        break;
                    case CHAR_EOF:
                        scanner_ungetChar(c);
                        state = STATE0_START;
                        break;
                    default:    //LETTER + NUMBER + NOT_IN_LANGUAGE + SIMPLE + COMPLEX
                        break;
                }
                break;
            /*
            ----------------
            Stav 13: AT_SIGN
            ----------------
            */
            case STATE13_AT_SIGN:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                switch (scanner_charIdentity(c)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        string_append_char(str, (char)c);
                        break;
                    case NOT_IN_LANGUAGE:
                        stopFSM = true;
                        Parser_errorWatcher(SET_ERROR_LEXICAL);
                        break;
                    case WHITESPACE:
                        if(strcmp(str->str, "import") == 0) {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                            stopFSM = true;
                        }
                        else {
                            stopFSM = true;
                            Parser_errorWatcher(SET_ERROR_LEXICAL);
                        }
                        break;
                    default:    //NUMBER + SIMPLE + COMPLEX + CHAR_EOF
                        scanner_ungetChar(c);
                        if(strcmp(str->str, "import") == 0) {
                            lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                            stopFSM = true;
                        }
                        else {
                            stopFSM = true;
                            Parser_errorWatcher(SET_ERROR_LEXICAL);
                        }
                        break;
                }
                break;
            /*
            ------------------------------
            Stav 14: LEFT_SQUARE_BRACKET_A
            ------------------------------
            */
            case STATE14_LEFT_SQUARE_BRACKET_A:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == ']') {
                    state = STATE15_LEFT_SQUARE_BRACKET_B;
                } else if(isspace(c)) {
                    //Nic
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                    break;
                }
                break;
            /*
            ------------------------------
            Stav 15: LEFT_SQUARE_BRACKET_B
            ------------------------------
            */
            case STATE15_LEFT_SQUARE_BRACKET_B:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == 'u') {
                    state = STATE16_LEFT_SQUARE_BRACKET_C;
                } else if(isspace(c)) {
                    //Nic
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                    break;
                }
                break;
            /*
            ------------------------------
            Stav 16: LEFT_SQUARE_BRACKET_C
            ------------------------------
            */
            case STATE16_LEFT_SQUARE_BRACKET_C:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == '8') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_u8);
                    stopFSM = true;
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                    break;
                }
                break;
            /*
            ----------------------
            Stav 17: QUESTION_MARK
            ----------------------
            */
            case STATE17_QUESTION_MARK:
                //printf("BASE\n");
                c = scanner_getNextChar();
                if(isspace(c)) {
                    //printf("space\n");
                    //Nic
                } else if(c == 'i') {
                    state = STATE18_QUESTION_MARK_A1;
                } else if(c == 'f') {
                    state = STATE19_QUESTION_MARK_B1;
                } else if(c == '[') {
                    state = STATE20_QUESTION_MARK_C1;
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 18: QUESTION_MARK_A1
            -------------------------
            */
            case STATE18_QUESTION_MARK_A1:
                //printf("A1\n");
                c = scanner_getNextChar();
                if(c == '3') {
                    //printf("if\n");
                    state = STATE21_QUESTION_MARK_A2;
                } else {
                    //printf("else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 19: QUESTION_MARK_B1
            -------------------------
            */
            case STATE19_QUESTION_MARK_B1:
                //printf("B1\n");
                c = scanner_getNextChar();
                if(c == '6') {
                    //printf("if\n");
                    state = STATE22_QUESTION_MARK_B2;
                } else {
                    //printf("else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 20: QUESTION_MARK_C1
            -------------------------
            */
            case STATE20_QUESTION_MARK_C1:
                //printf("C1\n");
                c = scanner_getNextChar();
                if(c == ']') {
                    //printf("if\n");
                    state = STATE23_QUESTION_MARK_C2;
                } else if(isspace(c)) {
                    //printf("space\n");
                    //Nic
                } else {
                    //printf("else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 21: QUESTION_MARK_A2
            -------------------------
            */
            case STATE21_QUESTION_MARK_A2:
                //printf("A2\n");
                c = scanner_getNextChar();
                if(c == '2') {
                    //printf("if\n");
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qi32);
                    stopFSM = true;
                } else {
                    //printf("else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 22: QUESTION_MARK_B2
            -------------------------
            */
            case STATE22_QUESTION_MARK_B2:
                //printf("B2\n");
                c = scanner_getNextChar();
                if(c == '4') {
                    //printf("if\n");
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qf64);
                    stopFSM = true;
                } else {
                    //printf("else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 23: QUESTION_MARK_C2
            -------------------------
            */
            case STATE23_QUESTION_MARK_C2:
                //printf("C2\n");
                c = scanner_getNextChar();
                if(c == 'u') {
                    //printf("ifC2\n");
                    state = STATE24_QUESTION_MARK_C3;
                } else if(isspace(c)) {
                    //printf("space\n");
                    //Nic
                } else {
                    //printf("elseC2\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            -------------------------
            Stav 24: QUESTION_MARK_C3
            -------------------------
            */
            case STATE24_QUESTION_MARK_C3:
                //printf("C3\n");
                c = scanner_getNextChar();
                if(c == '8') {
                    //printf("if\n");
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qu8);
                    stopFSM = true;
                } else {
                    //printf("else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                    break;
                }
                break;
            /*
            -------------------------------
            Stav 25: DOUBLE_QUOTATION_MARKS
            -------------------------------
            */
            case STATE25_DOUBLE_QUOTATION_MARKS:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == '"') {
                    lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                    stopFSM = true;
                } else if(c == 92) {    //92 = backslash
                    state = STATE26_ESCAPE_BACKSLASH;
                } else {
                    string_append_char(str, (char)c);
                }
                break;
            /*
            -------------------------
            Stav 26: ESCAPE_BACKSLASH
            -------------------------
            */
            case STATE26_ESCAPE_BACKSLASH:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == '"') {
                    string_append_char(str, '"');
                    state = STATE25_DOUBLE_QUOTATION_MARKS;
                } else if(c == 92) {    //92 = backslash
                    string_append_char(str, '\\');
                    state = STATE25_DOUBLE_QUOTATION_MARKS;
                } else if(c == 'n') {
                    string_append_char(str, '\n');
                    state = STATE25_DOUBLE_QUOTATION_MARKS;
                } else if(c == 'r') {
                    string_append_char(str, '\r');
                    state = STATE25_DOUBLE_QUOTATION_MARKS;
                } else if(c == 't') {
                    string_append_char(str, '\t');
                    state = STATE25_DOUBLE_QUOTATION_MARKS;
                } else if(c == 'x') {
                    state = STATE27_ESCAPE_BACKSLASH_X;
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro escape sekvenci ve stringu
                    break;
                }
                break;
            /*
            ---------------------------
            Stav 27: ESCAPE_BACKSLASH_X
            ---------------------------
            */
            case STATE27_ESCAPE_BACKSLASH_X:
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(isdigit(c) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102)) {   //0-9, A-F, a-f
                    if(isdigit(c)) {
                        x = 16*c;
                    }
                    if(c == 'A' || c == 'a') {
                        x = 16*10;
                    }
                    if(c == 'B' || c == 'b') {
                        x = 16*11;
                    }
                    if(c == 'C' || c == 'c') {
                        x = 16*12;
                    }
                    if(c == 'D' || c == 'd') {
                        x = 16*13;
                    }
                    if(c == 'E' || c == 'e') {
                        x = 16*14;
                    }
                    if(c == 'F' || c == 'f') {
                        x = 16*15;
                    }
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro X escape sekvenci ve stringu
                    break;
                }

                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(isdigit(c) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102)) {
                    if(isdigit(c)) {
                        x = x + c;
                    }
                    if(c == 'A' || c == 'a') {
                        x = x + 10;
                    }
                    if(c == 'B' || c == 'b') {
                        x = x + 11;
                    }
                    if(c == 'C' || c == 'c') {
                        x = x + 12;
                    }
                    if(c == 'D' || c == 'd') {
                        x = x + 13;
                    }
                    if(c == 'E' || c == 'e') {
                        x = x + 14;
                    }
                    if(c == 'F' || c == 'f') {
                        x = x + 15;
                    }
                } else {
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro X escape sekvenci ve stringu
                    break;
                }

                string_append_char(str, (char)x);
                state = STATE25_DOUBLE_QUOTATION_MARKS;
                break;
            /*
            ------------------
            Stav 28: BACKSLASH
            ------------------
            */
            case STATE28_BACKSLASH:
                //printf("S28 Start\n");
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == 92) {//92 = backslash
                    //printf("S28 if\n");
                    state = STATE29_DOUBLE_BACKSLASH;
                    break;
                } else {
                    //printf("S28 else\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načten nekompatibilní znak do tokenu stringu s variantou backslash
                    break;
                }
            /*
            -------------------------
            Stav 29: DOUBLE_BACKSLASH
            -------------------------
            */
            case STATE29_DOUBLE_BACKSLASH:
                //printf("S29 Start\n");
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(c == 10) {   //10 = LF
                    //printf("S29 if\n");
                    state = STATE30_DOUBLE_BACKSLASH_LF;
                    break;
                } else {
                    //printf("S29 else\n");
                    string_append_char(str, (char)c);
                    break;
                }
            /*
            -----------------------------
            Stav 30: DOUBLE_BACKSLASH_EOL
            -----------------------------
            */
            case STATE30_DOUBLE_BACKSLASH_LF:
                //printf("S30 Start\n");
                c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                while(c == ' ') {
                    c = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                }

                if(c != 92) {
                    //printf("S30 if1\n");
                    lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                    scanner_ungetChar(c);
                    stopFSM = true;
                    break;
                }
                //printf("S30 Mid\n");
                c = scanner_getNextChar();
                if(c != 92) {
                    //printf("S30 if2\n");
                    stopFSM = true;
                    Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načten nekompatibilní znak do tokenu stringu s variantou backslash
                    break;
                }
                //printf("S30 End\n");
                string_append_char(str, '\n');
                state = STATE29_DOUBLE_BACKSLASH;
                break;
            /*
            --------------------
            Stav DEFAULT (ERROR)
            --------------------
            */
            default: //stateFSM ERROR
                stopFSM = true;
                Parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
        }
    }

    if(lexToken.value == NULL || strcmp(lexToken.value->str, "import") == 0 || (keytest != 0 && keytest != 1)) {
        string_free(str);
    }
    return lexToken;
}

inline Token scanner_getNextToken() {  //Převaděč Tokenu pro Parser (vlastně nepotřebné, Token lze brát přímo z FSM)
    return scanner_FSM();
}
