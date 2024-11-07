#ifndef NONTERMINALS_H
#define NONTERMINALS_H

enum NonTerminal {
    NT_PROGRAM      = 0,            /**<  Neterminál pro program  */
    NT_PROLOGUE     = 1,            /**<  Neterminál pro prolog  */
    NT_FUN_DEF_LIST = 2,            /**<  Neterminál pro seznam definic funkcí  */
    NT_FUN_DEF      = 3,            /**<  Neterminál pro definici funkce  */
    NT_PARAMETERS   = 4,            /**<  Neterminál pro parametry  */
    NT_PARAM_LIST   = 5,            /**<  Neterminál pro seznam parametrů  */
    NT_PARAM        = 6,            /**<  Neterminál pro parametr  */
    NT_PARAM_LIST_REST = 7,         /**<  Neterminál pro zbytek seznamu parametrů  */
    NT_RETURN_TYPE     = 8,         /**<  Neterminál pro návratový typ  */
    NT_DATA_TYPE       = 9,         /**<  Neterminál pro datový typ  */
    NT_STATEMENT_LIST  = 10,        /**<  Neterminál pro seznam příkazů  */
    NT_STATEMENT       = 11,        /**<  Neterminál pro příkaz  */
    NT_STATEMENT_REST  = 12,        /**<  Neterminál pro zbytek příkazu  */
    NT_VAR_DEF       = 13,          /**<  Neterminál pro definici proměnné  */
    NT_MODIFIABLE    = 14,          /**<  Neterminál pro modifikovatelnou hodnotu  */
    NT_POSSIBLE_TYPE = 15,          /**<  Neterminál pro možný typ  */
    NT_IF        = 16,              /**<  Neterminál pro podmínku if  */
    NT_NULL_COND = 17,              /**<  Neterminál pro podmínku null  */
    NT_SEQUENCE  = 18,              /**<  Neterminál pro sekvenci příkazů (blok)  */
    NT_WHILE     = 19,              /**<  Neterminál pro cyklus while  */
    NT_RETURN    = 20,              /**<  Neterminál pro příkaz návratu  */
    NT_RETURN_REST     = 21,        /**<  Neterminál pro zbytek příkazu návratu  */
    NT_EXPR            = 22,        /**<  Neterminál pro výraz  */
    NT_REL_EXPR        = 23,        /**<  Neterminál pro relační výraz  */
    NT_REL_EXPR_REST   = 24,        /**<  Neterminál pro zbytek relačního výrazu  */
    NT_REL_OP          = 25,        /**<  Neterminál pro relační operátor  */
    NT_ADD_EXPR        = 26,        /**<  Neterminál pro aditivní výraz  */
    NT_ADD_EXPR_REST   = 27,        /**<  Neterminál pro zbytek aditivního výrazu  */
    NT_ADD_OP          = 28,        /**<  Neterminál pro aditivní operátor  */
    NT_MULTI_EXPR      = 29,        /**<  Neterminál pro multiplikativní výraz  */
    NT_MULTI_EXPR_REST = 30,        /**<  Neterminál pro zbytek multiplikativního výrazu  */
    NT_MULTI_OP  = 31,              /**<  Neterminál pro multiplikativní operátor  */
    NT_TERM      = 32,              /**<  Neterminál pro term  */
    NT_TERM_REST = 33,              /**<  Neterminál pro zbytek termu  */
    NT_ARGUMENTS = 34,              /**<  Neterminál pro argumenty  */
    NT_ARG_LIST  = 35,              /**<  Neterminál pro seznam argumentů  */
    NT_ARG       = 36,              /**<  Neterminál pro argument  */
    NT_LITERAL   = 37,              /**<  Neterminál pro literál  */
    NON_TERMINAL_COUNT
};

std::array<std::string, NON_TERMINAL_COUNT> invNonTerminalMap = {
    "PROGRAM",
    "PROLOGUE",
    "FUN_DEF_LIST",
    "FUN_DEF",
    "PARAMETERS",
    "PARAM_LIST",
    "PARAM",
    "PARAM_LIST_REST"
    "RETURN_TYPE",
    "DATA_TYPE",
    "STATEMENT_LIST",
    "STATEMENT",
    "STATEMENT_REST",
    "VAR_DEF",
    "MODIFIABLE",
    "POSSIBLE_TYPE",
    "IF",
    "NULL_COND",
    "SEQUENCE",
    "WHILE",
    "RETURN",
    "RETURN_REST",
    "EXPR",
    "REL_EXPR",
    "REL_EXPR_REST",
    "REL_OP",
    "ADD_EXPR",
    "ADD_EXPR_REST",
    "ADD_OP",
    "MULTI_EXPR",
    "MULTI_EXPR_REST",
    "MULTI_OP",
    "TERM",
    "TERM_REST",
    "ARGUMENTS",
    "ARG_LIST",
    "ARG",
    "LITERAL"
};
#endif // NONTERMINALS_H
