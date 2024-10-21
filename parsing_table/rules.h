#ifndef RULES_H
#define RULES_H

#include <string>
#include <array>
#include <vector>
#include <variant>
#include "nonterminals.h" // Include NonTerminal definitions
#include "terminals.h"    // Include Terminal definitions

using RHS = std::vector<std::variant<NonTerminal, Terminal>>;  // Can hold either non-terminals or terminals
// Define a type for a single production (left-hand side -> right-hand side)
struct Rule {
    NonTerminal lhs;                 // Non-terminal on the left-hand side
    RHS rhs;     // Sequence of terminals/non-terminals on the right-hand side

    bool operator==(const Rule& other) const {
        return lhs == other.lhs && rhs == other.rhs;
    }
};

using Rules = std::vector<Rule>;
Rules ruleSet = {
    {NT_PROGRAM, {NT_PROLOGUE, NT_FUN_DEF_LIST, T_EOF}},
    {NT_PROLOGUE, {T_CONST, T_IFJ, T_EQUAL, T_AT_IMPORT, T_LEFT_PARENTHESIS, T_STRING_LITERAL, T_RIGHT_PARENTHESIS, T_SEMICOLON}},
    {NT_FUN_DEF_LIST, {NT_FUN_DEF, NT_FUN_DEF_LIST}},
    {NT_FUN_DEF_LIST, {T_EPSILON}},
    {NT_FUN_DEF, {T_PUB, T_FN, T_ID, T_LEFT_PARENTHESIS, NT_PARAMETERS, T_RIGHT_PARENTHESIS, NT_RETURN_TYPE, NT_SEQUENCE}},
    {NT_PARAMETERS, {NT_PARAM_LIST}},
    {NT_PARAMETERS, {T_EPSILON}},
    {NT_PARAM_LIST, {NT_PARAM, NT_PARAM_LIST_REST}},
    {NT_PARAM_LIST_REST, {T_ID, NT_PARAM_LIST}},
    {NT_PARAM_LIST_REST, {T_EPSILON}},
    {NT_PARAM, {T_ID, T_COLON, NT_DATA_TYPE}},
    {NT_RETURN_TYPE, {NT_DATA_TYPE}},
    {NT_RETURN_TYPE, {T_VOID}},
    {NT_DATA_TYPE, {T_INT}},
    {NT_DATA_TYPE, {T_INT_OR_NULL}},
    {NT_DATA_TYPE, {T_FLOAT}},
    {NT_DATA_TYPE, {T_FLOAT_OR_NULL}},
    {NT_DATA_TYPE, {T_STRING}},
    {NT_DATA_TYPE, {T_STRING_OR_NULL}},
    {NT_STATEMENT_LIST, {NT_STATEMENT, NT_STATEMENT_LIST}},
    {NT_STATEMENT_LIST, {T_EPSILON}},
    {NT_STATEMENT, {NT_VAR_DEF, T_SEMICOLON}},
    {NT_STATEMENT, {T_ID, NT_STATEMENT_REST, T_SEMICOLON}},
    {NT_STATEMENT, {NT_IF}},
    {NT_STATEMENT, {NT_WHILE}},
    {NT_STATEMENT, {NT_RETURN, T_SEMICOLON}},
    {NT_VAR_DEF, {NT_MODIFIABLE, T_ID, NT_POSSIBLE_TYPE, T_EQUAL, NT_EXPR}},
    {NT_MODIFIABLE, {T_VAR}},
    {NT_MODIFIABLE, {T_CONST}},
    {NT_POSSIBLE_TYPE, {T_COLON, NT_DATA_TYPE}},
    {NT_POSSIBLE_TYPE, {T_EPSILON}},
    {NT_STATEMENT_REST, {T_EQUAL, NT_EXPR}},
    {NT_STATEMENT_REST, {T_LEFT_PARENTHESIS, NT_ARGUMENTS, T_RIGHT_PARENTHESIS}},
    {NT_STATEMENT_REST, {T_IFJ, T_DOT, T_ID, T_LEFT_PARENTHESIS, NT_ARGUMENTS, T_RIGHT_PARENTHESIS}},
    {NT_IF, {T_IF, T_LEFT_PARENTHESIS, NT_EXPR, T_RIGHT_PARENTHESIS, NT_NULL_COND, NT_SEQUENCE, T_ELSE, NT_SEQUENCE}},
    {NT_NULL_COND, {T_PIPE, T_ID, T_PIPE}},
    {NT_NULL_COND, {T_EPSILON}},
    {NT_SEQUENCE, {T_LEFT_CURLY_BRACKET, NT_STATEMENT_LIST, T_RIGHT_CURLY_BRACKET}},
    {NT_WHILE, {T_WHILE, T_LEFT_PARENTHESIS, NT_EXPR, T_RIGHT_PARENTHESIS, NT_NULL_COND, NT_SEQUENCE}},
    {NT_RETURN, {T_RETURN, NT_RETURN_REST}},
    {NT_RETURN_REST, {NT_EXPR}},
    {NT_RETURN_REST, {T_EPSILON}},
    {NT_EXPR, {NT_REL_EXPR}},
    {NT_REL_EXPR, {NT_ADD_EXPR, NT_REL_EXPR_REST}},
    {NT_REL_EXPR_REST, {NT_REL_OP, NT_ADD_EXPR}},
    {NT_REL_EXPR_REST, {T_EPSILON}},
    {NT_REL_OP, {T_DOUBLE_EQUAL}},
    {NT_REL_OP, {T_NOT_EQUAL}},
    {NT_REL_OP, {T_LESS_THAN}},
    {NT_REL_OP, {T_GREATER_THAN}},
    {NT_REL_OP, {T_LESS_THAN_OR_EQUAL}},
    {NT_REL_OP, {T_GREATER_THAN_OR_EQUAL}},
    {NT_ADD_EXPR, {NT_MULTI_EXPR, NT_ADD_EXPR_REST}},
    {NT_ADD_EXPR_REST, {NT_ADD_OP, NT_MULTI_EXPR, NT_ADD_EXPR_REST}},
    {NT_ADD_EXPR_REST, {T_EPSILON}},
    {NT_ADD_OP, {T_PLUS}},
    {NT_ADD_OP, {T_MINUS}},
    {NT_MULTI_EXPR, {NT_TERM, NT_MULTI_EXPR_REST}},
    {NT_MULTI_EXPR_REST, {NT_MULTI_OP, NT_TERM, NT_MULTI_EXPR_REST}},
    {NT_MULTI_EXPR_REST, {T_EPSILON}},
    {NT_MULTI_OP, {T_MULTIPLY}},
    {NT_MULTI_OP, {T_DIVIDE}},
    {NT_TERM, {T_ID, NT_TERM_REST}},
    {NT_TERM, {T_IFJ, T_DOT, T_ID, NT_TERM_REST}},
    {NT_TERM, {NT_LITERAL}},
    {NT_TERM, {T_LEFT_PARENTHESIS, NT_EXPR, T_RIGHT_PARENTHESIS}},
    {NT_TERM_REST, {T_LEFT_PARENTHESIS, NT_ARGUMENTS, T_RIGHT_PARENTHESIS}},
    {NT_TERM_REST, {T_EPSILON}},
    {NT_ARGUMENTS, {NT_ARG_LIST}},
    {NT_ARGUMENTS, {T_EPSILON}},
    {NT_ARG_LIST, {NT_EXPR, NT_ARG}},
    {NT_ARG, {T_COMMA, NT_EXPR, NT_ARG}},
    {NT_ARG, {T_EPSILON}},
    {NT_LITERAL, {T_INT_LITERAL}},
    {NT_LITERAL, {T_FLOAT_LITERAL}},
    {NT_LITERAL, {T_STRING_LITERAL}},
    {NT_LITERAL, {T_NULL_LITERAL}}
};

// Specialize std::hash for the Rule type
namespace std {
    template <>
    struct hash<Rule> {
        std::size_t operator()(const Rule& rule) const {
            std::size_t lhsHash = std::hash<int>()(static_cast<int>(rule.lhs)); // Hashing NonTerminal
            std::size_t rhsHash = 0;

            // Hash the RHS by combining the hashes of all its elements
            for (const auto& symbol : rule.rhs) {
                if (std::holds_alternative<Terminal>(symbol)) {
                    rhsHash ^= std::hash<int>()(std::get<Terminal>(symbol)) + 0x9e3779b9 + (rhsHash << 6) + (rhsHash >> 2);
                } else {
                    rhsHash ^= std::hash<int>()(std::get<NonTerminal>(symbol)) + 0x9e3779b9 + (rhsHash << 6) + (rhsHash >> 2);
                }
            }

            return lhsHash ^ rhsHash; // Combine hashes of LHS and RHS
        }
    };
}

#endif // RULES_H