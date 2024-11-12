/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.c                                       *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            12.11.2024                                                *
 * Poslední změna:   12.11.2024                                                *
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


ErrorType semantic_checkProgramStructure() {
    // Vytvoříme string main
    DString *str = string_charToDString("main");
    if(str == NULL) {
        return ERROR_INTERNAL;
    }

    SymtableItemPtr item_main;
    // Podíváme se na globální rámec, kde jsou definice funkcí
    symtable_result result = symtable_findItem(stack.bottom->frame, str, &item_main);

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

/*      DOMLUVIT SE S HONZOU A DODĚLAT
    // Zkontrolujeme prolog programu, že má správné literály
    if(string_compare_const_str(ASTroot->importedFile, "ifj24.zig") != STRING_EQUAL) {
        // Toto nelze jednoduše zařadit mezi jiné chyby, proto ERROR_SEM_OTHER
        return ERROR_SEM_OTHER;
    }
*/

    // Vše je v pořádku
    return 0;

}


ErrorType semantic_checkFunctionDefinitions() {
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


ErrorType semantic_checkVariables() {
    // Projdeme všechny rámce
    for(size_t i = 1; i <= stack.currentID; i++) {
        // Z rámce vezmeme tabulku symbolů
        SymtablePtr table = frameArray.array[i]->frame;
        SymtableItem item;
        // Pro každou položku z tabulky symbolů
        for(size_t j = 0; j < table->allocated_size; j++) {
            item = table->array[j];

            // Pokud položka na indexu není, tak přejdeme na další položku
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

    // Pokud jsme prošli všechny rámce bez vrácení, tak je vše v pořádku
    return 0;
}


ErrorType semantic_probeFunction(AST_FunDefNode *FunDefNode) {
    bool reachedReturn = false;
    ErrorType probeResult = semantic_probeBlock(FunDefNode->identifier, FunDefNode->body, &reachedReturn);
    
    // Pokud jsme našli sémantickou chybu, tak ji vrátíme 
    if(probeResult != 0) {
        return probeResult;
    }

    // Pokud jsme došli na konec funkce bez příkazu return, tak zkontrolujeme,
    // že funkce je void
    if(reachedReturn == false) {
        SymtableItemPtr symItem;
        // Funkci hledáme v globálním rámci
        symtable_result symResult = symtable_findItem(stack.bottom->frame, FunDefNode->identifier, &symItem);
        if(symResult != SYMTABLE_SUCCESS) {
            // Pokud funkce není v tabulce symbolů, tak je chyba v implementaci
            return ERROR_SEM_OTHER;
        }

        SymtableFunctionData *data = (SymtableFunctionData *)symItem->data;
        if(data == NULL) {
            // Pokud funce nemá data, tak je chyba v implementaci
            return ERROR_SEM_OTHER;
        }

        // Pokud funkce není void, tak jsme došli na konec funkce bez návratu
        if(data->return_type != SYMTABLE_TYPE_VOID) {
            return ERROR_SEM_RETURN_EXP;
        }
    }

    // Vše proběhlo v pořádku
    return 0;
}

/**
 * @brief Analyzuje blok příkazů
 */
ErrorType semantic_probeBlock(DString *FunId, AST_StatementNode *statement, bool* returned) {
    ErrorType result = 0;

    // Procházíme všechny příkazy těla funkce
    while(statement != NULL) {
        switch(statement->statementType) {
            // Pokud typ příkazu není definován, tak je nejspíš chyba v implementaci
            case(AST_STATEMENT_NOT_DEFINED):
                return ERROR_SEM_OTHER;

            case(AST_STATEMENT_VAR_DEF):
                result = semantic_analyseVarDef();
                break;

            case(AST_STATEMENT_EXPR):
                // Narazili jsme na výraz bez využití hodnoty
                result = ERROR_SEM_OTHER;
                break;

            case(AST_STATEMENT_FUN_CALL):
                result = semantic_analyseFunCallStatement(); // Uvnitř check, že je void a sedí počet/typy parametrů
                break;

            case(AST_STATEMENT_IF):
                result = semantic_analyseIf(); // Uvnitř volá funkce na analýzu bloků a trackuje return v obou větvích 
                break;

            case(AST_STATEMENT_WHILE):
                result = semantic_analyseWhile();
                break;

            case(AST_STATEMENT_RETURN):
                result = semantic_analyseReturn(FunId); // Uvnitř nastaví returned na true a analyzuje správnost return
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
 * @brief Pro binární operátory KROMĚ PŘIŘAZENÍ
 */
/*ErrorType semantic_analyseBinOp(DString *FunId, size_t FrameId, AST_BinOpNode node){
    // Rozhoduje se podle kombinace typů left a right a podle toho, jestli je něco literál nebo const
    // Pokud jsou typy kompatibilní provede případné implicitní úpravy a vrací 0
    get_type(left);
    get_type(right);
    hasKnownValue(left);
    hasKnownValue(right);
}

ErrorType semantic_analyseVarDef(AST_StatementNode *statement) {
    AST_BinOpNode assignNode = *(AST_BinOpNode*)statement->statement;

    ErrorType result = semantic_analyseExpr(DString *FunId, size_t FrameId, AST_BinOpNode node)
    if(result != 0) {
        return result;
    }

    assignNode.
}

ErrorType semantic_analyseExpr();*/

ErrorType semantic_analyseVarDef(){
    return 0;
}

ErrorType semantic_analyseFunCallStatement(){
    return 0;
}

ErrorType semantic_analyseIf(){
    return 0;
}

ErrorType semantic_analyseWhile(){
    return 0;
}

ErrorType semantic_analyseReturn(size_t FunId){
    if(FunId == 0){
        return 0;
    }
    return 0;
}

/*** Konec souboru semantic_analyser.c ***/
