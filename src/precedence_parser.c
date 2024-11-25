/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_parser.c                                       *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            10.11.2024                                                *
 * Poslední změna:   23.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file precedence_parser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementační soubor pro funkce pro precedenční syntaktickou analýzu
 *        výrazů v jazyce IFJ24.
 * @details Tento soubor obsahuje definice funkcí pro precedenční syntaktickou
 *          analýzu výrazů a operátorů v jazyce IFJ24. Precedenční syntaktický
 *          analyzátor zpracovává výrazy a operátory, implementuje algoritmy pro
 *          posun a redukci a vytváří abstraktní syntaktický strom (AST) pro
 *          další zpracování během překladu.
 */

#include "precedence_parser.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Spustí precedenční syntaktickou analýzu.
 *
 * @details Tato funkce spustí precedenční syntaktickou analýzu na základě
 *          aktuálního stavu zásobníku a vstupního tokenu.
 *
 * @param fromNonTerminal Neterminál předávající řízení precedenčnímu syntaktickému analyzátoru.
 */
AST_ExprNode *PrecParser_parse(LLNonTerminals fromNonTerminal) {
    // Zkontrolujeme, že v kontextu NEterminálu neparsujeme prázdný výraz
    if(PrecParser_parsingEmptyExpression(fromNonTerminal)) {
        return NULL;
    }

    // Namapujeme aktuální "dollar" terminál
    DollarTerminals currentDollar = CURRENT_DOLLAR_UNDEFINED;
    PrecTable_getDollarTerminalFromContext(fromNonTerminal, &currentDollar);


    // Inicializujeme sledovač úrovně zanoření závorek
    int bracketDepth = 0;

    // Tvorba a inicializace nového precedenčního zásobníku počátečním symbolem
    PrecStackList_push();

    // Namapujeme vstupní terminál na "dollar" terminál, pokud se o něj jedná
    PrecTerminals inTerminal = T_PREC_UNDEFINED;
    PrecParser_mapInTerminalToDollar(bracketDepth, currentDollar, &inTerminal);

    // Získání vrcholového terminálu na precedenčním zásobníku
    PrecTerminals topTerminal = T_PREC_UNDEFINED;
    PrecStack_getTopPrecTerminal(&topTerminal);

    // Hlavní cyklus precedenční analýzy
    while(true) {
        // Pokud došlo v rámci funkce "getTopPrecTerminal" k chybě, končíme smyčku
        if(Parser_errorWatcher(IS_PARSING_ERROR)) {
            goto precedenceParser_handleError;  // skok na řešení chyb na konci funkce
        }

        // Aktualizace úrovně zanoření na zákaldě vstupního terminálu
        // Pokud na vrcholu není ID a přišla levá závorka, vstupujeme do nové úrovně
        if (inTerminal == T_PREC_LEFT_BRACKET && !PrecStack_isIdOnTop()) {
            bracketDepth++;
        }

        // Získání precedence pro vstupní terminál a terminál na vrcholu zásobníku
        Precedence precedence = P_PRECEDENCE_UNDEFINED;
        PrecTable_findPrecedence(topTerminal, inTerminal, &precedence);

        // Pokud došlo v rámci funkce "findPrecedence" k chybě, končíme smyčku
        if(Parser_errorWatcher(IS_PARSING_ERROR)) {
            goto precedenceParser_handleError;  // skok na řešení chyb na konci funkce
        }

        /* Pokud došlo při aplikace precedence 'P_GREATER' (tj. ">") ke snížení
         * hodnoty zanoření na 0 a aktuálním "dollar terminálem" je pravá závorka,
         * značí to ukončení hlavní smyčky parsování
         */
        bool greaterStopFlag = false;

        // Zpracování vstupního terminálu na základě získané precedence
        switch(precedence) {
            /* Rovnost: 1) Vlož 'b' na vrchol zásobník
             *          2) Čti další symbol 'b'ze vstupu
             */
            case P_EQUAL:
                // Provedeme algoritmus pro rovnost precedencí "="
                PrecParser_applyEqualPrecedence(bracketDepth, currentDollar, &inTerminal, &topTerminal);
                break;

            /* Shift: 1) Změň na zásobníku 'a' na 'a<'
             *        2) Vlož 'b' na vrchol zásobníku
             *        3) Čti další symbol 'b' ze vstupu
             */
            case P_LESS:
                // Provedeme algoritmus pro precedenci menší než "<"
                PrecParser_applyLessPrecedence(bracketDepth, currentDollar, &inTerminal, &topTerminal);
                break;

            /* Redukce: 1) Je-li '<y' na vrcholu zásobníku &&
             *             Existuje-li pravidlo 'r: A -> y∈P'
             *          a) ANO: Zaměň '<y' za 'A' &&
             *                  Vypiš 'r' na výstup
             *          b) NE: chyba
             */
            case P_GREATER:
                // Provedeme algoritmus pro precedenci větší než ">"
                greaterStopFlag = PrecParser_applyGreaterPrecedence(&bracketDepth, \
                                  fromNonTerminal, currentDollar, &inTerminal, &topTerminal);
                break;

            // Jinak se nic nestane
            default:
                break;
        } // switch()

        // Pokud je splněná (částečně kontextově daná) podmínka, ukončujeme cyklus
        if (PrecParser_shouldStopParsingLoop(inTerminal, topTerminal, greaterStopFlag))
        {
            break;
        }
    } // while()

    // Zaktualizujeme vrcholový terminál
    PrecStack_getTopPrecTerminal(&topTerminal);

    // Aplikujeme zbývající redukce
    if (!PrecParser_applyRemainingReductions(&topTerminal)) {
        goto precedenceParser_handleError;  // skok na řešení chyb na konci funkce
    }

    // Po úspěšném parsování by měl na zásobníku zůstat jediný prvek s výsledným výrazem
    AST_ExprNode *result = NULL;
    PrecStack_getResult(&result);

    // Pokud došlo při získání výsledku k chybě, zničíme seznam zásobníků a končíme precedenční SA
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        goto precedenceParser_handleError;  // skok na řešení chyb na konci funkce
    }

    // Popneme aktuální zásobník ze seznamu zásobníků
    PrecStackList_pop();

    // Vracíme výsledný `AST_ExprNode *`
    return result;


    /***                                                                    ***/
    /*    ZPRACOVÁNÍ CHYB A UVOLNĚNÍ ZDROJŮ ALOKOVANÝCH PŘI PRECEDENČNÍ SA    */
    /***                                                                    ***/
    precedenceParser_handleError:
        // Uvolnění hodnotu currentTerminal
        Parser_freeCurrentTerminalValue();

        // Uvolníme všechny zásobníky včetně obsažených AST uzlů
        PrecStackList_purge();

        // Návrat s chybovým kódem
        return PARSING_ERROR;
} // PrecParser_parse()


