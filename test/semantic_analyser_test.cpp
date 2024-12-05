/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.cpp                                     *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            17.11.2024                                                *
 * Poslední změna:   30.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file semantic_analyser.cpp
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Testy sémantické analýzy
 * @details Tento zdrojový soubor obsahuje unit testy funkcí implementovaných
 *          v souboru `semantic_analyser.c`. Testy jsou vytvořené za využití 
 *          frameworku Google Test. Testy vychází z funkce lexkíláního
 *          analyzátoru a testují nejen sémantickou analýzu, ale i vytváření
 *          abstraktního syntaktického stromu.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

//#define PRINT_FRAME_STACK_OUT 1

#include "ifj24_compiler_test_utils.h"

void TestSemantic(){
    parser_errorWatcher(RESET_ERROR_FLAGS);
    LLparser_parseProgram();
    semantic_analyseProgram();
    exit(0);
}

// ----- Začátek souboru: simplest.zig -----
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//      }
// ----- Konec souboru: simplest.zig -----
TEST(Correct, Simplest){
    string path = semPath + "simplest.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    LLparser_parseProgram();
    
    //Test pro AST
    EXPECT_EQ(ASTroot->type, AST_PROGRAM_NODE);
    char *str1 = DString_DStringtoConstChar(ASTroot->importedFile->identifier);
    EXPECT_STREQ(str1, "ifj");
    free(str1);
    char *str2 = DString_DStringtoConstChar((DString*)ASTroot->importedFile->value);
    EXPECT_STREQ(str2, "ifj24.zig");
    free(str2);
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    EXPECT_EQ(fun->body, nullptr);
    char *str3 = DString_DStringtoConstChar(fun->identifier);
    EXPECT_STREQ(str3, "main");
    free(str3);
    EXPECT_EQ(fun->next, nullptr);
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    EXPECT_EQ(fun->parameters, nullptr);

    semantic_analyseProgram();

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: hello.zig -----
//        // Hello World example in IFJ24
//        // run it on Merlin.fit.vutbr.cz by: zig run hello.zig
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const y : i32 = 24;
//          ifj.write("Hello from IFJ"); // one-parameter function only
//          ifj.write(y);
//          ifj.write("\n");
//      }
// ----- Konec souboru: hello.zig -----
TEST(Correct, Hello){
    string path = examPath + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: example1.zig -----
//      //Program 1: Vypocet faktorialu (iterativne)
//      //Hlavni telo programu
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          ifj.write("Zadejte cislo pro vypocet faktorialu\n");
//          const a = ifj.readi32();
//          if (a) |val| {
//              if (val < 0) {
//                  ifj.write("Faktorial ");
//                  ifj.write(val);
//                  ifj.write(" nelze spocitat\n");
//              } else {
//                  var d: f64 = ifj.i2f(val);
//                  var vysl: f64 = 1.0;
//                  while (d > 0) {
//                      vysl = vysl * d;
//                      d = d - 1.0;
//                  }
//                  ifj.write("Vysledek: ");
//                  ifj.write(vysl);
//                  ifj.write(" = ");
//                  const vysl_i32 = ifj.f2i(vysl);
//                  ifj.write(vysl_i32);
//                  ifj.write("\n");
//              }
//          } else { // a == null
//              ifj.write("Faktorial pro null nelze spocitat\n");
//          }
//      }
// ----- Konec souboru: example1.zig -----
TEST(Correct, Example1){
    string path = examPath + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");
    
    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
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
TEST(Correct, Example2){
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
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
TEST(Correct, Example3){
    string path = examPath + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: fun.zig -----
//      const ifj = @import("ifj24.zig");
//      pub fn f (x : i32) i32    // seznam parametru
//      { //deklarace funkce; v IFJ24 nejsou blokove komentare
//        if(x<10){return x-1;}else{const y = x - 1; // cannot redefine x (shadowing is forbidden)
//          ifj.write("calling g with "); ifj.write(y); ifj.write("\n");
//          const res = g(y);
//          return res;
//        }
//      }
//      pub fn g(x:i32) i32 {
//        if (x > 0) {
//          ifj.write("calling f with "); ifj.write(x); ifj.write("\n"); 
//          const y = f(x); // inicializace konstanty volanim funkce
//          return y; 
//        } else {
//        return 200; 
//        }
//        }
//      pub fn main() void {
//        const res = g(10);
//      ifj.write("res: "); ifj.write(res); ifj.write("\n");
//    }
// ----- Konec souboru: fun.zig -----
TEST(Correct, Fun){
    string path = examPath + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: implicit_to_float.zig -----
//      Korektni program
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const x : f64 = 5 + 10.0;
//          const y : i32 = ifj.f2i(x);
//          ifj.write(y);
//      }
// ----- Konec souboru: implicit_to_float.zig -----
TEST(Correct, Implicit_to_float){
    string path = semPath + "implicit_to_float.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: implicit_to_int.zig -----
//      // Korektni program
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//      const x : i32 = 10.0;
//      const y : f64 = ifj.i2f(x);
//      ifj.write(y);
//      }
// ----- Konec souboru: implicit_to_int.zig -----
TEST(Correct, Implicit_to_int){
    string path = semPath + "implicit_to_int.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: pseudo.zig -----
//      // Korektni program
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const x : i32 = 5;
//          _ = f(x);   // spravne vyuziti pseudopromenne
//      }
//      pub fn f (x : i32) i32 {
//        return x;
//      }
// ----- Konec souboru: pseudo.zig -----
TEST(Correct, Pseudo){
    string path = semPath + "pseudo.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}
// ----- Začátek souboru: pseudo2.zig -----
//      // Korektni program
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          _ = 15 + 20;
//      }
// ----- Konec souboru: pseudo2.zig -----
TEST(Correct, Pseudo2){
    string path = semPath + "pseudo2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}
// ----- Začátek souboru: type_inference.zig -----
//      // Err 8/7
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          const x = foo();
//          ifj.write(x);
//      }
//      pub fn foo() void{
//          ifj.write(5);
//      }
// ----- Konec souboru: type_inference.zig -----
TEST(Correct, Inference){
    string path = semPath + "type_inference_good.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

// ----- Začátek souboru: void_fun_good.zig -----
//      // Korektni program
//      const ifj = @import("ifj24.zig");
//      pub fn main() void {
//          foo();
//      }
//      pub fn foo() void {
//          ifj.write("hello");
//      }
// ----- Konec souboru: void_fun_good.zig -----
TEST(Correct, Void_fun){
    string path = semPath + "void_fun_good.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Void_fun2){
    string path = semPath + "void_fun_good2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, Undefined_Var){
    string path = semPath + "semen_test_1_undefined_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Undefined_Fun){
    string path = semPath + "semen_test_2_undefined_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, ParamCount){
    string path = semPath + "semen_test_3_wrong_number_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ParamType){
    string path = semPath + "semen_test_4_wrong_type_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return){
    string path = semPath + "semen_test_5_incompatible_return_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return2){
    string path = semPath + "semen_test_6_missing_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(6), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return3){
    string path = semPath + "semen_test_7_missing_return_completely.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(6), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Var){
    string path = semPath + "semen_test_8_redefining_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
    
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(5), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Fun){
    string path = semPath + "semen_test_9_redefining_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(5), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ConstAssign){
    string path = semPath + "semen_test_10_assigning_value_to_const.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(5), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType){
    string path = semPath + "semen_test_11_uncompatible_assignment_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(7), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType2){
    string path = semPath + "semen_test_12_uncompatible_assignment_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(7), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_Var){
    string path = semPath + "semen_test_13_unused_variable.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(9), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, MainInt){
    string path = semPath + "main_type_int.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, NoMain){
    string path = semPath + "missing_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Prolog){
    string path = semPath + "prolog_issue.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;
 
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(2), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Prolog2){
    string path = semPath + "prolog_issue2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(2), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Inference){
    string path = semPath + "type_inference.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    
    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Inference2){
    string path = semPath + "type_inference2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    
    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unchanged_var){
    string path = semPath + "unchanged_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(9), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_value){
    string path = semPath + "unused_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(2), "");
    
    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_value2){
    string path = semPath + "unused_value2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Void_fun){
    string path = semPath + "void_fun_bad.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, CantDetermineNullCondType){
    string filename = "cant_determine_nullcond_type.zig";
    string path = semPath + filename;

    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;

    FILE* stdinBackup = stdin;
    stdin = f;

    // Sémantická analýza by měl skončit chybou
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    
    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}

TEST(Correct, TypeCasting){
    string filename = "more_advanced_type_casting.zig";
    string path = semPath + filename;

    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;

    FILE* stdinBackup = stdin;
    stdin = f;

    // Sémantická analýza by měl skončit chybou
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");
    
    // Uvolnění alokovaných zdrojů
    IFJ24Compiler_freeAllAllocatedMemory();

    // Navrácení STDIN do původního stavu a uzavření souboru
    stdin = stdinBackup;
    fclose(f);
}

TEST(SemanticError, Error3_UndefinedVariable){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_3_undef_var_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error3_UndefinedFunction){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_3_undef_fun_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error3_UndefinedIFJFunction){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_3_undef_ifj_fun_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_WrongNumberOfFunParams){
    for (int i = 1; i <= 10; i++) {
        string filename = "error_4_wrong_num_of_fun_params_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_WrongParamType){
    for (int i = 1; i <= 8; i++) {
        string filename = "error_4_wrong_param_type_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_WrongFunReturnType){
    for (int i = 1; i <= 3; i++) {
        string filename = "error_4_wrong_param_type_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error4_ReturnValueDump){
    for (int i = 1; i <= 2; i++) {
        string filename = "error_4_return_value_dump_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error5_VariableRedefinition){
    for (int i = 1; i <= 10; i++) {
        string filename = "error_5_variable_redefinition_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(5), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, Error6_ReturnExp){
    for (int i = 1; i <= 21; i++) {
        string filename = "error_6_return_exp_" + string(i < 10 ? "0" : "") + to_string(i) + ".zig";
        string path = semErrorPath + filename;

        cerr << COLOR_PINK << "TESTING: " << COLOR_RESET << filename << endl;

        FILE* f = fopen(path.c_str(), "r");
        ASSERT_NE(f, nullptr) << COLOR_PINK "Can't open file: " COLOR_RESET << filename;
        
        FILE* stdinBackup = stdin;
        stdin = f;

        // Sémantická analýza by měl skončit chybou
        EXPECT_EXIT(TestSemantic(), ExitedWithCode(6), "");
        
        cerr << COLOR_PINK << "DONE: " << COLOR_RESET << filename << endl << endl;

        // Uvolnění alokovaných zdrojů
        IFJ24Compiler_freeAllAllocatedMemory();

        // Navrácení STDIN do původního stavu a uzavření souboru
        stdin = stdinBackup;
        fclose(f);
    }
}

TEST(SemanticError, DeadCode){
    string path = semPath + "dead_after_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdinBackup = stdin;
    stdin = f;

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(10), "");
    
    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdinBackup;
    fclose(f);
    ASTroot = NULL;
}
