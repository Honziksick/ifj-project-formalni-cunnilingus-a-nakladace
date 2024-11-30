/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   30.11.2024                                                *
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
 * @details Tento soubor obsahuje implementaci funkcí a datových typů modulu
 *          scannner, které jsou deklarovány v souboru scanner.h.
 */


#include "scanner.h"
#include "parser_common.h"

//NEW STUFF
int lex_char;
bool lex_stopFSM;
bool lex_keyflag;
StateFSM lex_state;

/**
 * @brief Získá znak ze vstupu programu.
 */
inline int scanner_getNextChar() {      // Čte jeden znak ze souboru
    return getchar();
}

/**
 * @brief Vrátí potřebný znak zpět na vstup programu.
 */
inline void scanner_ungetChar(int c) {    // Vrátí char zpět do vstupního proudu
    ungetc(c, stdin);
}

/**
 * @brief Rozhodne o identitě znaku.
 */
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

/**
 * @brief V rámci FSM rozhodne o tom, zda je načtený řetězec znaků klíčovým slovem.
 */
Token scanner_isKeyword(DString *value) {
    if       (strcmp(value->str, "const") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_const);
    } else if(strcmp(value->str, "var") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_var);
    } else if(strcmp(value->str, "i32") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_i32);
    } else if(strcmp(value->str, "f64") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_f64);
    } else if(strcmp(value->str, "pub") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_pub);
    } else if(strcmp(value->str, "fn") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_fn);
    } else if(strcmp(value->str, "void") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_void);
    } else if(strcmp(value->str, "return") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_return);
    } else if(strcmp(value->str, "null") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_null);
    } else if(strcmp(value->str, "if") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_if);
    } else if(strcmp(value->str, "else") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_else);
    } else if(strcmp(value->str, "while") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_while);
    } else if(strcmp(value->str, "_") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_underscore);
    } else if(strcmp(value->str, "ifj") == 0) {
        lex_keyflag = true;
        return scanner_stringlessTokenCreate(TOKEN_K_ifj);
    } else {
        return scanner_tokenCreate(TOKEN_IDENTIFIER, value);
    }
}

/**
 * @brief Vytvoří nový token.
 */
Token scanner_tokenCreate(TokenType type, DString *value) {
    Token token;
    token.type = type;
    token.value = value;
    return token;
}

/**
 * @brief Vytvoří nový token BEZ STRINGu.
 */
Token scanner_stringlessTokenCreate(TokenType type) {
    Token token;
    token.type = type;
    token.value = NULL;
    return token;
}

/**
 * @brief Inicializace scanneru.
 */
Token scanner_init() {
    //Inicializace FSM po jednom volání Syntaktického analyzátoru
    lex_char = 0;
    lex_stopFSM = false;
    lex_keyflag = false;
    Token lexToken = scanner_tokenCreate(TOKEN_UNINITIALIZED, NULL);
    
    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem \.
 */
Token scanner_stateComplexBackslash(Token lexToken, DString *str) {

    while(lex_stopFSM == false) {

        switch(lex_state) {
            /*
            ------------------
            Stav 20: BACKSLASH
            ------------------
            */
            case STATE20_BACKSLASH:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == 92) {    //92 = backslash
                    lex_state = STATE21_DOUBLE_BACKSLASH;
                    break;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načten nekompatibilní znak do tokenu stringu s variantou backslash
                    break;
                }
            /*
            -------------------------
            Stav 21: DOUBLE_BACKSLASH
            -------------------------
            */
            case STATE21_DOUBLE_BACKSLASH:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == 10) {    //10 = LF
                    lex_state = STATE22_DOUBLE_BACKSLASH_LF;
                    break;
                } else {
                    string_append_char(str, (char)lex_char);
                    break;
                }
            /*
            -----------------------------
            Stav 22: DOUBLE_BACKSLASH_EOL
            -----------------------------
            */
            case STATE22_DOUBLE_BACKSLASH_LF:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                while(lex_char == ' ') {
                    lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                }

                if(lex_char != 92) {
                    lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                    scanner_ungetChar(lex_char);
                    lex_stopFSM = true;
                    break;
                }
                lex_char = scanner_getNextChar();
                if(lex_char != 92) {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);         //ERROR - načten nekompatibilní znak do tokenu stringu s variantou backslash
                    break;
                }
                string_append_char(str, '\n');
                lex_state = STATE21_DOUBLE_BACKSLASH;
                break;

            default:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);             //ERROR - chyba logiky
                break;
        }

    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem ".
 */
