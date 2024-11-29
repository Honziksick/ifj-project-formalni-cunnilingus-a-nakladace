/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           frame_stack_test.cpp                                      *
 * Autor:            Krejčí David      <xkrejcd00>                             *
 *                                                                             *
 *                                                                             *
 * Datum:            09.11.2024                                                *
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
 * @file frame_stack_test.cpp
 * @author Krejčí David      <xkrejcd00>
 *
 * @brief Testy funkcí knihovny s implementací zásobníku rámců a pole rámců
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `frame_stack.c`. Testy jsou vytvořené za využití frameworku
 *          Google Test.
 */

#include <unistd.h>
#include <fcntl.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ifj24_compiler_test_utils.h"

/**
 * @brief Testuje funkci `frameStack_init` pro inicializaci zásobníku rámců
 */
TEST(FrameStack, Init) {
    frameStack_init();

    // Ověříme, že zásobník byl inicializován
    ASSERT_NE(frameStack.top, nullptr);
    ASSERT_NE(frameStack.bottom, nullptr);
    ASSERT_EQ(frameStack.currentID, 0ULL);

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
    ASSERT_NE(frameStack.top, nullptr);
    ASSERT_EQ(frameStack.top->frameID, 1ULL);
    ASSERT_EQ(frameStack.top->searchStop, false);
    EXPECT_EQ(frameArray.array[1], frameStack.top);

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
    ASSERT_NE(frameStack.top, nullptr);
    ASSERT_EQ(frameStack.top->frameID, 1ULL);
    ASSERT_EQ(frameStack.top->searchStop, false);
    EXPECT_EQ(frameArray.array[1], frameStack.top);

    // Odstraníme vrchní rámec
    ASSERT_EQ(frameStack_pop(), FRAME_STACK_SUCCESS);
    ASSERT_NE(frameStack.top, nullptr);
    ASSERT_EQ(frameStack.top->frameID, 0ULL);
    ASSERT_EQ(frameStack.top->searchStop, true);
    EXPECT_EQ(frameArray.array[0], frameStack.top);
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
    item->symbolState = SYMTABLE_SYMBOL_VARIABLE_INT;

    // Přidáme nový rámec
    frameStack_push(false);
    // Ověříme, že položka je v rámci
    SymtableItemPtr item2;
    ASSERT_EQ(frameStack_findItem(key, &item2), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item2, item);
    ASSERT_EQ(item2->symbolState, SYMTABLE_SYMBOL_VARIABLE_INT);

    SymtableItemPtr item3;
    ASSERT_EQ(frameStack_addItem(key, &item3), FRAME_STACK_ITEM_ALREADY_EXISTS);
    ASSERT_EQ(item3, item);
    ASSERT_EQ(item3->symbolState, SYMTABLE_SYMBOL_VARIABLE_INT);

    // Přidáme rámec funkce
    frameStack_push(true);
    // Položku bychom neměli najít
    ASSERT_EQ(frameStack_findItem(key, NULL), FRAME_STACK_ITEM_DOESNT_EXIST);

    // Přidáme novou položku se stejným klíčem
    SymtableItemPtr item4;
    ASSERT_EQ(frameStack_addItem(key, &item4), FRAME_STACK_SUCCESS);
    ASSERT_NE(item4, item);
    EXPECT_EQ(item4->symbolState, SYMTABLE_SYMBOL_UNKNOWN);

    SymtableItemPtr item5;
    ASSERT_EQ(frameStack_findItem(key, &item5), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item5, item4);

    // Odstraníme vrchní rámec
    ASSERT_EQ(frameStack_pop(), FRAME_STACK_SUCCESS);
    // 1. položka by měla být opět v rámci
    SymtableItemPtr item6;
    ASSERT_EQ(frameStack_findItem(key, &item6), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item6, item);

    ASSERT_EQ(frameArray.array[1]->frame->usedSize, 1ULL);    
    ASSERT_EQ(frameArray.array[2]->frame->usedSize, 0ULL);
    ASSERT_EQ(frameArray.array[3]->frame->usedSize, 1ULL);

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

    MAKE_STRING(key1, "horse");
    MAKE_STRING(key2, "house");

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
    EXPECT_EQ(frameStack.currentID, 15ULL);

    // Ověříme, že 1. položka je v rámci a 2. není
    SymtableItemPtr item3;
    ASSERT_EQ(frameStack_findItem(key1, &item3), FRAME_STACK_SUCCESS);
    ASSERT_EQ(item3, item);
    ASSERT_EQ(frameStack_findItem(key2, NULL), FRAME_STACK_ITEM_DOESNT_EXIST);

    frameStack_destroyAll();
    string_free(key1);
    string_free(key2);
}

