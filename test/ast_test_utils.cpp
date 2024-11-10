/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           ast_test_utils.cpp                                        *
 * Autor:            Jan Kalina   <xkalinj00>                                  *
 *                                                                             *
 * Datum:            08.11.2024                                                *
 * Poslední změna:   08.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ast_test_utils.cpp
 * @author Jan Kalina
 *
 * @brief Implementace utility funkcí pro práci s AST v testovacím prostředí.
 * @details Tento soubor obsahuje implementace funkcí pro tisk, porovnání a
 *          formátování různých typů uzlů AST. Implementace využívají escape
 *          sekvence pro zobrazení barevného výstupu, což usnadňuje vizualizaci
 *          struktury AST během testování.
 */

#include "ast_test_utils.h"

using namespace std;


/*******************************************************************************
 *                                                                             *
 *                 IMPLEMENTACE VEŘEJNÝCH FUNKCÍ PRO VÝPIS AST                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Zachytí výstup tisku uzlu AST_NODE_PROGRAM.
 */
string ASTutils_printCapturedOutput(AST_ProgramNode *node, bool useColors) {
    stringstream buffer;
    vector<bool>  levels;
    ASTutils_printProgramNode(node, buffer, 0, useColors, levels, true);
    return buffer.str();
}  // ASTutils_printCapturedOutput()

/**
 * @brief Vytiskne výstup testu a referenční výstup.
 */
void ASTutils_printDiff(string output, string reference, ostream &out) {
    out << endl << COLOR_PINK << "Test result output:" << COLOR_RESET << endl;
    out << output << endl;
    out << COLOR_PINK << "Referential output:" << COLOR_RESET << endl;
    out << reference << endl;
}  // ASTutils_printDiff()


/*******************************************************************************
 *                                                                             *
 *                 IMPLEMENTACE INTERNÍCH FUNKCÍ PRO VÝPIS AST                 *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Vytiskne uzel AST_NODE_PROGRAM.
 */
void ASTutils_printProgramNode(AST_ProgramNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild) {
    if (node == nullptr) return;

    // Programový uzel je hlavní, tedy poslední uzel této úrovně
    ASTutils_printIndent(indent, out, levels, isLastChild);
    if (useColors)
        out << COLOR_GOLD << "Program Node" << COLOR_RESET << endl;
    else
        out << "Program Node" << endl;

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Ověření existence importovaného souboru
    if (node->importedFile != nullptr && node->importedFile->str != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, false);
        char *importedFile = string_toConstChar(node->importedFile);
        if (useColors)
            out << COLOR_BLUE << "Imported File: " << COLOR_RESET << importedFile << endl;
        else
            out << "Imported File: " << importedFile << endl;
        free(importedFile);
    } else {
        ASTutils_printIndent(indent + 1, out, levels, false);
        out << "Imported File: (null)" << endl;
    }

    // Iterace přes seznam funkcí a tisk každé funkce
    AST_FunDefNode *func = node->functionList;
    if (func == nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Function List: (null)" << endl;
    } else {
        while (func != nullptr) {
            ASTutils_printFunDefNode(func, out, indent + 1, useColors, levels, func->next == nullptr);
            func = func->next;
        }
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
} // ASTutils_printProgramNode()

/**
 * @brief Vytiskne uzel AST_NODE_FUN_DEF.
 */
void ASTutils_printFunDefNode(AST_FunDefNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení
    ASTutils_printIndent(indent, out, levels, isLastChild);

    // Ověření existence identifikátoru
    if (node->identifier != nullptr && node->identifier->str != nullptr) { 
        char *identifier = string_toConstChar(node->identifier);
        if (useColors)
            out << COLOR_GOLD << "Function Definition: " << COLOR_RESET << identifier << endl;
        else
            out << "Function Definition: " << identifier << endl;
        free(identifier);
    } else {
        // Pokud `identifier` nebo `identifier->str` není platný, ošetříme výstup
        out << "Function Definition: (null)" << endl;
    }

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Výpis parametrů funkce, pokud existují
    if(node->parameters != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, false);
        if (useColors)
            out << COLOR_BLUE << "Parameters:" << COLOR_RESET << endl;
        else
            out << "Parameters:" << endl;
        ASTutils_printArgOrParamNode(node->parameters, out, indent + 2, useColors, levels);
    } else {
        // Pokud parametry neexistují
        ASTutils_printIndent(indent + 1, out, levels, false);
        out << "Parameters: (null)" << endl;
    }

    // Výpis návratového typu funkce
    ASTutils_printIndent(indent + 1, out, levels, false);
    if (useColors)
        out << COLOR_BLUE << "Return Type: " << COLOR_RESET << node->returnType << endl;
    else
        out << "Return Type: " << node->returnType << endl;

    // Výpis těla funkce, pokud existuje
    if(node->body != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        if (useColors)
            out << COLOR_BLUE << "Function Body:" << COLOR_RESET << endl;
        else
            out << "Function Body:" << endl;
        ASTutils_printStatementNode(node->body, out, indent + 2, useColors, levels);
    } else {
        // Pokud tělo funkce neexistuje
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Function Body: (null)" << endl;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
}  // ASTutils_printFunDefNode()