/*******************************************************************************
 *                                                                             *
 *       IMPLEMENTACE INTERNÍCH FUNKCÍ PRO ALGORITMY APLIKACÍ PRECEDENCÍ       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Zpracuje terminál s precedencí P_EQUAL.
 *
 * @details Tato funkce pushne inicializovaný uzel na zásobník a čte další
 *          symbol ze vstupu.
 *
 * @param inTerminal Typ aktuálního terminálu.
 * @param bracketDepth Hloubka zanoření závorek.
 * @param topTerminal Ukazatel na proměnnou, do které bude uložen terminál na
 *                    vrcholu zásobníku.
 */
void PrecParser_applyEqualPrecedence(int bracketDepth, DollarTerminals dollarContext, \
                                     PrecTerminals *inTerminal, PrecTerminals *topTerminal)
{
    // Pushneme na zásboník vstupním terminálem inicializovaný "PrecStackNode" včetně AST uzlu
    PrecStack_pushBothStackAndASTNode(*inTerminal);

    // Žádáme scanner o další token
    Parser_getNextToken(POKE_SCANNER);

    // Mapování dalšího symbolu na precedenční terminál - chyby jsou ošetřeny při návratu
    PrecParser_mapInTerminalToDollar(bracketDepth, dollarContext, inTerminal);
    PrecStack_getTopPrecTerminal(topTerminal);
} // PrecParser_applyEqualPrecedence()

/**
 * @brief Zpracuje terminál s precedencí P_LESS.
 *
 * @details Tato funkce pushne handle za první terminál na zásobníku a čte
 *          další symbol ze vstupu.
 *
 * @param inTerminal Typ aktuálního terminálu.
 * @param bracketDepth Hloubka zanoření závorek.
 * @param topTerminal Ukazatel na proměnnou, do které bude uložen terminál
 *                    na vrcholu zásobníku.
 */
