/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            06.10.2024                                                *
 * Poslední změna:   15.11.2024                                                *
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
 *
 * @brief Hlavičkový soubor pro implementaci funkcí modulu scanner.
 * @details Tento soubor obsahuje deklaraci funkcí a datových typů modulu
 *          scannner, které jsou implementovány v souboru scanner.c.
 */

#ifndef SCANNER_H_
/** @cond  */
#define SCANNER_H_
/** @endcond  */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "dynamic_string.h"
#include "error.h"

// Definice různých typů tokenů
typedef enum {
    TOKEN_IDENTIFIER = 1,
    TOKEN_INT = 2,
    TOKEN_FLOAT = 3,
    TOKEN_STRING = 4,
    //SPECIAL SIMPLE
    TOKEN_LEFT_PARENTHESIS = 5,
    TOKEN_RIGHT_PARENTHESIS = 6,
    TOKEN_ASTERISK = 7,
    TOKEN_PLUS = 8,
    TOKEN_COMMA = 9,
    TOKEN_MINUS = 10,
    TOKEN_COLON = 11,
    TOKEN_SEMICOLON = 12,
    TOKEN_LEFT_CURLY_BRACKET = 13,
    TOKEN_VERTICAL_BAR = 14,
    TOKEN_RIGHT_CURLY_BRACKET = 15,
    //SPECIAL COMPLEX
    TOKEN_PERIOD = 16,
    TOKEN_EQUALITY_SIGN = 17,
    TOKEN_SLASH = 18,
    TOKEN_EQUAL_TO = 19,
    TOKEN_NOT_EQUAL_TO = 20,
    TOKEN_LESS_THAN = 21,
    TOKEN_LESS_EQUAL_THAN = 22,
    TOKEN_GREATER_THAN = 23,
    TOKEN_GREATER_EQUAL_THAN = 24,
    TOKEN_EOF = 25,
    //KEYWORDS
    TOKEN_K_const = 26,
    TOKEN_K_var = 27,
    TOKEN_K_i32 = 28,
    TOKEN_K_f64 = 29,
    TOKEN_K_u8 = 30,
    TOKEN_K_Qi32 = 31,
    TOKEN_K_Qf64 = 32,
    TOKEN_K_Qu8 = 33,
    TOKEN_K_pub = 34,
    TOKEN_K_fn = 35,
    TOKEN_K_void = 36,
    TOKEN_K_return = 37,
    TOKEN_K_null = 38,
    TOKEN_K_if = 39,
    TOKEN_K_else = 40,
    TOKEN_K_while = 41,
    TOKEN_K_import = 42,
    TOKEN_K_underscore = 43,
    TOKEN_K_ifj = 44
} TokenType;

// Definice rozlišovaných typů znaků (char) na vstupu
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    WHITESPACE = 3,
    NOT_IN_LANGUAGE = 4,
    SIMPLE = 5,
    COMPLEX = 6,
    CHAR_EOF = 7
} CharType;

// Definice stavů FSM lexikálního analyzátoru
typedef enum {
    STATE0_START = 0,
    STATE1_LETTERS = 1,
    STATE2_NUMBERS = 2,
    STATE3_FLOAT_UNREADY = 3,
    STATE4_FLOAT_READY = 4,
    STATE5_SIMPLE = 5,
    STATE6_COMPLEX = 6,
    STATE7_EXCLAMATION_MARK = 7,
    STATE8_LESS = 8,
    STATE9_GREATER = 9,
    STATE10_EQUALITY_MARK = 10,
    STATE11_SLASH = 11,
    STATE12_DOUBLE_SLASH = 12,
    STATE13_AT_SIGN = 13,
    STATE14_LEFT_SQUARE_BRACKET_A = 14,
    STATE15_LEFT_SQUARE_BRACKET_B = 15,
    STATE16_LEFT_SQUARE_BRACKET_C = 16,
    STATE17_QUESTION_MARK = 17,
    STATE18_QUESTION_MARK_A1 = 18,
    STATE19_QUESTION_MARK_B1 = 19,
    STATE20_QUESTION_MARK_C1 = 20,
    STATE21_QUESTION_MARK_A2 = 21,
    STATE22_QUESTION_MARK_B2 = 22,
    STATE23_QUESTION_MARK_C2 = 23,
    STATE24_QUESTION_MARK_C3 = 24,
    STATE25_DOUBLE_QUOTATION_MARKS = 25,
    STATE26_ESCAPE_BACKSLASH = 26,
    STATE27_ESCAPE_BACKSLASH_X = 27,
    STATE28_BACKSLASH = 28,
    STATE29_DOUBLE_BACKSLASH = 29,
    STATE30_DOUBLE_BACKSLASH_LF = 30
} StateFSM;

typedef enum {
    KEY_IDENTIFIER = 1,

    KEY_const = 26,
    KEY_var = 27,
    KEY_i32 = 28,
    KEY_f64 = 29,

    KEY_pub = 34,
    KEY_fn = 35,
    KEY_void = 36,
    KEY_return = 37,
    KEY_null = 38,
    KEY_if = 39,
    KEY_else = 40,
    KEY_while = 41,
    KEY_underscore = 43,
    KEY_ifj = 44
} Keywords;

/* Každý token má typ, hodnotu (tj. dynamický řetězec) a informace o pozici
   tokenu ve zdrojovém kódu (řádek a sloupec) */
typedef struct {
    TokenType type;     // ten TokenType tu musí být, aby mohl být enum pojmenovaný
    DString* value;
} Token;

// Práce se znaky na vstupu
// Přečte další znak ze zdrojového kódu
int scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar(int c);

// Rozhodování o typu znaku na vstupu
CharType scanner_charIdentity(int c);

// Rozhodování o tom, jestli LETTERS je IDENTIFIKÁTOR, nebo KEYWORD
Keywords scanner_isKeyword(DString *value);

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

#endif  // SCANNER_H_

/*** Konec souboru scanner.h ***/
