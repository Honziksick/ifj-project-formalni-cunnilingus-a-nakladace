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

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.h" // Vlastní knihovny
#include "error.h"  // (zatím nejsou implementovány)

/**
 * @brief   Datový typ dynamického řetězce.
 * @details Každý řetězec má ukazatel ukazatele na sebe, velikost místa, které
 *          je pro daný řetězec alokované a skutečnou délku, tedy počet znaků
 *          vyskytujících se v řetězci bez ukončovacího znaku '\0'.
 */
typedef struct {
    char *str;                 /**< Ukazatel na dynamické pole znaků, které tvoří řetězec ukončený '\0' */
    unsigned int allocateSize; /**< Velikost alokované paměti (kapacita řetězce včetně volných míst) */
    unsigned int length;       /**< Skutečná délka řetězce (počet znaků před '\0') */
} string;

/**
 * @brief   Inicializace dynamického řetězce s počáteční kapacitou
 * @details
 */
string string_init(unsigned int size);

/**
 * @brief   Uvolnění paměti dynamického řetězce
 * @details
 */
void string_free(string *str);

/**
 * @brief   Přidání jednoho znaku na konec dynamického řetězce
 * @details
 */
int string_append_char(string *str, char character);

/**
 * @brief   Zkopíruje obsah jednoho dynamického řetězce do druhého
 * @details
 */
int string_copy(string *str1, string *str2);

/**
 * @brief   Porovná dva dynamické řetězce
 * @details
 */
int string_compare(string *str1, string *str2);
