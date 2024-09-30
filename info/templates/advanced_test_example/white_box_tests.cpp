//========== Copyright (c) 2024, FIT VUT Brno, All rights reserved. ==========//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     JAN KALINA <xkalinj00@stud.fit.vutbr.cz>
// $Date:       $2024-02-14
//
//============================================================================//

/**
 * @file white_box_tests.cpp
 * @author JAN KALINA
 * 
 * @date 2024-02-26 Last update
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>
#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
//
// ** ZDE DOPLŇTE TESTY **
//
// Zde doplňte testy hašovací tabulky, testujte následující:
// 1. Veřejné rozhraní hašovací tabulky
//     - Všechny funkce z white_box_code.h
//     - Chování těchto metod testuje pro prázdnou i neprázdnou tabulku.
// 2. Chování tabulky v hranicích případech
//     - Otestujte chování při kolizích různých klíčů se stejným hashem
//     - Otestujte chování při kolizích hashů namapovaných na stejné místo v
//       indexu
//
//============================================================================//


/******************************************************************************/
/*                                                                            */
/*                         DŮLEŽITÉ INFORMACE ÚVODEM                          */
/*                                                                            */
/******************************************************************************/

/*
 * Následující kód je rozdělen do jednotlivých oddílů podle použitých
 * testovacích prostředí - "NoTable", "EmptyTable", "NonEmptyTableSmall",
 * "NonEmptyTableWithExpansion". V kostře programu "white_box_code.cpp",
 * resp. "white_box_code.h", je nevhodně pužita konstanta
 * "#define HASH_FUNCTION_PARAM_A 1794967309", čímž dochází k přetečení proměnné
 * "size_t hash" při generování hashů pro klíče. Z tohoto důvodu některé testy
 * odhalily chyby způsobené právě přetečením proměnné "size_t hash". Ve chvíli,
 * kdy nedochází k rozšíření kapacity hashovací tabulky podle pravidla "3/5" při
 * přidávání nového klíče, se chyba neprojeví, program dokáže pracovat s
 * nesprávnými hodnotami hashe. Chyba se projevuje až ve chvíli, kdy dochází
 * k rozšíření kapacity hashovací tabulky podle pravidla "3/5". V tuto chvíli
 * dochází při snaze zpětně vyhledat klíče ke špatnému "rozšifrování" hashe,
 * což způsobuje vyvolání chyb spojených s nenalezením klíče. Tento problém je
 * hezky vidět na testech "HashMapRemovePreExpansion" a
 * "HashMapRemovePreExpansion", kdy po rozšíření hashovací tabulky nejsme
 * schopni zpětně dohledat a odebrat první 4 položky, které byly vloženy
 * ještě před navýšením kapacity, ale můžeme bez problému dohledat a odebrat
 * položky vložené až po navýšení kapacity.
 * 
 * Testy, které odhalují chyby, mají v názvu test case prefix "ERROR_".
*/

/* 
 * POZOR: Pokud by byla definovaná hodnota "HASH_FUNCTION_PARAM_A" například
 *        na hodnotu 5, tak by "hash_function()" začal fungovat správně,
 *        proměnné by nepřetékaly. V případě opravy této chyby je očekáváno,
 *        že testy s prefixem "ERROR_" (kromě ERROR_HashFunction) budou hlásit
 *        selhání, jelikož předpokládají chybný výsledek (je očekáván).
*/


/******************************************************************************/
/*                                                                            */
/*                       PŘÍPRAVA FIXTURES PRO TESTOVÁNÍ                      */
/*                                                                            */
/******************************************************************************/

/*                                                                            */
/*          Příprava fixture pro testování PRÁZDNÉ hashovací tabulky          */
/*                                                                            */

class EmptyTable : public ::testing::Test
{
    protected:
        // Deklarace ukazatele na hashovací tabulku
        hash_map_t* map;

        // Konstanty počtu položek a kapicity tohoto prostředí
        const int SIZE = 0;
        const int CAPACITY = 8;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření hashovací tabulky 
            map = hash_map_ctor();
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro hashovací tabulku
            hash_map_dtor(map);
        }
};


/*                                                                            */
/*         Příprava fixture pro testování NEPRÁZDNÉ hashovací tabulky         */
/*                                                                            */

class NonEmptyTableSmall : public ::testing::Test
{
    protected:
        // Deklarace ukazatele na hashovací tabulku
        hash_map_t* map;

        // Konstanty počtu položek a kapicity tohoto prostředí
        /* Pozn. Kapacita bude pouze "3" jelikož "(3/5)*8 > SIZE(3)" */
        const int SIZE = 3;
        const int CAPACITY = 8;

        // Deklrace proměnných pro uložení hodnot klíčů
        int hello_val, extreme_val, hashing_val;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření hashovací tabulky
            map = hash_map_ctor();

            // Přidání položek do hashovací tabulky a uložení jejích hodnot
            hash_map_put(map, "Hello~World!", 1);
            hello_val = 1;

            hash_map_put(map, "EXTREMEtable", -1);
            extreme_val = -1;

            hash_map_put(map, "??WhyHashing, ufff.", 0);
            hashing_val = 0;
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro hashovací tabulku
            hash_map_dtor(map);
        }
};

class NonEmptyTableWithExpansion : public ::testing::Test
{
    protected:
        // Deklarace ukazatele na hashovací tabulku
        hash_map_t* map;

