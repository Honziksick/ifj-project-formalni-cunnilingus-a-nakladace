/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_table.c                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   03.12.2024                                                *
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
 * @brief Implementace správy precedenční tabulky pro precedenční syntaktickou
 *        analýzu výrazů.
 * @details Tento soubor obsahuje definice funkcí potřebných pro práci s
 *          precedenční tabulkou v syntaktické analýze. Tabulka definuje
 *          precedence pro operace a rozhoduje o pořadí výpočtů a zpracování
 *          výrazů v precedenční syntaktické analýze. Poskytuje základní
 *          prostředky pro určení precedencí a asociativit mezi operátory
 *          jazyka IFJ24.
 */

// Import submodulů parseru
#include "parser_common.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Najde pravidlo v precedenční tabulce na základě terminálu na vrcholu
 *        precedenčního zásobníku a vstupního terminálu.
 */
void PrecTable_findPrecedence(PrecTerminals stackTopTerminal, PrecTerminals inputTerminal, Precedence *precedence) {
    // Ověření platnosti předaného ukazatele - interní chyba
    if(precedence == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Pokud byl předán neplatný (nedefinovaný) precedenční terminál na vrcholu zásobníku
    if(stackTopTerminal == T_PREC_UNDEFINED) {
        parser_errorWatcher(SET_ERROR_INTERNAL);    // interní chyba
        *precedence = P_PRECEDENCE_UNDEFINED;
        return;
    }

    // Pokud byl předán neplatný (nedefinovaný) vstupní precedenční terminál
    if(inputTerminal == T_PREC_UNDEFINED) {
        parser_errorWatcher(SET_ERROR_SYNTAX);      // syntaktická chyba
        *precedence = P_PRECEDENCE_UNDEFINED;
        return;
    }

    /*
    * Precedenční tabulka:
    * V tabulce je řádek značen terminálem na vrcholu zásobníku. Kódem tohoto
    * terminálu je tabulka indexována. Sloupce tabulky odpovídají aktuálnímu
    * terminálu na vstupu. Souřadnice [Terminál na vrcholu zásobníku, Terminál na vstupu]
    * určují vztah precedence mezi dvěma terminály.
    *
    * Sloupce zleva doprava odpovídají pořadí řádků shora dolů:
    *                                               ID          INT_LITERAL     FLOAT_LITERAL   STRING_LITERAL    NULL_LITERAL          IFJ              DOT         LEFT_BRACKET     RIGHT_BRACKET        PLUS             MINUS       MULTIPLICATION      DIVISION          IDENTITY        NOT_EQUAL        LESS_THAN      GREATER_THAN      LESS_EQUAL      GREATER_EQUAL         COMMA           DOLLAR
    */
    const PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT] = {
        { T_PREC_ID,                       {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_INT_LITERAL,              {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_FLOAT_LITERAL,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_STRING_LITERAL,           {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_NULL_LITERAL,             {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_IFJ,                      {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,         P_EQUAL,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
        { T_PREC_DOT,                      {         P_EQUAL,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR } },
        { T_PREC_LEFT_BRACKET,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,         P_EQUAL,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR } },
        { T_PREC_RIGHT_BRACKET,            {  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,  P_SYNTAX_ERROR,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_PLUS,                     {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_MINUS,                    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_MULTIPLICATION,           {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_DIVISION,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_IDENTITY,                 {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_NOT_EQUAL,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_LESS_THAN,                {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_GREATER_THAN,             {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER,       P_GREATER } },
        { T_PREC_LESS_THAN_OR_EQUAL,       {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
        { T_PREC_GREATER_THAN_OR_EQUAL,    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
        { T_PREC_COMMA,                    {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,       P_GREATER,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,       P_GREATER,       P_GREATER } },
        { T_PREC_DOLLAR,                   {          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,  P_SYNTAX_ERROR,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,          P_LESS,  P_SYNTAX_ERROR } }
    }; // PrecedenceTable precedenceTable[]

    // Nastavení indexů pro binární vyhledávání
    int left = 0;
    int right = PREC_TERMINAL_COUNT - 1;
    int mid;

    // Binární vyhledávání
    while(left <= right) {
        mid = (left + right) / 2;

        // Pokud najdeme odpovídající index, vrátíme hodnotu precedence pro daný terminál
        if(precedenceTable[mid].key == stackTopTerminal) {
            // Pokud precedence značí snytaktickou chybu, došlo k syntaktické chybě
            if(precedenceTable[mid].value[inputTerminal] == P_SYNTAX_ERROR) {
                parser_errorWatcher(SET_ERROR_SYNTAX);
                *precedence = P_SYNTAX_ERROR;
                return;
            }
            // Jinak je precedence platná a vrátíme ji
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

    // Pokud precedence nebyla v tabulce nalezena nastavujeme syntaktickou chybu
    parser_errorWatcher(SET_ERROR_SYNTAX);
    *precedence = P_SYNTAX_ERROR;
} // PrecTable_findPrecedence()

/**
 * @brief Namapuje NEterminál, ze kterého bylo předáno řízení precedenčnímu
 *        syntaktickému analyzátoru, na odpovídající "dollar" terminál.
 */
void PrecTable_getDollarTerminalFromContext(LLNonTerminals fromNonTerminal, \
                                            DollarTerminals *currentDollar)
{
    // Ověření platnosti předaného ukazatele
    if(currentDollar == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    // Mapujeme typ LL NEterminálu na "dollar" terminál precedenčního parseru
    switch(fromNonTerminal) {
        // Mapování: NT_VAR_DEF, NT_STATEMENT, NT_STATEMENT_REST, NT_THROW_AWAY -> DOLLAR_T_SEMICOLON
        case NT_VAR_DEF:
        case NT_STATEMENT:
        case NT_STATEMENT_REST:
        case NT_THROW_AWAY:
            *currentDollar = DOLLAR_T_SEMICOLON; // "dollar" terminálem je středník
            break;

        // Mapování: NT_IF, NT_WHILE -> DOLLAR_T_RIGHT_BRACKET
        case NT_IF:
        case NT_WHILE:
            *currentDollar = DOLLAR_T_RIGHT_BRACKET; // "dollar" terminálem pravá závorka
            break;

        // Mapování: NT_ARGUMENTS -> DOLLAR_T_COMMA
        case NT_ARGUMENTS:
            *currentDollar = DOLLAR_T_COMMA; // "dollar" terminálem je čárka
            break;

        // Pro jakýkoliv jiný NEterminál není "dollar" terminál definován (nemělo by nastat)
        default:
            parser_errorWatcher(SET_ERROR_INTERNAL);
            *currentDollar = CURRENT_DOLLAR_UNDEFINED;  // značí syntaktickou chybu
            break;
    }
} // PrecTable_getDollarTerminalFromContext()

/**
 * @brief Mapuje typ vstupního precedenčního terminálu na "dollar" terminál.
 */
void PrecParser_mapInTerminalToDollar(int bracketDepth, DollarTerminals dollarContext, \
                                      PrecTerminals *inTerminal)
{
    // Ověření platnosti předaného ukazatele
    if(inTerminal == NULL) {
        parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Pokud je volána s nedefinovaným kontextovým terminálem, nastavujeme chybu
    if(dollarContext == CURRENT_DOLLAR_UNDEFINED) {
        parser_errorWatcher(SET_ERROR_SYNTAX);
        return;
    }

    // Mapujeme typ vstupního precedenčního terminálu na "dollar" terminál
    switch(currentTerminal.PrecTerminal) {
        // Mapování: T_PREC_RIGHT_BRACKET -> T_PREC_RIGHT_BRACKET nebo T_PREC_DOLLAR
        //         : U pravé závorky musíme brát v potaz aktuální úroveň zanoření
        case T_PREC_RIGHT_BRACKET:
            if(dollarContext ==  DOLLAR_T_RIGHT_BRACKET && bracketDepth == 0) {
                *inTerminal = T_PREC_DOLLAR;
            } else if(dollarContext ==  DOLLAR_T_COMMA && bracketDepth == 0) {
                *inTerminal = T_PREC_DOLLAR;
            }
            else {
                *inTerminal = currentTerminal.PrecTerminal;
            }
            break;

        // Mapování: T_PREC_COMMA -> T_PREC_COMMA nebo T_PREC_DOLLAR
        case T_PREC_COMMA:
                *inTerminal = T_PREC_DOLLAR;
            break;

        // Výchozí případ: Nejedná se o "dollar" terminál
        default:
            *inTerminal = currentTerminal.PrecTerminal; // vrací nezměněný typ terminálu
            break;
    } // switch()
} // PrecParser_mapInTerminalToDollar()

/*** Konec souboru precedence_table.c ***/
