/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable.h                                                *
 * Autor:            David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            01.10.2024                                                *
 * Poslední změna:   9.11.2024                                                 *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
* @file symtable.h
* @author Krejčí David \<xkrejcd00>
*
* @brief Hlavičkový soubor pro implementaci tabulky symbolů.
* @details Tento soubor obsahuje deklarace funkcí a datových typů pro práci s
*          tabulkou symbolů, která je implementována jako hashovací tabulka s
*          implicitním řetězením.
*/

#ifndef SYMTABLE_H_
/** @cond */
#define SYMTABLE_H_
/** @endcond */

#include <stdbool.h>
#include <stdlib.h>
#include "dynamic_string.h"
#include "scanner.h"


/**
 * @brief   Maximální naplnění tabulky před rozšířením
 * @details Jakmile je tabulka naplněna na 60 %, je rozšířena při dalším vložení.
 */
#define MAX_FULLNESS_BEFORE_EXPAND 60

/**
 * @brief Počáteční velikost tabulky symbolů při jejím vytvoření
 */
#define TABLE_INIT_SIZE 10


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief   Výčet stavů symbolů v tabulce
 * @details Tento výčet popisuje různé typy nebo stavy symbolů,
 *          které lze položce v tabulce symbolů přiřadit.
 */
typedef enum {
    SYMTABLE_SYMBOL_EMPTY,                      /**< Prázdná položka v tabulce */
    SYMTABLE_SYMBOL_DEAD,                       /**< Položka byla odstraněna */
    SYMTABLE_SYMBOL_VARIABLE_INT,               /**< Proměnná typu int */
    SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL,       /**< Proměnná typu int nebo NULL */
    SYMTABLE_SYMBOL_VARIABLE_DOUBLE,            /**< Proměnná typu double */
    SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL,    /**< Proměnná typu double nebo NULL */
    SYMTABLE_SYMBOL_VARIABLE_STRING,            /**< Proměnná typu string */
    SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL,    /**< Proměnná typu string nebo NULL */
    SYMTABLE_SYMBOL_FUNCTION,                   /**< Funkce */
    SYMTABLE_SYMBOL_UNKNOWN                     /**< Neznámý typ symbolu */
} symtable_symbolState;

/**
 * @brief   Výčet návratových hodnot funkcí tabulky symbolů
 * @details Tento výčet definuje různé návratové hodnoty,
 *          které mohou některé funkce tabulky symbolů vrátit
 *          (například při přidání nebo vyhledání položek).
 */
typedef enum {
    SYMTABLE_SUCCESS = 0,            /**< Operace proběhla úspěšně */
    SYMTABLE_ALLOCATION_FAIL,        /**< Selhala alokace paměti */
    SYMTABLE_ITEM_ALREADY_EXISTS,    /**< Přidávaná položka již existuje */
    SYMTABLE_ITEM_DOESNT_EXIST,      /**< Hledaná položka není v tabulce */
    SYMTABLE_TABLE_NULL,             /**< Předaná tabulka je NULL */
    SYMTABLE_RESIZE_FAIL,            /**< Chyba při zvětšení tabulky */
    SYMTABLE_KEY_NULL                /**< Předaný klíč je NULL */
} symtable_result;


typedef enum {
    SYMTABLE_TYPE_UNKNOWN = 0,      /**< Neznámý typ */
    SYMTABLE_TYPE_INT,              /**< Proměnná typu int */
    SYMTABLE_TYPE_DOUBLE,           /**< Proměnná typu double */
    SYMTABLE_TYPE_STRING,           /**< Proměnná typu string */
    SYMTABLE_TYPE_INT_OR_NULL,      /**< Proměnná typu int nebo NULL */
    SYMTABLE_TYPE_DOUBLE_OR_NULL,   /**< Proměnná typu double nebo NULL */
    SYMTABLE_TYPE_STRING_OR_NULL,   /**< Proměnná typu string nebo NULL */
    SYMTABLE_TYPE_VOID              /**< Funkce nic nevrací */
} symtable_functionReturnType;


/*******************************************************************************
 *                                                                             *
 *                              VEŘEJNÉ STRUKTURY                              *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief   Struktura položky v tabulce symbolů
 * @details Každá položka obsahuje klíč (řetězec), stav symbolu a ukazatel na
 *          dodatečná data spojená s položkou (např. hodnotu proměnné nebo
 *          signatura funkce).
 */
typedef struct {
    DString *key;                       /**< Klíč položky (identifikátor) */
    symtable_symbolState symbol_state;  /**< Stav symbolu dle výčtu `symtable_symbol_state` */
    bool used;                          /**< Příznak, zda je položka použita */
    bool defined;                       /**< Příznak, zda je položka definovaná */
    bool constant;                      /**< Příznak, zda je položka konstantní */
    bool changed;                       /**< Příznak, zda byla hodnota položky změněna */
    void *data;                         /**< Ukazatel na data asociovaná s položkou */
} SymtableItem, *SymtableItemPtr;