        // Konstanty počtu položek a kapicity tohoto prostředí
        /* Pozn. Kapacita bude pouze "16" jelikož "(3/5)*8 < SIZE(8)" */
        const int SIZE = 8;
        const int CAPACITY = 16;

        // Deklrace proměnných pro uložení hodnot klíčů
        int horse_val, giraffe_val, elephant_val, monkey_val;
        int turtle_val, panda_val, octopus_val, fox_val;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření hashovací tabulky
            map = hash_map_ctor();

            // Přidání položek do hashovací tabulky a uložení jejích hodnot
            hash_map_put(map, "Pink Horse", 42);
            horse_val = 42;

            hash_map_put(map, "GreenGiraffe", -666);
            giraffe_val = -666;

            hash_map_put(map, "Purple ELEPHANT", -1);
            elephant_val = -1;

            hash_map_put(map, "Ye||0w_-_Monkey", 0);
            monkey_val = 0;

            hash_map_put(map, "Red? Turtle!", -42);
            turtle_val = -42;

            hash_map_put(map, "Blue~Panda", 666);
            panda_val = 666;

            hash_map_put(map, "Orange Octopus", 1);
            octopus_val = 1;

            hash_map_put(map, "<-S.i,l.v,e.r,F.o,x->", 1024);
            fox_val = 1024;
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro hashovací tabulku
            hash_map_dtor(map);
        }
};


/******************************************************************************/
/*                                                                            */
/*               TESTOVÁNÍ BEZ DŘÍVE VYTVOŘENÉ HASHOVACÍ TABULKY              */
/*                                                                            */
/******************************************************************************/

/*                                                                            */
/*                        TESTOVÁNÍ "hash_map_ctor()"                         */
/*                                                                            */

/*
 * Pozn. Test správné funkčnosti funkcí "hash_map_dtor()" a "hash_map_clear()",
 *       lze nejlépe otestovat pomocí nástroje 'valgrind'. Jelikož po sobě
 *       spuštění testů pomocí příkazu "ctest -C Debug" způsobí nemožnost
 *       detekovat případné memory leak, doporučuji spouštět v adresáři 'build'
 *       přímo pouze tento test pomocí:
 *       "valgrind --leak-check=full --track-origins=yes ./white_box_test".  
*/

TEST(NoTable, HashMapCtor)
{
    // Vytvoření hashovací tabulky
    hash_map_t* map = hash_map_ctor();

    // Ověření, že nebyl vrácen ukazatel NULL, resp. podařilo se alokovat paměť
    ASSERT_NE(map, nullptr);

    // Ověření, že kapacita nové hashovací tabulky je 8 položek
    EXPECT_EQ(hash_map_capacity(map), 8);

    // Ověření správné inicializace všech prvků struktury "hash_map_t"
    EXPECT_NE(map->index, nullptr);
    EXPECT_EQ(map->first, nullptr);
    EXPECT_EQ(map->last,  nullptr);
    EXPECT_NE(map->dummy, nullptr);
    EXPECT_EQ(map->allocated, HASH_MAP_INIT_SIZE);
    EXPECT_EQ(map->used,      0);

    // Uvolnění celé prázdné hashovací tabulky z paměti
    hash_map_dtor(map);
}


/*                                                                            */
/*                        TESTOVÁNÍ "hash_function()"                         */
/*                                                                            */

/* 
 * Pozn. Přestože tato funkce nemusí být testována, rozhodl jsem se na ní
 *       demonstrovat hlavní problém programu. Je tedy nutné doplnit deklaraci
 *       funkce "hash_function()", jelikož chybí v hlavičkovém souboru
 *       "white_box_code.h"
 * 
 * Pozn. vzorec pro výpočet hashe je následující (last update: 2024-02-14):
 *             - HASH_FUNCTION_PARAM_A = 1794967309; HASH_FUNCTION_PARAM_B = 7
 *            => hash += HASH_FUNCTION_PARAM_A*str[idx] + HASH_FUNCTION_PARAM_B;
*/

size_t hash_function(const char* str);

/*           ~~~~~ "HashFunction": TESTEM BYLA NALEZENA CHYBA ~~~~~           */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*                    "HASH_FUNCTION_PARAM_A = 1794967309"                    */
/*   (pozn. použit "EXPECT_NE()" kvůli zdůraznění rozdílu oproti očekávání)   */

/* 
 * POZOR: Tento test bude vyhodnocen jako správný, přestože bude změněn
 *        např. "HASH_FUNCTION_PARAM_A = 5" (což by vyústilo v opravu hashovací 
 *        techniky), jelikož je použito EXPECT_NE().
 *        Tento test tedy není univerzální, je pouze demonstrativní pro aktuální
 *        situaci
*/

TEST(NoTable, ERROR_HashFunction)
{
    // Deklarace proměnné pro ukládání hashů
    size_t hash = 0;

    // Klíč 'a' by měl na základě implementace funkce hash 174 111 828 980
    hash = hash_function("a");
    EXPECT_NE(hash, 174111828980);

    // Klíč 'A' by měl na základě implementace funkce hash 116 672 875 092
    hash = hash_function("A");
    EXPECT_NE(hash, 116672875092);
}


/******************************************************************************/
/*                                                                            */
/*                    TESTOVÁNÍ PRÁZDNÉ HASHOVACÍ TABULKY                     */
/*                                                                            */
/******************************************************************************/

