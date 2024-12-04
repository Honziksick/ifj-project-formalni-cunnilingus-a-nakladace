/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable.c                                                *
 * Autor:            David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            01.10.2024                                                *
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
* @file symtable.c
* @author Krejčí David \<xkrejcd00>
*
* @brief Implementace tabulky symbolů.
* @details Tento soubor obsahuje implementaci funkcí pro práci s tabulkou symbolů,
*          která je implementována jako hashovací tabulka s implicitním řetězením.
*/

#include "symtable.h"


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE VEŘEJNÝCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

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
    table->allocatedSize = TABLE_INIT_SIZE;
    table->usedSize = 0;

    // Inicializujeme pole položek
    SymtableItemPtr items = symtable_initItems(TABLE_INIT_SIZE);
    // Pokud se nepodařilo alokovat paměť, uvolníme tabulku a vracíme NULL
    if(items == NULL) {
        free(table);
        return NULL;
    }
    // Přiřadíme pole položek do tabulky
    table->array = items;

    return table;
}  // symtable_init()

/**
 * @brief Přidá novou položku do tabulky symbolů
*/
Symtable_result symtable_addItem(Symtable *table, DString *key, SymtableItem **outItem) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    if(key == NULL) {
        return SYMTABLE_KEY_NULL;
    }

    // Vypočítáme aktuální zaplněnost tabulky
    size_t current_fullness = (100 * table->usedSize) / table->allocatedSize;
    // Pokud je tabulka plná, musíme ji rozšířit
    if(current_fullness > MAX_FULLNESS_BEFORE_EXPAND) {
        table = symtable_resize(table, table->allocatedSize * 2);
        // Pokud se nepodařilo rozšířit tabulku, vracíme SYMTABLE_RESIZE_FAIL
        if(table == NULL) {
            return SYMTABLE_RESIZE_FAIL;
        }
    }
    // Vypočítáme index, na kterém by se měla hledaná položka nacházet
    size_t index = symtable_hashFunction(key) % table->allocatedSize;

    // Procházíme tabulku dokud nenarazíme na prázdnou položku
    while(true) {
        SymtableItemPtr item = &table->array[index];
        // Pokud je nalezeno prázdné místo, vkládáme novou položku
        if(item->symbolState == SYMTABLE_SYMBOL_EMPTY ||
           item->symbolState == SYMTABLE_SYMBOL_DEAD) {

            // Vytvoříme kopii klíče
            DString *keyCopy = DString_init();
            // Pokud se nepodařilo alokovat paměť, vracíme SYMTABLE_ALLOCATION_FAIL
            if(keyCopy == NULL) {
                return SYMTABLE_ALLOCATION_FAIL;
            }
            // Pokud se nepodařilo zkopírovat klíč,
            // uvolníme alokovanou paměť a vracíme SYMTABLE_ALLOCATION_FAIL
            if(DString_copy(key,keyCopy) == STRING_COPY_FAIL) {
                DString_free(keyCopy);
                return SYMTABLE_ALLOCATION_FAIL;
            }

            // Inkrementujeme počet použitých položek
            if(item->symbolState == SYMTABLE_SYMBOL_EMPTY) {
                table->usedSize++;
            }

            // Inicializujeme novou položku
            item->key = keyCopy;
            item->data = NULL;
            item->symbolState = SYMTABLE_SYMBOL_UNKNOWN;
            item->constant = false;
            item->knownValue = false;
            item->used = false;
            item->changed = false;

            // Pokud je požadován odkaz na novou položku, vrátíme ho
            if(outItem != NULL) {
                *outItem = item;
            }
            return SYMTABLE_SUCCESS;
        }

        // Pokud položka s daným klíčem již existuje a je živá
        if(DString_compare(item->key, key) == STRING_EQUAL &&
           item->symbolState != SYMTABLE_SYMBOL_DEAD) {

            // Vrátíme odkaz na existující položku a
            if(outItem != NULL) {
                *outItem = item;
            }
            return SYMTABLE_ITEM_ALREADY_EXISTS;
        }
        // Jinak pokračujeme v hledání
        index = (index+1) % table->allocatedSize;
    }
}  // symtable_addItem()

