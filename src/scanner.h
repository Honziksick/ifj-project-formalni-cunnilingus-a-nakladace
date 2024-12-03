/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner.h                                                 *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            06.10.2024                                                *
 * Poslední změna:   02.12.2024                                                *
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


#define ALF 10                  /**< ASCII hodnota znaku NEW LINE LF*/
#define ASQ 39                  /**< ASCII hodnota znaku SINGLE QUOTE '*/
#define ABS 92                  /**< ASCII hodnota znaku BACLSLASH \*/
#define ADL 127                 /**< ASCII hodnota znaku DELETE*/

#define LOWEST_KEYWORD 26       /**< Hodnota Tokenu nejnižšího klíčového slova*/
#define HIGHEST_KEYWORD 44      /**< Hodnota Tokenu nejvyššího klíčového slova*/

#define HEXADECIMAL_SEQ_SIZE 3  /**< Velikost bufferu pro hexadecimálně zadaný znak
                                    "\xdd -> {d, d, '\0'}" */
#define HEXA_BASE 16            /**< Základem hexadecimální číslicové soustavy
                                je číslo "16" */
#define ASCII_VALUE_MAX 255     /**< Maximální ASCII hodnota */



/**
 * @brief Enum různých typů Tokenů.
 *
 * @details Tokeny se dělí podle odstavců: Základní, Jednoduché operátory,
 *          Složité operátory, Klíčová slova, Neinicializováno
 */
typedef enum {
    //Základní (4)
    TOKEN_IDENTIFIER = 1,           /**<  Token identifikátoru               */
    TOKEN_INT = 2,                  /**<  Token int čísla                    */
    TOKEN_FLOAT = 3,                /**<  Token float čísla                  */
    TOKEN_STRING = 4,               /**<  Token stringu                      */
    //Jednoduché operátory (11)
    TOKEN_LEFT_PARENTHESIS = 5,     /**<  Token levé kulaté závorky )        */
    TOKEN_RIGHT_PARENTHESIS = 6,    /**<  Token pravé kulaté závorky (       */
    TOKEN_ASTERISK = 7,             /**<  Token násobící hvězdičky *         */
    TOKEN_PLUS = 8,                 /**<  Token plus +                       */
    TOKEN_COMMA = 9,                /**<  Token čárky ,                      */
    TOKEN_MINUS = 10,               /**<  Token násobící hvězdičky *         */
    TOKEN_COLON = 11,               /**<  Token dvojtečky :                  */
    TOKEN_SEMICOLON = 12,           /**<  Token středníku ;                  */
    TOKEN_LEFT_CURLY_BRACKET = 13,  /**<  Token levé zaoblené závorky {      */
    TOKEN_VERTICAL_BAR = 14,        /**<  Token svislé čáry |                */
    TOKEN_RIGHT_CURLY_BRACKET = 15, /**<  Token pravé zaoblené závorky }     */
    //Složité operátory (10)
    TOKEN_PERIOD = 16,              /**<  Token tečky .                      */
    TOKEN_EQUALITY_SIGN = 17,       /**<  Token "rovná se" =                 */
    TOKEN_SLASH = 18,               /**<  Token dělícího lomítka /           */
    TOKEN_EQUAL_TO = 19,            /**<  Token rovnosti ==                  */
    TOKEN_NOT_EQUAL_TO = 20,        /**<  Token nerovnosti !=                */
    TOKEN_LESS_THAN = 21,           /**<  Token "menší než" <                */
    TOKEN_LESS_EQUAL_THAN = 22,     /**<  Token "menší nebo rovno než" <=    */
    TOKEN_GREATER_THAN = 23,        /**<  Token "větší než" >                */
    TOKEN_GREATER_EQUAL_THAN = 24,  /**<  Token "větší nebo rovno než" >=    */
    TOKEN_EOF = 25,                 /**<  Token konce souboru EOF            */
    //Klíčová slova (19)
    TOKEN_K_const = 26,             /**<  Token klíčového slova "const"      */
    TOKEN_K_var = 27,               /**<  Token klíčového slova "var"        */
    TOKEN_K_i32 = 28,               /**<  Token klíčového slova "i32"        */
    TOKEN_K_f64 = 29,               /**<  Token klíčového slova "f64"        */
    TOKEN_K_u8 = 30,                /**<  Token klíčového slova "[]u8"       */
    TOKEN_K_Qi32 = 31,              /**<  Token klíčového slova "?i32"       */
    TOKEN_K_Qf64 = 32,              /**<  Token klíčového slova "?f64"       */
    TOKEN_K_Qu8 = 33,               /**<  Token klíčového slova "?u8"        */
    TOKEN_K_pub = 34,               /**<  Token klíčového slova "pub"        */
    TOKEN_K_fn = 35,                /**<  Token klíčového slova "fn"         */
    TOKEN_K_void = 36,              /**<  Token klíčového slova "void"       */
    TOKEN_K_return = 37,            /**<  Token klíčového slova "return"     */
    TOKEN_K_null = 38,              /**<  Token klíčového slova "null"       */
    TOKEN_K_if = 39,                /**<  Token klíčového slova "if"         */
    TOKEN_K_else = 40,              /**<  Token klíčového slova "else"       */
    TOKEN_K_while = 41,             /**<  Token klíčového slova "while"      */
    TOKEN_K_import = 42,            /**<  Token klíčového slova "@import"    */
    TOKEN_K_underscore = 43,        /**<  Token klíčového slova "_"          */
    TOKEN_K_ifj = 44,               /**<  Token klíčového slova "ifj"        */
    //Neinicializováno (1)
    TOKEN_UNINITIALIZED = 45,       /**<  Token, který je neinicializován    */
    TOKEN_COMMENT = 46              /**<  Pomocný Token pro komentáře        */
} TokenType;



