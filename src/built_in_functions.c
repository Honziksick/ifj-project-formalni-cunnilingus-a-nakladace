/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           built_in_functions.c                                      *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            22.11.2024                                                *
 * Poslední změna:   26.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file built_in_functions.c
 * @author Hýža Pavel \<xhyzapa00>
 *
 * @brief Vestavěné funkce jazyka IFJ24
 * @details Tento soubor obsahuje definice vestavěných funkcí jazyka IFJ24
 *          pomocí assembler instrukcí poskytovaných jazykem IFJcode24 pro
 *          generátor kódu tac_generator.
 */

#include "built_in_functions.h"

//Ještě chybí dodělat 4 funkce
void built_in_functions() {

    char built_in_fun_substring[] = "\n\
            #---------------------------------------------------\n\
            #pub fn ifj.substring(s: []u8, i: i32, j: i32) ?[]u8\n\
            LABEL $$ifj$substring\n\
            PUSHFRAME\n\
            \n\
                LT GF@$?tempDEST LF@i int@0\n\
                JUMPIFEQ $$$null GF@$tempDEST bool@true\n\
                \n\
                LT GF@$?tempDEST LF@j int@0\n\
                JUMPIFEQ $$$null GF@$tempDEST bool@true\n\
                \n\
                GT GF@$?tempDEST LF@i LF@j\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
                \n\
                STRLEN GF@?tempSRC1 LF@s\n\
                \n\
                GT GF@$?tempDEST LF@i GF@?tempSRC1\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
                EQ GF@$?tempDEST LF@i GF@?tempSRC1\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
                \n\
                GT GF@$?tempDEST LF@j GF@?tempSRC1\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
            \n\
            LABEL $$$while\n\
            \n\
                JUMPIFEQ $$$sub_end TF@i TF@j\n\
                \n\
                GETCHAR GF@?tempSRC1 LF@s LF@i\n\
                CONCAT GF@?tempDEST GF@?tempDEST GF@?tempSRC1\n\
                ADD LF@i LF@i int@1\n\
                JUMP $$$while\n\
                \n\
                PUSHS GF@?tempDEST\n\
                JUMP &&&sub_end\n\
            \n\
            LABEL $$$null\n\
                PUSHS nil@nil\n\
            \n\
            LABEL $$$sub_end\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            ";

    char built_in_fun_strcmp[] = "\n\
            #-----------------------------------------\n\
            #pub fn ifj.strcmp(𝑠1: []u8, s2: []u8) i32\n\
            LABEL $$ifj$strcmp\n\
            PUSHFRAME\n\
            \n\
            STRLEN GF@?tempSRC1 LF@s1\n\
            STRLEN GF@?tempSRC2 LF@s2\n\
            LT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n\
            JUMPIFEQ $$$strcmp-1 GF@?tempDEST bool@true\n\
            GT GF@?tempDEST GF@?tempSRC1 GF?tempSRC2\n\
            JUMPIFEQ $$$strcmp1 GF@?tempDEST bool@true\n\
            \n\
            DEFVAR LF@len\n\
            MOVE LF@len GF@?tempSRC1\n\
            DEFVAR LF@i\n\
            MOVE LF@i int@0\n\
            LABEL $$$cmp_while\n\
            \n\
                JUMPIFEQ $$$strcmp0 LF@i LF@len\n\
                GETCHAR GF@?tempSRC1 LF@s1 LF@i\n\
                GETCHAR GF@?tempSRC2 LF@s2 LF@i\n\
                \n\
                LT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n\
                JUMPIFEQ $$$strcmp-1 GF@?tempDEST bool@true\n\
                \n\
                GT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n\
                JUMPIFEQ $$$strcmp2 GF@?tempDEST bool@true\n\
                \n\
                ADD LF@i LF@i int@1\n\
            \n\
            JUMP $$$cmp_while\n\
            \n\
            LABEL $$$strcmp-1\n\
                PUSHS int@-1\n\
                JUMP $$$cmp_end\n\
            \n\
            LABEL $$$strcmp1\n\
                PUSHS int@1\n\
                JUMP $$$cmp_end\n\
            \n\
            LABEL $$$strcmp0\n\
                PUSHS int@0\n\
                JUMP $$$cmp_end\n\
            \n\
            LABEL $$$cmp_end\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            ";

   char built_in_fun_ord[] = "\n\
            #-----------------------------------\n\
            #pub fn ifj.ord(𝑠: []u8, i: i32) i32\n\
            LABEL $$ifj$ord\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@x\n\
            STRLEN LF@$x LF@$s\n\
            DEFVAR LF@$z\n\
            \n\
            JUMPIFEQ $$$ord1 LF@$x int@0\n\
            DEFVAR LF@$y\n\
            GT LF@$y LF@$i LF@$x\n\
            JUMPIFEQ $$ord1 LF@$y bool@true\n\
                STRI2INT LF@$z LF@$s LF@$i\n\
                JUMP $$$ord2\n\
            \n\
            LABEL $$$ord1\n\
                MOVE LF@$z int@0\n\
            \n\
            LABEL $$$ord2\n\
            \n\
            PUSHS LF@$z\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    printf("%s", built_in_fun_substring);
    printf("%s", built_in_fun_strcmp);
    printf("%s", built_in_fun_ord);
}

/*** Konec souboru built_in_functions.c ***/
