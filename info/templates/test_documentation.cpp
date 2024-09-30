/*******************************************************************************
 *                                                                             *
 * Název projektu:   Calm CatCalc                                              *
 *                                                                             *
 * Soubor:           cat_calc_core_tests.cpp                                   *
 * Datum:            17.03.2024                                                *
 * Poslední změna:   22.04.2024                                                *
 *                                                                             *
 * Tým:      Calm CatCalc Coders                                               *
 *                                                                             *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *        	                                                                   *
 ******************************************************************************/
/**
 * @file cat_calc_core_tests.cpp
 * @author David Krejčí \<xkrejcd00>
 * 
 * @brief Soubor obsahující testy matematické knihovny _Calm CatCalc Core_.
 * @details Tento soubor obsahuje testovací případy pro funkce jádra kalkulačky.
 *          Testuje se správnost výpočtů, chování při chybných vstupech a další
 *          aspekty funkčnosti matematické knihovny _Calm CatCalc Core_.
 */

#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "cat_calc_core.h"

/** @brief Testovaná přesnost kalkulačky na 14 desetinných míst */
#define TESTED_PRECISION 0.00000000000001

using namespace ::testing;
using namespace std;
using namespace catMath;


/*******************************************************************************
 *                                                                             *
 *         SPECIÁLNÍ FUNKCE VYUŽÍVANÉ K TESTOVÁNÍ MATEMATICKÉ KNIHOVNY         *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Funkce porovná 2 vektory řetězců.
 * 
 * @details Tato funkce porovná dva vektory řetězců. Pokud mají vektory různou
 *          délku, funkce vrátí `false`. Pokud mají vektory stejnou délku,
 *          funkce porovná jednotlivé prvky vektory. Pokud jsou všechny prvky
 *          stejné, funkce vrátí `true`. Pokud je alespoň jeden prvek různý,
 *          funkce vrátí `false`.
 * 
 * @param[in] vec1 První vektor k porovnání.
 * @param[in] vec2 Druhý vektor k porovnání.
 * @return
 * - `true`, pokud jsou vektory stejné, jinak `false`.
 */
bool compareVectors(vector<string> vec1, vector<string> vec2){
    // Pokud mají vektory různou délku, nejsou stejné
    if (vec1.size() != vec2.size()){
        return false;
    }
    // Porovnání jednotlivých prvků
    for (size_t i = 0; i < vec1.size(); i++){
        if (vec1[i] != vec2[i]){
            return false;
        }
    }
    return true;
}


/*******************************************************************************
 *                                                                             *
 *               TESTY FUNKCÍ NA ZPRACOVÁNÍ MATEMATICKÝCH VÝRAZŮ               *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Testovací případ pro základní výrazy.
 *
 * @details Tento testovací případ ověřuje správnost výpočtu základních
 *          výrazů. <br>
 *          __Testuje následující situace:__
 *          - Opsání výrazu
 *          - Základní dělení
 *          - Zaokrouhlení při násobení
 *          - Dělení záporných desetinných čísel a práce se znaménky
 *          - Dvojí faktoriál
 *          - Implicitní odmocnina
 *          - Neukončená závorka
 *          - Implicitní odmocnina s mezerami
 *          - Implicitní odmocnina s dalšími operacemi
 *          - Implicitní mocnina
 *          - Implicitní mocnina s mezerami
 *          - Implicitní mocnina se závorkou a dalšími operacemi
 *          - Mocnina se závorkou a dalšími operacemi
 *          - Dva mínus za sebou
 *          - Jednoduché sčítání
 *          - Jednoduché dělení desetinným číslem
 */
