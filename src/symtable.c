/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable.c                                                *
 * Autor:            David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            01.10.2024                                                *
 * Poslední změna:   9.11.2024                                                *
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
SymtablePtr symtable_init() {
    // Vytvoříme novou tabulku symbolů
    SymtablePtr table = malloc(sizeof(Symtable));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(table == NULL) {
        return NULL;
    }

    // Inicializujeme velikost tabulky
    table->allocated_size = TABLE_INIT_SIZE;
    table->used_size = 0;

    // Inicializujeme pole položek
    SymtableItemPtr items = symtable_init_items(TABLE_INIT_SIZE);
    // Pokud se nepodařilo alokovat paměť, uvolníme tabulku a vracíme NULL
    if(items == NULL){
        free(table);
        return NULL;
    }
    // Přiřadíme pole položek do tabulky
    table->array = items;

    return table;
}

/**
 * @brief Přidá novou položku do tabulky symbolů
*/
symtable_result symtable_addItem(Symtable *table, DString *key, SymtableItem **out_item) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    if(key == NULL){
        return SYMTABLE_KEY_NULL;
    }

    // Vypočítáme aktuální zaplněnost tabulky
    size_t current_fullness = (100 * table->used_size) / table->allocated_size;
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
    while(true) {
        SymtableItemPtr item = &table->array[index];
        // Pokud je nalezeno prázdné místo, vkládáme novou položku
        if(item->symbol_state == SYMTABLE_SYMBOL_EMPTY ||
           item->symbol_state == SYMTABLE_SYMBOL_DEAD) {

            // Vytvoříme kopii klíče
            DString *key_copy = string_init();
            // Pokud se nepodařilo alokovat paměť, vracíme SYMTABLE_ALLOCATION_FAIL
            if(key_copy == NULL) {
                return SYMTABLE_ALLOCATION_FAIL;
            }
            // Pokud se nepodařilo zkopírovat klíč,
            // uvolníme alokovanou paměť a vracíme SYMTABLE_ALLOCATION_FAIL
            if(string_copy(key,key_copy) == STRING_COPY_FAIL) {
                string_free(key_copy);
                return SYMTABLE_ALLOCATION_FAIL;
            }

            // Inkrementujeme počet použitých položek
            if(item->symbol_state == SYMTABLE_SYMBOL_EMPTY) {
                table->used_size++;
            }

            // Inicializujeme novou položku
            item->key = key_copy;
            item->data = NULL;
            item->symbol_state = SYMTABLE_SYMBOL_UNKNOWN;
            item->constant = false;
            item->known_value = false;
            item->used = false;
            item->changed = false;

            // Pokud je požadován odkaz na novou položku, vrátíme ho
            if(out_item != NULL) {
                *out_item = item;
            }
            return SYMTABLE_SUCCESS;
        }

        // Pokud položka s daným klíčem již existuje a je živá
        if(string_compare(item->key,key) == STRING_EQUAL &&
           item->symbol_state != SYMTABLE_SYMBOL_DEAD) {

            // Vrátíme odkaz na existující položku a
            if(out_item != NULL) {
                *out_item = item;
            }
            return SYMTABLE_ITEM_ALREADY_EXISTS;
        }
        // Jinak pokračujeme v hledání
        index = (index+1) % table->allocated_size;
    }
}

/**
 * @brief Přidá novou položku do tabulky symbolů z tokenu
*/
symtable_result symtable_addToken(Symtable *table, Token token, SymtableItem **out_item){
    return symtable_addItem(table, token.value, out_item);
}