/**
 * @brief Vytiskne uzel AST_NODE_ARG_OR_PARAM.
 */
void ASTutils_printArgOrParamNode(AST_ArgOrParamNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels) {
    // Iterace přes všechny parametry nebo argumenty
    while(node != nullptr) {
        // Příznak posledního výrazu v seznamu
        bool isLast = (node->next == nullptr);

        // Tisknutí odsazení
        ASTutils_printIndent(indent, out, levels, isLast);

        // Aktualizace vektoru úrovní
        levels.push_back(!isLast);

        // Ověření existence identifikátoru a tisknutí identifikátoru
        if (node->identifier != nullptr && node->identifier->str != nullptr) { 
            char *identifier = string_toConstChar(node->identifier);
            if (useColors)
                out << COLOR_BLUE << "Parameter: " << COLOR_RESET << identifier << endl;
            else
                out << "Parameter: " << identifier << endl;
            free(identifier);
        } else {
            // Pokud `identifier` nebo `identifier->str` není platný, ošetříme výstup
            out << "Parameter: (null)" << endl;
        }
        
        // Výpis proměnné, pokud existuje
        if(node->variable != nullptr) {
            ASTutils_printVarNode(node->variable, out, indent + 1, useColors, levels, true);
        } else {
            // Pokud proměnná ani výraz neexistuje
            ASTutils_printIndent(indent + 1, out, levels, true);
            out << "Variable/Expression: (null)" << endl;
        }

        // Odstranit současnou úroveň z vektoru pro tisk svislice
        levels.pop_back();

        node = node->next;
    }
}   // ASTutils_printArgOrParamNode()

/**
 * @brief Vytiskne uzel AST_NODE_STATEMENT.
 */
void ASTutils_printStatementNode(AST_StatementNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels) {
    // Iterace přes všechny příkazy
    while(node != nullptr) {
        // Příznak posledního výrazu v seznamu
        bool isLast = (node->next == nullptr);

        // Tisknutí odsazení
        ASTutils_printIndent(indent, out, levels, isLast);

        // Aktualizace vektoru úrovní
        levels.push_back(!isLast);

        if (useColors)
            out << COLOR_BLUE << "Statement: " << COLOR_RESET;
        else
            out << "Statement: ";
        
        // Rozlišení typu uzlu pomocí switch
        switch(node->statementType) {
            case AST_STATEMENT_VAR_DEF:
                if (useColors)
                    out << COLOR_GOLD << "Variable Definition" << COLOR_RESET << endl;
                else
                    out << "Variable Definition" << endl;
                if(node->statement != nullptr) {
                    ASTutils_printVarNode((AST_VarNode*)node->statement, out, indent + 1, useColors, levels, true);
                } else {
                    ASTutils_printIndent(indent + 1, out, levels, true);
                    out << "Variable Definition: (null)" << endl;
                }
                break;
            case AST_STATEMENT_EXPR:
                if (useColors)
                    out << COLOR_GOLD << "Expression" << COLOR_RESET << endl;
                else
                    out << "Expression" << endl;
                if(node->statement != nullptr) {
                    ASTutils_printExprNode((AST_ExprNode*)node->statement, out, indent + 1, useColors, levels, true);
                } else {
                    ASTutils_printIndent(indent + 1, out, levels, true);
                    out << "Expression: (null)" << endl;
                }
                break;
            case AST_STATEMENT_FUN_CALL:
                if (useColors)
                    out << COLOR_GOLD << "Function Call" << COLOR_RESET << endl;
                else
                    out << "Function Call" << endl;
                if(node->statement != nullptr) {
                    ASTutils_printFunCallNode((AST_FunCallNode*)node->statement, out, indent + 1, useColors, levels, true);
                } else {
                    ASTutils_printIndent(indent + 1, out, levels, true);
                    out << "Function Call: (null)" << endl;
                }
                break;
            case AST_STATEMENT_IF:
                if (useColors)
                    out << COLOR_GOLD << "If Statement" << COLOR_RESET << endl;
                else
                    out << "If Statement" << endl;
                if(node->statement != nullptr) {
                    ASTutils_printIfNode((AST_IfNode*)node->statement, out, indent + 1, useColors, levels, true);
                } else {
                    ASTutils_printIndent(indent + 1, out, levels, true);
                    out << "If Statement: (null)" << endl;
                }
                break;
            case AST_STATEMENT_WHILE:
                if (useColors)
                    out << COLOR_GOLD << "While Loop" << COLOR_RESET << endl;
                else
                    out << "While Loop" << endl;
                if(node->statement != nullptr) {
                    ASTutils_printWhileNode((AST_WhileNode*)node->statement, out, indent + 1, useColors, levels, true);
                } else {
                    ASTutils_printIndent(indent + 1, out, levels, true);
                    out << "While Loop: (null)" << endl;
                }
                break;
            case AST_STATEMENT_RETURN:
                if (useColors)
                    out << COLOR_GOLD << "Return Statement" << COLOR_RESET << endl;
                else
                    out << "Return Statement" << endl;
                if(node->statement != nullptr) {
                    ASTutils_printExprNode((AST_ExprNode*)node->statement, out, indent + 1, useColors, levels, true);
                } else {
                    ASTutils_printIndent(indent + 1, out, levels, true);
                    out << "Return Statement: (null)" << endl;
                }
                break;
            default:
                if (useColors)
                    out << COLOR_GOLD << "Unknown Statement Type" << COLOR_RESET << endl;
                else
                    out << "Unknown Statement Type" << endl;
                break;
        }
        levels.pop_back();
        node = node->next;
    }
}  // ASTutils_printStatementNode()


