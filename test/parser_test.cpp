/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           parser_test.cpp                                           *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            08.11.2024                                                *
 * Poslední změna:   02.12.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file parser_test.cpp
 * @author Jan Kalina \<xkalinj00>
 *
 * @brief Testy syntaktického analyzátoru (parseru).
 * @details Tento zdrojový soubor obsahuje unit testy celkové funkčnosti
 *          parseru i jednotlivých submodulů parseru jako jsou LL a precedenční
 *          parsery, LL a precedenční tabulky a práce parseru se zásobníkem
 *          rámců. Testy jsou vytvořené za využití frameworku Google Test.
 */

/*******************************************************************************
 *                                                                             *
 *             AKTIVACE MAKER PRO VIZUALIZACI STRUKTUR PŘEKLADAČE              *
 *                                                                             *
 ******************************************************************************/

// Kompletně vypneme výpis
#define DISABLE_PRINT

// Budeme tisknout strom i obsah zásobníku rámců
#define PRINT_AST_OUT 1
#define PRINT_FRAME_STACK_OUT 1

// Definice názvu specifického testu, pro který se má provádět tisk
/* Pozn. Pokud není definováno, tisk se provádí pro všechny testy */
//#define SPECIFIC_TEST_NAME "Hello"


/*******************************************************************************
 *                                                                             *
 *                         IMPORT HLAVIČKOVÝCH SOUBORŮ                         *
 *                                                                             *
 ******************************************************************************/

// Import knihoven GoogleTest frameworku
#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Import sdílených knihoven překladače (test-utils)
#include "ifj24_compiler_test_utils.h"

/*******************************************************************************
 *                                                                             *
 *                   UNIT TESTY SUBMODULŮ PARSERU JAKO CELKU                   *
 *                                                                             *
 ******************************************************************************/

// ~~~~~ Začátek souboru: correct_prologue_and_empty_main.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { }
// ~~~~~ Konec souboru: correct_prologue_and_empty_main.zig ~~~~~
TEST(LLParserBasicsCorrect, PrologueAndEmptyMain) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_prologue_and_empty_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectPrologueAndEmptyMain);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_one_param.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const y: i32 = 42;
//        foo(y);
//    }
//    pub fn foo(x: i32) void {
//        ifj.write(x);
//    }
// ~~~~~ Konec souboru: correct_one_param.zig ~~~~~
TEST(LLParserBasicsCorrect, OneParam) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_one_param.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectOneParam);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_two_params.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { }
//    pub fn foo(x: i32, y: []u8) void {
//        ifj.write(x);
//        ifj.write(y);
//    }
// ~~~~~ Konec souboru: correct_two_params.zig ~~~~~
TEST(LLParserBasicsCorrect, TwoParams) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_two_params.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectTwoParams);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_two_fun.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { }
//    pub fn foo() void { }
// ~~~~~ Konec souboru: correct_two_fun.zig ~~~~~
TEST(LLParserBasicsCorrect, TwoFunctions) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_two_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectTwoFunctions);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_var_def.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x = 10;
//        x = x;
//    }
// ~~~~~ Konec souboru: correct_var_def.zig ~~~~~
TEST(LLParserBasicsCorrect, VarDef) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_var_def.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectVarDef);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_no_args.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        foo();
//    }
//    pub fn foo() void { }
// ~~~~~ Konec souboru: correct_fun_call_no_args.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallNoArgs) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_no_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);
    
    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallNoParam);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_nonvoid_fun.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { }
