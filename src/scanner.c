/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.c                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            06.10.2024                                                *
 * Poslední změna:   03.12.2024                                                *
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



/**
 * @brief Získá znak ze vstupu programu.
 */
inline int scanner_getNextChar() {
    return getchar();
}

/**
 * @brief Vrátí potřebný znak zpět na vstup programu.
 */
inline void scanner_ungetChar(int c) {
    ungetc(c, stdin);
}

/**
 * @brief Rozhodne o identitě znaku.
 */
CharType scanner_charIdentity(int c) {
    // Pokud je c písmeno nebo podtržítko
    if(isalpha(c) || c == '_') {
        return LETTER;
    }
    // Pokud je c číslice
    else if(isdigit(c)) {
        return NUMBER;
    }
    // Pokud je c bílý znak
    else if(isspace(c)) {
        return WHITESPACE;
    }
    // Pokud je c znak, který nepatří do jazyka
    else if(c == '#' || c == '$' || c == '%' ||
            c == '&' || c == ASQ || c == '^' ||
            c == '`' || c == '~' || c >= ADL) {
        return NOT_IN_LANGUAGE;
    }
    // Pokud je c jednouduchý operátor
    else if(c == '(' || c == ')' || c == '*' ||
            c == '+' || c == ',' || c == '-' ||
            c == ':' || c == ';' || c == '{' ||
            c == '|' || c == '}') { //c je speciální jednoduchý symbol
                                    //(special SIMPLE)
        return SIMPLE;
    }
    // Pokud je c složitý operátor
    else if(c == '!' || c == '"' || c == '.' ||
            c == '/' || c == '<' || c == '=' ||
            c == '>' || c == '?' || c == '@' ||
            c == '[' || c == ABS || c == ']') { //c je specialní složitý symbol
                                                //(special COMPLEX)
        return COMPLEX;
    }
    // Pokud je c EOF
    else if(c == EOF) {  //c je konec souboru
        return CHAR_EOF;
    }
    // Pokud je c není ani jedno z výše uvedených
    else {
        // c nespadá do žádné ze skupin znaků, jedná se o chybu
        parser_errorWatcher(SET_ERROR_LEXICAL);
        return CHAR_EOF;    //V případě erroru dává smysl sem vložit návratovou
                            //hodnotu EOF
    }
}   // scanner_charIdentity

/**
 * @brief V rámci FSM rozhodne o tom, zda je načtený řetězec znaků klíčovým slovem.
 */
Token scanner_isKeyword(DString *value) {
    // Pokud je ve stringu value "const"
    if     (strcmp(value->str, "const") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_const);
    }
    // Pokud je ve stringu value "var"
    else if(strcmp(value->str, "var") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_var);
    }
    // Pokud je ve stringu value "i32"
    else if(strcmp(value->str, "i32") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_i32);
    }
    // Pokud je ve stringu value "f64"
    else if(strcmp(value->str, "f64") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_f64);
    }
    // Pokud je ve stringu value "pub"
    else if(strcmp(value->str, "pub") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_pub);
    }
    // Pokud je ve stringu value "fn"
    else if(strcmp(value->str, "fn") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_fn);
    }
    // Pokud je ve stringu value "void"
    else if(strcmp(value->str, "void") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_void);
    }
    // Pokud je ve stringu value "return"
    else if(strcmp(value->str, "return") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_return);
    }
    // Pokud je ve stringu value "null"
    else if(strcmp(value->str, "null") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_null);
    }
    // Pokud je ve stringu value "if"
    else if(strcmp(value->str, "if") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_if);
    }
    // Pokud je ve stringu value "else"
    else if(strcmp(value->str, "else") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_else);
    }
    // Pokud je ve stringu value "while"
    else if(strcmp(value->str, "while") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_while);
    }
    // Pokud je ve stringu value "_"
    else if(strcmp(value->str, "_") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_underscore);
    }
    // Pokud je ve stringu value "ifj"
    else if(strcmp(value->str, "ifj") == 0) {
        return scanner_stringlessTokenCreate(TOKEN_K_ifj);
    }
    // Pokud je ve stringu value jiný řetězec znaků
    else {
        return scanner_tokenCreate(TOKEN_IDENTIFIER, value);
    }
}   // scanner_isKeyword

/**
 * @brief Vytvoří nový token.
 */
Token scanner_tokenCreate(TokenType type, DString *value) {
    Token token;
    token.type = type;
    token.value = value;
    return token;
}   // scanner_tokenCreate

/**
 * @brief Vytvoří nový token BEZ STRINGu.
 */
Token scanner_stringlessTokenCreate(TokenType type) {
    Token token;
    token.type = type;
    token.value = NULL;
    return token;
}   // scanner_stringlessTokenCreate

