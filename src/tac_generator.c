/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.c                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                   David Krejčí        <xkrejcd00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   29.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file tac_generator.c
 * @author Krejčí David \<xkrejcd00> (hlavní)
 * @author Farkašovský Lukáš \<xfarkal00> (edit)
 *
 * @brief Implementace funkcí pro generátor vnitřního kódu (3AK + Cílový jazyk).
 * @details Knihovna pro generování cílového kódu z AST.
 */

#include "tac_generator.h"
#include "semantic_analyser.h"
#include "built_in_functions.h"

/**
 * @brief Typ pro reset statických proměnných
 */
#define RESET_STATIC (AST_NodeType)123

/**
 * @brief Generuje cílový kód programu ze stromu AST.
 */
void TAC_generateProgram() {
    // Vyresetujeme statické proměnné pro více testů
    TAC_resetStatic();

    // Povinná hlavička (prolog)
    printf(".IFJcode24\n");

    // Definice pomocných proměnných pro výpočty
    printf("DEFVAR GF@?tempDEST\n");
    printf("DEFVAR GF@?tempSRC1\n");
    printf("DEFVAR GF@?tempSRC2\n");

    // Vytvoříme temporary frame pro main
    printf("CREATEFRAME\n");
    // Voláme funkci main
    printf("CALL $$main\n");
    // Jakmile se vrátíme z funkce main, skočíme na konec programu
    printf("JUMP $$end$$\n");

    // Definice vestavěných funkcí
    TAC_builtInFunctions();

    // Procházíme seznam funkcí a generujeme kód pro každou funkci
    AST_FunDefNode *node = ASTroot->functionList;
    while(node != NULL) {
        putchar('\n');
        TAC_generateFunctionDefinition(node);
        node = node->next;
    }

    // Konec programu
    // Vypíšeme buffer
    TAC_bufferPrint(NULL);
    putchar('\n');
    printf("LABEL $$end$$\n");
    putchar('\n');

}  // TAC_generateProgram

/**
 * @brief Generuje cílový kód definice funkce
 */
void TAC_generateFunctionDefinition(AST_FunDefNode *funDefNode) {
    printf("LABEL $$%s\n", funDefNode->identifier->str);
    printf("PUSHFRAME\n");

    TAC_generateStatementBlock(funDefNode->body, TAC_ALL);

    // Vypíšeme buffer
    TAC_bufferPrint(NULL);
    // Pokud je funkce void, tak jí přidáme return
    if(funDefNode->returnType == AST_DATA_TYPE_VOID) {
        printf("POPFRAME\n");
        printf("RETURN\n");
    }
    
}  // TAC_generateFunctionDefinition


/**
 * @brief Generuje cílový kód pro blok příkazů
 */
void TAC_generateStatementBlock(AST_StatementNode* statement, TAC_MODE mode) {
    // Projdeme všechny příkazy v bloku
    while(statement != NULL) {
        // Pro každý voláme funkci podle typu příkazu
        switch(statement->statementType) {
            case AST_STATEMENT_VAR_DEF:
                TAC_generateVarDef(statement->statement, mode);
                break;
            case AST_STATEMENT_EXPR:
                if(mode != TAC_VAR_DEF_ONLY) {
                    TAC_generateExpression(statement->statement);
                }
                break;
            case AST_STATEMENT_FUN_CALL:
                if(mode != TAC_VAR_DEF_ONLY) {
                    TAC_generateFunctionCall(statement->statement);
                }
                break;
            case AST_STATEMENT_IF:
                TAC_generateIf(statement->statement, mode);
                break;
            case AST_STATEMENT_WHILE:
                TAC_generateWhile(statement->statement, mode);
                break;
            case AST_STATEMENT_RETURN:
                if(mode != TAC_VAR_DEF_ONLY) {
                    TAC_generateReturn(statement->statement);
                }
                // Vracíme se z funkce
                return;
            default:
                error_handle(ERROR_INTERNAL);
        }
        // Přesuneme se na další příkaz
        statement = statement->next;
    }
}  // TAC_generateStatementBlock

/**
 * @brief Generuje cílový kód pro binární operace
 */
