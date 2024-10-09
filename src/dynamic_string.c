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
 * @brief Inicializace dynamického řetězce s počáteční kapacitou.
*/
string *string_init() {
    // Vytvoříme nový string
    string *stringCreated = malloc(sizeof(string));
    stringCreated->str = malloc(sizeof(STRING_INIT_SIZE));
    // Pokud se špatně alokovala paměť, vrátíme NULL
    if(stringCreated == NULL || stringCreated->str == NULL){
        return NULL;
    }

    stringCreated->allocatedSize = STRING_INIT_SIZE;
    // Na začátku není v řetězci žádný znak
    stringCreated->length = 0;

    return stringCreated;
}

/**
 * @brief Uvolnění paměti dynamického řetězce.
*/
void string_free(string *str) {
    // Pokud string existuje, smažeme ho
    if(str != NULL){
        // Nejdříve ukazatel na pole a až pak strukturu
        free(str->str);
        free(str);
    }
}

/**
 * @brief Přidání jednoho znaku na konec dynamického řetězce.
*/
int string_append_char(string *str, char character) {
    // Pokud je string plný, musíme ho zvětšit o jedno
    if(str->length+1 > str->allocatedSize){
        // Kontrola, zda neselhal resize
        str = string_resize(str, str->length+1);
        if(str == NULL){
            return STRING_RESIZE_FAIL;
        }
    }

    // Pokud máme string, do kterého jde appendovat
    if(str != NULL) {;
        // Přidání znaku na konec
        str->str[str->length] = character;
        // Řetězec se zvětší o jeden
        str->length++;
    }
    return STRING_SUCCESS;
}

/**
 * @brief Zkopíruje obsah jednoho dynamického řetězce do druhého.
*/
int string_copy(string *strCopied, string *strTo){
    // Pokud jeden z řetězců neexistuje, vrátí 0
    if(strCopied == NULL || strTo == NULL){
        return 0;
    }

    // Přendává prvky od str[0] do konce
    for(size_t i = 0; i < strCopied->length-1; i++){
        strTo->str[i] = strCopied->str[i];
    }
    // Délka se při zkopírování může měnit
    strTo->length = strCopied->length;
    return 1;
}

/**
 * @brief Porovná dva dynamické řetězce.
*/
bool string_compare(string *str1, string *str2){
    // Pokud je jeden ze stringů prázdný
    if(str1 == NULL || str2 == NULL){
        return false;
    }

    /* Pokud od začátku víme, že jsou růžně dlouhé oba řetězce,
       potom nemohou být stejné.*/
    if(str1->length != str2->length){
        return false;
    }

    // Projdeme oba stringy
    for(size_t i = 0; i < str1->length; i++){
        // Pokud si jsou odlišné, vrátíme false
        if(str1->str[i] != str2->str[i]){
            return false;
        }
    }
    return true;
}

/**
 * @brief Zvětší dynamický řetězec na požadovanou délku.
 */
string *string_resize(string *str, unsigned int size) {
    // Pokud nemáme řetězec, tak vracíme NULL
    if(str == NULL) {
        return NULL;
    }

    // Vytvoříme nový řetězec
    string *stringCreated = malloc(sizeof(string));
    stringCreated->str = malloc(sizeof(size));

    // Pokud se špatně malokuje, vrací NULL
    if(stringCreated == NULL || stringCreated->str == NULL){
        return NULL;
    }

    // Inicializujeme nový řetězec
    stringCreated->allocatedSize = size;
    stringCreated->length = 0;
    // Zkopíruje znaky z původního řetězce do nově vytvořeného
    if(string_copy(str, stringCreated) == 0){
        // Pokud selže, uvolní nově vytvořený řetězec a vrátí NULL
        free(stringCreated);
        return NULL;
    }

    // Uvolníme původní řetězec
    free(str);
    return stringCreated;
}
