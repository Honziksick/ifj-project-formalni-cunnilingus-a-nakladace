/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           scanner_test.cpp                                          *
 * Autor:            Farkašovský Lukáš  <xfarkal00>                            *
 *                   Krejčí David       <xhyzapa00>                            *
 *                                                                             *
 * Datum:            9.10.2024                                                 *
 * Poslední změna:   22.11.2024                                                *
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

#include <stdio.h>
#include <cstdio>
#include <cstring>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ifj24_compiler_test_utils.h"

#define TEST_HASH_FUNCTION(keyword, expected_hash) \
    MAKE_STRING(str_##keyword, #keyword); \
    EXPECT_EQ(symtable_hashFunction(str_##keyword), expected_hash); \
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
    if(token.type != expected_type){
        exit(2);
    }
}

/**
 * @brief Funkce na testování typu tokenů
 */
void testTokenString(const char* expected_string){
    Token token = scanner_FSM();
    EXPECT_EQ(token.type, TOKEN_STRING);
    if(token.value != NULL) {
        if(string_compare_const_str(token.value, expected_string) != STRING_EQUAL){
            char *actual_string = string_toConstChar(token.value);

            // Find the first mismatch position
            size_t mismatch_index = 0;
            while (expected_string[mismatch_index] != '\0' && actual_string[mismatch_index] != '\0' &&
                expected_string[mismatch_index] == actual_string[mismatch_index]) {
                mismatch_index++;
            }

            cerr << "Strings differ at position " << mismatch_index << ":\n";
            cerr << "Expected:\n" << expected_string << endl;
            cerr << "Got:\n" << actual_string << endl;
            cerr << "Expected character at position: '" << expected_string[mismatch_index] << "'\n";
            cerr << "Got character at position:      '" << actual_string[mismatch_index] << "'\n\n";

            // Clean up and exit
            string_free(token.value);
            free(actual_string);
            exit(2);
        }
        string_free(token.value);
    }else{
        exit(2);
    }
    if(token.type != TOKEN_STRING){
        exit(2);
    }
}

/**
 * @brief Test hasovací funkce pro klíčová slova
 */
/*TEST(Hash, Hash_Function) {
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


}*/

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
    EXPECT_EQ(scanner_charIdentity(' '), WHITESPACE);
    EXPECT_EQ(scanner_charIdentity('\t'), WHITESPACE);
    EXPECT_EQ(scanner_charIdentity('\n'), WHITESPACE);
}

TEST(Identity, Not_In_Language) {
    // Znak ~ není v IFJ24 definován
    EXPECT_EQ(scanner_charIdentity('~'), NOT_IN_LANGUAGE);
    EXPECT_EQ(scanner_charIdentity('#'), NOT_IN_LANGUAGE);
    EXPECT_EQ(scanner_charIdentity(200), NOT_IN_LANGUAGE);
}

TEST(Identity, eof) {
    EXPECT_EQ(scanner_charIdentity(EOF), CHAR_EOF);
}

TEST(Identity, error) {
    // Unprintable znaky v ASCII nebo znaky mimo základní tabulku
    scanner_charIdentity(0);
    bool result = Parser_errorWatcher(IS_PARSING_ERROR);
    EXPECT_TRUE(result);
    Parser_errorWatcher(RESET_ERROR_FLAGS);

    scanner_charIdentity(16);
    result = Parser_errorWatcher(IS_PARSING_ERROR);
    EXPECT_TRUE(result);
    Parser_errorWatcher(RESET_ERROR_FLAGS);

    scanner_charIdentity(17);
    result = Parser_errorWatcher(IS_PARSING_ERROR);
    EXPECT_TRUE(result);
    Parser_errorWatcher(RESET_ERROR_FLAGS);
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
    char c[] = "hello ";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    //for(size_t i = 0; i < strlen(c); i++){printf("%c\n", fgetc(stdin));} //kontrola, co se vypíše
    
    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_IDENTIFIER);
    string_free(state.value);
}

/** TOTO JE BLBĚ NAPSANÉ
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor =hello.
 */
/*TEST(FSM, FSM_Identifier_ERROR_1){
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
}*/

/** TOTO JE BLBĚ NAPSANÉ
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor .hello.
 */
/*TEST(FSM, FSM_Identifier_ERROR_2){
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
}*/

/**
 * @brief Testuje funkci `scanner_FSM` pro chybný identifikátor 1hello.
 */
