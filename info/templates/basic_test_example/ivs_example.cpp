#include "ivs_example.h"
#include <stdexcept>





BankovniUcet::BankovniUcet(){
    zustatek = 0;
}

BankovniUcet::~BankovniUcet(){
    zustatek = 0;
}

void BankovniUcet::vloz(double castka){
    if(castka <= 0){
        throw MyException1("castka musi byt vetsi nez 0!");
    }
    zustatek += castka;
}

void BankovniUcet::vyber(double castka){
    if(castka <= 0){
        throw MyException1("castka musi byt vetsi nez 0!");
    }
    else if((zustatek - castka) >= 0){
        zustatek -= castka;
    }
    else{
        throw MyException2("Nedostatek penez!");
    }
}