/**
 * @brief Enum typů znaků pro funkci scanner_charIdentity.
 *
 * @details LETTER = 1,             - Písmeno
 *          NUMBER = 2,             - Číslice
 *          WHITESPACE = 3,         - Prázdný znak
 *          NOT_IN_LANGUAGE = 4,    - Znak, který není v jazyce používán, ale může
 *                                  být v komentářích
 *          SIMPLE = 5,             - Jednoduchý operátor (ihned se bude vracet
 *                                  jako Token)
 *          COMPLEX = 6,            - Složitý operátor (pro zjištění typu Tokenu
 *                                  bude potřeba více kroků)
 *          CHAR_EOF = 7.           - Znak konce souboru
 */
typedef enum {
    LETTER = 1,             /**<  Skupina pro písmena                       */
    NUMBER = 2,             /**<  Skupina pro číslice                       */
    WHITESPACE = 3,         /**<  Skupina pro bílé znaky                    */
    NOT_IN_LANGUAGE = 4,    /**<  Skupina pro znaky, co nejsou v jazyce     */
    SIMPLE = 5,             /**<  Skupina pro znaky jednoduchých operátorů  */
    COMPLEX = 6,            /**<  Skupina pro znaky složitých operátorů     */
    CHAR_EOF = 7            /**<  Skupina pro znaky konců souborů           */
} CharType;



/**
 * @brief Definice pomocných stavů FSM lexikálního analyzátoru.
 */
typedef enum {
    STATE1_u8_A = 1,                        /**<  Stav pro zpracování klíč. slova []u8, podstav: [      */
    STATE2_u8_B = 2,                        /**<  Stav pro zpracování klíč. slova []u8, podstav: []     */
    STATE3_u8_C = 3,                        /**<  Stav pro zpracování klíč. slova []u8, podstav: []u    */

    STATE4_QMARK = 4,                       /**<  Stav pro zpracování klíčových slov ?i32, ?f64, ?[]u8  */
    STATE5_QMARK_i32_A = 5,               /**<  Stav pro zpracování klíč. slova ?i32, podstav: ?i       */
    STATE6_QMARK_f64_A = 6,               /**<  Stav pro zpracování klíč. slova ?f64, podstav: ?f       */
    STATE7_QMARK_u8_A = 7,                /**<  Stav pro zpracování klíč. slova ?[]u8, podstav: ?[      */
    STATE8_QMARK_i32_B = 8,               /**<  Stav pro zpracování klíč. slova ?i32, podstav: ?i3      */
    STATE9_QMARK_f64_B = 9,               /**<  Stav pro zpracování klíč. slova ?f64, podstav: ?f6      */
    STATE10_QMARK_u8_B = 10,                /**<  Stav pro zpracování klíč. slova ?[]u8, podstav: ?[]   */
    STATE11_QMARK_u8_C = 11,                /**<  Stav pro zpracování klíč. slova ?[]u8, podstav: ?[]u  */

    STATE12_DOUBLE_QUOTATION_MARKS = 12,    /**<  Stav pro úvodní zpracování stringu ""                 */
    STATE13_ESCAPE_BACKSLASH = 13,          /**<  Stav pro zpracování escape sekvencí ""                */
    STATE14_ESCAPE_BACKSLASH_X = 14,        /**<  Stav pro zpracování esc. sekv. \xdd ""                */

    STATE15_BACKSLASH = 15,                 /**<  Stav pro úvodní zpracování znaku \                    */
    STATE16_DOUBLE_BACKSLASH = 16,          /**<  Stav pro úvodní zpracování stringu \\                 */
    STATE17_DOUBLE_BACKSLASH_LF = 17        /**<  Stav pro zpracování stringu \\ při novém řádku        */
} StateFSM;



