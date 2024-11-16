/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           error.c                                                   *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            09.10.2024                                                *
 * Poslední změna:   16.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file error.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace funkcí pro řízení chybových stavů.
 * @details Tento zdrojový soubor obsahuje implementaci funkcí deklarovaných v
 *          hlavičkovém souboru `error.h`. Funkce v tomto souboru zajišťují
 *          zpracování chybových kódů, tisk chybových zpráv, uvolnění alokovaných
 *          dat a ukončení programu s příslušným chybovým kódem. Funkce také
 *          poskytují informace o souboru, řádku a funkci, kde chyba nastala,
 *          a umožňují extrakci názvu souboru z cesty.
 */

#include "error.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Funkce pro zpracování chybového kódu.
 */
void error_internalHandle(ErrorType error, const char *file, int line, const char *func) {
    // Extrahujeme název souboru z cesty
    const char* fileName = error_getFileName(file);

    // Vytiskneme chybovou zprávu s kódem chyby
    error_printMessage(error);

    // Vytiskneme informace o souboru, řádku a funkci, kde chyba nastala
    fprintf(stderr, YELLOW_COLOR "In file: %s:%d (%s)\n" RESET_COLOR, fileName, line, func);

    // Uvolníme alokovaná data (je-li to potřeba)
    //error_freeAllocatedData(error);

    // Ukončíme program s chybovým kódem odpovídajícím kódu zahlášené chyby
    error_killMePlease(error);
} // error_internalHandle()


/**
 * @brief Funkce pro tisk příslušné chybové zprávy na `STDERR`.
 */
void error_printMessage(ErrorType error) {
    switch(error) {
        // Lexikální chyba
        case ERROR_LEXICAL:
            fprintf(stderr, RED_COLOR "(Error %d) Lexical error: invalid " \
                            "structure of the current lexeme.\n" RESET_COLOR, error);
            break;

        // Syntaktická chyba
        case ERROR_SYNTAX:
            fprintf(stderr, RED_COLOR "(Error %d) Syntax error: invalid program " \
                            "syntax, missing header, etc.\n" RESET_COLOR, error);
            break;

        // Sémantická chyba: nedefinovaná funkce či proměnná
        case ERROR_SEM_UNDEF:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: undefined " \
                            "function or variable.\n" RESET_COLOR, error);
            break;

        // Sémantická chyba: špatný počet/typ parametrů u volání funkce
        case ERROR_SEM_PARAMS_OR_RETVAL:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: incorrect " \
                            "number/type of parameters in function call; " \
                            "incorrect type or disallowed discard of function " \
                            "return value.\n" RESET_COLOR, error);
            break;

        // Sémantická chyba: redefinice proměnné nebo funkce
        case ERROR_SEM_REDEF_OR_CONSTDEF:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: redefinition " \
                            "of variable or function; assignment to a " \
                            "non-modifiable variable.\n" RESET_COLOR, error);
            break;

        // Sémantická chyba: chybějící/přebývající výraz v příkazu návratu z funkce
        case ERROR_SEM_RETURN_EXP:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: missing/excessive " \
                            "expression in function return statement.\n" RESET_COLOR,
                            error);
            break;

        // Sémantická chyba: typová nekompatibilita ve výrazech
        case ERROR_SEM_TYPE_COMPATIBILITY:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: type " \
                            "incompatibility in arithmetic, string, and " \
                            "relational expressions; incompatible expression " \
                            "type (e.g., in assignment).\n" RESET_COLOR, error);
            break;

        // Sémantická chyba: typ proměnné není uveden a nelze odvodit od použitého výrazu
        case ERROR_SEM_TYPE_INFERENCE:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: variable type " \
                            "not specified and cannot be inferred from the " \
                            "expression used.\n" RESET_COLOR, error);
            break;

        // Sémantická chyba: nevyužitá proměnná v jejím rozsahu platnosti
        case ERROR_SEM_UNUSED_VAR:
            fprintf(stderr, RED_COLOR "(Error %d) Semantic error: unused variable " \
                            "within its scope; modifiable variable without " \
                            "the possibility of change after initialization.\n" \
                            RESET_COLOR, error);
            break;

        // Ostatní sémantické chyby
        case ERROR_SEM_OTHER:
            fprintf(stderr, RED_COLOR "(Error %d) Other semantic errors.\n" \
                            RESET_COLOR, error);
            break;

        // Interní chyba překladače
        default:
            fprintf(stderr, RED_COLOR "(Error %d) Internal compiler error: not " \
                            "influenced by the input program (e.g., memory " \
                            "allocation error).\n" RESET_COLOR, error);
            break;
    } // switch()
} // error_printMessage()

/**
 * @brief Funkce pro uvolnění alokovaných dat z haldy před ukončením programu
 */
void error_freeAllocatedData(ErrorType error) {
    (void)error;
} // error_freeAllocatedData()

/**
 * @brief Funkce pro ukončení programu s příslušným chybovým kódem.
 */
inline void error_killMePlease(ErrorType error) {
    exit(error);
} // error_killMePlease()

/**
 * @brief Extrahuje název souboru z cesty.
 */
const char *error_getFileName(const char *path) {
    // Najdeme poslední výskyt znaku '/'
    const char* fileName = strrchr(path, '/');

    // Pokud najdeme '/', vrátíme ukazatel na následující znak (tj. název souboru)
    if(fileName != NULL) {
        return fileName + 1;
    }

    // Pokud nenajdeme '/', vrátíme původní cestu
    return path;
} // error_getFileName()

/*** Konec souboru error.c ***/