/**
 * @brief Vytiskne uzel AST_NODE_FUN_CALL.
 */
void ASTutils_printFunCallNode(AST_FunCallNode *node, ostream &out, int indent, bool useColors, vector<bool>  &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení
    ASTutils_printIndent(indent, out, levels, isLastChild);

    // Ověření existence identifikátoru a tisknutí identifikátoru
    if (node->identifier != nullptr && node->identifier->str != nullptr) { 
        char *identifier = string_toConstChar(node->identifier);
        if (useColors)
            out << COLOR_GOLD << "Function Call: " << COLOR_RESET << identifier << endl;
        else
            out << "Function Call: " << identifier << endl;
        free(identifier);
    } else {
        // Pokud `identifier` nebo `identifier->str` není platný, ošetříme výstup
        out << "Function Call: (null)" << endl;
    }

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Výpis argumentů funkce, pokud existují
    if(node->arguments != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        if (useColors)
            out << COLOR_BLUE << "Arguments:" << COLOR_RESET << endl;
        else
            out << "Arguments:" << endl;
        ASTutils_printArgOrParamNode(node->arguments, out, indent + 2, useColors, levels);
    } else {
        // Pokud argumenty neexistují
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Arguments: (null)" << endl;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
}  // ASTutils_printFunCallNode()

/**
 * @brief Vytiskne uzel AST_NODE_IF.
 */
void ASTutils_printIfNode(AST_IfNode *node, ostream &out, int indent, bool useColors, vector<bool>  &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení a typu uzlu
    ASTutils_printIndent(indent, out, levels, isLastChild);
    if (useColors)
        out << COLOR_GOLD << "If Statement" << COLOR_RESET << endl;
    else
        out << "If Statement" << endl;

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Výpis podmínky if
    if(node->condition != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, false);
        if (useColors)
            out << COLOR_BLUE << "Condition:" << COLOR_RESET << endl;
        else
            out << "Condition:" << endl;
        ASTutils_printExprNode(node->condition, out, indent + 2, useColors, levels, true);
    } else {
        // Pokud podmínka neexistuje
        ASTutils_printIndent(indent + 1, out, levels, false);
        out << "Condition: (null)" << endl;
    }

    // Výpis větve 'then'
    if(node->thenBranch != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, false);
        if (useColors)
            out << COLOR_BLUE << "Then Branch:" << COLOR_RESET << endl;
        else
            out << "Then Branch:" << endl;
        ASTutils_printStatementNode(node->thenBranch, out, indent + 2, useColors, levels);
    } else {
        // Pokud 'then' větev neexistuje
        ASTutils_printIndent(indent + 1, out, levels, false);
        out << "Then Branch: (null)" << endl;
    }

    // Výpis větve 'else', pokud existuje
    if(node->elseBranch != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        if (useColors)
            out << COLOR_BLUE << "Else Branch:" << COLOR_RESET << endl;
        else
            out << "Else Branch:" << endl;
        ASTutils_printStatementNode(node->elseBranch, out, indent + 2, useColors, levels);
    } else {
        // Pokud 'else' větev neexistuje
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Else Branch: (null)" << endl;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
}  // ASTutils_printIfNode()

