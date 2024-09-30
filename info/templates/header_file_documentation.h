/*******************************************************************************
 *                                                                             *
 * Název projektu:   Calm CatCalc                                              *
 *                                                                             *
 * Soubor:           stddev.cpp                                                *
 * Datum:            01.04.2024                                                *
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
 * @file stddev.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Deklarace funkcí pro výpočet výběrové směrodatné odchylky.
 * @details Deklarované funkce zahrnují načítání dat ze standardního vstupu nebo
 *          ze souboru, výpočet součtu a součtu druhých mocnin načtených hodnot,
 *          a výpočet výběrové směrodatné odchylky. Tento soubor také definuje
 *          také konstanty, a výčtový typ pro návratové hodnoty funkcí.
 *
 * @see Deklarace funkcí tohoto hlavičkového souboru naleznete v sekci
 *      `Prostory jmen` u jmenného prostoru \link catStddev \endlink.
 *
 * @note __Tabulka vyjímek, které mohou nastat:__ <br>
 * |       Výjimka      |                         Popis                         |
 * | ------------------ | ----------------------------------------------------- |
 * | `invalid_argument` | Byly předány neplatné vstupní hodnoty (tj. nečíselné) |
 * | `runtime_error`    | Nepodařilo se otevřít soubor s vstupními daty.        |
 */

#pragma once
#ifndef STDDEV_H_

/** @cond */
#define STDDEV_H_
/** @endcond */

#include "cat_calc_core.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <random>
#include <regex>

#ifdef DEBUG
#define LOG0(s) fprintf(stderr, "%s:%-3u:%-20s" s "\n", __FILE__, __LINE__, __func__)
#define LOG(s, ...) fprintf(stderr, "%s:%-3u:%-20s" s "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
/**
 *
 * @brief Makra pro logování programu v režimu ladění na `stderr`.
 *
 * @details Tato makra vypisují zprávu na standardní chybový výstup (`stderr`).
 *          Zpráva obsahuje název souboru, číslo řádku a název funkce, kde bylo
 *          makro `LOG0` zavoláno, spolu se zprávou.
 *
 * @param[in] s Řetězec pro zprávu ve formátovacích specifikátorů.
 *
 * @note Pro vypnutí logování je třeba zakomentovat makro `DEBUG`.
 *
 */
#define LOG0(s) do {} while (0)

/**
 *
 * @brief Makra pro logování programu v režimu ladění na `stderr`.
 *
 * @details Tato makra vypisují zprávu na standardní chybový výstup (`stderr`).
 *          Zpráva obsahuje název souboru, číslo řádku a název funkce, kde bylo
 *          makro `LOG` zavoláno, spolu se zprávou.
 *
 * @param[in] s Formátovací řetězec pro zprávu. `LOG()` může kromě výstupního
 *              řetězce obsahovat také libovolný počet formátovacích
 *              specifikátorů, které jsou nahrazeny dalšími argumenty makra. <br>
 *         @c ... Další argumenty makra. Tyto argumenty jsou vloženy do
 *                formátovacího řetězce `s`.
 *
 * @note Pro vypnutí logování je třeba zakomentovat makro `DEBUG`.
 *
 */
#define LOG(s, ...) do {} while (0)
#endif


//Definice konstant pro generátor náhodných čísel
#define FILE_PATH "auto_gen.txt"  /**< Název generovaného souboru s náhodným daty */
#define AUTO_GEN_NUM  100000      /**< Počet náhodně generovaných čísel    */
#define AUTO_GEN_MIN -10000       /**< Nejmenší možná vygenerovaná hodnota */
#define AUTO_GEN_MAX  10000       /**< Největší možná vygenerovaná hodnota */
#define PRECISION 4        /**< Přesnost desetinné části vygenerovaných čísel */


// Definice konstant (stringů) matematických operací pro funkci 'calculate()'
#define ADD_OP "+"      /**< Operátor sčítání v podobě `string` */
#define SUB_OP "-"      /**< Operátor odčítání v podobě `string` */
#define MUL_OP "*"      /**< Operátor násobení v podobě `string` */
#define DIV_OP "/"      /**< Operátor dělení v podobě `string`  */
#define POW_OP  "^2"    /**< Operátor umocnění na druhou v podobě `string` */
#define ROOT_OP "2#"    /**< Operátor druhé odmocniny v podobě `string` */