/**
 * @brief Vyhledá položku v tabulce symbolů
*/
symtable_result symtable_findItem(Symtable *table, DString *searched_key, SymtableItem **out_item) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Pokud je klíč NULL, vracíme SYMTABLE_KEY_NULL
    if(searched_key == NULL){
        return SYMTABLE_KEY_NULL;
    }

    // Vypočítáme index, na kterém by se měla hledaná položka nacházet
    size_t index = symtable_hashFunction(searched_key) % table->allocated_size;

    // Vezmeme položku na indexu
    SymtableItemPtr item = &table->array[index];
    // Inkrement pro hledání
    size_t i = 0;
    // Procházíme položky dokud nenarazíme na prázdnou položku
    while(item->symbol_state != SYMTABLE_SYMBOL_EMPTY) {

        // Pokud je nalezena položka se stejným klíčem, vracíme ji
        if(string_compare(item->key,searched_key) == STRING_EQUAL) {
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
        item = &table->array[new_index];
    }
    return SYMTABLE_ITEM_DOESNT_EXIST;
}

/**
 * @brief Odstraní položku z tabulky symbolů
*/
symtable_result symtable_deleteItem(Symtable *table, DString *key) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Pokud je klíč NULL, vracíme SYMTABLE_KEY_NULL
    if(key == NULL){
        return SYMTABLE_KEY_NULL;
    }

    // Pokusíme se najít položku
    SymtableItemPtr item;
    symtable_result find_result = symtable_findItem(table,key,&item);

    // Při neúspěchu vracíme návratový kód
    if(find_result != SYMTABLE_SUCCESS) {
        return find_result;
    }

    // Při úspěchu položku odstraníme
    if(item->key != NULL){
        string_free(item->key);
        item->key = NULL;
    }
    if(item->data != NULL){
        // Pokud je položka funkce, uvolníme i její parametry
        if(item->symbol_state == SYMTABLE_SYMBOL_FUNCTION){
            SymtableFunctionData *data = (SymtableFunctionData *)item->data;
            for(size_t i = 0; i < data->param_count; i++){
                string_free(data->params[i].id);
            }
            free(data->params);
        }
        // Uvolníme data položky
        free(item->data);
        item->data = NULL;
    }
    item->symbol_state = SYMTABLE_SYMBOL_DEAD;
    return SYMTABLE_SUCCESS;
}

/**
 * @brief Vymaže všechny položky z tabulky symbolů
*/
void symtable_deleteAll(Symtable *table, bool keep_data) {
    // Pokud je tabulka NULL, nic se neprovede
    if(table == NULL) {
        return;
    }

    // Procházíme všechny položky v tabulce
    for(size_t i = 0; i < table->allocated_size; i++) {
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        SymtableItem item = table->array[i];

        // Pokud je položka živá, odstraníme ji
        if(item.symbol_state != SYMTABLE_SYMBOL_EMPTY &&
           item.symbol_state != SYMTABLE_SYMBOL_DEAD) {

            // Uvolníme data
            if(keep_data == false && item.data != NULL){
                // Pokud je položka funkce, uvolníme i její parametry
                if(item.symbol_state == SYMTABLE_SYMBOL_FUNCTION){
                    SymtableFunctionData *data = (SymtableFunctionData *)item.data;
                    // Pokud je funkce vestavěná, tak uvolníme stringy
                    if(item.key->length > 4){
                        if(memcmp(item.key->str, "ifj.", 4) == 0){
                            for(size_t j = 0; j < data->param_count; j++){
                                string_free(data->params[j].id);
                            }
                        }
                    }
                    free(data->params);
                    free(item.data);
                    item.data = NULL;
                } else
                // Pokud je položka string, uvolníme jako string
                if(item.symbol_state == SYMTABLE_SYMBOL_VARIABLE_STRING ||
                   item.symbol_state == SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL){
                    
                    item.data = NULL;
                }
                // Jinak jen uvolníme data
                else{
                    free(item.data);
                    item.data = NULL;
                }
            }

            // Uvolníme klíč
            if(item.key != NULL){
                string_free(item.key);
                item.key = NULL;
            }

            item.symbol_state = SYMTABLE_SYMBOL_DEAD;
        }
    }
}

/**
 * @brief Zničí tabulku symbolů a uvolní její paměť
*/
inline void symtable_destroyTable(Symtable *table) {
    // Pokud je tabulka NULL, nic se neprovede
    if(table == NULL) {
        return;
    }
    // Odstraníme všechny položky
    symtable_deleteAll(table, false);
    free(table->array);
    free(table);
}