/**
 * @brief Vytiskne uzel AST_NODE_WHILE.
 */
void ASTutils_printWhileNode(AST_WhileNode *node, ostream &out, int indent, bool useColors, vector<bool>  &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení a typu uzlu
    ASTutils_printIndent(indent, out, levels, isLastChild);
    if (useColors)
        out << COLOR_GOLD << "While Loop" << COLOR_RESET << endl;
    else
        out << "While Loop" << endl;

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Výpis podmínky while
    if(node->condition != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, false);
        if (useColors)
            out << COLOR_BLUE << "Condition:" << COLOR_RESET << endl;
        else
            out << "Condition:" << endl;
        ASTutils_printExprNode(node->condition, out, indent + 2, useColors, levels, true);
    } else {
        // Pokud podmínka neexistuje
        ASTutils_printIndent(indent + 1, out, levels, false);
        out << "Condition: (null)" << endl;
    }

    // Výpis těla cyklu
    if(node->body != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        if (useColors)
            out << COLOR_BLUE << "Body:" << COLOR_RESET << endl;
        else
            out << "Body:" << endl;
        ASTutils_printStatementNode(node->body, out, indent + 2, useColors, levels);
    } else {
        // Pokud tělo cyklu neexistuje
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Body: (null)" << endl;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
}  // ASTutils_printWhileNode()

/**
 * @brief Vytiskne uzel AST_NODE_EXPR.
 */
