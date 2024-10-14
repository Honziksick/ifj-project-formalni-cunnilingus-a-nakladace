/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable.c                                                *
 * Autor:            David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            01.10.2024                                                *
 * Poslední změna:   14.10.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
* @file symtable.c
* @author Krejčí David \<xkrejcd00>
*
* @brief Implementace tabulky symbolů.
* @details Tento soubor obsahuje implementaci funkcí pro práci s tabulkou symbolů,
*          která je implementována jako hashovací tabulka s implicitním řetězením.
*/

#include <stdbool.h>
#include <stdlib.h>
#include "dynamic_string.h"
#include "symtable.h"


/**
 * @brief Inicializuje novou tabulku symbolů
*/
symtable *symtable_init() {
    // Vytvoříme novou tabulku symbolů
    symtable *table = symtable_allocate(TABLE_INIT_SIZE);
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(table == NULL) {
        return NULL;
    }

    // Inicializujeme velikost tabulky
    table->allocated_size = TABLE_INIT_SIZE;
    table->used_size = 0;
    // Inicializujeme všechny položky v tabulce
    for(size_t i = 0; i < table->allocated_size; i++) {
        table->array[i].key = NULL;
        table->array[i].symbol_state = SYMTABLE_SYMBOL_EMPTY;
        table->array[i].data = NULL;
    }
    return table;
}

/**
 * @brief Přidá novou položku do tabulky symbolů
*/
symtable_result symtable_addItem(symtable *table, DString *key, symtable_item *out_item) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Vypočítáme aktuální zaplněnost tabulky
    double current_fullness = (double)table->used_size / (double)table->allocated_size;
    // Pokud je tabulka plná, musíme ji rozšířit
    if(current_fullness > MAX_FULLNESS_BEFORE_EXPAND) {
        table = symtable_resize(table, table->allocated_size * 2);
        // Pokud se nepodařilo rozšířit tabulku, vracíme SYMTABLE_RESIZE_FAIL
        if(table == NULL) {
            return SYMTABLE_RESIZE_FAIL;
        }
    }

    // Vypočítáme index, na kterém by se měla hledaná položka nacházet
    size_t index = symtable_hashFunction(key) % table->allocated_size;

    // Procházíme tabulku dokud nenarazíme na prázdnou položku
    while(1) {
        symtable_item *item = &table->array[index];
        // Pokud je nalezeno prázdné místo, vkládáme novou položku
        if(item->symbol_state == SYMTABLE_SYMBOL_EMPTY ||
           item->symbol_state == SYMTABLE_SYMBOL_DEAD) {

            // Přidáme novou položku
            item->key = key;
            item->data = NULL;
            item->symbol_state = SYMTABLE_SYMBOL_UNKNOWN;
            // Inkrementujeme počet použitých položek
            if(item->symbol_state == SYMTABLE_SYMBOL_EMPTY) {
                table->used_size++;
            }
            // Pokud je požadován odkaz na novou položku, vrátíme ho
            if(out_item != NULL) {
                *out_item = *item;
            }
            return SYMTABLE_SUCCESS;
        }

        // Pokud položka s daným klíčem již existuje a je živá
        if(string_compare(item->key,key) == 0 &&
           item->symbol_state != SYMTABLE_SYMBOL_DEAD) {

            // Vrátíme odkaz na existující položku a
            if(out_item != NULL) {
                *out_item = *item;
            }
            return SYMTABLE_ITEM_ALREADY_EXISTS;
        }
        // Jinak pokračujeme v hledání
        index = (index+1) % table->allocated_size;
    }
}

/**
 * @brief Vyhledá položku v tabulce symbolů
*/
symtable_result symtable_findItem(symtable *table, DString *searched_key, symtable_item *out_item) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Vypočítáme index, na kterém by se měla hledaná položka nacházet
    size_t index = symtable_hashFunction(searched_key) % table->allocated_size;

    // Vezmeme položku na indexu
    symtable_item item = table->array[index];
    // Inkrement pro hledání
    size_t i = 0;
    // Procházíme položky dokud nenarazíme na prázdnou položku
    while(item.symbol_state != SYMTABLE_SYMBOL_EMPTY) {

        // Pokud je nalezena položka se stejným klíčem, vracíme ji
        if(string_compare(item.key,searched_key) == 0) {
            if(out_item != NULL) {
                *out_item = item;
            }
            return SYMTABLE_SUCCESS;
        }
        // Jinak pokračujeme v hledání
        i++;
        size_t new_index = (index+i) % table->allocated_size;
        // Pokud jsme prošli celou tabulku, vracíme SYMTABLE_ITEM_DOESNT_EXIST
        if(new_index == index) {
            return SYMTABLE_ITEM_DOESNT_EXIST;
        }
        item = table->array[new_index];
    }
    return SYMTABLE_ITEM_DOESNT_EXIST;
}