/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_size()"                        */
/*                                                                            */

TEST_F(EmptyTable, HashMapSize)
{
    // Ověření existence hashovací tabulky
    ASSERT_NE(map, nullptr);

    // Zjištění velikosti prázdné předvytvořené tabulky
    size_t map_size = hash_map_size(map);

    // Rozměr prázdné hashovací tabulky by měl být 0
    EXPECT_EQ(map_size, SIZE);
    EXPECT_EQ(map_size, map->used);

}


/*                                                                            */
/*                       TESTOVÁNÍ "hash_map_reserve()"                       */
/*                                                                            */

TEST_F(EmptyTable, HashMapReserveNegative)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na zápornou hodnotu, tedy -5
    // Malloc vrátí NULL, což vyvodí MEMORY_ERROR
    EXPECT_EQ(hash_map_reserve(map, -5), MEMORY_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Ověření, že v tabulce je stále 0 položek a kapacita se nezměnila
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(EmptyTable, HashMapReserveEqual)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na stejnou hodnotu, tedy 0.
    // Je očekáván OK a změna kapacity z 8 na 0 položek
    EXPECT_EQ(hash_map_reserve(map, 0), OK);
    EXPECT_EQ(hash_map_capacity(map), 0);
    EXPECT_EQ(map->allocated, 0);

    // Ověření, že v tabulce je stále 0 položek, ale kapacita se změnila na 0
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, 0);
}

TEST_F(EmptyTable, HashMapReserveMore)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na více položek, na 4 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 4), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 4);
    EXPECT_EQ(map->allocated, SIZE + 4);

    // Pokus rozšířit kapacitu na více položek, na 8 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 8), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 8);
    EXPECT_EQ(map->allocated, SIZE + 8);

    // Pokus rozšířit kapacitu na více položek, na 14 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 14), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 14);
    EXPECT_EQ(map->allocated, SIZE + 14);

    // Ověření, že v tabulce je stále 0 položek, ale kapacita se navýšila na 14
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, 14);
}


/*                                                                            */
/*                       TESTOVÁNÍ "hash_map_remove()"                        */
/*                                                                            */

TEST_F(EmptyTable, HashMapRemoveInvalid)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus odstranit položku s neexistujícím klíčem -> KEY_ERROR
    EXPECT_EQ(hash_map_remove(map, "Hash Hell!"), KEY_ERROR);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_put()"                         */
/*                                                                            */

TEST_F(EmptyTable, HashMapPut)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Vložení první položky a kontrola stavu tabulky po jejím vložení
    hash_map_put(map, "abc", 159);
    EXPECT_EQ(hash_map_size(map), SIZE + 1);
    EXPECT_EQ(hash_map_size(map), map->used);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(hash_map_capacity(map), map->allocated);

    // Vložení druhé položky a kontrola stavu tabulky po jejím vložení
    hash_map_put(map, " acb", 357);
    EXPECT_EQ(hash_map_size(map), SIZE + 2);
    EXPECT_EQ(hash_map_size(map), map->used);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(hash_map_capacity(map), map->allocated);

    // Vložení třetí položky a kontrola stavu tabulky po jejím vložení
    hash_map_put(map, "bac", 753);
    EXPECT_EQ(hash_map_size(map), SIZE + 3);
    EXPECT_EQ(hash_map_size(map), map->used);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(hash_map_capacity(map), map->allocated);

    // Vložení čtvrté položky a kontrola stavu tabulky po jejím vložení
    hash_map_put(map, "bca", 951);
    EXPECT_EQ(hash_map_size(map), SIZE + 4);
    EXPECT_EQ(hash_map_size(map), map->used);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(hash_map_capacity(map), map->allocated);

    // Vložení páté položky a kontrola stavu tabulky po jejím vložení
    hash_map_put(map, "cab", -66);
    EXPECT_EQ(hash_map_size(map), SIZE + 5);
    EXPECT_EQ(hash_map_size(map), map->used);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(hash_map_capacity(map), map->allocated);

    // Vložení páté položky a kontrola stavu tabulky po jejím vložení
    /* 
     * Pozn. "USED / ALLOCATED"
     *          - Axiom:  HASH_MAP_REALLOCATION_THRESHOLD = 3/5 = 0,6
     *          - Now:    5/8 = 0,625
     *         => Result: 0,625 > 0,6 -> CAPACITY * 2
    */
    hash_map_put(map, "cba", 666);
    EXPECT_EQ(hash_map_size(map), SIZE + 6);
    EXPECT_EQ(hash_map_size(map), map->used);
    EXPECT_EQ(hash_map_capacity(map), 2 * CAPACITY);
    EXPECT_EQ(hash_map_capacity(map), map->allocated);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_pop()"                         */
/*                                                                            */

TEST_F(EmptyTable, HashMapPopInvalid)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokusí se vyjmout neexistující klíče z tabulky a ověří její stav
    /* Pozn. "pop_val" by měla zůstat nezměněna; "err_num = KEY_ERROR"   */
    int pop_value = -1;
    int err_num = 0;

    err_num = hash_map_pop(map, "baba", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, -1);

    err_num = hash_map_pop(map, "1,2<35$l/8", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, -1);

    err_num = hash_map_pop(map, "0", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, -1);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_get()"                         */
/*                                                                            */

