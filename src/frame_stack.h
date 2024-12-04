/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           frame_stack.h                                             *
 * Autor:            Jan Kalina   <xkalinj00> (návrh, hlavičkový soubor)       *
 *                   David Krejčí <xkrejcd00> (návrh)                          *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   29.11.2024                                                *
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

// Import standardních knihoven jazyka C
#include <stdlib.h>
#include <stdbool.h>

// Import knihoven pro tabulku symbolů
#include "symtable.h"

// Import sdílených knihoven překladače
#include "dynamic_string.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define FRAME_ARRAY_INIT_SIZE 10        /**< Počáteční velikost pole rámců. */
#define FRAME_ARRAY_EXPAND_FACTOR 2     /**< Při rozšíření pole rámců se velikost pole násobí tímto faktorem. */

#define IS_CONST true           /**< Přidáváme do tabulky symbolů konstantní proměnnou.       */
#define IS_VAR   false          /**< Přidáváme do tabulky symbolů modifikovatelnou proměnnou. */

#define IS_FUNCTION  true       /**< Vkládáme rámec označený jako rámec funkce.       */
#define NOT_FUNCTION false      /**< Vkládáme rámec, který je podřízený rámci funkce. */

/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief   Výčet návratových hodnot funkcí zásobníku rámců
 * @details Tento výčet definuje různé návratové hodnoty,
 *          které mohou některé funkce zásobníku rámců vrátit
 *          (například při přidání nebo vyhledání položek).
 */
typedef enum {
    FRAME_STACK_SUCCESS,                /**< Operace proběhla úspěšně       */
    FRAME_STACK_ALLOCATION_FAIL,        /**< Selhala alokace paměti         */
    FRAME_STACK_ITEM_ALREADY_EXISTS,    /**< Přidávaná položka již existuje */
    FRAME_STACK_ITEM_DOESNT_EXIST,      /**< Hledaná položka není v rozsahu */
    FRAME_STACK_NOT_INITIALIZED,        /**< Zásobník rámců není inicializován */
    FRAME_STACK_KEY_NULL,               /**< Předaný klíč je NULL           */
    FRAME_STACK_POP_GLOBAL              /**< Pokus o odstranění globálního rámce */
} FrameStack_result;


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
    FramePtr top;            /**< Ukazatel na vrchol zásobníku rámců.       */
    FramePtr bottom;         /**< Ukazatel na spodní část zásobníku rámců.  */
    size_t currentID;        /**< Aktuální ID nejvyššího rámce.             */
} FrameStack, *FrameStackPtr;

/**
 * @brief Struktura představující nafukovací pole rámců.
 */
typedef struct FrameArray {
    size_t allocated;       /**< Velikost alokovaného pole.         */
    FramePtr *array;        /**< Ukazatel na první položku pole.    */
} FrameArray;


/*******************************************************************************
 *                                                                             *
 *                       DEKLARACE GLOBÁLNÍCH PROMĚNNÝCH                       *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Globální zásobník rámců používaný v celém překladači.
 */
extern FrameStack frameStack;

/**
 * @brief Globální pole všech vytvořených rámců.
 */
extern FrameArray frameArray;


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
 * @details Odstraní vrcholový rámec ze zásobníku, ale ponechá jej v poli.
 */
FrameStack_result frameStack_pop();

/**
 * @brief Vyhledá položku v zásobníku rámců podle klíče.
 *
 * @details Prohledává rámce od vrcholu směrem dolů a hledá položku s daným
 *          klíčem. Hledání se zastaví, pokud je v rámci nastaven @c searchStop
 *          na @c true.
 *
 * @param [in]  key Klíč hledané položky.
 * @param [out] outItem Ukazatel pro uložení nalezené položky.
 * @return - @c FRAME_STACK_SUCCESS při úspěchu.
 *         - @c FRAME_STACK_ITEM_DOESNT_EXIST pokud se položka s daným klíčem
 *           nenachází v rozsahu platnosti.
 *         - @c FRAME_STACK_NOT_INITIALIZED pokud není inicializován globální
 *           zásobník rámců
 *         - @c FRAME_STACK_ALLOCATION_FAIL pokud selhalo alokování paměti
 *         - @c FRAME_STACK_KEY_NULL pokud byl předán klíč NULL
 */
FrameStack_result frameStack_findItem(DString *key, SymtableItem **outItem);