Token scanner_stateComplexQuotation(Token lexToken, DString *str) {

    int x = 0;  //Pomocná proměnná pro appendaci speciálních znaků v escape sekvencích.

    while(lex_stopFSM == false) {

        switch(lex_state) {
            /*
            -------------------------------
            Stav 17: DOUBLE_QUOTATION_MARKS
            -------------------------------
            */
            case STATE17_DOUBLE_QUOTATION_MARKS:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == '"') {
                    lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                    lex_stopFSM = true;
                } else if(lex_char == 92) {    //92 = backslash
                    lex_state = STATE18_ESCAPE_BACKSLASH;
                } else if(lex_char == EOF) {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - neukončení stringu
                } else {
                    string_append_char(str, (char)lex_char);
                }
                break;
            /*
            -------------------------
            Stav 18: ESCAPE_BACKSLASH
            -------------------------
            */
            case STATE18_ESCAPE_BACKSLASH:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == '"') {
                    string_append_char(str, '"');
                    lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
                } else if(lex_char == 92) {    //92 = backslash
                    string_append_char(str, '\\');
                    lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
                } else if(lex_char == 'n') {
                    string_append_char(str, '\n');
                    lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
                } else if(lex_char == 'r') {
                    string_append_char(str, '\r');
                    lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
                } else if(lex_char == 't') {
                    string_append_char(str, '\t');
                    lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
                } else if(lex_char == 'x') {
                    lex_state = STATE19_ESCAPE_BACKSLASH_X;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro escape sekvenci ve stringu
                    break;
                }
                break;
            /*
            ---------------------------
            Stav 19: ESCAPE_BACKSLASH_X
            ---------------------------
            */
            case STATE19_ESCAPE_BACKSLASH_X:
                x = 0;
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(isdigit(lex_char) || (lex_char >= 65 && lex_char <= 70) || (lex_char >= 97 && lex_char <= 102)) {   //0-9, A-F, a-f
                    if(isdigit(lex_char)) {
                        x = 16*lex_char;
                    }
                    if(lex_char == 'A' || lex_char == 'a') {
                        x = 16*10;
                    }
                    if(lex_char == 'B' || lex_char == 'b') {
                        x = 16*11;
                    }
                    if(lex_char == 'C' || lex_char == 'c') {
                        x = 16*12;
                    }
                    if(lex_char == 'D' || lex_char == 'd') {
                        x = 16*13;
                    }
                    if(lex_char == 'E' || lex_char == 'e') {
                        x = 16*14;
                    }
                    if(lex_char == 'F' || lex_char == 'f') {
                        x = 16*15;
                    }
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načten nekompatibilní znak pro X escape sekvenci ve stringu
                    break;
                }

                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(isdigit(lex_char) || (lex_char >= 65 && lex_char <= 70) || (lex_char >= 97 && lex_char <= 102)) {
                    if(isdigit(lex_char)) {
                        x = x + lex_char;
                    }
                    if(lex_char == 'A' || lex_char == 'a') {
                        x = x + 10;
                    }
                    if(lex_char == 'B' || lex_char == 'b') {
                        x = x + 11;
                    }
                    if(lex_char == 'C' || lex_char == 'c') {
                        x = x + 12;
                    }
                    if(lex_char == 'D' || lex_char == 'd') {
                        x = x + 13;
                    }
                    if(lex_char == 'E' || lex_char == 'e') {
                        x = x + 14;
                    }
                    if(lex_char == 'F' || lex_char == 'f') {
                        x = x + 15;
                    }
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);     //ERROR - načten nekompatibilní znak pro X escape sekvenci ve stringu
                    break;
                }

                string_append_char(str, (char)x);
                lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
                break;

            default:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);         //ERROR - chyba logiky
                break;
        }
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování klíčových slov ?i32, ?f64 a ?[]u8.
 */
