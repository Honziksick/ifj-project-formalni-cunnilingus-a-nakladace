/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           error.h                                                   *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            09.10.2024                                                *
 * Poslední změna:   09.10.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file error.h
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Hlavičkový soubory knihovny pro řízení chybových stavů.
 * @details Tento hlavičkový soubor obsahuje deklarace funkcí potřebnách k
 *          řízení chybových stavů. Funkce budou volány ze všech ostatních částí
 *          překladače. Chyby jsou označeny pomocí výčtového typu.
 */

#ifndef ERROR_H_
/** @cond */
#define ERROR_H_
/** @endcond */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Výčtový typ chybových kódu vycházející ze zadání projektu.
 */
typedef enum ErrorType {
    ERROR_LEXICAL = 1,                /**< Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému) */
    ERROR_SYNTAX = 2,                 /**< Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.) */
    ERROR_SEM_UNDEF = 3,              /**< Sémantická chyba v programu – nedefinovaná funkce či proměnná */
    ERROR_SEM_PARAMS_OR_RETVAL = 4,   /**< Sémantická chyba v programu – špatný počet/typ parametrů u volání funkce; špatný typ či nepovolené zahození návratové hodnoty z funkce */
    ERROR_SEM_REDEF_OR_CONSTDEF = 5,  /**< Sémantická chyba v programu – redefinice proměnné nebo funkce; přiřazení do nemodifikovatelné proměnné */
    ERROR_SEM_RETURN_EXP = 6,         /**< Sémantická chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce */
    ERROR_SEM_TYPE_COMPATIBILITY = 7, /**< Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech; nekompatibilní typ výrazu (např. při přiřazení) */
    ERROR_SEM_TYPE_INFERENCE = 8,     /**< Sémantická chyba odvození typu – typ proměnné není uveden a nelze odvodit od použitého výrazu */
    ERROR_SEM_UNUSED_VAR = 9,         /**< Sémantická chyba nevyužité proměnné v jejím rozsahu platnosti; modifikovatelná proměnná bez možnosti změny po její inicializaci */
    ERROR_SEM_OTHER = 10,             /**< Ostatní sémantické chyby */
    ERROR_INTERNAL = 99               /**< Interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.) */
} ErrorType;

/**
 * @brief Funkce pro zpracování chybového kódu.
 *
 * @details Tato funkce je volána z ostatních částí překladače s příslušným
 *          chybovým kódem. Na základě daného chybového kódu zavolá funkci volá
 *          volá další funkce k řízení chybového stavu.
 *
 * @param [in] error Chybový kód typu `ErrorType`, který specifikuje typ chyby.
 */
void error_handle(ErrorType error);

/**
 * @brief Funkce pro tisk příslušné chybové zprávy na `STDERR`.
 *
 * @details Tato funkce vytiskne chybovou hlášku na základě přijatého chybového
 *          kódu. Každý chybový kód má přiřazenou specifickou hlášku, která
 *          je vytištěna na standardní chybový výstup (stderr).
 *
 * @param [in] error Chybový kód typu `ErrorType`, který specifikuje typ chyby.
 */
void error_printMessage(ErrorType error);

/**
 * @brief Funkce pro uvolnění alokovaných dat z haldy před ukončením programu
 *
 * @details Tato funkce uvolní všechna do dané chvíle alokovaná data na základě zadaného chybového kódu.
 *          Uvolnění paměti je důležité pro zabránění únikům paměti a zajištění správného fungování
 *          programu. Funkce může uvolňovat různé typy dat v závislosti na tom, kde došlo k chybě.
 *
 * @param [in] error Chybový kód typu `ErrorType`, který specifikuje typ chyby.
 */
void error_freeAllocatedData(ErrorType error);

/**
 * @brief Funkce pro ukončení programu s příslušným chybovým kódem.
 *
 * @details Tato funkce ukončí program s daným chybovým kódem. Funkce provádí
 *          funkci exit s příslušným chybovým kódem
 *
 * @param [in] error Chybový kód typu `ErrorType`, který specifikuje typ chyby.
 */
void error_killMePlease(ErrorType error);

#endif  // ERROR_H_

/*** Konec souboru error.h ***/