/**
 * @brief Inicializace scanneru.
 */
Token scanner_init() {
    // Inicializace FSM po jednom volání Syntaktického analyzátoru
    Token lexToken = scanner_tokenCreate(TOKEN_UNINITIALIZED, NULL);

    return lexToken;
}   // scanner_init

/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem \.
 */
Token scanner_stateComplexBackslash(Token lexToken, DString *str) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Inicializuj lexState
    StateFSM lexState = STATE15_BACKSLASH;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Abstraktně: vybírej stav, podle pomocné proměnné lexState
        switch(lexState) {
            /*
            -------------------------
            Stav 1: STATE15_BACKSLASH
            -------------------------
            */
            case STATE15_BACKSLASH:
                lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
                // Pokud lexChar je backslash
                if(lexChar == ABS) {
                    lexState = STATE16_DOUBLE_BACKSLASH;
                    break;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načten nekompatibilní znak do tokenu stringu
                    // s variantou backslash
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    break;
                }
            /*
            --------------------------------
            Stav 2: STATE16_DOUBLE_BACKSLASH
            --------------------------------
            */
            case STATE16_DOUBLE_BACKSLASH:
                lexChar = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                // Pokud lexChar je znak nového řádku (LF)
                if(lexChar == ALF) {
                    lexState = STATE17_DOUBLE_BACKSLASH_LF;
                    break;
                }
                // Jinak
                else {
                    DString_appendChar(str, (char)lexChar);
                    break;
                }
            /*
            ------------------------------------
            Stav 17: STATE18_DOUBLE_BACKSLASH_LF
            ------------------------------------
            */
            case STATE17_DOUBLE_BACKSLASH_LF:
                lexChar = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                // Cykluj, dokud lexChar je mezera
                while(lexChar == ' ') {
                    lexChar = scanner_getNextChar();  //Vstup jednoho znaku z STDIN
                }
                // Pokud lexChar není backslash
                if(lexChar != ABS) {
                    lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                    scanner_ungetChar(lexChar);
                    lexStopFSM = true;
                    break;
                }
                lexChar = scanner_getNextChar();
                // Pokud lexChar není backslash
                if(lexChar != ABS) {
                    lexStopFSM = true;
                    // ERROR - načten nekompatibilní znak do tokenu stringu s variantou backslash
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    break;
                }
                DString_appendChar(str, '\n');
                lexState = STATE16_DOUBLE_BACKSLASH;
                break;

            default:
                lexStopFSM = true;
                // ERROR - chyba logiky
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
        }

    }

    return lexToken;
}   // scanner_stateComplexBackslash

/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem ".
 */
