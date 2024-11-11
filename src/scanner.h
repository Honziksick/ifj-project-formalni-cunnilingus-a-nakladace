/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00> (implementace)             *
 *                   Farkašovský Lukáš  <xfarkal00> (minor edit)               *
 *                                                                             *
 * Datum:            06.10.2024                                                *
 * Poslední změna:   11.11.2024                                                *
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
 * @author Hýža Pavel \<xhyzapa00> (implementace)
 * @author Farkašovský Lukáš \<xfarkal00> (minor edit)
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

// Definice hashů pro klíčová slova

#define KEYWORD_UNDERSCORE_HASH     177668ULL
#define KEYWORD_FN_HASH             5863385ULL
#define KEYWORD_IF_HASH             5863476ULL
#define KEYWORD_F64_HASH            193489909ULL
#define KEYWORD_I32_HASH            193493075ULL
#define KEYWORD_PUB_HASH            193502924ULL
#define KEYWORD_VAR_HASH            193508814ULL
#define KEYWORD_QF64_HASH           6383817844ULL
#define KEYWORD_QI32_HASH           6383821010ULL
#define KEYWORD_U8_HASH             6384816362ULL
#define KEYWORD_ELSE_HASH           6385192046ULL
#define KEYWORD_NULL_HASH           6385525056ULL
#define KEYWORD_VOID_HASH           6385805911ULL
#define KEYWORD_QU8_HASH            210665638217ULL
#define KEYWORD_CONST_HASH          210709068620ULL
#define KEYWORD_WHILE_HASH          210732529790ULL
#define KEYWORD_RETURN_HASH         6953974653989ULL
#define KEYWORD_IMPORT_HASH         229416738240608ULL
#define KEYWORD_COUNT 18



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
    TOKEN_K_underscore = 43
} TokenType;

// Definice rozlišovaných typů znaků (char) na vstupu
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    WHITE = 3,      //WHITESPACE
    //SPECIAL SIMPLE
    S_LEFT_PARENTHESIS = 4,
    S_RIGHT_PARENTHESIS = 5,
    S_ASTERISK = 6,
    S_PLUS = 7,
    S_COMMA = 8,
    S_MINUS = 9,
    S_COLON = 10,
    S_SEMICOLON = 11,
    S_LEFT_CURLY_BRACKET = 12,
    S_VERTICAL_BAR = 13,
    S_RIGHT_CURLY_BRACKET = 14,
    //SPECIAL COMPLEX
    C_EXCLAMATION_MARK = 15,
    C_DOUBLE_QUOTE = 16,
    C_PERIOD = 17,
    C_SLASH = 18,
    C_LESS_THAN = 19,
    C_EQUALITY_SIGN = 20,
    C_GREATER_THAN = 21,
    C_QUESTION_MARK = 22,
    C_AT_SIGN = 23,
    C_LEFT_SQUARE_BRACKET = 24,
    C_BACKSLASH = 25,
    C_RIGHT_SQUARE_BRACKET = 26,
    C_EOF = 27
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
    NON2b = 10,
    NON3a = 11,
    NON3b = 12,
    NON3c = 13,
    //[]u8 PATH
    U1 = 14,
    U2 = 15,
    U3 = 16,
    //@import PATH
    I1 = 17,
    I2 = 18,
    I3 = 19,
    I4 = 20,
    I5 = 21,
    I6 = 22,
    //= OR == PATH
    EQUAL = 23,
    //!= PATH
    NOT_EQUAL = 24,
    //< OR <= PATH
    LESS = 25,
    //> OR >= PATH
    GREATER = 26,
    //SLASH PATH
    SLASH = 27,
    DOUBLE_SLASH = 28,
    //STRING PATH
    DOUBLE_QUOTE = 29,
    ESCAPE = 30,
    ESCAPE_X = 31, //Tímto si nejsem jistý
    //BACKSLASH PATH
    BACKSLASH = 32,
    DOUBLE_BACKSLASH = 33
} StateFSM;


/* Každý token má typ, hodnotu (tj. dynamický řetězec) a informace o pozici
   tokenu ve zdrojovém kódu (řádek a sloupec) */
typedef struct {
    TokenType type;     // ten TokenType tu musí být, aby mohl být enum pojmenovaný
    DString* value;
} Token;

// Definice struktury pro pár KeywordHash - TokenType
typedef struct {
    size_t hash;
    TokenType type;
} Keyword;

// Práce se znaky na vstupu
// Přečte další znak ze zdrojového kódu
int scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar(int c);

// Rozhodování o typu znaku na vstupu
CharType scanner_charIdentity(int c);

// Rozhodování o tom, jestli LETTERS je IDENTIFIKÁTOR, nebo KEYWORD
TokenType scanner_isKeyword(DString *value);

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