TEST(FSM, FSM_Identifier_ERROR_3){
    char c[] = "1hello";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_UNINITIALIZED);
    ASSERT_EQ(state.value, nullptr);
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

//Muj test
TEST(FSM, FSM_Number_INT_One_Number){
    char c[] = "4 ";
    Token state; 

    FILE* f = fmemopen(c, strlen(c), "r");
    ASSERT_NE(f, nullptr);
    stdin = f;

    state = scanner_FSM();
    EXPECT_EQ(state.type, TOKEN_INT);
    string_free(state.value);
}

/**
 * @brief Testuje funkci `scanner_FSM` pro int číslo.
 */
TEST(FSM, FSM_Number_INT){
    char c[] = "42 ";
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

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_UNINITIALIZED);
    ASSERT_EQ(state.value, nullptr);
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
    ASSERT_EQ(state.value, nullptr);
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

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_UNINITIALIZED);
    ASSERT_EQ(state.value, nullptr);
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

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_UNINITIALIZED);
    ASSERT_EQ(state.value, nullptr);
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

    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_UNINITIALIZED);
    ASSERT_EQ(state.value, nullptr);
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
    EXPECT_EQ(state.type, TOKEN_EQUAL_TO);
    ASSERT_EQ(state.value, nullptr);
    state = scanner_FSM();
    ASSERT_EQ(state.type, TOKEN_UNINITIALIZED);
    ASSERT_EQ(state.value, nullptr);
}

/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro korektní vstupní program hello.zig
 */
