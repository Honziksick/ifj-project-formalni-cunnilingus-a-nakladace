/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            06.10.2024                                                *
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



/**
 * @brief Enum různých typů Tokenů.
 *
 * @details Tokeny se dělí podle odstavců: Základní, Jednoduché operátory, Složité operátory, Klíčová slova, Neinicializováno
 */
typedef enum {
    //Základní
    TOKEN_IDENTIFIER = 1,
    TOKEN_INT = 2,
    TOKEN_FLOAT = 3,
    TOKEN_STRING = 4,
    //Jednoduché operátory
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
    //Složité operátory
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
    //Klíčová slova
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
    TOKEN_K_ifj = 44,
    //Neinicializováno
    TOKEN_UNINITIALIZED = 45
} TokenType;



/**
 * @brief Enum typů znaků pro funkci scanner_charIdentity.
 *
 * @details LETTER = 1,             - Písmeno
 *          NUMBER = 2,             - Číslice
 *          WHITESPACE = 3,         - Prázdný znak
 *          NOT_IN_LANGUAGE = 4,    - Znak, který není v jazyce používán, ale může být v komentářích
 *          SIMPLE = 5,             - Jednoduchý operátor (ihned se bude vracet jako Token)
 *          COMPLEX = 6,            - Složitý operátor (pro zjištění typu Tokenu bude potřeba více kroků)
 *          CHAR_EOF = 7.           - Znak konce souboru
 */
typedef enum {
    LETTER = 1,
    NUMBER = 2,
    WHITESPACE = 3,
    NOT_IN_LANGUAGE = 4,
    SIMPLE = 5,
    COMPLEX = 6,
    CHAR_EOF = 7
} CharType;



/**
 * @brief Definice pomocných stavů FSM lexikálního analyzátoru.
 */
typedef enum {
    STATE1_NUMBERS = 1,
    STATE2_FLOAT_PERIOD = 2,
    STATE3_FLOAT_EXP = 3,
    STATE4_FLOAT = 4,
    STATE5_FLOAT_AFTER_PERIOD = 5,

    STATE6_u8_A = 6,
    STATE7_u8_B = 7,
    STATE8_u8_C = 8,

    STATE9_QMARK = 9,
    STATE10_QMARK_i32_A = 10,
    STATE11_QMARK_f64_A = 11,
    STATE12_QMARK_u8_A = 12,
    STATE13_QMARK_i32_B = 13,
    STATE14_QMARK_f64_B = 14,
    STATE15_QMARK_u8_B = 15,
    STATE16_QMARK_u8_C = 16,

    STATE17_DOUBLE_QUOTATION_MARKS = 17,
    STATE18_ESCAPE_BACKSLASH = 18,
    STATE19_ESCAPE_BACKSLASH_X = 19,

    STATE20_BACKSLASH = 20,
    STATE21_DOUBLE_BACKSLASH = 21,
    STATE22_DOUBLE_BACKSLASH_LF = 22
} StateFSM;



/**
 * @brief   Datový typ Token
 * 
 * @details Obsahuje typ Tokenu a případně ukazatel na jeho dynamický řetězec.
 *          Většina Tokenů mívá value nastavený na NULL.
 */
typedef struct {
    TokenType type;
    DString* value;
} Token;



/**
 * @brief Získá znak ze vstupu programu.
 *
 * @details Funguje pomocí funkce getchar().
 *
 * @return Vrací int hodnotu, která náleží ASCII hodnotě daného načteného znaku.
 */
int scanner_getNextChar();



/**
 * @brief Vrátí potřebný znak zpět na vstup programu.
 * 
 * @param [in] c Int hodnota ASCII daného znaku.
 *
 * @details Funguje pomocí funkce ungetc().
 */
void scanner_ungetChar(int c);



/**
 * @brief Rozhodne o identitě znaku.
 * 
 * @param [in] c Int hodnota ASCII daného znaku.
 *
 * @details Rozděluje znaky do 7 skupin CharType.
 * 
 * @return Vrací konkrétní CharType.
 */
CharType scanner_charIdentity(int c);



/**
 * @brief V rámci FSM rozhodne o tom, zda je načtený řetězec znaků klíčovým slovem.
 * 
 * @param [in] value Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_keyflag Globální proměnná, která oznamuje na konci programu,
 *                          že se jedná o token s klíčovým slovem, takže se uvolní DString.
 *
 * @details Rozhoduje, zda se jedná o identifikátor, nebo o jedno ze 14 klíčových slov.
 * 
 * @return Vrací token identifikátoru, nebo kokrétního klíčového slova.
 */
Token scanner_isKeyword(DString *value);



/**
 * @brief Vytvoří nový token.
 * 
 * @param [in] type Typ tokenu.
 * @param [in] value Ukazatel na dynamický řetězec s načtenými znaky.
 *
 * @details Vytvoří strukturu Tokenu, přiřadí typ a value.
 * 
 * @return Vrací vytvořený Token s přiřazenými hodnotami.
 */
Token scanner_tokenCreate(TokenType type, DString *value);



/**
 * @brief Vytvoří nový token BEZ STRINGu.
 * 
 * @param [in] type Typ tokenu.
 *
 * @details Vytvoří strukturu Tokenu, přiřadí typ a prvek DString nastavý na NULL.
 * 
 * @return Vrací vytvořený Token s přiřazenou hodnotou typu.
 */