// Definice konstant (znaků) pro závorky
#define O_BRACKET_SYM "("    /**< Otevírací závorka v podobě `string` */
#define C_BRACKET_SYM ")"    /**< Zavírací závorka v podobě `string` */


/**
 * @namespace MathSymbols
 * @brief Obsahuje třídy a konstanty pro matematické symboly.
 *
 * @details Tento namespace obsahuje třídy a konstanty pro různé matematické
 *          symboly, které jsou využívány matematickou knihovnou
 *          _Calm CatCalc Core_.
 */
namespace MathSymbols{
    /**
     * @class CoreOperands
     * @brief Třída obsahující operandy využívané matematickou knihovnou
     *        _Calm CatCalc Core_.
     *
     * @details Definované konstanty jsou statické, takže mohou být přístupné
     *          bez vytvoření instance třídy CoreOperands.
     */
    class CoreOperands{
    public:
        static const std::string ADD;        /**< Symbol pro sčítání.   */
        static const std::string SUB;        /**< Symbol pro odečítání. */
        static const std::string MUL;        /**< Symbol pro násobení.  */
        static const std::string DIV;        /**< Symbol pro dělení.    */
        static const std::string POW;        /**< Symbol pro mocninu.   */
        static const std::string ROOT;       /**< Symbol pro odmocninu. */
    };

    /**
     * @class CoreSymbols
     * @brief Třída obsahující konstanty pro další matematické symboly.
     *
     * @details Tyto konstanty jsou statické, takže mohou být přístupné bez
     *          vytvoření instance třídy CoreSymbols.
     */
    class CoreSymbols{
    public:
        static const std::string O_BRACKET;  /**< Symbol pro otevírací závorku. */
        static const std::string C_BRACKET;  /**< Symbol pro uzavírací závorku. */
    };
}

/**
 * @brief Výčtový typ pro návratové hodnoty funkcí.
 */
enum returnValues{
    OK,             /**< OK:    Funkce skončila úspěšně */
    E_INV_DATA,     /**< Error: Načtení nečíselné hodnoty */
    E_FILE_OPEN     /**< Error: Nepodařilo se otevřít soubor se vstupními daty */
};


/**
 * @namespace catStddev
 * @brief Obsahuje funkce pro výpočet standardní směrodatné odchylky.
 *
 * @details Tento namespace obsahuje třídy a funkce, které jsou využívány pro
 *          výpočet standardní směrodatné odchylky pomocí matematické knihovny
 *          _Calm CatCalc Core_.
 */
