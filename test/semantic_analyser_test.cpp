/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.cpp                                     *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            17.11.2024                                                *
 * Poslední změna:   20.11.2024                                                *
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

#include "ifj24_compiler_test_utils.h"

void TestSemantic(){
    semantic_analyseProgram();
    exit(0);
}

TEST(Correct, Simplest){
    string path = sem_path + "simplest.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;
    
    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    
    //Test pro AST
    EXPECT_EQ(ASTroot->type, AST_PROGRAM_NODE);
    char *str1 = string_toConstChar(ASTroot->importedFile->identifier);
    EXPECT_STREQ(str1, "ifj");
    free(str1);
    char *str2 = string_toConstChar((DString*)ASTroot->importedFile->value);
    EXPECT_STREQ(str2, "ifj24.zig");
    free(str2);
    EXPECT_NE(ASTroot->functionList, nullptr);
    AST_FunDefNode* fun = ASTroot->functionList;
    EXPECT_EQ(fun->body, nullptr);
    char *str3 = string_toConstChar(fun->identifier);
    EXPECT_STREQ(str3, "main");
    free(str3);
    EXPECT_EQ(fun->next, nullptr);
    EXPECT_EQ(fun->returnType, AST_DATA_TYPE_VOID);
    EXPECT_EQ(fun->parameters, nullptr);

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Correct, Hello){
    string path = exam_path + "hello.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example1){
    string path = exam_path + "example1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();

    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example2){
    string path = exam_path + "example2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Example3){
    string path = exam_path + "example3.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Fun){
    string path = exam_path + "fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Implicit_to_float){
    string path = sem_path + "implicit_to_float.zig";
    FILE* f = fopen(path.c_str(), "r");
    if(f == NULL){
        FAIL();
    }
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);

    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Implicit_to_int){
    string path = sem_path + "implicit_to_int.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Pseudo){
    string path = sem_path + "pseudo.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Pseudo2){
    string path = sem_path + "pseudo2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Inference){
    string path = sem_path + "type_inference_good.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Correct, Void_fun){
    string path = sem_path + "void_fun_good.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Correct, Void_fun2){
    string path = sem_path + "void_fun_good2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(0), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, Undefined_Var){
    string path = sem_path + "semen_test_1_undefined_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(3), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Undefined_Fun){
    string path = sem_path + "semen_test_2_undefined_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, ParamCount){
    string path = sem_path + "semen_test_3_wrong_number_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ParamType){
    string path = sem_path + "semen_test_4_wrong_type_of_parameters_in_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return){
    string path = sem_path + "semen_test_5_incompatible_return_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return2){
    string path = sem_path + "semen_test_6_missing_return.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(6), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Return3){
    string path = sem_path + "semen_test_7_missing_return_completely.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(6), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Var){
    string path = sem_path + "semen_test_8_redefining_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();
    
    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(5), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Redefined_Fun){
    string path = sem_path + "semen_test_9_redefining_fun.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(5), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, ConstAssign){
    string path = sem_path + "semen_test_10_assigning_value_to_const.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(5), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType){
    string path = sem_path + "semen_test_11_uncompatible_assignment_1.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(7), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, AssignType2){
    string path = sem_path + "semen_test_12_uncompatible_assignment_2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(7), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_Var){
    string path = sem_path + "semen_test_13_unused_variable.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    LLparser_parseProgram();

    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(9), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}


TEST(Incorrect, MainInt){
    string path = sem_path + "main_type_int.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, NoMain){
    string path = sem_path + "missing_main.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    LLparser_parseProgram();

    // Kořen je inicializován
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(3), "");

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Prolog){
    string path = sem_path + "prolog_issue.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();

    // Syntaktická analýza programu
    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");

    // Kořen je inicializován
    EXPECT_EQ(ASTroot, nullptr);
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Prolog2){
    string path = sem_path + "prolog_issue2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    // Syntaktická analýza programu
    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");

    // Kořen je inicializován
    EXPECT_EQ(ASTroot, nullptr);
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Inference){
    string path = sem_path + "type_inference.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Inference2){
    string path = sem_path + "type_inference2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unchanged_var){
    string path = sem_path + "unchanged_var.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(9), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_value){
    string path = sem_path + "unused_value.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    EXPECT_EXIT(LLparser_parseProgram(), ExitedWithCode(2), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Unused_value2){
    string path = sem_path + "unused_value2.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(4), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, Void_fun){
    string path = sem_path + "void_fun_bad.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

TEST(Incorrect, CantDetermineNullCondType){
    string path = sem_path + "cant_determine_nullcond_type.zig";
    FILE* f = fopen(path.c_str(), "r");
    ASSERT_NE(f, nullptr);
    FILE* stdin_backup = stdin;
    stdin = f;

    frameStack_init();


    LLparser_parseProgram();
    ASSERT_NE(ASTroot, nullptr);
    EXPECT_EXIT(TestSemantic(), ExitedWithCode(8), "");
    

    IFJ24Compiler_freeAllAllocatedMemory();
    stdin = stdin_backup;
    fclose(f);
    ASTroot = NULL;
}