Token scanner_stateComplexQuotation(Token lexToken, DString *str) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Inicializuj lexState
    StateFSM lexState = STATE12_DOUBLE_QUOTATION_MARKS;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Abstraktně: vybírej stav, podle pomocné proměnné lexState
        switch(lexState) {
            /*
            --------------------------------------
            Stav 1: STATE12_DOUBLE_QUOTATION_MARKS
            --------------------------------------
            */
            case STATE12_DOUBLE_QUOTATION_MARKS:
                lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
                // Pokud lexChar je "
                if(lexChar == '"') {
                    lexToken = scanner_tokenCreate(TOKEN_STRING, str);
                    lexStopFSM = true;
                }
                // Pokud lexChar je "
                else if(lexChar == ABS) {
                    lexState = STATE13_ESCAPE_BACKSLASH;
                }
                // Pokud lexChar je znak konce souboru (EOF)
                else if(lexChar == EOF) {
                    lexStopFSM = true;
                    // ERROR - neukončení stringu
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                // Jinak
                else {
                    DString_appendChar(str, (char)lexChar);
                }
                break;
            /*
            --------------------------------
            Stav 2: STATE13_ESCAPE_BACKSLASH
            --------------------------------
            */
            case STATE13_ESCAPE_BACKSLASH:
                lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
                // Pokud lexChar je "
                if(lexChar == '"') {
                    DString_appendChar(str, '"');
                    lexState = STATE12_DOUBLE_QUOTATION_MARKS;
                }
                // Pokud lexChar je backslash
                else if(lexChar == ABS) {
                    DString_appendChar(str, '\\');
                    lexState = STATE12_DOUBLE_QUOTATION_MARKS;
                }
                // Pokud lexChar je n
                else if(lexChar == 'n') {
                    DString_appendChar(str, '\n');
                    lexState = STATE12_DOUBLE_QUOTATION_MARKS;
                }
                // Pokud lexChar je r
                else if(lexChar == 'r') {
                    DString_appendChar(str, '\r');
                    lexState = STATE12_DOUBLE_QUOTATION_MARKS;
                }
                // Pokud lexChar je t
                else if(lexChar == 't') {
                    DString_appendChar(str, '\t');
                    lexState = STATE12_DOUBLE_QUOTATION_MARKS;
                }
                // Pokud lexChar je x
                else if(lexChar == 'x') {
                    lexState = STATE14_ESCAPE_BACKSLASH_X;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načten nekompatibilní znak pro escape sekvenci ve stringu
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    break;
                }
                break;
            /*
            ----------------------------------
            Stav 3: STATE14_ESCAPE_BACKSLASH_X
            ----------------------------------
            */
            case STATE14_ESCAPE_BACKSLASH_X:
                lexChar = scanner_getNextChar();

                // Pokud je načtený znak číslicí hexadecimální soustavy
                if(isxdigit(lexChar)) {
                    // Vytvoříme pole o třech znacích (\xdd -> {d, d, '\0'})
                    // načtený je prvním znakem
                    char hexa[HEXADECIMAL_SEQ_SIZE] = {(char)lexChar, '\0', '\0'};

                    // Načteme další znak
                    lexChar = scanner_getNextChar();

                    // Pokud je následující načtený znak také hexadecimální číslicí
                    if(isxdigit(lexChar)) {
                        // Přidáme nově načtený znak jako druhý znak řetězce
                        hexa[1] = (char)lexChar;

                        // Provedeme řetězec reprezentující hexadecimální číslo na číslo
                        long int value = strtol(hexa, NULL, HEXA_BASE);

                        // Pokud převedená hodnota přetekla maximální ASCII hodnotu
                        if (value > ASCII_VALUE_MAX) {
                            // hlásíme error
                            lexStopFSM = true;
                            parser_errorWatcher(SET_ERROR_LEXICAL);
                        }
                        // Jinak reprezentujeme číslo jako ASCII znak a připojíme ho
                        else {
                            DString_appendChar(str, (char)value);
                            lexState = STATE12_DOUBLE_QUOTATION_MARKS;
                        }
                    }
                    // Pokud načtený znak nebyl hexadecimální číslicí, hlásíme error
                    else {
                        lexStopFSM = true;
                        parser_errorWatcher(SET_ERROR_LEXICAL);
                    }
                }
                // Pokud načtený znak nebyl hexadecimální číslicí, hlásíme error
                else {
                    lexStopFSM = true;
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }

                break;

            default:
                lexStopFSM = true;
                // ERROR - chyba logiky
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
        }
    }

    return lexToken;
}   // scanner_stateComplexQuotation

/**
 * @brief Funkce scanneru pro zpracování klíčových slov ?i32, ?f64 a ?[]u8.
 */
Token scanner_stateComplexQuestion(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Inicializuj lexState
    StateFSM lexState = STATE4_QMARK;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Abstraktně: vybírej stav, podle pomocné proměnné lexState
        switch(lexState) {
            /*
            --------------------
            Stav 1: STATE4_QMARK
            --------------------
            */
            case STATE4_QMARK:
                lexChar = scanner_getNextChar();
                if(isspace(lexChar)) {
                    // Nic
                }
                // Pokud lexChar je i
                else if(lexChar == 'i') {
                    lexState = STATE5_QMARK_i32_A;
                }
                // Pokud lexChar je f
                else if(lexChar == 'f') {
                    lexState = STATE6_QMARK_f64_A;
                }
                // Pokud lexChar je [
                else if(lexChar == '[') {
                    lexState = STATE7_QMARK_u8_A;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            --------------------------
            Stav 2: STATE5_QMARK_i32_A
            --------------------------
            */
            case STATE5_QMARK_i32_A:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je 3
                if(lexChar == '3') {
                    lexState = STATE8_QMARK_i32_B;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            ----------------------------
            Stav 3: STATE6_QMARK_f64_A
            ----------------------------
            */
            case STATE6_QMARK_f64_A:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je 5
                if(lexChar == '6') {
                    lexState = STATE9_QMARK_f64_B;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            ---------------------------
            Stav 4: STATE7_QMARK_u8_A
            ---------------------------
            */
            case STATE7_QMARK_u8_A:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je ]
                if(lexChar == ']') {
                    lexState = STATE10_QMARK_u8_B;
                }
                // Pokud lexChar je bílý znak
                else if(isspace(lexChar)) {
                    // Nic
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            ----------------------------
            Stav 5: STATE8_QMARK_i32_B
            ----------------------------
            */
            case STATE8_QMARK_i32_B:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je 2
                if(lexChar == '2') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qi32);
                    lexStopFSM = true;
                // Jinak
                } else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            ----------------------------
            Stav 6: STATE9_QMARK_f64_B
            ----------------------------
            */
            case STATE9_QMARK_f64_B:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je 4
                if(lexChar == '4') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qf64);
                    lexStopFSM = true;
                // Jinak
                } else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            ---------------------------
            Stav 7: STATE10_QMARK_u8_B
            ---------------------------
            */
            case STATE10_QMARK_u8_B:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je u
                if(lexChar == 'u') {
                    lexState = STATE11_QMARK_u8_C;
                }
                // // Pokud lexChar je bílý znak
                else if(isspace(lexChar)) {
                    // Nic
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            /*
            ---------------------------
            Stav 8: STATE11_QMARK_u8_C
            ---------------------------
            */
            case STATE11_QMARK_u8_C:
                lexChar = scanner_getNextChar();
                // Pokud lexChar je 8
                if(lexChar == '8') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_Qu8);
                    lexStopFSM = true;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu NON
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;

            default:
                lexStopFSM = true;
                // ERROR - chyba logiky
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
        }
    }

    return lexToken;
}   // scanner_stateComplexQuestion