TEST(Calculate, BasicExpressions){
    // Opsání výrazu
    string expression = "4";
    string expectedResult= "4";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Základní dělení
    expression = "9/3";
    expectedResult= "3";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Zaokrouhlení při násobení
    expression = "99.999999999999999*10000000";
    expectedResult= "1000000000";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Základní dělení
    expression = "(2090493964.1320033)+(592518.79299660004)";
    expectedResult= "2091086482";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());
    

    // Dělení záporných desetinných čísel a práce se znaménky
    expression = "-5.5/-5.5*-1-1.5";
    expectedResult= "-2.5";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Dvojí faktoriál 2
    expression = "2!!";
    expectedResult= "2";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Dvojí faktoriál 3
    expression = "3!!";
    expectedResult= "720";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní odmocnina
    expression = "#4";
    expectedResult= "2";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Neukončená závorka
    expression = "(#4";
    expectedResult= "2";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní odmocnina s mezerami
    expression = "  #4";
    expectedResult= "2";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní odmocnina s dalšími operacemi
    expression = "(1+4)* #16";
    expectedResult= "20";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní odmocnina s dalšími operacemi
    expression = "5+#81/2";
    expectedResult= "9.5";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní mocnina
    expression = "5^";
    expectedResult= "25";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní mocnina s mezerami
    expression = "5^  ";
    expectedResult= "25";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní mocnina
    expression = "5^*3";
    expectedResult= "75";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Implicitní mocnina se závorkou a dalšími operacemi
    expression = "(2*3^) +1";
    expectedResult= "19";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Mocnina se závorkou a dalšími operacemi
    expression = "2*1+3^ (2+1)+2";
    expectedResult= "31";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Dva - za sebou
    expression = "4 - (-8)";
    expectedResult= "12";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Jednoduché sčítání
    expression = "3.1+ 8.2";
    expectedResult= "11.3";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Jednoduché dělení desetinným číslem
    expression = "45/4.5";
    expectedResult= "10";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

}

/**
 * @brief Testovací případ pro složité výrazy.
 *
 * @details Tento testovací případ ověřuje správnost výpočtu složitých
 *          výrazů. <br>
 *          __Testuje následující situace:__
 *          - Výrazy s více operacemi, včetně sčítání, násobení, odmocniny,
 *            kosinu a mocniny.
 *          - Výrazy obsahující goniometrické funkce a faktoriál.
 *          - Výrazy obsahující goniometrické funkce, faktoriál, sčítání,
 *            násobení a závorky.
 */
TEST(Calculate, ComplexExpressions){
    degRad = false;

    // Složitější výrazy s více operacemi
    string expression = "(3.1+ 8.2) * 3 #cos(48)^2";
    string expectedResult= "8.644748712019318";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    expression = "sin(4!) *10";
    expectedResult= "4.0673664307580024";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    expression = "((sin(4!)+1) *10)+2";
    expectedResult= "16.067366430758";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());
}

/**
 * @brief Testovací případ pro chybové stavy.
 *
 * @details Tento testovací případ ověřuje, že funkce `calculate()` správně
 *          vyhazuje výjimky pro chybové stavy. <br>
 *          __Testuje následující situace:__
 *          - Dělení nulou
 *          - Faktoriál desetinného čísla
 *          - Faktoriál záporného čísla
 *          - Chybějící začátek závorky
 *          - Odmocnina záporného čísla
 *          - Odmocnina o desetinném exponentu
 *          - Záporná odmocnina o základu nula
 *          - Mocnina s desetinným exponentem
 *          - Mocnina se záporným exponentem a základem nula
 *          - Více operací bez operandů
 *          - Dvě čísla bez operace
 *          - Nedostatek operandů
 *          - Neplatný znak
 */
TEST(Calculate, Errors){
    // Dělení nulou
    string expression = "3*8/(2-2)";
    EXPECT_ANY_THROW(calculate(expression));

    // Faktoriál desetinného čísla
    expression = "tan(40)!";
    EXPECT_ANY_THROW(calculate(expression));

    // Faktoríál záporného čísla
    expression = "(-4)!";
    EXPECT_ANY_THROW(calculate(expression));

    // Chybějící začátek závorky
    expression = "3*5)+4";
    EXPECT_ANY_THROW(calculate(expression));

    // Odmocnina záporného čísla
    expression = "2#-4";
    EXPECT_ANY_THROW(calculate(expression));

    // Odmocnina o desetinném exponentu
    expression = "2.5#3";
    EXPECT_ANY_THROW(calculate(expression));

    // Záporná odmocnina o základu nula
    expression = "-3#0";
    EXPECT_ANY_THROW(calculate(expression));

    // Mocnina s desetinným exponentem
    expression = "3^2.5";
    EXPECT_ANY_THROW(calculate(expression));

    // Mocnina se záporným exponentem a základem nula
    expression = "0^(-3)";
    EXPECT_ANY_THROW(calculate(expression));

    // Více operací bez operandů
    expression = "3*/4";
    EXPECT_ANY_THROW(calculate(expression));

    // Více operací bez operandů
    expression = "*89+9";
    EXPECT_ANY_THROW(calculate(expression));

    // Více operací bez operandů
    expression = "89+9*";
    EXPECT_ANY_THROW(calculate(expression));

    // Více operací bez operandů
    expression = "89+9-";
    EXPECT_ANY_THROW(calculate(expression));

    // Dvě čísla bez operace
    expression = "1 2";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "3/";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "^2";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "2#";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "!*5";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "5*s";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "5*c";
    EXPECT_ANY_THROW(calculate(expression));

    // Nedostatek operandů
    expression = "5*t";
    EXPECT_ANY_THROW(calculate(expression));

    // Neplatný znak
    expression = "5*?";
    EXPECT_ANY_THROW(calculate(expression));
}

