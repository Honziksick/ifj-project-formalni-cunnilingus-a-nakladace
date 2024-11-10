/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.c                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   10.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file precedence_table.c
 * @author Jan Kalina   \<xkalinj00>
 *
 * @brief Implementační soubor pro správu precedenční tabulky pro precedenční SA.
 * @details Tento implementační soubor obsahuje definice funkcí a datových
 *          struktur potřebných pro správu precedenční tabulky pro precedenční SA.
 *          Precedenční tabulka obsahuje pravidla pro zpracování výrazů.
 */

#include "precedence_table.h"

/*******************************************************************************
 *                                                                             *
 *                        GLOBÁLNÍ Precedenční Tabulka                         *
 *                                                                             *
 ******************************************************************************/

/*
 * V tabulce je řádek značen terminálem na vrcholu zásobníku. Kódem tohoto
 * terminálu je tabulka indexována. Sloupce tabulky odpovídají aktuálnímu
 * terminálu na vstupu. Souřadnice [Terminál na vrcholu zásobníku, Terminál na vstupu]
 * určují vztah precedence mezi dvěma terminály.
 */
struct PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT] = {
    { T_PREC_ID,                       {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER } },
    { T_PREC_INT_LITERAL,              {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER } },
    { T_PREC_FLOAT_LITERAL,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER } },
    { T_PREC_STRING_LITERAL,           {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER } },
    { T_PREC_NULL_LITERAL,             {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER } },
    { T_PREC_IFJ,                      {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER } },
    { T_PREC_DOT,                      {         P_EQUAL,          P_LESS,          P_LESS,          P_LESS,          P_LESS,         P_EQUAL,       P_GREATER,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LEFT_BRACKET,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,         P_EQUAL,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR } },
    { T_PREC_RIGHT_BRACKET,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER } },
    { T_PREC_PLUS,                     {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_MINUS,                    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_MULTIPLICATION,           {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_DIVISION,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_IDENTITY,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_NOT_EQUAL,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LESS_THAN,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_GREATER_THAN,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LESS_THAN_OR_EQUAL,       {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_GREATER_THAN_OR_EQUAL,    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_CALL_LL,                  {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
}; // PrecedenceTable precTable[];


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v precedenční tabulce na základě terminálů.
 */
int PrecTable_findRule(int stackTopTerminal, int inputTerminal) {
    // Nastavení indexů pro binární vyhledávání
    int left = 0;
    int right = PREC_TERMINAL_COUNT - 1;
    int mid;

    // Binární vyhledávání
    while(left <= right) {
        mid = (left + right) / 2;

        // Pokud najdeme odpovídající index, vrátíme hodnotu precedence pro daný terminál
        if(precedenceTable[mid].key == stackTopTerminal) {
            // Pokud pravidlo neexistuje, došlo k syntaktické chybě
            if(precedenceTable[mid].value[inputTerminal] == P_SYNTAX_ERROR) {
                error_handle(ERROR_SYNTAX);
            }
            // Jinak pravidlo existuje a vrátíme ho
            else {
                return precedenceTable[mid].value[inputTerminal];
            }
        }

        if(precedenceTable[mid].key < stackTopTerminal) {
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
} // PrecTable_findRule()

/*** Konec souboru precedence_table.c ***/
