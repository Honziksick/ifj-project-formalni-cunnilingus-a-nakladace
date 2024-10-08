/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           dynamic_string.c                                          *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            8.10.2024                                                 *
 * Poslední změna:   8.10.2024                                                 *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
*******************************************************************************/
/**
 * @file dynamic_string.c
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Definice a implementace funkcí modulu scanner.
 * @details Tento soubor obsahuje deklaraci funkcí a datových typů knihovny
 *          pro dynamický string.
*/

#include "dynamic_string.h"

/**
 * @brief Alokuje parametry stringu a pole
*/
inline string *string_allocate(size_t size){
    return malloc(sizeof(string)+size);
}

/**
 * @brief Inicializace dynamického řetězce s počáteční kapacitou
*/
string *string_init() {
    // Vytvoříme nový string
    string *stringCreated = string_allocate(STRING_INIT_SIZE);
    // Pokud se špatně alokovala paměť, vrátíme NULL
    if(stringCreated == NULL){
        return NULL;
    }

    stringCreated->allocatedSize = STRING_INIT_SIZE;
    // Na začátku není v řetězci žádný znak
    stringCreated->length = 0;
    stringCreated->str = NULL;

    return stringCreated;
}

/**
 * @brief Uvolnění paměti dynamického řetězce
*/
void string_free(string *str){
    // Pokud string existuje
    if(str != NULL){
        free(str);
    }
}

/**
 * @brief Přidání jednoho znaku na konec dynamického řetězce
*/
int string_append_char(string *str, char character);

/**
 * @brief Zkopíruje obsah jednoho dynamického řetězce do druhého
*/
int string_copy(string *str1, string *str2);

/**
 * @brief Porovná dva dynamické řetězce
*/
int string_compare(string *str1, string *str2);
