/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_parser.c                                       *
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
 * @file precedence_parser.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace precedenční syntaktické analýzy.
 * @details Implementuje funkce a struktury pro precedenční syntaktickou
 *          analýzu výrazů a operátorů. Obsahuje logiku pro posun a redukci
 *          ve výrazech a pro konstrukci AST.
 */

#include "precedence_parser.h"


/*******************************************************************************
 *                                                                             *
 *                 GLOBÁLNÍ PROMĚNNÉ PRO PRECEDENČNÍ PARSOVÁNÍ                 *
 *                                                                             *
 ******************************************************************************/

/*
 * Tato globální proměnná obsahuje pole struktur typu `ReductionRuleSet`,
 * kde každá struktura uchovává jedno redukční pravidlo a odpovídající sekvenci
 * symbolů, které jsou určeny k redukci. Pole obsahuje všechny definované
 * redukční pravidla používané při syntaktické analýze.
 */
const struct ReductionRuleSet reductionRuleSet[NUM_OF_REDUCTION_RULES] = {
    { REDUCE_E_ID,                      { PREC_STACK_SYM_ID,              PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_INT_LITERAL,             { PREC_STACK_SYM_INT_LITERAL,     PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_FLOAT_LITERAL,           { PREC_STACK_SYM_FLOAT_LITERAL,   PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_STRING_LITERAL,          { PREC_STACK_SYM_STRING_LITERAL,  PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_NULL_LITERAL,            { PREC_STACK_SYM_NULL_LITERAL,    PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_PLUS_E,                  { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_PLUS,                   PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_MINUS_E,                 { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_MINUS,                  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_MULT_E,                  { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_MULTIPLICATION,         PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_DIV_E,                   { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_DIVISION,               PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_IDENTITY_E,              { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_IDENTITY,               PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_NOT_EQUAL_E,             { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_NOT_EQUAL,              PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_LESS_THAN_E,             { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_LESS_THAN,              PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_GREATER_THAN_E,          { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_GREATER_THAN,           PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_LESS_EQUAL_E,            { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_LESS_THAN_OR_EQUAL,     PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_GREATER_EQUAL_E,         { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_GREATER_THAN_OR_EQUAL,  PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_INTO_BRACKETS,           { PREC_STACK_SYM_LEFT_BRACKET,    PREC_STACK_SYM_EXPRESSION,             PREC_STACK_SYM_RIGHT_BRACKET,   PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_FUN_CALL,                { PREC_STACK_SYM_ID,              PREC_STACK_SYM_LEFT_BRACKET,           PREC_STACK_SYM_ARG_LIST,        PREC_STACK_SYM_RIGHT_BRACKET,  PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_E_IFJ_CALL,                { PREC_STACK_SYM_IFJ,             PREC_STACK_SYM_DOT,                    PREC_STACK_SYM_ID,              PREC_STACK_SYM_LEFT_BRACKET,   PREC_STACK_SYM_ARG_LIST,   PREC_STACK_SYM_RIGHT_BRACKET } },
    { REDUCE_ARG_LIST_TO_E_ARG,         { PREC_STACK_SYM_EXPRESSION,      PREC_STACK_SYM_ARG,                    PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_ARG_TO_COMMA_E_ARG,        { PREC_STACK_SYM_COMMA,           PREC_STACK_SYM_EXPRESSION,             PREC_STACK_SYM_ARG,             PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_ARG_TO_COMMA,              { PREC_STACK_SYM_COMMA,           PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
    { REDUCE_TO_EPSILON,                { PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,              PREC_STACK_SYM_UNEDFINED,       PREC_STACK_SYM_UNEDFINED,      PREC_STACK_SYM_UNEDFINED,  PREC_STACK_SYM_UNEDFINED     } },
}; // ReductionRuleSet reductionRuleSet[];


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
void *PrecParser_parse(LLNonTerminals fromNonTerminal) {
    if(fromNonTerminal == NT_ARGUMENTS && currentToken.LLterminal == T_RIGHT_BRACKET) {
        return NULL;
    }

    // Získání množiny ukončovacích "dolar" terminálů pro daný NEterminál
    PrecTable_getFollowSet(fromNonTerminal);

    // Uroveň zanoření je inicializována
    int bracketDepth = 0;

    // Tvorba a inicializace precedenčního zásobníku počátečním symbolem
    PrecStack_create();
    if(precStack == NULL) {
        error_handle(ERROR_INTERNAL);
        return NULL;
    }
    PrecStack_init();

    // Zjištění precedence pro první přednačtený token
    PrecTerminals inTerminal = T_PREC_UNDEFINED;
    PrecParser_mapToDollar(bracketDepth, &inTerminal);

    // Získání termínál, který je na zásobníku nejvýše
    PrecTerminals topTerminal = T_PREC_UNDEFINED;
    PrecStack_getTopPrecTerminal(&topTerminal);

    // Hlavní cyklus precedenční analýzy
    // Kontrola ukončení: pokud 'b' i 'top' jsou rovny '$'
    while(true) {
        // Získání termínál, který je na zásobníku nejvýše
        topTerminal = T_PREC_UNDEFINED;
        PrecStack_getTopPrecTerminal(&topTerminal);

        if(topTerminal == T_PREC_UNDEFINED) {
            PrecStack_dispose();
            error_handle(ERROR_INTERNAL);
            return NULL;
        }

        // Aktualizace úrovně zanoření na zákaldě vstupního terminálu
        if (inTerminal == T_PREC_LEFT_BRACKET) {
            bracketDepth++;
        }
        else if (inTerminal == T_PREC_RIGHT_BRACKET) {
            bracketDepth--;

            // Ukončení parsování, pokud je závorka ve Follow množině a úroveň zanoření je 0
            if (bracketDepth == 0 && currentDollar ==  DOLLAR_T_RIGHT_BRACKET) {
                break;
            }
        }

        // Získání precedence pro aktuální terminál a terminál na vrcholu zásobníku
        Precedence precedence;
        PrecTable_findPrecedence(topTerminal, inTerminal, &precedence);
        if(precedence == P_SYNTAX_ERROR) {
            Parser_watchSyntaxError(SET_SYNTAX_ERROR);
            PrecStack_dispose();
            return NULL;
        }

        // Zpracování na základě precedence
        switch(precedence) {
            /* Rovnost: 1) Vlož 'b' na vrchol zásobník
             *          2) Čti další symbol 'b'ze vstupu
             */
            case P_EQUAL: {
                // Pushneme na zásboník inicializovaný "PrecStackNode"
                PrecStack_pushBothStackAndASTNode(inTerminal);

                // Čtení dalšího symbolu ze vstupu
                Parser_getNextToken(PREC_PARSER);

                // Mapování dalšího symbolu na precedenční terminál
                PrecParser_mapToDollar(bracketDepth, &inTerminal);
                break;
            } // case P_EQUAL

            /* Shift: 1) Změň na zásobníku 'a' na 'a<'
             *        2) Vlož 'b' na vrchol zásobníku
             *        3) Čti další symbol 'b' ze vstupu
             */
            case P_LESS: {
                // Pushneme na zásobník symbol "handle"
                PrecStack_pushPrecNonTerminal(PREC_STACK_NT_HANDLE, SN_WITHOUT_AST_TYPE, SN_WITHOUT_AST_PTR);

                // Pushneme na zásboník inicializovaný "PrecStackNode"
                PrecStack_pushBothStackAndASTNode(inTerminal);

                // Čtení dalšího symbolu ze vstupu
                Parser_getNextToken(PREC_PARSER);

                // Mapování dalšího symbolu na precedenční terminál
                PrecParser_mapToDollar(bracketDepth, &inTerminal);
                break;
            } // case P_LESS

            /* Redukce: 1) Je-li '<y' na vrcholu zásobníku &&
             *             Existuje-li pravidlo 'r: A -> y∈P'
             *          a) ANO: Zaměň '<y' za 'A' &&
             *                  Vypiš 'r' na výstup
             *          b) NE: chyba
             */
            case P_GREATER: {
                // Kontrola, zda je na vrcholu zásobníku "handle"
                if(PrecStack_isHandleOnTop()) {
                    // Vybereme redukční pravidlo
                    ReductionRule rule = REDUCE_RULE_UNDEFINED;
                    precParser_chooseReductionRule(&rule);

                    if(rule == REDUCE_RULE_UNDEFINED) {
                        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                        PrecStack_dispose();
                        return NULL;
                    }

                    // Provedeme redukci na základě zvoleného pravidla
                    if(rule != REDUCE_RULE_WAIT) {
                        PrecParser_reduce(rule);
                    }
                }
                // Pokud na vrcholu zásobníku "handle" není, došlo k syntaktické chybě
                else {
                    Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                    PrecStack_dispose();
                    return NULL;
                }
                break;
            } // case P_GREATER

            // Prázdné políčko v precedenční tabulce => syntaktická chyba
            case P_SYNTAX_ERROR:
            default:
                Parser_watchSyntaxError(SET_SYNTAX_ERROR);
                PrecStack_dispose();
                return NULL;
        } // switch()

        if(inTerminal == T_PREC_DOLLAR && topTerminal == T_PREC_DOLLAR) {
            break;
        }
    } // while()

    // Kontrola, zda je na vrcholu zásobníku "handle"
    while(true) {
        // Vybereme redukční pravidlo
        ReductionRule rule = REDUCE_RULE_UNDEFINED;
        precParser_chooseReductionRule(&rule);

        if(rule == REDUCE_RULE_UNDEFINED) {
            break;
        }

        // Provedeme redukci na základě zvoleného pravidla
        PrecParser_reduce(rule);
    }

    // Po úspěšném parsování by měl na zásobníku zůstat jediný prvek s výsledným výrazem
    AST_ExprNode *result = NULL;
    if(!PrecStack_getResult(&result)) {
        Parser_watchSyntaxError(SET_SYNTAX_ERROR);
        PrecStack_dispose();
        return NULL;
    }

    // Uvolníme zásobník
    PrecStack_dispose();

    // Vracíme výsledný AST_ExprNode
    return result;
} // PrecParser_parse()


/*******************************************************************************
 *                                                                             *
 *                   IMPLEMENTACE HLAVNÍCH INTERNÍCH FUNKCÍ                    *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vybere redukční pravidlo na základě aktuálního stavu zásobníku.
 *
 * @details Tato funkce prochází zásobník, dokud nenarazí na symbol `HANDLE`,
 *          a na základě aktuálního stavu a symbolů na zásobníku určí, které
 *          redukční pravidlo použít. Pokud pro danou sekvenci symbolů pravidlo
 *          neexistuje, dojde k chybě.
 *
 * @param [out] rule Ukazatel na proměnnou, do které bude uloženo nalezené
 *              redukční pravidlo.
 */
void precParser_chooseReductionRule(ReductionRule *rule) {
    // Pokud byl funkci předán neplatný ukazatel, hlásíme vnitřní chybu
    if(rule == NULL) {
        error_handle(ERROR_INTERNAL);
        return;
    }

    // Získání vrcholu zásobníku (pokud je zásobník prázdný, hlásíme interní chybu)
    PrecStackNode* stackNode = PrecStack_top();

    if(stackNode == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme pole pro zaznamenání sekvence terminálů až po nalezení "handle"
    ReduceSymbolArray toReduce;

    for(int i = 0; i < MAX_SYMBOLS_TO_REDUCE; i++) {
        toReduce.symbolArr[i] = PREC_STACK_SYM_UNEDFINED;  // počáteční inicializace
    }
    toReduce.symbolCount = 0;

    int distanceFromTop = 0;    // Vzdálenosti od vrcholu zásobníku
    int bracketDepth = 0;       // Hloubka závorek

    // Procházíme zásobník, dokud nenarazíme na symbol HANDLE nebo DOLLAR
    // nebo dokud neprojdeme maximální počet terminálů určených k redukci
    while ((stackNode->symbolType != STACK_NODE_TYPE_HANDLE &&
            stackNode->symbol != PREC_STACK_SYM_DOLLAR) &&
            distanceFromTop < MAX_SYMBOLS_TO_REDUCE) {
        // Aktualizace úrovně zanoření
        if (stackNode->symbol == PREC_STACK_SYM_LEFT_BRACKET) {
            bracketDepth++;
        } else if (stackNode->symbol == PREC_STACK_SYM_RIGHT_BRACKET) {
            bracketDepth--;
        }

        // Přidání aktuálního symbolu do redukční sekvence
        if (!(stackNode->symbol == PREC_STACK_SYM_COMMA && bracketDepth > 0)) {
            toReduce.symbolArr[distanceFromTop] = stackNode->symbol;
            distanceFromTop++;
        }

        // Posun na další uzel na zásobníku
        stackNode = stackNode->next;

        if(stackNode == NULL) {
            break;
        }
    }

    // Nastavení indexů pro binární vyhledávání
    int left = 0;
    int right = NUM_OF_REDUCTION_RULES - 1;
    int mid;

    // Binární vyhledávání v tabulec redukčních pravidel
    while(left <= right) {
        mid = (left + right) / 2;

        // Porovnání získané sekvence symbolů s příslušnými sekvencemi pro pravidla
        int cmp = memcmp(toReduce.symbolArr, reductionRuleSet[mid].symbolArr, \
                         MAX_SYMBOLS_TO_REDUCE * sizeof(PrecStackSymbol));

        // Pokud najdeme stejnou sekvenci symbolů, vrátíme odpovídající pravidlo
        if(cmp == 0) {
            if(reductionRuleSet[mid].rule == REDUCE_E_ID && currentToken.PrecTerminal == T_PREC_RIGHT_BRACKET) {
                *rule = REDUCE_RULE_WAIT;
            }
            else {
                *rule = reductionRuleSet[mid].rule;
            }
            return;
        }

        // Hledaná sekvence je výše, posouváme levý index doprava
        if(cmp > 0) {
            left = mid + 1;
        }
        // Hledaná sekvence je níže, posouváme pravý index doleva
        else {
            right = mid - 1;
        }
    }

    // Pokud nebylo nalezeno žádné pravidlo, hlásíme interní chybu
    *rule = REDUCE_RULE_UNDEFINED;
} // precParser_chooseReductionRule()

/**
 * @brief Provede redukci na základě daného redukčního pravidla.
 *
 * @details Tato funkce provede redukci na základě daného redukčního pravidla
 *          a aktualizuje syntaktický strom.
 *
 * @param rule Redukční pravidlo, které se má použít.
 */
void PrecParser_reduce(ReductionRule rule) {
    // Provedení redukce na základě vybraného pravidla
    switch(rule) {
        // Redukce: E -> id
        case REDUCE_E_ID:
            PrecParser_reduceVarOrLit(AST_VAR_NODE);
            break;

        // Redukce: E -> literal
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
            PrecParser_reduceBrackets();
            break;

        // Redukce: E -> id ( <ARGUMENTS> )
        case REDUCE_E_FUN_CALL:
            PrecParser_reduceFunCall();
            break;

        // Redukce: E -> ifj . id ( <ARGUMENTS> )
        case REDUCE_E_IFJ_CALL:
            PrecParser_reduceIfjFunCall();
            break;

        // Redukce: <ARG_LIST> -> E <ARG>
        case REDUCE_ARG_LIST_TO_E_ARG:
            PrecParser_reduceArgListExprArg();
            break;

        // Redukce: <ARG> -> , E <ARG>
        case REDUCE_ARG_TO_COMMA_E_ARG:
            PrecParser_reduceArgCommaExprArg();
            break;

        // Redukce: <ARG> -> ,
        case REDUCE_ARG_TO_COMMA:
            PrecParser_reduceArgComma();
            break;

        // Redukce: <ARG_LIST> | <ARG> -> ε
        case REDUCE_TO_EPSILON:
            break;

        // Pokud jsme dostali neznámé pravidlo, jde o interní chybu
        default:
            error_handle(ERROR_INTERNAL);
            break;
    } // switch()
} // PrecParser_reduce()


/*******************************************************************************
 *                                                                             *
 *                      IMPLEMENTACE REDUKČNÍCH PRAVIDEL                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Redukce podle pravidla E -> id nebo E -> literal.
 */
void PrecParser_reduceVarOrLit(AST_NodeType nodeType) {
    // Popnutí uzlu s proměnnou/literálem ze zásobníku
    PrecStackNode *stackNode = PrecStack_pop();

    // Vytvoření AST uzlu pro výraz
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);

    // AST_ExprNode propojíme s příslušným AST_VarNode
    switch(nodeType) {
        // Když jde o uzel proměnné
        case AST_VAR_NODE:
            AST_initNewExprNode(exprNode, AST_EXPR_VARIABLE, stackNode->node);
            break;

        // Když jde o uzel literálu
        case AST_LITERAL_NODE:
            AST_initNewExprNode(exprNode, AST_EXPR_LITERAL, stackNode->node);
            break;

        // Jakýkoliv jiný typ uzlu způsobí vnitřní chybu
        default:
            PrecStack_freeNode(stackNode);
            error_handle(ERROR_INTERNAL);
            break;
    } // switch()

    // Uvolnění popnutého uzlu, protože jeho AST uzel byl již použit
    free(stackNode);

    // Pushnutí nového neterminálu E na zásobník s AST uzlem
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

    // Popnutí levého operandu E
    PrecStackNode *leftNode = PrecStack_pop();

    // Vytvoření a inicializace AST uzlu pro binární operaci
    AST_BinOpNode *binOpNode = (AST_BinOpNode *)AST_createNode(AST_BIN_OP_NODE);
    AST_initNewBinOpNode(binOpNode, binOp, leftNode->node, rightNode->node);

    // Vytvoření a inicializace AST uzlu pro výraz s binární operací
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    AST_initNewExprNode(exprNode, AST_EXPR_BINARY_OP, binOpNode);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(leftNode);
    free(rightNode);

    // Pushnutí nového neterminálu E na zásobník s AST uzlem
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, AST_EXPR_NODE, exprNode);
} // PrecParser_reduceBinOp()

/**
 * @brief Redukce pro výraz v závorkách E -> ( E ).
 */
void PrecParser_reduceBrackets() {
    // Popnutí a uvolnění ')'
    PrecStackNode *rightBracket = PrecStack_pop();
    free(rightBracket);

    // Popnutí E
    PrecStackNode *exprNode = PrecStack_pop();

    // Popnutí a uvolnění '('
    PrecStackNode *leftBracket = PrecStack_pop();
    free(leftBracket);

    // Pushnutí E zpět na zásobník bez změny (výraz v závorkách)
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, exprNode->nodeType, exprNode->node);

    // Uvolníme původní StackNode pro výraz E
    free(exprNode);
} // PrecParser_reduceBrackets()

/**
 * @brief Redukce pro volání funkce E -> id ( <ARG_LIST> ).
 */
void PrecParser_reduceFunCall() {
    // Popnutí a uvolnění ')'
    PrecStackNode *rightBracket = PrecStack_pop();
    free(rightBracket);

    // Popnutí '<ARG_LIST>'
    PrecStackNode *argumentsNode = PrecStack_pop();

    // Popnutí a uvolnění '('
    PrecStackNode *leftBracket = PrecStack_pop();
    free(leftBracket);

    // Popnutí 'id'
    PrecStackNode *varNode = PrecStack_pop();

    // Vytvoření AST uzlu pro volání funkce
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    AST_VarNode *idNode = (AST_VarNode *)varNode->node;
    AST_initNewFunCallNode(funCallNode, idNode->identifier, false, argumentsNode->node);

    // Vytvoření AST uzlu pro výraz
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    AST_initNewExprNode(exprNode, AST_EXPR_FUN_CALL, funCallNode);

    // Pushnutí nového neterminálu E na zásobník s AST uzlem
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, AST_EXPR_NODE, exprNode);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(idNode);
    free(varNode);
    free(argumentsNode);
} // PrecParser_reduceFunCall()

/**
 * @brief Redukce pro volání vestavěné funkce E -> ifj . id ( <ARG_LIST> ).
 */
void PrecParser_reduceIfjFunCall() {
    // Popnutí a uvolnění ')'
    PrecStackNode *rightBracket = PrecStack_pop();
    free(rightBracket);

    // Popnutí '<ARG_LIST>'
    PrecStackNode *argumentsNode = PrecStack_pop();

    // Popnutí a uvolnění '('
    PrecStackNode *leftBracket = PrecStack_pop();
    free(leftBracket);

    // Popnutí 'id'
    PrecStackNode *varNode = PrecStack_pop();

    // Popnutí a uvolnění '.'
    PrecStackNode *dot = PrecStack_pop();
    free(dot);

    // Popnutí a uvolnění 'ifj'
    PrecStackNode *ifj = PrecStack_pop();
    free(ifj);

    // Vytvoření AST uzlu pro volání funkce
    AST_FunCallNode *funCallNode = (AST_FunCallNode *)AST_createNode(AST_FUN_CALL_NODE);
    AST_VarNode *idNode = (AST_VarNode *)varNode->node;
    AST_initNewFunCallNode(funCallNode, idNode->identifier, true, argumentsNode->node);

    // Vytvoření AST uzlu pro výraz
    AST_ExprNode *exprNode = (AST_ExprNode *)AST_createNode(AST_EXPR_NODE);
    AST_initNewExprNode(exprNode, AST_EXPR_FUN_CALL, funCallNode);

    // Pushnutí nového neterminálu E na zásobník s AST uzlem
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_EXPRESSION, AST_EXPR_NODE, exprNode);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(idNode);
    free(varNode);
    free(argumentsNode);
} // PrecParser_reduceIfjFunCall()

/**
 * @brief Redukce pro argumenty <ARG_LIST> -> E <ARG>.
 */
void PrecParser_reduceArgListExprArg() {
    // Popnutí '<ARG>'
    PrecStackNode *argNode = PrecStack_pop();

    // Popnutí 'E'
    PrecStackNode *exprNode = PrecStack_pop();

    // Vytvoření nového uzlu pro argument
    AST_ArgOrParamNode *argList = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    AST_initNewArgOrParamNode(argList, AST_DATA_TYPE_NOT_DEFINED, exprNode->node);
    argList->next = argNode->node;

    // Pushnutí neterminálu '<ARG_LIST>' s uzlem argumentů
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_ARG_LIST, AST_ARG_OR_PARAM_NODE, argList);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(argNode);
    free(exprNode);
} // PrecParser_reduceArgListExprArg()

/**
 * @brief Redukce pro seznam argumentů <ARG_LIST> -> ε.
 */
void PrecParser_reduceArgListEpsilon() {
    // Vytvoření nového prázdného uzlu pro argument
    AST_ArgOrParamNode *argList = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);

    // Pushnutí neterminálu '<ARG_LIST>' s uzlem argumentů
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_ARG_LIST, AST_ARG_OR_PARAM_NODE, argList);
} // PrecParser_reduceArgListEpsilon()

/**
 * @brief Redukce pro argument <ARG> -> , E <ARG>.
 */
void PrecParser_reduceArgCommaExprArg() {
    // Popnutí '<ARG>'
    PrecStackNode *argNode = PrecStack_pop();

    // Popnutí 'E'
    PrecStackNode *exprNode = PrecStack_pop();

    // Popnutí ','
    PrecStackNode *commaNode = PrecStack_pop();
    PrecStack_freeNode(commaNode);

    // Vytvoření nového uzlu pro argument
    AST_ArgOrParamNode *arg = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);
    AST_initNewArgOrParamNode(arg, AST_DATA_TYPE_NOT_DEFINED, exprNode->node);
    arg->next = argNode->node;

    // Pushnutí neterminálu '<ARG>' s uzlem argumentů
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_ARG, AST_ARG_OR_PARAM_NODE, arg);

    // Uvolnění popnutých uzlů, protože jejich AST uzly byly již použity
    free(argNode);
    free(exprNode);
} // PrecParser_reduceArgCommaExprArg()

/**
 * @brief Redukce pro argument <ARG> -> ,
 */
void PrecParser_reduceArgComma() {
    // Popnutí ','
    PrecStackNode *commaNode = PrecStack_pop();
    PrecStack_freeNode(commaNode);

    // Vytvoření nového prázdného uzlu pro argument
    AST_ArgOrParamNode *arg = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);

    // Pushnutí neterminálu '<ARG_LIST>' s uzlem argumentů
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_ARG, AST_ARG_OR_PARAM_NODE, arg);
} // PrecParser_reduceArgComma()


/**
 * @brief Redukce pro argument <ARG> -> ε
 */
void PrecParser_reduceArgEpsilon() {
    // Vytvoření nového prázdného uzlu pro argument
    AST_ArgOrParamNode *arg = (AST_ArgOrParamNode *)AST_createNode(AST_ARG_OR_PARAM_NODE);

    // Pushnutí neterminálu '<ARG_LIST>' s uzlem argumentů
    PrecStack_pushPrecNonTerminal(PREC_STACK_NT_ARG, AST_ARG_OR_PARAM_NODE, arg);
} // PrecParser_reduceArgEpsilon()


/*******************************************************************************
 *                                                                             *
 *                   IMPLEMENTACE POMOCNÝCH INTERNÍCH FUNKCÍ                   *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Namapuje typ tokenu na typ precedenčního terminálu.
 */
void PrecParser_mapToDollar(int bracketDepth, PrecTerminals *terminal) {
    // Ověření platnosti předaného ukazatele
    if (terminal == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    switch(currentToken.PrecTerminal) {
        // Mapování: T_PREC_RIGHT_BRACKET -> T_PREC_RIGHT_BRACKET nebo T_PREC_DOLLAR
        case T_PREC_RIGHT_BRACKET:
            if (currentDollar ==  DOLLAR_T_RIGHT_BRACKET && bracketDepth == 0) {
                *terminal = T_PREC_DOLLAR;
            }
            break;

        // Mapování: T_PREC_COMMA -> T_PREC_COMMA nebo T_PREC_DOLLAR
        case T_PREC_COMMA:
            if (currentDollar ==  DOLLAR_T_COMMA && bracketDepth == 0) {
                *terminal = T_PREC_DOLLAR;
            }
            break;

        // Defaultní případ: Nejedná se o dolar
        default:
            *terminal = currentToken.PrecTerminal;
            break;
    } // switch()
} // PrecParser_mapToDollar()

/**
 * @brief Mapuje neterminál na redukční pravidlo.
 */
void PrecParser_mapNonTerminalToRule(PrecStackSymbol symbol, ReductionRule *rule) {
    // Ověření platnosti předaného ukazatele
    if (rule == NULL) {
        error_handle(ERROR_INTERNAL);
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

        // Mapování: Seznam argumentů na základní výraz -> REDUCE_ARG_LIST_TO_E_ARG
        case PREC_STACK_SYM_ARG_LIST:
            *rule = REDUCE_ARG_LIST_TO_E_ARG;
            break;

        // Mapování: Oddělovač argumentů v seznamu -> REDUCE_ARG_TO_COMMA_E_ARG
        case PREC_STACK_SYM_COMMA:
            *rule = REDUCE_ARG_TO_COMMA_E_ARG;
            break;

        // Defaultní případ: interní chyba
        default:
            error_handle(ERROR_INTERNAL);
            break;
    } // switch()
} // PrecParser_mapNonTerminalToRule()

/*** Konec souboru precedence_parser.c ***/
