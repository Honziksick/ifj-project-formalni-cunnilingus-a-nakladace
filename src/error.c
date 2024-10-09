/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           error.c                                                   *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            09.10.2024                                                *
 * Poslední změna:   09.10.2024                                                *
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
 * @brief Implementací funkcí knihovny pro řízení chybových stavů.
 * @details Tento zdrojový soubor obsahuje implementaci funkcí uvedených v
 *          hlavičkovém souboru `error.h`
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/**
 * @brief Funkce pro zpracování chybového kódu.
 */
void error_handle(ErrorType error) {
    error_printMessage(error);
    //error_freeAllocatedData(error);
    error_killMePlease(error);
} // error_handle()


/**
 * @brief Funkce pro tisk příslušné chybové zprávy na `STDERR`.
 */
void error_printMessage(ErrorType error) {
    switch(error) {
        case ERROR_LEXICAL:
            fprintf(stderr, "(Error %d) Lexical error: invalid structure of " \
                            "the current lexeme.\n", ERROR_LEXICAL);
            break;
        case ERROR_SYNTAX:
            fprintf(stderr, "(Error %d) Syntax error: invalid program " \
                            "syntax, missing header, etc.\n", ERROR_SYNTAX);
            break;
        case ERROR_SEM_UNDEF:
            fprintf(stderr, "(Error %d) Semantic error: undefined function " \
                            "or variable.\n", ERROR_SEM_UNDEF);
            break;
        case ERROR_SEM_PARAMS_OR_RETVAL:
            fprintf(stderr, "(Error %d) Semantic error: incorrect number/type " \
                            "of parameters in function call; incorrect type " \
                            "or disallowed discard of function return value.\n",
                            ERROR_SEM_PARAMS_OR_RETVAL);
            break;
        case ERROR_SEM_REDEF_OR_CONSTDEF:
            fprintf(stderr, "(Error %d) Semantic error: redefinition of " \
                            "variable or function; assignment to a " \
                            "non-modifiable variable.\n", ERROR_SEM_REDEF_OR_CONSTDEF);
            break;
        case ERROR_SEM_RETURN_EXP:
            fprintf(stderr, "(Error %d) Semantic error: missing/excessive " \
                            "expression in function return statement.\n",
                            ERROR_SEM_RETURN_EXP);
            break;
        case ERROR_SEM_TYPE_COMPATIBILITY:
            fprintf(stderr, "(Error %d) Semantic error: type incompatibility " \
                            "in arithmetic, string, and relational expressions; " \
                            "incompatible expression type (e.g., in assignment).\n",
                            ERROR_SEM_TYPE_COMPATIBILITY);
            break;
        case ERROR_SEM_TYPE_INFERENCE:
            fprintf(stderr, "(Error %d) Semantic error: variable type not " \
                            "specified and cannot be inferred from the " \
                            "expression used.\n", ERROR_SEM_TYPE_INFERENCE);
            break;
        case ERROR_SEM_UNUSED_VAR:
            fprintf(stderr, "(Error %d) Semantic error: unused variable " \
                            "within its scope; modifiable variable without " \
                            "the possibility of change after initialization.\n",
                            ERROR_SEM_UNUSED_VAR);
            break;
        case ERROR_SEM_OTHER:
            fprintf(stderr, "(Error %d) Other semantic errors.\n", ERROR_SEM_OTHER);
            break;
        default:
            fprintf(stderr, "(Error %d) Internal compiler error: not " \
                            "influenced by the input program (e.g., memory " \
                            "allocation error).\n", ERROR_INTERNAL);
            break;
    }
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

/*** Konec souboru error.c ***/
