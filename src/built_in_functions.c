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
    char built_in_fun_readstr[] = "\n\
        #--------------------------\n\
        #pub fn ifj.readstr() ?[]u8\n\
            LABEL $$ifj$readstr\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            READ LF@$x string\n\
            PUSHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_readi32[] = "\n\
        #-------------------------\n\
        #pub fn ifj.readi32() ?i32\n\
            LABEL $$ifj$readi32\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            READ LF@$x int\n\
            PUSHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_readf64[] = "\n\
        #-------------------------\n\
        #pub fn ifj.readf64() ?f64\n\
            LABEL $$ifj$readf64\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            READ LF@$x float\n\
            PUSHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_write[] = "\n\
        #---------------------------\n\
        #pub fn ifj.write(term) void\n\
            LABEL $$ifj$write\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            TYPE LF@$x LF@$term\n\
            \n\
            JUMPIFNEQ $$$write1 LF@$x nil\n\
            \n\
                WRITE string@null\n\
                JUMP $$$write2\n\
            \n\
            LABEL $$$write1\n\
                WRITE LF@$x\n\
            \n\
            LABEL $$$write2\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_i2f[] = "\n\
            #-----------------------------\n\
            #pub fn ifj.i2f(term: i32) f64\n\
            LABEL $$ifj$i2f\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            INT2FLOAT LF@$x LF@$term\n\
            PUCHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_f2i[] = "\n\
            #-----------------------------\n\
            #pub fn ifj.f2i(term: f64) i32\n\
            LABEL $$ifj$f2i\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            FLOAT2INT LF@$x LF@$term\n\
            PUCHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_string[] = "\n\
            #----------------------------\n\
            #pub fn ifj.string(term) []u8\n\
            LABEL $$ifj$string\n\
            PUSHFRAME\n\
            \n\
            PUCHS LF@$term\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_length[] = "\n\
            #------------------------------\n\
            #pub fn ifj.length(s: []u8) i32\n\
            LABEL $$ifj$length\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            STRLEN LF@$x LF@$s\n\
            PUCHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_concat[] = "\n\
            #------------------------------------------\n\
            #pub fn ifj.concat(𝑠1: []u8, 𝑠2: []u8) []u8\n\
            LABEL $$ifj$concat\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$x\n\
            CONCAT LF@$x LF@$s1 LF@$s2\n\
            PUCHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_substring[] = "\n\
            #---------------------------------------------------\n\
            #pub fn ifj.substring(𝑠: []u8, i: i32, j: i32) ?[]u8\n\
            LABEL $$ifj$strcmp\n\
            PUSHFRAME\n\
            \n\
            DEFVAR LF@$tf\n\
            DEFVAR LF@$x\n\
            DEFVAR LF@$y\n\
            DEFVAR LF@$z\n\
            DEFVAR LF@$len\n\
            \n\
                LT LF@$tf LF@$i int@0\n\
                JUMPIFEQ $$$sub_err LF@$tf bool@true\n\
                \n\
                LT LF@$tf LF@$i int@0\n\
                JUMPIFEQ $$$sub_err LF@$tf bool@true\n\
                \n\
                GT LF@$tf LF@$i LF@$j\n\
                JUMPIFEQ $$$sub_err LF@$tf bool@true\n\
                \n\
                EQ LF@$tf LF@$i LF@$j\n\
                JUMPIFEQ $$$sub_end LF@$tf bool@true\n\
                \n\
                \n\
                STRLEN LF@$len LF@$s\n\
                \n\
                GT LF@$tf LF@$i LF@$len\n\
                JUMPIFEQ $$$sub_err LF@$tf bool@true\n\
                EQ LF@$tf LF@$i LF@$len\n\
                JUMPIFEQ $$$sub_err LF@$tf bool@true\n\
                \n\
                GT LF@$tf LF@$j LF@$len\n\
                JUMPIFEQ $$$sub_err LF@$tf bool@true\n\
            \n\
            GETCHAR LF@$x LF@$s LF@$i\n\
            \n\
            LABEL $$$sub_while\n\
            \n\
                ADD LF@$i LF@$i int@1\n\
                \n\
                LT LF@$tf LF@$i LF@$j\n\
                JUMPIFNEQ $$$sub_end LF@$tf bool@true\n\
                \n\
                    GETCHAR LF@$y LF@$s LF@$i\n\
                    CONCAT LF@$x LF@$x LF@$y\n\
            \n\
            JUMP $$$sub_while\n\
            \n\
            LABEL $$$sub_err\n\
                MOVE LF@$x nil@nil\n\
            \n\
            LABEL $$$sub_end\n\
            \n\
            PUCHS LF@$x\n\
            \n\
            POPFRAME\n\
            RETURN\n\
            \n\
            \n\
            \n\
            ";

    char built_in_fun_strcmp[] = "\n\
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
            PUCHS LF@$z\n\
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
            JUMPIFEQ LF@$y bool@true\n\
                STRI2INT LF@$z LF@$s LF@$i\n\
                JUMP $$$ord2\n\
            \n\
            LABEL $$$ord1\n\
                MOVE LF@$z int@0\n\
            \n\
            LABEL $$$ord2\n\
            \n\
            PUCHS LF@$z\n\
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
            ";
    printf("%s", built_in_fun_readstr);
    printf("%s", built_in_fun_readi32);
    printf("%s", built_in_fun_readf64);
    printf("%s", built_in_fun_write);
    printf("%s", built_in_fun_i2f);
    printf("%s", built_in_fun_f2i);
    printf("%s", built_in_fun_string);
    printf("%s", built_in_fun_length);
    printf("%s", built_in_fun_concat);
    printf("%s", built_in_fun_substring);
    printf("%s", built_in_fun_strcmp);
    printf("%s", built_in_fun_ord);
    printf("%s", built_in_fun_chr);
}

/*** Konec souboru built_in_functions.c ***/
