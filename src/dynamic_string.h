/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           dynamic_string.h                                          *
 * Autor:            Farkašovský Lukáš <xfarkal00>                             *
 *                                                                             *
 * Datum:            08.10.2024                                                *
 * Poslední změna:   27.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
*******************************************************************************/
/**
 * @file dynamic_string.h
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Hlavičkový soubor pro implementaci knihovny pro dynamický řetězec DString.
 * @details Tento soubor obsahuje deklarace funkcí a datových typů pro práci s
 *          dynamickým řetězcem.
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

#define STRING_INIT_SIZE 8    /**< Počáteční velikost dynamického řetězce při inicializaci */
#define DEFAULT_RESIZE_SIZE 8  /**< Délka, o kterou budeme dynamický řetězec zvětšovat */

/**
 * @brief   Výčet návratových hodnot funkcí v knihovně pro dynamický řetězec.
 * @details Tento výčet definuje různé návratové hodnoty,
 *          ke kterým může dojít v určitých funkcích.
 */
typedef enum {
    STRING_SUCCESS,                 /**< Operace se zdařila */
    STRING_ALLOCATION_FAIL,         /**< Selhání alokace paměti */
    STRING_EQUAL,                   /**< Řetězce jsou stejné */
    STRING_NOT_EQUAL,               /**< Řetězce nejsou stejné */
    STRING_COPY_FAIL,               /**< Řetězec se nepovedlo zkopírovat */
    STRING_APPEND_FAIL,             /**< Nepovedlo se přivěsit řetězec */
    STRING_RESIZE_FAIL              /**< Nepovedlo se zvětšení řetězce */
} string_result;

/**
 * @brief   Datový typ dynamického řetězce.
 *
 * @details Každý řetězec má ukazatel na pole znaků, velikost místa, které
 *          je pro daný řetězec alokované a skutečnou délku, tedy počet znaků
 *          vyskytujících se v řetězci.
 */
typedef struct {
    char *str;                      /**< Ukazatel na dynamické pole znaků */
    size_t allocatedSize;           /**< Velikost alokované paměti (kapacita řetězce včetně volných míst) */
    size_t length;                  /**< Skutečná délka řetězce */
} DString;

/**
 * @brief Inicializace dynamického řetězce s počáteční kapacitou.
 *
 * @details Tato funkce alokuje paměť pro nový datový typ řetězce, pro pole znaků
 *          a inicializuje všechny položky na prázdné.
 *
 * @return Při selhání vrací @c NULL.
 *         V opačném případě vrací ukazatel na nově vytvořený datový typ.
*/
DString *string_init();

/**
 * @brief Uvolnění paměti dynamického řetězce.
 *
 * @details Pokud daný řetězec skutečně existuje, uvolní se i s polem uvnitř.
 *
 * @param [in] str Ukazatel na datový typ @c DString
*/
void string_free(DString *str);

/**
 * @brief   Přidání jednoho znaku na konec dynamického řetězce.
 *
 * @details Přidá se znak na konec řetězce jen tehdy, když řetězec existuje.
 *          Pokud je řetězec plný, tak jej zvětšíme o @c DEFAULT_RESIZE_SIZE == 8.
 *
 * @param [in,out] str Ukazatel na datový typ DString
 * @param [in] character Znak, který se vloží na konec řetězce
 *
 * @return Vrací @c STRING_SUCCESS, pokud se vyvedla operace.
 *         Vrací @c STRING_RESIZE_FAIL, pokud se nezdařilo nafouknout datový typ.
 */
int string_append_char(DString *str, char character);

/**
 * @brief   Zkopíruje obsah jednoho dynamického řetězce do druhého.
 *
 * @details Vytvoří nový pomocný řetězec strCreated a přendá všechny parametry z
 *          strCopied a ukazatel na strTo a následně jej uvolní.
 *
 * @param [in] strCopied Řetězec, z kterého kopírujeme
 * @param [in,out] strTo Řetězec do kterého chceme kopírovat
 *
 * @return Pokud jeden z řetězců neexistuje nebo se nezdaří alokace vrátí @c STRING_COPY_FAIL.
 *         V opačném případě vrací @c STRING_SUCCESS.
 */