/**
 * @brief Struktura parametru funkce
 */
typedef struct {
    DString *id;                        /**< Identifikátor parametru */
    symtable_functionReturnType type;   /**< Typ parametru */
} SymtableParamPair;

/**
 * @brief Struktura dat funkce
 * @details Tato struktura obsahuje návratový typ funkce, počet parametrů
 *          a pole s jednotlivými parametry.
 */
typedef struct {
    symtable_functionReturnType return_type;    /**< Návratový typ funkce */
    size_t param_count;                         /**< Počet parametrů funkce */
    SymtableParamPair *params;                  /**< Pole parametrů funkce */
} SymtableFunctionData;



/**
 * @brief   Struktura hashovací tabulky
 * @details Tato struktura představuje hashovací tabulku pro ukládání symbolů.
 *          Obsahuje pole položek, jehož velikost se dynamicky upravuje
 *          při přidávání nových položek, když je zaplněno
 *          více než MAX_FULLNESS_BEFORE_EXPAND.
 */
typedef struct {
    size_t allocated_size;          /**< Velikost alokovaného pole položek */
    size_t used_size;               /**< Počet použitých položek včetně mrtvých*/
    SymtableItem *array;            /**< Pole položek */
} Symtable, *SymtablePtr;


/*******************************************************************************
 *                                                                             *
 *                      FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Inicializuje novou tabulku symbolů
 *
 * @details Tato funkce alokuje paměť pro novou tabulku symbolů a inicializuje
 *          všechny její položky na prázdné
 *
 * @return Ukazatel na nově vytvořenou tabulku symbolů, nebo NULL v případě chyby
 */
Symtable *symtable_init();

/**
 * @brief Přidá novou položku do tabulky symbolů
 *
 * @details Tato funkce přidá novou položku do tabulky symbolů.
 *          Pokud je tabulka plná, zvětší ji na dvojnásobek aktuální velikosti.
 *          Přidaná položka je vracena v parametru `out_item`.
 *          Pokud je `out_item` NULL, položka není vrácena.
 *          Pokud položka již existuje, je předán ukazatel na existující položku.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 * @param [in] key Klíč nové položky
 * @param [in] out_item Ukazatel, kam se má vrátit přidaná položka (výstupní parametr)
 *                  Pokud je NULL, položka není vrácena.
 * @return - @c SYMTABLE_SUCCESS při úspěšném vložení nové položky.
 *         - @c SYMTABLE_ITEM_ALREADY_EXISTS, pokud položka se zadaným klíčem
 *           již v tabulce existuje.
 *         - @c SYMTABLE_NULL, pokud byla předaná tabulka NULL.
 *         - @c SYMTABLE_RESIZE_FAIL, pokud selhalo rozšíření tabulky.
 *         - @c SYMTABLE_ALLOCATION_FAIL, pokud selhala alokace paměti pro nový prvek.
 *         - @c SYMTABLE_KEY_NULL, pokud byl předán klíč NULL.
 */
symtable_result symtable_addItem(Symtable *table, DString *key, SymtableItem **out_item);

/**
 * @brief Přidá novou položku do tabulky symbolů z tokenu
 *
 * @details Tato funkce zkopíruje klíč z tokenu a přidá jej do tabulky symbolů.
 *          Pokud je tabulka plná, zvětší ji na dvojnásobek aktuální velikosti.
 *          Přidaná položka je vracena v parametru `out_item`.
 *          Pokud je `out_item` NULL, položka není vrácena.
 *          Pokud položka již existuje, je předán ukazatel na existující položku.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 * @param [in] token Token nové položky
 * @param [in] out_item Ukazatel na přidanou položku (výstupní parametr)
 * @return - @c SYMTABLE_SUCCESS při úspěšném vložení nové položky.
 *         - @c SYMTABLE_ITEM_ALREADY_EXISTS, pokud položka se zadaným klíčem
 *           již v tabulce existuje.
 *         - @c SYMTABLE_NULL, pokud byla předaná tabulka NULL.
 *         - @c SYMTABLE_RESIZE_FAIL, pokud selhalo rozšíření tabulky.
 */
symtable_result symtable_addToken(Symtable *table, Token token, SymtableItem **out_item);