/**
 * @brief Vytiskne obsah tabulky symbolů
 */
void symtable_print(Symtable *table, FILE *file, bool print_data, bool cut_data) {
    // Pokud je tabulka NULL, tak se vrátíme
    if(table == NULL){
        return;
    }

    // Procházíme všechny položky v tabulce
    bool empty = true;
    for(size_t i = 0; i < table->allocated_size; i++){
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        SymtableItem item = table->array[i];
        if(item.symbol_state == SYMTABLE_SYMBOL_EMPTY ||
           (item.symbol_state == SYMTABLE_SYMBOL_DEAD)) {
            continue;
        }
        if(empty){
            fprintf(file, "\n|Key|               |Index|   ");
            if(print_data){
                fprintf(file, "|state|                  |const|       |known value|      |used|        |changed|    |dataAddr|");
            }
            fprintf(file, "\n");
            empty = false;
        }

        // Vytiskneme klíč
        char *key = string_toConstChar(item.key);
        if(cut_data){
            fprintf(file, "%-20.20s", key);
        }else{
            fprintf(file, "%-20s", key);
        }
        free(key);

        // Vytiskneme index
        fprintf(file, "%-10zu", i);

        // Vytiskneme data
        if(print_data){
            // Vytiskneme stav položky
            switch(item.symbol_state){
                case SYMTABLE_SYMBOL_UNKNOWN:
                    fprintf(file, "unknown                  ");
                    break;
                case SYMTABLE_SYMBOL_VARIABLE_INT:
                    fprintf(file, "var_int                  ");
                    break;
                case SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL:
                    fprintf(file, "var_int_or_null          ");
                    break;
                case SYMTABLE_SYMBOL_VARIABLE_DOUBLE:
                    fprintf(file, "var_double               ");
                    break;
                case SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL:
                    fprintf(file, "var_double_or_null       ");
                    break;
                case SYMTABLE_SYMBOL_VARIABLE_STRING:
                    fprintf(file, "var_string               ");
                    break;
                case SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL:
                    fprintf(file, "var_string_or_null       ");
                    break;
                case SYMTABLE_SYMBOL_FUNCTION:
                    fprintf(file, "function                 ");
                    break;
                default:
                    fprintf(file, "something wrong          ");
                    break;
            }
            // Vytiskneme konstantnost
            if(item.constant){
                fprintf(file, "const         ");
            }else{
                fprintf(file, "non-const     ");
            }
            // Vytiskneme definovanost
            if(item.known_value){
                fprintf(file, "known              ");
            }else{
                fprintf(file, "unknown            ");
            }
            // Vytiskneme použitost
            if(item.used){
                fprintf(file, "used          ");
            }else{
                fprintf(file, "unused        ");
            }
            // Vytiskneme změnu
            if(item.changed){
                fprintf(file, "changed       ");
            }else{
                fprintf(file, "unchanged     ");
            }
            // Vytiskneme adresu dat
            if(item.data == NULL){
                fprintf(file, "NULL");
            }else{
                fprintf(file, "%p", item.data);
            }
        }
        //
        fprintf(file, "\n");
    }
    if(empty){
        fprintf(file, "Table is empty\n");
    }
}




/**
 * @brief Hashovací funkce pro výpočet hashe z klíče
*/
size_t symtable_hashFunction(DString *key) {
    size_t hash = 5381;
    for(size_t i = 0; i < key->length; i++) {
        hash = ((hash << 5) + hash) + (size_t)key->str[i];
    }
    return hash;
}

