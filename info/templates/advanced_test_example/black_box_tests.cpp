//========== Copyright (c) 2024, FIT VUT Brno, All rights reserved. ==========//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JAN KALINA <xkalinj00@stud.fit.vutbr.cz>
// $Date:       $2024-02-14
//
//============================================================================//

/**
 * @file black_box_tests.cpp
 * @author JAN KALINA
 * 
* @date 2024-02-26 Last update
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>
#include "gtest/gtest.h"
#include "red_black_tree.h"

//============================================================================//
//
// ** ZDE DOPLŇTE TESTY **
//
// Zde doplňte testy Red-Black Tree, testujte následující:
// 1. Veřejné rozhraní stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chování těchto metod testuje pro prázdný i neprázdný strom.
// 2. Axiomy (tedy vždy platné vlastnosti) Red-Black Tree:
//    - Všechny listové uzly stromu jsou VŽDY černé.
//    - Každý červený uzel může mít POUZE černé potomky.
//    - Všechny cesty od každého listového uzlu ke kořeni stromu obsahují
//      *STEJNÝ* počet černých uzlů.
//
//============================================================================//

/******************************************************************************/
/*                                                                            */
/*                       PŘÍPRAVA FIXTURES PRO TESTOVÁNÍ                      */
/*                                                                            */
/******************************************************************************/

/*                                                                            */
/*         Příprava fixture pro testování prázdného stromu (EmptyTree)        */
/*                                                                            */

class EmptyTree : public ::testing::Test
{
    protected:
        // Deklarace ukazatele pro Red-Black Tree
        BinaryTree *tree;

        // Příprava počátečního testovacího prostředí
        // Pozn. "override" zaručí správnou interpretaci velkých písmen v "SetUp"
        virtual void SetUp() override
        {
            // Vytvoření stromu
            tree = new BinaryTree();
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro strom
            // Pozn. "delete" je vlastně "free()" akorát v C++
            delete tree;
        }
};


/*                                                                            */
/*      Příprava fixture pro testování neprázdného stromu (NonEmptyTree)      */
/*                                                                            */

class NonEmptyTree : public ::testing::Test
{
    protected:
        // Deklarace ukazatele pro Red-Black Tree
        BinaryTree *tree;

        // Deklarace ukazatele pro uzly
        BinaryTree::Node_t* node10;
        BinaryTree::Node_t* nodeN10;
        BinaryTree::Node_t* node20;
        BinaryTree::Node_t* nodeN20;
        BinaryTree::Node_t* node30;
        BinaryTree::Node_t* nodeN30;
        BinaryTree::Node_t* node40;
        BinaryTree::Node_t* nodeN40;
        BinaryTree::Node_t* node50;
        BinaryTree::Node_t* nodeN50;
        BinaryTree::Node_t* node0;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření stromu
            tree = new BinaryTree();

            // Vytvoření nového uzlu s hodnotou "10"
            std::pair<bool, BinaryTree::Node_t*> node10_pair = tree->InsertNode(10);
            node10 = node10_pair.second;

            // Vytvoření nového uzlu s hodnotou "-10"
            std::pair<bool, BinaryTree::Node_t*> nodeN10_pair = tree->InsertNode(-10);
            nodeN10 = nodeN10_pair.second;

            // Vytvoření nového uzlu s hodnotou "20"
            std::pair<bool, BinaryTree::Node_t*> node20_pair = tree->InsertNode(20);
            node20 = node20_pair.second;

            // Vytvoření nového uzlu s hodnotou "-20"
            std::pair<bool, BinaryTree::Node_t*> nodeN20_pair = tree->InsertNode(-20);
            nodeN20 = nodeN20_pair.second;

            // Vytvoření nového uzlu s hodnotou "30"
            std::pair<bool, BinaryTree::Node_t*> node30_pair = tree->InsertNode(30);
            node30 = node30_pair.second;

            // Vytvoření nového uzlu s hodnotou "-30"
            std::pair<bool, BinaryTree::Node_t*> nodeN30_pair = tree->InsertNode(-30);
            nodeN30 = nodeN30_pair.second;

            // Vytvoření nového uzlu s hodnotou "40"
            std::pair<bool, BinaryTree::Node_t*> node40_pair = tree->InsertNode(40);
            node40 = node40_pair.second;

            // Vytvoření nového uzlu s hodnotou "-40"
            std::pair<bool, BinaryTree::Node_t*> nodeN40_pair = tree->InsertNode(-40);
            nodeN40 = nodeN40_pair.second;

            // Vytvoření nového uzlu s hodnotou "50"
            std::pair<bool, BinaryTree::Node_t*> node50_pair = tree->InsertNode(50);
            node50 = node50_pair.second;

            // Vytvoření nového uzlu s hodnotou "-50"
            std::pair<bool, BinaryTree::Node_t*> nodeN50_pair = tree->InsertNode(-50);
            nodeN50 = nodeN50_pair.second;

            // Vytvoření nového uzlu s hodnotou "0"
            std::pair<bool, BinaryTree::Node_t*> node0_pair = tree->InsertNode(0);
            node0 = node0_pair.second;
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro strom
            delete tree;
        }
};


/*                                                                            */
/*      Příprava fixture pro testování axiomů Red-Black Tree (TreeAxioms)     */
/*                                                                            */

class TreeAxioms : public ::testing::Test
{
    protected:
        // Deklarace ukazatele pro Red-Black Tree
        BinaryTree *tree;