Token scanner_stateComplexQuestion(Token lexToken) {

    while(lex_stopFSM == false) {
        
        switch(lex_state) {
            /*
            --------------------
            Stav 9: STATE9_QMARK
            --------------------
            */
            case STATE9_QMARK:
                lex_char = scanner_getNextChar();
                if(isspace(lex_char)) {
                    //Nic
                } else if(lex_char == 'i') {
                    lex_state = STATE10_QMARK_i32_A;
                } else if(lex_char == 'f') {
                    lex_state = STATE11_QMARK_f64_A;
                } else if(lex_char == '[') {
                    lex_state = STATE12_QMARK_u8_A;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            --------------------------
            Stav 10: STATE10_QMARK_i32_A
            --------------------------
            */
            case STATE10_QMARK_i32_A:
                lex_char = scanner_getNextChar();
                if(lex_char == '3') {
                    lex_state = STATE13_QMARK_i32_B;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            ----------------------------
            Stav 11: STATE11_QMARK_f64_A
            ----------------------------
            */
            case STATE11_QMARK_f64_A:
                lex_char = scanner_getNextChar();
                if(lex_char == '6') {
                    lex_state = STATE14_QMARK_f64_B;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            ---------------------------
            Stav 12: STATE11_QMARK_u8_A
            ---------------------------
            */
            case STATE12_QMARK_u8_A:
                lex_char = scanner_getNextChar();
                if(lex_char == ']') {
                    lex_state = STATE15_QMARK_u8_B;
                } else if(isspace(lex_char)) {
                    //Nic
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            ----------------------------
            Stav 13: STATE13_QMARK_i32_B
            ----------------------------
            */
            case STATE13_QMARK_i32_B:
                lex_char = scanner_getNextChar();
                if(lex_char == '2') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qi32);
                    lex_stopFSM = true;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            ----------------------------
            Stav 14: STATE14_QMARK_f64_B
            ----------------------------
            */
            case STATE14_QMARK_f64_B:
                lex_char = scanner_getNextChar();
                if(lex_char == '4') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qf64);
                    lex_stopFSM = true;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            ---------------------------
            Stav 15: STATE15_QMARK_u8_B
            ---------------------------
            */
            case STATE15_QMARK_u8_B:
                lex_char = scanner_getNextChar();
                if(lex_char == 'u') {
                    lex_state = STATE16_QMARK_u8_C;
                } else if(isspace(lex_char)) {
                    //Nic
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;
            /*
            ---------------------------
            Stav 16: STATE16_QMARK_u8_C
            ---------------------------
            */
            case STATE16_QMARK_u8_C:
                lex_char = scanner_getNextChar();
                if(lex_char == '8') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qu8);
                    lex_stopFSM = true;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu NON
                }
                break;

            default:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);         //ERROR - chyba logiky
                break;
        }
    }
    
    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování klíčového slova []u8.
 */
Token scanner_stateComplexLeftSqrBr(Token lexToken) {

    while(lex_stopFSM == false) {

        switch(lex_state) {
            /*
            --------------------
            Stav 6: STATE5_u8_A
            --------------------
            */
            case STATE6_u8_A:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == ']') {
                    lex_state = STATE7_u8_B;
                } else if(isspace(lex_char)) {
                    //Nic
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                    break;
                }
                break;
            /*
            --------------------
            Stav 7: STATE6_u8_B
            --------------------
            */
            case STATE7_u8_B:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == 'u') {
                    lex_state = STATE8_u8_C;
                } else if(isspace(lex_char)) {
                    //Nic
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                    break;
                }
                break;
            /*
            -------------------
            Stav 8: STATE7_u8_C
            -------------------
            */
            case STATE8_u8_C:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                if(lex_char == '8') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_u8);
                    lex_stopFSM = true;
                } else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtení nekompatibilního znaku do tokenu []u8
                    break;
                }
                break;
            
            default:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);         //ERROR - chyba logiky
                break;
        }
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování klíčového slova @import.
 */