void TAC_generateBinaryOperator(AST_BinOpNode *binNode) {
    // Pokud operace není přiřazení, tak na vrchol datového zásobníku dáme levý operand
    if(binNode->op != AST_OP_ASSIGNMENT) {
        TAC_generateExpression(binNode->left);
    }
    // Na vrchol datového zásobníku dáme pravý operand
    TAC_generateExpression(binNode->right);

    // Kontrola NULL
    if(binNode == NULL) {
        return;
    }

    // Pomocné proměnné je potřeba definovat mimo switch-case
    Semantic_Data type = SEM_DATA_UNKNOWN;     /** Typ operandů (pro dělení) */
    AST_VarNode *var = NULL;       /** Uzel proměnné (pro přiřazení) */

    // Vytvoříme buffer pro instrukce
    char buffer[OPTIMIZE_BUFFER_SIZE] = {0};
    int writtenSize = 0;
    switch (binNode->op) {
        case AST_OP_ASSIGNMENT:
            // Kontrola NULL
            if(binNode->left->expression == NULL) {
                return;
            }

            var = (AST_VarNode *)binNode->left->expression;
            if(string_compare_const_str(var->identifier, "_" ) == STRING_EQUAL) {
                // Přiřazujeme do pseudoproměnné - zahodíme hodnotu
                //printf("POPS GF@?tempDEST\n");
                snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS GF@?tempSRC1\n");
                TAC_bufferPrint(&buffer);
            } else {
                // Nahrajeme hodnotu výrazu do proměnné
                //printf("POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
                writtenSize = snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
                // Pokud nám nestačí velikost bufferu
                if(writtenSize >= OPTIMIZE_BUFFER_SIZE) {
                    // Vypíšeme buffer
                    TAC_bufferPrint(NULL);
                    // Vypíšeme instukci přímo
                    printf("POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
                }
                TAC_bufferPrint(&buffer);
            }
            break;
            
        case AST_OP_ADD:
            TAC_bufferPrint(NULL);
            printf("ADDS\n");
            break;
        case AST_OP_SUBTRACT:
            TAC_bufferPrint(NULL);
            printf("SUBS\n");
            break;
        case AST_OP_MULTIPLY:
            TAC_bufferPrint(NULL);
            printf("MULS\n");
            break;
        case AST_OP_DIVIDE:
            // Musíme zjistit, zda se jedná o celočíselné nebo desetinné dělení
            if(semantic_analyseExpr(binNode->right, &type, NULL) != 0) {
                error_handle(ERROR_INTERNAL);
            }

            TAC_bufferPrint(NULL);
            if(type == SEM_DATA_INT) {
                printf("IDIVS\n");
            } else {
                printf("DIVS\n");
            }
            break;
        case AST_OP_EQUAL:
            TAC_bufferPrint(NULL);
            printf("EQS\n");
            break;
        case AST_OP_NOT_EQUAL:
            TAC_bufferPrint(NULL);
            printf("EQS\n");
            printf("NOTS\n");
            break;
        case AST_OP_LESS_THAN:
            TAC_bufferPrint(NULL);
            printf("LTS\n");
            break;
        case AST_OP_GREATER_THAN:
            TAC_bufferPrint(NULL);
            printf("GTS\n");
            break;
        case AST_OP_LESS_EQUAL:
            TAC_bufferPrint(NULL);
            printf("GTS\n");
            printf("NOTS\n");
            break;
        case AST_OP_GREATER_EQUAL:
            TAC_bufferPrint(NULL);
            printf("LTS\n");
            printf("NOTS\n");
            break;

        default:
            error_handle(ERROR_INTERNAL);
    }
    
}  // TAC_generateBinaryOperator

/**
 * @brief Generuje cílový kód pro definici proměnné
 */
void TAC_generateVarDef(AST_ExprNode *exprNode, TAC_MODE mode) {
    // Kontrola NULL
    if(exprNode == NULL || exprNode->expression == NULL) {
        return;
    }

    AST_BinOpNode *binNode = exprNode->expression;

    // Kontrola NULL
    if(binNode->left == NULL || binNode->left->expression == NULL) {
        return;
    }

    // Definujeme proměnnou
    AST_VarNode *var = (AST_VarNode *)binNode->left->expression;

    if(mode != TAC_EXCEPT_VAR_DEF) {
        TAC_bufferPrint(NULL);
        printf("DEFVAR LF@%s$%lu$ \n", var->identifier->str, var->frameID);
    }
    if(mode != TAC_VAR_DEF_ONLY) {
        // Na vrchol zásobníku vložíme hodnotu výrazu vpravo
        TAC_generateExpression(binNode->right);
        // Nahrajeme hodnotu výrazu do proměnné
        //printf("POPS LF@%s$%lu$ \n", var->identifier->str, var->frameID);
        // Vytvoříme buffer pro instrukce
        char buffer[OPTIMIZE_BUFFER_SIZE] = {0};
        int writtenSize = 0;
        writtenSize = snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS LF@%s$%lu$ \n", var->identifier->str, var->frameID);
        if(writtenSize >= OPTIMIZE_BUFFER_SIZE) {
            // Vypíšeme buffer
            TAC_bufferPrint(NULL);
            // Vypíšeme instukci přímo
            printf("POPS LF@%s$%lu$ \n", var->identifier->str, var->frameID);
        }
        TAC_bufferPrint(&buffer);
    }

}  // TAC_generateVarDef

/**
 * @brief Generuje cílový kód pro výraz
 */
void TAC_generateExpression(AST_ExprNode *expr) {
    // Kontrola NULL
    if(expr == NULL) {
        return;
    }

    AST_VarNode *var = (AST_VarNode*)expr->expression;
    // Vytvoříme buffer pro instrukce
    char buffer[OPTIMIZE_BUFFER_SIZE] = {0};
    int writtenSize = 0;
    // Podle typu výrazu voláme další funkce
    switch (expr->exprType) {
        case AST_EXPR_LITERAL:
            TAC_generateLiteral(var);
            break;
        case AST_EXPR_VARIABLE:
            // Pokud je proměnná, tak se její hodnota rovnou nahraje na vrchol zásobníku
            //printf("PUSHS LF@%s$%lu$\n", var->identifier->str, var->frameID);
            writtenSize = snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS LF@%s$%lu$\n", var->identifier->str, var->frameID);
            if(writtenSize >= OPTIMIZE_BUFFER_SIZE) {
                // Vypíšeme buffer
                TAC_bufferPrint(NULL);
                // Vypíšeme instukci přímo
                printf("PUSHS LF@%s$%lu$\n", var->identifier->str, var->frameID);
            }
            TAC_bufferPrint(&buffer);
            break;
        case AST_EXPR_BINARY_OP:
            TAC_generateBinaryOperator(expr->expression);
            break;
        case AST_EXPR_FUN_CALL:
            TAC_generateFunctionCall(expr->expression);
            break;
        default:

            error_handle(ERROR_INTERNAL);
    }
}  // TAC_generateExpression

/**
 * @brief Generuje cílový kód pro literál
 */
void TAC_generateLiteral(AST_VarNode *literal) {
    // Kontrola NULL
    if(literal == NULL) {
        return;
    }

    DString *value = NULL;
    // Vytvoříme buffer pro instrukce
    char buffer[OPTIMIZE_BUFFER_SIZE] = {0};
    // Podle typu literálu na vrchol zásobníku dáme hodnotu
    switch (literal->literalType) {
        case AST_LITERAL_INT:
            //printf("PUSHS int@%d\n", *(int*)literal->value);
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS int@%d \n", *(int*)literal->value);
            break;
        case AST_LITERAL_FLOAT:
            //printf("PUSHS float@%a\n", *(double*)literal->value);
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS float@%a \n", *(double*)literal->value);
            break;
        case AST_LITERAL_STRING:
            value = (DString*)literal->value;
            value = TAC_convertSpecialSymbols(value);
            //printf("PUSHS string@%s\n", value->str);
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS string@%s \n", value->str);
            string_free(value);
            break;
        case AST_LITERAL_NULL:
            //printf("PUSHS nil@nil\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS nil@nil \n");
            break;
        case AST_LITERAL_BOOL:
            if(*(bool*)literal->value) {
                //printf("PUSHS bool@true\n");
                snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS bool@true \n");
            } else {
                //printf("PUSHS bool@false\n");
                snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS bool@false \n");
            }
            break;
        default:
            error_handle(ERROR_INTERNAL);
    }
    TAC_bufferPrint(&buffer);
}  // TAC_generateLiteral

/**
 * @brief Generuje cílový kód pro podmíněný příkaz if
 */
void TAC_generateIf(AST_IfNode *ifNode, TAC_MODE mode) {
    // Kontrola NULL
    if(ifNode == NULL) {
        return;
    }

    // Unikátní identifikátor pro if
    static unsigned int count = 0;
    unsigned int id = count;
    count++;

    // Pro reset mezi testy
    if(ifNode->type == RESET_STATIC) {
        count = 0;
        return;
    }

    // Pokud pouze generujeme definice proměnných
    if(mode == TAC_VAR_DEF_ONLY) {
        // Pokud má NULL prodmínku, tak definujeme idWithoutNull
        if(ifNode->nullCondition != NULL) {
            DString *idWithoutNull = ifNode->nullCondition->identifier;
            TAC_bufferPrint(NULL);
            printf("DEFVAR LF@%s$%lu$\n", idWithoutNull->str, ifNode->nullCondition->frameID);
        }
        // Definujeme proměnné v then a else větvích
        TAC_generateStatementBlock(ifNode->thenBranch, TAC_VAR_DEF_ONLY);
        TAC_generateStatementBlock(ifNode->elseBranch, TAC_VAR_DEF_ONLY);
        return;
    }

    // Vyhodnotíme podmínku
    printf("# if_%d\n", id);
    TAC_generateExpression(ifNode->condition);
    printf("# then_%d\n", id);
    // je bool nebo null podmínka?
    if(ifNode->nullCondition == NULL) {
        TAC_bufferPrint(NULL);
        printf("PUSHS bool@true\n");
        printf("JUMPIFNEQS if_else$%d\n", id);
    }
    else {
        // Výsledek podmínky dáme do proměnné
        char buffer[OPTIMIZE_BUFFER_SIZE] = "POPS GF@?tempSRC1\n";
        TAC_bufferPrint(&buffer);
        TAC_bufferPrint(NULL);
        printf("JUMPIFEQ if_else$%d GF@?tempSRC1 nil@nil\n", id);
        // Definujeme idWithoutNull
        DString *idWithoutNull = ifNode->nullCondition->identifier;
        if(mode != TAC_EXCEPT_VAR_DEF) {
            printf("DEFVAR LF@%s$%lu$\n", idWithoutNull->str, ifNode->nullCondition->frameID);
        }
        printf("MOVE LF@%s$%lu$ GF@?tempSRC1\n", idWithoutNull->str, ifNode->nullCondition->frameID);
    }

    // Generujeme tělo if
    TAC_generateStatementBlock(ifNode->thenBranch, mode);
    TAC_bufferPrint(NULL);
    printf("JUMP if_end$%d\n", id);

    // Generujeme tělo else
    printf("LABEL if_else$%d\n", id);
    TAC_generateStatementBlock(ifNode->elseBranch, mode);
    TAC_bufferPrint(NULL);
    printf("LABEL if_end$%d\n", id);

}  // TAC_generateIf

/**
 * @brief Generuje cílový kód pro smyčku while
 */
void TAC_generateWhile(AST_WhileNode *whileNode, TAC_MODE mode) {
    // Unikátní identifikátor pro while
    static unsigned int count = 0;
    unsigned int id = count;

    count++;

    // Pro reset mezi testy
    if(whileNode->type == RESET_STATIC) {
        count = 0;
        return;
    }
    TAC_bufferPrint(NULL);

    // Pokud pouze generujeme definice proměnných
    if(mode == TAC_VAR_DEF_ONLY) {
        // Definujeme NULL podmínku pokud existuje
        if(whileNode->nullCondition != NULL) {
            DString *idWithoutNull = whileNode->nullCondition->identifier;
            printf("DEFVAR LF@%s$%lu$\n", idWithoutNull->str, whileNode->nullCondition->frameID);
        }
        // Definujeme proměnné v těle while
        TAC_generateStatementBlock(whileNode->body, TAC_VAR_DEF_ONLY);
        return;
    }
    // Pokud máme NULL podmínku, tak nejdříve definujeme idWithoutNull
    DString *idWithoutNull = NULL;
    if(whileNode->nullCondition != NULL && mode != TAC_EXCEPT_VAR_DEF) {
        idWithoutNull = whileNode->nullCondition->identifier;
        printf("DEFVAR LF@%s$%lu$\n", idWithoutNull->str, whileNode->nullCondition->frameID);
    }
    // Definujeme proměnné těla
    if(mode == TAC_ALL) {
        printf("# Definice vsech promennych v tele while\n");
        TAC_generateStatementBlock(whileNode->body, TAC_VAR_DEF_ONLY);
    }
    // Návěští začátku while
    printf("LABEL while_start$%d\n", id);
    // Vyhodnotíme podmínku
    TAC_generateExpression(whileNode->condition);
    printf("# while_body_%d\n", id);
    // Je pravdivostní nebo null podmínka?
    if(whileNode->nullCondition == NULL) {
        printf("PUSHS bool@true\n");
        printf("JUMPIFNEQS while_end$%d\n", id);
    }
    else {
        // Výsledek podmínky dáme do proměnné
        char buffer[OPTIMIZE_BUFFER_SIZE] = "POPS GF@?tempSRC1\n";
        TAC_bufferPrint(&buffer);
        TAC_bufferPrint(NULL);
        printf("JUMPIFEQ while_end$%d GF@?tempSRC1 nil@nil\n", id);
        // Přesuneme hodnotu do idWithoutNull

        printf("MOVE LF@%s$%lu$ GF@?tempSRC1\n", idWithoutNull->str, whileNode->nullCondition->frameID);
    }

    // Generujeme tělo while
    TAC_generateStatementBlock(whileNode->body, TAC_EXCEPT_VAR_DEF);
    TAC_bufferPrint(NULL);
    // Skočíme na začátek cyklu
    printf("JUMP while_start$%d\n", id);
    printf("LABEL while_end$%d\n", id);

}  // TAC_generateWhile

/**
 * @brief Generuje cílový kód pro návrat z funkce
 */
void TAC_generateReturn(AST_ExprNode *expr) {
    // Na datový zásobník vyhodnotíme výraz
    TAC_generateExpression(expr);
    TAC_bufferPrint(NULL);
    // Vrátíme se z funkce
    printf("POPFRAME\n");
    printf("RETURN\n");
}  // TAC_generateReturn

void TAC_generateFunctionCall(AST_FunCallNode *funCallNode) {
    TAC_bufferPrint(NULL);
    // Vytvoříme buffer pro instrukce
    char buffer[OPTIMIZE_BUFFER_SIZE] = {0};
    int writtenSize = 0;
    
    if(funCallNode->isBuiltIn) {
        if(string_compare_const_str(funCallNode->identifier, "readstr") == STRING_EQUAL) {
            // Načteme řetězec a výsledek nahrajeme na zásobník
            printf("READ GF@?tempSRC1 string\n");
            //printf("PUSHS GF@?tempSRC1\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS GF@?tempSRC1\n");
            TAC_bufferPrint(&buffer);
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "readi32") == STRING_EQUAL) {
            // Načteme číslo a výsledek nahrajeme na zásobník
            printf("READ GF@?tempSRC1 int\n");
            //printf("PUSHS GF@?tempSRC1\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS GF@?tempSRC1\n");
            TAC_bufferPrint(&buffer);
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "readf64") == STRING_EQUAL) {
            // Načteme číslo a výsledek nahrajeme na zásobník
            printf("READ GF@?tempSRC1 float\n");
            //printf("PUSHS GF@?tempSRC1\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "PUSHS GF@?tempSRC1\n");
            TAC_bufferPrint(&buffer);
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "write") == STRING_EQUAL) {
            AST_ArgOrParamNode *arg = funCallNode->arguments;       /**< Argument volání funkce */
            // Vyhodnotíme parametr
            TAC_generateExpression(arg->expression);
            //printf("POPS GF@?tempSRC1\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS GF@?tempSRC1\n");
            TAC_bufferPrint(&buffer);
            TAC_bufferPrint(NULL);
            printf("WRITE GF@?tempSRC1\n");
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "i2f") == STRING_EQUAL) {
            // Vyhodnotíme parametr
            TAC_generateExpression(funCallNode->arguments->expression);
            TAC_bufferPrint(NULL);
            printf("INT2FLOATS\n");
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "f2i") == STRING_EQUAL) {
            // Vyhodnotíme parametr
            TAC_generateExpression(funCallNode->arguments->expression);
            TAC_bufferPrint(NULL);
            printf("FLOAT2INTS\n");
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "string") == STRING_EQUAL) {
            TAC_generateExpression(funCallNode->arguments->expression);
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "length") == STRING_EQUAL) {
            // Vyhodnotíme parametr a nahrajeme do pomocné proměnné
            TAC_generateExpression(funCallNode->arguments->expression);
            //printf("POPS GF@?tempSRC1\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS GF@?tempSRC1\n");
            TAC_bufferPrint(&buffer);
            TAC_bufferPrint(NULL);
            // Zjistíme délku řetězce a výsledek nahrajeme na zásobník
            printf("STRLEN GF@?tempDEST GF@?tempSRC1\n");
            printf("PUSHS GF@?tempDEST\n");
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "concat") == STRING_EQUAL) {
            // Vyhodnotíme oba parametry a nahrajeme do pomocných proměnných
            TAC_generateExpression(funCallNode->arguments->expression);
            TAC_generateExpression(funCallNode->arguments->next->expression);
            //printf("POPS GF@?tempSRC2\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS GF@?tempSRC2\n");
            TAC_bufferPrint(&buffer);
            //printf("POPS GF@?tempSRC1\n");
            snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS GF@?tempSRC1\n");
            TAC_bufferPrint(&buffer);
            TAC_bufferPrint(NULL);
            // Spojíme oba řetězce a výsledek nahrajeme na zásobník
            printf("CONCAT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
            printf("PUSHS GF@?tempDEST\n");
            return;
        }
        else if(string_compare_const_str(funCallNode->identifier, "chr") == STRING_EQUAL) {
            // Vyhodnotíme parametr a nahrajeme do pomocné proměnné
            TAC_generateExpression(funCallNode->arguments->expression);
            TAC_bufferPrint(NULL);
            printf("INT2CHARS\n");
            return;
        }
    }

    // Vytvoříme temporary frame pro parametry funkce
    printf("CREATEFRAME\n");

    // Najdeme definici funkce
    DString *key = NULL;
    SymtableItemPtr function = NULL;

    // Pokud je funkce vestavěná, tak přidáme prefix ifj.
    if(funCallNode->isBuiltIn) {
        key = string_charToDString("ifj.");
        if(key == NULL) {
            error_handle(ERROR_INTERNAL);
        }
        for(size_t i = 0; i < funCallNode->identifier->length; i++) {
            string_append_char(key, funCallNode->identifier->str[i]);
        }
    }
    // Jinak jen překopírujeme identifikátor
    else {
        key = string_init();
        if(key == NULL) {
            error_handle(ERROR_INTERNAL);
        }
        if(string_copy(funCallNode->identifier, key) != STRING_SUCCESS) {
            error_handle(ERROR_INTERNAL);
        }
    }

    // Najdeme definici funkce
    if(symtable_findItem(frameStack.bottom->frame, key, &function) != SYMTABLE_SUCCESS) {
        string_free(key);
        error_handle(ERROR_INTERNAL);
    }

    SymtableFunctionData *functionData = function->data;    /**< Definovaná data funkce */
    AST_ArgOrParamNode *arg = funCallNode->arguments;       /**< Argumenty volání funkce */
    // Pro všechny parametry
    for(size_t i = 0; i < functionData->paramCount; i++) {
        char *paramId = functionData->params[i].id->str;
        if(funCallNode->isBuiltIn) {
            // Built-in funkce nemají rámcový suffix
            printf("DEFVAR TF@%s\n", paramId);
        }
        else {
            printf("DEFVAR TF@%s$%lu$\n", paramId, functionData->bodyFrameID);
        }
        // Na zásobník vyhodnotíme hodnotu parametru
        TAC_generateExpression(arg->expression);
        // Vytvoříme instrukci DEFVAR pro parametr
        // Pokud je funkce built-in, tak se nepřidává frameID do názvu
        if(funCallNode->isBuiltIn) {
            // Built-in funkce nemají rámcový suffix
            //printf("POPS TF@%s\n", functionData->params[i].id->str);
            writtenSize = snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS TF@%s\n", paramId);
            if(writtenSize >= OPTIMIZE_BUFFER_SIZE) {
                // Vypíšeme buffer
                TAC_bufferPrint(NULL);
                // Vypíšeme instukci přímo
                printf("POPS TF@%s\n", paramId);
            }
        }
        else {
            //printf("POPS TF@%s$%lu$\n", functionData->params[i].id->str, functionData->bodyFrameID);
            writtenSize = snprintf(buffer, OPTIMIZE_BUFFER_SIZE, "POPS TF@%s$%lu$\n", paramId, functionData->bodyFrameID);
            if(writtenSize >= OPTIMIZE_BUFFER_SIZE) {
                // Vypíšeme buffer
                TAC_bufferPrint(NULL);
                // Vypíšeme instukci přímo
                printf("POPS TF@%s$%lu$\n", paramId, functionData->bodyFrameID);
            }
        }
        TAC_bufferPrint(&buffer);
        TAC_bufferPrint(NULL);
        arg = arg->next;
    }

    // Přidáme skok na návěští funkce
    if(funCallNode->isBuiltIn) {
        printf("CALL $$ifj$%s\n", funCallNode->identifier->str);
    }
    else {
        printf("CALL $$%s\n", key->str);
    }
    string_free(key);

}  // TAC_generateFunctionCall

