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

#include "error.h"  // (zatím nejsou implementovány)

//Počáteční velikost dynamického řetězce při inicializaci
//10 volných míst
#define STRING_INIT_SIZE 10

/**
 * @brief   Datový typ dynamického řetězce.
 *
 * @details Každý řetězec má ukazatel ukazatele na sebe, velikost místa, které
 *          je pro daný řetězec alokované a skutečnou délku, tedy počet znaků
 *          vyskytujících se v řetězci.
 */
typedef struct {
    char *str;                 /**< Ukazatel na dynamické pole znaků */
    unsigned int allocatedSize; /**< Velikost alokované paměti (kapacita řetězce včetně volných míst) */
    unsigned int length;       /**< Skutečná délka řetězce */
} string;

/**
 * @brief Alokuje paměť pro dynamické pole.
 *
 * @param size Velikost dynamického pole
 *
 * @return Ukazatel na nové dynamické pole nebo NULL v případě chyby.
 */
string *string_allocate(size_t size);

/**
 * @brief Inicializace dynamického řetězce s počáteční kapacitou.
 *
 * @details Tato funkce alokuje paměť pro nový datový typ řetězce a inicializuje
 *          všechny položky na prázdné.
 *
 * @return Vrací ukazatel na nově vytvořený datový typ.
*/
string string_init();

/**
 * @brief Uvolnění paměti dynamického řetězce.
 *
 * @details Pokud daný string skutečně existuje, uvolní se, jinak nic nedělá.
 *
 * @param str Ukazatel na datový typ string
 *
 * @return Vrací vytvořený dynamický řetězec.
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
 */
int string_append_char(string *str, char character);

/**
 * @brief   Zkopíruje obsah jednoho dynamického řetězce do druhého.
 *
 * @details Zkopírujeme obsah z řetězce strCopied do řetězce strTo a
 *          zároveň se také strTo->length změní na délku strCopied.
 *
 * @param strCopied Řetězec, z kterého kopírujeme
 * @param strTo Řetězec do kterého kopírujeme
 *
 * @return Pokud jeden z řetězců neexistující nebo kopířujeme do
 *         menšího řetězce, vrátí 0.
 *         V opačném případě vrací 1.
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
bool string_compare(string *str1, string *str2);

/**
 * @brief   Zvětší dynamický řetězec na požadovanou délku.
 *
 * @details Pokud nastane situace, kdy budeme potřebovat zvětšit řetězec,
 *          je volána tato funkce. Používá se, aby se předešlo k možným
 *          segfaultům. Vytvoříme nový řetězec, do kterého potom zkopírujeme
 *          předešlý řetězec.
 *
 * @param str Ukazatel na datový typ string, který chceme zvětšit
 *
 * @return V případě, že požadovaný řetězec neexistuje, špatně se malokuje
 *         nebo se špatně zkopírovalo z původního řetězce
 *         vrací NULL.
 *         V případě, že se vše povedlo, vrátí nově zvětšený řetězec.
 */
string string_resize(string *str, size_t size);
