/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_stack.c                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            11.11.2024                                                *
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
 * @file precedence_stack.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementace funkcí pro správu precedenčního zásobníku.
 * @details Tento soubor obsahuje implementaci funkcí pro manipulaci se
 *          zásobníkem používaným v precedenční syntaktické analýze. Zahrnuje
 *          inicializaci, uvolnění paměti, kontrolu stavu zásobníku a operace
 *          pro práci s vrcholem. Umožňuje efektivní řízení přechodů mezi
 *          stavy při analýze výrazů.
 */

#include "precedence_stack.h"


/*******************************************************************************
 *                                                                             *
 *                POČÁTEČNÍ INICIALIZACE GLOBÁLNÍCH PROMĚNNÝCH                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro seznam precedenčních zásobníků.
 */
struct PrecStackList *precStackList = NULL;


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje globální seznam precedenčních zásobníků.
 */
void PrecStackList_create() {
    if(precStackList == NULL) {
        // Alokace paměti pro seznam precedenční zásobníků
        precStackList = (PrecStackList *)malloc(sizeof(PrecStackList));

        // Pokud se alokace nezdařile nastavujeme interní error
        if (precStackList == NULL) {
            Parser_errorWatcher(SET_ERROR_INTERNAL);
            return;
        }

        // Počáteční inicializace prvního zásobníku v seznamu na NULL
        precStackList->stack = NULL;
    }
} // PrecStackList_create()

/**
 * @brief Uvolní globální strukturu seznamu zásobníků.
 */
void PrecStackList_destroy() {
    if(precStackList != NULL) {
        free(precStackList);        // Uvvolnění paměti pro seznam zásobníků
        precStackList = NULL;       // Uvedení do neinicializovaného stavu
    }
} // PrecStackList_destroy()

/**
 * @brief Přidá nový precedenční zásobník na vrchol seznamu a inicializuje jej.
 */
