/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           built_in_functions.c                                      *
 * Autor:            Hýža Pavel         <xhyzapa00>                            *
 *                                                                             *
 * Datum:            22.11.2024                                                *
 * Poslední změna:   22.11.2024                                                *
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

//Ještě chybí dodělat 4 funkce
void built_in_functions() {
    char functions[] = "\n\
        #DONE_COUNT_9/13\n\
        #readstr-DONE\n\
        #readint-DONE\n\
        #readfloat-DONE\n\
        #write-DONE\n\
        #i2f-DONE\n\
        #f2i-DONE\n\
        #string-DONE\n\
        #length-DONE\n\
        #concat-DONE\n\
        #substring\n\
        #strcmp\n\
        #ord\n\
        #chr\n\
        \n\
        \n\
        \n\
        #--------------------------\n\
        #pub fn ifj.readstr() ?[]u8\n\
        LABEL readstr\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        READ LF@$x string\n\
        PUSHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #-------------------------\n\
        #pub fn ifj.readi32() ?i32\n\
        LABEL readi32\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        READ LF@$x int\n\
        PUSHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #-------------------------\n\
        #pub fn ifj.readf64() ?f64\n\
        LABEL readf64\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        READ LF@$x float\n\
        PUSHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #---------------------------\n\
        #pub fn ifj.write(term) void\n\
        LABEL write\n\
        \n\
        PUSHFRAME\n\
        \n\
        WRITE LF@$term\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #-----------------------------\n\
        #pub fn ifj.i2f(term: i32) f64\n\
        LABEL i2f\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        INT2FLOAT LF@$x LF@$term\n\
        PUCHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #-----------------------------\n\
        #pub fn ifj.f2i(term: f64) i32\n\
        LABEL f2i\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        FLOAT2INT LF@$x LF@$term\n\
        PUCHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #----------------------------\n\
        #pub fn ifj.string(term) []u8\n\
        LABEL string\n\
        \n\
        PUSHFRAME\n\
        \n\
        PUCHS LF@$term\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #------------------------------\n\
        #pub fn ifj.length(s: []u8) i32\n\
        LABEL length\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        STRLEN LF@$x LF@$s\n\
        PUCHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #------------------------------------------\n\
        #pub fn ifj.concat(𝑠1: []u8, 𝑠2: []u8) []u8\n\
        LABEL concat\n\
        \n\
        PUSHFRAME\n\
        \n\
        DEFVAR LF@$x\n\
        CONCAT LF@$x LF@$s1 LF@$s2\n\
        PUCHS LF@$x\n\
        \n\
        POPFRAME\n\
        RETURN\n\
        \n\
        #---------------------------------------------------\n\
        #pub fn ifj.substring(𝑠: []u8, i: i32, j: i32) ?[]u8\n\
        #-----------------------------------------\n\
        #pub fn ifj.strcmp(𝑠1: []u8, s2: []u8) i32\n\
        #-----------------------------------\n\
        #pub fn ifj.ord(𝑠: []u8, i: i32) i32\n\
        #---------------------------\n\
        #pub fn ifj.chr(i: i32) []u8\n\
    ";
    printf("%s", functions);
}