int string_copy(DString *strCopied, DString *strTo);

/**
 * @brief   Porovná dva dynamické řetězce.
 *
 * @details Projde dva dynamické řetězce a porovná je mezi sebou.
 *
 * @param [in] str1 První dynamický řetězec
 * @param [in] str2 Druhý dynamický řetězec
 *
 * @return Pokud nejsou stejně dlouhé, nebo pokud je ukazatel
 *         na jeden z řetězců prázdný, vrací @c STRING_NOT_EQUAL.
 *         Pokud jsou řetězce stejné, vrací @c STRING_EQUAL.
 */
int string_compare(DString *str1, DString *str2);

/**
 * @brief Porovná dynamický řetězec s konstantním řetězcem.
 *
 * @details Funkce zjistí délku konstantního řetězce a poté jej porovná
 *          s dynamickým řetězcem.
 *
 * @param [in] str Dynamický řetězec
 * @param [in] strConst Konstantní řetězec
 *
 * @return Pokud nejsou stejné řetězce, jejich délka nebo pokud je ukazatel
 *         na dynamické pole prázdný, vrací @c STRING_NOT_EQUAL.
 *         Pokud jsou řetězce stejné, vrací @c STRING_EQUAL.
*/
int string_compare_const_str(DString *str, const char *strConst);

/**
 * @brief   Zvětší dynamický řetězec na požadovanou délku.
 *
 * @details Pokud nastane situace, kdy budeme potřebovat zvětšit řetězec,
 *          je volána tato funkce. Používá se, aby se předešlo k možným
 *          segfaultům. Vytvoříme nový řetězec, do kterého potom zkopírujeme
 *          předešlý řetězec. Obecně se volá s velikostí @c DEFAULT_RESIZE_SIZE == 8.
 *
 * @param [in,out] str Ukazatel na datový typ DString, který chceme zvětšit.
 * @param [in] size Délka, o kterou chceme řetězec zvětšit
 *
 * @return V případě, že požadovaný řetězec neexistuje, špatně se malokuje
 *         nebo se špatně zkopírovalo z původního řetězce
 *         vrací @c NULL.
 *         V případě, že se vše povedlo, vrátí nově zvětšený řetězec.
 */
DString *string_resize(DString *str, size_t size);

/**
 * @brief Převede řetězec obsažený v DString na konstantní řetězec typu char.
 *
 * @details Tato funkce převede dynamický řetězec typu DString na konstantní
 *          řetězec typu char. Před samotným převedením řetězce dojde k alokaci paměti
 *          pro nový konstantní řetězec a doplnění nulového znaku na konec
 *          hodnoty DString.
 *
 * @param [in] str Ukazatel na DString, který chceme převést na char*.
 *
 * @return V případě, že požadovaný řetězec neexistuje nebo se špatně malokuje
 *         vrací @c NULL.
 *         V případě, že se vše povedlo, vrátí ukazatel na nový konstantní řetězec.
 */
char *string_toConstChar(DString *str);

/**
 * @brief Vytvoří nový dynamický řetězec z konstantního řetězce
 *
 * @details Tato funkce vytvoří nový dynamický řetězec z konstantního řetězce.
 *          Velikost a alokovaná paměť se nastaví na délku konstantního řetězce
 *          BEZ ukončujícího znaku '\0'.
 *
 * @param [in] strConst Konstantní řetězec, ze kterého vytvoříme dynamický řetězec
 *
 * @return V případě, že se nepovede alokace paměti, nemáme řetězec
 *         nebo nemá hodnotu vrací @c NULL.
 *         V případě, že se vše povedlo, vrátí ukazatel na nový dynamický řetězec.
 */
DString *string_charToDString(const char *strConst);

#endif  // DYNAMIC_STRING_H_

/*** Konec souboru dynamic_string.h ***/