/**
 * @brief Testovací případ pro situace, které by neměly vyvolat chybu.
 *
 * @details Tento testovací případ ověřuje, že funkce `calculate()` nevyhazuje
 *          výjimky pro situace, které by neměly být považovány za chyby. <br>
 *          __Testuje následující situace:__
 *          - Výrazy se závorkami a zápornými čísly
 *          - Lichá odmocnina záporného čísla
 *          - Výrazy s více plusy za sebou
 *          - Výrazy, které jsou jen jedno číslo
 *          - Výrazy s dvojitým faktoriálem
 *          - Prázdný výraz
 *          - Výrazy s mezerami na začátku
 *          - Výrazy s neuzavřenými závorkami
 */
TEST(Calculate, NoErrors){
    // Test závorek
    string expression = "3 + (-8)";
    EXPECT_NO_THROW(calculate(expression));

    // Test závorek
    expression = "3 - (-8)";
    EXPECT_NO_THROW(calculate(expression));

    // Lichá odmocnina záporného čísla
    expression = "3#-8";
    EXPECT_NO_THROW(calculate(expression));

    // Více + za sebou
    expression = "89++9";
    EXPECT_NO_THROW(calculate(expression));

    // Opsání výrazu
    expression = "3";
    EXPECT_NO_THROW(calculate(expression));
    
    // Opsání výrazu
    expression = "-3";
    EXPECT_NO_THROW(calculate(expression));

    // Dva faktoriály za sebou
    expression = "3!!";
    EXPECT_NO_THROW(calculate(expression));

    // Prázdný výraz
    expression = "";
    string expectedResult= "0";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());
    
    // Mezery na vstupu
    expression = "     ";
    expectedResult= "0";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Závorky na vstupu
    expression = "()((";
    expectedResult= "0";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

    // Závorka na vstupu
    expression = ")";
    expectedResult= "0";
    EXPECT_STREQ(expectedResult.c_str(), calculate(expression).c_str());

}

/**
 * @brief Testovací případ pro základní výrazy.
 *
 * @details Tento testovací případ ověřuje, že funkce `parse()` správně rozdělí
 *          základní výrazy na jednotlivé části. <br>
 *          __Testuje následující situace:__
 *          - Opsání výrazu
 *          - Základní rozdělení
 */
TEST(Parse, BasicExpressions){
    // Opsání výrazu
    vector<string> expected = {"5"};
    vector<string> result = parse("5");
    EXPECT_PRED2(compareVectors, expected, result);

    // Základní rozdělení
    expected = {"5.4", "+", "4", "!"};
    result = parse("5.4    +4 ! ");
    EXPECT_PRED2(compareVectors, expected, result);
}

/**
 * @brief Testovací případ pro parsování složitých výrazů.
 *
 * @details Tento testovací případ ověřuje, že funkce `parse()` správně rozdělí
 *          složitý výraz na jednotlivé části. <br>
 *          __Testuje následující situace:__
 *          - Výraz obsahuje násobení, odečítání, záporné číslo v závorkách,
 *            sčítání, odmocninu, mocninu a mezery na začátku a na konci.
 */
TEST(Parse, ComplexExpressions){
    // Rozdělení složitějšího výrazu
    vector<string> expected = {"558", "*", "0.5", "-", "(", "~", "2", ")",
                                "+", "2", "#", "3", "^", "4"};
    vector<string> result = parse(" 558*0.5 - (-2) + 2#3^4 ");
    EXPECT_PRED2(compareVectors, expected, result);
}