TEST(Lex, Hello) {
    string path = exam_path + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // const ifj = @import("ifj24.zig");
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,

        // const y: i32 = 24;
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_EQUALITY_SIGN, TOKEN_INT, TOKEN_SEMICOLON,

        // ifj.write("Hello from IFJ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(y);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Save the current position in the input file
        long int current_pos = ftell(stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Child process calls testTokenType and exits
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Parent process waits for the child
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Child process exited with an error
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Print the remaining input
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


/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro korektní vstupní program fun.zig
 */
TEST(Lex, Fun) {
    string path = exam_path + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // const ifj = @import("ifj24.zig");
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // pub fn f (x : i32) i32
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_i32, 

        // {
        TOKEN_LEFT_CURLY_BRACKET,

        // if (x < 10) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_LESS_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,

        // return x - 1;
        TOKEN_K_return, TOKEN_IDENTIFIER, TOKEN_MINUS, TOKEN_INT, TOKEN_SEMICOLON,

        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,

        // const y = x - 1;
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_MINUS, TOKEN_INT, TOKEN_SEMICOLON,

        // ifj.write("calling g with ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(y);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // const res = g(y);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // return res;
        TOKEN_K_return, TOKEN_IDENTIFIER, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // pub fn g(x: i32) i32 {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_i32, TOKEN_LEFT_CURLY_BRACKET,

        // if (x > 0) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_GREATER_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,

        // ifj.write("calling f with ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(x);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // const y = f(x);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // return y;
        TOKEN_K_return, TOKEN_IDENTIFIER, TOKEN_SEMICOLON,

        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,

        // return 200;
        TOKEN_K_return, TOKEN_INT, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,

        // const res = g(10);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("res: ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(res);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Save the current position in the input file
        long int current_pos = ftell(stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Child process calls testTokenType and exits
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Parent process waits for the child
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Child process exited with an error
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Print the remaining input
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


/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro korektní vstupní program example1.zig
 */
TEST(Lex, Example1){
    string path = exam_path + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Program 1: Vypocet faktorialu (iterativne)
        // Hlavni telo programu

        // const ifj = @import("ifj24.zig");
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,
        
        // ifj.write("Zadejte cislo pro vypocet faktorialu\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // const a = ifj.readi32();
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // if (a) |val| {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_VERTICAL_BAR, TOKEN_IDENTIFIER, TOKEN_VERTICAL_BAR, TOKEN_LEFT_CURLY_BRACKET,
        
        // if (val < 0) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_LESS_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,
        
        // ifj.write("Faktorial ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(val);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(" nelze spocitat\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,
        
        // var d: f64 = ifj.i2f(val);
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_f64, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
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
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(vysl);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(" = ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // const vysl_i32 = ifj.f2i(vysl);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write(vysl_i32);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // ifj.write("\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // }
        TOKEN_RIGHT_CURLY_BRACKET, 
        
        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET, // a == null
        
        // ifj.write("Faktorial pro null nelze spocitat\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
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


/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro korektní vstupní program example2.zig
 */
TEST(Lex, Example2) {
    string path = exam_path + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Program 2: Vypocet faktorialu (rekurzivne)
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,

        // ifj.write("Zadejte cislo pro vypocet faktorialu: ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // const inp = ifj.readi32();
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // if (inp) |INP| {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_VERTICAL_BAR, TOKEN_IDENTIFIER, TOKEN_VERTICAL_BAR, TOKEN_LEFT_CURLY_BRACKET,

        // if (INP < 0) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_LESS_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,

        // ifj.write("Faktorial nelze spocitat!\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,

        // const vysl = factorial(INP);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("Vysledek: ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(vysl);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,

        // ifj.write("Chyba pri nacitani celeho cisla!\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // pub fn decrement(n: i32, m: i32) i32 {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_COMMA, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_i32, TOKEN_LEFT_CURLY_BRACKET,

        // return n - m;
        TOKEN_K_return, TOKEN_IDENTIFIER, TOKEN_MINUS, TOKEN_IDENTIFIER, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // pub fn factorial(n: i32) i32 {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_i32, TOKEN_LEFT_CURLY_BRACKET,

        // var result: i32 = 0-1;
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_i32, TOKEN_EQUALITY_SIGN, TOKEN_INT, TOKEN_MINUS, TOKEN_INT, TOKEN_SEMICOLON,

        // if (n < 2) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_LESS_THAN, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,

        // result = 1;
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_INT, TOKEN_SEMICOLON,

        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,

        // const decremented_n = decrement(n, 1);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COMMA, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // const temp_result = factorial(decremented_n);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // result = n * temp_result;
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_IDENTIFIER, TOKEN_ASTERISK, TOKEN_IDENTIFIER, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // return result;
        TOKEN_K_return, TOKEN_IDENTIFIER, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Save the current position in the input file
        long int current_pos = ftell(stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Child process calls testTokenType and exits
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Parent process waits for the child
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Child process exited with an error
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Print the remaining input
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

/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro korektní vstupní program example3.zig
 */
TEST(Lex, Example3) {
    string path = exam_path + "example3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Program 3: Prace s retezci a vestavenymi funkcemi
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,

        // const str1 = ifj.string("...");
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // var str2 = ifj.string("...");
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // str2 = ifj.concat(str1, str2);
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COMMA, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(str1);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(str2);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write("Zadejte serazenou posloupnost vsech malych pismen a-h, ");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // var newInput = ifj.readstr();
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // var all: []u8 = ifj.string("");
        TOKEN_K_var, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_u8, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // while (newInput) |inpOK| {
        TOKEN_K_while, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_VERTICAL_BAR, TOKEN_IDENTIFIER, TOKEN_VERTICAL_BAR, TOKEN_LEFT_CURLY_BRACKET,

        // const abcdefgh = ifj.string("abcdefgh");
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // const strcmpResult = ifj.strcmp(inpOK, abcdefgh);
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COMMA, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // if (strcmpResult == 0) {
        TOKEN_K_if, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_EQUAL_TO, TOKEN_INT, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET,

        // ifj.write("Spravne zadano!\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(all);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // newInput = null;
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_null, TOKEN_SEMICOLON,

        // } else {
        TOKEN_RIGHT_CURLY_BRACKET, TOKEN_K_else, TOKEN_LEFT_CURLY_BRACKET,

        // ifj.write("Spatne zadana posloupnost, zkuste znovu:\\n");
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // all = ifj.concat(all, inpOK);
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_COMMA, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // newInput = ifj.readstr();
        TOKEN_IDENTIFIER, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Save the current position in the input file
        long int current_pos = ftell(stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Child process calls testTokenType and exits
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Parent process waits for the child
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Child process exited with an error
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Print the remaining input
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



//ZAKOMENTOVÁNO PROTOŽE TO JE TO SAMÉ JAKO DALŠÍ TEST, ale je v tom jinačí systém zpracování stringu, který je špatně
/**
 * @brief Testuje lexikální analyzátor pro vstupní program
 * 
 * @details Testuje výstup lexikálního analyzátoru pro korektní vstupní program multiline.zig
 *//*
TEST(Lex, Multiline){
    FILE* f = fopen("../ifj24_examples/ifj24_programs/multiline.zig", "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Ukazka prace s retezci a vestavenymi funkcemi 
        // const ifj = @import("ifj24.zig");
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,
        
        // const s1 : []u8 = ifj.string( 
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_u8, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS,

        // Multiline string
        TOKEN_STRING,

        // );
        TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(s1);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
    };

    const char* expected_strings[] = {
        "ifj24.zig",
        "Toto \n je \n\n nejaky \n text  // ve viceradkovem retezcovem literalu nelze mit komentar"
    };

    for (unsigned long i = 0, j = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Uložíme si pozici vstupu
        long int current_pos = ftell(stdin);
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Potomek zavolá testTokenType a ukončí se
            if(expected_arr[i] == TOKEN_STRING){
                if(j > sizeof(expected_strings) / sizeof(char*)){
                    cerr << "Error in token " << i << endl;
                    cerr << "Expected strings is too small: " << endl;
                    FAIL();
                }
                testTokenString(expected_strings[j]);
            } else {
                testTokenType(expected_arr[i]);
            }

            exit(0);
        } else {
            // Rodičovský proces čeká na potomka
            int status;
            waitpid(pid, &status, 0);
            if(WEXITSTATUS(status) == 0){
                if(expected_arr[i] == TOKEN_STRING){
                    j++;
                }
                continue;
            }else if (WEXITSTATUS(status) == 2) {
                // Potomek failnul testy, ale ne lex chybou
                ADD_FAILURE();
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;
            }else if (WEXITSTATUS(status) == 1) {
                // Potomek skončil s lex chybou
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
            if(expected_arr[i] == TOKEN_STRING){
                j++;
            }
        }
    }

    stdin = stdin_backup;
    fclose(f);


}*/

// ----- Začátek souboru: lex_test_multi.zig -----
//      //Ukazka prace s retezci a vestavenymi funkcemi 
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const s1 : []u8 = ifj.string( 
//          \\Toto
//          \\je
//          \\nejaky
//          \\text  // ve viceradkovem retezcovem literalu nelze mit komentar
//          ); // ukoncujici uvozovky ovlivnuji implicitni odsazeni vnitrnich radku retezce
//          ifj.write(s1);
//      }
// ----- Konec souboru: lex_test_multi.zig -----
TEST(Lex, lex_test_multi){
    string path = lex_path + "lex_test_multi.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Ukazka prace s retezci a vestavenymi funkcemi 
        // const ifj = @import("ifj24.zig");
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        
        // pub fn main() void {
        TOKEN_K_pub, TOKEN_K_fn, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_K_void, TOKEN_LEFT_CURLY_BRACKET,
        
        // const s1 : []u8 = ifj.string( 
        TOKEN_K_const, TOKEN_IDENTIFIER, TOKEN_COLON, TOKEN_K_u8, TOKEN_EQUALITY_SIGN, TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS,

        // Multiline string
        TOKEN_STRING,

        // );
        TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // ifj.write(s1);
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_IDENTIFIER, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,

        // }
        TOKEN_RIGHT_CURLY_BRACKET,

        // EOF
        TOKEN_EOF
    };

    const char* expected_strings[] = {
        "ifj24.zig",
        "Toto\nje\nnejaky\ntext  // ve viceradkovem retezcovem literalu nelze mit komentar"
    };

    for (unsigned long i = 0, j = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Uložíme si pozici vstupu
        long int current_pos = ftell(stdin);
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Potomek zavolá testTokenType a ukončí se
            if(expected_arr[i] == TOKEN_STRING){
                if(j > sizeof(expected_strings) / sizeof(char*)){
                    cerr << "Error in token " << i << endl;
                    cerr << "Expected strings is too small: " << endl;
                    FAIL();
                }
                testTokenString(expected_strings[j]);
            } else {
                testTokenType(expected_arr[i]);
            }

            exit(0);
        } else {
            // Rodičovský proces čeká na potomka
            int status;
            waitpid(pid, &status, 0);
            if(WEXITSTATUS(status) == 0){
                if(expected_arr[i] == TOKEN_STRING){
                    j++;
                }
                continue;
            }else if (WEXITSTATUS(status) == 2) {
                // Potomek failnul testy, ale ne lex chybou
                ADD_FAILURE();
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;
            }else if (WEXITSTATUS(status) == 1) {
                // Potomek skončil s lex chybou
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
            if(expected_arr[i] == TOKEN_STRING){
                j++;
            }
        }
    }

    stdin = stdin_backup;
    fclose(f);

}

// ----- Začátek souboru: lex_test_multi2.zig -----
//      ifj.concat(
//           \\prvni
//          \\parametr
//          ,
//          \\druhy
//          \\parametr
//      );
// ----- Konec souboru: lex_test_multi2.zig -----
TEST(Lex, lex_test_multi2){
    string path = lex_path + "lex_test_multi2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // ifj.concat(
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS,

        // první multiline
        TOKEN_STRING,

        // ,
        TOKEN_COMMA,

        // druhý multiline
        TOKEN_STRING,

        // );
        TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
    };

    const char* expected_strings[] = {
        "prvni\nparametr",
        "druhy\nparametr"
    };


    /**
     * @brief TESTOVACÍ STRUKTURA PRO NAČÍTÁNÍ MNOŽINY TOKENŮ S VALUE U STRINGŮ
     */
    for (unsigned long i = 0, j = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Uložíme si pozici vstupu
        long int current_pos = ftell(stdin);
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Potomek zavolá testTokenType a ukončí se
            if(expected_arr[i] == TOKEN_STRING){
                if(j > sizeof(expected_strings) / sizeof(char*)){
                    cerr << "Error in token " << i << endl;
                    cerr << "Expected strings is too small: " << endl;
                    FAIL();
                }
                testTokenString(expected_strings[j]);
            } else {
                testTokenType(expected_arr[i]);
            }

            exit(0);
        } else {
            // Rodičovský proces čeká na potomka
            int status;
            waitpid(pid, &status, 0);
            if(WEXITSTATUS(status) == 0){
                if(expected_arr[i] == TOKEN_STRING){
                    j++;
                }
                continue;
            }else if (WEXITSTATUS(status) == 2) {
                // Potomek failnul testy, ale ne lex chybou
                ADD_FAILURE();
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;
            }else if (WEXITSTATUS(status) == 1) {
                // Potomek skončil s lex chybou
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
            if(expected_arr[i] == TOKEN_STRING){
                j++;
            }
        }
    }

    stdin = stdin_backup;
    fclose(f);

}

// ----- Začátek souboru: lex_test_string_escapes.zig -----
//      ifj.concat(
//          "Toto je test escape sekvenci stringu. Lomeno n: \n; Lomeno lomeno: \\; Lomeno uvozovky: \"; Lomeno r: \r; Lomeno t: \t.",
//          "Toto je test x escape sekvence stringu. :(58) = \x3A; }(125) = \x7D; NL(10) = \x0A; ú(250) = \xFA."
//      );
// ----- Konec souboru: lex_test_string_escapes.zig -----
TEST(Lex, lex_test_string_escapes){
    string path = lex_path + "lex_test_string_escapes.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // ifj.concat(
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS,

        // první multiline
        TOKEN_STRING,

        // ,
        TOKEN_COMMA,

        // druhý multiline
        TOKEN_STRING,

        // );
        TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
    };

    const char* expected_strings[] = {
        "Toto je test escape sekvenci stringu. Lomeno n: \n; Lomeno lomeno: \\; Lomeno uvozovky: \"; Lomeno r: \r; Lomeno t: \t.",
        "Toto je test x escape sekvence stringu. :(58) = :; }(125) = }; NL(10) = \n; ú(250) = \xFA."
    };


    /**
     * @brief TESTOVACÍ STRUKTURA PRO NAČÍTÁNÍ MNOŽINY TOKENŮ S VALUE U STRINGŮ
     */
    for (unsigned long i = 0, j = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Uložíme si pozici vstupu
        long int current_pos = ftell(stdin);
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Potomek zavolá testTokenType a ukončí se
            if(expected_arr[i] == TOKEN_STRING){
                if(j > sizeof(expected_strings) / sizeof(char*)){
                    cerr << "Error in token " << i << endl;
                    cerr << "Expected strings is too small: " << endl;
                    FAIL();
                }
                testTokenString(expected_strings[j]);
            } else {
                testTokenType(expected_arr[i]);
            }

            exit(0);
        } else {
            // Rodičovský proces čeká na potomka
            int status;
            waitpid(pid, &status, 0);
            if(WEXITSTATUS(status) == 0){
                if(expected_arr[i] == TOKEN_STRING){
                    j++;
                }
                continue;
            }else if (WEXITSTATUS(status) == 2) {
                // Potomek failnul testy, ale ne lex chybou
                ADD_FAILURE();
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;
            }else if (WEXITSTATUS(status) == 1) {
                // Potomek skončil s lex chybou
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
            if(expected_arr[i] == TOKEN_STRING){
                j++;
            }
        }
    }

    stdin = stdin_backup;
    fclose(f);

}

// ----- Začátek souboru: lex_test_comment.zig -----
//      const ifj = @import("ifj24.zig");
//      //alphabetagamma
//      123;
//      //jedna dva tři čtyři pět šest
// ----- Konec souboru: lex_test_comment.zig -----
TEST(Lex, Comment) {
    string path = lex_path + "lex_test_comment.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Your corrected expected tokens
        TOKEN_K_const, TOKEN_K_ifj, TOKEN_EQUALITY_SIGN, TOKEN_K_import, TOKEN_LEFT_PARENTHESIS, TOKEN_STRING, TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON,
        TOKEN_INT, TOKEN_SEMICOLON,
        TOKEN_EOF
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Save the current position in the input file
        long int current_pos = ftell(stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Child process calls testTokenType and exits
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Parent process waits for the child
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Child process exited with an error
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Print the remaining input
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

// ----- Začátek souboru: lex_test_satanic.zig -----
//  ifj.insejn(
//    ?[]u8
//    ?
//    [
//                 ]    
//                        u8
//    ?    i32
//    ?
//     f64
//    (){}|.,:;+-/*=!=>>=<<=insejn123.123.05 ifelse _ _abrakadabra
//  );
// ----- Konec souboru: lex_test_satanic.zig -----
TEST(Lex, Satanic) {
    string path = lex_path + "lex_test_satanic.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    TokenType expected_arr[] = {
        // Your corrected expected tokens
        TOKEN_K_ifj, TOKEN_PERIOD, TOKEN_IDENTIFIER, TOKEN_LEFT_PARENTHESIS, TOKEN_K_Qu8, TOKEN_K_Qu8, TOKEN_K_Qi32, TOKEN_K_Qf64,
        TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_CURLY_BRACKET, TOKEN_RIGHT_CURLY_BRACKET, TOKEN_VERTICAL_BAR,
        TOKEN_PERIOD, TOKEN_COMMA, TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_PLUS, TOKEN_MINUS, TOKEN_SLASH, TOKEN_ASTERISK,
        TOKEN_EQUALITY_SIGN, TOKEN_NOT_EQUAL_TO, TOKEN_GREATER_THAN, TOKEN_GREATER_EQUAL_THAN, TOKEN_LESS_THAN, TOKEN_LESS_EQUAL_THAN,
        TOKEN_IDENTIFIER, TOKEN_PERIOD, TOKEN_FLOAT, TOKEN_IDENTIFIER, TOKEN_K_underscore, TOKEN_IDENTIFIER,
        TOKEN_RIGHT_PARENTHESIS, TOKEN_SEMICOLON, TOKEN_EOF
    };

    for (unsigned long i = 0; i < sizeof(expected_arr) / sizeof(TokenType); i++) {
        // Save the current position in the input file
        long int current_pos = ftell(stdin);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork\n");
            exit(1);
        } else if (pid == 0) {
            // Child process calls testTokenType and exits
            testTokenType(expected_arr[i]);
            exit(0);
        } else {
            // Parent process waits for the child
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0) {
                // Child process exited with an error
                cerr << "Error in token " << i << endl;
                cerr << "Expected type: " << expected_arr[i] << endl;

                // Print the remaining input
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

// ----- Začátek souboru: lex_test_unterminated_string_01.zig -----
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//          const s1 : []u8 = ifj.string("Unterminated string); // ukoncujici uvozovky ovlivnuji implicitni odsazeni vnitrnich radku retezce
//          ifj.write(s1);
//      }
// ----- Konec souboru: lex_test_unterminated_string_01.zig -----

// ----- Začátek souboru: lex_test_unterminated_string_02.zig -----
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//          const s1 : []u8 = "Unterminated string;
//          ifj.write(s1);
//      }
// ----- Konec souboru: lex_test_unterminated_string_02.zig -----

// ----- Začátek souboru: lex_test_unterminated_string_03.zig -----
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const s1 : []u8 = Unterminated_string";
//          ifj.write(s1);
//      }
// ----- Konec souboru: lex_test_unterminated_string_03.zig -----

// ----- Začátek souboru: lex_test_unterminated_string_04.zig -----
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const s1 : []u8 = ifj.string(Unterminated_string");
//          ifj.write(s1);
//      }
// ----- Konec souboru: lex_test_unterminated_string_04.zig -----
TEST(Lex, UnterminatedString){
    for (int i = 1; i <= 4; i++) {
        string filename = "lex_test_unterminated_string_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = lex_path + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(1), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}


/*** Konec souboru scanner_test.cpp ***/