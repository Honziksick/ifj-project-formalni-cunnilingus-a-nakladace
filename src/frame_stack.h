/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           frame_stack.h                                             *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   30.10.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file frame_stack.h
 * @author Jan Kalina \<xkalinj00> (návrh, hlavičkový soubor)
 * @author David Krejčí \<xkrejcd00> (návrh)
 *
 * @brief Hlavičkový soubor pro zásobník rámců.
 * @details Tento hlavičkový soubor obsahuje deklarace funkcí a datových struktur
 *          potřebných pro správu zásobníku rámců v překladači. Zásobník rámců
 *          je používán pro správu tabulek symbolů a jejich rámců během analýzy
 *          a generování kódu. Funkce deklarovan0 v tomto souboru umožňují
 *          inicializaci, manipulaci a vyhledávání v zásobníku rámců.
 */

#ifndef FRAME_STACK_H_
/** @cond  */
#define FRAME_STACK_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"
#include "dynamic_string.h"

/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální zásobník rámců používaný v celém překladači.
 */
extern FrameStack *frameStack;


/*******************************************************************************
 *                                                                             *
 *                             DEKLARACE STRUKTUR                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Struktura představující jeden rámec v zásobníku rámců.
 */
typedef struct Frame {
    SymtablePtr frame;       /**< Ukazatel na tabulku symbolů spojenou s rámcem. */
    size_t frameID;          /**< Unikátní identifikátor rámce. */
    bool searchStop;         /**< Příznak indikující zastavení hledání v tomto rámci (používá se pro funkce). */
    struct Frame *next;      /**< Ukazatel na další rámec v zásobníku. */
} Frame, *FramePtr;

/**
 * @brief Struktura představující zásobník rámců.
 */
typedef struct FrameStack {
    FramePtr top;            /**< Ukazatel na vrchol zásobníku rámců. */
    FramePtr bottom;         /**< Ukazatel na spodní část zásobníku rámců. */
    size_t currentID;        /**< Aktuální ID nejvyššího rámce. */
} FrameStack, *FrameStackPtr;


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje zásobník rámců.
 *
 * @details Tato funkce inicializuje globální zásobník rámců. Nastaví jeho
 *          vrchol a spodní část na nový rámec, který bude reprezentovat
 *          globální rámec. Aktuální ID je nastaveno na 0.
 */
void frameStack_init();

/**
 * @brief Vloží nový rámec na zásobník.
 *
 * @details Vytvoří novou tabulku symbolů a vloží ji na zásobník.
 *          Pokud se jedná o rámec funkce, nastaví @c isFunction na @c true.
 *
 * @param [in] isFunction Indikuje, zda je nový rámec funkcí.
 */
void frameStack_push(bool isFunction);

/**
 * @brief Odstraní vrchní rámec ze zásobníku.
 *
 * @details Odstraní vrcholový rámec ze zásobníku a uvolní s ním spojené zdroje.
 */
void frameStack_pop();

/**
 * @brief Vyhledá položku v zásobníku rámců podle klíče.
 *
 * @details Prohledává rámce od vrcholu směrem dolů a hledá položku s daným
 *          klíčem. Hledání se zastaví, pokud je v rámci nastaven @c searchStop
 *          na @c true.
 *
 * @param [in] key Klíč hledané položky.
 * @param [out] result Ukazatel pro uložení nalezené položky.
 * @return @c True pokud byla položka nalezena, jinak @c false.
 */
bool frameStack_findItem(DString key, SymtableItem *result);

/**
 * @brief Přidá novou položku do vrchního rámce zásobníku.
 *
 * @details Přidá novou položku do tabulky symbolů ve vrcholovém rámci zásobníku.
 *
 * @param [in] key Klíč nové položky.
 * @param [in] item Položka, která má být přidána.
 * @return @c True pokud byla položka úspěšně přidána, jinak @c false.
 */
bool frameStack_addSymtableItem(DString key, SymtableItem item);

/**
 * @brief Získá vrcholový rámec zásobníku.
 *
 * @return Ukazatel na vrcholový rámec.
 */
Frame *frameStack_top();

/**
 * @brief Kontroluje, zda je zásobník rámců prázdný.
 *
 * @return @c True, pokud je zásobník prázdný, jinak @c false.
 */
bool frameStack_isEmpty();

#endif  // FRAME_STACK_H_

/*** Konec souboru frame_stack.h ***/
