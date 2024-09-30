//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.h
// $Authors:    Filip Vaverka <ivaverka@fit.vutbr.cz>
//              David Grochol <igrochol@fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//

#pragma once

#ifndef IVS_CODE_H_
#define IVS_CODE_H_

#include <stdexcept>
#include <iostream>
#include <string>

class BankovniUcet
{
public:
    double zustatek;

    BankovniUcet();
    ~BankovniUcet();
    
    void vloz(double castka);
    void vyber(double castka);
};

struct MyException1: public std::runtime_error
{
    MyException1(std::string const& message)
        : std::runtime_error(message + " Was thrown")
    {}
};


struct MyException2: public std::runtime_error
{
    MyException2(std::string const& message)
        : std::runtime_error(message + " Was thrown")
    {}
};



#endif // IVS_CODE_H_
