#ifndef NONTERMINALS_H
#define NONTERMINALS_H

enum NonTerminal {
    NT_PROGRAM         = 0,     /**<  Neterminál pro program  */
    NT_PROLOGUE        = 1,     /**<  Neterminál pro prolog  */
    NT_FUN_DEF_LIST    = 2,     /**<  Neterminál pro seznam definic funkcí  */
    NT_FUN_DEF         = 3,     /**<  Neterminál pro definici funkce  */
    NT_PARAMETERS      = 4,     /**<  Neterminál pro parametry  */
    NT_PARAM_LIST      = 5,     /**<  Neterminál pro seznam parametrů  */
    NT_PARAM           = 6,     /**<  Neterminál pro parametr  */
    NT_PARAM_LIST_REST = 7,     /**<  Neterminál pro zbytek seznamu parametrů  */
    NT_RETURN_TYPE     = 8,     /**<  Neterminál pro návratový typ  */
    NT_DATA_TYPE       = 9,     /**<  Neterminál pro datový typ  */
    NT_STATEMENT_LIST  = 10,    /**<  Neterminál pro seznam příkazů  */
    NT_STATEMENT       = 11,    /**<  Neterminál pro příkaz  */
    NT_STATEMENT_REST  = 12,    /**<  Neterminál pro zbytek příkazu  */
    NT_THROW_AWAY      = 13,    /**<  Neterminál pro zahození návratové hodnoty  */
    NT_VAR_DEF         = 14,    /**<  Neterminál pro definici proměnné  */
    NT_MODIFIABLE      = 15,    /**<  Neterminál pro modifikovatelnou hodnotu  */
    NT_POSSIBLE_TYPE   = 16,    /**<  Neterminál pro možný typ  */
    NT_IF              = 17,    /**<  Neterminál pro podmínku if  */
    NT_NULL_COND       = 18,    /**<  Neterminál pro podmínku null  */
    NT_SEQUENCE        = 19,    /**<  Neterminál pro sekvenci příkazů (blok)  */
    NT_WHILE           = 20,    /**<  Neterminál pro cyklus while  */
    NT_ARGUMENTS       = 21,    /**<  Neterminál pro argumenty  */
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
    "THROW_AWAY",
    "VAR_DEF",
    "MODIFIABLE",
    "POSSIBLE_TYPE",
    "IF",
    "NULL_COND",
    "SEQUENCE",
    "WHILE",
    "ARGUMENTS",
};
#endif // NONTERMINALS_H
