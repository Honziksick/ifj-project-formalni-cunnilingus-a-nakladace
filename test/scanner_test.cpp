/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner_test.cpp                                          *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                   Krejčí David       <xhyzapa00>                            *
 *                                                                             *
 * Datum:            9.10.2024                                                 *
 * Poslední změna:   11.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
*******************************************************************************/
/**
 * @file scanner_test.cpp
 * @author Farkašovský Lukáš \<xfarkal00>
 *
 * @brief Testování podpůrné knihovny scanner.c.
 * @details Tento soubor obsahuje testy k ověření funkčnosti dané knihovny.
*/

// Potřebné knihovny k testování
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdio.h>
#include <cstdio>
#include <cstring>

extern "C" {
#include "scanner.h"
#include "symtable.h"
}

using namespace testing;
using namespace std;
using namespace internal;


/**
 * @brief Makro pro vytvoření stringu
 */
#define MAKE_STRING(id, str)                                        \
        DString* id = string_init();                                \
        do{                                                         \
            ASSERT_NE(id, nullptr);                                     \
            const char *tmp_str = str;                                  \
            for (size_t i = 0; i < strlen(tmp_str); i++) {              \
                ASSERT_EQ(string_append_char(id, tmp_str[i]), STRING_SUCCESS); \
            }                                                           \
        }while(0);

