/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                   Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            6.10.2024                                                 *
 * Poslední změna:   14.10.2024                                                 *
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

// Definice rozlišovaných typů znaků (char) na vstupu
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    OPERATOR = 3,
    DOT = 4,
    EMPTY = 5,
    CHAR_ERROR = 6
} CharType;

// Definice stavů FSM lexikálního analyzátoru
typedef enum {
    START = 1,
    CHARACTERS = 2,
    DIGITS = 3,
    FLOAT_UNREADY = 4,
    FLOAT_READY = 5,
    ERROR = 6
} stateFSM;

/* Každý token má typ, hodnotu (tj. dynamický řetězec) a informace o pozici
   tokenu ve zdrojovém kódu (řádek a sloupec) */
typedef struct {
    TokenType type;     // ten TokenType tu musí být, aby mohl být enum pojmenovaný
    string value;
} Token;

// Práce se znaky na vstupu
// Přečte další znak ze zdrojového kódu
char scanner_getNextChar();

// Vrácí znak zpět do vstupu (backtracking)
void scanner_ungetChar(char c);

// Rozhodování o typu znaku na vstupu
CharType scanner_charIdentity(char c);

// Vytvoří nový token
Token scanner_tokenCreate(TokenType type, string *value);

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
