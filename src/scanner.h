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


#define ALF 10                  /**< ASCII hodnota znaku NEW LINE LF*/
#define ASQ 39                  /**< ASCII hodnota znaku SINGLE QUOTE '*/
#define ABS 92                  /**< ASCII hodnota znaku BACLSLASH \*/
#define ADL 127                 /**< ASCII hodnota znaku DELETE*/

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
    TOKEN_UNINITIALIZED = 45        /**<  Token, který je neinicializován    */
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
    STATE1_NUMBERS = 1,                     /**<  Stav pro úvodní zpracování čísel                  */
    STATE2_FLOAT_PERIOD = 2,                /**<  Stav pro zpracování desetinné tečky               */
    STATE3_FLOAT_EXP = 3,                   /**<  Stav pro zpracování exponentu                     */
    STATE4_FLOAT = 4,                       /**<  Stav pro zpracovní float čísel                    */
    STATE5_FLOAT_AFTER_PERIOD = 5,          /**<  Stav pro zpracování desetinný float čísel         */

    STATE6_u8_A = 6,                        /**<  Stavy pro zpracování klíčového slova: []u8        */
    STATE7_u8_B = 7,                        /**<  []u8.                                             */
    STATE8_u8_C = 8,                        /**<                                                    */

    STATE9_QMARK = 9,
    STATE10_QMARK_i32_A = 10,               /**<  Stavy pro zpracování klíčových slov:              */
    STATE11_QMARK_f64_A = 11,               /**<  ?i32, ?f64, ?[]u8                                 */
    STATE12_QMARK_u8_A = 12,                /**<                                                    */
    STATE13_QMARK_i32_B = 13,               /**<                                                    */
    STATE14_QMARK_f64_B = 14,               /**<                                                    */
    STATE15_QMARK_u8_B = 15,                /**<                                                    */
    STATE16_QMARK_u8_C = 16,                /**<                                                    */

    STATE17_DOUBLE_QUOTATION_MARKS = 17,    /**<  Stav pro úvodní zpracování stringu ""             */
    STATE18_ESCAPE_BACKSLASH = 18,          /**<  Stav pro zpracování escape sekvencí ""            */
    STATE19_ESCAPE_BACKSLASH_X = 19,        /**<  Stav pro zpracování esc. sekv. \xdd ""            */

    STATE20_BACKSLASH = 20,                 /**<  Stav pro úvodní zpracování znaku \                */
    STATE21_DOUBLE_BACKSLASH = 21,          /**<  Stav pro úvodní zpracování stringu \\             */
    STATE22_DOUBLE_BACKSLASH_LF = 22        /**<  Stav pro zpracování stringu \\ při novém řádku    */
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
 * @return Vrací konkrétní CharType.
 */
CharType scanner_charIdentity(int c);



/**
 * @brief V rámci FSM rozhodne o tom, zda je načtený řetězec znaků klíčovým slovem.
 * 
 * @param [in] value Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lexKeyflag Globální proměnná, která oznamuje na konci programu,
 *                          že se jedná o token s klíčovým slovem, takže se uvolní
 *                          DString.
 *
 * @details Rozhoduje, zda se jedná o identifikátor, nebo o jedno ze 14 klíčových
 *          slov.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * @param [out] lexKeyflag Globální proměnná, která oznamuje na konci programu,
 *                          že se jedná o token s klíčovým slovem, takže se uvolní
 *                          DString.
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
 * @param [in] lexToken Nehotový token.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * @param [out] lexKeyflag Globální proměnná, která oznamuje na konci programu,
 *                          že se jedná o token s klíčovým slovem, takže se uvolní
 *                          DString.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Načte a zapomene všechny znaky za //, dokud nenarazí na konec řádku.
 */
void scanner_stateComplexDoubleSlash();



/**
 * @brief Funkce scanneru pro zpracování složitého operátoru /.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexEqual(Token lexToken);




/**
 * @brief Funkce scanneru pro zpracování složitých operátorů > a >=.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexGreater(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitých operátorů < a <=.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexLess(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování složitého operátoru !=.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @return Vrací Token do Complex.
 */
Token scanner_stateComplexExclamation(Token lexToken);



/**
 * @brief Funkce scanneru pro zpracování a řízení zpracování složitých operátorů.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in] lexLocChar Lokální lexikální char.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
Token scanner_stateComplex(Token lexToken, int lexLocChar);



/**
 * @brief Funkce scanneru pro zpracování jednoduchých operátorů.
 * 
 * @param [in] lexToken Nehotový token.
 * @param [in] lexLocChar Lokální lexikální char.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
 * 
 * @details Rozhodne o konkrétním druhu jednoduchého operátoru.
 * 
 * @return Vrací Token do FSM.
 */
Token scanner_stateSimple(Token lexToken, int lexLocChar);



/**
 * @brief Funkce scanneru pro zpracování řetězce s číslicemi.
 *
 * @param [in] lexToken Nehotový token.
 * @param [in] lexState Globální proměnná určující podstavy.
 * @param [in,out] str Ukazatel na dynamický řetězec s načtenými znaky.
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
 * @param [out] lexStopFSM Globální proměnná pro zastavení chodu FSM.
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
