/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
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
 * @file scanner.h
 * @author Hýža Pavel \<xhyzapa00>
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Hlavičkový soubor pro implementaci funkcí modulu scanner.
 * @details Tento soubor obsahuje deklaraci funkcí a datových typů modulu
 *          scannner, které jsou implementovány v souboru scanner.c.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "dynamic_string.h"//Vlastní knihovny
#include "error.h"

// Definice různých typů tokenů
typedef enum {
    TOKEN_IDENTIFIER = 1,
    TOKEN_KEYWORD = 2,
    TOKEN_INT = 3,
    TOKEN_FLOAT = 4,
    TOKEN_STRING = 5,
    //SPECIAL SIMPLE
    TOKEN_LEFT_PARENTHESIS = 6,
    TOKEN_RIGHT_PARENTHESIS = 7,
    TOKEN_ASTERISK = 8,
    TOKEN_PLUS = 9,
    TOKEN_COMMA = 10,
    TOKEN_MINUS = 11,
    TOKEN_COLON = 12,
    TOKEN_SEMICOLON = 13,
    TOKEN_LEFT_CURLY_BRACKET = 14,
    TOKEN_VERTICAL_BAR = 15,
    TOKEN_RIGHT_CURLY_BRACKET = 16,
    //SPECIAL COMPLEX
    TOKEN_PERIOD = 17,
    TOKEN_EQUALITY_SIGN = 18,
    TOKEN_SLASH = 19,
    TOKEN_EQUAL_TO = 20,
    TOKEN_NOT_EQUAL_TO = 21,
    TOKEN_LESS_THAN = 22,
    TOKEN_LESS_EQUAL_THAN = 23,
    TOKEN_GREATER_THAN = 24,
    TOKEN_GREATER_EQUAL_THAN = 25,
    TOKEN_EOF = 26,
    //KEYWORDS
    TOKEN_K_const = 27,
    TOKEN_K_var = 28,
    TOKEN_K_i32 = 29,
    TOKEN_K_f64 = 30,
    TOKEN_K_u8 = 31,
    TOKEN_K_Qi32 = 32,
    TOKEN_K_Qf64 = 33,
    TOKEN_K_Qu8 = 34,
    TOKEN_K_pub = 35,
    TOKEN_K_fn = 36,
    TOKEN_K_void = 36,
    TOKEN_K_return = 37,
    TOKEN_K_null = 38,
    TOKEN_K_if = 39,
    TOKEN_K_else = 40,
    TOKEN_K_while = 41,
    TOKEN_K_import = 42
} TokenType;

// Definice rozlišovaných typů znaků (char) na vstupu
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    WHITE = 3,      //WHITESPACE
    NIL = 4,        //NOT IN LANGUAGE
    //SPECIAL SIMPLE
    S_LEFT_PARENTHESIS = 5,
    S_RIGHT_PARENTHESIS = 6,
    S_ASTERISK = 7,
    S_PLUS = 8,
    S_COMMA = 9,
    S_MINUS = 10,
    S_COLON = 11,
    S_SEMICOLON = 12,
    S_LEFT_CURLY_BRACKET = 13,
    S_VERTICAL_BAR = 14,
    S_RIGHT_CURLY_BRACKET = 15,
    //SPECIAL COMPLEX
    C_EXCLAMATION_MARK = 16,
    C_DOUBLE_QUOTE = 17,
    C_PERIOD = 18,
    C_SLASH = 19,
    C_LESS_THAN = 20,
    C_EQUALITY_SIGN = 21,
    C_GREATER_THAN = 22,
    C_QUESTION_MARK = 23,
    C_AT_SIGN = 24,
    C_LEFT_SQUARE_BRACKET = 25,
    C_BACKSLASH = 26,
    C_RIGHT_SQUARE_BRACKET = 27,
    C_EOL = 28,
    C_EOF = 29
} CharType;

// Definice stavů FSM lexikálního analyzátoru
typedef enum {
    START = 1,
    LETTERS = 2,
    NUMBERS = 3,
    FLOAT_UNREADY = 4,
    FLOAT_READY = 5,
    //NUMBER OR NULL PATH
    NON = 6,
    NON1a = 7,
    NON1b = 8,
    NON2a = 9,
    NON2b = 11,
    NON3a = 12,
    NON3b = 13,
    NON3c = 14,
    //[]u8 PATH
    U1 = 15,
    U2 = 16,
    U3 = 17,
    //@import PATH
    I1 = 18,
    I2 = 19,
    I3 = 20,
    I4 = 21,
    I5 = 22,
    I6 = 23,
    //= OR == PATH
    EQUAL = 24,
    //!= PATH
    NOT_EQUAL = 25,
    //< OR <= PATH
    LESS = 26,
    //> OR >= PATH
    GREATER = 27,
    //SLASH PATH
    SLASH = 28,
    DOUBLE_SLASH = 29,
    //STRING PATH
    DOUBLE_QUOTE = 30,
    ESCAPE = 31,
    ESCAPE_X = 32, //Tímto si nejsem jistý
    //BACKSLASH PATH
    BACKSLASH = 33,
    DOUBLE_BACKSLASH = 34
} StateFSM;

/* Každý token má typ, hodnotu (tj. dynamický řetězec) a informace o pozici
   tokenu ve zdrojovém kódu (řádek a sloupec) */
typedef struct {
    TokenType type;     // ten TokenType tu musí být, aby mohl být enum pojmenovaný
    DString* value;
} Token;

// Práce se znaky na vstupu
// Přečte další znak ze zdrojového kódu
char scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar(char c);

// Rozhodování o typu znaku na vstupu
CharType scanner_charIdentity(char c);

// Rozhodování o tom, jestli LETTERS je IDENTIFIKÁTOR, nebo KEYWORD
bool scanner_isKeyword(DString *value);

// Vytvoří nový token
Token scanner_tokenCreate(TokenType type, DString *value);

// Vytvoří nový token bez stringu (všechny prvky stringu nastaveny na NULL)
Token scanner_stringlessTokenCreate(TokenType type);

// Finite State Machine lexikálního analyzátoru, z proudu charů na vstupu udělá Token na výstupu.
Token scanner_FSM();

// Volá parser, žádost o další token
Token scanner_getNextToken();

// Odstraní token
//void scanner_tokenDestroy(Token *token);

/* možná další is_něco() funkce nebo jiné funkce
   - je nějak potřeba poznat, zda se jedná o escape sekvenci
   - pozor na case sesitivitu
   - kontrolovat, zda má lexém správnou strukturu (např. pavel%honza obsahuje neplatný znak )
   - ...
*/
