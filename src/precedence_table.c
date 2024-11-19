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
    { T_PREC_ID,                       {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_INT_LITERAL,              {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_FLOAT_LITERAL,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_STRING_LITERAL,           {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_NULL_LITERAL,             {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_IFJ,                      {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
    { T_PREC_DOT,                      {         P_EQUAL,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
    { T_PREC_LEFT_BRACKET,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
    { T_PREC_RIGHT_BRACKET,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
    { T_PREC_PLUS,                     {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_MINUS,                    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_MULTIPLICATION,           {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_DIVISION,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_IDENTITY,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_NOT_EQUAL,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LESS_THAN,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_GREATER_THAN,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_LESS_THAN_OR_EQUAL,       {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_GREATER_THAN_OR_EQUAL,    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
    { T_PREC_COMMA,                    {          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER } },
    { T_PREC_DOLLAR,                   {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,  P_SYNTAX_ERROR } },
}; // PrecedenceTable precTable[];

/*
 * Toto pole obsahuje FOLOW množiny pro všechny NEterminály, ze kterých se dá
 * předat řízení precedenčnímu SA, s explicitním počtem symbolů v každé množině.
*/
const struct PrecDollars precDollars[FOLLOW_NON_TERMINALS_COUNT] = {
    { CALL_PREC_NT_VAR_DEF,        { DOLLAR_T_SEMICOLON,     CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_STATEMENT_REST, { DOLLAR_T_SEMICOLON,     CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_THROW_AWAY,     { DOLLAR_T_SEMICOLON,     CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_IF,             { DOLLAR_T_RIGHT_BRACKET, CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_WHILE,          { DOLLAR_T_RIGHT_BRACKET, CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_RETURN_REST,    { DOLLAR_T_SEMICOLON,     CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_ARGUMENTS,      { DOLLAR_T_RIGHT_BRACKET, CURRENT_DOLLAR_UNDEFINED } },
    { CALL_PREC_NT_ARG_LIST,       { DOLLAR_T_COMMA,         DOLLAR_T_RIGHT_BRACKET   } },
    { CALL_PREC_NT_ARG,            { DOLLAR_T_COMMA,         DOLLAR_T_RIGHT_BRACKET   } },
}; // PrecDollars precDollars[];

/*
 * Tato proměnná uchovává ukazatel na aktuální FOLOW množinu, která je používána
 * precedenčním parserem. Je aktualizována při zahájení precedenční syntaktické
 * analýzy a deinicializována při vrácení řízení zpět do LL parseru.
 */
DollarTerminals currentDollar[MAX_DOLLARS_IN_SET] = { CURRENT_DOLLAR_UNDEFINED, CURRENT_DOLLAR_UNDEFINED };


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
    // Nastavení indexů pro binární vyhledávání
    int left = 0;
    int right = FOLLOW_NON_TERMINALS_COUNT - 1;
    int mid;

    // Namapujeme obdržený LL neterminál na precedenční "follow" neterminál
    CallPrecNonTerminals followNonTerminal = CALL_PREC_NT_UNDEFINED;
    PrecTable_mapFollowNonTerminal(fromNonTerminal, &followNonTerminal);

    // Binární vyhledávání
    while(left <= right) {
        mid = left + (right - left) / 2;

        // Pokud najdeme odpovídající index, nastavíme aktuální množinu "dolar" terminálů
        if(precDollars[mid].fromNonTerminal == followNonTerminal) {
            memcpy(currentDollar, precDollars[mid].followSet, sizeof(precDollars[mid].followSet));
            return;
        }

        // Hledaný index je vyšší, posouváme levý index doprava
        if(precDollars[mid].fromNonTerminal < followNonTerminal) {
            left = mid + 1;
        }
        // Hledaný index je nižší, posouváme pravý index doleva
        else {
            right = mid - 1;
        }
    }

    // Pokud nebyla množina FOLLOW nalezena, hlásíme interní chybu překladače
    error_handle(ERROR_INTERNAL);
} // PrecTable_getFollowSet()

/**
 * @brief Kontroluje, zda je daný terminál v aktuální FOLLOW množině.
 */
bool PrecTable_isInFollowSet(DollarTerminals terminal) {
    // Prohledáváme pole "dollar" terminálů do jeho konce nebo nalezení 'CURRENT_DOLLAR_UNDEFINED'
    for (int i = 0; i < MAX_DOLLARS_IN_SET; i++) {
        // "Dolar" terminál nalezen
        if (currentDollar[i] == terminal) {
            return true;
        }
        // "Dolar" terminál NEnalezen - došli jsme na neinicializovanou část pole
        else if(currentDollar[i] == CURRENT_DOLLAR_UNDEFINED) {
            return false;
        }
    }

    // "Dolar" terminál NEnalezen - prošli jsme celé pole, ale nebyl tam
    return false;
} // PrecTable_isInFollowSet()


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/


/**
 * @brief Mapuje neterminál z LL syntaktické analýzy na neterminál pro předání
 *        řízení precedenčnímu syntaktickému analyzátoru.
 */
void PrecTable_mapFollowNonTerminal(LLNonTerminals fromNonTerminal, CallPrecNonTerminals *followNonTerminal) {
    // Pokud byl předán neplatný ukazatel, hlásíme interní chybu
    if (followNonTerminal == NULL) {
        error_handle(ERROR_INTERNAL);
        return;
    }

    // Mapování LL neterminálů na precedenční "follow" neterminály
    switch(fromNonTerminal) {
        case NT_STATEMENT_REST:
            *followNonTerminal = CALL_PREC_NT_STATEMENT_REST;
            break;
        case NT_THROW_AWAY:
            *followNonTerminal = CALL_PREC_NT_THROW_AWAY;
            break;
        case NT_VAR_DEF:
            *followNonTerminal = CALL_PREC_NT_VAR_DEF;
            break;
        case NT_IF:
            *followNonTerminal = CALL_PREC_NT_IF;
            break;
        case NT_WHILE:
            *followNonTerminal = CALL_PREC_NT_WHILE;
            break;
        case NT_RETURN_REST:
            *followNonTerminal = CALL_PREC_NT_RETURN_REST;
            break;
        case NT_ARG_LIST:
            *followNonTerminal = CALL_PREC_NT_ARG_LIST;
            break;
        case NT_ARG:
            *followNonTerminal = CALL_PREC_NT_ARG;
            break;
        // Pokud se nejednalo ani o jeden case výše, došlo k interní chybě (nemělo by nikdy nastat)
        default:
            error_handle(ERROR_INTERNAL);
            break;
    }
} // PrecTable_mapFollowNonTerminal()

/*** Konec souboru precedence_table.c ***/