Token scanner_stateComplexAtSign(Token lexToken, DString *value) {
    
    while(lex_stopFSM == false) {

        lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

        switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
            case LETTER:
                string_append_char(value, (char)lex_char);
                break;
            case NOT_IN_LANGUAGE:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
            case WHITESPACE:
                if(strcmp(value->str, "import") == 0) {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                    lex_keyflag = true;
                    lex_stopFSM = true;
                }
                else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            default:    //NUMBER + SIMPLE + COMPLEX + CHAR_EOF
                scanner_ungetChar(lex_char);
                if(strcmp(value->str, "import") == 0) {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                    lex_keyflag = true;
                    lex_stopFSM = true;
                }
                else {
                    lex_stopFSM = true;
                    
                    Parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
        }
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování komentářů //.
 */
void scanner_stateComplexDoubleSlash() {

    while(lex_stopFSM == false) {

        lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

        switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
            case WHITESPACE:
                if(lex_char == 10) {   //10 = LF
                    lex_stopFSM = true;
                }
                break;
            case CHAR_EOF:
                scanner_ungetChar(lex_char);
                lex_stopFSM = true;
                break;
            default:    //LETTER + NUMBER + NOT_IN_LANGUAGE + SIMPLE + COMPLEX
                break;
        }
    }

    return;
}

/**
 * @brief Funkce scanneru pro zpracování složitého operátoru /.
 */
Token scanner_stateComplexSlash(Token lexToken) {
    lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

    switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
        case NOT_IN_LANGUAGE:
            lex_stopFSM = true;
            
            Parser_errorWatcher(SET_ERROR_LEXICAL);
            break;
        case WHITESPACE:
            lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
            lex_stopFSM = true;
            break;
        case COMPLEX:
            if(lex_char == '/') {
                scanner_stateComplexDoubleSlash();
                lex_stopFSM = false;
            } else {
                lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                lex_stopFSM = true;
                scanner_ungetChar(lex_char);
            }
            break;
        default:    //LETTER + NUMBER + SIMPLE + CHAR_EOF
            lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
            lex_stopFSM = true;
            scanner_ungetChar(lex_char);
            break;
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování složitých operátorů = a ==.
 */
Token scanner_stateComplexEqual(Token lexToken) {
    lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

    if(lex_char == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUAL_TO);
        lex_stopFSM = true;
    } else {
        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
        scanner_ungetChar(lex_char);
        lex_stopFSM = true;
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování složitých operátorů > a >=.
 */
Token scanner_stateComplexGreater(Token lexToken) {
    lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

    if(lex_char == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_EQUAL_THAN);
        lex_stopFSM = true;
    } else {
        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
        scanner_ungetChar(lex_char);
        lex_stopFSM = true;
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování složitých operátorů < a <=.
 */
Token scanner_stateComplexLess(Token lexToken) {
    lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

    if(lex_char == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_EQUAL_THAN);
        lex_stopFSM = true;
    } else {
        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
        scanner_ungetChar(lex_char);
        lex_stopFSM = true;
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování složitého operátoru !=.
 */
Token scanner_stateComplexExclamation(Token lexToken) {
    lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

    if(lex_char == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_NOT_EQUAL_TO);
        lex_stopFSM = true;
    } else {
        lex_stopFSM = true;
        
        Parser_errorWatcher(SET_ERROR_LEXICAL);
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování a řízení zpracování složitých operátorů.
 */
Token scanner_stateComplexControl(Token lexToken, int loc_lex_char, DString *str) {
    
    switch(loc_lex_char) {
        case '.':
            lexToken = scanner_stringlessTokenCreate(TOKEN_PERIOD);
            lex_stopFSM = true;
            break;
        case '!':
            lexToken = scanner_stateComplexExclamation(lexToken);
            break;
        case '<':
            lexToken = scanner_stateComplexLess(lexToken);
            break;
        case '>':
            lexToken = scanner_stateComplexGreater(lexToken);
            break;
        case '=':
            lexToken = scanner_stateComplexEqual(lexToken);
            break;
        case '/':
            lexToken = scanner_stateComplexSlash(lexToken);
            break;
        case '@':
            lexToken = scanner_stateComplexAtSign(lexToken, str);
            break;
        case '[':
            lex_state = STATE6_u8_A;
            lexToken = scanner_stateComplexLeftSqrBr(lexToken);
            break;
        case ']':
            lex_stopFSM = true;
            
            Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtený znak ] bez [ před ním
            break;
        case '?':
            lex_state = STATE9_QMARK;
            lexToken = scanner_stateComplexQuestion(lexToken);
            break;
        case '"':
            lex_state = STATE17_DOUBLE_QUOTATION_MARKS;
            lexToken = scanner_stateComplexQuotation(lexToken, str);
            break;
        case 92:    // 92 = backslash
            lex_state = STATE20_BACKSLASH;
            lexToken = scanner_stateComplexBackslash(lexToken, str);
            break;
        default:
            lex_stopFSM = true;
            
            Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtený COMPLEX není validní
            break;
    }

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování jednoduchých operátorů.
 */
Token scanner_stateSimple(Token lexToken, int loc_lex_char) {
        
    switch(loc_lex_char) {
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
        default:
            
            Parser_errorWatcher(SET_ERROR_LEXICAL);    //ERROR - načtený SIMPLE není validní
            break;
    }

    lex_stopFSM = true;

    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi.
 */
Token scanner_stateNumbers(Token lexToken, DString *str) {
    while(lex_stopFSM == false) {

        switch(lex_state) {
            /*
            ---------------
            Stav 1: NUMBERS
            ---------------
            */
            case STATE1_NUMBERS:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

                switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        if(lex_char == 'e') {
                            string_append_char(str, (char)lex_char);
                            lex_state = STATE3_FLOAT_EXP;
                        } else {
                            lex_stopFSM = true;
                            
                            Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načítá se písmeno do Tokenu Int
                        }
                        break;
                    case NUMBER:
                        string_append_char(str, (char)lex_char);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        lex_stopFSM = true;
                        break;
                    case NOT_IN_LANGUAGE:
                        lex_stopFSM = true;
                        
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    case COMPLEX:
                        if(lex_char == '.') {
                            string_append_char(str, (char)lex_char);
                            lex_state = STATE2_FLOAT_PERIOD;
                        } else {
                            lexToken = scanner_tokenCreate(TOKEN_INT, str);
                            scanner_ungetChar(lex_char);
                            lex_stopFSM = true;
                        }
                        break;
                    default:    //SIMPLE + CHAR_EOF
                        lexToken = scanner_tokenCreate(TOKEN_INT, str);
                        scanner_ungetChar(lex_char);
                        lex_stopFSM = true;
                        break;
                }
                break;
            /*
            --------------------
            Stav 2: FLOAT_PERIOD
            --------------------
            */
            case STATE2_FLOAT_PERIOD:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

                switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
                    case NUMBER:
                        string_append_char(str, (char)lex_char);
                        lex_state = STATE5_FLOAT_AFTER_PERIOD;
                        break;
                    default:    //LETTER + NOT_IN_LANGUAGE + SIMPLE + COMPLEX + CHAR_EOF
                        lex_stopFSM = true;
                        
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - nebylo načteno číslo do Tokenu nehotového floatu
                        break;
                }
                break;
            /*
            -----------------
            Stav 3: FLOAT_EXP
            -----------------
            */
            case STATE3_FLOAT_EXP:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

                switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
                    case SIMPLE:
                        if(lex_char == '+') {
                            string_append_char(str, (char)lex_char);
                            lex_state = STATE4_FLOAT;
                        } else if (lex_char == '-') {
                            string_append_char(str, (char)lex_char);
                            lex_state = STATE4_FLOAT;
                        } else {
                            lex_stopFSM = true;
                            
                            Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - nebylo načteno +/- do Tokenu nehotového floatu varianty exponent
                        }
                        break;
                    case NUMBER:
                        string_append_char(str, (char)lex_char);
                        lex_state = STATE4_FLOAT;
                        break;
                    default:    //LETTER + NOT_IN_LANGUAGE + COMPLEX + CHAR_EOF
                        lex_stopFSM = true;
                        
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - nebylo načteno číslo do Tokenu nehotového floatu
                        break;
                }
                break;
            /*
            -------------
            Stav 4: FLOAT
            -------------
            */
            case STATE4_FLOAT:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

                switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        lex_stopFSM = true;
                        
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načítá se písmeno do Tokenu Float
                        break;
                    case NUMBER:
                        string_append_char(str, (char)lex_char);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        lex_stopFSM = true;
                        break;
                    case NOT_IN_LANGUAGE:
                        lex_stopFSM = true;
                        
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    default:    //SIMPLE + COMPLEX + CHAR_EOF
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        scanner_ungetChar(lex_char);
                        lex_stopFSM = true;
                        break;
                }
                break;
            /*
            --------------------------
            Stav 5: FLOAT_AFTER_PERIOD
            --------------------------
            */
            case STATE5_FLOAT_AFTER_PERIOD:
                lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

                switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
                    case LETTER:
                        if(lex_char == 'e') {
                            string_append_char(str, (char)lex_char);
                            lex_state = STATE3_FLOAT_EXP;
                        } else {
                            lex_stopFSM = true;
                            
                            Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načítá se písmeno do Tokenu Int
                        }
                        break;
                    case NUMBER:
                        string_append_char(str, (char)lex_char);
                        break;
                    case WHITESPACE:
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        lex_stopFSM = true;
                        break;
                    case NOT_IN_LANGUAGE:
                        lex_stopFSM = true;
                        
                        Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                        break;
                    default:    //SIMPLE + COMPLEX + CHAR_EOF
                        lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                        scanner_ungetChar(lex_char);
                        lex_stopFSM = true;
                        break;
                }
                break;
            
            default:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);         //ERROR - chyba logiky
                break;
        }
    }
    return lexToken;
}

/**
 * @brief Funkce scanneru pro zpracování řetězce s písmeny.
 */
Token scanner_stateLetters(Token lexToken, DString *str) {
    while(lex_stopFSM == false) {
        lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

        switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
            case LETTER:
                string_append_char(str, (char)lex_char);
                break;
            case NUMBER:
                string_append_char(str, (char)lex_char);
                break;
            case WHITESPACE:
                lexToken = scanner_isKeyword(str);
                lex_stopFSM = true;
                break;
            case NOT_IN_LANGUAGE:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                break;
            default:    //SIMPLE + COMPLEX + CHAR_EOF
                lexToken = scanner_isKeyword(str);
                scanner_ungetChar(lex_char);
                lex_stopFSM = true;
                break;
        }
    }
    return lexToken;
}

/**
 * @brief Hlavní řídící funkce scanneru.
 */
Token scanner_FSM() {
    DString *str = string_init();
    Token lexToken = scanner_init();
    
    while(lex_stopFSM == false) {
        lex_char = scanner_getNextChar();  //Vstup jednoho znaku z STDIN

        switch (scanner_charIdentity(lex_char)) {  //Identifikace znaku mezi 29 typů
            case LETTER:
                string_append_char(str, (char)lex_char);
                lexToken = scanner_stateLetters(lexToken, str);
                break;
            case NUMBER:
                string_append_char(str, (char)lex_char);
                lex_state = STATE1_NUMBERS;
                lexToken = scanner_stateNumbers(lexToken, str);
                break;
            case WHITESPACE:
                break;
            case NOT_IN_LANGUAGE:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - načtený znak nepatří mezi znaky jazyka
                break;
            case SIMPLE:
                lexToken = scanner_stateSimple(lexToken, lex_char);
                break;
            case COMPLEX:
                lexToken = scanner_stateComplexControl(lexToken, lex_char, str);
                break;
            case CHAR_EOF:
                lexToken = scanner_stringlessTokenCreate(TOKEN_EOF);
                scanner_ungetChar(lex_char);
                lex_stopFSM = true;
                break;
            default:
                lex_stopFSM = true;
                
                Parser_errorWatcher(SET_ERROR_LEXICAL);        //ERROR - charIdentity vrací default
                break;
            }
        }

    if(lexToken.value == NULL || lex_keyflag == true) {
        string_free(str);
    }

    return lexToken;
}

/**
 * @brief Získá jeden Token.
 */
inline Token scanner_getNextToken() {
    return scanner_FSM();
}

/*** Konec souboru scanner.c ***/