void ASTutils_printExprNode(AST_ExprNode *node, ostream &out, int indent, bool useColors, vector<bool>  &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení a typu výrazu
    ASTutils_printIndent(indent, out, levels, isLastChild);

    if (useColors)
        out << COLOR_BLUE << "Expression: " << COLOR_RESET;
    else
        out << "Expression: ";

    levels.push_back(!isLastChild);

    // Rozlišení typu výrazu pomocí switch
    switch(node->exprType) {
        case AST_EXPR_LITERAL:
            // Výraz je literál
            if (useColors)
                out << COLOR_GOLD << "Literal" << COLOR_RESET << endl;
            else
                out << "Literal" << endl;
            if(node->expression != nullptr) {
                // Aktualizace vektoru úrovní
                ASTutils_printLiteralNode((AST_VarNode*)node->expression, out, indent + 1, useColors, levels, true);
            } else {
                ASTutils_printIndent(indent + 1, out, levels, true);
                out << "Literal: (null)" << endl;
            }
            break;
        case AST_EXPR_VARIABLE:
            // Výraz je proměnná
            if (useColors)
                out << COLOR_GOLD << "Variable" << COLOR_RESET << endl;
            else
                out << "Variable" << endl;
            if(node->expression != nullptr) {
                // Aktualizace vektoru úrovní
                ASTutils_printVarNode((AST_VarNode*)node->expression, out, indent + 1, useColors, levels, true);
            } else {
                ASTutils_printIndent(indent + 1, out, levels, true);
                out << "Variable: (null)" << endl;
            }
            break;
        case AST_EXPR_FUN_CALL:
            // Výraz je volání funkce
            if (useColors)
                out << COLOR_GOLD << "Function Call" << COLOR_RESET << endl;
            else
                out << "Function Call" << endl;
            if(node->expression != nullptr) {
                // Aktualizace vektoru úrovní
                ASTutils_printFunCallNode((AST_FunCallNode*)node->expression, out, indent + 1, useColors, levels, true);
            } else {
                ASTutils_printIndent(indent + 1, out, levels, true);
                out << "Function Call: (null)" << endl;
            }
            break;
        case AST_EXPR_BINARY_OP:
            // Výraz je binární operace
            if (useColors)
                out << COLOR_GOLD << "Binary Operation" << COLOR_RESET << endl;
            else
                out << "Binary Operation" << endl;
            if(node->expression != nullptr) {
                // Aktualizace vektoru úrovní
                ASTutils_printBinOpNode((AST_BinOpNode*)node->expression, out, indent + 1, useColors, levels, true);
            } else {
                ASTutils_printIndent(indent + 1, out, levels, true);
                out << "Binary Operation: (null)" << endl;
            }
            break;
        default:
            // Neznámý typ výrazu
            if (useColors)
                out << COLOR_GOLD << "Unknown Expression Type" << COLOR_RESET << endl;
            else
                out << "Unknown Expression Type" << endl;
            break;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
}  // ASTutils_printExprNode()

/**
 * @brief Vytiskne uzel AST_NODE_BIN_OP.
 */
void ASTutils_printBinOpNode(AST_BinOpNode *node, ostream &out, int indent, bool useColors, vector<bool>  &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení a operátoru
    ASTutils_printIndent(indent, out, levels, isLastChild);
    if (useColors)
        out << COLOR_BLUE << "Operator: " << COLOR_RESET;
    else
        out << "Operator: ";
    
    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);
    
    // Rozlišení typu operátoru pomocí switch
    switch(node->op) {
        case AST_OP_EQUAL:
            // Operátor rovnosti ==
            if (useColors)
                out << COLOR_GOLD << "==" << COLOR_RESET << endl;
            else
                out << "==" << endl;
            break;
        case AST_OP_NOT_EQUAL:
            // Operátor nerovnosti !=
            if (useColors)
                out << COLOR_GOLD << "!=" << COLOR_RESET << endl;
            else
                out << "!=" << endl;
            break;
        case AST_OP_LESS_THAN:
            // Operátor menší než <
            if (useColors)
                out << COLOR_GOLD << "<" << COLOR_RESET << endl;
            else
                out << "<" << endl;
            break;
        case AST_OP_GREATER_THAN:
            // Operátor větší než >
            if (useColors)
                out << COLOR_GOLD << ">" << COLOR_RESET << endl;
            else
                out << ">" << endl;
            break;
        case AST_OP_LESS_EQUAL:
            // Operátor menší nebo rovno <=
            if (useColors)
                out << COLOR_GOLD << "<=" << COLOR_RESET << endl;
            else
                out << "<=" << endl;
            break;
        case AST_OP_GREATER_EQUAL:
            // Operátor větší nebo rovno >=
            if (useColors)
                out << COLOR_GOLD << ">=" << COLOR_RESET << endl;
            else
                out << ">=" << endl;
            break;
        case AST_OP_ADD:
            // Operátor sčítání +
            if (useColors)
                out << COLOR_GOLD << "+" << COLOR_RESET << endl;
            else
                out << "+" << endl;
            break;
        case AST_OP_SUBTRACT:
            // Operátor odčítání -
            if (useColors)
                out << COLOR_GOLD << "-" << COLOR_RESET << endl;
            else
                out << "-" << endl;
            break;
        case AST_OP_MULTIPLY:
            // Operátor násobení *
            if (useColors)
                out << COLOR_GOLD << "*" << COLOR_RESET << endl;
            else
                out << "*" << endl;
            break;
        case AST_OP_DIVIDE:
            // Operátor dělení /
            if (useColors)
                out << COLOR_GOLD << "/" << COLOR_RESET << endl;
            else
                out << "/" << endl;
            break;
        case AST_OP_ASSIGNEMENT:
            // Operátor přiřazení =
            if (useColors)
                out << COLOR_GOLD << "=" << COLOR_RESET << endl;
            else
                out << "=" << endl;
            break;
        default:
            // Neznámý operátor
            if (useColors)
                out << COLOR_GOLD << "Unknown Operator" << COLOR_RESET << endl;
            else
                out << "Unknown Operator" << endl;
            break;
    }

    // Výpis levého operandu, pokud existuje
    if(node->left != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, false);

        if (useColors)
            out << COLOR_BLUE << "Left Operand:" << COLOR_RESET << endl;
        else
            out << "Left Operand:" << endl;
        ASTutils_printExprNode(node->left, out, indent + 2, useColors, levels, true);
    } else {
        // Pokud levý operand neexistuje
        ASTutils_printIndent(indent + 1, out, levels, false);
        out << "Left Operand: (null)" << endl;
    }

    // Výpis pravého operandu, pokud existuje
    if(node->right != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        if (useColors)
            out << COLOR_BLUE << "Right Operand:" << COLOR_RESET << endl;
        else
            out << "Right Operand:" << endl;
        ASTutils_printExprNode(node->right, out, indent + 2, useColors, levels, true);
    } else {
        // Pokud pravý operand neexistuje
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Right Operand: (null)" << endl;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
}  // ASTutils_printBinOpNode()