/**
 * @brief Odstraní položku z tabulky symbolů
*/
symtable_result symtable_deleteItem(symtable *table, DString *key) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Pokusíme se najít položku
    symtable_item item;
    symtable_result find_result = symtable_findItem(table,key,&item);

    // Při neúspěchu vracíme návratový kód
    if(find_result != SYMTABLE_SUCCESS) {
        return find_result;
    }

    // Při úspěchu položku odstraníme
    item.symbol_state = SYMTABLE_SYMBOL_DEAD;
    string_free(item.key);
    item.key = NULL;
    free(item.data);
    item.data = NULL;
    return SYMTABLE_SUCCESS;
}

/**
 * @brief Vymaže všechny položky z tabulky symbolů
*/
void symtable_deleteAll(symtable *table) {
    // Pokud je tabulka NULL, nic se neprovede
    if(table == NULL) {
        return;
    }

    // Procházíme všechny položky v tabulce
    for(size_t i = 0; i < table->allocated_size; i++) {
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        symtable_item item = table->array[i];
        if(item.symbol_state != SYMTABLE_SYMBOL_EMPTY &&
           item.symbol_state != SYMTABLE_SYMBOL_DEAD) {

            // Pokud je položka živá, odstraníme ji
            string_free(item.key);
            item.key = NULL;
            free(item.data);
            item.data = NULL;
        }
    }
}

/**
 * @brief Zničí tabulku symbolů a uvolní její paměť
*/
inline void symtable_destroyTable(symtable *table) {
    // Pokud je tabulka NULL, nic se neprovede
    if(table == NULL) {
        return;
    }
    // Odstraníme všechny položky
    symtable_deleteAll(table);
    free(table);
}

/**
 * @brief Hashovací funkce pro výpočet hashe z klíče
*/
size_t symtable_hashFunction(DString *key) {
    size_t hash = 5381;
    for(size_t i = 0; i < key->length;i++) {
        hash = ((hash << 5) + hash) + (size_t)key->str[i];
    }
    return hash;
}

/**
 * @brief Přesun dat z jedné tabulky do druhé (používá se při rozšiřování)
*/
bool symtable_transfer(symtable *out_table, symtable *in_table) {
    // Pokud je některá z tabulek NULL, vracíme false (nemělo by nikdy nastat)
    if(out_table == NULL || in_table == NULL) {
        return false;
    }
    // Pokud by se položky nevešly do nové tabulky, vracíme false
    // (nemělo by nikdy nastat)
    if(out_table->used_size > in_table->allocated_size) {
        return false;
    }

    // Procházíme všechny položky v tabulce
    for(size_t i = 0; i < out_table->allocated_size; i++) {
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        if(out_table->array[i].symbol_state == SYMTABLE_SYMBOL_EMPTY ||
           out_table->array[i].symbol_state == SYMTABLE_SYMBOL_DEAD) {
            continue;
        }

        // Vytvoříme novou položku v cílové tabulce
        symtable_item new_location;
        symtable_result result = symtable_addItem(in_table, out_table->array[i].key, &new_location);
        // Pokud se nepodařilo přidat položku, vracíme false
        if(result != SYMTABLE_SUCCESS) {
            return false;
        }
        // Přeneseme data z jedné položky do druhé
        new_location.data = out_table->array[i].data;
        new_location.symbol_state = out_table->array->symbol_state;
    }
    return true;
}

/**
 * @brief Zvětší tabulku symbolů na novou velikost
*/
symtable *symtable_resize(symtable *table, size_t size) {
    // Pokud je tabulka NULL, vracíme NULL (nemělo by nikdy nastat)
    if(table == NULL) {
        return NULL;
    }

    // Vytvoříme novou tabulku
    symtable *new_table = symtable_allocate(size);
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(new_table == NULL) {
        return NULL;
    }

    // Inicializujeme novou tabulku
    new_table->allocated_size = size;
    new_table->used_size = 0;
    // Přeneseme data z původní tabulky do nové
    if(symtable_transfer(table,new_table) == false) {
        return NULL;
    }
    // Uvolníme původní tabulku
    free(table);
    return new_table;
}

/**
 * @brief Alokuje paměť pro novou tabulku symbolů
*/
inline symtable *symtable_allocate(size_t size) {
    return malloc(sizeof(symtable)+size*sizeof(symtable_item));
}

/*** Konec souboru symtable.c ***/
