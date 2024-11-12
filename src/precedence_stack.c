/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           precedence_stack.c                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            11.11.2024                                                *
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
 * @file precedence_stack.c
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Implementační soubor pro precedenční zásobník v syntaktické analýze.
 * @details Tento soubor obsahuje implementace funkcí pro správu zásobníku
 *          používaného v precedenční syntaktické analýze zdrojového kódu jazyka
 *          IFJ24.
 */

#include "precedence_stack.h"


/*******************************************************************************
 *                                                                             *
 *                POČÁTEČNÍ INICIALIZACE GLOBÁLNÍCH PROMĚNNÝCH                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální proměnná pro precedenční zásobník.
 */
PrecStack *precStack = NULL;


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Alokuje paměť pro globální precedenční zásobník.
 */
inline void PrecStack_create() {
    // Alokace paměti pro PrecStack
    precStack = (PrecStack *)malloc(sizeof(PrecStack));

    // Kontrola úspěšnosti alokace
    if(precStack == NULL) {
        error_handle(ERROR_INTERNAL);
    }
} // PrecStack_create()

/**
 * @brief Inicializuje globální precedenční zásobník.
 */
void PrecStack_init() {
    // Pokud zásbník existuje nastvíme vrchol na NULL
    if(precStack != NULL) {
        precStack->top = NULL;
    }
} // PrecStack_init()

/**
 * @brief Pushne terminál na globální precedenční zásobník.
 */
void PrecStack_push(PrecStackSymbol symbol, AST_NodeType type, void *node) {
    // Pokud není precStack alokovaný, nic se neděje
    if(precStack == NULL) {
        return;
    }

    PrecStackNode* newStackNode = (PrecStackNode *)malloc(sizeof(PrecStackNode));
    if(newStackNode == NULL) {
        // Ošetření chyby alokace paměti
        return;
    }

    switch(symbol) {
        case PREC_STACK_SYM_HANDLE:
            newStackNode->symbolType = STACK_NODE_TYPE_HANDLE;
            break;
        case PREC_STACK_SYM_EXPRESSION:
        case PREC_STACK_SYM_ARGUMENTS:
        case PREC_STACK_SYM_ARG_LIST:
        case PREC_STACK_SYM_ARG:
            newStackNode->symbolType = STACK_NODE_TYPE_NONTERMINAL;
            break;
        case PREC_STACK_SYM_DOLLAR:
            error_handle(ERROR_INTERNAL);
            break;
        default:
            newStackNode->symbolType = STACK_NODE_TYPE_TERMINAL;
    }

    newStackNode->symbol = symbol;
    newStackNode->nodeType = type;
    newStackNode->node = node;
    newStackNode->next = PrecStack_top();
    precStack->top = newStackNode;
} // PrecStack_push()

/**
 * @brief Popne uzel AST z globálního precedenčního zásobníku.
 */
PrecStackNode* PrecStack_pop() {
    if(precStack->top == NULL) {
        return NULL;
    }
    PrecStackNode* node = precStack->top;
    precStack->top = precStack->top->next;
    return node;
} // PrecStack_pop()

/**
 * @brief Získá ukazatel na uzel na vrcholu globálního precedenčního zásobníku
 *        bez jeho popnutí.
 */
PrecStackNode* PrecStack_top() {
    if(precStack->top == NULL) {
        return NULL;
    }
    return precStack->top;
} // PrecStack_top()

/**
 * @brief Uvolní všechny uzly z globálního precedenčního zásobníku a
 *        inicializuje ho do počátečního stavu.
 */
void PrecStack_dispose() {
    // Kontrola, že zásbník již není prázdný
    if(precStack == NULL) {
        return;
    }

    while(!PrecStack_isEmpty()) {
        // Odložíme si prvek na smazání a jako nový vrchol zásobníku nastavíme následníka
        PrecStackNode* toDelete = precStack->top;
        precStack->top = toDelete->next;

        // Uvolníme uzel AST, pokud je připojen k StackNode
        if(toDelete->node != NULL) {
            AST_destroyNode(toDelete->nodeType, toDelete->node);
        }

        // Uvolníme paměť pro první prvek
        free(toDelete);
    }

    // Uvedeme zásbník do počátečního stavu
    PrecStack_init();
} // PrecStack_dispose()


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Zkontroluje, zda je precedenční zásobník prázdný.
 */
bool PrecStack_isEmpty() {
    return (precStack == NULL || precStack->top == NULL);
} // PrecStack_isEmpty()

/*** Konec souboru precedence_stack.c ***/
