/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           error.h                                                   *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            09.10.2024                                                *
 * Poslední změna:   16.11.2024                                                *
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
 * @brief Hlavičkový soubor knihovny pro řízení chybových stavů.
 * @details Tento hlavičkový soubor obsahuje deklarace funkcí potřebných k
 *          řízení chybových stavů v překladači jazyka IFJ24. Funkce v tomto
 *          souboru zajišťují zpracování chybových kódů, tisk chybových zpráv,
 *          uvolnění alokovaných dat a ukončení programu s příslušným chybovým
 *          kódem. Funkce také poskytují informace o souboru, řádku a funkci,
 *          kde chyba nastala, a umožňují extrakci názvu souboru z cesty.
 */

#ifndef ERROR_H_
/** @cond */
#define ERROR_H_
/** @endcond */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*******************************************************************************
 *                                                                             *
 *                              DEFINICE KONSTANT                              *
 *                                                                             *
 ******************************************************************************/

#define RED_COLOR "\033[91m"        /**< ANSI escape sekvence pro červenou barvu */
#define YELLOW_COLOR "\033[33m"     /**< ANSI escape sekvence pro žlutou barvu */
#define RESET_COLOR "\033[0m"       /**< ANSI escape sekvence pro resetování barvy */


/*******************************************************************************
 *                                                                             *
 *                             VÝČTOVÉ DATOVÉ TYPY                             *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Výčtový typ chybových kódů vycházející ze zadání projektu.
 *
 * @details Tento výčtový typ definuje různé chybové kódy, které mohou nastat
 *          během lexikální, syntaktické a sémantické analýzy programu v jazyce
 *          IFJ24. Každý chybový kód je přiřazen specifickému typu chyby, který
 *          je podrobně popsán v komentářích u jednotlivých hodnot výčtu.
 */
typedef enum ErrorType {
    ERROR_LEXICAL                = 1,   /**< Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému). */
    ERROR_SYNTAX                 = 2,   /**< Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.). */
    ERROR_SEM_UNDEF              = 3,   /**< Sémantická chyba v programu – nedefinovaná funkce či proměnná. */
    ERROR_SEM_PARAMS_OR_RETVAL   = 4,   /**< Sémantická chyba v programu – špatný počet/typ parametrů u volání funkce; špatný typ či nepovolené zahození návratové hodnoty z funkce. */
    ERROR_SEM_REDEF_OR_CONSTDEF  = 5,   /**< Sémantická chyba v programu – redefinice proměnné nebo funkce; přiřazení do nemodifikovatelné proměnné. */
    ERROR_SEM_RETURN_EXP         = 6,   /**< Sémantická chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce. */
    ERROR_SEM_TYPE_COMPATIBILITY = 7,   /**< Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech; nekompatibilní typ výrazu (např. při přiřazení). */
    ERROR_SEM_TYPE_INFERENCE     = 8,   /**< Sémantická chyba odvození typu – typ proměnné není uveden a nelze odvodit od použitého výrazu. */
    ERROR_SEM_UNUSED_VAR         = 9,   /**< Sémantická chyba nevyužité proměnné v jejím rozsahu platnosti; modifikovatelná proměnná bez možnosti změny po její inicializaci. */
    ERROR_SEM_OTHER              = 10,  /**< Ostatní sémantické chyby. */
    ERROR_INTERNAL               = 99,  /**< Interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.). */
} ErrorType;


/*******************************************************************************
 *                                                                             *
 *                     DEKLARACE VEŘEJNÝCH FUNKCÍ A MAKER                      *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Makro pro automatické předání informací o souboru, řádku a funkci.
 *
 * @details Toto makro volá funkci `error_internalHandle` a automaticky předává
 *          informace o souboru, řádku a funkci, odkud byla chyba volána. Používá
 *          předdefinované makro konstanty `__FILE__`, `__LINE__` a `__func__`.
 *
 * @param [in] error Chybový kód typu ErrorType.
 */
#define error_handle(error) error_internalHandle(error, __FILE__, __LINE__, __func__)


/*******************************************************************************
 *                                                                             *
 *                         DEKLARACE INTERNÍCH FUNKCÍ                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Funkce pro zpracování chybového kódu.
 *
 * @details Tato funkce zpracuje chybový kód, vytiskne odpovídající chybovou
 *          zprávu spolu s informacemi o souboru, řádku a funkci, kde chyba
 *          nastala, a ukončí program s příslušným chybovým kódem.
 *
 * @param [in] error Chybový kód typu ErrorType.
 * @param [in] file Název souboru ze kterého byla chyba volána.
 * @param [in] line Číslo řádku ve zdrojovém souboru.
 * @param [in] func Název funkce, ze které byla chyba volána.
 */
void error_internalHandle(ErrorType error, const char *file, int line, const char *func);

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

/**
 * @brief Extrahuje název souboru z cesty.
 *
 * @details Tato funkce zpracuje zadanou cestu k souboru a vrátí pouze název
 *          souboru. Pokud cesta neobsahuje žádný znak '/', vrátí původní cestu,
 *          která je již názvem souboru.
 *
 * @param [in] path Cesta k souboru. Může obsahovat absolutní nebo relativní cestu.
 *
 * @return Ukazatel na název souboru. Pokud cesta obsahuje '/', vrátí ukazatel
 *         na následující znak po posledním výskytu '/', což je název souboru.
 *         Pokud cesta neobsahuje '/', vrátí původní cestu.
 *
 * @note Funkce neprovádí žádné alokace paměti, vrací ukazatel na část vstupního
 *       řetězce.
 */
const char *error_getFileName(const char *path);

#endif  // ERROR_H_

/*** Konec souboru error.h ***/