/**
 * @brief Přesun dat z jedné tabulky do druhé (používá se při rozšiřování)
*/
bool symtable_transfer(Symtable *out_table, Symtable *in_table) {
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
        SymtableItemPtr new_location;
        symtable_result result = symtable_addItem(in_table, out_table->array[i].key, &new_location);
        // Pokud se nepodařilo přidat položku, vracíme false
        if(result != SYMTABLE_SUCCESS) {
            return false;
        }
        // Přeneseme data z jedné položky do druhé
        new_location->data = out_table->array[i].data;
        new_location->symbol_state = out_table->array[i].symbol_state;
        new_location->constant = out_table->array[i].constant;
        new_location->known_value = out_table->array[i].known_value;
        new_location->used = out_table->array[i].used;
        new_location->changed = out_table->array[i].changed;
    }
    return true;
}

/**
 * @brief Zvětší tabulku symbolů na novou velikost
*/
Symtable *symtable_resize(Symtable *table, size_t size) {
    // Pokud je tabulka NULL, vracíme NULL (nemělo by nikdy nastat)
    if(table == NULL) {
        return NULL;
    }

    // Vytvoříme pomocnou tabulku
    SymtablePtr new_table = malloc(sizeof(Symtable));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(new_table == NULL) {
        return NULL;
    }

    // Alokujeme paměť pro nové pole položek
    new_table->array = symtable_init_items(size);
    // Pokud se nepodařilo alokovat paměť, uvolníme pomocnou tabulku a vracíme NULL
    if(new_table->array == NULL){
        free(new_table);
        return NULL;
    }

    // Inicializujeme pomocnou tabulku
    new_table->allocated_size = size;
    new_table->used_size = 0;
    // Přeneseme data z původní tabulky do pomocné
    if(symtable_transfer(table,new_table) == false) {
        return NULL;
    }
    // Uvolníme původní pole položek
    symtable_deleteAll(table, true);
    free(table->array);
    table->array = new_table->array;
    table->allocated_size = size;
    free(new_table);

    return table;
}

/**
 * @brief Alokuje paměť pro položky tabulky symbolů
*/
inline SymtableItemPtr symtable_init_items(size_t size) {
    // Alokujeme paměť pro size položek
    SymtableItemPtr items = malloc(size*sizeof(SymtableItem));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(items == NULL){
        return NULL;
    }

    // Každou položku inicializujeme na prázdnou
    for(size_t i = 0; i < size; i++) {
        items[i].key = NULL;
        items[i].symbol_state = SYMTABLE_SYMBOL_EMPTY;
        items[i].data = NULL;
        items[i].constant = false;
        items[i].known_value = false;
        items[i].used = false;
        items[i].changed = false;
    }
    return items;
}

SymtableFunctionData *symtable_init_function_data(size_t param_count){
    // Alokujeme paměť pro strukturu
    SymtableFunctionData *data = malloc(sizeof(SymtableFunctionData));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(data == NULL){
        return NULL;
    }
    // Pokud funkce nemá žádné parametry, inicializujeme strukturu a vracíme ji
    if(param_count == 0){
        data->param_count = 0;
        data->params = NULL;
        return data;
    }

    // Jinak alokujeme paměť pro pole parametrů
    SymtableParamPair *params = malloc(param_count * sizeof(SymtableParamPair));
    // Pokud se nepodařilo alokovat paměť, uvolníme data a vracíme NULL
    if(params == NULL){
        free(data);
        return NULL;
    }
    // Inicializujeme parametry
    for(size_t i = 0; i < param_count; i++){
        params[i].id = NULL;
        params[i].type = SYMTABLE_TYPE_UNKNOWN;
    }
    // Inicializujeme strukturu
    data->param_count = param_count;
    data->params = params;
    return data;
}

void *symtable_createLiteralData(symtable_symbolState state){
    switch(state) {
        case SYMTABLE_SYMBOL_VARIABLE_INT:
            return malloc(sizeof(int));
        case SYMTABLE_SYMBOL_VARIABLE_DOUBLE:
            return malloc(sizeof(double));
        case SYMTABLE_SYMBOL_VARIABLE_STRING:
            return string_init();
        case SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL:
            return malloc(sizeof(int));
        case SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL:
            return malloc(sizeof(double));
        case SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL:
            return string_init();
        default:
            return NULL;
    }
}

/*** Konec souboru symtable.c ***/
