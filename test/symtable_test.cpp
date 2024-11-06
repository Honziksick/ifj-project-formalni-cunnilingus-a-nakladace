/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           symtable_test.cpp                                         *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            06.11.2024                                                *
 * Poslední změna:   06.11.2024                                                *
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

class EmptyTable : public ::testing::Test
{
    protected:
        // Deklarace ukazatele na hashovací tabulku
        SymtablePtr map;

        // Konstanty počtu položek a kapicity tohoto prostředí
        size_t SIZE = 0;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření hashovací tabulky 
            map = symtable_init();
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro hashovací tabulku
            symtable_destroyTable(map);
        }
};

class NonEmptyTableWithExpansion : public ::testing::Test
{
    protected:
        // Deklarace ukazatele na hashovací tabulku
        SymtablePtr map;

        // Konstanty počtu položek a kapicity tohoto prostředí
        const size_t SIZE = 10;

        // Deklrace proměnných pro ulo6ení hodnot
        DString* horse_val, *giraffe_val, *elephant_val, *monkey_val;
        DString* turtle_val, *panda_val, *octopus_val, *fox_val;

        // Deklarace proměnných pro uložení položek
        SymtableItemPtr horse, giraffe, elephant, monkey;
        SymtableItemPtr turtle, panda, octopus, fox;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření hashovací tabulky
            map = symtable_init();

            // Přidání položek do hashovací tabulky a uložení jejích hodnot
            horse_val->str = strdup("42");
            symtable_addItem(map, horse_val, horse);

            giraffe_val->str = strdup("-666");
            symtable_addItem(map, giraffe_val, giraffe);

            elephant_val->str = strdup("-1");
            symtable_addItem(map, elephant_val, elephant);

            monkey_val->str = strdup("0");
            symtable_addItem(map, monkey_val, monkey);

            turtle_val->str = strdup("-42");
            symtable_addItem(map, turtle_val, turtle);

            panda_val->str = strdup("666");
            symtable_addItem(map, panda_val, panda);

            octopus_val->str = strdup("1");
            symtable_addItem(map, octopus_val, octopus);

            fox_val->str = strdup("1024");
            symtable_addItem(map, fox_val, fox);
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro hashovací tabulku
            symtable_destroyTable(map);
        }
};

TEST(NoTable, SymInit)
{
    // Vytvoření hashovací tabulky
    SymtablePtr map = symtable_init();

    // TABLE_INIT_SIZE = 10, je prázdná, tak by neměla nic používat USED_SIZE = 0
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
