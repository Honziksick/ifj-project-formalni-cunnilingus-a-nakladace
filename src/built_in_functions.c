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
                LT GF@$?tempDEST LF@$i int@0\n\
                JUMPIFEQ $$$null GF@$tempDEST bool@true\n\
                \n\
                LT GF@$?tempDEST LF@$j int@0\n\
                JUMPIFEQ $$$null GF@$tempDEST bool@true\n\
                \n\
                GT GF@$?tempDEST LF@$i LF@$j\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
                \n\
                STRLEN GF@?tempSRC1 LF@$s\n\
                \n\
                GT GF@$?tempDEST LF@$i GF@?tempSRC1\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
                EQ GF@$?tempDEST LF@$i GF@?tempSRC1\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
                \n\
                GT GF@$?tempDEST LF@$j GF@?tempSRC1\n\
                JUMPIFEQ $$$null GF@$?tempDEST bool@true\n\
            \n\
            LABEL $$$while\n\
            \n\
                JUMPIFEQ $$$sub_end TF@i TF@j\n\
                \n\
                GETCHAR GF@?tempSRC1 LF@$s LF@$i\n\
                CONCAT GF@?tempDEST GF@?tempDEST GF@?tempSRC1\n\
                ADD LF@$i LF@$i int@1\n\
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

   /* char built_in_fun_strcmp[] = "\n\
            #-----------------------------------------\n\
            #pub fn ifj.strcmp(𝑠1: []u8, s2: []u8) i32\n\
            LABEL $$ifj$strcmp\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            DEFVAR LF@$y\n\
            DEFVAR LF@$n\n\
            MOVE LF@$n int@0\n\
            DEFVAR LF@$z\n\
            \n\
            DEFVAR LF@$len1\n\
            STRLEN LF@$len1 LF@$s1\n\
            DEFVAR LF@$len2\n\
            STRLEN LF@$len2 LF@$s2\n\
            \n\
            DEFVAR LF@$tf1\n\
            DEFVAR LF@$tf2\n\
            \n\
            JUMPIFEQ $$$cmp_while2 LF@$len1 LF@$len2\n\
            \n\
            LABEL $$$cmp_while1\n\
            \n\
                GETCHAR LF@$x LF@$s1 LF@$n\n\
                GETCHAR LF@$y LF@$s2 LF@$n\n\
                \n\
                LT LF@$tf1 LF@$x LF@$y\n\
                JUMPIFEQ $$$strcmp1 LF@$tf1 bool@true\n\
                \n\
                GT LF@$tf2 LF@$x LF@$y\n\
                JUMPIFEQ $$$strcmp2 LF@$tf2 bool@true\n\
                \n\
                ADD LF@$n LF@$n int@1\n\
            \n\
            JUMP $$$cmp_while1\n\
            \n\
            LABEL $$$cmp_while2\n\
            \n\
                GETCHAR LF@$x LF@$s1 LF@$n\n\
                GETCHAR LF@$y LF@$s2 LF@$n\n\
                \n\
                LT LF@$tf1 LF@$x LF@$y\n\
                JUMPIFEQ $$$strcmp1 LF@$tf1 bool@true\n\
                \n\
                GT LF@$tf2 LF@$x LF@$y\n\
                JUMPIFEQ $$$strcmp2 LF@$tf2 bool@true\n\
                \n\
                ADD LF@$n LF@$n int@1\n\
                \n\
                JUMPIFEQ $$$strcmp3 LF@$n LF@$len1\n\
            \n\
            JUMP $$$cmp_while2\n\
            \n\
            LABEL $$$strcmp1\n\
                MOVE LF@$z int@-1\n\
                JUMP $$$cmp_end\n\
            \n\
            LABEL $$$strcmp2\n\
                MOVE LF@$z int@1\n\
                JUMP $$$cmp_end\n\
            \n\
            LABEL $$$strcmp3\n\
                MOVE LF@$z int@0\n\
                JUMP $$$cmp_end\n\
            \n\
            LABEL $$$cmp_end\n\
            \n\
            PUSHS LF@$z\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_ord[] = "\n\
            #-----------------------------------\n\
            #pub fn ifj.ord(𝑠: []u8, i: i32) i32\n\
            LABEL $$ifj$ord\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
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

    char built_in_fun_chr[] = "\n\
            #---------------------------\n\
            #pub fn ifj.chr(i: i32) []u8\n\
            LABEL $$ifj$chr\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            INT2CHAR LF@$x LF@$i\n\
            PUSHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";*/
    printf("%s", built_in_fun_substring);
    /*printf("%s", built_in_fun_strcmp);
    printf("%s", built_in_fun_ord);
    printf("%s", built_in_fun_chr);*/
}

/*** Konec souboru built_in_functions.c ***/