#define TEST_HASH_FUNCTION(keyword, expected_hash) \
    MAKE_STRING(str_##keyword, #keyword); \
    EXPECT_EQ(symtable_hashFunction(str_##keyword), expected_hash); \
    fprintf(stderr, "Hash of %s is index %zu\n", #keyword, symtable_hashFunction(str_##keyword)%KEYWORD_COUNT); \
    string_free(str_##keyword);


/**
 * @brief Funkce na testování typu tokenů
 */
void testTokenType(TokenType expected_type){
    Token token = scanner_FSM();
    EXPECT_EQ(token.type, expected_type);
    if(token.value != NULL) {
        string_free(token.value);
    }
}

/**
 * @brief Test hasovací funkce pro klíčová slova
 */
TEST(Hash, Hash_Function) {
    TEST_HASH_FUNCTION(const, KEYWORD_CONST_HASH);
    TEST_HASH_FUNCTION(var, KEYWORD_VAR_HASH);
    TEST_HASH_FUNCTION(i32, KEYWORD_I32_HASH);
    TEST_HASH_FUNCTION(f64, KEYWORD_F64_HASH);
    TEST_HASH_FUNCTION(pub, KEYWORD_PUB_HASH);
    TEST_HASH_FUNCTION(fn, KEYWORD_FN_HASH);
    TEST_HASH_FUNCTION(void, KEYWORD_VOID_HASH);
    TEST_HASH_FUNCTION(return, KEYWORD_RETURN_HASH);
    TEST_HASH_FUNCTION(null, KEYWORD_NULL_HASH);
    TEST_HASH_FUNCTION(if, KEYWORD_IF_HASH);
    TEST_HASH_FUNCTION(else, KEYWORD_ELSE_HASH);
    TEST_HASH_FUNCTION(while, KEYWORD_WHILE_HASH);

    MAKE_STRING(import, "@import");
    EXPECT_EQ(symtable_hashFunction(import), KEYWORD_IMPORT_HASH);
    string_free(import);

    MAKE_STRING(underscore, "_");
    EXPECT_EQ(symtable_hashFunction(underscore), KEYWORD_UNDERSCORE_HASH);
    string_free(underscore);

    MAKE_STRING(i32N, "?i32");
    EXPECT_EQ(symtable_hashFunction(i32N), KEYWORD_QI32_HASH);
    string_free(i32N);

    MAKE_STRING(f64N, "?f64");
    EXPECT_EQ(symtable_hashFunction(f64N), KEYWORD_QF64_HASH);
    string_free(f64N);

    MAKE_STRING(u8N, "?[]u8");
    EXPECT_EQ(symtable_hashFunction(u8N), KEYWORD_QU8_HASH);
    string_free(u8N);

    MAKE_STRING(u8, "[]u8");
    EXPECT_EQ(symtable_hashFunction(u8), KEYWORD_U8_HASH);
    string_free(u8);


}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro písmena.
 */
TEST(Identity, Letter) {
    // Krajní hodnoty
    EXPECT_EQ(scanner_charIdentity('A'), LETTER);
    EXPECT_EQ(scanner_charIdentity('Z'), LETTER);
    EXPECT_EQ(scanner_charIdentity('a'), LETTER);
    EXPECT_EQ(scanner_charIdentity('z'), LETTER);
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro čísla.
 */
TEST(Identity, Number) {
    for(int i = 0; i < 10; i++){

        // Převedení čísla i do char numC
        char numC =(char) i + '0';
        EXPECT_EQ(scanner_charIdentity(numC), NUMBER);
    }
}

/**
 * @brief Testuje funkci `scanner_charIdentity` pro operátory.
 */
TEST(Identity, White) {
    EXPECT_EQ(scanner_charIdentity(' '), WHITE);
    EXPECT_EQ(scanner_charIdentity('\t'), WHITE);
    EXPECT_EQ(scanner_charIdentity('\n'), WHITE);
}

TEST(Identity, Error) {
    // Znak ~ není v IFJ24 definován
    EXPECT_EXIT(scanner_charIdentity('~'), ExitedWithCode(1), "");
    // Nedovolená hodnota
    EXPECT_EXIT(scanner_charIdentity(16), ExitedWithCode(1), "");
    EXPECT_EXIT(scanner_charIdentity(200), ExitedWithCode(1), "");
}

TEST(Identity, eof) {
    EXPECT_EQ(scanner_charIdentity(EOF), C_EOF);
}


/**
 * @brief Testuje funkci `scanner_getNextChar` pro náhodné pole.
 */
TEST(Get, Get_Next_Char) {
    char c[] = "Hello";
    
    for(int i = 0; i < 5; i++){
        // Simulace stdin pomocí ungetc
        ungetc(c[i], stdin);
        int c2 = scanner_getNextChar();
        EXPECT_EQ(c2, c[i]);
    }
}



/**
 * @brief Testuje funkci `scanner_FSM` pro identifikátor hello.
 */
TEST(FSM, FSM_Identifier){
    char c[] = "hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    //for(size_t i = 0; i < strlen(c); i++){printf("%c\n", fgetc(stdin));} //kontrola, co se vypíše
    
    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_IDENTIFIER);
    string_free(state.value);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor =hello.
 */
TEST(FSM, FSM_Identifier_ERROR_1){
    char c[] = "=hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor .hello.
 */
TEST(FSM, FSM_Identifier_ERROR_2){
    char c[] = ".hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    CaptureStderr();
    error_printMessage(ERROR_LEXICAL);
    string GotErrMsg = GetCapturedStderr();
    string ExpectedErrMsg = "(Error 1) Lexical error: invalid structure of the " \
                            "current lexeme.\n";

    state = scanner_FSM();
    EXPECT_EQ(GotErrMsg, ExpectedErrMsg);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor 1hello.
 */
TEST(FSM, FSM_Identifier_ERROR_3){
    char c[] = "1hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;


    ASSERT_EXIT(state = scanner_FSM(), ExitedWithCode(1), "");
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor 'hello.'.
 */
TEST(FSM, FSM_Identifier_ERROR_4){
    char c[] = "hello.";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    // Načteme hello
    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_IDENTIFIER);
    ASSERT_NE(state.value, nullptr);
    ASSERT_EQ(string_compare_const_str(state.value, "hello"), STRING_EQUAL);
    string_free(state.value);


    // Načteme .
    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_PERIOD);

}

/**
 * @brief Testuje funkci `scanner_FSM` pro int číslo.
 */
TEST(FSM, FSM_Number_INT){
    char c[] = "42";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_INT);
    string_free(state.value);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné int číslo.
 */
TEST(FSM, FSM_Number_INT_ERROR_1){
    char c[] = "42c";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    ASSERT_EXIT(state = scanner_FSM(), ExitedWithCode(1), "");
}

/**
 * @brief Testuje funkci `scanner_FSM` pro float číslo.
 */
TEST(FSM, FSM_Number_FLOAT){
    char c[] = "42.42";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_FLOAT);
    string_free(state.value);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné float číslo.
 */
TEST(FSM, FSM_Number_FLOAT_ERROR_1){
    char c[] = ".42";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_PERIOD);
    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_INT);
    string_free(state.value);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné float číslo.
 */
TEST(FSM, FSM_Number_FLOAT_ERROR_2){
    char c[] = "42.";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    ASSERT_EXIT(state = scanner_FSM(), ExitedWithCode(1), "");
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybné float číslo.
 */
TEST(FSM, FSM_Number_FLOAT_ERROR_3){
    char c[] = "42c.0";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    ASSERT_EXIT(state = scanner_FSM(), ExitedWithCode(1), "");
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor =.
 */
TEST(FSM, FSM_OPERATOR_1){
    char c[] = "=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_EQUALITY_SIGN);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor *.
 */
TEST(FSM, FSM_OPERATOR_2){
    char c[] = "*";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_ASTERISK);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor /.
 */
TEST(FSM, FSM_OPERATOR_3){
    char c[] = "/";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_SLASH);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor +.
 */
TEST(FSM, FSM_OPERATOR_4){
    char c[] = "+";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_PLUS);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor -.
 */
TEST(FSM, FSM_OPERATOR_5){
    char c[] = "-";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_MINUS);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor ==.
 */
TEST(FSM, FSM_OPERATOR_6){
    char c[] = "==";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_EQUAL_TO);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor !=.
 */
TEST(FSM, FSM_OPERATOR_7){
    char c[] = "!=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_NOT_EQUAL_TO);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor <=.
 */
TEST(FSM, FSM_OPERATOR_8){
    char c[] = "<=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_LESS_EQUAL_THAN);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor >=.
 */
TEST(FSM, FSM_OPERATOR_9){
    char c[] = ">=";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_GREATER_EQUAL_THAN);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor >.
 */
TEST(FSM, FSM_PERATOR_10){
    char c[] = ">";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_GREATER_THAN);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro operátor <.
 */
TEST(FSM, FSM_OPERATOR_11){
    char c[] = "<";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_LESS_THAN);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný operátor !.
 */
TEST(FSM, FSM_OPERATOR_ERROR_1){
    char c[] = "!";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    ASSERT_EXIT(state = scanner_FSM(), ExitedWithCode(1), "");
}

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný operátor ==!.
 */
TEST(FSM, FSM_OPERATOR_ERROR_2){
    char c[] = "==!";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_EQUAL_TO);
    ASSERT_EXIT(state = scanner_FSM(), ExitedWithCode(1), "");
}


/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro krátký korektní vstupní program
 */
TEST(Lex, Simple_Correct){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/example1.zig", "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Program 1: Vypocet faktorialu (iterativne)
        // Hlavni telo programu

        // const ifj = @import("ifj24.zig");
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,
        
        // ifj.write("Zadejte cislo pro vypocet faktorialu\n");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // const a = ifj.readi32();
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // if (a) |val| {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_VERTICAL_BAR, TOKEN_IDENTIFIER, TOKEN_VERTICAL_BAR, TOKEN_LEFT_CURLY_BRACKET,
        
        // if (val < 0) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_LESS_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,
        
        // ifj.write("Faktorial ");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(val);
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(" nelze spocitat\n");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,
        
        // var d: f64 = ifj.i2f(val);
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_f64, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // var vysl: f64 = 1.0;
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_f64, TOKEN_EQUALITY_SIGN, TOKEN_FLOAT, TOKEN_SEMICOLON,
        
        // while (d > 0) {
        TOKEN_K_while, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_GREATER_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,
        
        // vysl = vysl * d;
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_ASTERISK, TOKEN_IDENTIFIER, TOKEN_SEMICOLON,
        
        // d = d - 1.0;
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_MINUS, TOKEN_FLOAT, TOKEN_SEMICOLON,
        
        // }
        TOKEN_RIGHT_CURLY_BRACKET,
        
        // ifj.write("Vysledek: ");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(vysl);
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(" = ");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // const vysl_i32 = ifj.f2i(vysl);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(vysl_i32);
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write("\n");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // }
        TOKEN_RIGHT_CURLY_BRACKET, 
        
        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET, // a == null
        
        // ifj.write("Faktorial pro null nelze spocitat\n");
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Uložíme si pozici vstupu
        long int current_pos = ftell(stdin);
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Potomek zavolá testTokenType a ukončí se
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Rodičovský proces čeká na potomka
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Potomek skončil s chybou
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Vypíšeme zbytek vstupu
                fseek(stdin, current_pos, SEEK_SET);
                char buffer[256];
                cerr << "Remaining input: ";
                while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    cerr << buffer;
                }
                cerr << endl;

                stdin = stdin_backup;
                fclose(f);
                
                FAIL();
            }
        }
    }

    stdin = stdin_backup;
    fclose(f);
}




/*** Konec souboru scanner_test.cpp ***/