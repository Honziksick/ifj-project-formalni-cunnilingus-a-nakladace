/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           tac_generator.c                                           *
 * Autor:            Lukáš Farkašovský   <xfarkal00>                           *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   25.11.2024                                                *
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

#define RESET_STATIC (AST_NodeType)123

/**
 * @brief Generuje cílový kód programu ze stromu AST.
 */
void TAC_generateProgram() {
    // Vyresetujeme statické proměnné pro více testů
    TAC_resetStatic();

    // Povinná hlavička (prolog)
    printf(".IFJcode24\n");

    // Definice vestavěných funkcí
    built_in_functions();

    // Definice pomocných proměnných pro výpočty
    printf("DEFVAR GF@?tempDEST\n");
    printf("DEFVAR GF@?tempSRC1\n");
    printf("DEFVAR GF@?tempSRC2\n");

    // Voláme funkci main
    printf("CALL $$main\n");
    // Jakmile se vrátíme z funkce main, skočíme na konec programu
    printf("JUMP $$end$$\n");

    // Procházíme seznam funkcí a generujeme kód pro každou funkci
    AST_FunDefNode *node = ASTroot->functionList;
    while(node != NULL) {
        putchar('\n');
        TAC_generateFunctionDefinition(node);
        node = node->next;
    }

    // Konec programu
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

    TAC_generateStatementBlock(funDefNode->body);

    // Tady by šlo přidat podmínku, že se toto přidá jen u void funkce
    // Ostatní funkce by už měly mít, takže toto může být mrtvý kód
    printf("POPFRAME\n");
    printf("RETURN\n");
}  // TAC_generateFunctionDefinition


/**
 * @brief Generuje cílový kód pro blok příkazů
 */
