/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           dynamic_string.c                                          *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                                                                             *
 * Datum:            8.10.2024                                                 *
 * Poslední změna:   9.10.2024                                                 *
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
DString *string_init() {
    // Vytvoříme nový string
    DString *stringCreated = malloc(sizeof(DString));
    stringCreated->str = malloc(sizeof(STRING_INIT_SIZE));
    // Pokud se špatně alokovala paměť, vrátíme NULL
    if(stringCreated == NULL || stringCreated->str == NULL) {
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
void string_free(DString *str) {
    // Pokud string existuje, smažeme ho
    if(str != NULL) {
        // Nejdříve ukazatel na pole a až pak strukturu
        free(str->str);
        free(str);
    }
}

/**
 * @brief Přidání jednoho znaku na konec dynamického řetězce.
*/
int string_append_char(DString *str, char character) {
    // Pokud je string plný, musíme ho zvětšit o jedno
    if(str->length+1 > str->allocatedSize) {
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
int string_copy(DString *strCopied, DString *strTo) {
    // Pokud jeden z řetězců neexistuje, vrátí 0
    if(strCopied == NULL || strTo == NULL) {
        return STRING_COPY_FAIL;
    }

    DString *stringCreated = malloc(sizeof(DString));
    stringCreated->str = malloc(sizeof(strCopied->allocatedSize));

    // Přidělení pole
    stringCreated->str = strCopied->str;
    // Délka se při zkopírování může měnit
    stringCreated->length = strCopied->length;
    // Přendáme ukazatele, aby ukazoval na správnou překopírovanou strukturu
    *stringCreated = *strTo;

    string_free(strTo);
    return STRING_SUCCESS;
}

/**
 * @brief Porovná dva dynamické řetězce.
*/
int string_compare(DString *str1, DString *str2) {
    // Pokud je jeden ze stringů prázdný
    if(str1 == NULL || str2 == NULL) {
        return STRING_NOT_EQUAL;
    }

    /* Pokud od začátku víme, že jsou růžně dlouhé oba řetězce,
       potom nemohou být stejné.*/
    if(str1->length != str2->length) {
        return STRING_NOT_EQUAL;
    }

    // Projdeme oba stringy
    for(size_t i = 0; i < str1->length; i++) {
        // Pokud si jsou odlišné, vrátíme false
        if(str1->str[i] != str2->str[i]){
            return STRING_NOT_EQUAL;
        }
    }
    return STRING_EQUAL;
}

/**
 * @brief Porovná dynamický řetězec s konstantním řetězcem.
*/
int string_compare_const_str(DString *str, const char *strConst) {
    // Pokud porovnávaný string neexistuje
    if(str == NULL) {
        return STRING_NOT_EQUAL;
    }
    // Proměnná pro uložení délky strConst
    size_t length = strlen(strConst);
    // Pokud nejsou stejně dlouhé, tak nejsou ani stejné
    if(str->length != length) {
        return STRING_NOT_EQUAL;
    }
    // Projdeme oba řetězce a porovnáme je
    for(size_t i = 0; i < str->length; i++) {
        // Pokud nejsou stejné vracíme STRING_NOT_EQUAL
        if(str->str[i] != strConst[i]) {
            return STRING_NOT_EQUAL;
        }
    }
    return STRING_EQUAL;
}

/**
 * @brief Zvětší dynamický řetězec na požadovanou délku.
 */
DString *string_resize(DString *str, size_t size) {
    // Pokud nemáme řetězec, tak vracíme NULL
    if(str == NULL) {
        return NULL;
    }

    // Vytvoříme nový řetězec
    DString *stringCreated = malloc(sizeof(DString));
    stringCreated->str = malloc(sizeof(size));

    // Pokud se špatně malokuje, vrací NULL
    if(stringCreated == NULL || stringCreated->str == NULL) {
        return NULL;
    }

    // Inicializujeme nový řetězec
    stringCreated->allocatedSize = size;
    stringCreated->length = 0;
    // Zkopíruje znaky z původního řetězce do nově vytvořeného
    if(string_copy(str, stringCreated) == 0) {
        // Pokud selže, uvolní nově vytvořený řetězec a vrátí NULL
        free(stringCreated);
        return NULL;
    }

    // Uvolníme původní řetězec
    free(str->str);
    free(str);
    return stringCreated;
}

/*** Konec souboru dynamic_string.c ***/