        // Deklarace ukazatele pro uzly
        BinaryTree::Node_t* node42;
        BinaryTree::Node_t* node60;
        BinaryTree::Node_t* node35;
        BinaryTree::Node_t* node20;
        BinaryTree::Node_t* node26;
        BinaryTree::Node_t* node58;
        BinaryTree::Node_t* node3;
        BinaryTree::Node_t* node66;
        BinaryTree::Node_t* node2;
        BinaryTree::Node_t* node37;
        BinaryTree::Node_t* node64;
        BinaryTree::Node_t* node21;
        BinaryTree::Node_t* node68;

        // Příprava počátečního testovacího prostředí
        virtual void SetUp() override
        {
            // Vytvoření stromu
            tree = new BinaryTree();

            // Vytvoření nového uzlu s hodnotou "42"
            std::pair<bool, BinaryTree::Node_t*> node42_pair = tree->InsertNode(42);
            node42 = node42_pair.second;

            // Vytvoření nového uzlu s hodnotou "60"
            std::pair<bool, BinaryTree::Node_t*> node60_pair = tree->InsertNode(60);
            node60 = node60_pair.second;

            // Vytvoření nového uzlu s hodnotou "35"
            std::pair<bool, BinaryTree::Node_t*> node35_pair = tree->InsertNode(35);
            node35 = node35_pair.second;

            // Vytvoření nového uzlu s hodnotou "20"
            std::pair<bool, BinaryTree::Node_t*> node20_pair = tree->InsertNode(20);
            node20 = node20_pair.second;

            // Vytvoření nového uzlu s hodnotou "26"
            std::pair<bool, BinaryTree::Node_t*> node26_pair = tree->InsertNode(26);
            node26 = node26_pair.second;

            // Vytvoření nového uzlu s hodnotou "58"
            std::pair<bool, BinaryTree::Node_t*> node58_pair = tree->InsertNode(58);
            node58 = node58_pair.second;

            // Vytvoření nového uzlu s hodnotou "3"
            std::pair<bool, BinaryTree::Node_t*> node3_pair = tree->InsertNode(3);
            node3 = node3_pair.second;

            // Vytvoření nového uzlu s hodnotou "66"
            std::pair<bool, BinaryTree::Node_t*> node66_pair = tree->InsertNode(66);
            node66 = node66_pair.second;

            // Vytvoření nového uzlu s hodnotou "2"
            std::pair<bool, BinaryTree::Node_t*> node2_pair = tree->InsertNode(2);
            node2 = node2_pair.second;

            // Vytvoření nového uzlu s hodnotou "37"
            std::pair<bool, BinaryTree::Node_t*> node37_pair = tree->InsertNode(37);
            node37 = node37_pair.second;

            // Vytvoření nového uzlu s hodnotou "64"
            std::pair<bool, BinaryTree::Node_t*> node64_pair = tree->InsertNode(64);
            node64 = node64_pair.second;

            // Vytvoření nového uzlu s hodnotou "21"
            std::pair<bool, BinaryTree::Node_t*> node21_pair = tree->InsertNode(21);
            node21 = node21_pair.second;

            // Vytvoření nového uzlu s hodnotou "68"
            std::pair<bool, BinaryTree::Node_t*> node68_pair = tree->InsertNode(68);
            node68 = node68_pair.second;
        }

        // Úklid po provedení testů
        virtual void TearDown() override
        {
            // Uvolnění paměti alokované pro strom
            delete tree;
        }
};



/******************************************************************************/
/*                                                                            */
/*                     Testy prázdného stromu "EmptyTree"                     */
/*                                                                            */
/******************************************************************************/

/* pozn. budu testovat postupnou stavbu jednoduchého stromu,
 *       který bude nakonec vypadat následovně (B = BLACK, R = RED):
 *
 *                          0(B)
 *                         /    \
 *                        /      \
 *                  -999(B)      42(B)      
 *                  / \            / \
 *                 /   \          /   \
 *            NIL(B) -5(R)      1(R)  NIL(B)
 *                   / \         / \
 *                  /   \       /   \
 *             NIL(B) NIL(B) NIL(B) NIL(B)
*/

