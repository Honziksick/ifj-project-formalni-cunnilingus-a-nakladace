/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable_test.cpp                                         *
 * Autor:            Farkašovský Lukáš  <xfarkal00> (hlavní testy)             *
 *                   Krejčí David       <xkrejcd00> (doplnění testů)           *
 *                                                                             *
 * Datum:            06.11.2024                                                *
 * Poslední změna:   09.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file symtable_test.cpp
 * @author Farkašovský Lukáš  \<xfarkal00>
 *
 * @brief Testy funkcí knihovny s implementací tabulky symbolů
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `symtable.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "symtable.h"
}

using namespace testing;

/**
 * @brief Testuje funkci `symtable_destroyTable` pro zničení tabulky
 */
TEST(NoTable, HashFunction) {
    DString *str = string_init();
    ASSERT_NE(str, nullptr);

    // String "abc"
    ASSERT_EQ(string_append_char(str, 'a'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'b'), STRING_SUCCESS);
    ASSERT_EQ(string_append_char(str, 'c'), STRING_SUCCESS);

    size_t hash = symtable_hashFunction(str);
    // Po dosazení do hashovací funkce hash = ((hash << 5) + hash) + (size_t)key->str[i];
    size_t result = 193485963;

    EXPECT_EQ(hash, result);

    string_free(str);
}

/**
 * @brief Testuje funkci `symtable_deleteAll` pro odstranění všech položek tabulky, která je prázdná
 */
TEST(NoTable, SymDelAll) {
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    symtable_deleteAll(map, true);

    free(map->array);
    free(map);
}

/**
 * @brief Testuje funkci `symtable_destroyTable` pro zničení tabulky
 */
TEST(NoTable, DestroyTable) {
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    symtable_destroyTable(map);
    symtable_destroyTable(NULL);
}

/**
 * @brief Testuje funkci `symtable_init_items` pro inicializaci položek tabulky
 */
TEST(NoTable, SymInitItem){
    SymtableItemPtr item = symtable_init_items(10);

    ASSERT_NE(item, nullptr);
    ASSERT_EQ(item[0].key, nullptr);
    ASSERT_EQ(item[0].symbol_state, SYMTABLE_SYMBOL_EMPTY);

    free(item);
}

/**
 * @brief Testuje funkci `symtable_init` pro inicializaci tabulky
 */
TEST(NoTable, SymInit)
{
    // Vytvoření hashovací tabulky
    SymtablePtr map = symtable_init();

    // TABLE_INIT_SIZE 10, je prázdná, tak by neměla nic používat USED_SIZE = 0
    size_t SIZE = 10;
    size_t USED_SIZE = 0;

    // Ověření, že nebyl vrácen ukazatel NULL, resp. podařilo se alokovat paměť
    ASSERT_NE(map, nullptr);

    // Ověření správné inicializace všech prvků struktury "hash_map_t"
    EXPECT_NE(map->array, nullptr);
    EXPECT_EQ(map->allocated_size, SIZE);
    EXPECT_EQ(map->used_size, USED_SIZE);

    // Uvolnění celé prázdné hashovací tabulky z paměti
    symtable_destroyTable(map);
}

/**
 * @brief Testuje funkci `symtable_addItem` pro přidání položek do tabulky
 */
TEST(Table, SymAddItems)
{
    // Vložení první položky a kontrola stavu tabulky po jejím vložení
    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);
    DString* horse_val1 = string_init();
    ASSERT_NE(horse_val1, nullptr);
    DString* horse_val2 = string_init();
    ASSERT_NE(horse_val2, nullptr);
    DString* horse_val3 = string_init();
    ASSERT_NE(horse_val3, nullptr);
    DString* horse_val4 = string_init();
    ASSERT_NE(horse_val4, nullptr);
    DString* horse_val5 = string_init();
    ASSERT_NE(horse_val5, nullptr);
    DString* horse_val6 = string_init();
    ASSERT_NE(horse_val6, nullptr);
    DString* horse_val7 = string_init();
    ASSERT_NE(horse_val7, nullptr);
    DString* horse_val8 = string_init();
    ASSERT_NE(horse_val8, nullptr);

    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);


    // Konstanty počtu položek a kapicity tohoto prostředí
    size_t SIZE = 10;
    size_t USED_SIZE = 0;

    // Konstanty pro testování stringů
    const char *strConst  = "horse";
    const char *strConst1 = "horse1";
    const char *strConst2 = "horse2";
    const char *strConst3 = "horse3";
    const char *strConst4 = "horse4";
    const char *strConst5 = "horse5";
    const char *strConst6 = "horse6";
    const char *strConst7 = "horse7";
    const char *strConst8 = "horse8";

    // Vložení slova do horse_val
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
    }
    // Vložení slova do horse_val1 - horse_val7
    for(size_t i = 0; i < strlen(strConst1); i++) {
        ASSERT_EQ(string_append_char(horse_val1, strConst1[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val2, strConst2[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val3, strConst3[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val4, strConst4[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val5, strConst5[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val6, strConst6[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val7, strConst7[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(horse_val8, strConst8[i]), STRING_SUCCESS);
    }
    

    size_t index = symtable_hashFunction(horse_val) % map->allocated_size;

    EXPECT_EQ(map->used_size, USED_SIZE);
    EXPECT_EQ(map->allocated_size, SIZE);

    // Pokud je klíč NULL nebo tabulka NULL, vracíme chybu
    EXPECT_EQ(symtable_addItem(map, NULL, NULL), SYMTABLE_KEY_NULL);
    EXPECT_EQ(symtable_addItem(NULL, horse_val, NULL), SYMTABLE_TABLE_NULL);
    EXPECT_EQ(map->used_size, USED_SIZE);

    // Přidání itemu do tabulky
    ASSERT_EQ(symtable_addItem(map, horse_val, NULL), SYMTABLE_SUCCESS);

    EXPECT_EQ(map->used_size, USED_SIZE + 1);
    EXPECT_EQ(map->allocated_size, SIZE);
    EXPECT_EQ(string_compare(map->array[index].key, horse_val), STRING_EQUAL);

    index = symtable_hashFunction(horse_val1) % map->allocated_size;
    symtable_addItem(map, horse_val1, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 2);
    EXPECT_EQ(map->allocated_size, SIZE);
    EXPECT_EQ(string_compare(map->array[index].key, horse_val1), STRING_EQUAL);
    

    symtable_addItem(map, horse_val2, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 3);
    EXPECT_EQ(map->allocated_size, SIZE);

    symtable_addItem(map, horse_val3, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 4);
    EXPECT_EQ(map->allocated_size, SIZE);

    symtable_addItem(map, horse_val4, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 5);
    EXPECT_EQ(map->allocated_size, SIZE);

    ASSERT_EQ(symtable_addItem(map, horse_val5, NULL), SYMTABLE_SUCCESS);

    EXPECT_EQ(map->used_size, USED_SIZE + 6);
    EXPECT_EQ(map->allocated_size, SIZE);

    symtable_addItem(map, horse_val6, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 7);
    EXPECT_EQ(map->allocated_size, SIZE);

    symtable_addItem(map, horse_val7, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 8);
    EXPECT_EQ(map->allocated_size, SIZE*2);

    symtable_addItem(map, horse_val8, NULL);

    EXPECT_EQ(map->used_size, USED_SIZE + 9);
    EXPECT_EQ(map->allocated_size, SIZE*2);

    symtable_destroyTable(map);
    string_free(horse_val);
    string_free(horse_val1);
    string_free(horse_val2);
    string_free(horse_val3);
    string_free(horse_val4);
    string_free(horse_val5);
    string_free(horse_val6);
    string_free(horse_val7);
    string_free(horse_val8);
}

/**
 * @brief Testuje funkci `symtable_deleteItem` pro odstranění položky z tabulky
 */
TEST(Table, SymRemItem){

    // Vložení první položky a kontrola stavu tabulky po jejím vložení
    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);

    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    // Konstanty pro testování stringů
    const char *strConst  = "horse";

     for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
    }

    // Přidání itemu do tabulky
    ASSERT_EQ(symtable_addItem(map, horse_val, NULL), SYMTABLE_SUCCESS);

    // Odstranění položky
    EXPECT_EQ(symtable_deleteItem(map, horse_val), SYMTABLE_SUCCESS);
    EXPECT_EQ(symtable_deleteItem(NULL, horse_val), SYMTABLE_TABLE_NULL);
    EXPECT_EQ(symtable_deleteItem(map, NULL), SYMTABLE_KEY_NULL);

    symtable_destroyTable(map);
    string_free(horse_val);
}

/**
 * @brief Testuje funkci `symtable_findItem` pro nalezení položky v tabulce
 */
TEST(Table, SymFindItem){
    
        // Vložení první položky a kontrola stavu tabulky po jejím vložení
        DString* horse_val = string_init();
        ASSERT_NE(horse_val, nullptr);
    
        SymtablePtr map = symtable_init();
        ASSERT_NE(map, nullptr);
    
        // Konstanty pro testování stringů
        const char *strConst  = "horse";
    
        for(size_t i = 0; i < strlen(strConst); i++) {
            ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
        }
    
        // Přidání itemu do tabulky
        ASSERT_EQ(symtable_addItem(map, horse_val, NULL), SYMTABLE_SUCCESS);
    
        // Odstranění položky
        EXPECT_EQ(symtable_findItem(map, horse_val, NULL), SYMTABLE_SUCCESS);
        EXPECT_EQ(symtable_findItem(NULL, horse_val, NULL), SYMTABLE_TABLE_NULL);
        EXPECT_EQ(symtable_findItem(map, NULL, NULL), SYMTABLE_KEY_NULL);
    
        symtable_destroyTable(map);
        string_free(horse_val);
}

/**
 * @brief Testuje funkci `symtable_deleteAll` pro odstranění všech položek tabulky
 */
TEST(Table, SymDelALL){
        
    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);
        
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);
        
    SymtableItemPtr item;
        
    // Konstanty pro testování stringů
     const char *strConst  = "horse";
        
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
     }
        
     // Přidání itemu do tabulky
     ASSERT_EQ(symtable_addItem(map, horse_val, &item), SYMTABLE_SUCCESS);
        
     // Odstranění položky
     symtable_deleteAll(map, true);

     // Free bordelu, co jsem s i v testu vytvořil
     free(map->array);
     free(map);
     string_free(horse_val);
}

/**
 * @brief Testuje funkci `symtable_addToken` pro přidání položky z tokenu do tabulky
 */
TEST(Table, SymAddToken){

    Token* token = (Token*)malloc(sizeof(Token));
    ASSERT_NE(token, nullptr);

    DString* horse_val = string_init();

    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    // Konstanty pro testování stringů
    const char *strConst  = "horse";

    token->type = TOKEN_IDENTIFIER;
    token->value = horse_val;
    ASSERT_NE(token->value, nullptr);

    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(token->value, strConst[i]), STRING_SUCCESS);
    }

    // Přidání itemu do tabulky
    ASSERT_EQ(symtable_addItem(map, token->value, NULL), SYMTABLE_SUCCESS);

    // Odstranění položky
    symtable_destroyTable(map);
    string_free(token->value);
    free(token);
}

/**
 * @brief Testuje funkci `symtable_transfer` pro přenesení položek z jedné tabulky do druhé
 */
TEST(Table, SymTransfer){
    
    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);
    
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    SymtablePtr map2 = symtable_init();
    ASSERT_NE(map2, nullptr);

    // Konstanty pro testování stringů
    const char *strConst  = "horse";
    
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
    }
    
    // Přidání itemu do tabulky
    ASSERT_EQ(symtable_addItem(map, horse_val, NULL), SYMTABLE_SUCCESS);
    
    // Přeneseme data z jedné položky do druhé
    EXPECT_TRUE(symtable_transfer(map, map2));

    EXPECT_FALSE(symtable_transfer(NULL, map2));
    EXPECT_FALSE(symtable_transfer(NULL, map));
    EXPECT_FALSE(symtable_transfer(map, NULL));
    EXPECT_FALSE(symtable_transfer(map2, NULL));

    EXPECT_EQ(symtable_findItem(map2, horse_val, NULL), SYMTABLE_SUCCESS);
    
    symtable_destroyTable(map);
    symtable_destroyTable(map2);
    string_free(horse_val);
}

/**
 * @brief Testuje funkci `symtable_transfer` pro přenesení položek z jedné prázdné tabulky do druhé
 */
TEST(Table, SymTransferEmpty){
    
    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);
    
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    SymtablePtr map2 = symtable_init();
    ASSERT_NE(map2, nullptr);

    // Konstanty pro testování stringů
    const char *strConst  = "horse";
    
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
    }
    
    // Přidání itemu do tabulky
    ASSERT_EQ(symtable_addItem(map, horse_val, NULL), SYMTABLE_SUCCESS);
    
    // Přeneseme data z jedné položky do druhé
    EXPECT_TRUE(symtable_transfer(map2, map));
    
    symtable_destroyTable(map);
    symtable_destroyTable(map2);
    string_free(horse_val);
}

/**
 * @brief Testuje funkci `symtable_resize` pro zvětšení tabulky
 */
TEST(Table, SymResize){

    size_t SIZE = 10;

    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);
    
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    SymtablePtr map2 = symtable_init();
    ASSERT_NE(map2, nullptr);
    
    // Konstanty pro testování stringů
    const char *strConst  = "horse";
    
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
    }
    
    // Přidání itemu do tabulky
    EXPECT_EQ(symtable_addItem(map, horse_val, NULL), SYMTABLE_SUCCESS);
    EXPECT_EQ(map->allocated_size, SIZE);
    
    map2 = symtable_resize(map2, map2->allocated_size*2);

    EXPECT_EQ(map2->allocated_size, SIZE*2);

    symtable_destroyTable(map);
    symtable_destroyTable(map2);
    string_free(horse_val);
}

/**
 * @brief Testuje práci s daty proměnné v tabulce symbolů
 */
TEST(Table, ItemData){
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);

    // Konstanty pro testování stringů
    const char *strConst  = "horseInt";
    
    for(size_t i = 0; i < strlen(strConst); i++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[i]), STRING_SUCCESS);
    }


    SymtableItemPtr item1;
    ASSERT_EQ(symtable_addItem(map, horse_val, &item1), SYMTABLE_SUCCESS);
    ASSERT_NE(item1, nullptr);
    ASSERT_EQ(string_compare(item1->key, horse_val), STRING_EQUAL);

    item1->constant = true;
    item1->known_value = true;
    item1->used = false;
    item1->symbol_state = SYMTABLE_SYMBOL_VARIABLE_INT;
    item1->data = malloc(sizeof(int));
    ASSERT_NE(item1->data, nullptr);
    *((int*)item1->data) = 5;

    SymtableItemPtr item2;
    ASSERT_EQ(symtable_findItem(map, horse_val, &item2), SYMTABLE_SUCCESS);
    ASSERT_EQ(item1->constant, item2->constant);
    ASSERT_EQ(item1->known_value, item2->known_value);
    ASSERT_EQ(item1->used, item2->used);
    ASSERT_EQ(item1->symbol_state, item2->symbol_state);
    ASSERT_EQ(*((int*)item2->data), 5);

    symtable_destroyTable(map);
    string_free(horse_val);
}

/**
 * @brief Testuje práci s daty funkce v tabulce symbolů
 */
TEST(Table, FunctionData){
    SymtablePtr map = symtable_init();
    ASSERT_NE(map, nullptr);

    DString* horse_val = string_init();
    ASSERT_NE(horse_val, nullptr);

    DString* a = string_init();
    ASSERT_NE(a, nullptr);
    ASSERT_EQ(string_append_char(a, 'a'), STRING_SUCCESS);
    DString* b = string_init();
    ASSERT_NE(b, nullptr);
    ASSERT_EQ(string_append_char(b, 'b'), STRING_SUCCESS);
    DString* c = string_init();
    ASSERT_NE(c, nullptr);
    ASSERT_EQ(string_append_char(c, 'c'), STRING_SUCCESS);
    DString* d = string_init();
    ASSERT_NE(d, nullptr);
    ASSERT_EQ(string_append_char(d, 'd'), STRING_SUCCESS);
    DString* e = string_init();
    ASSERT_NE(e, nullptr);
    ASSERT_EQ(string_append_char(e, 'e'), STRING_SUCCESS);
    DString* f = string_init();
    ASSERT_NE(f, nullptr);
    ASSERT_EQ(string_append_char(f, 'f'), STRING_SUCCESS);
    DString* g = string_init();
    ASSERT_NE(g, nullptr);
    ASSERT_EQ(string_append_char(g, 'g'), STRING_SUCCESS);
    DString* h = string_init();
    ASSERT_NE(h, nullptr);
    ASSERT_EQ(string_append_char(h, 'h'), STRING_SUCCESS);
    DString* i = string_init();
    ASSERT_NE(i, nullptr);
    ASSERT_EQ(string_append_char(i, 'i'), STRING_SUCCESS);


    // Konstanty pro testování stringů
    const char *strConst  = "morseFn";
    
    for(size_t k = 0; k < strlen(strConst); k++) {
        ASSERT_EQ(string_append_char(horse_val, strConst[k]), STRING_SUCCESS);
    }

    // Přidání položky do tabulky
    SymtableItemPtr item1;
    ASSERT_EQ(symtable_addItem(map, horse_val, &item1), SYMTABLE_SUCCESS);

    // Inicializace dat funkce
    SymtableFunctionData *data = symtable_init_function_data(2);
    ASSERT_NE(data, nullptr);
    ASSERT_EQ(data->param_count, 2ULL);

    data->return_type = SYMTABLE_TYPE_VOID;

    // Nastavíme data 1. parametru
    data->params[0].id = string_init();
    ASSERT_NE(data->params[0].id, nullptr);
    ASSERT_EQ(string_append_char(data->params[0].id, 'p'), STRING_SUCCESS);
    data->params[0].type = SYMTABLE_TYPE_INT;

    // Nastavíme data 2. parametru
    data->params[1].id = string_init();
    ASSERT_NE(data->params[1].id, nullptr);
    ASSERT_EQ(string_append_char(data->params[1].id, 'l'), STRING_SUCCESS);
    data->params[1].type = SYMTABLE_TYPE_STRING_OR_NULL;

    // Přiřazení dat funkce do položky
    item1->data = data;
    item1->known_value = true;
    item1->symbol_state = SYMTABLE_SYMBOL_FUNCTION;
    item1->used = false;

    // Přidáme další položky do tabulky
    ASSERT_EQ(symtable_addItem(map, a, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, b, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, c, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, d, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, e, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, f, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, g, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, h, NULL), SYMTABLE_SUCCESS);
    ASSERT_EQ(symtable_addItem(map, i, NULL), SYMTABLE_SUCCESS);
    
    // Pokusíme se najít položku
    ASSERT_EQ(symtable_findItem(map, horse_val, &item1), SYMTABLE_SUCCESS);
    SymtableFunctionData *found_data = (SymtableFunctionData *)item1->data;
    // Porovnáme data
    ASSERT_NE(found_data, nullptr);
    ASSERT_EQ(found_data->param_count, 2ULL);
    ASSERT_EQ(found_data->return_type, SYMTABLE_TYPE_VOID);
    ASSERT_EQ(string_compare_const_str(found_data->params[0].id, "p"), STRING_EQUAL);
    ASSERT_EQ(found_data->params[0].type, SYMTABLE_TYPE_INT);
    ASSERT_EQ(string_compare_const_str(found_data->params[1].id, "l"), STRING_EQUAL);
    ASSERT_EQ(found_data->params[1].type, SYMTABLE_TYPE_STRING_OR_NULL);

    // Uvolňuje se v rámci destrukce AST
    string_free(data->params[0].id);
    string_free(data->params[1].id);

    // Uvolňuje se v rámci destrukce Symtable
    symtable_destroyTable(map);
    string_free(horse_val);
    string_free(a);
    string_free(b);
    string_free(c);
    string_free(d);
    string_free(e);
    string_free(f);
    string_free(g);
    string_free(h);
    string_free(i);
}