/**
 * @brief Testovací případ pro základní výrazy převedené do postfixové notace.
 *
 * @details Tento testovací případ ověřuje, že funkce `postfix()` správně převede
 *          základní výrazy do postfixové notace. <br>
 *          __Testuje následující situace:__
 *          - Opsání výrazu
 *          - Základní příklad s operací sčítání a faktoriálem
 */
TEST(Postfix, BasicExpressions){
    // Opsání výrazu
    vector<string> expected = {"5"};
    vector<string> result = postfix({"5"});
    EXPECT_PRED2(compareVectors, expected, result);

    // Základní příklad
    expected = {"5.4", "4", "!", "+"};
    result = postfix({"5.4", "+", "4", "!"});
    EXPECT_PRED2(compareVectors, expected, result);
}

/**
 * @brief Testovací případ pro složité výrazy převedené do postfixové notace.
 *
 * @details Tento testovací případ ověřuje, že funkce `postfix()` správně převede
 *          složité výrazy do postfixové notace. <br>
 *          __Testuje následující situace:__
 *          - Složitý výraz obsahující operace násobení, odečítání, negace,
 *            sčítání, odmocniny a mocniny
 */
TEST(Postfix, ComplexExpressions){
    // Složitější výraz
    vector<string> expected = {"558", "0.5", "*", "2", "~", "-",
                                 "2", "3", "4", "^", "#", "+"};
    vector<string> result = postfix({"558", "*", "0.5", "-", "(", "~",
                                    "2", ")", "+", "2", "#", "3", "^", "4"});
    EXPECT_PRED2(compareVectors, expected, result);
}

/**
 * @brief Testovací případ pro vyhodnocení základních výrazů.
 *
 * @details Tento testovací případ ověřuje, že funkce `evaluate()()` správně
 *          vyhodnotí základní výrazy. <br>
 *          __Testuje následující situace:__
 *          - Opsání výrazu
 *          - Základní příklad s operací sčítání a faktoriálem
 */
TEST(Evaluate, BasicExpressions){
    // Opsání výrazu
    string expected = "5";
    string result = evaluate({"5"});
    EXPECT_STREQ(expected.c_str(), result.c_str());

    // Základní příklad
    expected = "29.4";
    result = evaluate({"5.4", "4", "!", "+"});
    EXPECT_STREQ(expected.c_str(), result.c_str());
}

/**
 * @brief Testovací případ pro vyhodnocení složitých výrazů.
 *
 * @details Tento testovací případ ověřuje, že funkce `evaluate()()` správně
 *          vyhodnotí složité výrazy. <br>
 *          __Testuje následující situace:__
 *          - Složitý výraz obsahující operace násobení, odečítání, negace,
 *            sčítání, odmocniny a mocniny
 */
TEST(Evaluate, ComplexExpressions){
    // Složitější výraz
    string expected = "290";
    string result = evaluate({"558", "0.5", "*", "2", "~", "-",
                                 "2", "3", "4", "^", "#", "+"});
    EXPECT_STREQ(expected.c_str(), result.c_str());
}


/*******************************************************************************
 *                                                                             *
 *                    TESTY FUNKCÍ PRO MATEMATICKÉ OPERACE                     *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Testovací případ pro absolutní hodnotu.
 *
 * @details Tento testovací případ ověřuje, že funkce `absVal()` správně
 *          vypočítá absolutní hodnotu čísla.
 */
TEST(Operations, AbsoluteValue){
    EXPECT_EQ(15, absVal(15));
    EXPECT_EQ(15, absVal(-15));
}

/**
 * @brief Testovací případ pro odmocninu.
 *
 * @details Tento testovací případ ověřuje, že funkce `root()` správně vypočítá
 *          odmocninu čísla.
 */
