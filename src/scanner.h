/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
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
#include "scanner.c"        //Vlastní knihovny
#include "dynamic_string.h"
#include "error.h"

// Definice různých typů tokenů
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_OPERATOR,
    TOKEN_DOT,
    TOKEN_KEYWORD,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_EOF
    // ... Opět možná další enumy, plus by bylo lepší přiřadit explicitní hodnoty
} TokenType;

/* Každý token má typ, hodnotu (tj. dynamický řetězec) a informace o pozici
   tokenu ve zdrojovém kódu (řádek a sloupec) */
typedef struct {
    TokenType type;     // ten TokenType tu musí být, aby mohl být enum pojmenovaný
    String value;
} Token;

// Moje práce

// Definice rozlišovaných typů znaků (char) na vstupu
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    OPERATOR = 3,
    DOT = 4,
    EMPTY = 5,
    ERROR = 6;
} CharType;

// Práce se znaky na vstupu
// Přečte další znak ze zdrojového kódu
char scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar();

// Rozhodování o typu znaku na vstupu
CharType scanner_charIdentity(char c);

void scanner_FSM();

// Není moje práce
// Vytvoří nový token
Token scanner_tokenCreate(TokenType type, String *value);

// Odstraní token
void scanner_tokenDestroy(Token *token);

// Volá parser, žádost o další token
Token scanner_getNextToken();

// Přeskočí bílé znaky
void scanner_skipWhitespace();

// Přeskočí komentáře
void scanner_skipComment();

// Je podtržítko? (`_`)
bool scanner_isUnderscore(char c);

// Je operátor?
//bool scanner_isOperator(char c);

// Je slovo klíčové slovo?
bool scanner_isKeyword(String *s);

// Je alfanumerický znak? (písmeno nebo číslice)
bool scanner_isAlphanumeric(char c);

// Je znak?
bool scanner_isAlpha(char c);

// Je číslice?
bool scanner_isDigit(char c);

/* možná další is_něco() funkce nebo jiné funkce
   - je nějak potřeba poznat, zda se jedná o escape sekvenci
   - pozor na case sesitivitu
   - kontrolovat, zda má lexém správnou strukturu (např. pavel%honza obsahuje neplatný znak )
   - ...
*/