/**
 * @brief Přidá novou položku do tabulky symbolů z tokenu
*/
Symtable_result symtable_addToken(Symtable *table, Token token, SymtableItem **outItem) {
    return symtable_addItem(table, token.value, outItem);
}  // symtable_addToken()

/**
 * @brief Vyhledá položku v tabulce symbolů
*/
Symtable_result symtable_findItem(Symtable *table, DString *searchedKey, SymtableItem **outItem) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Pokud je klíč NULL, vracíme SYMTABLE_KEY_NULL
    if(searchedKey == NULL) {
        return SYMTABLE_KEY_NULL;
    }

    // Vypočítáme index, na kterém by se měla hledaná položka nacházet
    size_t index = symtable_hashFunction(searchedKey) % table->allocatedSize;

    // Vezmeme položku na indexu
    SymtableItemPtr item = &table->array[index];
    // Inkrement pro hledání
    size_t i = 0;
    // Procházíme položky dokud nenarazíme na prázdnou položku
    while(item->symbolState != SYMTABLE_SYMBOL_EMPTY) {

        // Pokud je nalezena položka se stejným klíčem, vracíme ji
        if(DString_compare(item->key, searchedKey) == STRING_EQUAL) {
            if(outItem != NULL) {
                *outItem = item;
            }
            return SYMTABLE_SUCCESS;
        }
        // Jinak pokračujeme v hledání
        i++;
        size_t new_index = (index+i) % table->allocatedSize;
        // Pokud jsme prošli celou tabulku, vracíme SYMTABLE_ITEM_DOESNT_EXIST
        if(new_index == index) {
            return SYMTABLE_ITEM_DOESNT_EXIST;
        }
        item = &table->array[new_index];
    }
    return SYMTABLE_ITEM_DOESNT_EXIST;
}  // symtable_findItem()

/**
 * @brief Odstraní položku z tabulky symbolů
*/
Symtable_result symtable_deleteItem(Symtable *table, DString *key) {
    // Pokud je tabulka NULL, vracíme SYMTABLE_TABLE_NULL
    if(table == NULL) {
        return SYMTABLE_TABLE_NULL;
    }

    // Pokud je klíč NULL, vracíme SYMTABLE_KEY_NULL
    if(key == NULL) {
        return SYMTABLE_KEY_NULL;
    }

    // Pokusíme se najít položku
    SymtableItemPtr item;
    Symtable_result findResult = symtable_findItem(table,key,&item);

    // Při neúspěchu vracíme návratový kód
    if(findResult != SYMTABLE_SUCCESS) {
        return findResult;
    }

    // Při úspěchu položku odstraníme
    if(item->key != NULL) {
        DString_free(item->key);
        item->key = NULL;
    }
    if(item->data != NULL) {
        // Pokud je položka funkce, uvolníme i její parametry
        if(item->symbolState == SYMTABLE_SYMBOL_FUNCTION) {
            SymtableFunctionData *data = (SymtableFunctionData *)item->data;
            for(size_t i = 0; i < data->paramCount; i++) {
                DString_free(data->params[i].id);
            }
            free(data->params);
        }
        // Uvolníme data položky
        free(item->data);
        item->data = NULL;
    }
    item->symbolState = SYMTABLE_SYMBOL_DEAD;
    return SYMTABLE_SUCCESS;
}  // symtable_deleteItem()

/**
 * @brief Vymaže všechny položky z tabulky symbolů
*/
void symtable_deleteAll(Symtable *table, bool keepData) {
    // Pokud je tabulka NULL, nic se neprovede
    if(table == NULL) {
        return;
    }

    // Procházíme všechny položky v tabulce
    for(size_t i = 0; i < table->allocatedSize; i++) {
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        SymtableItem item = table->array[i];

        // Pokud je položka živá, odstraníme ji
        if(item.symbolState != SYMTABLE_SYMBOL_EMPTY &&
           item.symbolState != SYMTABLE_SYMBOL_DEAD) {

            // Uvolníme data
            if(keepData == false && item.data != NULL) {
                // Pokud je položka funkce, uvolníme i její parametry
                if(item.symbolState == SYMTABLE_SYMBOL_FUNCTION) {
                    SymtableFunctionData *data = (SymtableFunctionData *)item.data;
                    // Pokud je funkce vestavěná, tak uvolníme stringy
                    if(item.key->length > 4) {
                        if(memcmp(item.key->str, "ifj.", 4) == 0) {
                            for(size_t j = 0; j < data->paramCount; j++) {
                                DString_free(data->params[j].id);
                            }
                        }
                    }
                    free(data->params);
                    free(item.data);
                    item.data = NULL;
                // Pokud je položka string, uvolníme jako string
                } else if(item.symbolState == SYMTABLE_SYMBOL_VARIABLE_STRING ||
                          item.symbolState == SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL) {

                    item.data = NULL;
                }
                // Jinak jen uvolníme data
                else {
                    //free(item.data);
                    item.data = NULL;
                }
            }

            // Uvolníme klíč
            if(item.key != NULL) {
                DString_free(item.key);
                item.key = NULL;
            }

            item.symbolState = SYMTABLE_SYMBOL_DEAD;
        }
    }
}  // symtable_deleteAll()

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
}  // symtable_destroyTable()