TEST_F(EmptyTable, HashMapGetInvalid)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokusí se vyhledaz neexistující klíče v tabulce
    /* Pozn. "get_value" by měla zůstat nezměněna; "err_num = KEY_ERROR"   */
    int get_value = -42;
    int get_value_checker = -42;
    int err_num = 0;

    err_num = hash_map_pop(map, "baba", &get_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(get_value, get_value_checker);

    err_num = hash_map_pop(map, "1,2<35$l/8", &get_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(get_value, get_value_checker);

    err_num = hash_map_pop(map, "0", &get_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(get_value, get_value_checker);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                      TESTOVÁNÍ "hash_map_contains()"                       */
/*                                                                            */

TEST_F(EmptyTable, HashMapContainsFalse)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Ověření, že se dané klíče v tabulce NEvyskytují
    EXPECT_FALSE(hash_map_contains(map, "baba"));
    EXPECT_FALSE(hash_map_contains(map, "1,2<35$l/8"));
    EXPECT_FALSE(hash_map_contains(map, "0"));

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/******************************************************************************/
/*                                                                            */
/*        TESTOVÁNÍ NEPRÁZDNÉ HASHOVACÍ TABULKY BEZ ROZŠÍŘENÍ "SMALL"         */
/*                                                                            */
/******************************************************************************/

/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_size()"                        */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapSize)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Rozměr prázdné hashovací tabulky by měl být 3
    size_t map_size = hash_map_size(map);
    EXPECT_EQ(map_size, SIZE);
    EXPECT_EQ(map_size, map->used);

    // Navýšení kapacity na 42 položek a ověření, že počet položek je nezměněn
    hash_map_reserve(map, 42);
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, 42);
}


/*                                                                            */
/*                       TESTOVÁNÍ "hash_map_reserve()"                       */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapReserveNegative)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na zápornou hodnotu, tedy -999
    // Malloc vrátí NULL, což vyvodí MEMORY_ERROR
    EXPECT_EQ(hash_map_reserve(map, -999), MEMORY_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapReserveLess1)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus zmenšit kapacitu na pouhou 1 položku
    // Toto je ilegální jelikož "1 < SIZE(3)"" -> vznikne VALUE_ERROR
    EXPECT_EQ(hash_map_reserve(map, 1), VALUE_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapReserveLess2)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus zmenšit kapacitu na pouhých 0 položek
    // Toto je ilegální jelikož "0 < SIZE(3)"" -> vznikne VALUE_ERROR
    EXPECT_EQ(hash_map_reserve(map, 0), VALUE_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapReserveEqual)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na stejnou hodnotu, tedy 8.
    // Je očekáván OK a a jelikož máme plnou tabulku kapacita se nezmění
    EXPECT_EQ(hash_map_reserve(map, 8), OK);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapReserveMore1)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na více položek, na 33 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 33), OK);
    EXPECT_EQ(hash_map_capacity(map), 33);
    EXPECT_EQ(map->allocated, 33);

    // Ověřená nezměného počtu položek a navýšení kapacity na 33
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, 33);
}

TEST_F(NonEmptyTableSmall, HashMapReserveMore2)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na více položek, na 9 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 9), OK);
    EXPECT_EQ(hash_map_capacity(map), 9);
    EXPECT_EQ(map->allocated, 9);

    // Ověřená nezměného počtu položek a navýšení kapacity na 9
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, 9);
}