TEST(Operations, Root){
    EXPECT_EQ(5, root(2,25));
    EXPECT_EQ(2, root(12,4096));
    EXPECT_EQ(0.5, root(-3,8));
    EXPECT_EQ(-3, root(3,-27));
    EXPECT_EQ(-1, root(3,-1));
    EXPECT_NEAR(1.286648351223739, root(3, 2.13), TESTED_PRECISION);
    EXPECT_NEAR(1.732050807568877, root(2,3), TESTED_PRECISION);
    EXPECT_NEAR(0.3894322904960899, root(4,0.023), TESTED_PRECISION);
    EXPECT_NEAR(-0.284386697985156, root(3,-0.023), TESTED_PRECISION);
    double expectedResult = 11111.111060555555441;
    EXPECT_NEAR(expectedResult, root(2,123456789), expectedResult*TESTED_PRECISION);
    EXPECT_ANY_THROW(root(2,-16));
}

/**
 * @brief Testovací případ pro mocninu.
 *
 * @details Tento testovací případ ověřuje, že funkce `power()` správně vypočítá
 *          mocninu čísla.
 */
TEST(Operations, Power){
    EXPECT_EQ(64, power(2,8));
    EXPECT_EQ(81, power(4,3));
    EXPECT_EQ(0.25, power(-2,2));
    EXPECT_EQ(16, power(2,-4));
    EXPECT_EQ(-64, power(3,-4));

}

/**
 * @brief Testovací případ pro sinus.
 *
 * @details Tento testovací případ ověřuje, že funkce `csin()` správně vypočítá
 *          sinus úhlu.
 */
TEST(Operations, Sine){
    degRad = true;
    EXPECT_NEAR(0.5, csin(PI/6), TESTED_PRECISION);
    degRad = false;
    EXPECT_NEAR(0.5, csin(30), TESTED_PRECISION);
    EXPECT_NEAR(-0.5, csin(-30), TESTED_PRECISION);
    EXPECT_NEAR(csin(5),csin(365), TESTED_PRECISION);
    EXPECT_NEAR(0.939692620785908, csin(70), TESTED_PRECISION);
}

/**
 * @brief Testovací případ pro kosinus.
 *
 * @details Tento testovací případ ověřuje, že funkce `ccos()` správně vypočítá
 *          kosinus úhlu.
 */
TEST(Operations, Cosine){
    degRad = true;
    EXPECT_NEAR(0.5, ccos(PI/3), TESTED_PRECISION);
    degRad = false;
    EXPECT_NEAR(0.5,ccos(60), TESTED_PRECISION);
    EXPECT_NEAR(0.5,ccos(-60), TESTED_PRECISION);
    EXPECT_NEAR(ccos(6),ccos(366), TESTED_PRECISION);
    EXPECT_NEAR(0.342020143325668, ccos(70), TESTED_PRECISION);
}

/**
 * @brief Testovací případ pro tangens.
 *
 * @details Tento testovací případ ověřuje, že funkce `ctan()` správně vypočítá
 *          tangens úhlu.
 */
TEST(Operations, Tan){
    degRad = true;
    EXPECT_NEAR(1,ctan(PI/4), TESTED_PRECISION);
    degRad = false;
    EXPECT_NEAR(1,ctan(45), TESTED_PRECISION);
    EXPECT_NEAR(ctan(24),ctan(204), TESTED_PRECISION);
    EXPECT_NEAR(2.747477419454622, ctan(70), TESTED_PRECISION);
    EXPECT_ANY_THROW(ctan(270));
}

/**
 * @brief Testovací případ pro faktoriál.
 *
 * @details Tento testovací případ ověřuje, že funkce `factorial()` správně
 *          vypočítá faktoriál čísla.
 */
TEST(Operations, Factorial){
    size_t expectedResult = 24;
    EXPECT_EQ(expectedResult,factorial(4));
    expectedResult = 2432902008176640000;
    EXPECT_EQ(expectedResult,factorial(20));
    EXPECT_ANY_THROW(factorial(-5));
}

/**
 * @brief Testovací případ pro ověření, zda je znak operátorem.
 *
 * @details Tento testovací případ ověřuje, že funkce `isOperator()` správně
 *          rozpozná, zda je znak operátorem.
 */
TEST(Other, IsOperator){
    string testExpression = "4 /(7+1.2)";
    bool expectedResult[] = {false,false,true,false,false,true,false,false,false,false};
    for(int i = 0; i < 10; i++){
        EXPECT_EQ(expectedResult[i], isOperator(testExpression[i]));
    }
}

/*** Konec souboru cat_calc_core_tests.cpp ***/