/**
 * @brief Vytiskne obsah tabulky symbolů
 */
void symtable_print(Symtable *table, FILE *file, bool printData, bool cutData) {
    // Pokud je tabulka NULL, tak se vrátíme
    if(table == NULL) {
        return;
    }

    // Procházíme všechny položky v tabulce
    bool empty = true;
    for(size_t i = 0; i < table->allocatedSize; i++) {
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        SymtableItem item = table->array[i];
        if(item.symbolState == SYMTABLE_SYMBOL_EMPTY ||
           (item.symbolState == SYMTABLE_SYMBOL_DEAD)) {
            continue;
        }
        if(empty) {
            fprintf(file, "\n|Key|               |Index|   ");
            if(printData) {
                fprintf(file, "|state|                  |const|       |known value|      |used|        |changed|    |dataAddr|");
            }
            fprintf(file, "\n");
            empty = false;
        }

        // Vytiskneme klíč
        char *key = DString_DStringtoConstChar(item.key);
        if(cutData) {
            fprintf(file, "%-20.20s", key);
        }
        else {
            fprintf(file, "%-20s", key);
        }
        free(key);

        // Vytiskneme index
        fprintf(file, "%-10zu", i);

        // Vytiskneme data
        if(printData) {
            // Vytiskneme stav položky
            switch(item.symbolState) {
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
            if(item.constant) {
                fprintf(file, "const         ");
            }
            else {
                fprintf(file, "non-const     ");
            }
            // Vytiskneme definovanost
            if(item.knownValue) {
                fprintf(file, "known              ");
            }
            else {
                fprintf(file, "unknown            ");
            }
            // Vytiskneme použitost
            if(item.used) {
                fprintf(file, "used          ");
            }
            else {
                fprintf(file, "unused        ");
            }
            // Vytiskneme změnu
            if(item.changed) {
                fprintf(file, "changed       ");
            }
            else {
                fprintf(file, "unchanged     ");
            }
            // Vytiskneme adresu dat
            if(item.data == NULL) {
                fprintf(file, "NULL");
            }
            else {
                fprintf(file, "%p", item.data);
            }
        }
        //
        fprintf(file, "\n");
    }
    if(empty) {
        fprintf(file, "Table is empty\n");
    }
}  // symtable_print()


/*******************************************************************************
 *                                                                             *
 *                        IMPLEMENTACE INTERNÍCH FUNKCÍ                        *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Hashovací funkce pro výpočet hashe z klíče
*/
size_t symtable_hashFunction(DString *key) {
    size_t hash = 5381;
    for(size_t i = 0; i < key->length; i++) {
        hash = ((hash << 5) + hash) + (size_t)key->str[i];
    }
    return hash;
}  // symtable_hashFunction()

/**
 * @brief Přesun dat z jedné tabulky do druhé (používá se při rozšiřování)
*/
bool symtable_transfer(Symtable *outTable, Symtable *inTable) {
    // Pokud je některá z tabulek NULL, vracíme false (nemělo by nikdy nastat)
    if(outTable == NULL || inTable == NULL) {
        return false;
    }
    // Pokud by se položky nevešly do nové tabulky, vracíme false
    // (nemělo by nikdy nastat)
    if(outTable->usedSize > inTable->allocatedSize) {
        return false;
    }

    // Procházíme všechny položky v tabulce
    for(size_t i = 0; i < outTable->allocatedSize; i++) {
        // Pokud je položka prázdná nebo mrtvá, pokračujeme
        if(outTable->array[i].symbolState == SYMTABLE_SYMBOL_EMPTY ||
           outTable->array[i].symbolState == SYMTABLE_SYMBOL_DEAD) {
            continue;
        }

        // Vytvoříme novou položku v cílové tabulce
        SymtableItemPtr newLocation;
        Symtable_result result = symtable_addItem(inTable, outTable->array[i].key, &newLocation);
        // Pokud se nepodařilo přidat položku, vracíme false
        if(result != SYMTABLE_SUCCESS) {
            return false;
        }
        // Přeneseme data z jedné položky do druhé
        newLocation->data = outTable->array[i].data;
        newLocation->symbolState = outTable->array[i].symbolState;
        newLocation->constant = outTable->array[i].constant;
        newLocation->knownValue = outTable->array[i].knownValue;
        newLocation->used = outTable->array[i].used;
        newLocation->changed = outTable->array[i].changed;
    }
    return true;
}  // symtable_transfer()

/**
 * @brief Zvětší tabulku symbolů na novou velikost
*/
Symtable *symtable_resize(Symtable *table, size_t size) {
    // Pokud je tabulka NULL, vracíme NULL (nemělo by nikdy nastat)
    if(table == NULL) {
        return NULL;
    }

    // Vytvoříme pomocnou tabulku
    SymtablePtr newTable = malloc(sizeof(Symtable));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(newTable == NULL) {
        return NULL;
    }

    // Alokujeme paměť pro nové pole položek
    newTable->array = symtable_initItems(size);
    // Pokud se nepodařilo alokovat paměť, uvolníme pomocnou tabulku a vracíme NULL
    if(newTable->array == NULL) {
        free(newTable);
        return NULL;
    }

    // Inicializujeme pomocnou tabulku
    newTable->allocatedSize = size;
    newTable->usedSize = 0;
    // Přeneseme data z původní tabulky do pomocné
    if(symtable_transfer(table, newTable) == false) {
        return NULL;
    }
    // Uvolníme původní pole položek
    symtable_deleteAll(table, true);
    free(table->array);
    table->array = newTable->array;
    table->allocatedSize = size;
    free(newTable);

    return table;
}  // symtable_resize()

/**
 * @brief Alokuje paměť pro položky tabulky symbolů
*/
inline SymtableItemPtr symtable_initItems(size_t size) {
    // Alokujeme paměť pro size položek
    SymtableItemPtr items = malloc(size*sizeof(SymtableItem));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(items == NULL) {
        return NULL;
    }

    // Každou položku inicializujeme na prázdnou
    for(size_t i = 0; i < size; i++) {
        items[i].key = NULL;
        items[i].symbolState = SYMTABLE_SYMBOL_EMPTY;
        items[i].data = NULL;
        items[i].constant = false;
        items[i].knownValue = false;
        items[i].used = false;
        items[i].changed = false;
    }
    return items;
}  // symtable_initItems()

/**
 * @brief Inicializuje novou strukturu pro data funkce
 */
SymtableFunctionData *symtable_initFunctionData(size_t paramCount) {
    // Alokujeme paměť pro strukturu
    SymtableFunctionData *data = malloc(sizeof(SymtableFunctionData));
    // Pokud se nepodařilo alokovat paměť, vracíme NULL
    if(data == NULL) {
        return NULL;
    }
    // Pokud funkce nemá žádné parametry, inicializujeme strukturu a vracíme ji
    if(paramCount == 0) {
        data->paramCount = 0;
        data->params = NULL;
        return data;
    }

    // Jinak alokujeme paměť pro pole parametrů
    SymtableParamPair *params = malloc(paramCount * sizeof(SymtableParamPair));
    // Pokud se nepodařilo alokovat paměť, uvolníme data a vracíme NULL
    if(params == NULL) {
        free(data);
        return NULL;
    }
    // Inicializujeme parametry
    for(size_t i = 0; i < paramCount; i++) {
        params[i].id = NULL;
        params[i].type = SYMTABLE_TYPE_UNKNOWN;
    }
    // Inicializujeme strukturu
    data->bodyFrameID = 0;
    data->paramCount = paramCount;
    data->params = params;
    return data;
}  // symtable_initFunctionData()

/*** Konec souboru symtable.c ***/