/*                                                                            */
/*                       TESTOVÁNÍ "hash_map_remove()"                        */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapRemoveInvalid)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus odstranit položku s neexistujícím klíčem -> KEY_ERROR
    EXPECT_EQ(hash_map_remove(map, "IVS is useful!"), KEY_ERROR);

    // Pokus odstranit položku s neexistujícím klíčem -> KEY_ERROR
    EXPECT_EQ(hash_map_remove(map, "42"), KEY_ERROR);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapRemoveAll)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus odstranit všechny položky v hashovací tabulce
    EXPECT_EQ(hash_map_remove(map, "EXTREMEtable"), OK);
    EXPECT_EQ(hash_map_remove(map, "Hello~World!"), OK);
    EXPECT_EQ(hash_map_remove(map, "??WhyHashing, ufff."), OK);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_put()"                         */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapPut)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus přidat duplikát položky se stejným klíčem i hodnotou
    EXPECT_EQ(hash_map_put(map, "Hello~World!", 1), KEY_ALREADY_EXISTS);

    // Pokus přidat duplikát položky se stejným klíčem ale jinou hodnotou
    EXPECT_EQ(hash_map_put(map, "Hello~World!", 4000526), KEY_ALREADY_EXISTS);

    // Přidání položky s jedinečným klíčem ale NEjedinečnou hodnotou - tj. OK
    EXPECT_EQ(hash_map_put(map, "It is legal!", 1), OK);

    // Přidání položky s jedinečným klíčem a jedinečnou hodnotou - tj. OK
    EXPECT_EQ(hash_map_put(map, "I am unique", 542), OK);

    // Ověřená změného počtu položek a nezměněné kapacity;
    EXPECT_EQ(map->used, SIZE + 2);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_pop()"                         */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapPopAll)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Vyjme záznam všech hodnot z hashovací tabulky a ověří její stav
    int pop_value = 0;
    int err_num = 0;

    err_num = hash_map_pop(map, "Hello~World!", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, hello_val);

    err_num = hash_map_pop(map, "EXTREMEtable", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, extreme_val);

    err_num = hash_map_pop(map, "??WhyHashing, ufff.", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, hashing_val);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapPopInvalid)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokusí se vyjmout neexistující klíče z předvyplněné tabulky a 
    // ověří její stav
    /* Pozn. "pop_value" by měla zůstat nezměněna; "err_num = KEY_ERROR"   */
    int pop_value = -42;
    int pop_value_checker = -42;
    int err_num = 0;

    err_num = hash_map_pop(map, "abraka dabra", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, pop_value_checker);

    err_num = hash_map_pop(map, "*-/+><*#|", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, pop_value_checker);

    err_num = hash_map_pop(map, "666", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, pop_value_checker);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapPopAlreadyRemoved)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Vyjme existující záznam z hashovací tabulky
    int pop_value = 0;
    int err_num = 0;

    err_num = hash_map_pop(map, "EXTREMEtable", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, extreme_val);


    // Pokusíme se vyjmout stejný záznam ještě jednou
    /* Pozn. "err_num" se změní na KEY_ERROR, ale hodnota uložená v "pop_value"
     *       po prvotním úspěšném vyjmutí se nezmění, takže s ní lze dále 
     *       pracovat => TO NENÍ ÚPLNĚ ŠŤASTNÉ ŘEŠENÍ, každopádně specifikace
     *       se o tom, co se má při chybě KEY_ERROR stát s pop_value nezmiňuje.
    */
    err_num = hash_map_pop(map, "EXTREMEtable", &pop_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(pop_value, extreme_val);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_get()"                         */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapGetValid)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Získá hodnotu klíče z hashovací tabulky
    int get_value = 0;
    int err_num = 0;

    err_num = hash_map_get(map, "Hello~World!", &get_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(get_value, hello_val);

    err_num = hash_map_get(map, "EXTREMEtable", &get_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(get_value, extreme_val);

    err_num = hash_map_get(map, "??WhyHashing, ufff.", &get_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(get_value, hashing_val);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapGetInvalid)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokusí se vyjmout neexistující klíče z předvyplněné tabulky a 
    // ověří její stav
    /* Pozn. "get_value" by měla zůstat nezměněna; "err_num = KEY_ERROR"   */
    int get_value = -42;
    int get_value_checker = -42;
    int err_num = 0;

    err_num = hash_map_get(map, "abraka dabra", &get_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(get_value, get_value_checker);

    err_num = hash_map_get(map, "*-/+><*#|", &get_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(get_value, get_value_checker);

    err_num = hash_map_get(map, "666", &get_value);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_EQ(get_value, get_value_checker);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                      TESTOVÁNÍ "hash_map_contains()"                       */
/*                                                                            */

TEST_F(NonEmptyTableSmall, HashMapContainsTrue)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Ověření, že se dané klíče v tabulce NEvyskytují
    EXPECT_TRUE(hash_map_contains(map, "Hello~World!"));
    EXPECT_TRUE(hash_map_contains(map, "EXTREMEtable"));
    EXPECT_TRUE(hash_map_contains(map, "??WhyHashing, ufff."));

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableSmall, HashMapContainsFalse)
{
    // Ověření existence prázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Ověření, že se dané klíče v tabulce NEvyskytují
    EXPECT_FALSE(hash_map_contains(map, "abraka dabra"));
    EXPECT_FALSE(hash_map_contains(map, "*-/+><*#|"));
    EXPECT_FALSE(hash_map_contains(map, "666"));

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/******************************************************************************/
/*                                                                            */
/*   TESTOVÁNÍ NEPRÁZDNÉ HASHOVACÍ TABULKY PO ROZŠÍŘENÍ - "WITH EXPANSION"    */
/*                                                                            */
/******************************************************************************/

/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_size()"                        */
/*                                                                            */

TEST_F(NonEmptyTableWithExpansion, HashMapSize)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Rozměr prázdné hashovací tabulky by měl být 8
    size_t map_size;
    map_size = hash_map_size(map);
    EXPECT_EQ(map_size, SIZE);
    EXPECT_EQ(map_size, map->used);
}


/*                                                                            */
/*                       TESTOVÁNÍ "hash_map_reserve()"                       */
/*                                                                            */

TEST_F(NonEmptyTableWithExpansion, HashMapReserveNegative)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na zápornou hodnotu, tedy -42
    // Malloc vrátí NULL, což vyvodí MEMORY_ERROR
    EXPECT_EQ(hash_map_reserve(map, -42), MEMORY_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Ověření, že v hashovací tabulce je stále 8 položek
    EXPECT_EQ(map->used, SIZE);
}

TEST_F(NonEmptyTableWithExpansion, HashMapReserveLess)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus zmenšit kapacitu na pouhých 5 položek - vznikne VALUE_ERROR
    EXPECT_EQ(hash_map_reserve(map, 5), VALUE_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Pokus zmenšit kapacitu na pouhých 0 položek - vznikne VALUE_ERROR
    EXPECT_EQ(hash_map_reserve(map, 0), VALUE_ERROR);
    EXPECT_EQ(hash_map_capacity(map), CAPACITY);
    EXPECT_EQ(map->allocated, CAPACITY);

    // Pokus o zmenšení kapacity na pouhých 10 položek - legální OK
    EXPECT_EQ(hash_map_reserve(map, 10), OK);
    EXPECT_EQ(hash_map_capacity(map), 10);
    EXPECT_EQ(map->allocated, 10);

    // Ověření, že v hashovací tabulce je stále 8 položek
    EXPECT_EQ(map->used, SIZE);
}

TEST_F(NonEmptyTableWithExpansion, HashMapReserveEqual)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na stejnou hodnotu, tedy 16.
    // Je očekáván OK a změna kapacity z 16 na 16 položek
    EXPECT_EQ(hash_map_reserve(map, 16), OK);
    EXPECT_EQ(hash_map_capacity(map),16);
    EXPECT_EQ(map->allocated, 16);

    // Ověření, že v hashovací tabulce je stále 8 položek
    EXPECT_EQ(map->used, SIZE);
}

TEST_F(NonEmptyTableWithExpansion, HashMapReserveMore)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus rozšířit kapacitu na více položek, na 18 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 18), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 10);
    EXPECT_EQ(map->allocated, SIZE + 10);

    // Pokus rozšířit kapacitu na více položek, na 23 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 23), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 15);
    EXPECT_EQ(map->allocated, SIZE + 15);

    // Pokus rozšířit kapacitu na více položek, na 24 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 24), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 16);
    EXPECT_EQ(map->allocated, SIZE + 16);

    // Pokus rozšířit kapacitu na více položek, na 25 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 25), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 17);
    EXPECT_EQ(map->allocated, SIZE + 17);

    // Pokus rozšířit kapacitu na více položek, na 998 - očekáván OK
    EXPECT_EQ(hash_map_reserve(map, 98), OK);
    EXPECT_EQ(hash_map_capacity(map), SIZE + 90);
    EXPECT_EQ(map->allocated, SIZE + 90);

    // Ověření, že v hashovací tabulce je stále 8 položek
    EXPECT_EQ(map->used, SIZE);
}