/**
 * @brief Převede speciální znaky na escape sekvence
 */
DString *TAC_convertSpecialSymbols(DString *origin) {
    // Vytvoříme nový řetězec pro výsledek
    DString *transformed = string_init();
    if(transformed == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    // Pro každý znak v originálním řetězci
    for(size_t i = 0; i < origin->length; i++) {
        char c = origin->str[i];
        // Pokud je znak alfanumerický, tak ho přidáme do výsledku
        if(isalnum(c)) {
            string_append_char(transformed, c);
        }
        // Pokud je znak speciální, tak ho převedeme na escape sekvenci
        else {
            char buffer[MAX_BUFFER_SIZE];     // 5 = lomítko + 3 číslice + '\0'
            snprintf(buffer, sizeof(buffer), "\\%03d", (unsigned char)c);
            for(int j = 0; j < MAX_BUFFER_SIZE-1; j++) {
                string_append_char(transformed, buffer[j]);
            }
        }
    }
    string_append_char(transformed, '\0');

    return transformed;
}  // TAC_convertSpecialSymbols

/**
 * @brief Resetuje statické proměnné v případě více testů v jednom spuštění
 */
void TAC_resetStatic() {
    AST_IfNode ifNode = {.type = RESET_STATIC};
    AST_WhileNode whileNode = {.type = RESET_STATIC};
    TAC_generateIf(&ifNode, TAC_ALL);
    TAC_generateWhile(&whileNode, TAC_ALL);
}  // TAC_resetStatic

void TAC_bufferPrint(char (*newInstruction)[OPTIMIZE_BUFFER_SIZE]) {

    static char buffered[OPTIMIZE_BUFFER_SIZE] = {'\0'};

    // Pokud jsme dostali NULL, tak vypíšeme buffer
    if(newInstruction == NULL) {
        if(*buffered != '\0') {
            //printf("#buf flush\n");
            printf("%s", buffered);
            *buffered = '\0';
        }
        return;
    }

    // Pokud zatím nemáme uloženou instrukci, tak ji uložíme
    if(*buffered == '\0') {
        strcpy(buffered, *newInstruction);
        return;
    }
    // Pokud jsme dostali nový řetězec, tak se podíváme, zda jde optimalizovat
    //První intrukce je push a druhá pop
    if(strncmp(buffered, "PUSHS", 5) == 0 && strncmp(*newInstruction, "POPS", 4) == 0) {
        // Zjistíme, zda se jedná o stejnou proměnnou
        if(strcmp(buffered + 6, *newInstruction + 5) == 0) {
            // Můžeme odstranit zbytečné instukce
            *buffered = '\0';
            return;
        }

        // Pokud se nejedná o stejnou proměnnou, tak nahradíme instukcí move
        printf("MOVE ");
        char *ptr = *newInstruction + 5;
        while(*ptr != '\0' && *ptr != '\n') {
            putchar(*ptr);
            ptr++;
        }
        putchar(' ');
        ptr = buffered + 6;
        while(*ptr != '\0' && *ptr != '\n') {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
        *buffered = '\0';
    }
    else {
        // Pokud se nejedná o push a pop, tak vypíšeme buffer
        printf("%s", buffered);
        strcpy(buffered, *newInstruction);
    }

}  // TAC_bufferPrint