TEST_F(EmptyTree, InsertNode)
{
    /*                                                                        */
    /*                   ~~~ EXISTENCE PRÁZDNÉHO STROMU ~~~                   */
    /*                                                                        */

    // Test, že strom je skutečně prázdný
    // Node_t* BinaryTree::GetRoot() 	
    ASSERT_EQ(tree->GetRoot(), nullptr);


    /*                  ~~~ VLOŽENÍ KOŘENE S HODNOTOU 0 ~~~                   */
    /*                                                                        */
    /*                                  0(B)                                  */
    /*                                 /    \                                 */
    /*                              NIL(B)  NIL(B)                            */
    /*                                                                        */

    // Vytvoření uzlu s hodnotou 0 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node0_pair_root = tree->InsertNode(0);

    // Test, úspěšného vložení uzlu
    ASSERT_NE(tree->GetRoot(), nullptr);
    ASSERT_TRUE(node0_pair_root.second != nullptr);
    ASSERT_TRUE(node0_pair_root.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* node0 = node0_pair_root.second;

    /* Ověření základních vlastností nového uzlu, tj. KOŘENE */
    /* pozn. pro zjednodušení budu komentáře psát stručně    */
    // Uzel je kořenem?
    ASSERT_EQ(tree->GetRoot(), node0);
    
    // Uzel nemá rodiče?
    EXPECT_EQ(node0->pParent, nullptr);

    // Uzel má prázdný levý a pravý list?
    EXPECT_NE(node0->pLeft, nullptr);
    EXPECT_NE(node0->pRight, nullptr);

    // Uzel je černý?
    EXPECT_EQ(node0->color, BinaryTree::BLACK);

    // Klíčem uzlu je 0?
    EXPECT_EQ(node0->key, 0);

    /* Ověření základních vlastností levého a pravého listu kořene,*/
    /* tj. pLeft a pRight                                          */
    BinaryTree::Node_t* node0_left = node0->pLeft;
    BinaryTree::Node_t* node0_right = node0->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(node0_left, nullptr);
    ASSERT_NE(node0_right, nullptr);

    // Rodičem uzlu je skutečně "node0", tj. kořen?
    EXPECT_EQ(node0_left->pParent, node0);
    EXPECT_EQ(node0_right->pParent, node0);

    // Klíčem rodiče je skutečně hodnota 0?
    EXPECT_EQ(node0_left->pParent->key, 0);
    EXPECT_EQ(node0_right->pParent->key, 0);

    // Listy již nemají další potomky?
    EXPECT_EQ(node0_left->pLeft, nullptr);
    EXPECT_EQ(node0_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(node0_left->color, BinaryTree::BLACK);
    EXPECT_EQ(node0_right->color, BinaryTree::BLACK);


    /*                                                                        */
    /*           ~~~ POKUS O VLOŽENÍ DALŠÍHO UZLU S HODNOTOU 0 ~~~            */
    /*                                                                        */
   
    // Opětovné vytvoření uzlu s hodnotou 0 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node0_pair_again = tree->InsertNode(0);

    // Test, NEúspěšného opětovného vložení uzlu s hondotou 0
    ASSERT_NE(tree->GetRoot(), nullptr);
    EXPECT_EQ(node0_pair_again.second, node0);
    EXPECT_FALSE(node0_pair_again.first);


    /*        ~~~ VLOŽENÍ PRAVÉHO UZLU KOŘENE S KLADNOU HODNOTOU 42 ~~~       */
    /*                                                                        */
    /*                                  0(B)                                  */
    /*                                 /    \                                 */
    /*                             NIL(B)   42(R)                             */
    /*                                      /   \                             */
    /*                                  NIL(B)  NIL(B)                        */
    /*                                                                        */

    // Vytvoření uzlu s hodnotou 42 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node42_pair = tree->InsertNode(42);

    // Test, úspěšného vložení uzlu
    ASSERT_TRUE(node42_pair.second != nullptr);
    ASSERT_TRUE(node42_pair.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* node42 = node42_pair.second;

    /* Ověření základních vlastností uzlu s hodnotou 42   */
    // Uzel není kořenem?
    ASSERT_NE(tree->GetRoot(), node42);
    
    // Uzel má rodiče?
    EXPECT_EQ(node42->pParent, node0);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(node42->pLeft, nullptr);
    EXPECT_NE(node42->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(node42->color, BinaryTree::RED);

    // Klíčem uzlu je 42?
    EXPECT_EQ(node42->key, 42);

    /* Ověření základních vlastností levého a pravého listu "node42" */
    /* tj. pLeft a pRight                                            */
    BinaryTree::Node_t* node42_left = node42->pLeft;
    BinaryTree::Node_t* node42_right = node42->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(node42_left, nullptr);
    ASSERT_NE(node42_right, nullptr);

    // Rodičem listu je skutečně "node42"?
    EXPECT_EQ(node42_left->pParent, node42);
    EXPECT_EQ(node42_right->pParent, node42);

    // Klíčem rodiče je skutečně hodnota 42?
    EXPECT_EQ(node42_left->pParent->key, 42);
    EXPECT_EQ(node42_right->pParent->key, 42);

    // Listy již nemají další potomky?
    EXPECT_EQ(node42_left->pLeft, nullptr);
    EXPECT_EQ(node42_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(node42_left->color, BinaryTree::BLACK);
    EXPECT_EQ(node42_right->color, BinaryTree::BLACK);

    /* Ověření propojení "node0" jako rodiče a "node42" jako potomka */
    // Pravým potomkem "node0" je "node42"?
    EXPECT_EQ(node0->pRight, node42);


    /*      ~~~ VLOŽENÍ LEVÉHO UZLU KOŘENE SE ZÁPORNOU HODNOTOU -999 ~~~      */
    /*                                                                        */
    /*                                  0(B)                                  */
    /*                                 /    \                                 */
    /*                            -999(R)    42(R)                            */
    /*                           /    \      /    \                           */
    /*                       NIL(B) NIL(B) NIL(B) NIL(B)                      */
    /*                                                                        */

    // Vytvoření uzlu s hodnotou -999 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> nodeN999_pair = tree->InsertNode(-999);

    // Test, úspěšného vložení uzlu
    ASSERT_TRUE(nodeN999_pair.second != nullptr);
    ASSERT_TRUE(nodeN999_pair.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* nodeN999 = nodeN999_pair.second;

    /* Ověření základních vlastností uzlu s hodnotou -999 */
    // Uzel není kořenem?
    ASSERT_NE(tree->GetRoot(), nodeN999);
    
    // Uzel má rodiče?
    EXPECT_EQ(nodeN999->pParent, node0);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(nodeN999->pLeft, nullptr);
    EXPECT_NE(nodeN999->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(nodeN999->color, BinaryTree::RED);

    // Klíčem uzlu je -999?
    EXPECT_EQ(nodeN999->key, -999);

    /* Ověření základních vlastností levého a pravého listu "nodeN999" */
    /* tj. pLeft a pRight                                              */
    BinaryTree::Node_t* nodeN999_left = nodeN999->pLeft;
    BinaryTree::Node_t* nodeN999_right = nodeN999->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(nodeN999_left, nullptr);
    ASSERT_NE(nodeN999_right, nullptr);

    // Rodičem listu je skutečně "nodeN999"?
    EXPECT_EQ(nodeN999_left->pParent, nodeN999);
    EXPECT_EQ(nodeN999_right->pParent, nodeN999);

    // Klíčem rodiče je skutečně hodnota -999?
    EXPECT_EQ(nodeN999_left->pParent->key, -999);
    EXPECT_EQ(nodeN999_right->pParent->key, -999);

    // Listy již nemají další potomky?
    EXPECT_EQ(nodeN999_left->pLeft, nullptr);
    EXPECT_EQ(nodeN999_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(nodeN999_left->color, BinaryTree::BLACK);
    EXPECT_EQ(nodeN999_right->color, BinaryTree::BLACK);

    /* Ověření propojení "node0" jako rodiče a "nodeN999" jako potomka */
    // Levým potomkem "node0" je "nodeN999"?
    EXPECT_EQ(node0->pLeft, nodeN999);

    // Pravým potomkem "node0" je "node42"?
    EXPECT_EQ(node0->pRight, node42);


    /*     ~~~ VLOŽENÍ LEVÉHO UZLU "NODEN999" SE ZÁPORNOU HODNOTOU -5 ~~~     */
    /*                                                                        */
    /*                                  0(B)                                  */
    /*                                 /    \                                 */
    /*                            -999(B)    42(B)                            */
    /*                            /    \     /    \                           */
    /*                       NIL(B) -5(R) NIL(B) NIL(B)                       */
    /*                              /  \                                      */
    /*                         NIL(B) NIL(B)                                  */
    /*                                                                        */

    // Vytvoření uzlu s hodnotou -5 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> nodeN5_pair = tree->InsertNode(-5);

    // Test, úspěšného vložení uzlu
    ASSERT_TRUE(nodeN5_pair.second != nullptr);
    ASSERT_TRUE(nodeN5_pair.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* nodeN5 = nodeN5_pair.second;

    /* Ověření základních vlastností uzlu s hodnotou -5 */
    // Uzel není kořenem?
    ASSERT_NE(tree->GetRoot(), nodeN5);
    
    // Uzel má rodiče?
    EXPECT_EQ(nodeN5->pParent, nodeN999);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(nodeN5->pLeft, nullptr);
    EXPECT_NE(nodeN5->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(nodeN5->color, BinaryTree::RED);

    // Klíčem uzlu je -5?
    EXPECT_EQ(nodeN5->key, -5);

    /* Ověření základních vlastností levého a pravého listu "nodeN5" */
    /* tj. pLeft a pRight                                              */
    BinaryTree::Node_t* nodeN5_left = nodeN5->pLeft;
    BinaryTree::Node_t* nodeN5_right = nodeN5->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(nodeN5_left, nullptr);
    ASSERT_NE(nodeN5_right, nullptr);

    // Rodičem listu je skutečně "nodeN5"?
    EXPECT_EQ(nodeN5_left->pParent, nodeN5);
    EXPECT_EQ(nodeN5_right->pParent, nodeN5);

    // Klíčem rodiče je skutečně hodnota -5?
    EXPECT_EQ(nodeN5_left->pParent->key, -5);
    EXPECT_EQ(nodeN5_right->pParent->key, -5);

    // Listy již nemají další potomky?
    EXPECT_EQ(nodeN5_left->pLeft, nullptr);
    EXPECT_EQ(nodeN5_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(nodeN5_left->color, BinaryTree::BLACK);
    EXPECT_EQ(nodeN5_right->color, BinaryTree::BLACK);

    /* Ověření propojení "nodeN999" jako rodiče a "nodeN5" jako potomka */
    // Pravým potomkem "nodeN999" je "nodeN5"?
    EXPECT_EQ(nodeN999->pRight, nodeN5);


    /*       ~~~ VLOŽENÍ LEVÉHO UZLU "NODE42" S KLADNOU HODNOTOU 1 ~~~        */
    /*                                                                        */
    /*                                  0(B)                                  */
    /*                                 /    \                                 */
    /*                            -999(B)    42(B)                            */
    /*                            /    \       /  \                           */
    /*                       NIL(B) -5(R)     1(R) NIL(B)                     */
    /*                             /  \       /   \                           */
    /*                        NIL(B) NIL(B) NIL(B) NIL(B)                     */
    /*                                                                        */

    // Vytvoření uzlu s hodnotou 1 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node1_pair = tree->InsertNode(1);

    // Test, úspěšného vložení uzlu
    ASSERT_TRUE(node1_pair.second != nullptr);
    ASSERT_TRUE(node1_pair.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* node1 = node1_pair.second;

    /* Ověření základních vlastností uzlu s hodnotou -5 */
    // Uzel není kořenem?
    ASSERT_NE(tree->GetRoot(), node1);
    
    // Uzel má rodiče?
    EXPECT_EQ(node1->pParent, node42);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(node1->pLeft, nullptr);
    EXPECT_NE(node1->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(node1->color, BinaryTree::RED);

    // Klíčem uzlu je 1?
    EXPECT_EQ(node1->key, 1);

    /* Ověření základních vlastností levého a pravého listu "node1" */
    /* tj. pLeft a pRight                                              */
    BinaryTree::Node_t* node1_left = node1->pLeft;
    BinaryTree::Node_t* node1_right = node1->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(node1_left, nullptr);
    ASSERT_NE(node1_right, nullptr);

    // Rodičem listu je skutečně "node1"?
    EXPECT_EQ(node1_left->pParent, node1);
    EXPECT_EQ(node1_right->pParent, node1);

    // Klíčem rodiče je skutečně hodnota 1?
    EXPECT_EQ(node1_left->pParent->key, 1);
    EXPECT_EQ(node1_right->pParent->key, 1);

    // Listy již nemají další potomky?
    EXPECT_EQ(node1_left->pLeft, nullptr);
    EXPECT_EQ(node1_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(node1_left->color, BinaryTree::BLACK);
    EXPECT_EQ(node1_right->color, BinaryTree::BLACK);

    /* Ověření propojení "node42" jako rodiče a "node1" jako potomka */
    // Levým potomkem "node42" je "node1"?
    EXPECT_EQ(node42->pLeft, node1);


    /*                                                                        */
    /*           ~~~ POKUS O VLOŽENÍ DALŠÍHO UZLU S HODNOTOU 1 ~~~            */
    /*                                                                        */
   
    // Opětovné vytvoření uzlu s hodnotou 1 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node1_pair_again = tree->InsertNode(1);

    // Test, NEúspěšného opětovného vložení uzlu s hondotou 1
    EXPECT_EQ(node1_pair_again.second, node1);
    EXPECT_FALSE(node1_pair_again.first);


    /*                                                                        */
    /*         ~~~ POKUS O VLOŽENÍ DALŠÍHO UZLU S HODNOTOU -999, ~~~          */
    /*         ~~~      KTERÝ NEBYL BEZPROSTŘEDNĚ VYTVOŘEN       ~~~          */
    /*                                                                        */
   
    // Opětovné vytvoření uzlu s hodnotou -999 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> nodeN999_pair_again = tree->InsertNode(-999);

    // Test, NEúspěšného opětovného vložení uzlu s hondotou -999
    EXPECT_EQ(nodeN999_pair_again.second, nodeN999);
    EXPECT_FALSE(nodeN999_pair_again.first);
}

TEST_F(EmptyTree, DeleteNode)
{
    // Test, že strom je skutečně prázdný
    // Node_t* BinaryTree::GetRoot() 	
    ASSERT_EQ(tree->GetRoot(), nullptr);

    // Pokus smazat uzly s hraničními hodnotami
    // bool BinaryTree::DeleteNode(int key) 	
    EXPECT_FALSE(tree->DeleteNode(42));
    EXPECT_FALSE(tree->DeleteNode(1));
    EXPECT_FALSE(tree->DeleteNode(0));
    EXPECT_FALSE(tree->DeleteNode(-1));
    EXPECT_FALSE(tree->DeleteNode(-42));
}

TEST_F(EmptyTree, FindNode)
{
    // Test, že strom je skutečně prázdný
    // Node_t* BinaryTree::GetRoot() 	
    ASSERT_EQ(tree->GetRoot(), nullptr);

    // Pokus NAJÍT uzly s hraničními hodnotami
    // Node_t* BinaryTree::FindNode(int	key) const	
    EXPECT_EQ(tree->FindNode(42), nullptr);
    EXPECT_EQ(tree->FindNode(1), nullptr);
    EXPECT_EQ(tree->FindNode(0), nullptr);
    EXPECT_EQ(tree->FindNode(-1), nullptr);
    EXPECT_EQ(tree->FindNode(-42), nullptr);
}


/******************************************************************************/
/*                                                                            */
/*                  Testy NEprázdného stromu "NonEmptyTree"                   */
/*                                                                            */
/******************************************************************************/

/* Vychozí předvytvořený Red-Black Tree:
 * (N = NIL; B = BLACK; R = RED)
 *
 *                                    10(B)
 *                                      |
 *                           −−−−−−−−−−−−−−−−−−−−−−
 *                           |                    |                                  
 *                        -20(R)                30(R)      
 *                        / \                     / \
 *                       /   \                   /   \
 *                  -40(B)   -10(B)          20(B)   40(B)
 *                  / \         / \          / \       / \
 *                 /   \       /   \        /   \     /   \
 *            -50(R) -30(R)  N(B) 0(R)    N(B) N(B) N(B)  50(R)
 *            / \       / \        / \                     / \
 *           /   \     /   \      /   \                   /   \
 *         N(B) N(B) N(B) N(B)  N(B) N(B)               N(B) N(B)
 * 
*/


TEST_F(NonEmptyTree, InsertNode_NoColorSwap)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    /*        ~~~ VLOŽENÍ PRAVÉHO POTOMKA S HODNOTOU 25 K UZLU 20 ~~~         */
    /*                                                                        */
    /*                                  ...                                   */
    /*                                   |                                    */
    /*                                  30(R)                                 */
    /*                                  /   \                                 */
    /*                               20(B)  ...                               */
    /*                               /   \                                    */
    /*                           NIL(B)  25(R)                                */
    /*                                   /   \                                */
    /*                               NIL(B) NIL(B)                            */

    // Vytvoření uzlu s hodnotou 25 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node25_pair = tree->InsertNode(25);

    // Test, úspěšného vložení uzlu
    ASSERT_TRUE(node25_pair.second != nullptr);
    ASSERT_TRUE(node25_pair.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* node25 = node25_pair.second;

    /* Ověření základních vlastností uzlu s hodnotou 25 */
    // Uzel není kořenem?
    ASSERT_NE(tree->GetRoot(), node25);
    
    // Rodičem je uzel s hodnotou 20?
    EXPECT_EQ(node25->pParent, node20);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(node25->pLeft, nullptr);
    EXPECT_NE(node25->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(node25->color, BinaryTree::RED);

    // Klíčem uzlu je 25?
    EXPECT_EQ(node25->key, 25);

    /* Ověření základních vlastností levého a pravého listu "node25" */
    /* tj. pLeft a pRight                                            */
    BinaryTree::Node_t* node25_left = node25->pLeft;
    BinaryTree::Node_t* node25_right = node25->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(node25_left, nullptr);
    ASSERT_NE(node25_right, nullptr);

    // Rodičem listu je skutečně "node25"?
    EXPECT_EQ(node25_left->pParent, node25);
    EXPECT_EQ(node25_right->pParent, node25);

    // Klíčem rodiče listů je skutečně hodnota 25?
    EXPECT_EQ(node25_left->pParent->key, 25);
    EXPECT_EQ(node25_right->pParent->key, 25);

    // Listy již nemají další potomky?
    EXPECT_EQ(node25_left->pLeft, nullptr);
    EXPECT_EQ(node25_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(node25_left->color, BinaryTree::BLACK);
    EXPECT_EQ(node25_right->color, BinaryTree::BLACK);

    /* Ověření propojení "node20" jako rodiče a "node25" jako potomkem */
    // Pravým potomkem "node20" je "node25"?
    EXPECT_EQ(node20->pRight, node25);

    // NEdošlo ke změně barvy "node20"?
    EXPECT_EQ(node25->pParent->color, BinaryTree::BLACK);

    // Levý potomek "node20" je stále prázdný?
    EXPECT_EQ(node20->pLeft->key, NULL);

    /*                                                                        */
    /*           ~~~ POKUS O VLOŽENÍ DALŠÍHO UZLU S HODNOTOU 25 ~~~           */
    /*                                                                        */
   
    // Opětovné vytvoření uzlu s hodnotou 25 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node25_pair_again = tree->InsertNode(25);

    // Test, NEúspěšného opětovného vložení uzlu s hondotou 25
    EXPECT_EQ(node25_pair_again.second, node25);
    EXPECT_FALSE(node25_pair_again.first);
}

TEST_F(NonEmptyTree, InsertNode_ColorSwap)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    /*        ~~~ VLOŽENÍ PRAVÉHO POTOMKA S HODNOTOU 5 K UZLU -10, ~~~        */
    /*        ~~~    DOJDE K PROHOZENÍ UZLŮ A ZMĚNĚ JEJICH BAREV   ~~~        */
    /*                                                                        */
    /*                 ...                        ...                         */
    /*                  |                          |                          */
    /*                -20(R)                     -20(R)                       */
    /*                /    \         ==>         /    \                       */
    /*              ...   -10(B)               ...    0(B)                    */
    /*                    /    \                      /   \                   */
    /*                NIL(B)   0(R)             -10(R)     5(R)               */
    /*                         /  \            / \           / \              */
    /*                    NIL(B)  NIL(B)  NIL(B) NIL(B) NIL(B) NIL(B)         */

    // Vytvoření uzlu s hodnotou 5 a vložení tohoto uzlu do stromu
    std::pair<bool, BinaryTree::Node_t*> node5_pair = tree->InsertNode(5);

    // Test, úspěšného vložení uzlu
    ASSERT_TRUE(node5_pair.second != nullptr);
    ASSERT_TRUE(node5_pair.first);
    
    // Získání ukazatele na vytvořený uzel kvůli dalšímu ověřování
    BinaryTree::Node_t* node5 = node5_pair.second;

    /* Ověření základních vlastností uzlu s hodnotou 5 */
    // Uzel není kořenem?
    ASSERT_NE(tree->GetRoot(), node5);
    
    // Rodičem je uzel s hodnotou 0?
    // Pozn. došlo k prohození uzlu 0 za uzel -10
    EXPECT_EQ(node5->pParent, node0);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(node5->pLeft, nullptr);
    EXPECT_NE(node5->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(node5->color, BinaryTree::RED);

    // Klíčem uzlu je 5?
    EXPECT_EQ(node5->key, 5);

    /* Ověření základních vlastností levého a pravého listu "node5" */
    /* tj. pLeft a pRight                                            */
    BinaryTree::Node_t* node5_left = node5->pLeft;
    BinaryTree::Node_t* node5_right = node5->pRight;
    
    // Ukazatel není NULL?
    ASSERT_NE(node5_left, nullptr);
    ASSERT_NE(node5_right, nullptr);

    // Rodičem listu je skutečně "node5"?
    EXPECT_EQ(node5_left->pParent, node5);
    EXPECT_EQ(node5_right->pParent, node5);

    // Klíčem rodiče listů je skutečně hodnota 5?
    EXPECT_EQ(node5_left->pParent->key, 5);
    EXPECT_EQ(node5_right->pParent->key, 5);

    // Listy již nemají další potomky?
    EXPECT_EQ(node5_left->pLeft, nullptr);
    EXPECT_EQ(node5_right->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(node5_left->color, BinaryTree::BLACK);
    EXPECT_EQ(node5_right->color, BinaryTree::BLACK);


    /*                                                                        */
    /*               ~~~ OVĚŘENÍ VLASTNOSTÍ UZLU "nodeN10" ~~~                */
    /*                                                                        */

    // Rodičem je uzel s hodnotou 0?
    EXPECT_EQ(nodeN10->pParent, node0);

    // Uzel má levý a pravý prázdný list?
    EXPECT_NE(nodeN10->pLeft, nullptr);
    EXPECT_NE(nodeN10->pRight, nullptr);

    // Uzel je červený?
    EXPECT_EQ(nodeN10->color, BinaryTree::RED);

    // Listy již nemají další potomky?
    EXPECT_EQ(nodeN10->pLeft->pLeft, nullptr);
    EXPECT_EQ(nodeN10->pRight->pRight, nullptr);

    // Listy jsou černé?
    EXPECT_EQ(nodeN10->pLeft->color, BinaryTree::BLACK);
    EXPECT_EQ(nodeN10->pRight->color, BinaryTree::BLACK);


    /*                                                                        */
    /*                ~~~ OVĚŘENÍ VLASTNOSTÍ UZLU "node0" ~~~                 */
    /*                                                                        */

    // Rodičem je uzel s hodnotou -20?
    EXPECT_EQ(node0->pParent, nodeN20);

    // Uzel má levý a pravý NEprázdný list?
    // Levý list = "nodeN10"; Pravý list = "node5"
    EXPECT_EQ(node0->pLeft, nodeN10);
    EXPECT_EQ(node0->pRight, node5);

    // Uzel je černý?
    EXPECT_EQ(node0->color, BinaryTree::BLACK);

    // Levý list "nodeN10" má své vlastní listy
    EXPECT_EQ(node0->pLeft->pLeft, nodeN10->pLeft);
    EXPECT_EQ(node0->pLeft->pRight, nodeN10->pRight);

    // Pravý list "node5" má své vlastní listy
    EXPECT_EQ(node0->pRight->pLeft, node5_left);
    EXPECT_EQ(node0->pRight->pRight, node5_right);
}

TEST_F(NonEmptyTree, DeleteNode)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    /*      ~~~ ODSTRANĚNÍ LEVÉHO POTOMKA S HODNOTOU -30 OD UZLU -40 ~~~      */
    /*                                                                        */
    /*                                  ...                                   */
    /*                                   |                                    */
    /*                                 -40(B)                                 */
    /*                                 /    \                                 */
    /*                             -50(R)   NIL(B)                            */
    /*                             /    \                                     */
    /*                         NIL(B)  NIL(B)                                 */

    // Odstranění uzlu s hodnotou -30
    EXPECT_TRUE(tree->DeleteNode(-30));
    
    /* Ověření základních vlastností nyní prázdného pravého listu "nodeN40" */
    // Ukazatel není NULL?
    ASSERT_NE(nodeN40->pRight, nullptr);

    // Rodičem listu je skutečně "nodeN40"?
    EXPECT_EQ(nodeN40->pRight->pParent, nodeN40);

    // Klíčem rodiče je skutečně hodnota -40?
    EXPECT_EQ(nodeN40->pRight->pParent->key, -40);

    // List již nemá další potomky?
    EXPECT_EQ(nodeN40->pRight->pLeft, nullptr);
    EXPECT_EQ(nodeN40->pRight->pRight, nullptr);

    // Pravý list "nodeN40" je černý?
    EXPECT_EQ(nodeN40->pRight->color, BinaryTree::BLACK);


    /*                                                                        */
    /*        ~~~ NEÚSPĚŠNÉ ODSTRANĚNÍ NEEXISTUJÍCÍHO UZLU "node42" ~~~       */
    /*                                                                        */

    EXPECT_FALSE(tree->DeleteNode(42));
}

TEST_F(NonEmptyTree, FindNode)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    // Pokus úspěšně najít uzly vytvořené ve fixture
    EXPECT_NE(tree->FindNode(10), nullptr);
    EXPECT_NE(tree->FindNode(-10), nullptr);
    EXPECT_NE(tree->FindNode(20), nullptr);
    EXPECT_NE(tree->FindNode(-20), nullptr);
    EXPECT_NE(tree->FindNode(30), nullptr);
    EXPECT_NE(tree->FindNode(-30), nullptr);
    EXPECT_NE(tree->FindNode(40), nullptr);
    EXPECT_NE(tree->FindNode(-40), nullptr);
    EXPECT_NE(tree->FindNode(50), nullptr);
    EXPECT_NE(tree->FindNode(-50), nullptr);
    EXPECT_NE(tree->FindNode(0), nullptr);

    // Pokus najít neexistující uzly
    EXPECT_EQ(tree->FindNode(1), nullptr);
    EXPECT_EQ(tree->FindNode(-1), nullptr);
}


/******************************************************************************/
/*                                                                            */
/*                          Testy axiomů "TreeAxioms"                         */
/*                                                                            */
/******************************************************************************/

/* Tři základní axiomy Red-Black Tree:
 *          => 1) Všechny listové uzly (tedy uzly bez potomků) jsou "černé".
 *          => 2) Pokud je uzel "červený", pak jsou jeho oba potomci "černé".
 *          => 3) Každá cesta od každého listového uzlu ke kořeni obsahuje
 *                stejný počet "černých" uzlů.
*/

/* Vychozí předvytvořený Red-Black Tree:
 * (N = NIL; B = BLACK; R = RED)
 *
 *                                      42(B)
 *                                        |
 *                   −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
 *                   |                                       |                                  
 *                 26(B)                                   60(B)
 *                   |                                       |
 *          −−−−−−−−−−−−−−−−−−                      −−−−−−−−−−−−−−−−−−
 *          |                |                      |                |
 *         3(R)            35(B)                  58(B)            66(B)
 *         / \              / \                    / \              / \
 *        /   \            /   \                  /   \            /   \
 *     2(B)   20(B)     N(B) 37(R)              N(B) N(B)      64(R)   68(R)
 *     / \      / \           / \                              / \       / \
 *    /   \    /   \         /   \                            /   \     /   \
 * N(B) N(B) N(B) 21(R)     N(B) N(B)                       N(B) N(B) N(B) N(B)
 *                 / \
 *                /   \
 *              N(B) N(B)
*/

/* Definice konstanty počtu černých uzlů na cestě od listu ke kořeni pro 
   Vychozí předvytvořený Red-Black Tree:                                 */
#define BLACKPATH 4

// Axiom 1: Všechny listové uzly (tedy uzly bez potomků) jsou "černé".
TEST_F(TreeAxioms, Axiom1)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    // Deklarace pole ukazatelů na listové uzly 
    std::vector<Node_t *> LeafNodesArray;

    // Uložení ukazatelů na všechny listové uzly do pole
    tree->GetLeafNodes(LeafNodesArray);

    // Ověření, že pole listů není prázdné
    /* Pozn. operátor "<<" u GTestu znamená přesměrování na výstup. */
    ASSERT_FALSE(LeafNodesArray.empty()) << "Pole listů je prázdné.";

    // Ověření 1. axiomu Red-Black Tree
    /* Pozn. C++ má vestavěný mechanismus pro ukončení cyklu na rozdíl od C. */
    for(Node_t* leaf : LeafNodesArray)
    {
        // Ověření, že jsou všechny listy černé (BLACK)
        EXPECT_EQ(leaf->color, BinaryTree::BLACK) 
                  << "List s klíčem " << leaf->key <<
                  " není černý, což je v rozporu s 1. axiomem Red-Black Tree.";
    }
}

// Axiom 2: Pokud je uzel "červený", pak jsou jeho oba potomci "černé".
TEST_F(TreeAxioms, Axiom2)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    // Deklarace pole ukazatelů pro všechny uzly stromu
    std::vector<Node_t *> AllNodesArray;

    // Uložení ukazatelů na všechny uzly stromu do pole
    tree->GetAllNodes(AllNodesArray);

    // Ověření, že pole listů není prázdné
    ASSERT_FALSE(AllNodesArray.empty()) << "Pole všech uzlů stromu je prázdné.";

    // Ověření 2. axiomu Red-Black Tree
    for(Node_t* node : AllNodesArray)
    {
        // Nalezení červených (RED) uzlů stromu a kontrola barvy jejích potomků
        if(node->color == BinaryTree::RED)
        {
            EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK) 
                  << "Levý potomek s klíčem" << node->pLeft->key << 
                  " červeného uzlu s klíčem " << node->key <<
                  " není černý, což je v rozporu s 2. axiomem Red-Black Tree.";

            EXPECT_EQ(node->pRight->color, BinaryTree::BLACK) 
                  << "Pravý potomek s klíčem" << node->pRight->key << 
                  " červeného uzlu s klíčem " << node->key <<
                  " není černý, což je v rozporu s 2. axiomem Red-Black Tree.";
        }
    }
}

// Axiom 3: Každá cesta od každého listového uzlu ke kořeni obsahuje stejný
//          počet "černých" uzlů.
TEST_F(TreeAxioms, Axiom3)
{
    // Test, že strom není prázdný
    ASSERT_NE(tree->GetRoot(), nullptr);

    // Deklarace pole ukazatelů na listové uzly 
    std::vector<Node_t *> leaf_nodes_array;

    // Uložení ukazatelů na všechny listové uzly do pole
    tree->GetLeafNodes(leaf_nodes_array);

    // Ověření, že pole listů není prázdné
    ASSERT_FALSE(leaf_nodes_array.empty()) << "Pole listů je prázdné.";

    // Nalezení kořene stromu a ověření, že došlo k úspěšnému uložení ukazatele
    BinaryTree::Node_t* root;
    root = tree->GetRoot();
    ASSERT_NE(root, nullptr);

    /* Pozn. Algoritmus je založený na principu vyhledávání v binárním stromě.
     *       Jelikož známe hodnotu kořene, můžeme se snadno dostat až k rodiči 
     *       listu a cestou kontrolovat barvy uzlů, které procházíme. Musíme 
     *       ovšem počítat s tím, že musíme do černých uzlů započítat kořen i
     *       samotný list.
    */

    // Deklarace počítadla černých uzlů na cestě od kořene k listu
    int black_cnt;

    // Deklarace proměnné pro uložení hodnoty klíče rodiče listu
    int parent_key;

    // Deklarace pole pro ukládání počtu čených uzlů cesty
    std::vector<int> black_path;

    // Ověření 3. axiomu Red-Black Tree
    for (Node_t* leaf : leaf_nodes_array)
    {
        // Deklarace proměnné pro uložení aktuální uzlu (a inicializace kořenem)
        BinaryTree::Node_t* current_node;
        current_node = root;

        // Uložení hodnoty klíče rodiče aktuálního kontrolovaného listu
        parent_key = leaf->pParent->key;
 
        // Inicializace počítadla černých uzlů pro konkrétní iteraci
        /* Pozn. začínáme od "1", jelikož provedeme průchod pouze k rodiči daného
         *       listu, tedy by cesta jinak byla o 1 kratší, jelikož by se
         *       nezapočítal samotný list.
        */
       black_cnt = 1;

        // Hledání černých na cestě od kořene k rodičovskému uzlu listu
        while(current_node->key != parent_key)
        {
            // Kontrola barvy aktuálního uzlu
            if (current_node->color == BinaryTree::BLACK) {
                black_cnt++;
            }

            // Přechod do dalšího uzlu
            if(parent_key < current_node->key)
            {
                current_node = current_node->pLeft;
            }
            else if(parent_key > current_node->key)
            {
                current_node = current_node->pRight;
            }
        }

        // Kontrola barvy rodičovského uzlu
            if (leaf->pParent->color == BinaryTree::BLACK) {
                black_cnt++;
            }
        
        // Přidání hodnoty "black_cnt" do pole "black_path"
        black_path.push_back(black_cnt);
    }

    ASSERT_EQ(black_path.size(), leaf_nodes_array.size());

    for (size_t i = 0; i < black_path.size() - 1; ++i)
    {
        // Ověření správného počet černých uzlů na cestě od kořene k listu
        EXPECT_EQ(black_path[i], black_path[i + 1]) << "Počty černých uzlů na" 
                  " cestě od kořene s klíčem " << root->key << " k listům,"
                  " jejichž rodiče mají klíč " << (leaf_nodes_array[i])->pParent->key <<
                  " a " << (leaf_nodes_array[i + 1])->pParent->key << " se neshodují.";
    }
}


/*** Konec souboru black_box_tests.cpp ***/
