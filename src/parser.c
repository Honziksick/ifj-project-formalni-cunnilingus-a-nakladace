/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser.c                                                  *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   12.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file parser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace sdílených funkcí a proměnných pro parser.
 * @details Tento soubor implementuje sdílené funkce a globální proměnné, které
 *          ostatní moduly parseru využívají k syntaktické analýze. Umožňuje
 *          správu aktuálního tokenu a vytváření abstraktního syntaktického
 *          stromu (AST), což jsou základní komponenty potřebné pro práci parseru.
 */

#include "parser.h"

/*******************************************************************************
 *                                                                             *
 *                POČÁTEČNÍ INICIALIZACE GLOBÁLNÍCH PROMĚNNÝCH                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální statická proměnná pro aktuální token, který je zpracováván.
 */
Token currentToken = { TOKEN_EOF, NULL};        // počáteční inicializace

/**
 * @brief Globální kořen abstraktního syntaktického stromu.
 */
AST_ProgramNode *ASTroot = NULL;                // počáteční inicializace


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Získá další token ze scanneru a aktualizuje globální současný token.
 */
inline void Parser_pokeScanner() {
    currentToken = scanner_getNextToken();
}

/**
 * @brief Přidá suffix k jménu proměnné ve formátu `$frameID$`.
 */
void Parser_addIdSuffix(size_t frameID, DString *id) {
    if(id == NULL || id->str == NULL) {
        error_handle(ERROR_INTERNAL);
        return;
    }

    // Dočasný C řetězec o velikost maximálního počtu číslic 'size_t'
    char suffix[MAX_SIZE_T_DIGITS] = "\0";

    // Převod hodnoty "frameID" na znaky
    snprintf(suffix, MAX_SIZE_T_DIGITS*sizeof(char), "%zu", frameID);

    // Přidání uvozovacího oddělovače frameID od indetifikátoru
    int error = string_append_char(id, FRAME_ID_SEPARATOR);

    // Přípojení hodnoty "frameID" jako suffix identifikátoru
    for(int i = 0; i < (int)strlen(suffix); i++) {
        error = string_append_char(id, suffix[i]);
        Parser_checkAppendSuccess(error);
    }

    // Přidání ukončovacího oddělovače frameID od indetifikátoru
    error = string_append_char(id, FRAME_ID_SEPARATOR);
    Parser_checkAppendSuccess(error);
}


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Kontroluje úspěšnost připojení znaku do DString a případně hlásí chybu.
 */
inline void Parser_checkAppendSuccess(int error) {
    // Pokud připojení znaku selhalo, hlásíme interní chybu
    if(error == STRING_RESIZE_FAIL) {
        error_handle(ERROR_INTERNAL);
    }
} // parser_checkAppendSuccess()

/*** Konec souboru parser.c ***/