/**
 * @brief Funkce scanneru pro zpracování klíčového slova []u8.
 */
Token scanner_stateComplexLeftSqrBr(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Inicializuj lexState
    StateFSM lexState = STATE1_u8_A;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Abstraktně: vybírej stav, podle pomocné proměnné lexState
        switch(lexState) {
            /*
            --------------------
            Stav 1: STATE1_u8_A
            --------------------
            */
            case STATE1_u8_A:
                lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
                // Pokud lexChar je ]
                if(lexChar == ']') {
                    lexState = STATE2_u8_B;
                }
                // Pokud lexChar je bílý znak
                else if(isspace(lexChar)) {
                    // Nic
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu []u8
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    break;
                }
                break;
            /*
            --------------------
            Stav 2: STATE2_u8_B
            --------------------
            */
            case STATE2_u8_B:
                lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
                // // Pokud lexChar je u
                if(lexChar == 'u') {
                    lexState = STATE3_u8_C;
                }
                // Pokud lexChar je bílý znak
                else if(isspace(lexChar)) {
                    // Nic
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu []u8
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    break;
                }
                break;
            /*
            -------------------
            Stav 3: STATE3_u8_C
            -------------------
            */
            case STATE3_u8_C:
                lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
                // Pokud lexChar je 8
                if(lexChar == '8') {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_u8);
                    lexStopFSM = true;
                }
                // Jinak
                else {
                    lexStopFSM = true;
                    // ERROR - načtení nekompatibilního znaku do tokenu []u8
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    break;
                }
                break;

            default:
                lexStopFSM = true;
                // ERROR - chyba logiky
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
        }
    }

    return lexToken;
}   // scanner_stateComplexLeftSqrBr

/**
 * @brief Funkce scanneru pro zpracování klíčového slova @import.
 */
Token scanner_stateComplexAtSign(Token lexToken, DString *value) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {

        lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
        // Abstraktně: vybírej podle typu znaku
        switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
            // Pokud znak je písmeno
            case LETTER:
                DString_appendChar(value, (char)lexChar);
                break;
            // Pokud znak není v jazyce
            case NOT_IN_LANGUAGE:
                lexStopFSM = true;
                // ERROR - znak, co není v jazyce se dostal do zpracování Complex
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
            // Pokud znak je bílý znak
            case WHITESPACE:
                if(strcmp(value->str, "import") == 0) {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                    lexStopFSM = true;
                }
                else {
                    lexStopFSM = true;
                    // ERROR - řetězec znaků po znaku @ není roven "import"
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
            // Jinak
            default:    // NUMBER + SIMPLE + COMPLEX + CHAR_EOF
                scanner_ungetChar(lexChar);
                if(strcmp(value->str, "import") == 0) {
                    lexToken = scanner_stringlessTokenCreate(TOKEN_K_import);
                    lexStopFSM = true;
                }
                else {
                    lexStopFSM = true;
                    // ERROR - řetězec znaků po znaku @ není roven "import"
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                }
                break;
        }
    }

    return lexToken;
}   // scanner_stateComplexAtSign

/**
 * @brief Funkce scanneru pro zpracování komentářů //.
 */
void scanner_stateComplexDoubleSlash() {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {

        lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
        // Abstraktně: vybírej podle typu znaku
        switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
            // Pokud znak je bílý znak
            case WHITESPACE:
                if(lexChar == ALF) {
                    lexStopFSM = true;
                }
                break;
            // Pokud znak je znak konce souboru (EOF)
            case CHAR_EOF:
                scanner_ungetChar(lexChar);
                lexStopFSM = true;
                break;
            // Jinak
            default:    // LETTER + NUMBER + NOT_IN_LANGUAGE + SIMPLE + COMPLEX
                break;
        }
    }

    return;
}   // scanner_stateComplexDoubleSlash

