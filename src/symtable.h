/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable.h                                                *
 * Autor:            David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            01.10.2024                                                *
 * Poslední změna:   15.10.2024                                                *
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
#define MAX_FULLNESS_BEFORE_EXPAND 0.6

/**
 * @brief Počáteční velikost tabulky symbolů při jejím vytvoření
 */
#define TABLE_INIT_SIZE 10


/*******************************************************************************
 *                                                                             *
 *                           VÝČTOVÉ DATOVÉ TYPY                               *
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
    SYMTABLE_SUCCESS,                /**< Operace proběhla úspěšně */
    SYMTABLE_ALLOCATION_FAIL,        /**< Selhala alokace paměti */
    SYMTABLE_ITEM_ALREADY_EXISTS,    /**< Přidávaná položka již existuje */
    SYMTABLE_ITEM_DOESNT_EXIST,      /**< Hledaná položka není v tabulce */
    SYMTABLE_TABLE_NULL,             /**< Předaná tabulka je NULL */
    SYMTABLE_RESIZE_FAIL             /**< Chyba při zvětšení tabulky */
} symtable_result;


/*******************************************************************************
 *                                                                             *
 *                            VEŘEJNÉ STRUKTURY                                *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief   Struktura položky v tabulce symbolů
 * @details Každá položka obsahuje klíč (řetězec), stav symbolu a ukazatel na
 *          dodatečná data spojená s položkou (např. hodnotu proměnné nebo
 *          signatura funkce).
 */
typedef struct {
    DString *key;                   /**< Klíč položky (identifikátor) */
    unsigned char symbol_state;     /**< Stav symbolu dle výčtu `symtable_symbol_state` */
    bool used;                      /**< Příznak, zda je položka použita */
    bool declared;                  /**< Příznak, zda je položka deklarována */
    bool constant;                  /**< Příznak, zda je položka konstantní */
    void *data;                     /**< Ukazatel na data asociovaná s položkou */
} symtable_item;

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
    symtable_item array[];          /**< Pole položek */
} symtable;


/*******************************************************************************
 *                                                                             *
 *                    FUNKCE URČENÉ PRO VEŘEJNÉ VYUŽITÍ                        *
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
symtable *symtable_init();

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
 * @param [in] out_item Ukazatel na přidanou položku (výstupní parametr)
 * @return - @c SYMTABLE_SUCCESS při úspěšném vložení nové položky.
 *         - @c SYMTABLE_ITEM_ALREADY_EXISTS, pokud položka se zadaným klíčem
 *           již v tabulce existuje.
 *         - @c SYMTABLE_NULL, pokud byla předaná tabulka NULL.
 *         - @c SYMTABLE_RESIZE_FAIL, pokud selhalo rozšíření tabulky.
 *         - @c SYMTABLE_ALLOCATION_FAIL, pokud selhala alokace paměti pro nový prvek.
 */
symtable_result symtable_addItem(symtable *table, DString *key, symtable_item *out_item);

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
symtable_result symtable_addToken(symtable *table, Token token, symtable_item *out_item);

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
 */
symtable_result symtable_findItem(symtable *table, DString *key, symtable_item *out_item);

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
 */
symtable_result symtable_deleteItem(symtable *table, DString *key);

/**
 * @brief Vymaže všechny položky z tabulky symbolů
 *
 * @details Vymaže všechny položky z tabulky symbolů a uvolní paměť položek.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 */
void symtable_deleteAll(symtable *table);

/**
 * @brief Zničí tabulku symbolů a uvolní její paměť
 *
 * @details Uvolní paměť alokovanou pro tabulku symbolů a všechny její položky.
 *
 * @param [in] table Ukazatel na tabulku symbolů
 */
void symtable_destroyTable(symtable *table);


/*******************************************************************************
 *                                                                             *
 *                    FUNKCE URČENÉ PRO PRIVÁTNÍ VYUŽITÍ                       *
 *         (neměly by být použity mimo implementaci tabulky symbolů)           *
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
bool symtable_transfer(symtable *table_out_array, symtable *table_in_array);

/**
 * @brief Zvětší tabulku symbolů na novou velikost
 *
 * @param [in] table Ukazatel na aktuální tabulku symbolů
 * @param [in] size Nová velikost tabulky
 * @return Ukazatel na novou tabulku, nebo `NULL` v případě chyby.
 */
symtable *symtable_resize(symtable *table, size_t size);

/**
 * @brief Alokuje paměť pro novou tabulku symbolů
 * @param [in] size Velikost nové tabulky
 * @return Ukazatel na novou tabulku, nebo `NULL` v případě chyby
 */
symtable *symtable_allocate(size_t size);

#endif  // SYMTABLE_H_

/*** Konec souboru symtable.h ***/
