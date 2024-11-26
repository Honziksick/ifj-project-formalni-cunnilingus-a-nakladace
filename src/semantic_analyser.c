/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.c                                       *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   18.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file semantic_analyser.c
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Implementace funkcí pro sémantický analyzátor.
 * @details Implementace funkcí z hlavičkového souboru semantic_analyser.h
 */


#include "semantic_analyser.h"
#include <math.h>


/**
 * @brief Provede sémantickou analýzu celého programu.
 */
void semantic_analyseProgram() {
    ErrorType error = semantic_analyseProgramStructure();
    if(error != 0) {
        error_handle(error);
    }

    error = semantic_analyseFunctionDefinitions();
    if(error != 0) {
        error_handle(error);
    }

    error = semantic_analyseVariables();
    if(error != 0) {
        error_handle(error);
    }
}

/**
 * @brief Provede sémantickou analýzu základní struktury programu.
 */
ErrorType semantic_analyseProgramStructure() {
    // Vytvoříme string main
    DString *str = string_charToDString("main");
    if(str == NULL) {
        return ERROR_INTERNAL;
    }

    SymtableItemPtr item_main;
    // Podíváme se na globální rámec, kde jsou definice funkcí
    symtable_result result = symtable_findItem(frameStack.bottom->frame, str, &item_main);

    // Uvolníme string pro hledání
    string_free(str);

    // Pokud se nepodařilo najít main
    if(result != SYMTABLE_SUCCESS) {
        return ERROR_SEM_UNDEF;
    }

    // Pokud jsme našli main, ověříme, že je funkce
    if(item_main->symbol_state != SYMTABLE_SYMBOL_FUNCTION) {
        // Jestli se toto stane, tak je to chyba v implementaci
        return ERROR_SEM_OTHER;
    }

    // Zkontrolujeme, že má data
    if(item_main->data == NULL) {
        // Jestli se toto stane, tak je to chyba v implementaci
        return ERROR_SEM_OTHER;
    }

    // Převedeme její data
    SymtableFunctionData *data = (SymtableFunctionData *)item_main->data;
    // Zkontrolujeme, že má správný počet parametrů a návratový typ
    if(data->param_count != 0 || data->return_type != SYMTABLE_TYPE_VOID) {
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Zkontrolujeme prolog programu, že má správné literály
    if(string_compare_const_str(ASTroot->importedFile->identifier, "ifj") != STRING_EQUAL) {
        // Toto nelze jednoduše zařadit mezi jiné chyby, proto ERROR_SEM_OTHER
        return ERROR_SEM_OTHER;
    }
    str = ASTroot->importedFile->value;
    if(string_compare_const_str(str, "ifj24.zig") != STRING_EQUAL) {
        // Toto nelze jednoduše zařadit mezi jiné chyby, proto ERROR_SEM_OTHER
        return ERROR_SEM_OTHER;
    }

    // Vše je v pořádku
    return 0;

}

/**
 * @brief Provede sémantickou analýzu definic funkcí
 */
ErrorType semantic_analyseFunctionDefinitions() {
    AST_FunDefNode *node = ASTroot->functionList;
    while(node != NULL) {
        ErrorType error = semantic_probeFunction(node);
        if(error != 0) {
            return error;
        }
        node = node->next;
    }

    return 0;
}

/**
 * @brief Provede sémantickou analýzu všech proměnných v programu
 */
ErrorType semantic_analyseVariables() {
    // Projdeme všechny rámce
    for(size_t i = 1; i <= frameStack.currentID; i++) {
        // Z rámce vezmeme tabulku symbolů
        SymtablePtr table = frameArray.array[i]->frame;
        SymtableItem item;
        // Pro každý index tabulky symbolů
        for(size_t j = 0; j < table->allocated_size; j++) {
            item = table->array[j];

            // Pokud na indexu není položka, tak přejdeme na další index
            if(item.symbol_state == SYMTABLE_SYMBOL_EMPTY) {
                continue;
            }

            if(item.symbol_state == SYMTABLE_SYMBOL_FUNCTION) {
                // Nemělo by se nikdy stát, jinak je chyba v implementaci
                // Funkce by měly být definovány pouze v globálním rámci
                return ERROR_SEM_OTHER;
            }

            if(item.symbol_state == SYMTABLE_SYMBOL_DEAD) {
                // Nemělo by se nikdy stát, jinak je chyba v implementaci
                // Položky by se měly mazat jen na konci běhu prohramu
                return ERROR_SEM_OTHER;
            }

            // Zkontrolujeme, že je proměnná využita
            if(item.used == false) {
                return ERROR_SEM_UNUSED_VAR;
            }

            // Pokud je položka konstantní, tak zkontrolujeme, že se nemění
            if(item.constant == true) {
                if(item.changed == true) {
                    return ERROR_SEM_REDEF_OR_CONSTDEF;
                }
            }else {  // Pokud není konstantní, tak zkontrolujeme, že se změní
                if(item.changed == false) {
                    return ERROR_SEM_UNUSED_VAR;
                }
            }

        }
    }

    // Zkontrolujeme, že proměnná ifj v globálním rámci není použita
    SymtablePtr table = frameStack.bottom->frame;
    SymtableItemPtr item;
    DString *key = string_charToDString("ifj");
    if(symtable_findItem(table, key, &item) != SYMTABLE_SUCCESS){
        string_free(key);
        return ERROR_INTERNAL;
    }
    if(item->changed == true || item->used == true){
        string_free(key);
        return ERROR_SEM_OTHER;
    }
    string_free(key);

    // Pokud jsme prošli všechny rámce bez vrácení, tak je vše v pořádku
    return 0;
}

/**
 * @brief Provede sémantickou analýzu bloku funkce
 */
ErrorType semantic_probeFunction(AST_FunDefNode *FunDefNode) {
    bool reachedReturn = false;
    Semantic_Data return_type = semantic_ASTToSemType(FunDefNode->returnType);
    ErrorType probeResult = semantic_probeBlock(return_type, FunDefNode->body, &reachedReturn);

    // Pokud jsme našli sémantickou chybu, tak ji vrátíme
    if(probeResult != 0) {
        return probeResult;
    }

    // Pokud jsme došli na konec funkce bez příkazu return, tak zkontrolujeme,
    // že funkce je void
    if(reachedReturn == false) {

        // Pokud funkce není void, tak jsme došli na konec funkce bez návratu
        if(return_type != SEM_DATA_VOID) {
            return ERROR_SEM_RETURN_EXP;
        }
    }

    // Vše proběhlo v pořádku
    return 0;
}

/**
 * @brief Provede sémantickou analýzu bloku příkazů
 */
ErrorType semantic_probeBlock(Semantic_Data fun_return,      \
                              AST_StatementNode *statement, bool* returned) {
    ErrorType result = 0;
    Semantic_Data type;

    AST_ExprNode *expr;     /**< Pouze pro přiřazení */
    AST_BinOpNode bin_op;   /**< Pouze pro přiřazení */

    // Procházíme všechny příkazy těla funkce
    while(statement != NULL) {
        switch(statement->statementType) {
            // Pokud typ příkazu není definován, tak je nejspíš chyba v implementaci
            case(AST_STATEMENT_NOT_DEFINED):
                return ERROR_SEM_OTHER;

            case(AST_STATEMENT_VAR_DEF):
                result = semantic_analyseVarDef(statement);
                break;

            case(AST_STATEMENT_EXPR):
                // Pokud je přiřazení, tak je ok, jinak je zahození hodnoty
                expr = (AST_ExprNode*)statement->statement;
                if(expr->exprType == AST_EXPR_BINARY_OP){
                    bin_op = *(AST_BinOpNode*)expr->expression;
                    if(bin_op.op == AST_OP_ASSIGNMENT){
                        result = semantic_analyseBinOp(expr, &type, NULL);
                        if(result != 0){
                            return result;
                        }
                        break;
                    }
                }
                result = ERROR_SEM_OTHER;
                break;

            case(AST_STATEMENT_FUN_CALL):
                // Ověříme parametry funkce a získáme její navratový typ
                result = semantic_analyseFunCall(statement->statement, &type);
                // Pokud typ není void, tak bychom zahodili návratovou hodnotu
                if(type != SEM_DATA_VOID){
                    return ERROR_SEM_PARAMS_OR_RETVAL;
                }

                break;

            case(AST_STATEMENT_IF):
                result = semantic_analyseIf(fun_return,
                                            (AST_IfNode*)statement->statement,
                                            returned);
                break;

            case(AST_STATEMENT_WHILE):
                result = semantic_analyseWhile(fun_return,
                                               (AST_WhileNode*)statement->statement,
                                               returned);
                break;

            case(AST_STATEMENT_RETURN):
                result = semantic_analyseReturn(fun_return, statement->statement);
                *returned = true;
                break;

            // Pokud typ příkazu je něco nedefinovaného, tak je chyba v implementaci
            default:
                return ERROR_SEM_OTHER;
        }

        // Pokud návratováý kód není v pořádku, tak ho pošleme dál
        if(result != 0) {
            return result;
        }

        // Pokud jsme narazili na return a vše je v pořádku, tak končíme úspěšně
        if(*returned == true) {
            return 0;
        }

        statement = statement->next;
    }

    // Pokud jsme prošli celý blok, tak končíme úspěšně
    return 0;
}

/**
 * @brief Provede sémantickou analýzu binárního operátoru
 */
ErrorType semantic_analyseBinOp(AST_ExprNode *node, Semantic_Data *type, void** value) {
    AST_BinOpNode bin_node = *(AST_BinOpNode*)node->expression;
    // Pokud jsou typy kompatibilní provede případné implicitní úpravy a vrací 0

    ErrorType result;

    if(bin_node.op == AST_OP_ASSIGNMENT){
        // Pokud vlevo není proměnná
        if(bin_node.left->exprType != AST_EXPR_VARIABLE){
            return ERROR_SEM_OTHER;
        }

        AST_VarNode *left_node = bin_node.left->expression;
        // Pokud je vlevo pseudoproměnná, tak jen zkontrolujeme pravou stranu
        if(string_compare_const_str(left_node->identifier, "_" ) == STRING_EQUAL){
            result = semantic_analyseExpr(bin_node.right, type, NULL);
            return result;
        }

        // Zjitíme typ rhs
        Semantic_Data r_type;
        void *r_value;
        result = semantic_analyseExpr(bin_node.right, &r_type, &r_value);
        if(result != 0) {
            return result;
        }

        // Zjistíme typ proměnné vlevo
        SymtablePtr table = frameArray.array[left_node->frameID]->frame;
        SymtableItemPtr item;
        symtable_result sym_res = symtable_findItem(table, left_node->identifier, &item);

        if(sym_res != SYMTABLE_SUCCESS){
            return ERROR_INTERNAL;
        }

        if(item->constant == true){
            return ERROR_SEM_REDEF_OR_CONSTDEF;
        }

        Semantic_Data l_type = semantic_stateToSemType(item->symbol_state);

        result = semantic_compatibleAssign(l_type, r_type);
        if(result != 0) {
            if(result == ERROR_SEM_PARAMS_OR_RETVAL){
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
            return result;
        }

        item->changed = true;
        return 0;

    }


    Semantic_Data left_type;
    Semantic_Data right_type;
    void *left_value;
    void *right_value;

    result = semantic_analyseExpr(bin_node.left, &left_type, &left_value);
    if(result != 0) {
        return result;
    }

    result = semantic_analyseExpr(bin_node.right, &right_type, &right_value);
    if(result != 0) {
        return result;
    }

    // Pokud je operátor aritmetického typu
    if(bin_node.op >= AST_OP_ADD){
        result = semantic_compatibleArithmetic(left_type, right_type);
        if(result != 0) {
            return result;
        }

        /* Pokud známe obě hodnoty, tak můžeme uzel nahradit
        if(left_value != NULL && right_value != NULL){
            AST_VarNode* new_node = semantic_magic(node);
        }*/

        // Pokud jsou typy stejné, tak nic neřešíme
        if(left_type == right_type){
            *type = left_type;
            if(value != NULL){
                *value = NULL;
            }
            return 0;
        }

        // Pokud neznáme ani jednu hodnotu a typy nejsou stejné, tak je chyba
        if(left_value == NULL && right_value == NULL){
            return ERROR_SEM_TYPE_COMPATIBILITY;
        }

        // Pokud jsou typy rozdílné, tak se pokusíme o konverzi
        // Levý typ je celočíselný
        if(left_type == SEM_DATA_INT){
            if(left_value != NULL){
                // Konvertujeme left na float
                result = semantic_toFloat(bin_node.left);
                *type = SEM_DATA_FLOAT;

            }else{
                // Pokusíme se konvertovat right na int
                result = semantic_toInt(bin_node.right);
                *type = SEM_DATA_INT;
            }
        }else{
            if(right_value != NULL){
                // Konvertujeme right na float
                result = semantic_toFloat(bin_node.right);
                *type = SEM_DATA_FLOAT;
            }else{
                // Pokusíme se konvertovat left na int
                result = semantic_toInt(bin_node.left);
                *type = SEM_DATA_INT;
            }
        }
        return 0;
    }

    // Jedná se o relační operátor
    if(bin_node.op == AST_OP_EQUAL || bin_node.op == AST_OP_NOT_EQUAL){
        result = semantic_compatibleEqual(left_type, right_type);
        if(result != 0) {
            return result;
        }

        // Pokud je jeden typ i32 a druhý f64, tak se pokusíme o konverzi
        if(left_type == SEM_DATA_INT && right_type == SEM_DATA_FLOAT){
            if(left_value != NULL){
                //konverze left na float
                result = semantic_toFloat(bin_node.left);
                *type = SEM_DATA_FLOAT;
            }else if(right_value != NULL){
                // konverze right na int
                result = semantic_toInt(bin_node.right);
                *type = SEM_DATA_INT;
            }else{
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }else if(right_type == SEM_DATA_INT && left_type == SEM_DATA_FLOAT){
            if(right_value != NULL){
                //konverze right na float
                result = semantic_toFloat(bin_node.right);
                *type = SEM_DATA_FLOAT;
            }else if(left_value != NULL){
                // konverze left na int
                result = semantic_toInt(bin_node.left);
                *type = SEM_DATA_INT;
            }else{
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }

        if(result != 0){
            return result;
        }

        *type = SEM_DATA_BOOL;
        if(value != NULL){
            *value = NULL;
        }

    }else {
        result = semantic_compatibleRelation(left_type, right_type);
        if(result != 0) {
            return result;
        }

        // Pokud je jeden typ i32 a druhý f64, tak se pokusíme o konverzi
        // Pokud je jeden typ i32 a druhý f64, tak se pokusíme o konverzi
        if(left_type == SEM_DATA_INT && right_type == SEM_DATA_FLOAT){
            if(left_value != NULL){
                //konverze left na float
                result = semantic_toFloat(bin_node.left);
                *type = SEM_DATA_FLOAT;
            }else if(right_value != NULL){
                // konverze right na int
                result = semantic_toInt(bin_node.right);
                *type = SEM_DATA_INT;
            }else{
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }else if(right_type == SEM_DATA_INT && left_type == SEM_DATA_FLOAT){
            if(right_value != NULL){
                //konverze right na float
                result = semantic_toFloat(bin_node.right);
                *type = SEM_DATA_FLOAT;
            }else if(left_value != NULL){
                // konverze left na int
                result = semantic_toInt(bin_node.left);
                *type = SEM_DATA_INT;
            }else{
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }

        if(result != 0){
            return result;
        }

        *type = SEM_DATA_BOOL;
        if(value != NULL){
            *value = NULL;
        }
    }

    return 0;
}

/**
 * @brief Provede sémantickou analýzu definice proměnné
 */
ErrorType semantic_analyseVarDef(AST_StatementNode *statement) {
    AST_ExprNode *expr = statement->statement;
    AST_BinOpNode assignNode = *(AST_BinOpNode*)expr->expression;

    if(assignNode.op != AST_OP_ASSIGNMENT) {
        return ERROR_INTERNAL;
    }

    // Zjitíme typ pravé strany
    Semantic_Data r_type;
    void *value;
    ErrorType result = semantic_analyseExpr(assignNode.right, &r_type, &value);
    if(result != 0) {
        return result;
    }

    // Vlevo musí být proměnná
    if(assignNode.left->exprType != AST_EXPR_VARIABLE){
        return ERROR_INTERNAL;
    }
    AST_VarNode *left_node = assignNode.left->expression;

    // Zjistíme typ proměnné na levé straně
    SymtablePtr table = frameArray.array[left_node->frameID]->frame;
    SymtableItemPtr item;
    DString *identifier = left_node->identifier;
    symtable_result sym_res = symtable_findItem(table, identifier, &item);

    if(sym_res != SYMTABLE_SUCCESS){
        return ERROR_INTERNAL;
    }

    Semantic_Data l_type = semantic_stateToSemType(item->symbol_state);

    // Pokud typ proměnné není definován
    if(l_type == SEM_DATA_UNKNOWN){
        // A nejde odvodit z pravé strany
        if(r_type == SEM_DATA_NULL){
            return ERROR_SEM_TYPE_INFERENCE;
        }

        // Jinak typ odvodíme z pravé strany
        item->symbol_state = semantic_semTypeToState(r_type);
        if(item->symbol_state == SYMTABLE_SYMBOL_UNKNOWN){
            return ERROR_SEM_TYPE_INFERENCE;
        }

    } else {
        // Zkontrolujeme, že typy jsou kompatibilní
        result = semantic_compatibleAssign(l_type, r_type);
        if(result != 0) {
            // Podíváme se, jestli můžeme provést konverzi
            if( (l_type == SEM_DATA_FLOAT || l_type == SEM_DATA_FLOAT_OR_NULL)  \
                && r_type == SEM_DATA_INT && value != NULL){
                result = semantic_toFloat(assignNode.right);
                if(result != 0){
                    return result;
                }
            }else if( (l_type == SEM_DATA_INT || l_type == SEM_DATA_INT_OR_NULL)
                      && r_type == SEM_DATA_FLOAT && value != NULL){
                result = semantic_toInt(assignNode.right);
                if(result != 0){
                    return result;
                }
            }else {
                return result;
            }
        }

    }

    if(item->constant == true){
        if(value != NULL){
            item->known_value = true;
            item->data = value;
        } else {
            item->known_value = false;
        }
    }

    return 0;
}

/**
 * @brief Provede sémantickou analýzu výrazu
 */
ErrorType semantic_analyseExpr(AST_ExprNode *expr_node, Semantic_Data *type, void **value){
    // Podle typu výrazu buď vyhodnotíme jeho typ a hodnotu nebo voláme další funkce
    ErrorType result;
    AST_VarNode *node;

    switch(expr_node->exprType) {
        case AST_EXPR_LITERAL:
            node = expr_node->expression;
            // Nastavíme typ výrazu podle typu literálu
            *type = semantic_literalToSemType(node->literalType);

            // Předáme ukazatel na hodnotu
            if(value != NULL){
                *value = node->value;
            }

            return 0;

        case AST_EXPR_VARIABLE:
            // Hledáme proměnnou v tabulce symbolů
            node = expr_node->expression;
            SymtablePtr table = frameArray.array[node->frameID]->frame;
            SymtableItemPtr item;
            symtable_result sym_res = symtable_findItem(table, node->identifier, &item);

            if(sym_res != SYMTABLE_SUCCESS){
                // Pokud proměnná není v rámci, tak je chyba v implementaci
                return ERROR_INTERNAL;
            }

            item->used = true;

            // Pokud známe hodnotu, ale je NULL,
            if(item->known_value == true && item->data == NULL){
                *type = SEM_DATA_NULL;
                if(value != NULL){
                    *value = NULL;
                }
                return 0;
            }

            *type = semantic_stateToSemType(item->symbol_state);

            if(value != NULL){
                *value = node->value;
            }

            // Pokud je proměnná konstantní, tak se ji pokusíme nahradit literálem
            if(item->constant == true){
                AST_LiteralType lit_type = semantic_semToLiteral(*type);
                if(lit_type == AST_LITERAL_NOT_DEFINED){
                    return 0;
                }
                string_free(node->identifier);
                node->identifier = NULL;
                node->literalType = lit_type;
            }

            // Není co dále kontrolovat, v pořádku se vracíme
            return 0;

        case AST_EXPR_FUN_CALL:
            result = semantic_analyseFunCall(expr_node->expression, type);
            if(result != 0){
                return result;
            }

            if(value != NULL){
                *value = NULL;
            }
            break;

        case AST_EXPR_BINARY_OP:
            result = semantic_analyseBinOp(expr_node, type, value);
            if(result != 0){
                return result;
            }
            break;

        default:
            // Typ výrazu není definován
            return ERROR_SEM_OTHER;
    };

    return 0;
}

/**
 * @brief Provede sémantickou analýzu volání funkce
 */
ErrorType semantic_analyseFunCall(AST_FunCallNode *fun_node, Semantic_Data *return_type){
    SymtableItemPtr item;
    DString *key;

    // Specíální případ funkce ifj.write
    if(fun_node->isBuiltIn &&
       string_compare_const_str(fun_node->identifier, "write") == STRING_EQUAL){
        *return_type = SEM_DATA_VOID;
        return semantic_checkIFJWrite(fun_node);
    }
    // Speciální případ funkce ifj.string
    else if(fun_node->isBuiltIn &&
            string_compare_const_str(fun_node->identifier, "string") == STRING_EQUAL){
        *return_type = SEM_DATA_STRING;
        return semantic_checkIFJString(fun_node);
    }

    // Pokud je funkce vestavěná, tak přidáme prefix ifj.
    if(fun_node->isBuiltIn){
        key = string_charToDString("ifj.");
        if(key == NULL){
            return ERROR_INTERNAL;
        }
        for(size_t i = 0; i < fun_node->identifier->length; i++){
            string_append_char(key, fun_node->identifier->str[i]);
        }
    }else {
        key = string_init();
        if(key == NULL){
            return ERROR_INTERNAL;
        }
        if(string_copy(fun_node->identifier, key) != STRING_SUCCESS){
            return ERROR_INTERNAL;
        }
    }

    symtable_result sym_result = symtable_findItem(frameStack.bottom->frame,
                                               key, &item);
    if(sym_result != SYMTABLE_SUCCESS){
        string_free(key);
        return ERROR_SEM_UNDEF;
    }
    SymtableFunctionData *data = (SymtableFunctionData*)item->data;

    AST_ArgOrParamNode *arg = fun_node->arguments;

    for(size_t i = 0; i < data->param_count; i++){
        // Málo argumentů ve volání
        if(arg == NULL){
            string_free(key);
            return ERROR_SEM_PARAMS_OR_RETVAL;
        }

        // THE HOLY EMPEROR WAS HERE - NEW START !!!

        // Analýza výrazu argumentu
        Semantic_Data actual_type;
        ErrorType result = semantic_analyseExpr(arg->expression, &actual_type, NULL);
        if(result != 0){
            string_free(key);
            return result;
        }

        // Zjistíme, jestli sedí typ argumentu
        Semantic_Data defined_type = semantic_returnToSemType(data->params[i].type);
        result = semantic_compatibleAssign(defined_type, actual_type);

        // THE HOLY EMPEROR WAS HERE - NEW END !!!

        /* THE HOLY EMPEROR WAS HERE - OLD START !!!

        // Najdeme tabulku symbolů pro argument
        AST_VarNode *var_node = arg->expression->expression;
        SymtablePtr table = frameArray.array[var_node->frameID]->frame;

        Semantic_Data actual_type;
        if(var_node->identifier == NULL){
            // Argument je literál
            actual_type = semantic_literalToSemType(var_node->literalType);

        }else{
            // Argument je proměnná
            // Najdeme argument v tabulce symbolů
            sym_result = symtable_findItem(table, var_node->identifier, &item);
            if(sym_result != SYMTABLE_SUCCESS){
                string_free(key);
                return ERROR_INTERNAL;
            }
            actual_type = semantic_stateToSemType(item->symbol_state);
            item->used = true;
        }

        // Zjistíme, jestli sedí typ argumentu
        Semantic_Data defined_type = semantic_returnToSemType(data->params[i].type);
        ErrorType result = semantic_compatibleAssign(defined_type, actual_type);

        THE HOLY EMPEROR WAS HERE - OLD END !!! */

        if(result != 0){
            string_free(key);
            return result;
        }

        arg = arg->next;
    }

    string_free(key);
    // Přebývají argumenty
    if(arg != NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Předáme typ návratové hodnoty funkce
    if(return_type != NULL){
        *return_type = semantic_returnToSemType(data->return_type);
    }

    return 0;
}

/**
 * @brief Provede sémantickou analýzu podmíněného příkazu if
 */
ErrorType semantic_analyseIf(Semantic_Data fun_return,       \
                             AST_IfNode *if_node, bool *returned){
    bool then_returned = false;
    bool else_returned = false;

    //bool then_unreachable = false;
    //bool else_unreachable = false;
    ErrorType result;
    result = semantic_analyseCondition(if_node);
    if(result != 0){
        return result;
    }

    // Projdeme bloky if a else
    result = semantic_probeBlock(fun_return, if_node->thenBranch, &then_returned);
    if(result != 0){
        return result;
    }
    result = semantic_probeBlock(fun_return, if_node->elseBranch, &else_returned);
    if(result != 0){
        return result;
    }

    // Pokud se vrátily oba bloky, tak předáme informaci výš
    if(then_returned == true && else_returned == true){
        *returned = true;
    }
    return 0;
}

/**
 * @brief Provede sémantickou analýzu cyklu while
 */
ErrorType semantic_analyseWhile(Semantic_Data fun_return,       \
                                AST_WhileNode *while_node, bool *returned){

    ErrorType result;

    result = semantic_analyseCondition((AST_IfNode*)while_node);

    // Projdeme tělo while
    result = semantic_probeBlock(fun_return, while_node->body, returned);
    if(result != 0){
        return result;
    }
    return 0;
}

/**
 * @brief Provede sémantickou analýzu návratového příkazu return
 */
ErrorType semantic_analyseReturn(Semantic_Data fun_return, AST_ExprNode *node){
    // Pokud return nemá výraz
    if(node == NULL){
        // Pokud je funkce void, tak je vše v pořádku
        if(fun_return == SEM_DATA_VOID){
            return 0;
        }

        // Jinak vracíme error
        return ERROR_SEM_RETURN_EXP;
    }

    // Return má výraz, tak získáme jeho typ
    Semantic_Data type;
    ErrorType result = semantic_analyseExpr(node, &type, NULL);
    if(result != 0) {
        return result;
    }

    return semantic_compatibleAssign(fun_return, type);

}

ErrorType semantic_analyseCondition(AST_IfNode *if_while_node){
        // Zjistíme typ výsledku výrazu v podmínce
    Semantic_Data type;
    void *condition_value;
    ErrorType result = semantic_analyseExpr(if_while_node->condition, &type, &condition_value);
    if(result != 0){
        return result;
    }

    // Nemáme NULL podmínku
    if(if_while_node->nullCondition == NULL){
        // Pokud typ není relační, tak vracíme chybu
        if(type != SEM_DATA_BOOL){
            return ERROR_SEM_TYPE_COMPATIBILITY;
        }
    }else{
        // Pokud máme NULL podmínku, tak typ musí obsahovat NULL
        if(type != SEM_DATA_NULL            &&  \
           type != SEM_DATA_FLOAT_OR_NULL   &&  \
           type != SEM_DATA_INT_OR_NULL     &&  \
           type != SEM_DATA_STRING_OR_NULL){
            return ERROR_SEM_TYPE_COMPATIBILITY;
        }

        // Odvodíme typ hodnoty bez null
        Semantic_Data type_without_null;
        if(type == SEM_DATA_NULL){
            return ERROR_SEM_TYPE_INFERENCE;
        }else if(type == SEM_DATA_FLOAT_OR_NULL){
            type_without_null = SEM_DATA_FLOAT;
        }else if(type == SEM_DATA_INT_OR_NULL){
            type_without_null = SEM_DATA_INT;
        }else {
            type_without_null = SEM_DATA_STRING;
        }

        DString *key = if_while_node->nullCondition->identifier;
        SymtableItemPtr item;
        SymtablePtr table = frameArray.array[if_while_node->nullCondition->frameID]->frame;
        if(symtable_findItem(table, key, &item) != SYMTABLE_SUCCESS){
            return ERROR_INTERNAL;
        }
        // Hodnota bez null je konstanta
        item->constant = true;
        item->changed = false;
        item->symbol_state = semantic_semTypeToState(type_without_null);
        item->used = false;

        if(condition_value != NULL){
            // Pokud máme hodnotu, tak ji uložíme
            item->known_value = true;
            if(type_without_null == SEM_DATA_STRING){
                DString *str = string_init();
                if(str == NULL){
                    return ERROR_INTERNAL;
                }
                if(string_copy((DString*)condition_value, str) != STRING_SUCCESS){
                    return ERROR_INTERNAL;
                }
                item->data = str;
            }else if(type_without_null == SEM_DATA_INT){
                int *val = malloc(sizeof(int));
                if(val == NULL){
                    return ERROR_INTERNAL;
                }
                *val = *(int*)condition_value;
                item->data = val;
            }else{
                double *val = malloc(sizeof(double));
                if(val == NULL){
                    return ERROR_INTERNAL;
                }
                *val = *(double*)condition_value;
                item->data = val;
            }
            if_while_node->nullCondition->value = item->data;

            // Známe hodnotu a není NULL, takže se nedostaneme do then větve
            //then_unreachable = true;
        }
    }
    return 0;
}

/*AST_VarNode* semantic_magic(AST_ExprNode *node){
    AST_BinOpNode bin_node = *(AST_BinOpNode*)node->expression;
    if(bin_node.left->exprType != AST_VAR_NODE &&
       bin_node.right->exprType != AST_VAR_NODE){
        // David neuvařil, snaží se zjednodušit něco, co nejde
        error_handle(ERROR_INTERNAL);
    }
    AST_VarNode l_node = *(AST_VarNode*)bin_node.left->expression;
    AST_VarNode r_node = *(AST_VarNode*)bin_node.right->expression;
    if(l_node.identifier != NULL || r_node.identifier != NULL){
        // David neuvařil, myslel si, že tady musí být literály
        error_handle(ERROR_INTERNAL);
    }

    void *new_value;
    // Typy jsou stejné
    if(l_node.literalType == r_node.literalType){
        if(l_node.literalType == AST_LITERAL_INT){
            // Oba jsou int
            new_value = semantic_evaluateInt(bin_node.op, l_node.value, r_node.value);
        }else{
            // Oba jsou float
            new_value = semantic_evaluateFloat(bin_node.op, l_node.value, r_node.value);
        }
    }else {
        if(l_node.literalType == AST_LITERAL_INT){
            // L - int | R - float
            semantic_toInt
        }else {
            // L - float | R - int
            semantic_tiInt
        }
    }

    AST_VarNode *new_node =
}*/

/**
 * @brief Vyhodnotí, zda lze provést přiřazení z typu from do typu to
 */
ErrorType semantic_compatibleAssign(Semantic_Data to, Semantic_Data from) {
    switch(to){
        // Pro typy bez NULL se musí typy rovnat
        case SEM_DATA_FLOAT:
            // Projdeme dál
        case SEM_DATA_INT:
            // Projdeme dál
        case SEM_DATA_STRING:

            if(to != from){
                return ERROR_SEM_PARAMS_OR_RETVAL;
            }
            break;

        // Pro typy s NULL mohou být stejného typy s NULL i bez nebo čistě NULL
        case SEM_DATA_FLOAT_OR_NULL:
            if(to != SEM_DATA_FLOAT &&             \
               to != SEM_DATA_FLOAT_OR_NULL &&     \
               to != SEM_DATA_NULL) {
                return ERROR_SEM_PARAMS_OR_RETVAL;
            }
            break;

        case SEM_DATA_INT_OR_NULL:
            if(to != SEM_DATA_INT &&               \
               to != SEM_DATA_INT_OR_NULL &&       \
               to != SEM_DATA_NULL) {
                return ERROR_SEM_PARAMS_OR_RETVAL;
            }
            break;

        case SEM_DATA_STRING_OR_NULL:
            if(to != SEM_DATA_STRING &&            \
               to != SEM_DATA_STRING_OR_NULL &&    \
               to != SEM_DATA_NULL) {
                return ERROR_SEM_PARAMS_OR_RETVAL;
            }
            break;

        // THE HOLY EMPEROR WAS HERE - NEW START !!!
        default:
            return ERROR_SEM_RETURN_EXP;
    }


    return SUCCESS;
    // THE HOLY EMPEROR WAS HERE - NEW END !!!
}

/**
 * @brief Vyhodnotí, zda lze provést aritmetickou operaci mezi typy
 */
ErrorType semantic_compatibleArithmetic(Semantic_Data type_1, Semantic_Data type_2){
    if(type_1 != SEM_DATA_INT && type_1 != SEM_DATA_FLOAT){
        return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    if(type_2 != SEM_DATA_INT && type_2 != SEM_DATA_FLOAT){
        return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    return 0;
}

/**
 * @brief Vyhodnotí, zda lze provést rovnostní operaci mezi typy
 */
ErrorType semantic_compatibleEqual(Semantic_Data type_1, Semantic_Data type_2){
    switch(type_1){
        case SEM_DATA_INT:
            if(type_2 == SEM_DATA_INT || type_2 == SEM_DATA_FLOAT ||
                type_2 == SEM_DATA_INT_OR_NULL){
                return 0;
            }
            break;
        case SEM_DATA_FLOAT:
            if(type_2 == SEM_DATA_INT || type_2 == SEM_DATA_FLOAT ||
                type_2 == SEM_DATA_FLOAT_OR_NULL){
                return 0;
            }
            break;
        case SEM_DATA_INT_OR_NULL:
            if(type_2 == SEM_DATA_INT || type_2 == SEM_DATA_FLOAT_OR_NULL ||
                type_2 == SEM_DATA_INT_OR_NULL){
                return 0;
            }
            break;
        case SEM_DATA_FLOAT_OR_NULL:
            if(type_2 == SEM_DATA_INT_OR_NULL || type_2 == SEM_DATA_FLOAT ||
                type_2 == SEM_DATA_FLOAT_OR_NULL){
                return 0;
            }
            break;
        case SEM_DATA_NULL:
            if(type_2 == SEM_DATA_NULL || type_2 == SEM_DATA_INT_OR_NULL ||
                type_2 == SEM_DATA_FLOAT_OR_NULL){
                return 0;
            }
            break;
        default:
            return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    return ERROR_SEM_TYPE_COMPATIBILITY;
}

/**
 * @brief Vyhodnotí, zda lze provést relační operaci mezi typy
 */
ErrorType semantic_compatibleRelation(Semantic_Data type_1, Semantic_Data type_2){
    switch(type_1){
        case SEM_DATA_INT:
            if(type_2 == SEM_DATA_INT || type_2 == SEM_DATA_FLOAT){
                return 0;
            }
            break;
        case SEM_DATA_FLOAT:
            if(type_2 == SEM_DATA_INT || type_2 == SEM_DATA_FLOAT){
                return 0;
            }
            break;
        default:
            return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    return ERROR_SEM_TYPE_COMPATIBILITY;
}

/**
 * @brief Převede hodnotu enum symtable_symbolState na typ Semantic_Data
 */
Semantic_Data semantic_stateToSemType(symtable_symbolState state){
    switch(state){
        case SYMTABLE_SYMBOL_VARIABLE_DOUBLE:
            return SEM_DATA_FLOAT;
        case SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL:
            return SEM_DATA_FLOAT_OR_NULL;
        case SYMTABLE_SYMBOL_VARIABLE_INT:
            return SEM_DATA_INT;
        case SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL:
            return SEM_DATA_INT_OR_NULL;
        case SYMTABLE_SYMBOL_VARIABLE_STRING:
            return SEM_DATA_STRING;
        case SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL:
            return SEM_DATA_STRING_OR_NULL;
        default:
            return SEM_DATA_UNKNOWN;
    }
}

/**
 * @brief Převede hodnotu enum symtable_functionReturnType na typ Semantic_Data
 */
Semantic_Data semantic_returnToSemType(symtable_functionReturnType type){
    switch(type){
        case SYMTABLE_TYPE_DOUBLE:
            return SEM_DATA_FLOAT;
        case SYMTABLE_TYPE_DOUBLE_OR_NULL:
            return SEM_DATA_FLOAT_OR_NULL;
        case SYMTABLE_TYPE_INT:
            return SEM_DATA_INT;
        case SYMTABLE_TYPE_INT_OR_NULL:
            return SEM_DATA_INT_OR_NULL;
        case SYMTABLE_TYPE_STRING:
            return SEM_DATA_STRING;
        case SYMTABLE_TYPE_STRING_OR_NULL:
            return SEM_DATA_STRING_OR_NULL;
        case SYMTABLE_TYPE_VOID:
            return SEM_DATA_VOID;
        default:
            return SEM_DATA_UNKNOWN;
    }
}

/**
 * @brief Převede hodnotu enum AST_LiteralType na typ Semantic_Data
 */
Semantic_Data semantic_literalToSemType(AST_LiteralType type){
    switch(type){
        case AST_LITERAL_INT:
            return SEM_DATA_INT;
        case AST_LITERAL_FLOAT:
            return SEM_DATA_FLOAT;
        case AST_LITERAL_STRING:
            return SEM_DATA_RAW_STRING;
        case AST_LITERAL_NULL:
            return SEM_DATA_NULL;
        default:
            return SEM_DATA_UNKNOWN;
    }
}

/**
 * @brief Převede hodnotu enum AST_DataType na typ Semantic_Data
 */
Semantic_Data semantic_ASTToSemType(AST_DataType type){
    switch(type){
        case AST_DATA_TYPE_FLOAT:
            return SEM_DATA_FLOAT;
        case AST_DATA_TYPE_FLOAT_OR_NULL:
            return SEM_DATA_FLOAT_OR_NULL;
        case AST_DATA_TYPE_INT:
            return SEM_DATA_INT;
        case AST_DATA_TYPE_INT_OR_NULL:
            return SEM_DATA_INT_OR_NULL;
        case AST_DATA_TYPE_STRING:
            return SEM_DATA_STRING;
        case AST_DATA_TYPE_STRING_OR_NULL:
            return SEM_DATA_STRING_OR_NULL;
        case AST_DATA_TYPE_VOID:
            return SEM_DATA_VOID;
        case AST_DATA_TYPE_BOOL:
            return SEM_DATA_BOOL;
        default:
            return SEM_DATA_UNKNOWN;
    }
}

/**
 * @brief Převede hodnotu z typu int na typ float
 */
ErrorType semantic_toFloat(AST_ExprNode *node) {
    AST_VarNode *var_node = node->expression;

    int value = *(int*)var_node->value;
    // Alokujeme paměť pro výsledek
    double* result = (double*)malloc(sizeof(double));
    if (result == NULL) {
        return ERROR_INTERNAL;
    }
    *result = (double)value;
    free(var_node->value);
    var_node->value = result;
    var_node->literalType = AST_LITERAL_FLOAT;

    return 0;
}

/**
 * @brief Převede hodnotu z typu float na typ int
 */
ErrorType semantic_toInt(AST_ExprNode *node) {
    AST_VarNode *var_node = node->expression;
    double value = *(double*)var_node->value;
    // Zkontrolujeme, jestli má value desetinnou část
    if (value != trunc(value)) {
        return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    // Zkontrolujeme, jestli se value vejde do int
    if (value < INT_MIN || value > INT_MAX) {
        return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    // Alokujeme pamět pro výsledek
    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        return ERROR_INTERNAL;
    }
    *result = (int)value;
    free(var_node->value);
    var_node->value = result;
    var_node->literalType = AST_LITERAL_INT;

    return 0;
}

/**
 * @brief Převede hodnotu typu Semantic_Data na enum symtable_symbolState
 */
symtable_symbolState semantic_semTypeToState(Semantic_Data type){
    switch(type){
        case SEM_DATA_FLOAT:
            return SYMTABLE_SYMBOL_VARIABLE_DOUBLE;
        case SEM_DATA_FLOAT_OR_NULL:
            return SYMTABLE_SYMBOL_VARIABLE_DOUBLE_OR_NULL;
        case SEM_DATA_INT:
            return SYMTABLE_SYMBOL_VARIABLE_INT;
        case SEM_DATA_INT_OR_NULL:
            return SYMTABLE_SYMBOL_VARIABLE_INT_OR_NULL;
        case SEM_DATA_STRING:
            return SYMTABLE_SYMBOL_VARIABLE_STRING;
        case SEM_DATA_STRING_OR_NULL:
            return SYMTABLE_SYMBOL_VARIABLE_STRING_OR_NULL;
        default:
            return SYMTABLE_SYMBOL_UNKNOWN;
    }
}

/**
 * @brief Převede hodnotu typu Semantic_Data na enum AST_LiteralType
 */
AST_LiteralType semantic_semToLiteral(Semantic_Data type){
    switch(type){
        case SEM_DATA_FLOAT:
            return AST_LITERAL_FLOAT;
        case SEM_DATA_INT:
            return AST_LITERAL_INT;
        case SEM_DATA_STRING:
            return AST_LITERAL_STRING;
        default:
            return AST_LITERAL_NOT_DEFINED;
    }
}

/**
 * @brief Kontroluje počet parametrů volání funkce ifj.write
 */
ErrorType semantic_checkIFJWrite(AST_FunCallNode *fun_node){
    AST_ArgOrParamNode *arg = fun_node->arguments;
    // Funkce má právě jeden parametr
    if(arg == NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    } else if(arg->next != NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Argument je použit
    AST_ExprNode *expr = arg->expression->expression;
    if(expr->exprType == AST_EXPR_VARIABLE){
        AST_VarNode *var_node = expr->expression;
        if(var_node->identifier != NULL){
            // Argument je proměnná
            SymtablePtr table = frameArray.array[var_node->frameID]->frame;
            SymtableItemPtr item;
            if(symtable_findItem(table, var_node->identifier, &item) != SYMTABLE_SUCCESS){
                return ERROR_INTERNAL;
            }

            item->used = true;
        }
    }
    return 0;
}

/**
 * @brief Kontroluje typ a počet parametrů volání funkce ifj.string
 */
ErrorType semantic_checkIFJString(AST_FunCallNode *fun_node){
    // Funkce má právě jeden parametr
    if(fun_node->arguments == NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    } else if(fun_node->arguments->next != NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Parametr musí být string nebo raw_string literál
    AST_ArgOrParamNode *arg = fun_node->arguments;
    AST_VarNode *var_node = arg->expression->expression;

    if(var_node->identifier == NULL){
        // Argument je literál
        if(semantic_literalToSemType(var_node->literalType) != SEM_DATA_RAW_STRING){
            return ERROR_SEM_PARAMS_OR_RETVAL;
        }

    }else{
        // Argument je proměnná
        // Najdeme argument v tabulce symbolů
        SymtablePtr table = frameArray.array[var_node->frameID]->frame;
        SymtableItemPtr item;
        if(symtable_findItem(table, var_node->identifier, &item) != SYMTABLE_SUCCESS){
            return ERROR_INTERNAL;
        }

        if(semantic_stateToSemType(item->symbol_state) != SEM_DATA_STRING){
            return ERROR_SEM_PARAMS_OR_RETVAL;
        }
        item->used = true;
    }
    return 0;
}


/*** Konec souboru semantic_analyser.c ***/