/**
 * @brief Funkce scanneru pro zpracování složitého operátoru /.
 */
Token scanner_stateComplexSlash(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Abstraktně: vybírej podle typu znaku
    switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
        // Pokud znak není v jazyce
        case NOT_IN_LANGUAGE:
            // ERROR - do zpracování lomítka se dostal znak, co není jazyce
            parser_errorWatcher(SET_ERROR_LEXICAL);
            break;
        // Pokud znak je bílý znak
        case WHITESPACE:
            lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
            break;
        // Pokud znak je složitý operátor
        case COMPLEX:
            if(lexChar == '/') {
                scanner_stateComplexDoubleSlash();
                lexToken = scanner_stringlessTokenCreate(TOKEN_COMMENT);
            } else {
                lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
                scanner_ungetChar(lexChar);
            }
            break;
        // Jinak
        default:    // LETTER + NUMBER + SIMPLE + CHAR_EOF
            lexToken = scanner_stringlessTokenCreate(TOKEN_SLASH);
            scanner_ungetChar(lexChar);
            break;
    }

    return lexToken;
}   // scanner_stateComplexSlash

/**
 * @brief Funkce scanneru pro zpracování složitých operátorů = a ==.
 */
Token scanner_stateComplexEqual(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Pokud lexChar je =
    if(lexChar == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUAL_TO);
    }
    // Jinak
    else {
        lexToken = scanner_stringlessTokenCreate(TOKEN_EQUALITY_SIGN);
        scanner_ungetChar(lexChar);
    }

    return lexToken;
}   // scanner_stateComplexEqual

/**
 * @brief Funkce scanneru pro zpracování složitých operátorů > a >=.
 */
Token scanner_stateComplexGreater(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Pokud lexChar je =
    if(lexChar == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_EQUAL_THAN);
    }
    // Jinak
    else {
        lexToken = scanner_stringlessTokenCreate(TOKEN_GREATER_THAN);
        scanner_ungetChar(lexChar);
    }

    return lexToken;
}   // scanner_stateComplexGreater

/**
 * @brief Funkce scanneru pro zpracování složitých operátorů < a <=.
 */
Token scanner_stateComplexLess(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Pokud lexChar je =
    if(lexChar == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_EQUAL_THAN);
    }
    // Jinak
    else {
        lexToken = scanner_stringlessTokenCreate(TOKEN_LESS_THAN);
        scanner_ungetChar(lexChar);
    }

    return lexToken;
}   // scanner_stateComplexLess

/**
 * @brief Funkce scanneru pro zpracování složitého operátoru !=.
 */
Token scanner_stateComplexExclamation(Token lexToken) {
    // Inicializuj lexChar
    int lexChar;
    lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Pokud lexChar je =
    if(lexChar == '=') {
        lexToken = scanner_stringlessTokenCreate(TOKEN_NOT_EQUAL_TO);
    }
    // Jinak
    else {
        // ERROR - po znaku ! vždy musí následovat =, není tomu tak
        parser_errorWatcher(SET_ERROR_LEXICAL);
    }

    return lexToken;
}   // scanner_stateComplexExclamation

/**
 * @brief Funkce scanneru pro zpracování a řízení zpracování složitých operátorů.
 */
Token scanner_stateComplexControl(Token lexToken, int lexChar, DString *str) {
    // Vybirej podle konrétního znaku v lexLocChar
    switch(lexChar) {
        // Pokud znak je .
        case '.':
            lexToken = scanner_stringlessTokenCreate(TOKEN_PERIOD);
            break;
        // Pokud znak je !
        case '!':
            lexToken = scanner_stateComplexExclamation(lexToken);
            break;
        // Pokud znak je <
        case '<':
            lexToken = scanner_stateComplexLess(lexToken);
            break;
        // Pokud znak je >
        case '>':
            lexToken = scanner_stateComplexGreater(lexToken);
            break;
        // Pokud znak je =
        case '=':
            lexToken = scanner_stateComplexEqual(lexToken);
            break;
        // Pokud znak je /
        case '/':
            lexToken = scanner_stateComplexSlash(lexToken);
            break;
        // Pokud znak je @
        case '@':
            lexToken = scanner_stateComplexAtSign(lexToken, str);
            break;
        // Pokud znak je [
        case '[':
            lexToken = scanner_stateComplexLeftSqrBr(lexToken);
            break;
        // Pokud znak je ]
        case ']':
            // ERROR - načtený znak ] bez [ před ním
            parser_errorWatcher(SET_ERROR_LEXICAL);
            break;
        // Pokud znak je ?
        case '?':
            lexToken = scanner_stateComplexQuestion(lexToken);
            break;
        // Pokud znak je "
        case '"':
            lexToken = scanner_stateComplexQuotation(lexToken, str);
            break;
        // Pokud znak je backslash
        case ABS:
            lexToken = scanner_stateComplexBackslash(lexToken, str);
            break;
        // Jinak
        default:
            // ERROR - načtený COMPLEX není validní
            parser_errorWatcher(SET_ERROR_LEXICAL);
            break;
    }

    return lexToken;
}   // scanner_stateComplexControl