void TAC_generateStatementBlock(AST_StatementNode* statement) {
    while(statement != NULL) {
        switch(statement->statementType) {
            case AST_STATEMENT_VAR_DEF:
                TAC_generateVarDef(statement->statement);
                break;
            case AST_STATEMENT_EXPR:
                TAC_generateExpression(statement->statement);
                break;
            case AST_STATEMENT_FUN_CALL:
                TAC_generateFunctionCall(statement->statement);
                break;
            case AST_STATEMENT_IF:
                TAC_generateIf(statement->statement);
                break;
            case AST_STATEMENT_WHILE:
                TAC_generateWhile(statement->statement);
                break;
            case AST_STATEMENT_RETURN:
                TAC_generateReturn(statement->statement);
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
void TAC_generateBinaryOperator(AST_BinOpNode *bin_node) {
    // Na vrchol datového zásobníku dáme levý operand
    TAC_generateExpression(bin_node->left);
    // Na vrchol datového zásobníku dáme pravý operand
    TAC_generateExpression(bin_node->right);

    // Pomocné proměnné je potřeba definovat mimo switch-case
    Semantic_Data type;     /** Typ operandů (pro dělení) */
    AST_VarNode *var;       /** Uzel proměnné (pro přiřazení) */

    switch (bin_node->op) {
    case AST_OP_ASSIGNMENT:
        var = (AST_VarNode *)bin_node->left->expression;
        if(string_compare_const_str(var->identifier, "_" ) == STRING_EQUAL){
            // Přiřazujeme do pseudoproměnné - zahodíme hodnotu
            printf("POPS GF@?tempDEST\n");
        } else {
            // Definujeme proměnnou
            printf("DEFVAR LF@%s$%lu$\n", var->identifier->str, var->frameID);
            // Nahrajeme hodnotu výrazu do proměnné
            printf("POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
        }
        // Zahodíme hodnotu levé strany ze zásobníku
        printf("POPS GF@?tempDEST\n");
        break;
    case AST_OP_ADD:
        printf("ADDS\n");
        break;
    case AST_OP_SUBTRACT:
        printf("SUBS\n");
        break;
    case AST_OP_MULTIPLY:
        printf("MULS\n");
        break;
    case AST_OP_DIVIDE:
        // Musíme zjistit, zda se jedná o celočíselné nebo desetinné dělení
        if(semantic_analyseExpr(bin_node->right, &type, NULL) != 0) {
            error_handle(ERROR_INTERNAL);
        }

        if(type == SEM_DATA_INT) {
            printf("IDIVS\n");
        } else{
            printf("DIVS\n");
        }
        break;
    case AST_OP_EQUAL:
        printf("EQS\n");
        break;
    case AST_OP_NOT_EQUAL:
        printf("EQS\n");
        printf("NOTS\n");
        break;
    case AST_OP_LESS_THAN:
        printf("LTS\n");
        break;
    case AST_OP_GREATER_THAN:
        printf("GTS\n");
        break;
    case AST_OP_LESS_EQUAL:
        // Musíme udělat dvě porovnání a pak je spojít operátorem OR
        // Abychom operandy mohly porovnávat vícekrát, tak nesmí být na zásobníku
        printf("POPS GF@?tempSRC2\n");
        printf("POPS GF@?tempSRC1\n");
        printf("LT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("EQ GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("ORS\n");
        break;
    case AST_OP_GREATER_EQUAL:
        // Musíme udělat dvě porovnání a pak je spojít operátorem OR
        // Abychom operandy mohly porovnávat vícekrát, tak nesmí být na zásobníku
        printf("POPS GF@?tempSRC2\n");
        printf("POPS GF@?tempSRC1\n");
        printf("GT GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("EQ GF@?tempDEST GF@?tempSRC1 GF@?tempSRC2\n");
        printf("PUSHS GF@?tempDEST\n");
        printf("ORS\n");
        break;

    default:
        error_handle(ERROR_INTERNAL);
    }
}  // TAC_generateBinaryOperator

/**
 * @brief Generuje cílový kód pro definici proměnné
 */
void TAC_generateVarDef(AST_ExprNode *expr_node) {
    AST_BinOpNode *bin_node = expr_node->expression;
    // Na vrchol zásobníku vložíme hodnotu výrazu vpravo
    TAC_generateExpression(bin_node->right);

    // Definujeme proměnnou
    AST_VarNode *var = (AST_VarNode *)bin_node->left->expression;
    printf("DEFVAR LF@%s$%lu$\n", var->identifier->str, var->frameID);
    // Nahrajeme hodnotu výrazu do proměnné
    printf("POPS LF@%s$%lu$\n", var->identifier->str, var->frameID);
}  // TAC_generateVarDef

/**
 * @brief Generuje cílový kód pro výraz
 */
void TAC_generateExpression(AST_ExprNode *expr) {
    AST_VarNode *var = (AST_VarNode*)expr->expression;
    switch (expr->exprType) {
        case AST_EXPR_LITERAL:
            TAC_generateLiteral(var);
            break;
        case AST_EXPR_VARIABLE:
            printf("PUSHS LF@%s$%lu$\n", var->identifier->str, var->frameID);
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
    DString *value = (DString*)literal->value;
    switch (literal->literalType) {
        case AST_LITERAL_INT:
            printf("PUSHS int@%d\n", *(int*)literal->value);
            break;
        case AST_LITERAL_FLOAT:
            printf("PUSHS float@%lf\n", *(double*)literal->value);
            break;
        case AST_LITERAL_STRING:
            value = TAC_convertSpecialSymbols(value);
            printf("PUSHS string@%s\n", value->str);
            string_free(value);
            break;
        case AST_LITERAL_NULL:
            printf("PUSHS nil@nil\n");
            break;
        default:
            error_handle(ERROR_INTERNAL);
    }
}  // TAC_generateLiteral

/**
 * @brief Generuje cílový kód pro podmíněný příkaz if
 */
void TAC_generateIf(AST_IfNode *if_node) {
    // Unikátní identifikátor pro if
    static unsigned int count = 0;

    // Pro reset mezi testy
    if(if_node->type == RESET_STATIC){
        count = 0;
        return;
    }

    // Vyhodnotíme podmínku
    TAC_generateExpression(if_node->condition);
    // je normální nebo null condition?
    if(if_node->nullCondition == NULL) {
        printf("PUSHS bool@true\n");
        printf("JUMPIFNEQS if_else$%d\n", count);
    }else {
        // Výsledek podmínky dáme do proměnné
        printf("POPS GF@?tempSRC1\n");
        printf("JUMPIFEQ if_else$%d GF@?tempSRC1 nil@nil\n", count);
        // Definujeme id_bez_null
        DString *id_bez_null = if_node->nullCondition->identifier;
        printf("DEFVAR LF@%s$%lu$\n", id_bez_null->str, if_node->nullCondition->frameID);
        printf("MOVE LF@%s$%lu$ GF@?tempSRC1\n", id_bez_null->str, if_node->nullCondition->frameID);
    }

    // Generujeme tělo if
    TAC_generateStatementBlock(if_node->thenBranch);
    printf("JUMP if_end$%d\n", count);

    printf("LABEL if_else$%d\n", count);
    // Generujeme tělo else
    TAC_generateStatementBlock(if_node->elseBranch);
    printf("LABEL if_end$%d\n", count);
    count++;
}  // TAC_generateIf

/**
 * @brief Generuje cílový kód pro smyčku while
 */
void TAC_generateWhile(AST_WhileNode *while_node) {
    // Unikátní identifikátor pro while
    static unsigned int count = 0;

    // Pro reset mezi testy
    if(while_node->type == RESET_STATIC){
        count = 0;
        return;
    }

    // Label začátku while
    printf("LABEL while_start$%d\n", count);
    // Vyhodnotíme podmínku
    TAC_generateExpression(while_node->condition);
    // je normální nebo null condition?
    if(while_node->nullCondition == NULL) {
        printf("PUSHS bool@true\n");
        printf("JUMPIFNEQS while_end$%d\n", count);
    }else {
        // Výsledek podmínky dáme do proměnné
        printf("POPS GF@?tempSRC1\n");
        printf("JUMPIFEQ while_end$%d GF@?tempSRC1 nil@nil\n", count);
        // Definujeme id_bez_null
        DString *id_bez_null = while_node->nullCondition->identifier;
        printf("DEFVAR LF@%s$%lu$\n", id_bez_null->str, while_node->nullCondition->frameID);
        printf("MOVE LF@%s$%lu$ GF@?tempSRC1\n", id_bez_null->str, while_node->nullCondition->frameID);
    }

    // Generujeme tělo while
    TAC_generateStatementBlock(while_node->body);
    printf("JUMP while_start$%d\n", count);
    printf("LABEL while_end$%d\n", count);

    count++;
}  // TAC_generateWhile

/**
 * @brief Generuje cílový kód pro návrat z funkce
 */
void TAC_generateReturn(AST_ExprNode *expr) {
    // Na datový zásobník vyhodnotíme výraz
    TAC_generateExpression(expr);
    // Vrátíme se z funkce
    printf("POPFRAME\n");
    printf("RETURN\n");
}  // TAC_generateReturn

void TAC_generateFunctionCall(AST_FunCallNode *funCallNode) {
    // Vytvoříme temporary frame pro parametry funkce
    printf("CREATEFRAME\n");

    // Najdeme definici funkce
    DString *key;
    SymtableItemPtr function;


    // Pokud je funkce vestavěná, tak přidáme prefix ifj.
    if(funCallNode->isBuiltIn){
        key = string_charToDString("ifj.");
        if(key == NULL){
            error_handle(ERROR_INTERNAL);
        }
        for(size_t i = 0; i < funCallNode->identifier->length; i++){
            string_append_char(key, funCallNode->identifier->str[i]);
        }
    }else {
        key = string_init();
        if(key == NULL){
            error_handle(ERROR_INTERNAL);
        }
        if(string_copy(funCallNode->identifier, key) != STRING_SUCCESS){
            error_handle(ERROR_INTERNAL);
        }
    }

    if(symtable_findItem(frameStack.bottom->frame, key, &function) != SYMTABLE_SUCCESS) {
        string_free(key);
        error_handle(ERROR_INTERNAL);
    }

    SymtableFunctionData *functionData = function->data;    /**< Definovaná data funkce */
    AST_ArgOrParamNode *arg = funCallNode->arguments;       /**< Argumenty volání funkce */
    // Pro všechny parametry
    for(size_t i = 0; i < functionData->param_count; i++) {
        // Na zásobník vyhodnotíme hodnotu parametru
        TAC_generateExpression(arg->expression);

        // Vytvoříme instrukci DEFVAR pro parametr
        // Pokud je funkce built-in, tak se nepřidává frameID do názvu
        if(funCallNode->isBuiltIn) {
            printf("DEFVAR TF@%s\n", functionData->params[i].id->str);
            printf("POPS TF@%s\n", functionData->params[i].id->str);
        }else {
            printf("DEFVAR TF@%s$%lu$\n", functionData->params[i].id->str, functionData->body_frameID);
            printf("POPS TF@%s$%lu$\n", functionData->params[i].id->str, functionData->body_frameID);
        }
    }

    // Přidáme skok na návěští funkce
    printf("CALL $$%s\n", key->str);
    string_free(key);
}  // TAC_generateFunctionCall

DString *TAC_convertSpecialSymbols(DString *origin) {
    DString *transformed = string_init();
    if(transformed == NULL) {
        error_handle(ERROR_INTERNAL);
    }

    for(size_t i = 0; i < origin->length; i++) {
        char c = origin->str[i];
        if(isalpha(c)) {
            string_append_char(transformed, c);
        }
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
}

void TAC_resetStatic() {
    AST_IfNode if_node = {.type = RESET_STATIC};
    AST_WhileNode while_node = {.type = RESET_STATIC};
    TAC_generateIf(&if_node);
    TAC_generateWhile(&while_node);
}
