#ifndef RULES_H
#define RULES_H

#include <string>
#include <array>
#include <vector>
#include <variant>
#include <map>
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
    {NT_PROLOGUE, {T_CONST, T_IFJ, T_ASSIGNMENT, T_IMPORT, T_LEFT_BRACKET, T_CALL_PRECEDENCE, T_RIGHT_BRACKET, T_SEMICOLON}},
    {NT_FUN_DEF_LIST, {NT_FUN_DEF, NT_FUN_DEF_LIST}},
    {NT_FUN_DEF_LIST, {T_EPSILON}},
    {NT_FUN_DEF, {T_PUB, T_FN, T_ID, T_LEFT_BRACKET, NT_PARAMETERS, T_RIGHT_BRACKET, NT_RETURN_TYPE, NT_SEQUENCE}},
    {NT_PARAMETERS, {NT_PARAM_LIST}},
    {NT_PARAMETERS, {T_EPSILON}},
    {NT_PARAM_LIST, {NT_PARAM, NT_PARAM_LIST_REST}},
    {NT_PARAM_LIST_REST, {T_COMMA, NT_PARAM_LIST}},
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
    {NT_STATEMENT, {T_DUMP, T_ASSIGNMENT, NT_THROW_AWAY, T_SEMICOLON}},
    {NT_STATEMENT, {NT_IF}},
    {NT_STATEMENT, {NT_WHILE}},
    {NT_STATEMENT, {NT_RETURN, T_SEMICOLON}},
    {NT_VAR_DEF, {NT_MODIFIABLE, T_ID, NT_POSSIBLE_TYPE, T_ASSIGNMENT, T_CALL_PRECEDENCE}},
    {NT_MODIFIABLE, {T_VAR}},
    {NT_MODIFIABLE, {T_CONST}},
    {NT_POSSIBLE_TYPE, {T_COLON, NT_DATA_TYPE}},
    {NT_POSSIBLE_TYPE, {T_EPSILON}},
    {NT_STATEMENT_REST, {T_ASSIGNMENT, T_CALL_PRECEDENCE}},
    {NT_STATEMENT_REST, {T_LEFT_BRACKET, NT_ARGUMENTS, T_RIGHT_BRACKET}},
    {NT_STATEMENT_REST, {T_IFJ, T_DOT, T_ID, T_LEFT_BRACKET, NT_ARGUMENTS, T_RIGHT_BRACKET}},
    {NT_THROW_AWAY, {T_CALL_PRECEDENCE}},
    {NT_THROW_AWAY, {T_LEFT_BRACKET, NT_ARGUMENTS, T_RIGHT_BRACKET}},
    {NT_THROW_AWAY, {T_IFJ, T_DOT, T_ID, T_LEFT_BRACKET, NT_ARGUMENTS, T_RIGHT_BRACKET}},
    {NT_IF, {T_IF, T_LEFT_BRACKET, T_CALL_PRECEDENCE, T_RIGHT_BRACKET, NT_NULL_COND, NT_SEQUENCE, T_ELSE, NT_SEQUENCE}},
    {NT_NULL_COND, {T_PIPE, T_ID, T_PIPE}},
    {NT_NULL_COND, {T_EPSILON}},
    {NT_SEQUENCE, {T_LEFT_CURLY_BRACKET, NT_STATEMENT_LIST, T_RIGHT_CURLY_BRACKET}},
    {NT_WHILE, {T_WHILE, T_LEFT_BRACKET, T_CALL_PRECEDENCE, T_RIGHT_BRACKET, NT_NULL_COND, NT_SEQUENCE}},
    {NT_RETURN, {T_RETURN, NT_RETURN_REST}},
    {NT_RETURN_REST, {T_CALL_PRECEDENCE}},
    {NT_RETURN_REST, {T_EPSILON}},
    {NT_ARGUMENTS, {NT_ARG_LIST}},
    {NT_ARGUMENTS, {T_EPSILON}},
    {NT_ARG_LIST, {T_CALL_PRECEDENCE, NT_ARG}},
    {NT_ARG, {T_COMMA, T_CALL_PRECEDENCE, NT_ARG}},
    {NT_ARG, {T_EPSILON}},
};

// Mapování hodnot na odpovídající řetězce
std::map<int, std::string> valueMap = {
    {-1, "SYNTAX_ERROR"},
    {0, "PROGRAM"},
    {1, "PROLOGUE"},
    {2, "FUN_DEF_LIST_1"},
    {3, "FUN_DEF_LIST_2"},
    {4, "FUN_DEF"},
    {5, "PARAMETERS_1"},
    {6, "PARAMETERS_2"},
    {7, "PARAM_LIST"},
    {8, "PARAM_LIST_REST_1"},
    {9, "PARAM_LIST_REST_2"},
    {10, "PARAM"},
    {11, "RETURN_TYPE_1"},
    {12, "RETURN_TYPE_2"},
    {13, "DATA_TYPE_1"},
    {14, "DATA_TYPE_2"},
    {15, "DATA_TYPE_3"},
    {16, "DATA_TYPE_4"},
    {17, "DATA_TYPE_5"},
    {18, "DATA_TYPE_6"},
    {19, "STATEMENT_LIST_1"},
    {20, "STATEMENT_LIST_2"},
    {21, "STATEMENT_1"},
    {22, "STATEMENT_2"},
    {23, "STATEMENT_3"},
    {24, "STATEMENT_4"},
    {25, "STATEMENT_5"},
    {26, "STATEMENT_6"},
    {27, "VAR_DEF"},
    {28, "MODIFIABLE_1"},
    {29, "MODIFIABLE_2"},
    {30, "POSSIBLE_TYPE_1"},
    {31, "POSSIBLE_TYPE_2"},
    {32, "STATEMENT_REST_1"},
    {33, "STATEMENT_REST_2"},
    {34, "STATEMENT_REST_3"},
    {35, "THROW_AWAY_1"},
    {36, "THROW_AWAY_2"},
    {37, "THROW_AWAY_3"},
    {38, "IF"},
    {39, "NULL_COND_1"},
    {40, "NULL_COND_2"},
    {41, "SEQUENCE"},
    {42, "WHILE"},
    {43, "RETURN"},
    {44, "RETURN_REST_1"},
    {45, "RETURN_REST_2"},
    {46, "ARGUMENTS_1"},
    {47, "ARGUMENTS_2"},
    {48, "ARG_LIST"},
    {49, "ARG_1"},
    {50, "ARG_2"}
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