/**
 * @brief Funkce scanneru pro zpracování jednoduchých operátorů.
 */
Token scanner_stateSimple(Token lexToken, int lexChar) {
    // Vybirej podle konrétního znaku v lexLocChar
    switch(lexChar) {
        // Pokud znak je (
        case '(':
            lexToken = scanner_stringlessTokenCreate(TOKEN_LEFT_PARENTHESIS);
            break;
        // Pokud znak je )
        case ')':
            lexToken = scanner_stringlessTokenCreate(TOKEN_RIGHT_PARENTHESIS);
            break;
        // Pokud znak je *
        case '*':
            lexToken = scanner_stringlessTokenCreate(TOKEN_ASTERISK);
            break;
        // Pokud znak je +
        case '+':
            lexToken = scanner_stringlessTokenCreate(TOKEN_PLUS);
            break;
        // Pokud znak je ,
        case ',':
            lexToken = scanner_stringlessTokenCreate(TOKEN_COMMA);
            break;
        // Pokud znak je -
        case '-':
            lexToken = scanner_stringlessTokenCreate(TOKEN_MINUS);
            break;
        // Pokud znak je :
        case ':':
            lexToken = scanner_stringlessTokenCreate(TOKEN_COLON);
            break;
        // Pokud znak je ;
        case ';':
            lexToken = scanner_stringlessTokenCreate(TOKEN_SEMICOLON);
            break;
        // Pokud znak je {
        case '{':
            lexToken = scanner_stringlessTokenCreate(TOKEN_LEFT_CURLY_BRACKET);
            break;
        // Pokud znak je |
        case '|':
            lexToken = scanner_stringlessTokenCreate(TOKEN_VERTICAL_BAR);
            break;
        // Pokud znak je }
        case '}':
            lexToken = scanner_stringlessTokenCreate(TOKEN_RIGHT_CURLY_BRACKET);
            break;
        // Jinak
        default:
            // ERROR - načtený SIMPLE není validní
            parser_errorWatcher(SET_ERROR_LEXICAL);
            break;
    }

    return lexToken;
}   // scanner_stateSimple

/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PO přijetí exponenciálního znaku.
 */
Token scanner_stateNumbersFloat(Token lexToken, DString *str) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Abstraktně: cykluj, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Vstup jednoho znaku z STDIN
        lexChar = scanner_getNextChar();
        // Abstraktně: vybírej podle typu znaku
        switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
            // Pokud znak je písmeno
            case LETTER:
                // ERROR - načítá se písmeno do Tokenu Float
                parser_errorWatcher(SET_ERROR_LEXICAL);
                lexStopFSM = true;
                break;
            // Pokud znak je číslo
            case NUMBER:
                DString_appendChar(str, (char)lexChar);
                break;
            // Pokud znak je bílý znak
            case WHITESPACE:
                lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                lexStopFSM = true;
                break;
            // Pokud znak není v jazyce
            case NOT_IN_LANGUAGE:
                // ERROR - načtený znak nepatří mezi znaky jazyka
                parser_errorWatcher(SET_ERROR_LEXICAL);
                lexStopFSM = true;
                break;
            // Jinak
            default:    // SIMPLE + COMPLEX + CHAR_EOF
                lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                scanner_ungetChar(lexChar);
                lexStopFSM = true;
                break;
        }
    }

    return lexToken;
}   // scanner_stateNumbersFloat

/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PRO přijetí exponenciálního znaku.
 */
