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
    TOKEN_EOF = 26
} TokenType;

// Definice rozlišovaných typů znaků (char) na vstupu
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    WHITE = 3,      //WHITESPACE
    NIL = 4,        //NOT IN LANGUAGE
    SIMPLE = 5,     //SPECIAL SIMPLE
    //SPECIAL COMPLEX
    C_EXCLAMATION_MARK = 6,
    C_DOUBLE_QUOTE = 7,
    C_PERIOD = 8,
    C_SLASH = 9,
    C_LESS_THAN = 10,
    C_EQUALITY_SIGN = 11,
    C_GREATER_THAN = 12,
    C_QUESTION_MARK = 13,
    C_AT_SIGN = 14,
    C_LEFT_SQUARE_BRACKET = 15,
    C_BACKSLASH = 16,
    C_RIGHT_SQUARE_BRACKET = 17,
    C_EOL = 18,
    C_EOF = 19
} CharType;

// Definice stavů FSM lexikálního analyzátoru
typedef enum {
    START = 1,
    LETTERS = 2,
    DIGITS = 3,
    FLOAT_UNREADY = 4,
    FLOAT_READY = 5,
    ERROR = 6
} stateFSM;

/* Každý token má typ, hodnotu (tj. dynamický řetězec) a informace o pozici
   tokenu ve zdrojovém kódu (řádek a sloupec) */
typedef struct {
    TokenType type;     // ten TokenType tu musí být, aby mohl být enum pojmenovaný
    DString value;
} Token;

// Práce se znaky na vstupu
// Přečte další znak ze zdrojového kódu
char scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar(char c);

// Rozhodování o typu znaku na vstupu
CharType scanner_charIdentity(char c);

// Vytvoří nový token
Token scanner_tokenCreate(TokenType type, DString *value);

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