namespace catStddev{

/**
 * @brief Funkce na výpis nápovědy k programu `stddev`.
 *
 * @details Funkce `help()` vypíše na standardní výstup nápovědu k programu na
 *          výpočet výběrové směrodatné odchylky. Nápověda obsahuje informace o
 *          tom, jak program používat, jaké jsou možné vstupy a jaké jsou
 *          návratové hodnoty programu.
 *
 * @note Nápovědu může uživatel zobrazit spuštěním programu s parametrem `-h`
 *       nebo `--help`.
 */
void help();


/**
 * @brief Funkce pro generování náhodných čísel do souboru `auto_gen.txt`.
 *
 * @details Tato funkce generuje náhodná čísla v rozsahu definovaném konstantami
 *          `PRECISION_MIN` a `PRECISION_MAX` a zapisuje je do souboru určeného
 *          konstantou `FILE_PATH`. Počet generovaných čísel je určen konstantou
 *          `AUTO_GEN_NUM`. Generovaná čísla jsou vypsána na standardní výstup a
 *          zapsána do souboru s pevnou přesností určenou konstantou
 *          `PRECISION`.
 */
void generateNumbers();


/**
 * @brief Funkce pro načítání čísel z datového proudu.
 *
 * @details Tato funkce načítá hodnoty ze zadaného datového proudu (může to být
 *          standardní vstup nebo soubor) do doby, dokud jsou k dispozici data.
 *          Pokud načtená hodnota není číslo, funkce vyhodí výjimku typu
 *          `invalid_argument`.
 *
 * @param [in] dataStream Reference na datový proud (`istream`), ze kterého se
 *                        načítají data.
 * @param [in] valueSum Reference na řetězec, který uchovává součet načtených
 *                      hodnot.
 * @param [in] powerSum Reference na řetězec, který uchovává součet druhých mocnin.
 *                      načtených hodnot
 * @param [in] N Reference na počítadlo načtených hodnot.
 *
 * @throws invalid_argument Pokud byly předány neplatné vstupní hodnoty (tj. nečíselné).
 */
void readData(istream& dataStream, string &valueSum, string &powerSum, unsigned long &N);


/**
 * @brief Funkce pro načítání čísel ze vygenerovaného souboru `auto_gen.txt`.
 *
 * @details Tato funkce otevírá soubor určený konstantou `FILE_PATH` a volá
 *          funkci `readData()` s otevřeným souborem jako parametrem, což
 *          umožňuje načítání dat přímo ze souboru. Pokud se soubor nepodaří
 *          otevřít, vypíše chybovou zprávu "Unable to open file".
 *
 * @param [in] valueSum Reference na řetězec, který uchovává součet načtených
 *                      hodnot.
 * @param [in] powerSum Reference na řetězec, který uchovává součet druhých
 *                      mocnin načtených hodnot.
 * @param [in] N Reference na počítadlo načtených hodnot.
 *
 * @throws runtime_error` Pokud se nepodařilo otevřít soubor s vstupními daty.
 *
 * @see readData()
 */
void readDataFromAutoGenFile(string &valueSum, string &powerSum, unsigned long &N);


/**
 * @brief Funkce pro načítání čísel ze standardního vstupu (`stdin`).
 *
 * @details Tato funkce volá funkci `readData()` s `cin` jako parametrem, což
 *          umožňuje načítání dat přímo ze standardního vstupu. Je určena
 *          k použití, když je program spuštěn s přesměrováním vstupu ze
 *          souboru, například `./stddev < data.txt`.
 *
 * @param [in] valueSum Reference na řetězec, který uchovává součet načtených
 *                      hodnot.
 * @param [in] powerSum Reference na řetězec, který uchovává součet druhých
 *                      mocnin načtených hodnot.
 * @param [in] N Reference na počítadlo načtených hodnot.
 *
 * @see readData()
 */
void readDataFromStdin(string &valueSum, string &powerSum, unsigned long &N);


/**
 * @brief Funkce na výpočet výběrové směrodatné odchylky.
 *
 * @details V případě, že je funkce spuštěna jako `./stddev < data.txt`, je
 *          výběrová směrodatná odchylka vypočtena z hodnot v souboru `data.txt`.
 *          V opačném případě je generátor náhodných čísel vygenerován soubor
 *          s daty `auto_gen.txt` a vstup programu je braný právě z tohoto souboru.
 *
 * @note __Vzorec pro výpočet výběrové směrodatné odchylky:__
 *       \f[ s=\sqrt{\frac{1}{N-1}\left(\sum_{i=1}^{N}x_{i}^{2}-
 *           N\overline{x}^{2}\right)} \f]
 *
 * @return
 * - Výsledek výpočtu výběrové směrodatné odchylky.
 */
string standardDeviation();

}  // konec catStddev


/**
 * @brief Hlavní funkce programu.
 *
 * @details Tato funkce volá funkci `standardDeviation()` pro výpočet výběrové
 *          směrodatné odchylky. Výsledek je vypsán na standardní výstup
 *          (`stdout`). Pokud funkce standardDeviation() vyhodí výjimku typu
 *          `invalid_argument`, chybová zpráva je vypsána na standardní chybový
 *          výstup (`stderr`) a funkce `main()` vrátí `1`. Pokud nedošlo k žádné
 *          chybě, funkce `main()` vrátí `0`. <br>
 *          Pokud je jako argument předán přepínač `-h` nebo `--help`, funkce
 *          vypíše nápovědu k programu a vrátí `0`.
 *
 * @return
 * - @c OK Pokud nedošlo k žádné chybě
 * - @c E_INV_DATA Pokud byla vyhozena výjimku typu `invalid_argument`
 * - @c E_FILE_OPEN Pokud byla vyhozena výjimka typu `runtime_error`
 */
int main(int argc, char **argv);

#endif // STDDEV_H_

/*** Konec souboru stddev.h ***/