/**
 * @brief Vyhledá položku v tabulce symbolů
 *
 * @details Vyhledá položku v tabulce podle zadaného klíče.
 *          Používá lineární vyhledávání pro řešení kolizí.
 *          Nalezená položka je vracena v parametru `out_item`.
 *          Pokud je `out_item` NULL, položka není vrácena.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 * @param [in] key Klíč položky, kterou chceme vyhledat
 * @param [in] out_item Ukazatel na nalezenou položku (výstupní parametr)
 * @return - @c SYMTABLE_SUCCESS při úspěchu.
 *         - @c SYMTABLE_ITEM_DOESNT_EXIST, pokud se položka s daným klíčem
 *           v tabulce nenachází.
 *         - @c SYMTABLE_NULL, pokud byla předaná tabulka NULL
 *         - @c SYMTABLE_KEY_NULL, pokud byl předán klíč NULL
 */
symtable_result symtable_findItem(Symtable *table, DString *key, SymtableItem **out_item);

/**
 * @brief Odstraní položku z tabulky symbolů
 *
 * @details Odstraní položku z tabulky podle zadaného klíče.
 *          Položka je označena jako mrtvá, a její data jsou uvolněna.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 * @param [in] key Klíč položky, kterou chceme odstranit
 * @return - @c SYMTABLE_SUCCESS při úspěchu.
 *         - @c SYMTABLE_ITEM_DOESNT_EXIST, pokud se položka s daným klíčem
 *           v tabulce nenachází.
 *         - @c SYMTABLE_NULL, pokud byla předaná tabulka NULL
 *         - @c SYMTABLE_KEY_NULL, pokud byl předán klíč NULL
 */
symtable_result symtable_deleteItem(Symtable *table, DString *key);

/**
 * @brief Vymaže všechny položky z tabulky symbolů
 *
 * @details Vymaže všechny položky z tabulky symbolů a uvolní paměť položek.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 * @param [in] keep_data Pokud je `true`, data položek nebudou uvolněna (true při resize)
 *                  
 */
void symtable_deleteAll(Symtable *table, bool keep_data);

/**
 * @brief Zničí tabulku symbolů a uvolní její paměť
 *
 * @details Uvolní paměť alokovanou pro tabulku symbolů a všechny její položky.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 */
void symtable_destroyTable(Symtable *table);

/**
 * @brief Inicializuje novou strukturu pro data funkce
 * 
 * @details Alokuje místo pro data pro funkci s daným počtem parametrů
 * 
 * @param [in] param_count Počet parametrů funkce
 */
SymtableFunctionData *symtable_init_function_data(size_t param_count);

/**
 * @brief Vytiskne obsah tabulky symbolů
 * 
 * @details Tato funkce vytiskne obsah tabulky symbolů do souboru.
 * @param [in] table Ukazatel na tabulku symbolů
 * @param [in] file Ukazatel na soubor, kam se má tisknout
 * @param [in] print_data Pokud je `true`, vytisknou se i data položek jinak pouze klíče
 * @param [in] cut_data Pokud je `true`, data budou zkrácena, aby se vešla do sloupců
 */
void symtable_print(Symtable *table, FILE *file, bool print_data, bool cut_data);

/*******************************************************************************
 *                                                                             *
 *                     FUNKCE URČENÉ PRO PRIVÁTNÍ VYUŽITÍ                      *
 *          (neměly by být použity mimo implementaci tabulky symbolů)          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Hashovací funkce pro výpočet hashe z klíče
 *
 * @details Djb2 hashovací funkce
 *
 * @param [in] key Ukazatel na klíč (řetězec)
 * @return Hash klíče
 */
size_t symtable_hashFunction(DString *key);

/**
 * @brief Přesun dat z jedné tabulky do druhé
 *
 * @details Přesune všechny položky z jedné tabulky do druhé.
 *          Položky jsou naindexovány znovu podle nové velikosti tabulky.
 * @note Funkce se využívá při rozšiřování
 *
 * @param [in] table_out_array Zdrojová tabulka
 * @param [in] table_in_array Cílová tabulka
 * @return @c TRUE, pokud byl přesun úspěšný, jinak @c FALSE.
 */
bool symtable_transfer(Symtable *table_out_array, Symtable *table_in_array);

/**
 * @brief Zvětší tabulku symbolů na novou velikost
 *
 * @param [in] table Ukazatel na aktuální tabulku symbolů
 * @param [in] size Nová velikost tabulky
 * @return Ukazatel na novou tabulku, nebo `NULL` v případě chyby.
 */
Symtable *symtable_resize(Symtable *table, size_t size);

/**
 * @brief Alokuje paměť pro pole položek tabulky symbolů
 * @param [in] size Velikost pole
 * @return Ukazatel na novou tabulku, nebo `NULL` v případě chyby
 */
SymtableItemPtr symtable_init_items(size_t size);

#endif  // SYMTABLE_H_

/*** Konec souboru symtable.h ***/
