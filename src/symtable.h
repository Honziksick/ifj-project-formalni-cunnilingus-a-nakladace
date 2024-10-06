/*******************************************************************************
* *
* Název projektu: Implementace překladače imperativního jazyka IFJ24 *
* *
* Soubor: symtable.h *
* Autor: Krejčí David <xkrejcd00> *
* *
* Datum: 01.10.2024 *
* Poslední změna: 06.10.2024 *
* *
* Tým: Tým xkalinj00 *
* Členové: Farkašovský Lukáš <xfarkal00> *
* Hýža Pavel <xhyzapa00> *
* Kalina Jan <xkalinj00> *
* Krejčí David <xkrejcd00> *
* *
******************************************************************************/
/**
* @file symtable.h
* @author Krejčí David \<xkrejcd00>
*
* @brief Hlavičkový soubor pro implementaci tabulky symbolů.
* @details Tento soubor obsahuje deklarace funkcí a datových typů pro práci s 
*          tabulkou symbolů, která je implementována jako hashovací tabulka s
*          implicitním řetězením.
* 
*/


#ifndef SYMTABLE_H__
#define SYMTABLE_H__

#include <stdbool.h>
#include <stdlib.h>


// Maximální naplnění tabulky před rozšířením
// Jakmile je tabulka naplněna na 60%, je rozšířena při dalším vložení
#define MAX_FULLNESS_BEFORE_EXPAND 0.6

// Počáteční velikost tabulky symbolů při jejím vytvoření
#define TABLE_INIT_SIZE 10


/**
 * @brief   Výčet stavů symbolů v tabulce
 * @details Tento výčet popisuje různé typy nebo stavy symbolů,
 *          které lze položce v tabulce symbolů přiřadit
 */
typedef enum {
    SYMTABLE_SYMBOL_EMPTY,           /**< Prázdná položka v tabulce */
    SYMTABLE_SYMBOL_DEAD,            /**< Položka byla odstraněna */
    SYMTABLE_SYMBOL_VARIABLE_INT,    /**< Proměnná typu int */
    SYMTABLE_SYMBOL_VARIABLE_DOUBLE, /**< Proměnná typu double */
    SYMTABLE_SYMBOL_VARIABLE_STRING, /**< Proměnná typu string */
    SYMTABLE_SYMBOL_FUNCTION,        /**< Funkce */
    SYMTABLE_SYMBOL_UNKNOWN          /**< Neznámý typ symbolu */
} symtable_symbolState;

/**
 * @brief   Výčet návratových hodnot funkcí tabulky symbolů
 * @details Tento výčet definuje různé návratové hodnoty,
 *          které mohou některé funkce tabulky symbolů vrátit
 *          (například při přidání nebo vyhledání položek)
 */
typedef enum {
    SYMTABLE_SUCCESS,                /**< Operace proběhla úspěšně */
    SYMTABLE_ALLOCATION_FAIL,        /**< Selhala alokace paměti */
    SYMTABLE_ITEM_ALREADY_EXISTS,    /**< Přidávaná položka již existuje */
    SYMTABLE_ITEM_DOESNT_EXIST,      /**< Hledaná položka není v tabulce */
    SYMTABLE_TABLE_NULL,             /**< Předaná tabulka je NULL */
    SYMTABLE_RESIZE_FAIL             /**< Chyba při zvětšení tabulky */
} symtable_result;


/******************************************************************************
*   Veřejné struktury
*
******************************************************************************/

/**
 * @brief   Struktura položky v tabulce symbolů
 * @details Každá položka obsahuje klíč (řetězec), stav symbolu a ukazatel na 
 *          dodatečná data spojená s položkou 
 *          (např. hodnotu proměnné nebo signatura funkce)
 */
typedef struct {
    string *key;                    /**< Klíč položky (identifikátor) */
    unsigned char symbol_state;     /**< Stav symbolu dle výčtu symtable_symbol_state */
    void *data;                     /**< Ukazatel na data asociovaná s položkou */
} symtable_item;

/**
 * @brief   Struktura hashovací tabulky
 * @details Tato struktura představuje hashovací tabulku pro ukládání symbolů.
 *          Obsahuje pole položek, jehož velikost se dynamicky upravuje
 *          při přidávání nových položek,
 *          když je zaplněno více než MAX_FULLNESS_BEFORE_EXPAND
 */
typedef struct {
    size_t allocated_size;          /**< Velikost alokovaného pole položek */
    size_t used_size;               /**< Počet použitých položek včetně mrtvých*/
    symtable_item array[];          /**< Pole položek */
} symtable;


/******************************************************************************
*   Funkce určené pro veřejné využití
*
******************************************************************************/


/**
 * @brief Inicializuje novou tabulku symbolů
 * 
 * @details Tato funkce alokuje paměť pro novou tabulku symbolů a inicializuje
 *          všechny její položky na prázdné
 * @return Ukazatel na nově vytvořenou tabulku symbolů, nebo NULL v případě chyby
 */