Token scanner_stringlessTokenCreate(TokenType type);



/**
 * @brief Inicializace scanneru.
 * 
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * @param [out] lex_keyflag Globální proměnná, která oznamuje na konci programu,
 *                          že se jedná o token s klíčovým slovem, takže se uvolní DString.
 * 
 * @details Inicializuje ovládací prvky automatu na začátku programu. Inicializuje token
 *          na neinicializovaný typ a string = NULL.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_init();



/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem \.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Načítá znaky do stringu. Má možnost víceřádkových stringů.
 *          Větví se na 3 podstavy.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexBackslash(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem ".
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Načítá znaky do stringu. Má možnost načítat i escape sekvence.
 *          Větví se na 3 podstavy.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexQuotation(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování klíčových slov ?i32, ?f64 a ?[]u8.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Postupně načítá znaky s tím, že mezi ? [ a ] a u8
 *          mohou být mezery, také mezi ? a i32, ? a f64.
 *          Větví se na 7 podstavů.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexQuestion(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování klíčového slova []u8.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Postupně načítá znaky s tím, že mezi [ a ] a u8
 *          mohou být mezery. Větví se na 3 podstavy.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexLeftSqrBr(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování klíčového slova @import.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * @param [out] lex_keyflag Globální proměnná, která oznamuje na konci programu,
 *                          že se jedná o token s klíčovým slovem, takže se uvolní DString.
 * 
 * @details Do nekonečna načítá znaky, dokud nenarazí na něco, co není
 *          písmeno. Zjistí, zda je načteno "import" a podle toho vrací
 *          token nebo error.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexAtSign(Token lexToken, DString *value);



/**
 * @brief Funkce scanneru pro zpracování komentářů //.
 * 
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Načte a zapomene všechny znaky za //, dokud nenarazí na konec řádku.
 */
void scanner_stateComplexDoubleSlash();



/**
 * @brief Funkce scanneru pro zpracování složitého operátoru /.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Větví se do funkce: scanner_stateComplexDoubleSlash.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexSlash(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitých operátorů = a ==.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexEqual(Token lexToken);




/**
 * @brief Funkce scanneru pro zpracování složitých operátorů > a >=.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexGreater(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitých operátorů < a <=.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexLess(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitého operátoru !=.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexExclamation(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování a řízení zpracování složitých operátorů.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in] loc_lex_char Lokální lexikální char.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Rozhodne o konkrétním druhu složitého operátoru,
 *          větví se do funkcí:
 *          scanner_stateComplexExclamation,
 *          scanner_stateComplexLess,
 *          scanner_stateComplexGreater,
 *          scanner_stateComplexEqual,
 *          scanner_stateComplexSlash,
 *          scanner_stateComplexAtSign,
 *          scanner_stateComplexLeftSqrBr,
 *          scanner_stateComplexQuestion,
 *          scanner_stateComplexQuotation,
 *          scanner_stateComplexBackslash.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateComplex(Token lexToken, int loc_lex_char);



/**
 * @brief Funkce scanneru pro zpracování jednoduchých operátorů.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in] loc_lex_char Lokální lexikální char.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Rozhodne o konkrétním druhu jednoduchého operátoru.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateSimple(Token lexToken, int loc_lex_char);



/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi.
 *
 * @param [in] lexToken Nehotový token.
 * @param [in] lex_state Globální proměnná určující podstavy.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Postupně načítá číslice do řetězce, který se může stát int, nebo float,
 *          větví se na 5 podstavů.
 *          Čísla mohou mít tvar například: 123
 *                                          123.123
 *                                          123e123
 *                                          123.123e123
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateNumbers(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování řetězce s písmeny.
 *
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Postupně načítá písmena do řetězce, který se může stát idenfikátorem,
 *          nebo klíčovým slovem, dokud nenarazí na ukončovací znak.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateLetters(Token lexToken, DString *str);



/**
 * @brief Řídící funkce scanneru.
 *
 * @param [out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lex_stopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Stavový automat lexikálního analyzátoru,
 *          z proudu znaků na vstupu vytvoří Token na výstupu.
 * 
 *          Může volat funkce:  scanner_init,
 *                              scanner_tokenCreate,
 *                              scanner_stringlessTokenCreate,
 *                              scanner_getNextToken,
 *                              scanner_isKeyword,
 *                              scanner_charIdentity,
 *                              scanner_getNextChar,
 *                              scanner_ungetChar,
 *                              funkce knihoven dynamic_string a error.
 * 
 *          Tyto funkce mohou volat i funkce, do který se větví:
 *                              scanner_stateLetters,
 *                              scanner_stateNumbers,
 *                              scanner_stateSimple,
 *                              scanner_stateComplexControl.
 * 
 * @return Vrací Token.
 */
Token scanner_FSM();



/**
 * @brief Získá jeden Token.
 *
 * @details Tato funkce je volána parserem, když si žádá další Token.
 * 
 * @return Předává Token parseru. 
 */
Token scanner_getNextToken();

#endif  // SCANNER_H_

/*** Konec souboru scanner.h ***/