void PrecParser_applyLessPrecedence(int bracketDepth, DollarTerminals dollarContext, \
                                    PrecTerminals *inTerminal, PrecTerminals *topTerminal)
{
    /* KONTEXT: Zpracovávání argumentů funkce */
    // Pokud je na vrcholu zásobníku identifikátor a vstpním symbolem je "("
    if (PrecStack_isIdOnTop() && (*inTerminal == T_PREC_LEFT_BRACKET)) {
        // Žádáme scanner o další token
        Parser_getNextToken(POKE_SCANNER);

        // Dočasně předáváme řízení LL parseru a parsujeme neterminál <ARGUMNETS>
        AST_ArgOrParamNode *argList = LLparser_parseArguments();

        // Získali jsme kombinovanou rekurzi LL a precedenčního parseru seznam
        // argumentů funkce, který pushneme na precedenční zásobník
        PrecStack_pushPrecNonTerminal(PREC_STACK_NT_ARG_LIST, AST_ARG_OR_PARAM_NODE, argList);

        // Žádáme scanner o další token
        Parser_getNextToken(POKE_SCANNER);

        // Mapujeme nový terminál na vrcholu zásobníku, jelikož byl změněn
        PrecStack_getTopPrecTerminal(topTerminal);
    }
    /* KONTEXT: Běžná aplikace algoritmu pro precedenční analýzu při "<" precedenci */
    else {
        // Pushneme na zásobník symbol "handle"
        PrecStack_pushHandleAfterFirstTerminal();

        // Pushneme na zásboník vstupbím symbolem inicializovaný "PrecStackNode"
        PrecStack_pushBothStackAndASTNode(*inTerminal);

        // Žádáme scanner o další token
        Parser_getNextToken(POKE_SCANNER);
    }

    // Mapování dalšího symbolu na precedenční terminál - chyby jsou ošetřeny při návratu
    PrecParser_mapInTerminalToDollar(bracketDepth, dollarContext, inTerminal);
    PrecStack_getTopPrecTerminal(topTerminal);
} // PrecParser_applyLessPrecedence()

/**
 * @brief Zpracuje terminál s precedencí P_GREATER.
 */
bool PrecParser_applyGreaterPrecedence(int *bracketDepth, LLNonTerminals fromNonTerminal, \
                                       DollarTerminals dollarContext, PrecTerminals *inTerminal, \
                                       PrecTerminals *topTerminal)
{
    // Vybereme redukční pravidlo
    ReductionRule rule = REDUCE_RULE_UNDEFINED;
    PrecParser_chooseReductionRule(&rule);

    // Pokud došlo při výběru pravidla k chybě, redukci neprovádíme
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        return false;
    }

    // Aplikujeme redukční pravidlo
    PrecParser_reduce(rule);

    // Pokud na vrcholu není NEterminál a přišla pravá závorka, vystupujeme z úrovně
    if (rule == REDUCE_E_INTO_BRACKETS) {
        (*bracketDepth)--;

        /* Navíc, pokud je hloubka zanoření nula a pravá závorka je kontextovým
         * "dollar" terminálem, ukončujeme cyklus precednčeního parsování
         */
        if (*bracketDepth == 0 && dollarContext ==  DOLLAR_T_RIGHT_BRACKET) {
            if(fromNonTerminal != NT_IF && fromNonTerminal != NT_WHILE) {
                return true;
            }
        }
    }

    // Zjistíme aktuální vrcholový terminál
    PrecStack_getTopPrecTerminal(topTerminal);

    // Pokud je vrcholovým terminálem "dollar" a na vstupu pravá závorka,
    // aktualizujeme také vstupní terminál
    if (*topTerminal == T_PREC_DOLLAR && *inTerminal == T_PREC_RIGHT_BRACKET) {
        PrecParser_mapInTerminalToDollar(*bracketDepth, dollarContext, inTerminal);
    }

    return false;
} // PrecParser_applyGreaterPrecedence()


/*******************************************************************************
 *                                                                             *
 *           IMPLEMENTACE INTERNÍCH FUNKCÍ VOLBU REDUKČNÍHO PRAVIDLA           *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vybere redukční pravidlo na základě aktuálního stavu zásobníku.
 */
void PrecParser_chooseReductionRule(ReductionRule *rule) {
    // Pokud byl funkci předán neplatný ukazatel, zaznamenáme vnitřní chybu
    if(rule == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        *rule = REDUCE_RULE_UNDEFINED;
        return;
    }

    // Získání vrcholu zásobníku
    PrecStackNode *stackNode = PrecStack_top();

    // Pokud je zásobník prázdný, hlásíme interní chybu
    if(stackNode == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        *rule = REDUCE_RULE_UNDEFINED;
        return;
    }

    // Vytvoříme strukturu pro zaznamenání sekvence zásobníkových symbolů
    PrecStackSymbol symbolsToReduce[MAX_SYMBOLS_TO_REDUCE];

    // Pole inicializujeme do stavu, že obsahuje nedefinované zásobníkové symboly
    for(unsigned char i = 0; i < MAX_SYMBOLS_TO_REDUCE; i++) {
        symbolsToReduce[i] = PREC_STACK_SYM_UNEDFINED;
    }

    // Inicializujeme počítadlo zaznamenaných symbolů (aby nedošlo k out-of-bounds)
    unsigned char distanceFromTop = 0;

    // Procházíme zásobník, dokud nenarazíme na symbol HANDLE nebo DOLLAR
    // nebo dokud neprojdeme maximální počet terminálů určených k redukci
    while(PrecParser_shouldEndRuleSelecetion(distanceFromTop, stackNode)) {
        // Přidání aktuálního symbolu do redukční sekvence
        symbolsToReduce[distanceFromTop] = stackNode->symbol;

        // Pokud jsme právě přečetli handle,ukončíme smyčku
        if(symbolsToReduce[distanceFromTop] == PREC_STACK_SYM_HANDLE) {
            break;
        }

        // Inkrementujeme počet zpracovaných symbolů
        distanceFromTop++;

        // Posun na další zásobníkový uzel (symbol)
        stackNode = stackNode->next;
    } // while()

    // Vyhledáme pravidlo v tabulce redukčních pravidel
    PrecParser_findReductionRule(symbolsToReduce, rule);

    // Pokud nebylo nalezeno konkrétní pravidlo, nstavíme syntaktickou chybu
    if(*rule == REDUCE_RULE_UNDEFINED) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
    }
} // PrecParser_chooseReductionRule()