Token scanner_stateNumbersFloatExp(Token lexToken, DString *str) {
    int lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Abstraktně: vybírej podle typu znaku
    switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
        // Pokud znak je jednoduchý operátor
        case SIMPLE:
            // Pokud jednoduchý operátor je +
            if(lexChar == '+') {
                DString_appendChar(str, (char)lexChar);
                lexToken = scanner_stateNumbersFloat(lexToken, str);
            }
            // Pokud jednoduchý operátor je -
            else if (lexChar == '-') {
                DString_appendChar(str, (char)lexChar);
                lexToken = scanner_stateNumbersFloat(lexToken, str);
            }
            // Jinak
            else {
                // ERROR - nebylo načteno +/- do Tokenu nehotového floatu varianty exponent
                parser_errorWatcher(SET_ERROR_LEXICAL);
            }
            break;
        // Pokud znak je číslo
        case NUMBER:
            DString_appendChar(str, (char)lexChar);
            lexToken = scanner_stateNumbersFloat(lexToken, str);
            break;
        // Jinak
        default:    // LETTER + NOT_IN_LANGUAGE + COMPLEX + CHAR_EOF
            // ERROR - nebylo načteno číslo do Tokenu nehotového floatu
            parser_errorWatcher(SET_ERROR_LEXICAL);
            break;
    }

    return lexToken;
}   // scanner_stateNumbersFloatExp

/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PO přijetí desetinné tečky.
 */
Token scanner_stateNumbersAfterPeriod(Token lexToken, DString *str) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Abstraktně: cykluj, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Vstup jednoho znaku z STDIN
        lexChar = scanner_getNextChar();
        // Abstraktně: vybírej podle typu znaku
        switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
            // Pokud znak je písmeno
            case LETTER:
                // Pokud písmeno je e
                if(lexChar == 'e') {
                    DString_appendChar(str, (char)lexChar);
                    lexToken = scanner_stateNumbersFloatExp(lexToken, str);
                    lexStopFSM = true;
                }
                // Jinak
                else {
                    // ERROR - načítá se písmeno do Tokenu Int
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    lexStopFSM = true;
                }
                break;
            // Pokud znak je číslo
            case NUMBER:
                DString_appendChar(str, (char)lexChar);
                break;
            // Pokud znak je bílý znak
            case WHITESPACE:
                lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                lexStopFSM = true;
                break;
            // Pokud znak není v jazyce
            case NOT_IN_LANGUAGE:
                // ERROR - načtený znak nepatří mezi znaky jazyka
                parser_errorWatcher(SET_ERROR_LEXICAL);
                lexStopFSM = true;
                break;
            // Jinak
            default:    // SIMPLE + COMPLEX + CHAR_EOF
                lexToken = scanner_tokenCreate(TOKEN_FLOAT, str);
                scanner_ungetChar(lexChar);
                lexStopFSM = true;
                break;
        }
    }

    return lexToken;
}   // scanner_stateNumbersAfterPeriod

/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PRO přijetí desetinné tečky.
 */
Token scanner_stateNumbersFloatPeriod(Token lexToken, DString *str) {
    int lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
    // Abstraktně: vybírej podle typu znaku
    if (scanner_charIdentity(lexChar) == NUMBER) {  // Identifikace znaku
        // Pokud znak je číslo
            DString_appendChar(str, (char)lexChar);
            lexToken = scanner_stateNumbersAfterPeriod(lexToken, str);
    }
    // Jinak
    else {      // LETTER + NOT_IN_LANGUAGE + SIMPLE + COMPLEX + CHAR_EOF
        // ERROR - nebylo načteno číslo do Tokenu nehotového floatu
        parser_errorWatcher(SET_ERROR_LEXICAL);
    }
    return lexToken;
}   // scanner_stateNumbersFloatPeriod

/**
 * @brief Funkce scanneru pro úvodní zpracování řetězce s číslicemi.
 */
Token scanner_stateNumbers(Token lexToken, DString *str) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Abstraktně: cykluj, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        // Vstup jednoho znaku z STDIN
        lexChar = scanner_getNextChar();  
        // Identifikace znaku
        switch (scanner_charIdentity(lexChar)) {
            // Pokud znak je písmeno
            case LETTER:
                // Pokud je písmeno e
                if(lexChar == 'e') {
                    DString_appendChar(str, (char)lexChar);
                    lexToken = scanner_stateNumbersFloatExp(lexToken, str);
                    lexStopFSM = true;
                }
                // Jinak
                else {
                    // ERROR - načítá se špatné písmeno do Tokenu Int
                    parser_errorWatcher(SET_ERROR_LEXICAL);
                    lexStopFSM = true;
                }
                break;
            // Pokud znak je číslo
            case NUMBER:
                DString_appendChar(str, (char)lexChar);
                break;
            // Pokud znak je bílý znak
            case WHITESPACE:
                lexToken = scanner_tokenCreate(TOKEN_INT, str);
                lexStopFSM = true;
                break;
            // Pokud znak není v jazyce
            case NOT_IN_LANGUAGE:
                // ERROR - načtený znak nepatří mezi znaky jazyka
                parser_errorWatcher(SET_ERROR_LEXICAL);
                lexStopFSM = true;
                break;
            // Pokud znak je složitý operátor
            case COMPLEX:
                // Pokud složitý operátor je .
                if(lexChar == '.') {
                    DString_appendChar(str, (char)lexChar);
                    lexToken = scanner_stateNumbersFloatPeriod(lexToken, str);
                    lexStopFSM = true;
                }
                // Jinak
                else {
                    lexToken = scanner_tokenCreate(TOKEN_INT, str);
                    scanner_ungetChar(lexChar);
                    lexStopFSM = true;
                }
                break;
            // Jinak
            default:    // SIMPLE + CHAR_EOF
                lexToken = scanner_tokenCreate(TOKEN_INT, str);
                scanner_ungetChar(lexChar);
                lexStopFSM = true;
                break;
        }
    }

    return lexToken;
}   // scanner_stateNumbers

