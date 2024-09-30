//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue tests
//
// $NoKeywords: $ivs_project_1 $tdd_tests.cpp
// $Authors:    Filip Vaverka <ivaverka@fit.vutbr.cz>
//              David Grochol <igrochol@fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//

#include "gtest/gtest.h"
#include "ivs_example.h"

class osobniUcet : public ::testing::Test
{
protected:
    BankovniUcet *ucet;

    virtual void SetUp() {
        ucet = new BankovniUcet();
    }
    virtual void TearDown() {
      delete ucet;
    }

};

TEST_F(osobniUcet, vlozeni){
    EXPECT_NO_THROW(ucet->vloz(100));
    EXPECT_EQ(ucet->zustatek, 100);
}

// zamerna chyba v testu
TEST_F(osobniUcet, vlozeni_err){
    //EXPECT: Continues after the failure.
    EXPECT_EQ(ucet->zustatek, 0);
    //ASSERT: Fails fast, aborting the current function.
    ASSERT_ANY_THROW(ucet->vloz(-100));
    //EXPECT: Continues after the failure.
    EXPECT_EQ(ucet->zustatek, 0);
}


TEST_F(osobniUcet, vyber_err){
    EXPECT_ANY_THROW(ucet->vyber(1000)); 
    EXPECT_EQ(ucet->zustatek, 0);
    EXPECT_ANY_THROW(ucet->vyber(-100));
    EXPECT_EQ(ucet->zustatek, 0);
}

TEST_F(osobniUcet, operace){
    ASSERT_NO_THROW(ucet->vloz(200));
    EXPECT_NO_THROW(ucet->vyber(100));
    EXPECT_EQ(ucet->zustatek, 100);
}