void PrecParser_findReductionRule(PrecStackSymbol symbolsToReduce[], ReductionRule *rule) {
    // Ověření platnosti předaného ukazatele - interní chyba
    if(rule == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    /*
     * Tabulka redukčních pravidel:
     * Tato globální proměnná obsahuje pole struktur typu `ReductionRuleSet`,
     * kde každá struktura uchovává jedno redukční pravidlo a odpovídající sekvenci
     * symbolů, které jsou určeny k redukci. Pole obsahuje všechny definované
     * redukční pravidla používané při syntaktické analýze.
     */
    const ReductionRuleSet reductionRuleSet[NUM_OF_REDUCTION_RULES] = {
        { REDUCE_E_ID,                      {  PREC_STACK_SYM_ID,              PREC_STACK_SYM_HANDLE,                 PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_INT_LITERAL,             {  PREC_STACK_SYM_INT_LITERAL,     PREC_STACK_SYM_HANDLE,                 PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_FLOAT_LITERAL,           {  PREC_STACK_SYM_FLOAT_LITERAL,   PREC_STACK_SYM_HANDLE,                 PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_STRING_LITERAL,          {  PREC_STACK_SYM_STRING_LITERAL,  PREC_STACK_SYM_HANDLE,                 PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_NULL_LITERAL,            {  PREC_STACK_SYM_NULL_LITERAL,    PREC_STACK_SYM_HANDLE,                 PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_PLUS_E,                  {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_PLUS,                   PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_MINUS_E,                 {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_MINUS,                  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_MULT_E,                  {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_MULTIPLICATION,         PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_DIV_E,                   {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_DIVISION,               PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_IDENTITY_E,              {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_IDENTITY,               PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_NOT_EQUAL_E,             {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_NOT_EQUAL,              PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_LESS_THAN_E,             {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_LESS_THAN,              PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_GREATER_THAN_E,          {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_GREATER_THAN,           PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_LESS_EQUAL_E,            {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_LESS_THAN_OR_EQUAL,     PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_GREATER_EQUAL_E,         {  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_GREATER_THAN_OR_EQUAL,  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_INTO_BRACKETS,           {  PREC_STACK_SYM_RIGHT_BRACKET,   PREC_STACK_SYM_EXPRESSION,             PREC_STACK_SYM_LEFT_BRACKET,    PREC_STACK_SYM_HANDLE,         PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_FUN_CALL,                {  PREC_STACK_SYM_ARG_LIST,        PREC_STACK_SYM_ID,                     PREC_STACK_SYM_HANDLE,          PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED  } },
        { REDUCE_E_IFJ_CALL,                {  PREC_STACK_SYM_ARG_LIST,        PREC_STACK_SYM_ID,                     PREC_STACK_SYM_DOT,             PREC_STACK_SYM_IFJ,            PREC_STACK_SYM_HANDLE     } },
    }; // ReductionRuleSet reductionRuleSet[];

    // Sekvenční vyhledávání v seznamu redukčních pravidel
    for (int i = 0; i < NUM_OF_REDUCTION_RULES; i++) {
        // Porovnání získané sekvence symbolů s příslušnými sekvencemi pro pravidla
        int cmp = memcmp(symbolsToReduce, reductionRuleSet[i].symbolArr, MAX_SYMBOLS_TO_REDUCE * sizeof(PrecStackSymbol));

        // Pokud najdeme stejnou sekvenci symbolů, vrátíme odpovídající pravidlo
        if (cmp == 0) {
            *rule = reductionRuleSet[i].key;
            return;
        }
    }

    // Pokud nebylo pravidlo nalezeno, vrátíme nedefinované pravidlo
    *rule = REDUCE_RULE_UNDEFINED;
} // PrecTable_findPrecedence()

/**
 * @brief Zavolá specializovanou redukční funkci an základě zvoleného pravidla.
 *
 * @param rule Redukční pravidlo, které se má aplikovat.
 */
void PrecParser_reduce(ReductionRule rule) {
    // Výběr specializované redukční funkce podle zvoleného pravidla
    switch(rule) {
        // Redukce: E -> id
        case REDUCE_E_ID:
            PrecParser_reduceVarOrLit(AST_VAR_NODE);
            break;

        // Redukce: E -> literal (tj. i32, f64, []u8, null)
        case REDUCE_E_INT_LITERAL:
        case REDUCE_E_FLOAT_LITERAL:
        case REDUCE_E_STRING_LITERAL:
        case REDUCE_E_NULL_LITERAL:
            PrecParser_reduceVarOrLit(AST_LITERAL_NODE);
            break;

        // Redukce: E -> E + E
        case REDUCE_E_PLUS_E:
            PrecParser_reduceBinOp(AST_OP_ADD);
            break;

        // Redukce: E -> E - E
        case REDUCE_E_MINUS_E:
            PrecParser_reduceBinOp(AST_OP_SUBTRACT);
            break;

        // Redukce: E -> E * E
        case REDUCE_E_MULT_E:
            PrecParser_reduceBinOp(AST_OP_MULTIPLY);
            break;

        // Redukce: E -> E / E
        case REDUCE_E_DIV_E:
            PrecParser_reduceBinOp(AST_OP_DIVIDE);
            break;

        // Redukce: E -> E == E
        case REDUCE_E_IDENTITY_E:
            PrecParser_reduceBinOp(AST_OP_EQUAL);
            break;

        // Redukce: E -> E != E
        case REDUCE_E_NOT_EQUAL_E:
            PrecParser_reduceBinOp(AST_OP_NOT_EQUAL);
            break;

        // Redukce: E -> E < E
        case REDUCE_E_LESS_THAN_E:
            PrecParser_reduceBinOp(AST_OP_LESS_THAN);
            break;

        // Redukce: E -> E > E
        case REDUCE_E_GREATER_THAN_E:
            PrecParser_reduceBinOp(AST_OP_GREATER_THAN);
            break;

        // Redukce: E -> E <= E
        case REDUCE_E_LESS_EQUAL_E:
            PrecParser_reduceBinOp(AST_OP_LESS_EQUAL);
            break;

        // Redukce: E -> E >= E
        case REDUCE_E_GREATER_EQUAL_E:
            PrecParser_reduceBinOp(AST_OP_GREATER_EQUAL);
            break;

        // Redukce: E -> ( E )
        case REDUCE_E_INTO_BRACKETS:
            PrecParser_reduceIntoBrackets();
            break;

        // Redukce: E -> id <ARG_LIST>
        case REDUCE_E_FUN_CALL:
            PrecParser_reduceFunCall(IS_USER_FUNCTION);
            break;

        // Redukce: E -> ifj . id <ARG_LIST>
        case REDUCE_E_IFJ_CALL:
            PrecParser_reduceFunCall(IS_BUILT_IN_FUNCTION);
            break;

        // Pokud jsme dostali neznámé pravidlo, jde o interní chybu
        default:
            Parser_errorWatcher(SET_ERROR_INTERNAL);
            break;
    } // switch()
} // PrecParser_reduce()

/**
 * @brief Aplikuje zbývající redukce, dokud nebude vrcholovým terminálem "dollar".
 */
bool PrecParser_applyRemainingReductions(PrecTerminals *topTerminal) {
    // Aplikujeme redukce, dokud vrcholovým terminálem nebude "dollar"
    while(*topTerminal != T_PREC_DOLLAR) {
        // Vybereme redukční pravidlo
        ReductionRule rule = REDUCE_RULE_UNDEFINED;
        PrecParser_chooseReductionRule(&rule);

        // Pokud došlo při výběru pravidla k chybě, končíme cyklus
        if (Parser_errorWatcher(IS_PARSING_ERROR)) {
            return false;
        }

        // Provedeme redukci na základě zvoleného pravidla
        PrecParser_reduce(rule);

        // Pokud došlo při redukci k chybě, končíme cyklus
        if (Parser_errorWatcher(IS_PARSING_ERROR)) {
            return false;
        }

        // Namapujeme nový vrcholový terminál
        PrecStack_getTopPrecTerminal(topTerminal);

        // Pokud došlo při mapování k chybě, končíme cyklus
        if (Parser_errorWatcher(IS_PARSING_ERROR)) {
            return false;
        }
    }

    // Všechny redukce proběhly úspešně
    return true;
}


/*******************************************************************************
 *                                                                             *
 *       IMPLEMENTACE INTERNÍCH FUNKCÍ PRO APLIKACI REDUKČNÍCH PRAVIDEL        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Redukce podle pravidla E -> id nebo E -> literal.
 */
void PrecParser_reduceVarOrLit(AST_NodeType nodeType) {
    // Popnutí uzlu s proměnnou/literálem ze zásobníku
    PrecStackNode *stackNode = PrecStack_pop();

    // Popnutí a uvolnění handle
    PrecStackNode *handleNode = PrecStack_pop();
    free(handleNode);

    // Do pomocné proměnné dereferncujeme uzel pro proměnnou/literál
    AST_VarNode *variable = (AST_VarNode *)stackNode->node;

    if(variable == NULL) {
        PrecStack_freeNode(stackNode);  // uvolníme zásobníkový a AST uzel pro id/literál
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Vytvoření AST uzlu pro výraz
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    // Pokud došlo při tvorbě uzlu k chybě, ukončíme funkci
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        PrecStack_freeNode(stackNode);  // uvolníme zásobníkový a AST uzel pro id/literál
        return;
    }

    // AST_ExprNode propojíme s příslušným AST_VarNode
    if(nodeType == AST_VAR_NODE) {
        // Zkontrolujeme, že se nesnažíme použít proměnnou mimo její rozsah platnosti
        SymtableItem *foundItem = NULL;
        frame_stack_result res = frameStack_findItem(variable->identifier, &foundItem);

        if(res != FRAME_STACK_SUCCESS) {
            if(res == FRAME_STACK_ITEM_DOESNT_EXIST) {
                Parser_errorWatcher(SET_ERROR_SEM_UNDEF);
            }
            else {
                Parser_errorWatcher(SET_ERROR_INTERNAL);
            }

            PrecStack_freeNode(stackNode);              // Uvolníme zásobníkový uzel včetně AST uzlu
            AST_destroyNode(AST_EXPR_NODE, exprNode);   // Uvolníme vytvořený AST uzel pro výraz
            return;
        }

        // Pokud proměnná nalezena byla, označíme ji jako použitou
        foundItem->used = true;

        // Nastavíme správné ID proměnné
        variable->frameID = frameStack_getId(variable->identifier);

        // Nyní můžeme inicializovat uzel pro výraz obsahující proměnnou
        AST_initNewExprNode(exprNode, AST_EXPR_VARIABLE, stackNode->node);
    }
    // Jinak incializujeme uzel pro výraz obsahující literál
    else if(nodeType == AST_LITERAL_NODE) {
        AST_initNewExprNode(exprNode, AST_EXPR_LITERAL, stackNode->node);
    }
    else {
        PrecStack_freeNode(stackNode);
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    } // if-ifelse-else()

    // Uvolnění popnutého uzlu, protože jeho AST uzel byl již použit
    free(stackNode);

    // Pushnutí nového uzlu pro neterminál "E" na zásobník s AST uzlem pro výraz
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, AST_EXPR_NODE, exprNode);
} // PrecParser_reduceVarOrLit()

/**
 * @brief Redukce pro binární operaci E -> E op E.
 */
void PrecParser_reduceBinOp(AST_BinOpType binOp) {
    // Popnutí pravého operandu E
    PrecStackNode *rightNode = PrecStack_pop();

    // Popnutí a uvolnění operátoru
    PrecStackNode *opNode = PrecStack_pop();
    free(opNode);

    // Popnutí levého operandu 'E'
    PrecStackNode *leftNode = PrecStack_pop();

    // Popnutí a uvolnění handle
    PrecStackNode *handleNode = PrecStack_pop();
    free(handleNode);

    // Vytvoření a inicializace AST uzlu pro binární operaci
    AST_BinOpNode *binOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);

    // Pokud došlo při tvorbě uzlu k chybě, ukončíme funkci
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        PrecStack_freeNode(leftNode);   // uvolníme zásobníkový a AST uzel pro levý operand
        PrecStack_freeNode(rightNode);  // uvolníme zásobníkový a AST uzel pro pravý operand
        return;
    }

    // Inicializujeme nový uzel pro binární operaci levým a pravým operandem
    AST_initNewBinOpNode(binOpNode, binOp, leftNode->node, rightNode->node);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(leftNode);
    free(rightNode);

    // Vytvoření a inicializace AST uzlu pro výraz s binární operací
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    // Pokud došlo při tvorbě uzlu k chybě, ukončíme funkci
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_BIN_OP_NODE, binOpNode);    // uvolníme uzel pro binární operaci
        return;
    }

    // Inicializujeme nový uzel pro výraz uzlem pro binární operac
    AST_initNewExprNode(exprNode, AST_EXPR_BINARY_OP, binOpNode);

    // Pushnutí nového neterminálu E na zásobník s AST uzlem
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, AST_EXPR_NODE, exprNode);
} // PrecParser_reduceBinOp()

/**
 * @brief Redukce pro výraz v závorkách E -> ( E ).
 */
void PrecParser_reduceIntoBrackets() {
    // Popnutí a uvolnění ')'
    PrecStackNode *rightBracket = PrecStack_pop();
    free(rightBracket);

    // Popnutí 'E'
    PrecStackNode *innerNode = PrecStack_pop();

    // Popnutí a uvolnění '('
    PrecStackNode *leftBracket = PrecStack_pop();
    free(leftBracket);

    // Popnutí a uvolnění handle
    PrecStackNode *handleNode = PrecStack_pop();
    free(handleNode);

    // Pushnutí E zpět na zásobník bez změny (pouze jsme odstranili závorky okolo)
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, innerNode->nodeType, innerNode->node);

    // Uvolníme původní StackNode pro výraz E (funkce výše vytvořila nový)
    free(innerNode);
} // PrecParser_reduceIntoBrackets()

/**
 * @brief Redukce pro volání funkce E -> id <ARG_LIST>  NEBO  E -> ifj . id <ARG_LIST>,
 */
void PrecParser_reduceFunCall(bool isBuiltIn) {
    // Popnutí '<ARG_LIST>'
    PrecStackNode *argumentsNode = PrecStack_pop();

    // Popnutí 'id'
    PrecStackNode *varNode = PrecStack_pop();

    // Pokud se jedná o vestavěnou funkci, popneme dva zásobníkové uzly navíc
    if(isBuiltIn) {
        // Popnutí a uvolnění '.'
        PrecStackNode *dot = PrecStack_pop();
        free(dot);

        // Popnutí a uvolnění 'ifj'
        PrecStackNode *ifj = PrecStack_pop();
        free(ifj);
    }

    // Popnutí a uvolnění handle
    PrecStackNode *handleNode = PrecStack_pop();
    free(handleNode);

    // Vytvoření AST uzlu pro volání funkce
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);

    // Pokud došlo při tvorbě uzlu k chybě, ukončíme funkci
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        PrecStack_freeNode(argumentsNode);  // Uvolníme zaábníkový uzel se seznamem argumentů
        PrecStack_freeNode(varNode);        // Uvolníme zásobníkový uzel s AST uzle pro id
        return;
    }

    // Do pomocné proměnné dereferencujeme AST uzel pro identifikátor
    AST_VarNode *idNode = (AST_VarNode *)varNode->node;

    // Inicializujeme nový uzel pro volání funkce
    AST_initNewFunCallNode(funCallNode, idNode->identifier, isBuiltIn, argumentsNode->node);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(idNode);
    free(varNode);
    free(argumentsNode);

    // Vytvoření AST uzlu pro výraz
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    // Pokud došlo při tvorbě uzlu k chybě, ukončíme funkci
    if(Parser_errorWatcher(IS_PARSING_ERROR)) {
        AST_destroyNode(AST_FUN_CALL_NODE, funCallNode); // uvolníme vytvořný uzel pro volání funkce
        return;
    }

    // Inicializujeme nový uzel pro výraz voláním funkce
    AST_initNewExprNode(exprNode, AST_EXPR_FUN_CALL, funCallNode);

    // Pushnutí nového neterminálu E na zásobník s AST uzlem
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, AST_EXPR_NODE, exprNode);
} // PrecParser_reduceFunCall()

/*******************************************************************************
 *                                                                             *
 *              IMPLEMENTACE OSTATNÍCH INTERNÍCH POMOCNÝCH FUNKCÍ              *
 *                                                                             *
 ******************************************************************************/

inline bool PrecParser_parsingEmptyExpression(LLNonTerminals fromNonTerminal) {
    return (fromNonTerminal == NT_ARGUMENTS && currentTerminal.LLterminal == T_RIGHT_BRACKET) || \
           (fromNonTerminal == NT_STATEMENT && currentTerminal.LLterminal == T_SEMICOLON);
} // PrecParser_parsingEmptyExpression()

inline bool PrecParser_shouldEndRuleSelecetion(unsigned char distanceFromTop, \
                                               PrecStackNode *stackNode)
{
    return (stackNode->symbol != PREC_STACK_SYM_DOLLAR && \
            distanceFromTop < MAX_SYMBOLS_TO_REDUCE && \
            stackNode != NULL);
} // PrecParser_shouldEndRuleSelecetion()

/**
 * @brief Zkontroluje podmínky pro ukončení hlavní smyčky precedenční syntaktické analýzy.
 */
inline bool PrecParser_shouldStopParsingLoop(PrecTerminals inTerminal, \
                                             PrecTerminals topTerminal, \
                                             bool greaterStopFlag)
{
    // Pokud došlo k chybě během parsování, ukončujeme smyčku
    if (Parser_errorWatcher(IS_PARSING_ERROR)) {
        return true;
    }

    // Kontrola flagu pro speciální ukončení po aplikaci 'P_GREATER'
    if (greaterStopFlag) {
        return true;
    }

    // Kontrola ukončení: pokud 'inTerminal' i 'topTerminal' jsou rovny '$'
    if (inTerminal == T_PREC_DOLLAR && topTerminal == T_PREC_DOLLAR) {
        return true;
    }

    // Pokud žádná z podmínek není splněna, smyčka pokračuje
    return false;
} // PrecParser_shouldStopParsingLoop

/**
 * @brief Mapuje neterminál na redukční pravidlo.
 */
void PrecParser_mapNonTerminalToRule(PrecStackSymbol symbol, ReductionRule *rule) {
    // Ověření platnosti předaného ukazatele
    if (rule == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    switch (symbol) {
        // Mapování: PREC_STACK_SYM_ID -> REDUCE_E_ID
        case PREC_STACK_SYM_ID:
            *rule = REDUCE_E_ID;
            break;

        // Mapování: PREC_STACK_SYM_INT_LITERAL -> REDUCE_E_INT_LITERAL
        case PREC_STACK_SYM_INT_LITERAL:
            *rule = REDUCE_E_INT_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_FLOAT_LITERAL -> REDUCE_E_FLOAT_LITERAL
        case PREC_STACK_SYM_FLOAT_LITERAL:
            *rule = REDUCE_E_FLOAT_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_STRING_LITERAL -> REDUCE_E_STRING_LITERAL
        case PREC_STACK_SYM_STRING_LITERAL:
            *rule = REDUCE_E_STRING_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_NULL_LITERAL -> REDUCE_E_NULL_LITERAL
        case PREC_STACK_SYM_NULL_LITERAL:
            *rule = REDUCE_E_NULL_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_PLUS -> REDUCE_E_PLUS_E
        case PREC_STACK_SYM_PLUS:
            *rule = REDUCE_E_PLUS_E;
            break;

        // Mapování: PREC_STACK_SYM_MINUS -> REDUCE_E_MINUS_E
        case PREC_STACK_SYM_MINUS:
            *rule = REDUCE_E_MINUS_E;
            break;

        // Mapování: PREC_STACK_SYM_MULTIPLICATION -> REDUCE_E_MULT_E
        case PREC_STACK_SYM_MULTIPLICATION:
            *rule = REDUCE_E_MULT_E;
            break;

        // Mapování: PREC_STACK_SYM_DIVISION -> REDUCE_E_DIV_E
        case PREC_STACK_SYM_DIVISION:
            *rule = REDUCE_E_DIV_E;
            break;

        // Mapování: PREC_STACK_SYM_IDENTITY -> REDUCE_E_IDENTITY_E
        case PREC_STACK_SYM_IDENTITY:
            *rule = REDUCE_E_IDENTITY_E;
            break;

        // Mapování: PREC_STACK_SYM_NOT_EQUAL -> REDUCE_E_NOT_EQUAL_E
        case PREC_STACK_SYM_NOT_EQUAL:
            *rule = REDUCE_E_NOT_EQUAL_E;
            break;

        // Mapování: PREC_STACK_SYM_LESS_THAN -> REDUCE_E_LESS_THAN_E
        case PREC_STACK_SYM_LESS_THAN:
            *rule = REDUCE_E_LESS_THAN_E;
            break;

        // Mapování: PREC_STACK_SYM_GREATER_THAN -> REDUCE_E_GREATER_THAN_E
        case PREC_STACK_SYM_GREATER_THAN:
            *rule = REDUCE_E_GREATER_THAN_E;
            break;

        // Mapování: PREC_STACK_SYM_LESS_THAN_OR_EQUAL -> REDUCE_E_LESS_EQUAL_E
        case PREC_STACK_SYM_LESS_THAN_OR_EQUAL:
            *rule = REDUCE_E_LESS_EQUAL_E;
            break;

        // Mapování: PREC_STACK_SYM_GREATER_THAN_OR_EQUAL -> REDUCE_E_GREATER_EQUAL_E
        case PREC_STACK_SYM_GREATER_THAN_OR_EQUAL:
            *rule = REDUCE_E_GREATER_EQUAL_E;
            break;

        // Mapování: PREC_STACK_SYM_LEFT_BRACKET -> REDUCE_E_INTO_BRACKETS
        case PREC_STACK_SYM_LEFT_BRACKET:
            *rule = REDUCE_E_INTO_BRACKETS;
            break;

        // Mapování: PREC_STACK_SYM_DOT -> REDUCE_E_IFJ_CALL
        case PREC_STACK_SYM_DOT:
            *rule = REDUCE_E_IFJ_CALL;
            break;

        // Defaultní případ: interní chyba
        default:
            Parser_errorWatcher(SET_ERROR_INTERNAL);
            break;
    } // switch()
} // PrecParser_mapNonTerminalToRule()

/*** Konec souboru precedence_parser.c ***/
