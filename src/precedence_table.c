/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.c                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   13.11.2024                                                *
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
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace správy precedenční tabulky.
 * @details Obsahuje definice funkcí a struktury pro správu precedenční tabulky,
 *          která slouží k vyhodnocování precedencí a pravidel pro operátory
 *          v jazyce IFJ24. Tento modul zajišťuje, že výrazy jsou analyzovány
 *          podle stanovených pravidel precedenční syntaktické analýzy.
 */

#include "precedence_table.h"

/*******************************************************************************
 *                                                                             *
 *                    GLOBÁLNÍ PROMĚNNÉ PRO PRECEDENČNÍ SA                     *
 *                                                                             *
 ******************************************************************************/

/*
 * V tabulce je řádek značen terminálem na vrcholu zásobníku. Kódem tohoto
 * terminálu je tabulka indexována. Sloupce tabulky odpovídají aktuálnímu
 * terminálu na vstupu. Souřadnice [Terminál na vrcholu zásobníku, Terminál na vstupu]
 * určují vztah precedence mezi dvěma terminály.
 *
 * Sloupce zleva doprava odpovídají pořadí řádků shora dolů:
 *                                               ID          INT_LITERAL     FLOAT_LITERAL   STRING_LITERAL    NULL_LITERAL          IFJ              DOT         LEFT_BRACKET     RIGHT_BRACKET        PLUS             MINUS       MULTIPLICATION      DIVISION          IDENTITY        NOT_EQUAL        LESS_THAN      GREATER_THAN      LESS_EQUAL      GREATER_EQUAL         COMMA           DOLLAR
 */
const struct PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT] = {
    { T_PREC_ID,                       {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_INT_LITERAL,              {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_FLOAT_LITERAL,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_STRING_LITERAL,           {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_NULL_LITERAL,             {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_IFJ,                      {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,         P_EQUAL,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
    { T_PREC_DOT,                      {         P_EQUAL,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
    { T_PREC_LEFT_BRACKET,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,         P_EQUAL,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR } },
    { T_PREC_RIGHT_BRACKET,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_PLUS,                     {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_MINUS,                    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_MULTIPLICATION,           {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_DIVISION,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_IDENTITY,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_NOT_EQUAL,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LESS_THAN,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_GREATER_THAN,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LESS_THAN_OR_EQUAL,       {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_GREATER_THAN_OR_EQUAL,    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_COMMA,                    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_DOLLAR,                   {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR } }
};


/*
 * Tato proměnná uchovává ukazatel na aktuální FOLOW množinu, která je používána
 * precedenčním parserem. Je aktualizována při zahájení precedenční syntaktické
 * analýzy a deinicializována při vrácení řízení zpět do LL parseru.
 */
PrecDollars currentDollar = { NT_UNDEFINED, CURRENT_DOLLAR_UNDEFINED };


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v precedenční tabulce na základě terminálů.
 */
void PrecTable_findPrecedence(PrecTerminals stackTopTerminal, PrecTerminals inputTerminal, Precedence *precedence) {
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
                *precedence = P_SYNTAX_ERROR;
                return;
            }
            // Jinak pravidlo existuje a vrátíme ho
            else {
                *precedence = precedenceTable[mid].value[inputTerminal];
                return;
            }
        }

        // Hledaný index je vyšší, posouváme levý index doprava
        if(precedenceTable[mid].key < stackTopTerminal) {
            left = mid + 1;
        }
        // Hledaný index je nižší, posouváme pravý index doleva
        else {
            right = mid - 1;
        }
    }

    // Vrací interní chybový kód, pokud terminál není nalezen v tabulce
    // (aka něco je velmi špatně, protože toto by nikdy nemělo nastat)
    error_handle(ERROR_INTERNAL);
} // PrecTable_findPrecedence()


/**
 * @brief Získá FOLOW množinu pro daný NEterminál pomocí binárního vyhledávání
 *        a aktualizuje globální proměnnou.
 */
void PrecTable_getFollowSet(LLNonTerminals fromNonTerminal) {
    switch(fromNonTerminal) {
        case NT_VAR_DEF:
        case NT_STATEMENT_REST:
        case NT_THROW_AWAY:
        case NT_RETURN_REST:
            currentDollar.followSet = DOLLAR_T_SEMICOLON;
            break;

        case NT_IF:
        case NT_WHILE:
            currentDollar.followSet = DOLLAR_T_RIGHT_BRACKET;
            break;

        case NT_ARGUMENTS:
            currentDollar.followSet = DOLLAR_T_COMMA;
            break;

        default:
            currentDollar.followSet = CURRENT_DOLLAR_UNDEFINED;
            break;
    }

    // Uložíme neterminál, ze kterého jmen do precedenčního parseru vstoupili
    currentDollar.fromNonTerminal = fromNonTerminal;
} // PrecTable_getFollowSet()

/*** Konec souboru precedence_table.c ***/
