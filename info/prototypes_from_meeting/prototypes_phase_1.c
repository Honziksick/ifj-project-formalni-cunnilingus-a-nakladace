/******************************************************************************/
/*                            LEXIKÁLNÍ ANALYZÁTOR                            */
/******************************************************************************/

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

/* Zeptám se, zda můžeme používat ctype.h
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
*/

/* možná další is_něco() funkce nebo jiné funkce
   - je nějak potřeba poznat, zda se jedná o escape sekvenci
   - pozor na case sesitivitu
   - kontrolovat, zda má lexém správnou strukturu (např. pavel%honza obsahuje neplatný znak )
   - ...
*/

/******************************************************************************/
/*                       KNIHOVNA PRO DYNAMICKÝ STRING                        */
/******************************************************************************/

/*
   - ještě mě napadlo, že pokud bychom skutečně chtěli nějakým způsobem využívat
     funkce ze String.h jako je strcmp(), tak stringy musí mít komcový nulový znak,
     aby s nimi tyto funkce uměly pracovat
*/

typedef struct {
    char *str;                    // Ukazatel na dynamické pole znaků, které tvoří řetězec ukončený '\0'
    unsigned int allocatedSize;   // Velikost alokované paměti (kapacita řetězce včetně volných míst)
    unsigned int length;          // Skutečná délka řetězce (počet znaků před ukončovacím znakem '\0')
} String;

/* Např. init voláme:
    #define STR_SIZE 10
    string_init(STR_SIZE) -default
    string_init(konkretni_delka) – když by se do něj kopíroval String známé délky
*/

// Inicializace dynamického řetězce s počáteční kapacitou (Domluvit se !!!)
String string_init(unsigned int length);

// Uvolní paměť dynamického řetězce
void string_free(String *s);

// Přidá jeden znak na konec dynamického řetězce
int string_appendChar(String *s, char c);

// Zkopíruje obsah jednoho dynamického řetězce do druhého
int string_copy(String *s1, String *s2);

// Porovná dva dynamické řetězce
int string_compare(String *s1, String *s2);

// Porovná dynamický řetězec s konstantním C řetězcem
int string_compareConstStr(String *s1, const char *s2);

// Získá aktuální délku dynamického řetězce
unsigned int string_getLength(String *s);

// Získá aktuální kapacitu (tj. alokovanou paměť / alokovaný počet znaků) dynamického řetězce
unsigned int string_GetAllocatedSize(String *s);

/******************************************************************************/
/*                    KNIHOVNA PRO ŘÍZENÍ CHYBOVÝCH STAVŮ                     */
/******************************************************************************/

// Typy chyb, které mohou nastat v různých fázích překladače
typedef enum {
    ERROR_LEXICAL      // Lexikální chyba (např. neplatný token)
    // viz zadání
} ErrorType;

// Vypsání chyby na STDERR a ukončení programu (volána z jiných knihoven)
void error_handle(ErrorType error);

void error_printMessage(ErrorType error);

// Uvolnění alokovaných dat z haldy
void error_freeAllocatedData(ErrorType error);

/******************************************************************************/
/*                      KNIHOVNA PRO TABULKU SYMBOLŮ TRP                      */
/******************************************************************************/

// TO-DO
