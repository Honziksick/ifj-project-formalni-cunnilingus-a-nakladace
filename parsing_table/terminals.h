#ifndef TERMINALS_H
#define TERMINALS_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "nonterminals.h"


enum Terminal {
    T_ID                    = 0,        /**<  Identifikátor  */
    T_IMPORT                = 1,        /**<  Klíčové slovo "@import"  */
    T_IFJ                   = 2,        /**<  Klíčové slovo "ifj"  */
    T_PUB                   = 3,        /**<  Klíčové slovo "pub"  */
    T_FN                    = 4,        /**<  Klíčové slovo "fn"  */
    T_CONST                 = 5,        /**<  Klíčové slovo "const"  */
    T_VAR                   = 6,        /**<  Klíčové slovo "var"  */
    T_IF                    = 7,        /**<  Klíčové slovo "if"  */
    T_ELSE                  = 8,        /**<  Klíčové slovo "else"  */
    T_WHILE                 = 9,        /**<  Klíčové slovo "while"  */
    T_RETURN                = 10,       /**<  Klíčové slovo "return"  */
    T_ASSIGNMENT            = 11,       /**<  Operátor přiřazení "="  */
    T_INT                   = 12,       /**<  Klíčové slovo "i32"  */
    T_INT_OR_NULL           = 13,       /**<  Klíčové slovo "?i32"  */
    T_FLOAT                 = 14,       /**<  Klíčové slovo "f64"  */
    T_FLOAT_OR_NULL         = 15,       /**<  Klíčové slovo "?f64"  */
    T_STRING                = 16,       /**<  Klíčové slovo "[]u8"  */
    T_STRING_OR_NULL        = 17,       /**<  Klíčové slovo "?[]u8"  */
    T_VOID                  = 18,       /**<  Klíčové slovo "void"  */
    T_DUMP                  = 19,       /**<  Speciální proměnná "_" pro zahození návratové hodnoty  */
    T_DOT                   = 20,       /**<  Symbol tečky "."  */
    T_COMMA                 = 21,       /**<  Symbol čárky ","  */
    T_COLON                 = 22,       /**<  Symbol dvojtečky ":"  */
    T_SEMICOLON             = 23,       /**<  Symbol středníku ";"  */
    T_PIPE                  = 24,       /**<  Symbol svislice "|"  */
    T_LEFT_BRACKET          = 25,       /**<  Symbol levé závorky "("  */
    T_RIGHT_BRACKET         = 26,       /**<  Symbol pravé závorky ")"  */
    T_LEFT_CURLY_BRACKET    = 27,       /**<  Symbol levé složené závorky "{"  */
    T_RIGHT_CURLY_BRACKET   = 28,       /**<  Symbol pravé složené závorky "}"  */
    T_EOF                   = 29,       /**<  Signalizace, že nastal konec souboru  */
    T_CALL_PRECEDENCE       = 30,       /**<  Řízení parsování předáno precedenčnímu parseru */
    T_EPSILON               = 31,
    TERMINAL_COUNT,  // Number of terminals
};

std::unordered_map<std::string, Terminal> terminalMap = {
    {"ID", T_ID},
    {"IMPORT", T_IMPORT},
    {"IFJ", T_IFJ},
    {"PUB", T_PUB},
    {"FN", T_FN},
    {"CONST", T_CONST},
    {"VAR", T_VAR},
    {"IF", T_IF},
    {"ELSE", T_ELSE},
    {"WHILE", T_WHILE},
    {"RETURN", T_RETURN},
    {"ASSIGNMENT", T_ASSIGNMENT},
    {"INT", T_INT},
    {"INT_OR_NULL", T_INT_OR_NULL},
    {"FLOAT", T_FLOAT},
    {"FLOAT_OR_NULL", T_FLOAT_OR_NULL},
    {"STRING", T_STRING},
    {"STRING_OR_NULL", T_STRING_OR_NULL},
    {"VOID", T_VOID},
    {"DUMP", T_DUMP},
    {"DOT", T_DOT},
    {"COMMA", T_COMMA},
    {"COLON", T_COLON},
    {"SEMICOLON", T_SEMICOLON},
    {"PIPE", T_PIPE},
    {"LEFT_BRACKET", T_LEFT_BRACKET},
    {"RIGHT_BRACKET", T_RIGHT_BRACKET},
    {"LEFT_CURLY_BRACKET", T_LEFT_CURLY_BRACKET},
    {"RIGHT_CURLY_BRACKET", T_RIGHT_CURLY_BRACKET},
    {"EOF", T_EOF},
    {"PRECEDENCE", T_CALL_PRECEDENCE},
    {"EPSILON", T_EPSILON},
};

// Mapa pro určení pořadí terminálů
std::map<int, int> terminalOrder = {
    {T_ID, 0},
    {T_IMPORT, 1},
    {T_IFJ, 2},
    {T_PUB, 3},
    {T_FN, 4},
    {T_CONST, 5},
    {T_VAR, 6},
    {T_IF, 7},
    {T_ELSE, 8},
    {T_WHILE, 9},
    {T_RETURN, 10},
    {T_ASSIGNMENT, 11},
    {T_INT, 12},
    {T_INT_OR_NULL, 13},
    {T_FLOAT, 14},
    {T_FLOAT_OR_NULL, 15},
    {T_STRING, 16},
    {T_STRING_OR_NULL, 17},
    {T_VOID, 18},
    {T_DUMP, 19},
    {T_DOT, 20},
    {T_COMMA, 21},
    {T_COLON, 22},
    {T_SEMICOLON, 23},
    {T_PIPE, 24},
    {T_LEFT_BRACKET, 25},
    {T_RIGHT_BRACKET, 26},
    {T_LEFT_CURLY_BRACKET, 27},
    {T_RIGHT_CURLY_BRACKET, 28},
    {T_EOF, 29},
    {T_CALL_PRECEDENCE, 30},
    {T_EPSILON, 31}
};

std::array<std::string, TERMINAL_COUNT> invTerminalMap = {
    "T_ID",
    "T_IMPORT",
    "T_IFJ",
    "T_PUB",
    "T_FN",
    "T_CONST",
    "T_VAR",
    "T_IF",
    "T_ELSE",
    "T_WHILE",
    "T_RETURN",
    "T_ASSIGNMENT",
    "T_INT",
    "T_INT_OR_NULL",
    "T_FLOAT",
    "T_FLOAT_OR_NULL",
    "T_STRING",
    "T_STRING_OR_NULL",
    "T_VOID",
    "T_DUMP",
    "T_DOT",
    "T_COMMA",
    "T_COLON",
    "T_SEMICOLON",
    "T_PIPE",
    "T_LEFT_BRACKET",
    "T_RIGHT_BRACKET",
    "T_LEFT_CURLY_BRACKET",
    "T_RIGHT_CURLY_BRACKET",
    "T_EOF",
    "T_CALL_PRECEDENCE",
    "T_EPSILON",
};

Terminal getTerminal(const std::string& token) {
    // Ensure the token exists in the map to avoid exceptions
    auto it = terminalMap.find(token);
    if (it != terminalMap.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid token"); // Handle the case where the token is not found
}

#endif // TERMINALS_H