symtable *symtable_init();

/**
 * @brief Přidá novou položku do tabulky symbolů
 * 
 * @details Tato funkce přidá novou položku do tabulky symbolů.
 *          Pokud je tabulka plná, zvětší ji na dvojnásobek aktuální velikosti.
 *          Přidaná položka je vracena v parametru out_item.
 *          Pokud je out_item NULL, položka není vrácena.
 *          Pokud položka již existuje, je předán ukazatel na existující položku.
 * 
 * @param table Ukazatel na tabulku symbolů
 * @param key Klíč nové položky
 * @param out_item Ukazatel na přidanou položku (výstupní parametr)
 * @return  SYMTABLE_SUCCESS při úspěchu.
 *          SYMTABLE_ITEM_ALREADY_EXISTS,
 *          pokud položka se zadaným klíčem již v tabulce existuje.
 *          SYMTABLE_NULL, pokud byla předaná tabulka NULL.
 *          SYMTABLE_RESIZE_FAIL, pokud selhalo rozšíření tabulky.
 */
symtable_result symtable_addItem(symtable *table, string *key, symtable_item *out_item);

/**
 * @brief Vyhledá položku v tabulce symbolů
 * 
 * @details Vyhledá položku v tabulce podle zadaného klíče.
 *          Používá lineární vyhledávání pro řešení kolizí.
 *          Nalezená položka je vracena v parametru out_item.
 *          Pokud je out_item NULL, položka není vrácena.
 * 
 * @param table Ukazatel na tabulku symbolů
 * @param key Klíč položky, kterou chceme vyhledat
 * @param out_item Ukazatel na nalezenou položku (výstupní parametr)
 * @return  SYMTABLE_SUCCESS při úspěchu.
 *          SYMTABLE_ITEM_DOESNT_EXIST,
 *          pokud se položka s daným klíčem v tabulce nenachází.
 *          SYMTABLE_NULL, pokud byla předaná tabulka NULL
 */
symtable_result symtable_findItem(symtable *table, string *key, symtable_item *out_item);

/**
 * @brief Odstraní položku z tabulky symbolů
 * 
 * @details Odstraní položku z tabulky podle zadaného klíče.
 *          Položka je označena jako mrtvá, a její data jsou uvolněna.
 * 
 * @param table Ukazatel na tabulku symbolů
 * @param key Klíč položky, kterou chceme odstranit
 * @return  SYMTABLE_SUCCESS při úspěchu.
 *          SYMTABLE_ITEM_DOESNT_EXIST,
 *          pokud se položka s daným klíčem v tabulce nenachází.
 *          SYMTABLE_NULL, pokud byla předaná tabulka NULL
 */
symtable_result symtable_deleteItem(symtable *table, string *key);

/**
 * @brief Vymaže všechny položky z tabulky symbolů
 * 
 * @details Vymaže všechny položky z tabulky symbolů a uvolní paměť položek.
 *
 * @param table Ukazatel na tabulku symbolů
 */
void symtable_deleteAll(symtable *table);

/**
 * @brief Zničí tabulku symbolů a uvolní její paměť
 * 
 * @details Uvolní paměť alokovanou pro tabulku symbolů a všechny její položky.
 * 
 * @param table Ukazatel na tabulku symbolů
 */
void symtable_destroyTable(symtable *table);


/*******************************************************************************
*   Funkce určené pro privátní využití
*   (neměli by být použity mimo implementaci tabulky)
*
******************************************************************************/


/**
 * @brief Hashovací funkce pro výpočet hashe z klíče
 * 
 * @details Djb2 hashovací funkce
 * 
 * @param key Ukazatel na klíč (řetězec)
 * @return Hash klíče
 */
size_t symtable_hashFunction(string *key);

/**
 * @brief Přesun dat z jedné tabulky do druhé (používá se při rozšiřování)
 * 
 * @details Přesune všechny položky z jedné tabulky do druhé.
 *          Položky jsou naindexovány znovu podle nové velikosti tabulky.
 * 
 * @param table_out_array Zdrojová tabulka
 * @param table_in_array Cílová tabulka
 * @return True, pokud byl přesun úspěšný, jinak false.
 */
bool symtable_transfer(symtable *table_out_array, symtable *table_in_array);

/**
 * @brief Zvětší tabulku symbolů na novou velikost
 * 
 * @param table Ukazatel na aktuální tabulku symbolů
 * @param size Nová velikost tabulky
 * @return Ukazatel na novou tabulku, nebo NULL v případě chyby.
 */
symtable *symtable_resize(symtable *table, size_t size);

/**
 * @brief Alokuje paměť pro novou tabulku symbolů
 * @param size Velikost nové tabulky
 * @return Ukazatel na novou tabulku, nebo NULL v případě chyby
 */
symtable *symtable_allocate(size_t size);


#endif  // SYMTABLE_H__