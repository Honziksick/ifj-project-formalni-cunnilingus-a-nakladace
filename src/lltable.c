/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable.c                                                 *
 * Autor:            Jan Kalina   <xkalinj00> (návrh LL-gramatiky,             *
 *                                             implementace LL-tabulky)        *
 *                   David Krejčí <xkrejcd00> (tvorba množin)                  *
 *                                                                             *
 * Datum:            24.10.2024                                                *
 * Poslední změna:   24.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file lltable.c
 * @author Jan Kalina   \<xkalinj00> (návrh LL-gramatiky, implementace LL-tabulky)
 * @author David Krejčí \<xkrejcd00> (tvorba množin)
 *
 * @brief Implementace LL tabulky.
 * @details Obsahuje implementaci LL tabulky a funkcí pro správu LL tabulky,
 *          která určuje pravidla pro syntaktickou analýzu podle LL gramatiky.
 *          Implementace zajišťuje, že analyzátor provádí přechody mezi stavy
 *          podle daných pravidel a umožňuje správné parsování.
 */

#include "parser_common.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v LL tabulce na základě neterminálu a kódu terminálu.
 */
void LLtable_findRule(LLTerminals terminal, LLNonTerminals nonTerminal, LLRuleSet *rule) {
    // Ověření platnosti předaného ukazatele - interní chyba
    if(rule == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        *rule = RULE_UNDEFINED;
        return;
    }

    // Pokud byl předán neplatný (nedefinovaný) LL NEterminál - interní chyba
    if(nonTerminal == NT_UNDEFINED) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        *rule = RULE_UNDEFINED;
        return;
    }

    // Pokud byl předán neplatný (nedefinovaný) LL terminál - syntaktická chyba
    if(terminal == T_UNDEFINED) {
        parser_errorWatcher(SET_ERROR_SYNTAX);
        *rule = RULE_UNDEFINED;
        return;
    }

    /*
    * Inicializace LL tabulky:
    * V tabulce je příslušný řádek pro každý terminál, jejichž kódem je tabulka
    * indexována. Sloupce tabulky odpovídají jednotlivým NEterminálům. Souřadnice
    * [Terminál, NEterminál] určují aplikaci příslušného pravidla.
    *
    * Sloupce odpovídají NEterminálům:
    *                                    PROGRAM           PROLOGUE         FUN_DEF_LIST          FUN_DEF           PARAMETERS         PARAM_LIST           PARAM         PARAM_LIST_REST       RETURN_TYPE        DATA_TYPE      STATEMENT_LIST         STATEMENT        STATEMENT_REST       THROW_AWAY          VAR_DEF           MODIFIABLE       POSSIBLE_TYPE            IF             NULL_COND           SEQUENCE            WHILE             ARGUMENTS
    */
    const struct LLtable LLtable[LL_TERMINAL_COUNT] = {
        { T_ID,                     {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      PARAMETERS_1,        PARAM_LIST,             PARAM,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_IMPORT,                 {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_IFJ,                    {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_7,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_PUB,                    {  SYNTAX_ERROR,      SYNTAX_ERROR,    FUN_DEF_LIST_1,           FUN_DEF,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_FN,                     {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_CONST,                  {       PROGRAM,          PROLOGUE,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_1,      SYNTAX_ERROR,      SYNTAX_ERROR,           VAR_DEF,      MODIFIABLE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_VAR,                    {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_1,      SYNTAX_ERROR,      SYNTAX_ERROR,           VAR_DEF,      MODIFIABLE_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_IF,                     {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_4,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,                IF,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_ELSE,                   {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_WHILE,                  {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_5,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,             WHILE,      SYNTAX_ERROR } },
        { T_RETURN,                 {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_6,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_ASSIGNMENT,             {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_REST_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,   POSSIBLE_TYPE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_INT,                    {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_INT_OR_NULL,            {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_FLOAT,                  {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_3,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_FLOAT_OR_NULL,          {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_4,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_STRING,                 {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_5,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_STRING_OR_NULL,         {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_6,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_VOID,                   {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_DUMP,                   {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_3,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_DOT,                    {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_COMMA,                  {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR, PARAM_LIST_REST_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_COLON,                  {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,   POSSIBLE_TYPE_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_SEMICOLON,              {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_PIPE,                   {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,       NULL_COND_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_LEFT_BRACKET,           {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_REST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_RIGHT_BRACKET,          {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      PARAMETERS_2,      SYNTAX_ERROR,      SYNTAX_ERROR, PARAM_LIST_REST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_LEFT_CURLY_BRACKET,     {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,       NULL_COND_2,          SEQUENCE,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_RIGHT_CURLY_BRACKET,    {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_EOF,                    {  SYNTAX_ERROR,      SYNTAX_ERROR,    FUN_DEF_LIST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
        { T_CALL_PRECEDENCE,        {  SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,        THROW_AWAY,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,         ARGUMENTS } },
    }; // LLtable LLtable[];

    // Nastavení indexů pro binární vyhledávání
    int left = 0;
    int right = LL_TERMINAL_COUNT - 1;
    int mid;

    // Binární vyhledávání
    while(left <= right) {
        mid = (left + right) / 2;

        // Pokud najdeme odpovídající index, vrátíme hodnotu pravidla pro daný neterminál
        if(LLtable[mid].key == terminal) {
            // Pokud pravidlo neexistuje, došlo k syntaktické chybě
            if(LLtable[mid].value[nonTerminal] == SYNTAX_ERROR) {
                parser_errorWatcher(SET_ERROR_SYNTAX);
                *rule = SYNTAX_ERROR;
                return;
            }
            // Jinak pravidlo existuje a vrátíme ho
            else {
                *rule = LLtable[mid].value[nonTerminal];
                return;
            }
        }

        // Hledaný index je vyšší, posouváme levý index doprava
        if(LLtable[mid].key < terminal) {
            left = mid + 1;
        }
        // Hledaný index je nižší, posouváme pravý index doleva
        else {
            right = mid - 1;
        }
    }

    // Pokud terminál nebyl v LL tabulce nalezen nastavujeme syntaktickou chybu
    parser_errorWatcher(SET_ERROR_SYNTAX);
    *rule = SYNTAX_ERROR;
} // LLtable_findRule()

/*** Konec souboru lltable.c ***/