//    pub fn foo_1() i32 {
//        return 42;
//    }
//    pub fn foo_2() ?i32 {
//        return null;
//    }
//    pub fn foo_3() f64 {
//        return 3.14;
//    }
//    pub fn foo_4() ?f64 {
//        return null;
//    }
//    pub fn foo_5() []u8 {
//        return ifj.string("hello");
//    }
//    pub fn foo_6() ?[]u8 {
//        return null;
//    }
// ~~~~~ Konec souboru: correct_nonvoid_fun.zig ~~~~~
TEST(LLParserBasicsCorrect, NonVoidFun) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_nonvoid_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectNonVoidFun);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_int_lit.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        _ = foo(10);
//        const ten_days_till_deadline = foo(10);
//        ifj.write(ten_days_till_deadline);
//    }
//    pub fn foo(x: i32) i32 {
//        return x;
//    }
// ~~~~~ Konec souboru: correct_fun_call_int_lit.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallIntLit) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_int_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallIntLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_float_lit.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        _ = foo(3.14);
//        const pi = foo(3.14);
//        ifj.write(pi);
//    }
//    pub fn foo(x: ?f64) ?f64 {
//        return x;
//    }
// ~~~~~ Konec souboru: correct_fun_call_float_lit.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallFloatLit) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_float_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallFloatLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_string_lit.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        _ = foo(ifj.string("Hello, World!"));
//        const greeting: ?[]u8 = foo(ifj.string("Hello, World!"));
//        ifj.write(greeting);
//    }
//    pub fn foo(x: []u8) []u8 {
//        return x;
//    }
// ~~~~~ Konec souboru: correct_fun_call_string_lit.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallStringLit) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_string_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallStringLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_var.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x: i32 = 10;
//        x = x;
//        _ = foo(x);
//    }
//    pub fn foo(z: i32) i32 {
//        return z;
//    }
// ~~~~~ Konec souboru: correct_fun_call_var.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallVar) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallVar);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_two_args.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        foo(3.14, ifj.string("Please work"));
//    }
//    pub fn foo(x: f64, y: []u8) void {
//        ifj.write(x);
//        ifj.write(y);
//    }
// ~~~~~ Konec souboru: correct_fun_call_two_args.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallTwoArgs) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_two_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallTwoArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_many_args.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const var_with_very_long_identifier: ?f64 = 2.71;
//        foo(10, 3.14, var_with_very_long_identifier, null, "Please work");
//        var assign_the_monster = foo(10, 3.14, var_with_very_long_identifier, null, "Please work");
//    }
// ~~~~~ Konec souboru: correct_fun_call_many_args.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallManyArgs) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_many_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallManyArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_fun_call_comma_last.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        foo(
//            "Somebody left comma after me!",
//        );
//        bar(
//            1024,
//            "Somebody left comma after me, AGAIN!",
//        );
//        const y = foo(
//            "Somebody left comma after me!",
//        );
//        var z = bar(
//            1024,
//            "Somebody left comma after me, AGAIN!",
//        );
//    }
// ~~~~~ Konec souboru: correct_fun_call_comma_last.zig ~~~~~
TEST(LLParserBasicsCorrect, FunCallCommaLast) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_fun_call_comma_last.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunCallCommaLast);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_no_args.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        foo();
//    }
//    pub fn foo() void { }
// ~~~~~ Konec souboru: correct_ifj_fun_call_no_args.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallNoArgs) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_no_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallIntLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_int_lit.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        ifj.write(42);
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_int_lit.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallIntLit) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_int_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallIntLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_float_lit.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        _ = ifj.i2f(10);
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_float_lit.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallFloatLit) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_float_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallFloatLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_string_lit.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        ifj.write("Hello, World!");
//        const greeting: ?[]u8 = null;
//        ifj.write(greeting);
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_string_lit.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallStringLit) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_string_lit.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallStringLit);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_var.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 10;
//        const y = foo(x);
//        ifj.write(y);
//    }
//    pub fn foo(z: i32) ?[]u8 {
//        ifj.write(z);
//        return null;
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_var.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallVar) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallVar);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_two_args.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        _ = ifj.concat(ifj.string("3.14..."), ifj.string("Please work!"));
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_two_args.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallTwoArgs) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_two_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallTwoArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_many_args.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const var_with_very_long_identifier = ifj.string("2.71");
//        const nullable: []u8 = ifj.string("null");
//        const pi = ifj.string("3.14");
//        const ten = ifj.string("10");
//        const assign_the_monster =
//            ifj.concat(ifj.string(ten), (ifj.concat(ifj.string(pi),
//            (ifj.concat(ifj.string(var_with_very_long_identifier), 
//            ifj.concat(ifj.string(nullable), ifj.string("Please work")))))));
//        ifj.write(assign_the_monster);
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_many_args.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallManyArgs) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_many_args.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallManyArgs);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_ifj_fun_call_comma_last.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        ifj.foo(
//            "Somebody left comma after me!",
//        );
//        ifj.bar(
//            1024,
//            "Somebody left comma after me, AGAIN!",
//        );
//        const y = ifj.foo(
//            "Somebody left comma after me!",
//        );
//        var z = ifj.bar(
//            1024,
//            "Somebody left comma after me, AGAIN!",
//        );
//    }
// ~~~~~ Konec souboru: correct_ifj_fun_call_comma_last.zig ~~~~~
TEST(LLParserBasicsCorrect, IFJFunCallCommaLast) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_ifj_fun_call_comma_last.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIFJFunCallCommaLast);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: error_excesive_curly_brackets.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { {} }
// ~~~~~ Konec souboru: error_excesive_curly_brackets.zig ~~~~~
TEST(LLParserError, ExcesiveCurlyBrackets) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "error_excesive_curly_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");

    // Kořen je inicializován
    EXPECT_EQ(ASTroot, nullptr);

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_ifj_dot_etc.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var vysl: i32 = 0;
//     vysl = 65536;
//        var habib: ?[]u8 = ifj.string("");
//     habib = ifj.string("baba$Ja$Ga");
//        var abus_babus: ?f64 = 3.14;
//     abus_babus = null;
//        ifj.write(ifj.string("#ifj.wrajt"));
//        const a = ifj.f2i(vysl);
//        const b = ifj.readi32();
//        const c = ifj.string("#ifj.stryng");
//        const d = ifj.strcmp(habib, abus_babus);
//        ifj.write(ifj.concat(ifj.string(a), ifj.concat(ifj.string(b), c)), ifj.string(d));
//    }
// ~~~~~ Konec souboru: X_correct_ifj_dot_etc.zig ~~~~~
TEST(LLParserBasicsCorrect, IfjDotEtc) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_ifj_dot_etc.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIfjDotEtc);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_add_operators_minus.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x: i32 = 42;
//        x = x - x;
//    }
// ~~~~~ Konec souboru: X_correct_add_operators_minus.zig ~~~~~
TEST(LLParserBasicsCorrect, AddOperatorsMinus) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_add_operators_minus.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectAddOperatorsMinus);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_add_operators_plus.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x = 5.811547;
//        x = x + x;
//    }
// ~~~~~ Konec souboru: X_correct_add_operators_plus.zig ~~~~~
TEST(LLParserBasicsCorrect, AddOperatorsPlus) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_add_operators_plus.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectAddOperatorsPlus);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_multi_operators_asterisk.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x: f64 = 0.000005;
//        x = x * x;
//    }
// ~~~~~ Konec souboru: X_correct_multi_operators_asterisk.zig ~~~~~
TEST(LLParserBasicsCorrect, MultiOperatorsAsterisk) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_multi_operators_asterisk.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectMultiOperatorsAsterisk);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_multi_operators_slash.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x = 0;
//        x = x / x;
//    }
// ~~~~~ Konec souboru: X_correct_multi_operators_slash.zig ~~~~~
TEST(LLParserBasicsCorrect, MultiOperatorsSlash) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_multi_operators_slash.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectMultiOperatorsSlash);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_rel_operators_EE.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x = 59 + 3.14;
//        if (x == x) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_rel_operators_EE.zig ~~~~~
TEST(LLParserBasicsCorrect, RelOperatorsEE) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_rel_operators_EE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectRelOperatorsEE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_rel_operators_G.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x = 3.14;
//        x = 0.0000000000000000000;
//        if (x > x) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_rel_operators_G.zig ~~~~~
TEST(LLParserBasicsCorrect, RelOperatorsG) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_rel_operators_G.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectRelOperatorsG);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_rel_operators_GE.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 10058051;
//        if (x >= x) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_rel_operators_GE.zig ~~~~~
TEST(LLParserBasicsCorrect, RelOperatorsGE) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_rel_operators_GE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectRelOperatorsGE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_rel_operators_L.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x = 58 / 52;
//        if (x < x) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_rel_operators_L.zig ~~~~~
TEST(LLParserBasicsCorrect, RelOperatorsL) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_rel_operators_L.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectRelOperatorsL);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_rel_operators_LE.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: f64 = 3591452472653462.1658248;
//        if (x <= x) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_rel_operators_LE.zig ~~~~~
TEST(LLParserBasicsCorrect, RelOperatorsLE) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_rel_operators_LE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectRelOperatorsLE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_rel_operators_NE.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x = 69.78;
//        if (x != x) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_rel_operators_NE.zig ~~~~~
TEST(LLParserBasicsCorrect, RelOperatorsNE) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_rel_operators_NE.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectRelOperatorsNE);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_data_types.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { }
//    pub fn foo(a: i32, b: ?i32, c: f64, d: ?f64, e: []u8, f: ?[]u8) void {
//        ifj.write(a);
//        ifj.write(b);
//        ifj.write(c);
//        ifj.write(d);
//        ifj.write(e);
//        ifj.write(f);
//    }
// ~~~~~ Konec souboru: X_correct_data_types.zig ~~~~~
TEST(LLParserBasicsCorrect, DataTypes) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_data_types.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectDataTypes);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_if_null_condition.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: ?f64 = 54 + 3.14;
//        if (x == x) |y| {
//        //        ifj.write(y);
//        } else {
//        //        var z: ?f64 = null;
//            z = x;
//        //        ifj.write(z);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_null_condition.zig ~~~~~
TEST(LLParserBasicsCorrect, IfNullCondition) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_if_null_condition.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIfNullCondition);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_throw_away.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x: i32 = 10;
//        x = 42;
//        _ = x / x;
//    }
// ~~~~~ Konec souboru: X_correct_throw_away.zig ~~~~~
TEST(LLParserBasicsCorrect, ThrowAway) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_throw_away.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectThrowAway);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_fun_return.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void { }
//    pub fn foo(a: ?f64) ?i32 {
//        return a;
//    }
// ~~~~~ Konec souboru: X_correct_fun_return.zig ~~~~~
TEST(LLParserBasicsCorrect, FunReturn) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_fun_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectFunReturn);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_if_else.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 10;
//        if (x == 5) { } else { }
//    }
// ~~~~~ Konec souboru: X_correct_if_else.zig ~~~~~
TEST(LLParserBasicsCorrect, IfElse) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_if_else.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectIfElse);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_while.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x: i32 = 10;
//        while (x > 0) {
//        //        ifj.write("while\n");
//        //        x = x - 1;
//        }
//    }
// ~~~~~ Konec souboru: X_correct_while.zig ~~~~~
TEST(LLParserBasicsCorrect, While) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_while.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectWhile);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_no_brackets.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 42;
//        const y: i32 = 64.0;
//        const z: i32 = 0 - 6;
//        const vysledek = x + y * z - 5 / x + 10 * y - 15 / z * x + 20 - y * 
//                         25 / x + z * 30 - 35 / y * z + 40;
//        ifj.write(vysledek);
//    }
// ~~~~~ Konec souboru: correct_complex_expr_no_brackets.zig ~~~~~
TEST(LLParserComplexCorrect, ExprNoBrackets) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_no_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprNoBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_brackets.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 42;
//        const y: i32 = 64.0;
//        const z: i32 = 0 - 6;
//        const vysledek = ((((x + (((y * z))))) - ((5 / x))) + (((((10 * y))) - 
//                         (((15 / (z * x)))))) + (20 - ((((((y * 25)) / x)))) + 
//                         ((z * 30)) - (((35 / (y * z)))) + 40));
//        ifj.write(vysledek);
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_brackets.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithBrackets) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_fun_no_brackets.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: ?i32 = 42;
//        var y = 64.8;
//        var z: f64 = 0 - 6;
//        const vysledek = func1(x) + func2(y, z) * func3(5) - ifj.func4(x, 10) / 
//                         func5(y, z, 15) + func6(20, y) * func7(z) - 
//                         ifj.func8(25, x, y) / ifj.func9() * x + 35 - y * 
//                         ifj.func10(40, z) / x + func11(y, 45) * 50 - 55 / 
//                         ifj.func12(60, y, z) * func13() + 70;
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_fun_no_brackets.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithFunCallNoBrackets) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_fun_no_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithFunCallNoBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_fun_in_brackets.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const vysledek = ((func1(10, 3.14, "hello") + func2()));
//    }
// ~~~~~ Konec souboru: correct_complex_expr_fun_in_brackets.zig ~~~~~
TEST(LLParserComplexCorrect, ExprFunInBrackets) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_fun_in_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprFunInBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_fun_and_brackets.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 42;
//        const y: i32 = 64;
//        const z: i32 = 0 - 6;
//        const float: f64 = 3.14;
//        const vysledek = ((((((func1(x))) +
//            ((((func2(y, z))) *
//            ((func3(5)))))) -
//            ((((ifj.f2i(float))) /
//            ((func5(y, z, 15)))))) +
//            (((func6(20, y)) *
//            (func7(z)))) -
//            (((ifj.f2i(float)) /
//            (ifj.f2i(float))) * x)) +
//            (((35) -
//            ((y) *
//            (ifj.f2i(float)))) / x) +
//            ((func11(y, 45)) * 50) -
//            (((55) /
//            (ifj.f2i(float))) *
//            (func13())) +
//            70);
//        ifj.write(vysledek);
//    }
//    pub fn func1(x: i32) i32 {
//        return x;
//    }
//    pub fn func2(x: i32, y: i32) i32 {
//        return x * y;
//    }
//    pub fn func3(x: i32) i32 {
//        return x - 8;
//    }
//    pub fn func4(x: i32, y: i32) i32 {
//        return x / y;
//    }
//    pub fn func5(x: i32, y: i32, z: i32) i32 {
//        return 8 / 2 * z - 4 * x + y;
//    }
//    pub fn func6(x: i32, y: i32) i32 {
//        return 4 * x + y;
//    }
//    pub fn func7(x: i32) i32 {
//        return 42 - x;
//    }
//    pub fn func11(x: i32, y: i32) i32 {
//        return 2 * y - x;
//    }
//    pub fn func13() i32 {
//        return 1;
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_fun_and_brackets.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithFunCallAndBrackets) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithFunCallAndBrackets);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_fun_and_brackets_hellish_1.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: ?i32 = 42;
//        var y = 64.8;
//        var z: f64 = 0 - 6;
//        const vysledek = (((((((func1(((((((x +
//            (((5 -
//            (((y *
//            (((z)))))))))))))))))) +
//            ((((func2((((((y /
//            (((2)))))))), (((((z -
//            (((3)))))))))) *
//            ((((func3(((((((5 *
//            (((2 /
//            (((y +
//            (((z))))))))))))))))))))))))) -
//            ((((ifj.func4((((((x -
//            (((10)))))))), (((((10 +
//            (((z -
//            (((y))))))))))))) /
//            (((func5((((((y *
//            (((z)))))))), (((((z /
//            (((2 +
//            (((x))))))))))), (((((15 -
//            (((x)))))))))))))))) +
//            ((((func6((((((20 *
//            (((x -
//            (((y))))))))))), (((((y +
//            (((z)))))))))) *
//            (((func7((((((z *
//            (((5 /
//            (((x)))))))))))))))))) -
//            ((((ifj.func8((((((25 -
//            (((y)))))))), (((((x +
//            (((z)))))))), (((((y *
//            (((3)))))))))) /
//            (((ifj.func9())))) *
//            (((x)))))) +
//            (((((35 -
//            (((y *
//            (((ifj.func10(((((40 +
//            (((x -
//            (((z)))))))))), ((((z *
//            (((y))))))))))))))) /
//            (((x))))))) +
//            (((func11(((((y +
//            (((3))))))), ((((45 -
//            (((z))))))))) *
//            (((50))))) -
//            ((((55 /
//            (((ifj.func12((((((60 *
//            (((z)))))))), ((((y -
//            (((2))))))), ((((z +
//            (((5 -
//            (((x))))))))))))))) *
//            (((func13((((((x *
//            (((2 +
//            (((y -
//            (((z))))))))))))))))))))) +
//            ((((70 -
//            (((x +
//            (((y)))))))))));
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_fun_and_brackets_hellish_1.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish1) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithFunCallAndBracketsHellish1);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_fun_and_brackets_hellish_2.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const vysledek = func1((anotherFunc1(10)));
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_fun_and_brackets_hellish_2.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish2) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithFunCallAndBracketsHellish2);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_fun_and_brackets_hellish_3.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//       const x: ?i32 = 42;
//       var y = 64.8;
//       var z: f64 = 0 - 6;
//       const vysledek = (func1(((x +
//            (5 -
//            (y *
//            (z +
//            anotherFunc1(x + (y * z))))))))) +
//            (func2((y / 2), (z -
//            anotherFunc2(x - y + (z * func4(x))))) *
//            func3(5 * (2 / (y + z)))) -
//            (ifj.func4(x - 10, 10 + anotherFunc5(x * y)) /
//            func5(y * z, z / (2 + x), 15 - x)) +
//            (func6(20 * (x - y), y + z) *
//            func7(z * (5 / x))) +
//            (ifj.func8(25 - y, x + z, y * 3) /
//            ifj.func9()) / x +
//            (35 -
//            (y * ifj.func10(40 + x, z * y))) / x +
//            (func11(y + 3, 45 - z) * 50) -
//            (55 /
//            ifj.func12(60 * z, y - 2, z + (5 - x)) * func13(x * (2 + (y - z)))) +
//            (70 - (x + y));
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_fun_and_brackets_hellish_3.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish3) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithFunCallAndBracketsHellish3);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_complex_expr_with_fun_and_brackets_hellish_4.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: ?i32 = 42;
//        var y = 64.8;
//        var z: f64 = 0 - 6;
//        const vysledek = (((((((func1(((((((x +
//            (((5 -
//            (((y *
//            (((z +
//            ((anotherFunc1(((x +
//            (((y *
//            ((z)))))))))))))))))))))))))))) +
//            ((((func2((((((y /
//            (((2 +
//            ((anotherFunc2(((z -
//            (((y)))))))))))))))), (((((z -
//            (((3 *
//            (((anotherFunc3(((x -
//            (((y +
//            ((z *
//            ((func4((x -
//            y))))))))))))))))))))))))) *
//            ((((func3(((((((5 *
//            (((2 /
//            (((y +
//            (((z *
//            ((anotherFunc4((x + y - z))))))))))))))))))))))))))))) -
//            ((((ifj.func4((((((x -
//            (((10 +
//            (anotherFunc5((x * y - z))))))))))), (((((10 +
//            (((z -
//            (((y *
//            ((func6((x - y))))))))))))))))) /
//            (((func5((((((y *
//            (((z *
//            (anotherFunc6((y + z))))))))))), (((((z /
//            (((2 +
//            (anotherFunc7((x + y))))))))))), (((((15 -
//            (((x *
//            (anotherFunc8((z - y))))))))))))))))))) +
//            ((((func6((((((20 *
//            (((x -
//            (anotherFunc9((y + z))))))))))), (((((y +
//            (((z *
//            (anotherFunc10((x - y))))))))))))) *
//            (((func7((((((z *
//            (((5 /
//            (((x +
//            (anotherFunc11((x - y))))))))))))))))))))) -
//            ((((ifj.func8((((((25 -
//            (((y *
//            (anotherFunc12((x + z))))))))))), (((((x +
//            (((z *
//            (anotherFunc13((y - z))))))))))), (((((y *
//            (((3 +
//            (anotherFunc14((x * y))))))))))))) /
//            (((ifj.func9())))) *
//            (((x)))))) +
//            (((((35 -
//            (((y *
//            (((ifj.func10(((((40 +
//            (((x -
//            (((z +
//            (anotherFunc15((y + z))))))))))))), ((((z *
//            (((y +
//            (anotherFunc16((x - z)))))))))))))))))) /
//            (((x))))))) +
//            (((func11(((((y +
//            (((3 *
//            (anotherFunc17((y - z)))))))))), ((((45 -
//            (((z *
//            (anotherFunc18((x + y)))))))))))) *
//            (((50))))) -
//            ((((55 /
//            (((ifj.func12((((((60 *
//            (((z *
//            (anotherFunc19((x + z))))))))))), ((((y -
//            (((2 *
//            (anotherFunc20((z - y)))))))))), ((((z +
//            (((5 -
//            (((x *
//            (anotherFunc21((y + z)))))))))))))))))) *
//            (((func13((((((x *
//            (((2 +
//            (((y -
//            (((z *
//            (anotherFunc22((x - y)))))))))))))))))))))))) +
//            ((((70 -
//            (((x +
//            (((y *
//            (anotherFunc23((z - x))))))))))))));
//    }
// ~~~~~ Konec souboru: correct_complex_expr_with_fun_and_brackets_hellish_4.zig ~~~~~
TEST(LLParserComplexCorrect, ExprWithFunCallAndBracketsHellish4) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_complex_expr_with_fun_and_brackets_hellish_4.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectExprWithFunCallAndBracketsHellish3);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: X_correct_while_null_condition.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        var x: i32 = 10;
//        while (x > 0) |id_bez_null| {
//        //        ifj.write("while\n");
//        //        x = x - 1;
//        }
//    }
// ~~~~~ Konec souboru: X_correct_while_null_condition.zig ~~~~~
TEST(LLParserBasicsCorrect, WhileNullCondition) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "X_correct_while_null_condition.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectWhileNullCondition);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_return.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn return_epsilon(r: f64, t: ?f64) f64 {
//        ifj.write(r);
//        ifj.write(t);
//        return 3.14;
//    }
//    pub fn main() void { }
//    pub fn return_var(r: f64, t: ?f64) f64 {
//        ifj.write(t);
//        return r;
//    }
//    pub fn return_expr(r: f64, t: f64) f64 {
//        return r + t;
//    }
//    pub fn return_recursive(r: f64, t: f64) f64 {
//        var x: ?f64 = r + t - return_var(t, t / r);
//        x = x;
//        x = null;
//        return return_expr(t * r, r);
//    }
// ~~~~~ Konec souboru: correct_return.zig ~~~~~
TEST(LLParserBasicsCorrect, Return) {
    // Otevření testovacího souboru a příprava STDIN
    string path = syntPath + "correct_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectReturn);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: correct_priority.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 20;
//        var half_x = x - 2 * (x / 2);
//    }
// ~~~~~ Konec souboru: correct_priority.zig ~~~~~
TEST(LLParserBasicsCorrect, Priority) {
    // Načtení souboru s programem na STDIN
    string path = syntPath + "correct_priority.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(CorrectPriority);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// Testovací soubory jsou z prostorových důvodů umístěny na konci tohoto souboru
TEST(LLParserBasicsCorrect, IfCondition) {
    for (int i = 1; i <= 7; i++) {
        string filename = "correct_if_condition_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        LLparser_parseProgram();
        EXPECT_NE(ASTroot, nullptr);
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

// ~~~~~ Začátek souboru: example1.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        ifj.write("Zadejte cislo pro vypocet faktorialu\n");
//        const a = ifj.readi32();
//        if (a) |val| {
//            if (val < 0) {
//                ifj.write("Faktorial ");
//                ifj.write(val);
//                ifj.write(" nelze spocitat\n");
//            } else {
//                var d: f64 = ifj.i2f(val);
//                var vysl: f64 = 1.0;
//                while (d > 0) {
//                    vysl = vysl * d;
//                    d = d - 1.0;
//                }
//                ifj.write("Vysledek: ");
//                ifj.write(vysl);
//                ifj.write(" = ");
//                const vysl_i32 = ifj.f2i(vysl);
//                ifj.write(vysl_i32);
//                ifj.write("\n");
//            }
//        } else { // a == null
//            ifj.write("Faktorial pro null nelze spocitat\n");
//        }
//    }
// ~~~~~ Konec souboru: example1.zig ~~~~~
TEST(LLParserExamples, Example1) {
    // Otevření testovacího souboru a příprava STDIN
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(Example1);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: example2.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        ifj.write("Zadejte cislo pro vypocet faktorialu: ");
//        const inp = ifj.readi32();
//        if (inp) |INP| {
//            if (INP < 0) {
//                ifj.write("Faktorial nelze spocitat!\n");
//            } else {
//                const vysl = factorial(INP);
//                ifj.write("Vysledek: ");
//                ifj.write(vysl);
//            }
//        } else {
//            ifj.write("Chyba pri nacitani celeho cisla!\n");
//        }
//    }
//    pub fn decrement(n: i32, m: i32) i32 {
//        return n - m;
//    }
//    pub fn factorial(n: i32) i32 {
//        var result: i32 = 0 - 1;
//        if (n < 2) {
//            result = 1;
//        } else {
//            const decremented_n = decrement(n, 1);
//            const temp_result = factorial(decremented_n);
//            result = n * temp_result;
//        }
//        return result;
//    }
// ~~~~~ Konec souboru: example2.zig ~~~~~
TEST(LLParserExamples, Example2) {
    // Otevření testovacího souboru a příprava STDIN
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(Example2);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: example3.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const str1 = ifj.string("Toto je nejaky text v programu jazyka IFJ24");
//        var str2 = ifj.string(", ktery jeste trochu obohatime");
//        str2 = ifj.concat(str1, str2);
//        ifj
//            .  write(str1);
//        ifj.write("\n");
//        ifj.write(str2);
//        ifj.write("\n");
//        ifj.write("Zadejte serazenou posloupnost vsech malych pismen a-h, ");
//        var newInput = ifj.readstr();
//        var all: []u8 = ifj.string("");
//        while (newInput) |inpOK| {
//            const abcdefgh = ifj.string("abcdefgh"); 
//            const strcmpResult = ifj.strcmp(inpOK, abcdefgh);
//            if (strcmpResult == 0) {
//                ifj.write("Spravne zadano!\n");
//                ifj.write(all); 
//                newInput = null; 
//            } else {
//                ifj.write("Spatne zadana posloupnost, zkuste znovu:\n");
//                all = ifj.concat(all, inpOK); 
//                newInput = ifj.readstr();
//            }
//        }
//    }
// ~~~~~ Konec souboru: example3.zig ~~~~~
TEST(LLParserExamples, Example3) {
    // Otevření testovacího souboru a příprava STDIN
    string path = examPath + "example3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(Example3);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: fun.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn f (x : i32) i32
//    {
//        if(x<10){return x-1;}else{const y = x - 1; 
//            ifj.write("calling g with "); ifj.write(y); ifj.write("\n");
//            const res = g(y);
//            return res;
//        }
//    }
//    pub fn g(x:i32) i32 {
//        if (x > 0) {
//            ifj.write("calling f with "); ifj.write(x); ifj.write("\n"); 
//            const y = f(x);
//            return y; 
//        } else {
//            return 200;
//        }
//    }
//    pub fn main() void {
//        const res = g(10);
//        ifj.write("res: "); ifj.write(res); ifj.write("\n");
//    }
// ~~~~~ Konec souboru: fun.zig ~~~~~
TEST(LLParserExamples, Fun) {
    // Otevření testovacího souboru a příprava STDIN
    string path = examPath + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(Fun);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: hello.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const y : i32 = 24;
//        ifj.write("Hello from IFJ");
//        ifj.write(y);
//        ifj.write("\n");
//    }
// ~~~~~ Konec souboru: hello.zig ~~~~~
TEST(LLParserExamples, Hello) {
    // Otevření testovacího souboru a příprava STDIN
    string path = examPath + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(Hello);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// ~~~~~ Začátek souboru: multiline.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const s1 : []u8 = ifj.string( 
//            \\To\tto 
//            \\ je 
//            \\ _
//            \\ nejaky\n 
//        \\  text  // ve viceradkovem retezcovem literalu nelze mit komentar
//        );
//        ifj.write(s1);
//    }
// ~~~~~ Konec souboru: multiline.zig ~~~~~
TEST(LLParserExamples, Multiline) {
    // Otevření testovacího souboru a příprava STDIN
    string path = examPath + "multiline.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    EXPECT_NE(ASTroot, nullptr);

    // Tisk obdrženého stromu a stavu zásobníku rámcu pro vizuální kontrolu
    PRINT_TEST_LOG(Multiline);

    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdin_backup;
    fclose(f);
}

// Testovací soubory jsou z prostorových důvodů umístěny na konci tohoto souboru
TEST(LLParserError, Prologue) {
    for (int i = 1; i <= 12; i++) {
        string filename = "error_prologue_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntErrorPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << path;
        
        FILE* stdin_backup = stdin;
        stdin = f;
        
        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

// Testovací soubory jsou z prostorových důvodů umístěny na konci tohoto souboru
TEST(LLParserError, FunctionDefinition) {
    for (int i = 1; i <= 21; i++) {
        string filename = "error_fun_def_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntErrorPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();
        
        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}

// Testovací soubory jsou z prostorových důvodů umístěny na konci tohoto souboru
TEST(LLParserError, Statements) {
    for (int i = 1; i <= 118; i++) {
        string filename = "error_statement_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = syntErrorPath + filename;
        
        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << "Can't open file: " << filename;
        
        FILE* stdin_backup = stdin;
        stdin = f;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        // Syntaktická analýza programu
        EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;
        
        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdin_backup;
        fclose(f);
    }
}


/*******************************************************************************
 *                                                                             *
 *               TESTOVACÍ SOUBORY "correct_if_condition_XX.zig"               *
 *                                                                             *
 ******************************************************************************/

// ~~~~~ Začátek souboru: correct_if_condition_01.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const float = 3.14;
//        const x: i32 = 32;
//        const y: i32 = float * 89;
//        if ((y < x)) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_01.zig ~~~~~
// ~~~~~ Začátek souboru: correct_if_condition_02.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 32;
//        const y: i32 = 89;
//        if (8 + (y < x)) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_02.zig ~~~~~
// ~~~~~ Začátek souboru: correct_if_condition_03.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const float = 3.14;
//        const x: i32 = 32;
//        const y: i32 = float * 89;
//        if (((y)) < x) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_03.zig ~~~~~
// ~~~~~ Začátek souboru: correct_if_condition_04.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 32;
//        const y: i32 = 89;
//        if ((x)) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_04.zig ~~~~~
// ~~~~~ Začátek souboru: correct_if_condition_05.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 32;
//        const y: i32 = 89;
//        if (x / ((x + y) * 8)) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_05.zig ~~~~~
// ~~~~~ Začátek souboru: correct_if_condition_06.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const x: i32 = 32;
//        const y: i32 = 89;
//        if (ifj.write("hello")) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_06.zig ~~~~~
// ~~~~~ Začátek souboru: correct_if_condition_07.zig ~~~~~
//    const ifj = @import("ifj24.zig");
//    pub fn main() void {
//        const float = 3.14;
//        const x: i32 = 32;
//        const y: i32 = float * 89;
//        if (ifj.i2f(0 - ifj.i2f(0))) {
//        //        ifj.write(1458242);
//        } else {
//        //        ifj.write(53878);
//        }
//    }
// ~~~~~ Konec souboru: correct_if_condition_07.zig ~~~~~


/*******************************************************************************
 *                                                                             *
 *                  TESTOVACÍ SOUBORY "error_prologue_XX.zig"                  *
 *                                                                             *
 ******************************************************************************/


/*******************************************************************************
 *                                                                             *
 *                  TESTOVACÍ SOUBORY "error_fun_def_XX.zig"                   *
 *                                                                             *
 ******************************************************************************/


/*******************************************************************************
 *                                                                             *
 *                 TESTOVACÍ SOUBORY "error_statement_XX.zig"                  *
 *                                                                             *
 ******************************************************************************/


/*** Konec souboru parser_test.cpp ***/