/**
 * @brief Funkce scanneru pro zpracování řetězce s písmeny.
 */
Token scanner_stateLetters(Token lexToken, DString *str) {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
        // Abstraktně: vybírej podle typu znaku
        switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
            // Pokud znak je písmeno
            case LETTER:
                DString_appendChar(str, (char)lexChar);
                break;
            case NUMBER:
            // Pokud znak je číslo
                DString_appendChar(str, (char)lexChar);
                break;
            case WHITESPACE:
            // Pokud znak je bílý znak
                lexToken = scanner_isKeyword(str);
                lexStopFSM = true;
                break;
            // Pokud znak není v jazyce
            case NOT_IN_LANGUAGE:
                lexStopFSM = true;
                // ERROR - načtený znak nepatří mezi znaky jazyka
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
            //Jinak
            default:    // SIMPLE + COMPLEX + CHAR_EOF
                lexToken = scanner_isKeyword(str);
                scanner_ungetChar(lexChar);
                lexStopFSM = true;
                break;
        }
    }
    return lexToken;
}   // scanner_stateLetters

/**
 * @brief Hlavní řídící funkce scanneru.
 */
Token scanner_FSM() {
    // Inicializuj lexChar
    int lexChar;
    // Inicializuj lexStopFSM
    bool lexStopFSM = false;
    // Inicializuj string
    DString *str = DString_init();
    // Inicializuj token
    Token lexToken = scanner_init();
    // Abstraktně: cykluje, dokud nepřijde příkaz k zastavení FSM
    while(lexStopFSM == false) {
        lexChar = scanner_getNextChar();  // Vstup jednoho znaku z STDIN
        // Abstraktně: vybírej podle typu znaku
        switch (scanner_charIdentity(lexChar)) {  // Identifikace znaku
            // Pokud znak je písmeno
            case LETTER:
                DString_appendChar(str, (char)lexChar);
                lexToken = scanner_stateLetters(lexToken, str);
                lexStopFSM = true;
                break;
            // Pokud znak je číslo
            case NUMBER:
                DString_appendChar(str, (char)lexChar);
                lexToken = scanner_stateNumbers(lexToken, str);
                lexStopFSM = true;
                break;
            // Pokud znak je bílý znak
            case WHITESPACE:
                break;
            // Pokud znak není v jazyce
            case NOT_IN_LANGUAGE:
                lexStopFSM = true;
                // ERROR - načtený znak nepatří mezi znaky jazyka
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
            // Pokud znak je jednoduchý operátor
            case SIMPLE:
                lexToken = scanner_stateSimple(lexToken, lexChar);
                lexStopFSM = true;
                break;
            // Pokud znak je složitý operátor
            case COMPLEX:
                lexToken = scanner_stateComplexControl(lexToken, lexChar, str);
                lexStopFSM = true;
                if(lexToken.type == TOKEN_COMMENT) {
                    lexStopFSM = false;
                }
                break;
            // Pokud znak je znak konce souboru (EOF)
            case CHAR_EOF:
                lexToken = scanner_stringlessTokenCreate(TOKEN_EOF);
                scanner_ungetChar(lexChar);
                lexStopFSM = true;
                break;
            // Jinak
            default:
                lexStopFSM = true;
                // ERROR - charIdentity vrací default
                parser_errorWatcher(SET_ERROR_LEXICAL);
                break;
            }
        }

    // Pokud je hodnota value tokenu rovna NULL nebo došlo k vytvoření tokenu s klíčovým slovem
    if(lexToken.value == NULL || (lexToken.type >= LOWEST_KEYWORD && lexToken.type <= HIGHEST_KEYWORD)) {
        // Uvolní string
        DString_free(str);
    }

    return lexToken;
}   // scanner_FSM

/**
 * @brief Získá jeden Token.
 */
inline Token scanner_getNextToken() {
    return scanner_FSM();
}   // scanner_getNextToken

/*** Konec souboru scanner.c ***/