/*                                                                            */
/*                       TESTOVÁNÍ "hash_map_remove()"                        */
/*                                                                            */

TEST_F(NonEmptyTableWithExpansion, HashMapRemoveInvalid)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus odstranit položku s neexistujícím klíčem -> KEY_ERROR
    EXPECT_EQ(hash_map_remove(map, "ProjectsAreFun!"), KEY_ERROR);

    // Pokus odstranit položku s neexistujícím klíčem -> KEY_ERROR
    EXPECT_EQ(hash_map_remove(map, "-42"), KEY_ERROR);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*        ~~~~~ "HashMapRemoveFirst": TESTEM BYLA NALEZENA CHYBA ~~~~~        */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*                    "HASH_FUNCTION_PARAM_A = 1794967309"                    */
/*      (pozn. použit "EXPECT_NE()": očekáván "OK", získán "KEY_ERROR")       */

TEST_F(NonEmptyTableWithExpansion, ERROR_HashMapRemoveFirst)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);
    
    // Proměnná pro návratovou hodnotu
    int err_num = hash_map_remove(map, "Pink Horse");

    // Pokus odstranit položku s nejkratším klíčem s nejnižším hashem
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);

    // Ověřená nezměného počtu položek a kapacity 
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*    ~~~~~ "HashMapRemovePreExpansion": TESTEM BYLA NALEZENA CHYBA ~~~~~     */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*                    "HASH_FUNCTION_PARAM_A = 1794967309"                    */
/*      (pozn. použit "EXPECT_NE()": očekáván "OK", získán "KEY_ERROR")       */

TEST_F(NonEmptyTableWithExpansion, ERROR_HashMapRemovePreExpansion)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Deklarace proměnné pro návratovou hodnotu "hash_map_remove()"
    int err_num = -1;

    // 1. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "Pink Horse");
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);

    // 2. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "GreenGiraffe");
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);

    // 3. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "Purple ELEPHANT");
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);

    // 4. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "Ye||0w_-_Monkey");
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);

    // 5. přidaný klíč "Pink Horse" - přidána již po expanzce, očekáván proto OK
    err_num = hash_map_remove(map, "Red? Turtle!");
    EXPECT_EQ(err_num, OK);

    // Ověřená nezměného počtu položek a kapacity 
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


TEST_F(NonEmptyTableWithExpansion, HashMapRemovePostExpansion)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Deklarace proměnné pro návratovou hodnotu "hash_map_remove()"
    int err_num;

    // 6. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "Blue~Panda");
    EXPECT_EQ(err_num, OK);

    // 7. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "Orange Octopus");
    EXPECT_EQ(err_num, OK);

    // 8. přidaný klíč "Pink Horse"
    err_num = hash_map_remove(map, "<-S.i,l.v,e.r,F.o,x->");
    EXPECT_EQ(err_num, OK);

    // Ověřená nezměného počtu položek a kapacity 
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}

TEST_F(NonEmptyTableWithExpansion, HashMapRemoveLast)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus odstranit položku s nejdelším klíčem s nejvyšším hashem
    EXPECT_EQ(hash_map_remove(map, "<-S.i,l.v,e.r,F.o,x->"), OK);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_put()"                         */
/*                                                                            */

/*      ~~~~~ "HashMapPutPreExpansion": TESTEM BYLA NALEZENA CHYBA ~~~~~      */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*            "HASH_FUNCTION_PARAM_A = 1794967309", což způsobuje             */
/*                     MOŽNOST VLOŽENÍ DUPLICITNÍCH KLÍČŮ                     */
/*     (pozn. "EXPECT_NE()": očekáván "KEY_ALREADY_EXISTS", získán "OK")      */

