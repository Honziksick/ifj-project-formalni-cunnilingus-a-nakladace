/*******************************************************************************
 *                                                                             *
 * Název projektu: Implementace překladače imperativního jazyka IFJ24          *
 *                                                                             *
 * Soubor:         dynamic_string.h                                            *
 * Autor:          Farkašovský Lukáš <xfarkal00>                               *
 *                                                                             *
 * Datum:          08.10.2024                                                  *
 * Poslední změna: 08.10.2024                                                  *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš <xfarkal00>                                     *
 *           Hýža Pavel <xhyzapa00>                                            *
 *           Kalina Jan <xkalinj00>                                            *
 *           Krejčí David <xkrejcd00>                                          *
 *                                                                             *
*******************************************************************************/
/**
 * @file dynamic_string.h
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Hlavičkový soubor pro implementaci knihovny pro dynamický string.
 * @details Tento soubor obsahuje deklarace funkcí a datových typů pro práci s
 *          dynamickým stringem.
 *
*/

#ifndef DYNAMIC_STRING_H_
/** @cond */
#define DYNAMIC_STRING_H_
/** @endcond */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "error.h"

//Počáteční velikost dynamického řetězce při inicializaci
//10 volných míst
#define STRING_INIT_SIZE 8

/**
 * @brief   Výčet návratových hodnot funkcí
 * @details Tento výčet definuje různé návratové hodnoty,
 *          ke kterým může dojít v určitých funkcích.
 */
typedef enum {
    STRING_SUCCESS,                 /**< Operace se zdařila */
    STRING_ALLOCATION_FAIL,         /**< Selhání alokace paměti */
    STRING_EQUAL,                   /**< Řetězce jsou stejné */
    STRING_NOT_EQUAL,               /**< Řetězce nejsou stejné */
    STRING_COPY_FAIL,               /**< Řetězec se nepovedlo zkopírovat */
    STRING_APPEND_FAIL,             /**< Nepovedlo se appendnout řetězec */
    STRING_RESIZE_FAIL              /**< Nepovedlo se zvětšení řetězce */
} string_result;

/**
 * @brief   Datový typ dynamického řetězce.
 *
 * @details Každý řetězec má ukazatel ukazatele na sebe, velikost místa, které
 *          je pro daný řetězec alokované a skutečnou délku, tedy počet znaků
 *          vyskytujících se v řetězci.
 */
typedef struct {
    char *str;                      /**< Ukazatel na dynamické pole znaků */
    size_t allocatedSize;     /**< Velikost alokované paměti (kapacita řetězce včetně volných míst) */
    size_t length;            /**< Skutečná délka řetězce */
} string;

/**
 * @brief Inicializace dynamického řetězce s počáteční kapacitou.
 *
 * @details Tato funkce alokuje paměť pro nový datový typ řetězce, pro pole znaků
 *          a inicializuje všechny položky na prázdné.
 *
 * @return Vrací ukazatel na nově vytvořený datový typ nebo NULL, když selže.
*/
string *string_init();

/**
 * @brief Uvolnění paměti dynamického řetězce.
 *
 * @details Pokud daný string skutečně existuje, uvolní se i s pole vevnitř.
 *
 * @param str Ukazatel na datový typ string
*/
void string_free(string *str);

/**
 * @brief   Přidání jednoho znaku na konec dynamického řetězce.
 *
 * @details Přidá se znak na konec řetězce jen tehdy, když řetězec existuje.
 *          Pokud je řetězec plný, tak jej zvětšíme na požadovanou délku, což
 *          je o jeden znak větší.
 *
 * @param str Ukazatel na datový typ string
 * @param character Znak, který se vloží na konec řetězce
 *
 * @return Vrací STRING_SUCCESS, pokud se vyvedla operace.
 *         Vrací STRING_RESIZE_FAIL, pokud se nezdařilo nafouknout datový typ.
 */
int string_append_char(string *str, char character);

/**
 * @brief   Zkopíruje obsah jednoho dynamického řetězce do druhého.
 *
 * @details Vytvoří nový pomocný řetězec strCreated a přendá všechny parametry z
 *          strCopied a ukazatel na strTo a uvolní strTo.
 *
 * @param strCopied Řetězec, z kterého kopírujeme
 * @param strTo Řetězec do kterého chceme kopírovat
 *
 * @return Pokud jeden z řetězců neexistující vrátí STRING_COPY_FAIL.
 *         V opačném případě vrací STRING_SUCCESS.
 */
int string_copy(string *strCopied, string *strTo);

/**
 * @brief   Porovná dva dynamické řetězce.
 *
 * @details Projde dva stringy a porovná je mezi sebou.
 *
 * @param str1 První řetězec
 * @param str2 Druhý řetězec
 *
 * @return Vrací true, pokud jsou stejné, jinak vrací false.
 */
int string_compare(string *str1, string *str2);

/**
 * @brief Porovná dynamický řetězec s konstantním řetězcem.
 *
 * @details Funkce zjistí délku konstantního řetězce a poté jej porovná
 *          s dynamickým řetězcem.
 *
 * @param str Dynamický řetězec
 * @param strConst Konstantní řetězec
 *
 * @return Pokud nejsou stejné řetězce, jejich délka nebo pokud je ukazatel
 *         na dynamický pole prázdný, vrací STRING_NOT_EQUAL.
 *         Pokud jsou řetězce stejné, vrací STRING_EQUAL.
*/
int string_compare_const_str(string *str, const char *strConst);

/**
 * @brief   Zvětší dynamický řetězec na požadovanou délku.
 *
 * @details Pokud nastane situace, kdy budeme potřebovat zvětšit řetězec,
 *          je volána tato funkce. Používá se, aby se předešlo k možným
 *          segfaultům. Vytvoříme nový řetězec, do kterého potom zkopírujeme
 *          předešlý řetězec.
 *
 * @param str Ukazatel na datový typ string, který chceme zvětšit.
 *
 * @return V případě, že požadovaný řetězec neexistuje, špatně se malokuje
 *         nebo se špatně zkopírovalo z původního řetězce
 *         vrací NULL.
 *         V případě, že se vše povedlo, vrátí nově zvětšený řetězec.
 */
string *string_resize(string *str, size_t size);

#endif  // DYNAMIC_STRING_H_

/*** Konec souboru dynamic_string.h ***/