TEST(FrameStack, print){
    frameStack_init();

    MAKE_STRING(key1, "horse");
    MAKE_STRING(key2, "house");
    MAKE_STRING(key3, "hose");
    MAKE_STRING(key4, "hose");
    MAKE_STRING(key5, "hound");
    MAKE_STRING(key6, "plane");
    MAKE_STRING(key7, "car");
    MAKE_STRING(key8, "train");
    MAKE_STRING(key9, "bus");
    MAKE_STRING(key10, "bike");
    MAKE_STRING(key11, "boat");
    MAKE_STRING(key12, "ship");
    MAKE_STRING(key13, "submarine");
    MAKE_STRING(key14, "helicopter");
    MAKE_STRING(key15, "airplane");
    MAKE_STRING(key16, "rocket");
    MAKE_STRING(key17, "space shuttle");
    MAKE_STRING(key18, "satellite");
    MAKE_STRING(key19, "moon");
    MAKE_STRING(key20, "mars");

    //const char *expected = " ";
    // Přidáme položku do globálního rámce
    ASSERT_EQ(frameStack_addItem(key1, NULL), FRAME_STACK_SUCCESS);


    // Přidáme rámec
    frameStack_push(false);
    // Přidáme položku do rámce
    ASSERT_EQ(frameStack_addItem(key2, NULL), FRAME_STACK_SUCCESS);

    // Přidáme a odebereme množství rámců
    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key3, NULL), FRAME_STACK_SUCCESS);
    frameStack_pop();
    frameStack_push(true);
    ASSERT_EQ(frameStack_addItem(key4, NULL), FRAME_STACK_SUCCESS);
    ASSERT_EQ(frameStack_addItem(key5, NULL), FRAME_STACK_SUCCESS);

    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key6, NULL), FRAME_STACK_SUCCESS);
    ASSERT_EQ(frameStack_addItem(key7, NULL), FRAME_STACK_SUCCESS);
    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key8, NULL), FRAME_STACK_SUCCESS);
    frameStack_pop();
    frameStack_push(true);
    ASSERT_EQ(frameStack_addItem(key9, NULL), FRAME_STACK_SUCCESS);

    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key10, NULL), FRAME_STACK_SUCCESS);
    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key11, NULL), FRAME_STACK_SUCCESS);
    frameStack_pop();
    frameStack_push(true);
    ASSERT_EQ(frameStack_addItem(key12, NULL), FRAME_STACK_SUCCESS);
    ASSERT_EQ(frameStack_addItem(key13, NULL), FRAME_STACK_SUCCESS);

    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key14, NULL), FRAME_STACK_SUCCESS);
    frameStack_push(false);
    frameStack_pop();
    frameStack_push(true);

    frameStack_push(false);
    frameStack_push(false);
    ASSERT_EQ(frameStack_addItem(key15, NULL), FRAME_STACK_SUCCESS);
    frameStack_pop();
    frameStack_push(true);
    ASSERT_EQ(frameStack_addItem(key16, NULL), FRAME_STACK_SUCCESS);
    ASSERT_EQ(frameStack_addItem(key17, NULL), FRAME_STACK_SUCCESS);
    ASSERT_EQ(frameStack_addItem(key18, NULL), FRAME_STACK_SUCCESS);
    ASSERT_EQ(frameStack_addItem(key19, NULL), FRAME_STACK_SUCCESS);
    SymtableItemPtr item;
    ASSERT_EQ(frameStack_addItem(key20, &item), FRAME_STACK_SUCCESS);
    item->symbolState = SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL;
    item->changed = true;
    item->constant = true;
    item->knownValue = true;
    item->used = true;
    item->data = (void*)malloc(sizeof(int));


    /**
     * Nastavíme prostředí na test stdout
     */

    // Uložíme originální stdout
    int savedStdout = dup(STDOUT_FILENO);

    // Vyrobíme potrubí a přesměrujeme stdout na zápisový konec potrubí
    int fds[2];
    pipe(fds);
    dup2(fds[1], STDOUT_FILENO);
    close(fds[1]);

    // Voláme funkci na print
    frameStack_print(stdout, true, true);

    // Obnovíme stdout
    fflush(stdout);
    dup2(savedStdout, STDOUT_FILENO);
    close(savedStdout);

    // Přečteme výstup z potrubí
    char buffer[1];
    ssize_t n = read(fds[0], buffer, sizeof(buffer));
    close(fds[0]);

    // Otestujeme, že se něco zapsalo
    EXPECT_GT(n, 0);

    
    // Uvolňuje AST
    free(item->data);

    // Vše uvolníme
    frameStack_destroyAll();
    string_free(key1);
    string_free(key2);
    string_free(key3);
    string_free(key4);
    string_free(key5);
    string_free(key6);
    string_free(key7);
    string_free(key8);
    string_free(key9);
    string_free(key10);
    string_free(key11);
    string_free(key12);
    string_free(key13);
    string_free(key14);
    string_free(key15);
    string_free(key16);
    string_free(key17);
    string_free(key18);
    string_free(key19);
    string_free(key20);

}