TEST_F(NonEmptyTableWithExpansion, ERROR_HashMapPutPreExpansion)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    /* Toto by správně měla být ilegální akce, ale kvůli chybě při hashování 
       je způsobena ztráta klíče "Pink Horse" a "GreenGiraffe" a tím pádem je 
       možné vložit novou hodnotu s klíčem "Pink Horse" a "GreenGiraffe", 
       přestože při správném chování by se  mělo jednat o duplikát            */
    // Pokus přidat duplikát položky se stejným klíčem i hodnotou
    EXPECT_NE(hash_map_put(map, "Pink Horse", 42), KEY_ALREADY_EXISTS);

    // Pokus přidat duplikát položky se stejným klíčem ale jinou hodnotou
    EXPECT_NE(hash_map_put(map, "GreenGiraffe", 4000526), KEY_ALREADY_EXISTS);

    /* Následující akce jsou již legální, jelikož dané klíče zatím neexistují */
    // Přidání položky s jedinečným klíčem ale NEjedinečnou hodnotou - tj. OK
    EXPECT_EQ(hash_map_put(map, "I am legal!", -666), OK);

    // Přidání položky s jedinečným klíčem a jedinečnou hodnotou - tj. OK
    EXPECT_EQ(hash_map_put(map, "I am unique too!!!", 542), OK);

    /* Zde opět dochází k chybnému chování, správně by se totiž měly přidat
       pouze 2 položky, a tedy by počet položek SIZE měl být navýšen pouze o 2.
       Namísto toho je navýšen o 4, jelikož dochází k přidání duplicit klíče
       "Pink Horse" a "GreenGiraffe" kvůli ztrátě jejich klíčů                */
    // Ověřená změného počtu položek a nezměněné kapacity;
    EXPECT_NE(map->used, SIZE + 2);
}

TEST_F(NonEmptyTableWithExpansion, HashMapPutPostExpansion)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Pokus přidat duplikát položky se stejným klíčem i hodnotou
    EXPECT_EQ(hash_map_put(map, "Orange Octopus", 1), KEY_ALREADY_EXISTS);

    // Pokus přidat duplikát položky se stejným klíčem ale jinou hodnotou
    EXPECT_EQ(hash_map_put(map, "Orange Octopus", 123), KEY_ALREADY_EXISTS);

    // Přidání položky s jedinečným klíčem ale NEjedinečnou hodnotou - tj. OK
    EXPECT_EQ(hash_map_put(map, "I am legal!", 1024), OK);

    // Přidání položky s jedinečným klíčem a jedinečnou hodnotou - tj. OK
    EXPECT_EQ(hash_map_put(map, "I am unique too!!!", 542), OK);

    // Ověřená změného počtu položek a nezměněné kapacity;
    EXPECT_EQ(map->used, SIZE + 2);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_pop()"                         */
/*                                                                            */

/*          ~~~~~ "HashMapPopAll": TESTEM BYLA NALEZENA CHYBA ~~~~~           */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*                    "HASH_FUNCTION_PARAM_A = 1794967309"                    */
/*  (pozn. "EXPECT_NE()": očekáván "OK", získán "KEY_ERROR" pro položky 1-4 ) */
/*     (pozn. "EXPECT_EQ()": očekáván "OK", získán "OK" pro položky 5-8 )     */