void PrecStackList_push() {
    // Kontrolujeme, že seznam zásobníků existuje
    if(precStackList == NULL) {
        return;
    }

    // Vytvoření nového precedenčního zásobníku a kontrola úspěchu alokace
    PrecStack *newStack = (PrecStack *)malloc(sizeof(PrecStack));
    if (newStack == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Inicializujem nový zásobník jako prázdný
    newStack->top = NULL;

    // Přidání nového zásobníku do seznamu zásobníků
    newStack->next = precStackList->stack;
    precStackList->stack = newStack;

    // Inicializace nového zásobníku - push počátečního "dollar" symbolu
    PrecStack_pushPrecTerminal(T_PREC_DOLLAR, SN_WITHOUT_AST_TYPE, SN_WITHOUT_AST_PTR);
} // PrecStackList_push()

/**
 * @brief Odebere a uvolní vrcholový zásobník ze seznamu (bez uvolnění AST uzlů).
 */
void PrecStackList_pop() {
    // Kontrola, že seznam a zásobníků a jeho vrchol existují
    if (precStackList == NULL || precStackList->stack == NULL) {
        return;
    }

    // Nastavíme nový vrchol seznamu zásobníků
    PrecStack *topStack = precStackList->stack;
    precStackList->stack = topStack->next;

    PrecStack_dispose(topStack);    // Uvolní zásobníka a zásobníkové uzly bez AST uzlů
} // PrecStackList_pop()

/**
 * @brief Uvolní všechny zásobníky a jejich obsah ze seznamu
 *        pomocí @c PrecStack_purge() (včetně uvolnění AST uzlů).
 */
void PrecStackList_purge() {
    // Uvolní všechny zásobníky včetně AST uzlů
    while(precStackList != NULL && precStackList->stack != NULL) {
        PrecStack_purge(precStackList->stack);
    }

    if(precStackList != NULL) {
        free(precStackList);
        precStackList = NULL;
    }
} // PrecStackList_purge()

/**
 * @brief Pushne precedenční terminál na aktuální precedenční zásobník.
 */
void PrecStack_pushPrecTerminal(PrecTerminals terminal, AST_NodeType type, void *node) {
    // Pokud není precStack alokovaný, nic se neděje
    if(precStackList == NULL || precStackList->stack == NULL) {
        return;
    }

    // Namapujeme typ precedenčního terminálu na typ zásobníkového symbolu
    PrecStackSymbol stackSymbol = PREC_STACK_SYM_UNEDFINED;
    PrecStack_mapPrecTerminalToStackSymbol(terminal, &stackSymbol);

    // Vytvoříme nový uzel zásobníku
    PrecStackNode *newStackNode = PrecStack_createStackNode();

    // Inicializace nové zásobníkového uzlu pro terminál
    if(newStackNode != NULL) {
        newStackNode->symbolType = STACK_NODE_TYPE_TERMINAL;
        newStackNode->symbol = stackSymbol;
        newStackNode->nodeType = type;
        newStackNode->node = node;
        newStackNode->next = PrecStack_top();

        // Nastavení nového uzlu jako vrcholu zásobníku
        precStackList->stack->top = newStackNode;
    }
} // PrecStack_pushPrecTerminal()

/**
 * @brief Pushne NEterminál na globální precedenční zásobník.
 */
void PrecStack_pushPrecNonTerminal(PrecStackNonTerminals symbol, AST_NodeType type, void *node) {
    // Pokud není precStack alokovaný, nic se neděje
    if(precStackList == NULL || precStackList->stack == NULL) {
        return;
    }

    // Namapujeme typ zásobníkového neterminálu na typ zásobníkového symbolu
    PrecStackSymbol stackSymbol = PREC_STACK_SYM_UNEDFINED;
    PrecStack_mapStackNonTerminalToStackSymbol(symbol, &stackSymbol);

    // Vytvoříme nový uzel zásobníku
    PrecStackNode *newStackNode = PrecStack_createStackNode();

    // Inicializace nové zásobníkového uzlu pro neterminál
    if(newStackNode != NULL) {
        // Nastavení typu symbolu nového uzlu na základě typu neterminálu
        switch(symbol) {
            case PREC_STACK_NT_EXPRESSION:
            case PREC_STACK_NT_ARG_LIST:
                newStackNode->symbolType = STACK_NODE_TYPE_NONTERMINAL;
                break;
            case PREC_STACK_NT_HANDLE:
                newStackNode->symbolType = STACK_NODE_TYPE_HANDLE;
                break;
            default:
                Parser_errorWatcher(SET_ERROR_INTERNAL);
        }

        // Inicializujeme zbytek nového uzlu
        newStackNode->symbol = stackSymbol;
        newStackNode->nodeType = type;
        newStackNode->node = node;
        newStackNode->next = PrecStack_top();

        // Nastavení nového uzlu jako vrcholu zásobníku
        precStackList->stack->top = newStackNode;
    }
} // PrecStack_pushPrecNonTerminal()

/**
 * @brief Pushne inicializovaný Stack uzel na zásobník, popř. i s AST uzlem.
 */
void PrecStack_pushBothStackAndASTNode(PrecTerminals inTerminal) {
    // Switch specifikující uzel pushnutý na zásbník
    switch(inTerminal) {
        // Pro uzel pro proměnnou
        case T_PREC_ID: {
            // Vytvoření a konkrétní inicializace uzlu pro proměnnou (identifikátor)
            AST_VarNode *pushNode = (AST_VarNode *)AST_createNode(AST_VAR_NODE);
            AST_initNewVarNode(pushNode, AST_VAR_NODE, currentTerminal.value,
                               frameStack.currentID, AST_LITERAL_NOT_DEFINED,
                               AST_VAL_UNDEFINED);

            // Pushnutí uzlu na zásobník
            PrecStack_pushPrecTerminal(inTerminal, AST_VAR_NODE, pushNode);
            break;
        } // case T_PREC_ID

        // Pro uzel pro pro literál typu "i32"
        case T_PREC_INT_LITERAL: {
            // Vytvoření a konkrétní inicializace uzlu pro i32 literál
            AST_VarNode *pushNode = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
            AST_initNewVarNode(pushNode, AST_LITERAL_NODE, AST_ID_UNDEFINED,
                               frameStack.currentID, AST_LITERAL_INT,
                               currentTerminal.value);

            // Pushnutí uzlu na zásobník
            PrecStack_pushPrecTerminal(inTerminal, AST_LITERAL_NODE, pushNode);
            break;
        } // case T_PREC_INT_LITERAL

        // Pro uzel pro pro literál typu "f64"
        case T_PREC_FLOAT_LITERAL: {
            // Vytvoření a konkrétní inicializace uzlu pro f64 literál
            AST_VarNode *pushNode = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
            AST_initNewVarNode(pushNode, AST_LITERAL_NODE, AST_ID_UNDEFINED,
                               frameStack.currentID, AST_LITERAL_FLOAT,
                               currentTerminal.value);

            // Pushnutí uzlu na zásobník
            PrecStack_pushPrecTerminal(inTerminal, AST_LITERAL_NODE, pushNode);
            break;
        } // case T_PREC_FLOAT_LITERAL

        // Pro uzel pro pro literál typu "[]u8"
        case T_PREC_STRING_LITERAL: {
            // Vytvoření a konkrétní inicializace uzlu pro []u8 literál
            AST_VarNode *pushNode = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
            AST_initNewVarNode(pushNode, AST_LITERAL_NODE, AST_ID_UNDEFINED,
                               frameStack.currentID, AST_LITERAL_STRING,
                               currentTerminal.value);

            // Pushnutí uzlu na zásobník
            PrecStack_pushPrecTerminal(inTerminal, AST_LITERAL_NODE, pushNode);
            break;
        } // case T_PREC_STRING_LITERAL

        // Pro uzel pro pro literál typu "NULL"
        case T_PREC_NULL_LITERAL: {
            // Vytvoření a konkrétní inicializace uzlu pro null literál
            AST_VarNode *pushNode = (AST_VarNode *)AST_createNode(AST_LITERAL_NODE);
            AST_initNewVarNode(pushNode, AST_LITERAL_NODE, AST_ID_UNDEFINED,
                               frameStack.currentID, AST_LITERAL_NULL,
                               currentTerminal.value);

            // Pushnutí uzlu na zásobník
            PrecStack_pushPrecTerminal(inTerminal, AST_LITERAL_NODE, pushNode);
            break;
        } // case T_PREC_NULL_LITERAL

        // Jinak oushujeme terminálový uzel na zásobník bez vytvoření AST uzlu
        default:
            PrecStack_pushPrecTerminal(inTerminal, SN_WITHOUT_AST_TYPE, SN_WITHOUT_AST_PTR);
    } // switch()
} // PrecStack_pushBothStackAndASTNode()

/**
 * @brief Pushne handle za první terminál na zásobníku.
 */
void PrecStack_pushHandleAfterFirstTerminal() {
    // Kontrolujeme, že aexstuje seznam zásbníků a vrcholový zásobník
    if (precStackList == NULL || precStackList->stack == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Připravíme si pomocné ukazatele k průchodu aktuálním zásobníkem
    PrecStackNode *current = PrecStack_top();
    PrecStackNode *prev = NULL;

    // Procházáme zásobník, dokud nenajdieme první terminál na zásobníku
    while (current != NULL && current->symbolType != STACK_NODE_TYPE_TERMINAL) {
        prev = current;
        current = current->next;
    }

    // Pokud jsme žádný nenalezly, nastala interní chyba (nemělo by nikdy nastat)
    if(current == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
    }

    // Vytvoření nového uzlu pro handle
    PrecStackNode *handleNode = PrecStack_createStackNode();

    // Inicializujeme uzel pro handle
    if(handleNode != NULL) {
        handleNode->symbolType = STACK_NODE_TYPE_HANDLE;
        handleNode->symbol = PREC_STACK_SYM_HANDLE;
        handleNode->nodeType = SN_WITHOUT_AST_TYPE;
        handleNode->node = SN_WITHOUT_AST_PTR;

        // Vložení handle za první terminál
        if (prev == NULL) {
            // Pokud je první terminál na vrcholu zásobníku
            handleNode->next = PrecStack_top();
            precStackList->stack->top = handleNode;
        }
        // Pokud je první terminál někde uvnitř zásobníku
        else {
            handleNode->next = current;
            prev->next = handleNode;
        }
    }
} // PrecStack_pushHandleAfterFirstTerminal()

/**
 * @brief Popne uzel AST z globálního precedenčního zásobníku.
 */
PrecStackNode *PrecStack_pop() {
    // Zkontrolujeme, že je co popnout
    if(precStackList == NULL || precStackList->stack == NULL || precStackList->stack->top == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return PARSING_ERROR;
    }

    // Aktualizujeme vrcholový uzel aktuálního zásobníku
    PrecStackNode *node = PrecStack_top();
    precStackList->stack->top = node->next;

    // Vracíme ukazatel na vyňatý uzel
    return node;
} // PrecStack_pop()

/**
 * @brief Získá ukazatel na uzel na vrcholu globálního precedenčního zásobníku
 *        bez jeho popnutí.
 */
inline PrecStackNode* PrecStack_top() {
    // Kontrola, zda je zásobník alokovaný
    if(precStackList == NULL || precStackList->stack == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return PARSING_ERROR;
    }

    // Vracíme ukazatel na vrcholový prvek (může být NULL)
    return precStackList->stack->top;
} // PrecStack_top()

/**
 * @brief Uvolní všechny zdroje spojené s uzlem @c PrecStackNode.
 */
void PrecStack_freeNode(PrecStackNode *stackNode) {
    // Zkontrolujeme, platnost předaného ukazatele
    if(stackNode == NULL) {
        return;
    }

    // Uvolnění uzlu AST spojeného s terminálem nebo neterminálem
    if(stackNode->node != NULL) {
        // Pokud se jedná o seznam argumentů, voláme specializovanou funkci k jeho destrukci
        if(stackNode->symbol == PREC_STACK_SYM_ARG_LIST) {
            AST_destroyArgOrParamList(stackNode->node);
        }
        // Jinak voláme obecnou funkci na destrukci AST uzlu
        else {
            AST_destroyNode(stackNode->nodeType, stackNode->node);
        }
    }

    // Uvolnění samotného uzlu "PrecStackNode"stackNode
    free(stackNode);
    stackNode = NULL;
} // PrecStack_freeNode()

/**
 * @brief Získá terminál na vrcholu zásobníku, který je nejblíže vrcholu.
 */
void PrecStack_getTopPrecTerminal(PrecTerminals *topTerminal) {
    // Kontrolujem platnost přijatého ukazatele a struktur zásobníku
    if(topTerminal == NULL || precStackList == NULL || precStackList->stack == NULL) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
        *topTerminal = T_PREC_UNDEFINED;
        return;
    }

    // Získání vrcholu zásobníku
    PrecStackNode* stackTopNode = PrecStack_top();

    // Pokud je zásobník prázdný nastala syntaktická chyba
    if(stackTopNode == NULL) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
        *topTerminal = T_PREC_UNDEFINED;
        return;
    }

    // Cyklíme, dokud nenajdeme první terminál na zásobníku
    while(stackTopNode != NULL && stackTopNode->symbolType != STACK_NODE_TYPE_TERMINAL) {
        stackTopNode = stackTopNode->next;
    }

    // Pokud nebyl na zásobníku teminál nalezen, nastala interní chyba překladače
    if(stackTopNode == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        *topTerminal = T_PREC_UNDEFINED;
    }

    // Namapujeme zásobníkový symbol na typ precedenčního terminálu
    PrecStack_mapStackSymbolToPrecTerminal(stackTopNode->symbol, topTerminal);
} // PrecStack_getTopPrecTerminal()


