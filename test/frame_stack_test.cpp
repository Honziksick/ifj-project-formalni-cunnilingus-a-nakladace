/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           frame_stack_test.cpp                                      *
 * Autor:            Krejčí David      <xkrejcd00>                             *
 *                                                                             *
 *                                                                             *
 * Datum:            09.11.2024                                                *
 * Poslední změna:   10.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file frame_stack_test.cpp
 * @author Krejčí David      <xkrejcd00>
 *
 * @brief Testy funkcí knihovny s implementací zásobníku rámců a pole rámců
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `frame_stack.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
#include "frame_stack.h"
}

using namespace testing;

/**
 * @brief Testuje funkci `frameStack_init` pro inicializaci zásobníku rámců
 */
TEST(FrameStack, Init) {
    frameStack_init();

    // Ověříme, že zásobník byl inicializován
    ASSERT_NE(stack.top, nullptr);
    ASSERT_NE(stack.bottom, nullptr);
    ASSERT_EQ(stack.currentID, 0ULL);

    // Uvolníme zásobník
    frameStack_destroyAll();
}

/**
 * @brief Testuje funkci `frameStack_push` pro přidání nového rámce na zásobník
 */
TEST(FrameStack, Push) {
    // Zkusíme přidat rámec na neinicializovaný zásobník
    EXPECT_EXIT(frameStack_push(false), ExitedWithCode(99), "");

    frameStack_init();

    // Přidáme nový rámec, vrátíme chybu
    frameStack_push(false);

    // Ověříme, že rámec byl přidán
    ASSERT_NE(stack.top, nullptr);
    ASSERT_EQ(stack.top->frameID, 1ULL);
    ASSERT_EQ(stack.top->searchStop, false);
    EXPECT_EQ(frameArray.array[1], stack.top);

    // Uvolníme zásobník
    frameStack_destroyAll();
}

/**
 * @brief Testuje funkci `frameStack_pop` pro odstranění vrchního rámce ze zásobníku
 */
TEST(FrameStack, Pop) {
    EXPECT_EQ(frameStack_pop(), FRAME_STACK_NOT_INITIALIZED);

    frameStack_init();

    // Přidáme nový rámec
    frameStack_push(false);

    // Ověříme, že rámec byl přidán
    ASSERT_NE(stack.top, nullptr);
    ASSERT_EQ(stack.top->frameID, 1ULL);
    ASSERT_EQ(stack.top->searchStop, false);
    EXPECT_EQ(frameArray.array[1], stack.top);

    // Odstraníme vrchní rámec
    ASSERT_EQ(frameStack_pop(), FRAME_STACK_SUCCESS);
    ASSERT_NE(stack.top, nullptr);
    ASSERT_EQ(stack.top->frameID, 0ULL);
    ASSERT_EQ(stack.top->searchStop, true);
    EXPECT_EQ(frameArray.array[0], stack.top);
    // Rámec zůstal v poli
    ASSERT_EQ(frameArray.array[1]->frameID, 1ULL);

    EXPECT_EQ(frameStack_pop(), FRAME_STACK_POP_GLOBAL);

    // Uvolníme zásobník
    frameStack_destroyAll();
}

TEST(FrameStack, General){
    frameStack_init();

    DString* key = string_init();
    ASSERT_NE(key, nullptr);

    const char *horse = "horse";

    for(size_t i = 0; i < strlen(horse); i++) {
        ASSERT_EQ(string_append_char(key, horse[i]), STRING_SUCCESS);
    }

    // Přidáme nový rámec
    frameStack_push(false);

    // Přidáme položku do rámce
    SymtableItemPtr item;
    ASSERT_EQ(frameStack_addItem(key, &item), FRAME_STACK_SUCCESS);

    // Ověříme, že položka byla přidána
    ASSERT_NE(item, nullptr);
    ASSERT_EQ(string_compare(item->key, key), STRING_EQUAL);
    item->symbol_state = SYMTABLE_SYMBOL_VARIABLE_INT;

    // Přidáme nový rámec
    frameStack_push(false);
    // Ověříme, že položka je v rámci
    SymtableItemPtr item2;
    ASSERT_EQ(frameStack_findItem(key, &item2), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item2, item);
    ASSERT_EQ(item2->symbol_state, SYMTABLE_SYMBOL_VARIABLE_INT);

    SymtableItemPtr item3;
    ASSERT_EQ(frameStack_addItem(key, &item3), FRAME_STACK_ITEM_ALREADY_EXISTS);
    ASSERT_EQ(item3, item);
    ASSERT_EQ(item3->symbol_state, SYMTABLE_SYMBOL_VARIABLE_INT);

    // Přidáme rámec funkce
    frameStack_push(true);
    // Položku bychom neměli najít
    ASSERT_EQ(frameStack_findItem(key, NULL), FRAME_STACK_ITEM_DOESNT_EXIST);

    // Přidáme novou položku se stejným klíčem
    SymtableItemPtr item4;
    ASSERT_EQ(frameStack_addItem(key, &item4), FRAME_STACK_SUCCESS);
    ASSERT_NE(item4, item);
    EXPECT_EQ(item4->symbol_state, SYMTABLE_SYMBOL_UNKNOWN);

    SymtableItemPtr item5;
    ASSERT_EQ(frameStack_findItem(key, &item5), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item5, item4);

    // Odstraníme vrchní rámec
    ASSERT_EQ(frameStack_pop(), FRAME_STACK_SUCCESS);
    // 1. položka by měla být opět v rámci
    SymtableItemPtr item6;
    ASSERT_EQ(frameStack_findItem(key, &item6), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item6, item);

    ASSERT_EQ(frameArray.array[1]->frame->used_size, 1ULL);    
    ASSERT_EQ(frameArray.array[2]->frame->used_size, 0ULL);
    ASSERT_EQ(frameArray.array[3]->frame->used_size, 1ULL);

    // Uvolníme zásobník
    frameStack_destroyAll();
    string_free(key);
}

/**
 * @brief Testuje funkci `frameStack_findItem` pro vyhledání položky v globálním rámci
 */
TEST(FrameStack, GlobalFind){
    frameStack_init();

    DString* key = string_init();
    ASSERT_NE(key, nullptr);

    const char *horse = "horse";

    for(size_t i = 0; i < strlen(horse); i++) {
        ASSERT_EQ(string_append_char(key, horse[i]), STRING_SUCCESS);
    }

    // Přidáme položku do globálního rámce
    SymtableItemPtr item;
    ASSERT_EQ(frameStack_addItem(key, &item), FRAME_STACK_SUCCESS);

    // Přidáme nový rámec funkce
    frameStack_push(true);

    SymtableItemPtr item2;
    ASSERT_EQ(frameStack_findItem(key, &item2), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item2, item);

    // Uvolníme zásobník
    frameStack_destroyAll();
    string_free(key);
}

TEST(FrameStack, Many){
    frameStack_init();

    DString* key1 = string_init();
    ASSERT_NE(key1, nullptr);

    DString* key2 = string_init();
    ASSERT_NE(key2, nullptr);

    const char *horse = "horse";
    const char *house = "house";

    for(size_t i = 0; i < strlen(horse); i++) {
        ASSERT_EQ(string_append_char(key1, horse[i]), STRING_SUCCESS);
        ASSERT_EQ(string_append_char(key2, house[i]), STRING_SUCCESS);
    }

    SymtableItemPtr item;
    // Přidáme položku do globálního rámce
    ASSERT_EQ(frameStack_addItem(key1, &item), FRAME_STACK_SUCCESS);

    // Přidáme rámec
    frameStack_push(false);
    // Přidáme položku do rámce
    SymtableItemPtr item2;
    ASSERT_EQ(frameStack_addItem(key2, &item2), FRAME_STACK_SUCCESS);

    // Přidáme a odebereme množství rámců
    frameStack_push(false);
    frameStack_pop();
    frameStack_push(true);

    frameStack_push(false);
    frameStack_push(false);
    frameStack_pop();
    frameStack_push(true);

    frameStack_push(false);
    frameStack_push(false);
    frameStack_pop();
    frameStack_push(true);

    frameStack_push(false);
    frameStack_push(false);
    frameStack_pop();
    frameStack_push(true);

    frameStack_push(false);
    frameStack_push(false);
    frameStack_pop();
    frameStack_push(true);

    // Oveříme očekávané hodnoty
    EXPECT_EQ(frameArray.allocated, 20ULL);
    EXPECT_EQ(stack.currentID, 15ULL);

    // Ověříme, že 1. položka je v rámci a 2. není
    SymtableItemPtr item3;
    ASSERT_EQ(frameStack_findItem(key1, &item3), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item3, item);
    ASSERT_EQ(frameStack_findItem(key2, NULL), FRAME_STACK_ITEM_DOESNT_EXIST);

    frameStack_destroyAll();
    string_free(key1);
    string_free(key2);
}