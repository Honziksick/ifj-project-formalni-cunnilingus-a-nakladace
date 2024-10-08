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
#include "dynamic_string.h" //Vlastní knihovny
#include "error.h"  //(zatím nejsou implementovány)

// Definice různých typů tokenů
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_INT,
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
bool scanner_isOperator(char c);

// Je bílý znak?
bool scanner_isWhitespace(char c);

// Je slovo klíčové slovo?
bool scanner_isKeyword(String *s);

// Přečte další znak ze zdrojového kódu
char scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar();

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