TEST_F(NonEmptyTableWithExpansion, ERROR_HashMapPopAll)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Vyjme záznam všech hodnot z hashovací tabulky a ověří jejich stav
    int pop_value = 0;
    int err_num = 0;

    /* 
     * Pozn. "err_num" se změní na KEY_ERROR, ale hodnota uložená v "pop_value"
     *       bude v případě přetečení 0. Toto zavádějící řešení funkce "pop" 
     *       demonstruje testový případ pro klíč "Ye||0w_-_Monkey". Přestože
     *       návratovou hodnotou je KEY_ERROR a klíč nebyl nalezen, porovnání
     *       hodnoty "pop_value" a skutečnou hodnotou klíče "Ye||0w_-_Monkey"
     *       bude vyhodnocen jako pravdivý, jelikož klíč "Ye||0w_-_Monkey" má
     *       hodnotu 0. Na 0 bl právě nastaven take "pop_value", který 
     *       přistoupilna prázdný index. Tedy by v provozu nebylo možné poznat, 
     *       zda je hodnota 'value' funkce "pop" důvěryhodná či ne.
    */
    err_num = hash_map_pop(map, "Pink Horse", &pop_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_TRUE(pop_value != horse_val);

    err_num = hash_map_pop(map, "GreenGiraffe", &pop_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_TRUE(pop_value != giraffe_val);

    err_num = hash_map_pop(map, "Purple ELEPHANT", &pop_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_TRUE(pop_value != elephant_val);

    // použit opačný EXPECT_FALSE() pro ukázku nevhodné implementace funkce "pop"
    err_num = hash_map_pop(map, "Ye||0w_-_Monkey", &pop_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);              // klíč nebyl nalezen
    EXPECT_FALSE(pop_value != monkey_val);      // přesto hodnota 'value' souhlasí


    /* Test položek 5-8, které byly přidány po rozšířením tabulky,
       které v případě přetečení proměnné pro hash ztaím nebudou ztaceny */
    err_num = hash_map_pop(map, "Red? Turtle!", &pop_value);
    EXPECT_EQ(err_num, OK); 
    EXPECT_EQ(pop_value, turtle_val);

    err_num = hash_map_pop(map, "Blue~Panda", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, panda_val);

    err_num = hash_map_pop(map, "Orange Octopus", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, octopus_val);

    hash_map_pop(map, "<-S.i,l.v,e.r,F.o,x->", &pop_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(pop_value, fox_val);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                         TESTOVÁNÍ "hash_map_get()"                         */
/*                                                                            */

/*          ~~~~~ "HashMapGetAll": TESTEM BYLA NALEZENA CHYBA ~~~~~           */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*                    "HASH_FUNCTION_PARAM_A = 1794967309"                    */
/*  (pozn. "EXPECT_NE()": očekáván "OK", získán "KEY_ERROR" pro položky 1-4 ) */
/*     (pozn. "EXPECT_EQ()": očekáván "OK", získán "OK" pro položky 5-8 )     */

TEST_F(NonEmptyTableWithExpansion, ERROR_HashMapGetAll)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    // Získá záznam hodnot všech klíčů z hashovací tabulky
    int get_value = 0;
    int err_num = 0;

    /* 
     * Pozn. Pro tento test platí stejné zdůvodnění jako pro "HashMapPopAll"
    */
    err_num = hash_map_pop(map, "Pink Horse", &get_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_TRUE(get_value != horse_val);

    err_num = hash_map_pop(map, "GreenGiraffe", &get_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_TRUE(get_value != giraffe_val);

    err_num = hash_map_pop(map, "Purple ELEPHANT", &get_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);
    EXPECT_TRUE(get_value != elephant_val);

    // použit opačný EXPECT_FALSE() pro ukázku nevhodné implementace funkce "get"
    err_num = hash_map_pop(map, "Ye||0w_-_Monkey", &get_value);
    ASSERT_NE(err_num, OK);
    EXPECT_EQ(err_num, KEY_ERROR);              // klíč nebyl nalezen
    EXPECT_FALSE(get_value != monkey_val);      // přesto hodnota 'value' souhlasí


    /* Test položek 5-8, které byly přidány po rozšířením tabulky,
       které v případě přetečení proměnné pro hash ztaím nebudou ztaceny */
    err_num = hash_map_pop(map, "Red? Turtle!", &get_value);
    EXPECT_EQ(err_num, OK); 
    EXPECT_EQ(get_value, turtle_val);

    err_num = hash_map_pop(map, "Blue~Panda", &get_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(get_value, panda_val);

    err_num = hash_map_pop(map, "Orange Octopus", &get_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(get_value, octopus_val);

    hash_map_pop(map, "<-S.i,l.v,e.r,F.o,x->", &get_value);
    EXPECT_EQ(err_num, OK);
    EXPECT_EQ(get_value, fox_val);

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}


/*                                                                            */
/*                      TESTOVÁNÍ "hash_map_contains()"                       */
/*                                                                            */

/*         ~~~~~ "HashMapContains": TESTEM BYLA NALEZENA CHYBA ~~~~~          */
/*        DOCHÁZÍ K PŘETEČENÍ "size_t hash" KVŮLI PŘILIŠ VELKÉ HODNOTĚ        */
/*                    "HASH_FUNCTION_PARAM_A = 1794967309"                    */
/*  (pozn. "EXPECT_NE()": očekáván "OK", získán "KEY_ERROR" pro položky 1-4 ) */
/*     (pozn. "EXPECT_EQ()": očekáván "OK", získán "OK" pro položky 5-8 )     */

TEST_F(NonEmptyTableWithExpansion, ERROR_HashMapContains)
{
    // Ověření existence NEprázdné hashovací tabulky
    ASSERT_NE(map, nullptr);
    ASSERT_EQ(map->used, SIZE);
    ASSERT_EQ(map->allocated, CAPACITY);

    /* 
     * Pozn. Pro tento test platí podobné zdůvodnění jako pro "HashMapPopAll"
     *       Kvůli chybnému přetečení proměnné "hash" se předpokládá "FALSE"
    */
    EXPECT_FALSE(hash_map_contains(map, "Pink Horse"));
    EXPECT_FALSE(hash_map_contains(map, "GreenGiraffe"));
    EXPECT_FALSE(hash_map_contains(map, "Purple ELEPHANT"));

    /* Jelikož funkce vrací "true/false" tak tentokrát i pro "Ye||0w_-_Monkey" 
       vrátí "FALSE" -> tedy nevrací konkrétní číselnou hodnotu
       (pomineme případ, že se FALSE dá interpretovat jako 0)                 */
    EXPECT_FALSE(hash_map_contains(map, "Ye||0w_-_Monkey"));

    /* Test položek 5-8, které byly přidány po rozšířením tabulky,
       které v případě přetečení proměnné pro hash ztaím nebudou ztaceny 
       Zde, se přetečení proměnné "hash" neprojeví, proto "TRUE"         */
    EXPECT_TRUE(hash_map_contains(map, "Red? Turtle!"));
    EXPECT_TRUE(hash_map_contains(map, "Blue~Panda"));
    EXPECT_TRUE(hash_map_contains(map, "Orange Octopus"));
    EXPECT_TRUE(hash_map_contains(map, "<-S.i,l.v,e.r,F.o,x->"));

    // Ověřená nezměného počtu položek a kapacity
    EXPECT_EQ(map->used, SIZE);
    EXPECT_EQ(map->allocated, CAPACITY);
}





/*** Konec souboru white_box_tests.cpp ***/