/**
 * @brief Přidá novou položku do vrchního rámce zásobníku.
 *
 * @details Přidá novou položku do tabulky symbolů ve vrcholovém rámci zásobníku.
 *
 * @param [in]  key Klíč nové položky.
 * @param [out] outItem Ukazatel pro uložení přidané položky nebo již
 *                       existující položky nebo NULL,
 *                       pokud není vrácení potřeba
 * @return - @c FRAME_STACK_SUCCESS při úspěchu.
 *         - @c FRAME_STACK_ITEM_ALREADY_EXISTS pokud už položka s daným klíčem
 *           v rozsahu platnosti existuje.
 *         - @c FRAME_STACK_NOT_INITIALIZED pokud není inicializován globální
 *           zásobník rámců
 *         - @c FRAME_STACK_ALLOCATION_FAIL pokud selhalo alokování paměti
 *         - @c FRAME_STACK_KEY_NULL pokud byl předán klíč NULL
 */
FrameStack_result frameStack_addItem(DString *key, SymtableItem **outItem);

/**
 * @brief Přidá novou položku s daty do vrchního rámce zásobníku.
 *
 * @details Přidá novou položku do tabulky symbolů ve vrcholovém rámci zásobníku
 *          a nastaví její data.
 *
 * @param [in] key Klíč nové položky
 * @param [in] state Stav symbolu
 * @param [in] constant Příznak, zda je položka konstantní
 * @param [in] data Ukazatel na data, která se mají uložit
 * @param [in] outItem Ukazatel, kam se má vrátit přidaná položka (výstupní parametr)
 *                  Pokud je NULL, položka není vrácena.
 *
 * @return - @c FRAME_STACK_SUCCESS při úspěchu.
 *         - @c FRAME_STACK_ITEM_ALREADY_EXISTS pokud už položka s daným klíčem
 *              v rozsahu platnosti existuje.
 *         - @c FRAME_STACK_NOT_INITIALIZED pokud není inicializován globální
 *              zásobník rámců
 *         - @c FRAME_STACK_ALLOCATION_FAIL pokud selhalo alokování paměti
 *         - @c FRAME_STACK_KEY_NULL pokud byl předán klíč NULL
 */
FrameStack_result frameStack_addItemExpress(DString *key,
                    Symtable_symbolState state, bool constant, void* data, SymtableItem **outItem);

/**
 * @brief Vrátí ID rámce, ve kterém je položka s daným klíčem.
 *
 * @details Volá symtable_findItem na rámce od vrcholu dolů dokud nenajde položku.
 *
 * @param [in] key Klíč položky
 *
 * @return ID rámce, ve kterém je položka s daným klíčem
 *         nebo 0 pokud se položku nepodařilo najít.
 */
size_t frameStack_getId(DString *key);

/**
 * @brief Uvolní všechny rámce v zásobníku a uvede zásobník do počátečního stavu.
 *
 * @details Tato funkce uvolní všechny rámce uložené v zásobníku, včetně
 *          uvolnění paměti pro tabulky symbolů a jednotlivé rámce. Poté
 *          nastaví zásobník a pole rámců do jejich počátečního stavu.
 *          Pokud zásobník není inicializován (je prázdný), funkce se
 *          okamžitě vrátí.
 */
void frameStack_destroyAll();

/**
 * @brief Vytiskne obsah zásobníku pole rámců
 *
 * @details Tato funkce vytiskne obsah pole rámců podle parametrů
 * @param [in] file Ukazatel na soubor, kam se má tisknout
 * @param [in] print_data Pokud je `true`, vytisknou se i data položek jinak pouze klíče a indexy
 * @param [in] cut_data Pokud je `true`, data mohou být oříznuta aby se vešla do sloupců
 */
void frameStack_printArray(FILE *file, bool print_data, bool cut_data);

/**
 * @brief Vytiskne obsah zásobníku rámců
 *
 * @details Tato funkce vytiskne obsah zásobníku rámců podle parametrů
 * @param [in] file Ukazatel na soubor, kam se má tisknout
 * @param [in] print_data Pokud je `true`, vytisknou se i data položek jinak pouze klíče a indexy
 * @param [in] cut_data Pokud je `true`, data mohou být oříznuta aby se vešla do sloupců
 */
void frameStack_print(FILE *file, bool print_data, bool cut_data);

/**
 * @brief Vytiskne obsah zásobníku rámců
 *
 * @details Volá funkci frameStack_print s parametry stdout, print_data = false, cut_data = true
 *          Vytiskne obsah zásobníku rámců na stdout bez dat,
 *          a s ořezaním dat, aby byly krásně ve sloupcích.
 */
void frameStack_printSimple();

/**
 * @brief Přidá vestavěné funkce do zásobníku rámců.
 */
void frameStack_addEmbeddedFunctions();

/**
 * @brief Přidá funkci do zásobníku rámců.
 *
 * @param [in] key Klíč funkce
 * @param [in] data Ukazatel na data funkce
 */
FrameStack_result frameStack_addFunction(const char* key, void* data);

#endif  // FRAME_STACK_H_

/*** Konec souboru frame_stack.h ***/