/**
 * @brief   Datový typ Token
 * 
 * @details Obsahuje typ Tokenu a případně ukazatel na jeho dynamický řetězec.
 *          Většina Tokenů mívá value nastavený na NULL.
 */
typedef struct {
    TokenType type;     /**<  Enum typu tokenu                                                          */
    DString* value;     /**<  Hodnota tokenu reprezentována jako ukazatel na dynamický řetězec DString  */
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
 * @details Funguje pomocí funkce ungetc().
 * 
 * @param [in] c Int hodnota ASCII daného znaku.
 */
void scanner_ungetChar(int c);



/**
 * @brief Rozhodne o identitě znaku.
 * 
 * @details Rozděluje znaky do 7 skupin CharType:
 *          LETTER = 1,             - Písmeno
 *          NUMBER = 2,             - Číslice
 *          WHITESPACE = 3,         - Prázdný znak
 *          NOT_IN_LANGUAGE = 4,    - Znak, který není v jazyce používán, ale může
 *                                  být v komentářích
 *          SIMPLE = 5,             - Jednoduchý operátor (ihned se bude vracet
 *                                  jako Token)
 *          COMPLEX = 6,            - Složitý operátor (pro zjištění typu Tokenu
 *                                  bude potřeba více kroků)
 *          CHAR_EOF = 7.           - Znak konce souboru
 *           
 *          Skupina 1 je definována funkcí isalpha + '_'.
 *          Skupina 2 je definována funkcí isdigit.
 *          Skupina 3 je definována funkcí isspace.
 *          Další skupiny jsou ručně definovány po individuálních znacích dle
 *          specifickace zadání.
 * 
 * @param [in] c Int hodnota ASCII daného znaku.
 * 
 * @return Vrací konkrétní CharType.
 */
CharType scanner_charIdentity(int c);



/**
 * @brief V rámci FSM rozhodne o tom, zda je načtený řetězec znaků klíčovým slovem.
 * 
 * @details Rozhoduje, zda se jedná o identifikátor, nebo o jedno ze 14 klíčových
 *          slov.
 * 
 * @param [in] value Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací token identifikátoru, nebo kokrétního klíčového slova.
 */
Token scanner_isKeyword(DString *value);



/**
 * @brief Vytvoří nový token.
 * 
 * @details Vytvoří strukturu Tokenu, přiřadí typ a value.
 * 
 * @param [in] type Typ tokenu.
 * @param [in] value Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací vytvořený Token s přiřazenými hodnotami.
 */
Token scanner_tokenCreate(TokenType type, DString *value);



/**
 * @brief Vytvoří nový token BEZ STRINGu.
 * 
 * @details Vytvoří strukturu Tokenu, přiřadí typ a prvek DString nastavý na NULL.
 * 
 * @param [in] type Typ tokenu.
 * 
 * @return Vrací vytvořený Token s přiřazenou hodnotou typu.
 */
Token scanner_stringlessTokenCreate(TokenType type);



/**
 * @brief Inicializace scanneru.
 * 
 * @details Inicializuje ovládací prvky automatu na začátku programu. Inicializuje
 *          token na neinicializovaný typ a string = NULL.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_init();



/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem \.
 * 
 * @details Načítá znaky do stringu. Má možnost víceřádkových stringů.
 *          Větví se na 3 podstavy.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexBackslash(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování stringů vyvolaných znakem ".
 * 
 * @details Načítá znaky do stringu. Má možnost načítat i escape sekvence.
 *          Větví se na 3 podstavy.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexQuotation(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování klíčových slov ?i32, ?f64 a ?[]u8.
 * 
 * @details Postupně načítá znaky s tím, že mezi ? [ a ] a u8
 *          mohou být mezery, také mezi ? a i32, ? a f64.
 *          Větví se na 7 podstavů.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexQuestion(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování klíčového slova []u8.
 * 
 * @details Postupně načítá znaky s tím, že mezi [ a ] a u8
 *          mohou být mezery. Větví se na 3 podstavy.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexLeftSqrBr(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování klíčového slova @import.
 * 
 * @details Do nekonečna načítá znaky, dokud nenarazí na něco, co není
 *          písmeno. Zjistí, zda je načteno "import" a podle toho vrací
 *          token nebo error.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexAtSign(Token lexToken, DString *value);



/**
 * @brief Funkce scanneru pro zpracování komentářů //.
 * 
 * @details Načte a zapomene všechny znaky za //, dokud nenarazí na konec řádku.
 * 
 */
void scanner_stateComplexDoubleSlash();



/**
 * @brief Funkce scanneru pro zpracování složitého operátoru /.
 * 
 * @details Větví se do funkce: scanner_stateComplexDoubleSlash.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexSlash(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitých operátorů = a ==.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexEqual(Token lexToken);




/**
 * @brief Funkce scanneru pro zpracování složitých operátorů > a >=.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexGreater(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitých operátorů < a <=.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexLess(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitého operátoru !=.
 * 
 * @param [in] lexToken Nehotový token.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexExclamation(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování a řízení zpracování složitých operátorů.
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
 * @param [in] lexToken Nehotový token.
 * @param [in] lexChar Lexikální char.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateComplexControl(Token lexToken, int lexChar, DString *str);



/**
 * @brief Funkce scanneru pro zpracování jednoduchých operátorů.
 * 
 * @details Rozhodne o konkrétním druhu jednoduchého operátoru.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in] lexChar Lexikální char.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateSimple(Token lexToken, int lexChar);



/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PO přijetí exponenciálního znaku.
 * 
 * @details Načítá dál číslice bez omezení, ale kontroluje, aby z STDIN nepřišla
 *          desetinná tečka nebo exponenciální znak.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do řízení zpracování číslic.
 */
Token scanner_stateNumbersFloat(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PRO přijetí exponenciálního znaku.
 * 
 * @details Po přijetí exponenciálního znaku "e" ověřuje, zda další znak je buď:
 *          číslice, znak "+", nebo znak "-".
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do řízení zpracování číslic.
 */
Token scanner_stateNumbersFloatExp(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PO přijetí desetinné tečky.
 * 
 * @details Načítá dál číslice bez omezení, ale kontroluje, aby z STDIN nepřišla další
 *          desetinná tečka.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do řízení zpracování číslic.
 */
Token scanner_stateNumbersAfterPeriod(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi PRO přijetí desetinné tečky.
 * 
 * @details Po přijetí znaku desetinné tečky ověřuje, že další znak ze STDIN je číslice.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do řízení zpracování číslic.
 */
Token scanner_stateNumbersFloatPeriod(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro řízení a zpracování řetězce s číslicemi.
 *
 * @details Postupně načítá číslice do řetězce, který se může stát int, nebo float.
 *          Pracuje s funkcemi: scanner_stateNumbersFloatPeriod
 *                              scanner_stateNumbersFloatAfterPeriod
 *                              scanner_stateNumbersFloatExp
 *                              scanner_stateNumbersFloat
 * 
 *          Čísla mohou mít tvar například: 123
 *                                          123.123
 *                                          123e123
 *                                          123.123e123
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateNumbers(Token lexToken, DString *str);



/**
 * @brief Funkce scanneru pro zpracování řetězce s písmeny.
 *
 * @details Postupně načítá písmena do řetězce, který se může stát idenfikátorem,
 *          nebo klíčovým slovem, dokud nenarazí na ukončovací znak.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateLetters(Token lexToken, DString *str);



/**
 * @brief Řídící funkce scanneru.
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
 * @param [out] str Ukazatel na dynamický řetězec s načtenými znaky.
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