/**
 * @brief Vytiskne uzel AST_NODE_LITERAL.
 */
void ASTutils_printLiteralNode(AST_VarNode *node, ostream &out, int indent, bool useColors, vector<bool>  &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení a typu literálu
    ASTutils_printIndent(indent, out, levels, isLastChild);

    // Tisknutí typu literálu
    if (useColors)
        out << COLOR_GOLD << "Literal: " << COLOR_RESET;
    else
        out << "Literal: ";

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Rozlišení typu literálu pomocí switch
    switch(node->literalType) {
        case AST_LITERAL_INT:
            // Literál typu integer
            if (node->value != nullptr)
                out << "Integer, Value: " << *(int*)(node->value) << endl;
            else
                out << "Integer, Value: (null)" << endl;
            break;
        case AST_LITERAL_FLOAT:
            // Literál typu float
            if (node->value != nullptr)
                out << "Float, Value: " << *(double*)(node->value) << endl;
            else
                out << "Float, Value: (null)" << endl;
            break;
        case AST_LITERAL_STRING:
            // Literál typu string
            if (node->value != nullptr)
                out << "String, Value: " << (char*)(node->value) << endl;
            else
                out << "String, Value: (null)" << endl;
            break;
        case AST_LITERAL_NULL:
            // Literál typu null
            out << "Null" << endl;
            break;
        default:
            // Neznámý typ literálu
            out << "Unknown Literal Type" << endl;
            break;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();

}  // ASTutils_printLiteralNode()

/**
 * @brief Vytiskne uzel AST_NODE_VAR_DEF.
 */
void ASTutils_printVarNode(AST_VarNode *node, ostream &out, int indent, bool useColors, vector<bool> &levels, bool isLastChild) {
    if(node == nullptr) return;

    // Tisknutí odsazení
    ASTutils_printIndent(indent, out, levels, isLastChild);

    // Ověření existence identifikátoru a tisknutí identifikátoru
    if (node->identifier != nullptr && node->identifier->str != nullptr) { 
        char *identifier = string_toConstChar(node->identifier);
        if (useColors)
            out << COLOR_GOLD << "Variable: " << COLOR_RESET << identifier << endl;
        else
            out << "Variable: " << identifier << endl;
        free(identifier);
    } else {
        // Pokud `identifier` nebo `identifier->str` není platný, ošetříme výstup
        out << "Variable: (null)" << endl;
    }

    // Aktualizace vektoru úrovní
    levels.push_back(!isLastChild);

    // Výpis hodnoty proměnné, pokud existuje
    if(node->value != nullptr) {
        ASTutils_printIndent(indent + 1, out, levels, true);
        if (useColors)
            out << COLOR_BLUE << "Value:" << COLOR_RESET << endl;
        else
            out << "Value:" << endl;
        // Zjistíme typ hodnoty a podle toho zavoláme příslušnou funkci
        AST_NodeType valueType = *((AST_NodeType*)(node->value)); // Assume the first field is the type
        switch(valueType) {
            case AST_LITERAL_NODE:
                ASTutils_printLiteralNode((AST_VarNode*)node->value, out, indent + 2, useColors, levels, true);
                break;
            case AST_EXPR_NODE:
                ASTutils_printExprNode((AST_ExprNode*)node->value, out, indent + 2, useColors, levels, true);
                break;
            default:
                ASTutils_printIndent(indent + 2, out, levels, true);
                out << "Unknown Value Type" << endl;
                break;
        }
    } else {
        // Pokud `value` není platný, ošetříme výstup
        ASTutils_printIndent(indent + 1, out, levels, true);
        out << "Value: (null)" << endl;
    }

    // Odstranit současnou úroveň z vektoru pro tisk svislice
    levels.pop_back();
} // ASTutils_printVarNode()

/**
 * @brief Pomocná funkce pro výpis odsazení
 */
void ASTutils_printIndent(int indent, ostream &out, const vector<bool> &levels, bool isLastChild) {
    for (int i = 0; i < indent; ++i) {
        if ((size_t)i < levels.size()) {
            if (levels[i])
                out << "│   ";
            else
                out << "    ";
           //out << "│   ";
        } else {
            out << "    ";
        }
    }
    if (indent > 0) {
        out << (isLastChild ? "└── " : "├── ");
    } 
} // ASTutils_printIndent()


/*** Konec souboru ast_test_utils.cpp ***/
