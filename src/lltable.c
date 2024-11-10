/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           lltable.c                                                 *
 * Autor:            Jan Kalina   <xkalinj00> (návrh LL-gramatiky,             *
 *                                             implementace LL-tabulky)        *
 *                   David Krejčí <xkrejcd00> (návrh LL-tabulky,               *
 *                                             tvorba množin)                  *
 *                                                                             *
 * Datum:            24.10.2024                                                *
 * Poslední změna:   08.11.2024                                                *
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
 * @author David Krejčí \<xkrejcd00> (návrh LL-tabulky, tvorba množin)
 *
 * @brief Implementační soubor pro správu LL tabulky.
 * @details Tento implementační soubor obsahuje definice funkcí a datových
 *          struktur potřebných pro správu LL tabulky v překladači.
 *          LL tabulka je používána pro LL syntaktickou analýzu a obsahuje
 *          pravidla pro přechody mezi stavy.
 */

#include "lltable.h"

/* Definice globální proměnné pro LL tabulku. Každý řádek tabulky obsahuje    */
/* kód terminálu (index) a pole hodnot pro neterminály. */
LLtable table[LL_TERMINAL_COUNT] = {
    { T_ID,                     {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      PARAMETERS_1,        PARAM_LIST,             PARAM,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_IMPORT,                 {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_IFJ,                    {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_REST_3,      THROW_AWAY_3,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_PUB,                    {     SYNTAX_ERROR,      SYNTAX_ERROR,    FUN_DEF_LIST_1,           FUN_DEF,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_FN,                     {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_CONST,                  {          PROGRAM,          PROLOGUE,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_1,      SYNTAX_ERROR,      SYNTAX_ERROR,           VAR_DEF,      MODIFIABLE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_VAR,                    {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_1,      SYNTAX_ERROR,      SYNTAX_ERROR,           VAR_DEF,      MODIFIABLE_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_IF,                     {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_4,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,                IF,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_ELSE,                   {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_WHILE,                  {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_5,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,             WHILE,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_RETURN,                 {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_6,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,            RETURN,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_ASSIGNMENT,             {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_REST_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,   POSSIBLE_TYPE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_INT,                    {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_INT_OR_NULL,            {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_FLOAT,                  {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_3,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_FLOAT_OR_NULL,          {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_4,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_STRING,                 {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_5,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_STRING_OR_NULL,         {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_1,       DATA_TYPE_6,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_VOID,                   {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_TYPE_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_DUMP,                   {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_1,       STATEMENT_3,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_DOT,                    {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_COMMA,                  {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR, PARAM_LIST_REST_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,             ARG_1 } },
    { T_COLON,                  {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,   POSSIBLE_TYPE_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_SEMICOLON,              {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_REST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_PIPE,                   {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,       NULL_COND_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_LEFT_BRACKET,           {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_REST_2,      THROW_AWAY_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_RIGHT_BRACKET,          {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      PARAMETERS_2,      SYNTAX_ERROR,      SYNTAX_ERROR, PARAM_LIST_REST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,       ARGUMENTS_2,      SYNTAX_ERROR,             ARG_2 } },
    { T_LEFT_CURLY_BRACKET,     {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,       NULL_COND_2,          SEQUENCE,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_RIGHT_CURLY_BRACKET,    {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,  STATEMENT_LIST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_EOF,                    {     SYNTAX_ERROR,      SYNTAX_ERROR,    FUN_DEF_LIST_2,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR } },
    { T_CALL_PRECEDENCE,        {     SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      THROW_AWAY_1,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,      SYNTAX_ERROR,     RETURN_REST_1,       ARGUMENTS_1,          ARG_LIST,      SYNTAX_ERROR } },
};


/**
 * @brief Najde pravidlo v LL tabulce na základě neterminálu a kódu terminálu.
 */
int LLtable_findRule(int nonTerminal, int terminal) {
    // Nastavení indexů pro binární vyhledávání
    int left = 0;
    int right = LL_TERMINAL_COUNT - 1;
    int mid;

    // Binární vyhledávání
    while(left <= right) {
        mid = (left + right) / 2;

        // Pokud najdeme odpovídající index, vrátíme hodnotu pravidla pro daný neterminál
        if(table[mid].key == terminal) {
            // Pokud pravidlo neexistuje, došlo k syntaktické chybě
            if(table[mid].value[nonTerminal] == SYNTAX_ERROR) {
                error_handle(ERROR_SYNTAX);
            }
            // Jinak pravidlo existuje a vrátíme ho
            else {
                return table[mid].value[nonTerminal];
            }
        }

        if(table[mid].key < terminal) {
            // Hledaný index je vyšší, posouváme levý index doprava
            left = mid + 1;
        }
        else {
             // Hledaný index je nižší, posouváme pravý index doleva
            right = mid - 1;
        }
    }

    // Vrací interní chybový kód, pokud terminál není nalezen v tabulce
    // (aka něco je velmi špatně, protože toto by nikdy nemělo nastat)
    error_handle(ERROR_INTERNAL);
    return ERROR_INTERNAL;
} // LLtable_findRule()

/*** Konec souboru lltable.c ***/
