/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.c                                       *
 * Autor:            Krejčí David   <xkrejcd00>                                *
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
 * @file semantic_analyser.c
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Implementace funkcí pro sémantický analyzátor.
 * @details Implementace funkcí z hlavičkového souboru semantic_analyser.h
 */


#include "semantic_analyser.h"


/**
 * @brief Provede sémantickou analýzu celého programu.
 */
void semantic_analyseProgram() {
    ErrorType error = semantic_analyseProgramStructure();
    if(error != SEMANTIC_OK) {
        error_handle(error);
    }

    error = semantic_analyseFunctionDefinitions();
    if(error != SEMANTIC_OK) {
        error_handle(error);
    }

    error = semantic_analyseVariables();
    if(error != SEMANTIC_OK) {
        error_handle(error);
    }
}  // semantic_analyseProgram

/**
 * @brief Provede sémantickou analýzu základní struktury programu.
 */
ErrorType semantic_analyseProgramStructure() {
    // Vytvoříme string main
    DString *str = string_charToDString("main");
    if(str == NULL) {
        return ERROR_INTERNAL;
    }

    SymtableItemPtr itemMain;
    // Podíváme se na globální rámec, kde jsou definice funkcí
    symtable_result result = symtable_findItem(frameStack.bottom->frame, str, &itemMain);

    // Uvolníme string pro hledání
    string_free(str);

    // Pokud se nepodařilo najít main
    if(result != SYMTABLE_SUCCESS) {
        return ERROR_SEM_UNDEF;
    }

    // Pokud jsme našli main, ověříme, že je funkce
    if(itemMain->symbolState != SYMTABLE_SYMBOL_FUNCTION) {
        // Jestli se toto stane, tak je to chyba v implementaci
        return ERROR_INTERNAL;
    }

    // Zkontrolujeme, že má data
    if(itemMain->data == NULL) {
        // Jestli se toto stane, tak je to chyba v implementaci
        return ERROR_INTERNAL;
    }

    // Převedeme její data
    SymtableFunctionData *data = (SymtableFunctionData *)itemMain->data;
    // Zkontrolujeme, že má správný počet parametrů a návratový typ
    if(data->paramCount != 0 || data->returnType != SYMTABLE_TYPE_VOID) {
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
    return SEMANTIC_OK;

}  // semantic_analyseProgramStructure

/**
 * @brief Provede sémantickou analýzu definic funkcí
 */
ErrorType semantic_analyseFunctionDefinitions() {
    AST_FunDefNode *node = ASTroot->functionList;
    // Pro každou definici funkce
    while(node != NULL) {
        ErrorType error = semantic_probeFunction(node);
        if(error != SEMANTIC_OK) {
            return error;
        }
        node = node->next;
    }

    return SEMANTIC_OK;
}  // semantic_analyseFunctionDefinitions

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
        for(size_t j = 0; j < table->allocatedSize; j++) {
            item = table->array[j];

            // Pokud na indexu není položka, tak přejdeme na další index
            if(item.symbolState == SYMTABLE_SYMBOL_EMPTY) {
                continue;
            }

            if(item.symbolState == SYMTABLE_SYMBOL_FUNCTION) {
                // Nemělo by se nikdy stát, jinak je chyba v implementaci
                // Funkce by měly být definovány pouze v globálním rámci
                return ERROR_INTERNAL;
            }

            if(item.symbolState == SYMTABLE_SYMBOL_DEAD) {
                // Nemělo by se nikdy stát, jinak je chyba v implementaci
                // Položky by se měly mazat jen na konci běhu programu
                return ERROR_INTERNAL;
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
    return SEMANTIC_OK;
}  // semantic_analyseVariables

/**
 * @brief Provede sémantickou analýzu bloku funkce
 */
ErrorType semantic_probeFunction(AST_FunDefNode *FunDefNode) {
    bool reachedReturn = false;
    Semantic_Data returnType = semantic_ASTToSemType(FunDefNode->returnType);
    ErrorType probeResult = semantic_probeBlock(returnType, FunDefNode->body, &reachedReturn);

    // Pokud jsme našli sémantickou chybu, tak ji vrátíme
    if(probeResult != SEMANTIC_OK) {
        return probeResult;
    }

    // Pokud jsme došli na konec funkce bez příkazu return, tak zkontrolujeme,
    // že funkce je void
    if(reachedReturn == false) {

        // Pokud funkce není void, tak jsme došli na konec funkce bez návratu
        if(returnType != SEM_DATA_VOID) {
            return ERROR_SEM_RETURN_EXP;
        }
    }

    // Vše proběhlo v pořádku
    return SEMANTIC_OK;
}  // semantic_probeFunction

/**
 * @brief Provede sémantickou analýzu bloku příkazů
 */
ErrorType semantic_probeBlock(Semantic_Data fun_return,      \
                              AST_StatementNode *statement, bool* returned) {
    ErrorType result = SEMANTIC_OK;
    Semantic_Data type;

    AST_ExprNode *expr;     /**< Pouze pro přiřazení */
    AST_BinOpNode binOp;    /**< Pouze pro přiřazení */

    // Procházíme všechny příkazy těla funkce
    while(statement != NULL) {
        switch(statement->statementType) {
            // Pokud typ příkazu není definován, tak je nejspíš chyba v implementaci
            case(AST_STATEMENT_NOT_DEFINED):
                return ERROR_INTERNAL;

            case(AST_STATEMENT_VAR_DEF):
                result = semantic_analyseVarDef(statement);
                break;

            case(AST_STATEMENT_EXPR):
                // Pokud je přiřazení, tak je ok, jinak je zahození hodnoty
                expr = (AST_ExprNode*)statement->statement;
                if(expr->exprType == AST_EXPR_BINARY_OP){
                    binOp = *(AST_BinOpNode*)expr->expression;
                    if(binOp.op == AST_OP_ASSIGNMENT){
                        result = semantic_analyseBinOp(expr, &type, NULL);
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
                // Vracíme se z funkce
                *returned = true;
                break;

            // Pokud typ příkazu je něco nedefinovaného, tak je chyba v implementaci
            default:
                return ERROR_SEM_OTHER;
        }

        // Pokud návratováý kód není v pořádku, tak ho pošleme dál
        if(result != SEMANTIC_OK) {
            return result;
        }

        // Pokud jsme narazili na return a vše je v pořádku, tak končíme úspěšně
        if(*returned == true) {
            return SEMANTIC_OK;
        }

        statement = statement->next;
    }

    // Pokud jsme prošli celý blok, tak končíme úspěšně
    return SEMANTIC_OK;
}  // semantic_probeBlock

/**
 * @brief Provede sémantickou analýzu binárního operátoru
 */
ErrorType semantic_analyseBinOp(AST_ExprNode *node, Semantic_Data *type, void** value) {
    AST_BinOpNode binNode = *(AST_BinOpNode*)node->expression;
    // Pokud jsou typy kompatibilní provede případné implicitní úpravy a vrací SEMANTIC_OK

    ErrorType result;

    if(binNode.op == AST_OP_ASSIGNMENT){
        // Pokud vlevo není proměnná
        if(binNode.left->exprType != AST_EXPR_VARIABLE){
            return ERROR_SEM_OTHER;
        }

        AST_VarNode *leftNode = binNode.left->expression;
        // Pokud je vlevo pseudoproměnná, tak jen zkontrolujeme pravou stranu
        if(string_compare_const_str(leftNode->identifier, "_" ) == STRING_EQUAL){
            result = semantic_analyseExpr(binNode.right, type, NULL);
            return result;
        }

        // Zjitíme typ rhs
        Semantic_Data r_type;
        void *r_value;
        result = semantic_analyseExpr(binNode.right, &r_type, &r_value);
        if(result != SEMANTIC_OK) {
            return result;
        }

        // Zjistíme typ proměnné vlevo
        SymtablePtr table = frameArray.array[leftNode->frameID]->frame;
        SymtableItemPtr item;
        symtable_result sym_res = symtable_findItem(table, leftNode->identifier, &item);

        if(sym_res != SYMTABLE_SUCCESS){
            return ERROR_INTERNAL;
        }

        if(item->constant == true){
            return ERROR_SEM_REDEF_OR_CONSTDEF;
        }

        Semantic_Data l_type = semantic_stateToSemType(item->symbolState);

        // Zjistíme, zda jsou typy kompatibilní
        result = semantic_compatibleAssign(l_type, r_type);
        if(result != SEMANTIC_OK) {
            if(result == ERROR_SEM_PARAMS_OR_RETVAL){
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
            return result;
        }

        // Hodnota proměnné se změnila
        item->changed = true;
        return SEMANTIC_OK;
    }

    // Pokud typ není přiřazení, tak zjistíme typy a hodnoty operandů
    Semantic_Data left_type;
    Semantic_Data right_type;
    void *left_value;
    void *right_value;

    result = semantic_analyseExpr(binNode.left, &left_type, &left_value);
    if(result != SEMANTIC_OK) {
        return result;
    }

    result = semantic_analyseExpr(binNode.right, &right_type, &right_value);
    if(result != SEMANTIC_OK) {
        return result;
    }

    // Pokud je operátor aritmetického typu
    if(binNode.op >= AST_OP_ADD){
        result = semantic_compatibleArithmetic(left_type, right_type);
        if(result != SEMANTIC_OK) {
            return result;
        }

        // Pokud jsou typy stejné, tak nic neřešíme
        if(left_type == right_type){
            *type = left_type;
            if(value != NULL){
                *value = NULL;
            }
            return SEMANTIC_OK;
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
                result = semantic_toFloat(binNode.left);
                *type = SEM_DATA_FLOAT;

            }
            else {
                // Pokusíme se konvertovat right na int
                result = semantic_toInt(binNode.right);
                *type = SEM_DATA_INT;
            }
        }
        else {
            if(right_value != NULL){
                // Konvertujeme right na float
                result = semantic_toFloat(binNode.right);
                *type = SEM_DATA_FLOAT;
            }
            else {
                // Pokusíme se konvertovat left na int
                result = semantic_toInt(binNode.left);
                *type = SEM_DATA_INT;
            }
        }
        return SEMANTIC_OK;
    }

    // Jedná se o relační operátor
    if(binNode.op == AST_OP_EQUAL || binNode.op == AST_OP_NOT_EQUAL){
        result = semantic_compatibleEqual(left_type, right_type);
        if(result != SEMANTIC_OK) {
            return result;
        }

        // Pokud je jeden typ i32 a druhý f64, tak se pokusíme o konverzi
        if(left_type == SEM_DATA_INT && right_type == SEM_DATA_FLOAT){
            if(left_value != NULL){
                //konverze left na float
                result = semantic_toFloat(binNode.left);
                *type = SEM_DATA_FLOAT;
            }
            else if(right_value != NULL){
                // konverze right na int
                result = semantic_toInt(binNode.right);
                *type = SEM_DATA_INT;
            }
            else {
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }
        else if(right_type == SEM_DATA_INT && left_type == SEM_DATA_FLOAT){
            if(right_value != NULL){
                //konverze right na float
                result = semantic_toFloat(binNode.right);
                *type = SEM_DATA_FLOAT;
            }
            else if(left_value != NULL){
                // konverze left na int
                result = semantic_toInt(binNode.left);
                *type = SEM_DATA_INT;
            }
            else {
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }

        if(result != SEMANTIC_OK){
            return result;
        }

        *type = SEM_DATA_BOOL;
        if(value != NULL){
            *value = NULL;
        }

    }
    // Jinak je operátor relačního typu
    else {
        // Zjistíme, zda jsou typy kompatibilní
        result = semantic_compatibleRelation(left_type, right_type);
        if(result != SEMANTIC_OK) {
            return result;
        }

        // Pokud je jeden typ i32 a druhý f64, tak se pokusíme o konverzi
        if(left_type == SEM_DATA_INT && right_type == SEM_DATA_FLOAT){
            if(left_value != NULL){
                //konverze left na float
                result = semantic_toFloat(binNode.left);
                *type = SEM_DATA_FLOAT;
            }
            else if(right_value != NULL){
                // konverze right na int
                result = semantic_toInt(binNode.right);
                *type = SEM_DATA_INT;
            }
            else {
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }
        else if(right_type == SEM_DATA_INT && left_type == SEM_DATA_FLOAT){
            if(right_value != NULL){
                //konverze right na float
                result = semantic_toFloat(binNode.right);
                *type = SEM_DATA_FLOAT;
            }
            else if(left_value != NULL){
                // konverze left na int
                result = semantic_toInt(binNode.left);
                *type = SEM_DATA_INT;
            }
            else {
                return ERROR_SEM_TYPE_COMPATIBILITY;
            }
        }

        if(result != SEMANTIC_OK){
            return result;
        }

        *type = SEM_DATA_BOOL;
        if(value != NULL){
            *value = NULL;
        }
    }

    return SEMANTIC_OK;
}  // semantic_analyseBinOp

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
    if(result != SEMANTIC_OK) {
        return result;
    }

    // Vlevo musí být proměnná
    if(assignNode.left->exprType != AST_EXPR_VARIABLE){
        return ERROR_INTERNAL;
    }
    AST_VarNode *leftNode = assignNode.left->expression;

    // Zjistíme typ proměnné na levé straně
    SymtablePtr table = frameArray.array[leftNode->frameID]->frame;
    SymtableItemPtr item;
    DString *identifier = leftNode->identifier;
    symtable_result sym_res = symtable_findItem(table, identifier, &item);

    if(sym_res != SYMTABLE_SUCCESS){
        return ERROR_INTERNAL;
    }

    Semantic_Data l_type = semantic_stateToSemType(item->symbolState);

    // Pokud typ proměnné není definován
    if(l_type == SEM_DATA_UNKNOWN){
        // A nejde odvodit z pravé strany
        if(r_type == SEM_DATA_NULL){
            return ERROR_SEM_TYPE_INFERENCE;
        }

        // Jinak typ odvodíme z pravé strany
        item->symbolState = semantic_semTypeToState(r_type);
        if(item->symbolState == SYMTABLE_SYMBOL_UNKNOWN){
            return ERROR_SEM_TYPE_INFERENCE;
        }

    }
    else {
        // Zkontrolujeme, že typy jsou kompatibilní
        result = semantic_compatibleAssign(l_type, r_type);
        if(result != SEMANTIC_OK) {
            // Podíváme se, jestli můžeme provést konverzi
            if( (l_type == SEM_DATA_FLOAT || l_type == SEM_DATA_FLOAT_OR_NULL)  \
                && r_type == SEM_DATA_INT && value != NULL){
                result = semantic_toFloat(assignNode.right);
                if(result != SEMANTIC_OK){
                    return result;
                }
            }
            else if( (l_type == SEM_DATA_INT || l_type == SEM_DATA_INT_OR_NULL)
                      && r_type == SEM_DATA_FLOAT && value != NULL){
                result = semantic_toInt(assignNode.right);
                if(result != SEMANTIC_OK){
                    return result;
                }
            }
            else {
                return result;
            }
        }

    }

    if(item->constant == true){
        if(value != NULL){
            item->knownValue = true;
            item->data = value;
        }
        else {
            item->knownValue = false;
        }
    }

    return SEMANTIC_OK;
}  // semantic_analyseVarDef

/**
 * @brief Provede sémantickou analýzu výrazu
 */
ErrorType semantic_analyseExpr(AST_ExprNode *exprNode, Semantic_Data *type, void **value){
    // Podle typu výrazu buď vyhodnotíme jeho typ a hodnotu nebo voláme další funkce
    ErrorType result;
    AST_VarNode *node;

    switch(exprNode->exprType) {
        case AST_EXPR_LITERAL:
            node = exprNode->expression;
            // Nastavíme typ výrazu podle typu literálu
            *type = semantic_literalToSemType(node->literalType);

            // Předáme ukazatel na hodnotu
            if(value != NULL){
                *value = node->value;
            }

            return SEMANTIC_OK;

        case AST_EXPR_VARIABLE:
            // Hledáme proměnnou v tabulce symbolů
            node = exprNode->expression;
            SymtablePtr table = frameArray.array[node->frameID]->frame;
            SymtableItemPtr item;
            symtable_result sym_res = symtable_findItem(table, node->identifier, &item);

            if(sym_res != SYMTABLE_SUCCESS){
                // Pokud proměnná není v rámci, tak je chyba v implementaci
                return ERROR_INTERNAL;
            }

            item->used = true;

            // Pokud je hodnota NULL, tak vracíme typ NULL
            if(item->knownValue == true && item->data == NULL){
                *type = SEM_DATA_NULL;
                if(value != NULL){
                    *value = NULL;
                }
                return SEMANTIC_OK;
            }

            *type = semantic_stateToSemType(item->symbolState);

            if(value != NULL){
                *value = node->value;
            }

            // Není co dále kontrolovat, v pořádku se vracíme
            return SEMANTIC_OK;

        case AST_EXPR_FUN_CALL:
            result = semantic_analyseFunCall(exprNode->expression, type);
            if(result != SEMANTIC_OK){
                return result;
            }

            if(value != NULL){
                *value = NULL;
            }
            break;

        case AST_EXPR_BINARY_OP:
            result = semantic_analyseBinOp(exprNode, type, value);
            if(result != SEMANTIC_OK){
                return result;
            }
            break;

        default:
            // Typ výrazu není definován
            return ERROR_INTERNAL;
    };

    return SEMANTIC_OK;
}  // semantic_analyseExpr

/**
 * @brief Provede sémantickou analýzu volání funkce
 */
ErrorType semantic_analyseFunCall(AST_FunCallNode *funNode, Semantic_Data *returnType){
    SymtableItemPtr item = NULL;
    DString *key = NULL;

    // Specíální případ funkce ifj.write
    if(funNode->isBuiltIn &&
       string_compare_const_str(funNode->identifier, "write") == STRING_EQUAL){
        *returnType = SEM_DATA_VOID;
        return semantic_checkIFJWrite(funNode);
    }
    // Speciální případ funkce ifj.string
    else if(funNode->isBuiltIn &&
            string_compare_const_str(funNode->identifier, "string") == STRING_EQUAL){
        *returnType = SEM_DATA_STRING;
        return semantic_checkIFJString(funNode);
    }

    // Pokud je funkce vestavěná, tak přidáme prefix ifj.
    if(funNode->isBuiltIn){
        key = string_charToDString("ifj.");
        if(key == NULL){
            return ERROR_INTERNAL;
        }
        for(size_t i = 0; i < funNode->identifier->length; i++){
            string_append_char(key, funNode->identifier->str[i]);
        }
    }
    // Jinak zkopírujeme identifikátor
    else {
        key = string_init();
        if(key == NULL){
            return ERROR_INTERNAL;
        }
        if(string_copy(funNode->identifier, key) != STRING_SUCCESS){
            return ERROR_INTERNAL;
        }
    }

    // Zjistíme, zda je funkce definována
    symtable_result sym_result = symtable_findItem(frameStack.bottom->frame,
                                               key, &item);
    if(sym_result != SYMTABLE_SUCCESS){
        string_free(key);
        return ERROR_SEM_UNDEF;
    }
    SymtableFunctionData *data = (SymtableFunctionData*)item->data;

    AST_ArgOrParamNode *arg = funNode->arguments;

    // Projdeme všechny argumenty
    for(size_t i = 0; i < data->paramCount; i++){
        // Málo argumentů ve volání
        if(arg == NULL){
            string_free(key);
            return ERROR_SEM_PARAMS_OR_RETVAL;
        }

        // Analýza výrazu argumentu
        Semantic_Data actualType = SEM_DATA_UNKNOWN;
        ErrorType result = semantic_analyseExpr(arg->expression, &actualType, NULL);
        if(result != SEMANTIC_OK){
            string_free(key);
            return result;
        }

        // Zjistíme, jestli sedí typ argumentu
        Semantic_Data defined_type = semantic_returnToSemType(data->params[i].type);

        result = semantic_compatibleAssign(defined_type, actualType);

        if(result != SEMANTIC_OK){
            string_free(key);
            return result;
        }

        // Přejdeme na další argument
        arg = arg->next;
    }

    string_free(key);
    // Přebývají argumenty
    if(arg != NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Předáme typ návratové hodnoty funkce
    if(returnType != NULL){
        *returnType = semantic_returnToSemType(data->returnType);
    }

    return SEMANTIC_OK;
}  // semantic_analyseFunCall


/**
 * @brief Provede sémantickou analýzu podmíněného příkazu if
 */
ErrorType semantic_analyseIf(Semantic_Data fun_return,       \
                             AST_IfNode *ifNode, bool *returned){
    bool then_returned = false;
    bool else_returned = false;

    // Analyzujeme podmínku
    ErrorType result;
    result = semantic_analyseCondition(ifNode);
    if(result != SEMANTIC_OK){
        return result;
    }

    // Projdeme bloky if a else
    result = semantic_probeBlock(fun_return, ifNode->thenBranch, &then_returned);
    if(result != SEMANTIC_OK){
        return result;
    }
    result = semantic_probeBlock(fun_return, ifNode->elseBranch, &else_returned);
    if(result != SEMANTIC_OK){
        return result;
    }

    // Pokud se vrátily oba bloky, tak předáme informaci výš
    if(then_returned == true && else_returned == true){
        *returned = true;
    }
    return SEMANTIC_OK;
}  // semantic_analyseIf

/**
 * @brief Provede sémantickou analýzu cyklu while
 */
ErrorType semantic_analyseWhile(Semantic_Data fun_return,       \
                                AST_WhileNode *whileNode, bool *returned){

    ErrorType result;

    // Analyzujeme podmínku
    // Přetypovaní na AST_IfNode, protože má stejnou strukturu pro podmínku
    result = semantic_analyseCondition((AST_IfNode*)whileNode);
    if(result != SEMANTIC_OK){
        return result;
    }

    // Projdeme tělo while
    result = semantic_probeBlock(fun_return, whileNode->body, returned);
    if(result != SEMANTIC_OK){
        return result;
    }
    return SEMANTIC_OK;
}  // semantic_analyseWhile

/**
 * @brief Provede sémantickou analýzu návratového příkazu return
 */
ErrorType semantic_analyseReturn(Semantic_Data fun_return, AST_ExprNode *node){
    // Pokud return nemá výraz
    if(node == NULL){
        // Pokud je funkce void, tak je vše v pořádku
        if(fun_return == SEM_DATA_VOID){
            return SEMANTIC_OK;
        }

        // Jinak vracíme error
        return ERROR_SEM_RETURN_EXP;
    }

    // Return má výraz, tak získáme jeho typ
    Semantic_Data type;
    ErrorType result = semantic_analyseExpr(node, &type, NULL);
    if(result != SEMANTIC_OK) {
        return result;
    }

    return semantic_compatibleAssign(fun_return, type);
}  // semantic_analyseReturn

ErrorType semantic_analyseCondition(AST_IfNode *ifWhileNode){
        // Zjistíme typ výsledku výrazu v podmínce
    Semantic_Data type;
    void *conditionValue;
    ErrorType result = semantic_analyseExpr(ifWhileNode->condition, &type, &conditionValue);
    if(result != SEMANTIC_OK){
        return result;
    }

    // Nemáme NULL podmínku
    if(ifWhileNode->nullCondition == NULL){
        // Pokud typ není relační, tak vracíme chybu
        if(type != SEM_DATA_BOOL){
            return ERROR_SEM_TYPE_COMPATIBILITY;
        }
    }
    else {
        // Pokud máme NULL podmínku, tak typ musí obsahovat NULL
        if(type != SEM_DATA_NULL            &&  \
           type != SEM_DATA_FLOAT_OR_NULL   &&  \
           type != SEM_DATA_INT_OR_NULL     &&  \
           type != SEM_DATA_STRING_OR_NULL){
            return ERROR_SEM_TYPE_COMPATIBILITY;
        }

        // Odvodíme typ hodnoty bez null
        Semantic_Data typeWithoutNull;
        if(type == SEM_DATA_NULL){
            return ERROR_SEM_TYPE_INFERENCE;
        }
        else if(type == SEM_DATA_FLOAT_OR_NULL){
            typeWithoutNull = SEM_DATA_FLOAT;
        }
        else if(type == SEM_DATA_INT_OR_NULL){
            typeWithoutNull = SEM_DATA_INT;
        }
        else {
            typeWithoutNull = SEM_DATA_STRING;
        }

        // Najdeme proměnnou v tabulce symbolů
        DString *key = ifWhileNode->nullCondition->identifier;
        SymtableItemPtr item;
        SymtablePtr table = frameArray.array[ifWhileNode->nullCondition->frameID]->frame;
        if(symtable_findItem(table, key, &item) != SYMTABLE_SUCCESS){
            return ERROR_INTERNAL;
        }
        // Hodnota bez null je konstanta
        item->constant = true;
        item->changed = false;
        item->symbolState = semantic_semTypeToState(typeWithoutNull);
        item->used = false;

        if(conditionValue != NULL){
            // Pokud máme hodnotu, tak ji uložíme
            item->knownValue = true;
            if(typeWithoutNull == SEM_DATA_STRING){
                DString *str = string_init();
                if(str == NULL){
                    return ERROR_INTERNAL;
                }
                if(string_copy((DString*)conditionValue, str) != STRING_SUCCESS){
                    return ERROR_INTERNAL;
                }
                item->data = str;
            }
            else if(typeWithoutNull == SEM_DATA_INT){
                int *val = malloc(sizeof(int));
                if(val == NULL){
                    return ERROR_INTERNAL;
                }
                *val = *(int*)conditionValue;
                item->data = val;
            }
            else {
                double *val = malloc(sizeof(double));
                if(val == NULL){
                    return ERROR_INTERNAL;
                }
                *val = *(double*)conditionValue;
                item->data = val;
            }
            ifWhileNode->nullCondition->value = item->data;

            // Známe hodnotu a není NULL, takže se nedostaneme do then větve
        }
    }
    return SEMANTIC_OK;
}  // semantic_analyseCondition

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

        default:
            return ERROR_SEM_RETURN_EXP;
    }

    return SUCCESS;
}  // semantic_compatibleAssign

/**
 * @brief Vyhodnotí, zda lze provést aritmetickou operaci mezi typy
 */
ErrorType semantic_compatibleArithmetic(Semantic_Data type1, Semantic_Data type2){
    if(type1 != SEM_DATA_INT && type1 != SEM_DATA_FLOAT){
        return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    if(type2 != SEM_DATA_INT && type2 != SEM_DATA_FLOAT){
        return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    return SEMANTIC_OK;
}  // semantic_compatibleArithmetic

/**
 * @brief Vyhodnotí, zda lze provést rovnostní operaci mezi typy
 */
ErrorType semantic_compatibleEqual(Semantic_Data type1, Semantic_Data type2){
    switch(type1){
        case SEM_DATA_INT:
            if(type2 == SEM_DATA_INT || type2 == SEM_DATA_FLOAT ||
                type2 == SEM_DATA_INT_OR_NULL){
                return SEMANTIC_OK;
            }
            break;
        case SEM_DATA_FLOAT:
            if(type2 == SEM_DATA_INT || type2 == SEM_DATA_FLOAT ||
                type2 == SEM_DATA_FLOAT_OR_NULL){
                return SEMANTIC_OK;
            }
            break;
        case SEM_DATA_INT_OR_NULL:
            if(type2 == SEM_DATA_INT || type2 == SEM_DATA_FLOAT_OR_NULL ||
                type2 == SEM_DATA_INT_OR_NULL){
                return SEMANTIC_OK;
            }
            break;
        case SEM_DATA_FLOAT_OR_NULL:
            if(type2 == SEM_DATA_INT_OR_NULL || type2 == SEM_DATA_FLOAT ||
                type2 == SEM_DATA_FLOAT_OR_NULL){
                return SEMANTIC_OK;
            }
            break;
        case SEM_DATA_NULL:
            if(type2 == SEM_DATA_NULL || type2 == SEM_DATA_INT_OR_NULL ||
                type2 == SEM_DATA_FLOAT_OR_NULL){
                return SEMANTIC_OK;
            }
            break;
        default:
            return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    return ERROR_SEM_TYPE_COMPATIBILITY;
}  // semantic_compatibleEqual

/**
 * @brief Vyhodnotí, zda lze provést relační operaci mezi typy
 */
ErrorType semantic_compatibleRelation(Semantic_Data type1, Semantic_Data type2){
    switch(type1){
        case SEM_DATA_INT:
            if(type2 == SEM_DATA_INT || type2 == SEM_DATA_FLOAT){
                return SEMANTIC_OK;
            }
            break;
        case SEM_DATA_FLOAT:
            if(type2 == SEM_DATA_INT || type2 == SEM_DATA_FLOAT){
                return SEMANTIC_OK;
            }
            break;
        default:
            return ERROR_SEM_TYPE_COMPATIBILITY;
    }
    return ERROR_SEM_TYPE_COMPATIBILITY;
}  // semantic_compatibleRelation

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
}  // semantic_stateToSemType

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
}  // semantic_returnToSemType

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
}  // semantic_literalToSemType

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
}  // semantic_ASTToSemType

/**
 * @brief Převede hodnotu z typu int na typ float
 */
ErrorType semantic_toFloat(AST_ExprNode *node) {
    AST_VarNode *varNode = node->expression;

    int value = *(int*)varNode->value;
    // Alokujeme paměť pro výsledek
    double* result = (double*)malloc(sizeof(double));
    if (result == NULL) {
        return ERROR_INTERNAL;
    }
    *result = (double)value;
    free(varNode->value);
    varNode->value = result;
    varNode->literalType = AST_LITERAL_FLOAT;

    return SEMANTIC_OK;
}  // semantic_toFloat

/**
 * @brief Převede hodnotu z typu float na typ int
 */
ErrorType semantic_toInt(AST_ExprNode *node) {
    AST_VarNode *varNode = node->expression;
    double value = *(double*)varNode->value;
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
    free(varNode->value);
    varNode->value = result;
    varNode->literalType = AST_LITERAL_INT;

    return SEMANTIC_OK;
}  // semantic_toInt

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
}  // semantic_semTypeToState

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
}  // semantic_semToLiteral

/**
 * @brief Kontroluje počet parametrů volání funkce ifj.write
 */
ErrorType semantic_checkIFJWrite(AST_FunCallNode *funNode){
    AST_ArgOrParamNode *arg = funNode->arguments;
    // Funkce má právě jeden parametr
    if(arg == NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }
    else if(arg->next != NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Analýza výrazu argumentu
    Semantic_Data actualType = SEM_DATA_UNKNOWN;

    return semantic_analyseExpr(arg->expression, &actualType, NULL);
}  // semantic_checkIFJWrite

/**
 * @brief Kontroluje typ a počet parametrů volání funkce ifj.string
 */
ErrorType semantic_checkIFJString(AST_FunCallNode *funNode){
    // Funkce má právě jeden parametr
    if(funNode->arguments == NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }
    else if(funNode->arguments->next != NULL){
        return ERROR_SEM_PARAMS_OR_RETVAL;
    }

    // Parametr musí být string nebo raw_string literál
    AST_ArgOrParamNode *arg = funNode->arguments;
    AST_VarNode *varNode = arg->expression->expression;

    if(varNode->identifier == NULL){
        // Argument je literál
        if(semantic_literalToSemType(varNode->literalType) != SEM_DATA_RAW_STRING){
            return ERROR_SEM_PARAMS_OR_RETVAL;
        }

    }
    else {
        // Argument je proměnná
        // Najdeme argument v tabulce symbolů
        SymtablePtr table = frameArray.array[varNode->frameID]->frame;
        SymtableItemPtr item;
        if(symtable_findItem(table, varNode->identifier, &item) != SYMTABLE_SUCCESS){
            return ERROR_INTERNAL;
        }

        if(semantic_stateToSemType(item->symbolState) != SEM_DATA_STRING){
            return ERROR_SEM_PARAMS_OR_RETVAL;
        }
        item->used = true;
    }
    return SEMANTIC_OK;
}  // semantic_checkIFJString


/*** Konec souboru semantic_analyser.c ***/