/**
 * @brief Zkontroluje, zda je na vrcholu zásobníku symbol ID.
 */
bool PrecStack_isIdOnTop() {
    // Získání vrcholu zásobníku
    PrecStackNode* stackTopNode = PrecStack_top();

    // Pokud je zásobník prázdný, vrátíme false
    if(stackTopNode == NULL) {
        return false;
    }

    // Kontrola, zda je na vrcholu zásobníku symbol ID
    return (stackTopNode->symbol == PREC_STACK_SYM_ID);
} // PrecStack_isIdOnTop()

/**
 * @brief Získá výsledný AST uzel výrazu z precedenčního zásobníku.
 */
void PrecStack_getResult(AST_ExprNode **result) {
    // Kontrola, že zásobník není prázdný
    if(PrecStack_isEmpty()) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Získáme vrchní prvek na zásobníku
    PrecStackNode *topNode = PrecStack_top();
    if(topNode == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Ověříme, že vrchní prvek obsahuje výraz
    if(topNode->symbol != PREC_STACK_SYM_EXPRESSION || topNode->node == NULL) {
        Parser_errorWatcher(SET_ERROR_SYNTAX);
        return;
    }

    // Uložíme výsledný "AST_ExprNode *""
    *result = topNode->node;
} // PrecStack_getResult()


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku a
 *        inicializuje ho do počátečního stavu.
 */
void PrecStack_dispose(PrecStack *stack) {
    // Kontrola, že zásbník již není prázdný
    if(stack == NULL) {
        return;
    }

    // Sekvenční mazání prvků na zásobníku (bez AST uzlů)
    while(stack->top != NULL) {
        // Odložíme si prvek na smazání a jako nový vrchol zásobníku nastavíme následníka
        PrecStackNode* toDelete = stack->top;
        stack->top = toDelete->next;

        // Uvolníme paměť pro aktuální vrvcholový prvek
        free(toDelete);
    }

    // Uvolníme strukturu zásobníku a zaktualizjeme vrchol seznamu zásobníků
    PrecStack *nextStack = stack->next;
    free(stack);
    precStackList->stack = nextStack;
} // PrecStack_dispose()

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku pomocí
 *        funkce @c PrecStack_freeNode().
 */
void PrecStack_purge(PrecStack *stack) {
    // Kontrola, že zásobník není prázdný
    if(stack == NULL) {
        return;
    }

    // Sekvenční mazání prvků na zásobníku (včetně AST uzlů)
    while (stack->top != NULL) {
        // Uložíme ukazatel na další uzel před uvolněním aktuálního uzlu
        PrecStackNode* toDelete = stack->top;
        stack->top = toDelete->next;

        // Uvolníme vrcholový uzel pomocí funkce PrecStack_freeNode
        PrecStack_freeNode(toDelete);
    }

    // Uvolníme strukturu zásobníku a zaktualizjeme vrchol seznamu zásobníků
    PrecStack *nextStack = stack->next;
    free(stack);
    precStackList->stack = nextStack;
} // PrecStack_purge()

/**
 * @brief Zkontroluje, zda je aktuální precedenční zásobník prázdný.
 */
bool PrecStack_isEmpty() {
    // Kontrola, zda je zásobník alokovaný
    if (precStackList == NULL || precStackList->stack == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return false;
    }

    // Jinak zkontroluj existenci vrchního prvku
    return (precStackList->stack->top == NULL);
} // PrecStack_isEmpty()

/**
 * @brief Vytvoří nový zásobníkový uzel typu typu @c PrecStackNode.
 */
PrecStackNode *PrecStack_createStackNode() {
    // Kontrola, zda je zásobník alokovaný
    if (precStackList == NULL || precStackList->stack == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return NULL;
    }

    // Alokujeme paměť pro nový uzel typu "PrecStackNode"
    PrecStackNode *node = (PrecStackNode *)malloc(sizeof(PrecStackNode));

    // Pokud se alokace nezdařila, hlásíme interní chybu překladače
    if (node == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return NULL;
    }

    // Počáteční inicializace členů uzlu
    node->symbolType = STACK_NODE_TYPE_UNDEFINED;
    node->symbol = PREC_STACK_SYM_UNEDFINED;
    node->nodeType = SN_WITHOUT_AST_TYPE;
    node->node = SN_WITHOUT_AST_PTR;
    node->next = NULL;

    // Vracíme ukazatel na nový uzel typu "PrecStackNode"
    return node;
} // PrecStack_createStackNode()

/**
 * @brief Namapuje typ precedenčního terminálu na typ zásobníkového symbolu.
 */
void PrecStack_mapPrecTerminalToStackSymbol(PrecTerminals terminal, PrecStackSymbol *stackSymbol) {
    // Ověření platnosti předaného ukazatele
    if(stackSymbol == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Mapujeme precedenční terminál na zásobníkobý symbol
    switch(terminal) {
        // Mapování: T_PREC_ID -> PREC_STACK_SYM_ID
        case T_PREC_ID:
            *stackSymbol = PREC_STACK_SYM_ID;
            break;

        // Mapování: T_PREC_INT_LITERAL -> PREC_STACK_SYM_INT_LITERAL
        case T_PREC_INT_LITERAL:
            *stackSymbol = PREC_STACK_SYM_INT_LITERAL;
            break;

        // Mapování: T_PREC_FLOAT_LITERAL -> PREC_STACK_SYM_FLOAT_LITERAL
        case T_PREC_FLOAT_LITERAL:
            *stackSymbol = PREC_STACK_SYM_FLOAT_LITERAL;
            break;

        // Mapování: T_PREC_STRING_LITERAL -> PREC_STACK_SYM_STRING_LITERAL
        case T_PREC_STRING_LITERAL:
            *stackSymbol = PREC_STACK_SYM_STRING_LITERAL;
            break;

        // Mapování: T_PREC_NULL_LITERAL -> PREC_STACK_SYM_NULL_LITERAL
        case T_PREC_NULL_LITERAL:
            *stackSymbol = PREC_STACK_SYM_NULL_LITERAL;
            break;

        // Mapování: T_PREC_IFJ -> PREC_STACK_SYM_IFJ
        case T_PREC_IFJ:
            *stackSymbol = PREC_STACK_SYM_IFJ;
            break;

        // Mapování: T_PREC_DOT -> PREC_STACK_SYM_DOT
        case T_PREC_DOT:
            *stackSymbol = PREC_STACK_SYM_DOT;
            break;

        // Mapování: T_PREC_LEFT_BRACKET -> PREC_STACK_SYM_LEFT_BRACKET
        case T_PREC_LEFT_BRACKET:
            *stackSymbol = PREC_STACK_SYM_LEFT_BRACKET;
            break;

        // Mapování: T_PREC_RIGHT_BRACKET -> PREC_STACK_SYM_RIGHT_BRACKET
        case T_PREC_RIGHT_BRACKET:
            *stackSymbol = PREC_STACK_SYM_RIGHT_BRACKET;
            break;

        // Mapování: T_PREC_PLUS -> PREC_STACK_SYM_PLUS
        case T_PREC_PLUS:
            *stackSymbol = PREC_STACK_SYM_PLUS;
            break;

        // Mapování: T_PREC_MINUS -> PREC_STACK_SYM_MINUS
        case T_PREC_MINUS:
            *stackSymbol = PREC_STACK_SYM_MINUS;
            break;

        // Mapování: T_PREC_MULTIPLICATION -> PREC_STACK_SYM_MULTIPLICATION
        case T_PREC_MULTIPLICATION:
            *stackSymbol = PREC_STACK_SYM_MULTIPLICATION;
            break;

        // Mapování: T_PREC_DIVISION -> PREC_STACK_SYM_DIVISION
        case T_PREC_DIVISION:
            *stackSymbol = PREC_STACK_SYM_DIVISION;
            break;

        // Mapování: T_PREC_IDENTITY -> PREC_STACK_SYM_IDENTITY
        case T_PREC_IDENTITY:
            *stackSymbol = PREC_STACK_SYM_IDENTITY;
            break;

        // Mapování: T_PREC_NOT_EQUAL -> PREC_STACK_SYM_NOT_EQUAL
        case T_PREC_NOT_EQUAL:
            *stackSymbol = PREC_STACK_SYM_NOT_EQUAL;
            break;

        // Mapování: T_PREC_LESS_THAN -> PREC_STACK_SYM_LESS_THAN
        case T_PREC_LESS_THAN:
            *stackSymbol = PREC_STACK_SYM_LESS_THAN;
            break;

        // Mapování: T_PREC_GREATER_THAN -> PREC_STACK_SYM_GREATER_THAN
        case T_PREC_GREATER_THAN:
            *stackSymbol = PREC_STACK_SYM_GREATER_THAN;
            break;

        // Mapování: T_PREC_LESS_THAN_OR_EQUAL -> PREC_STACK_SYM_LESS_THAN_OR_EQUAL
        case T_PREC_LESS_THAN_OR_EQUAL:
            *stackSymbol = PREC_STACK_SYM_LESS_THAN_OR_EQUAL;
            break;

        // Mapování: T_PREC_GREATER_THAN_OR_EQUAL -> PREC_STACK_SYM_GREATER_THAN_OR_EQUAL
        case T_PREC_GREATER_THAN_OR_EQUAL:
            *stackSymbol = PREC_STACK_SYM_GREATER_THAN_OR_EQUAL;
            break;

        // Mapování: T_PREC_COMMA -> PREC_STACK_SYM_COMMA
        case T_PREC_COMMA:
            *stackSymbol = PREC_STACK_SYM_COMMA;
            break;

        // Mapování: T_PREC_DOLLAR -> PREC_STACK_SYM_DOLLAR
        case T_PREC_DOLLAR:
            *stackSymbol = PREC_STACK_SYM_DOLLAR;
            break;

        // Defaultní případ: syntaktická chyba
        default:
            Parser_errorWatcher(SET_ERROR_SYNTAX);
            break;
    } // switch()
}  // PrecStack_mapPrecTerminalToStackSymbol()

/**
 * @brief Namapuje typ zásobníkového symbolu na typ precedenčního terminálu.
 */
void PrecStack_mapStackSymbolToPrecTerminal(PrecStackSymbol stackSymbol, PrecTerminals *terminal) {
    if (terminal == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Mapujeme symbol na terminál
    switch(stackSymbol) {
        // Mapování: PREC_STACK_SYM_ID -> T_PREC_ID
        case PREC_STACK_SYM_ID:
            *terminal = T_PREC_ID;
            break;

        // Mapování: PREC_STACK_SYM_INT_LITERAL -> T_PREC_INT_LITERAL
        case PREC_STACK_SYM_INT_LITERAL:
            *terminal = T_PREC_INT_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_FLOAT_LITERAL -> T_PREC_FLOAT_LITERAL
        case PREC_STACK_SYM_FLOAT_LITERAL:
            *terminal = T_PREC_FLOAT_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_STRING_LITERAL -> T_PREC_STRING_LITERAL
        case PREC_STACK_SYM_STRING_LITERAL:
            *terminal = T_PREC_STRING_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_NULL_LITERAL -> T_PREC_NULL_LITERAL
        case PREC_STACK_SYM_NULL_LITERAL:
            *terminal = T_PREC_NULL_LITERAL;
            break;

        // Mapování: PREC_STACK_SYM_IFJ -> T_PREC_IFJ
        case PREC_STACK_SYM_IFJ:
            *terminal = T_PREC_IFJ;
            break;

        // Mapování: PREC_STACK_SYM_DOT -> T_PREC_DOT
        case PREC_STACK_SYM_DOT:
            *terminal = T_PREC_DOT;
            break;

        // Mapování: PREC_STACK_SYM_LEFT_BRACKET -> T_PREC_LEFT_BRACKET
        case PREC_STACK_SYM_LEFT_BRACKET:
            *terminal = T_PREC_LEFT_BRACKET;
            break;

        // Mapování: PREC_STACK_SYM_RIGHT_BRACKET -> T_PREC_RIGHT_BRACKET
        case PREC_STACK_SYM_RIGHT_BRACKET:
            *terminal = T_PREC_RIGHT_BRACKET;
            break;

        // Mapování: PREC_STACK_SYM_PLUS -> T_PREC_PLUS
        case PREC_STACK_SYM_PLUS:
            *terminal = T_PREC_PLUS;
            break;

        // Mapování: PREC_STACK_SYM_MINUS -> T_PREC_MINUS
        case PREC_STACK_SYM_MINUS:
            *terminal = T_PREC_MINUS;
            break;

        // Mapování: PREC_STACK_SYM_MULTIPLICATION -> T_PREC_MULTIPLICATION
        case PREC_STACK_SYM_MULTIPLICATION:
            *terminal = T_PREC_MULTIPLICATION;
            break;

        // Mapování: PREC_STACK_SYM_DIVISION -> T_PREC_DIVISION
        case PREC_STACK_SYM_DIVISION:
            *terminal = T_PREC_DIVISION;
            break;

        // Mapování: PREC_STACK_SYM_IDENTITY -> T_PREC_IDENTITY
        case PREC_STACK_SYM_IDENTITY:
            *terminal = T_PREC_IDENTITY;
            break;

        // Mapování: PREC_STACK_SYM_NOT_EQUAL -> T_PREC_NOT_EQUAL
        case PREC_STACK_SYM_NOT_EQUAL:
            *terminal = T_PREC_NOT_EQUAL;
            break;

        // Mapování: PREC_STACK_SYM_LESS_THAN -> T_PREC_LESS_THAN
        case PREC_STACK_SYM_LESS_THAN:
            *terminal = T_PREC_LESS_THAN;
            break;

        // Mapování: PREC_STACK_SYM_GREATER_THAN -> T_PREC_GREATER_THAN
        case PREC_STACK_SYM_GREATER_THAN:
            *terminal = T_PREC_GREATER_THAN;
            break;

        // Mapování: PREC_STACK_SYM_LESS_THAN_OR_EQUAL -> T_PREC_LESS_THAN_OR_EQUAL
        case PREC_STACK_SYM_LESS_THAN_OR_EQUAL:
            *terminal = T_PREC_LESS_THAN_OR_EQUAL;
            break;

        // Mapování: PREC_STACK_SYM_GREATER_THAN_OR_EQUAL -> T_PREC_GREATER_THAN_OR_EQUAL
        case PREC_STACK_SYM_GREATER_THAN_OR_EQUAL:
            *terminal = T_PREC_GREATER_THAN_OR_EQUAL;
            break;

        // Mapování: PREC_STACK_SYM_COMMA -> T_PREC_COMMA
        case PREC_STACK_SYM_COMMA:
            *terminal = T_PREC_COMMA;
            break;

        // Mapování: PREC_STACK_SYM_DOLLAR -> T_PREC_DOLLAR
        case PREC_STACK_SYM_DOLLAR:
            *terminal = T_PREC_DOLLAR;
            break;

        // Defaultní případ: syntaktická chyba
        default:
            Parser_errorWatcher(SET_ERROR_SYNTAX);
            break;
    } // switch()
}  // PrecStack_mapStackSymbolToPrecTerminal()

/**
 * @brief Namapuje typ zásobníkového neterminálu na typ zásobníkového symbolu.
 */
void PrecStack_mapStackNonTerminalToStackSymbol(PrecStackNonTerminals stackNonTerminal, PrecStackSymbol *symbol) {
    // Ověření platnosti předaného ukazatele
    if(symbol == NULL) {
        Parser_errorWatcher(SET_ERROR_INTERNAL);
        return;
    }

    // Mapujeme zásobníkový neterminál na zásobníkový symbol
    switch(stackNonTerminal) {
        // Mapování: PREC_STACK_NT_EXPRESSION -> PREC_STACK_SYM_EXPRESSION
        case PREC_STACK_NT_EXPRESSION:
            *symbol = PREC_STACK_SYM_EXPRESSION;
            break;

        // Mapování: PREC_STACK_NT_ARG_LIST -> PREC_STACK_SYM_ARG_LIST
        case PREC_STACK_NT_ARG_LIST:
            *symbol = PREC_STACK_SYM_ARG_LIST;
            break;

        // Mapování: PREC_STACK_NT_HANDLE -> PREC_STACK_SYM_HANDLE
        case PREC_STACK_NT_HANDLE:
            *symbol = PREC_STACK_SYM_HANDLE;
            break;

        // Defaultní případ: syntaktická chyba
        default:
            Parser_errorWatcher(SET_ERROR_SYNTAX);
            break;
    } // switch()
} // PrecStack_mapStackNonTerminalToStackSymbol()

/*** Konec souboru precedence_stack.c ***/
