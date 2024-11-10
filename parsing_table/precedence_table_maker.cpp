#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

enum Terminal {
    T_PREC_ID,
    T_PREC_INT_LITERAL,
    T_PREC_FLOAT_LITERAL,
    T_PREC_STRING_LITERAL,
    T_PREC_NULL_LITERAL,
    T_PREC_IFJ,
    T_PREC_DOT,
    T_PREC_LEFT_BRACKET,
    T_PREC_RIGHT_BRACKET,
    T_PREC_PLUS,
    T_PREC_MINUS,
    T_PREC_MULTIPLICATION,
    T_PREC_DIVISION,
    T_PREC_IDENTITY,
    T_PREC_NOT_EQUAL,
    T_PREC_LESS_THAN,
    T_PREC_GREATER_THAN,
    T_PREC_LESS_THAN_OR_EQUAL,
    T_PREC_GREATER_THAN_OR_EQUAL,
    T_PREC_CALL_LL, // End of input
    TERMINAL_COUNT  // Number of terminals used in precedence parsing
};

enum Precedence {
    SYNTAX_ERROR,
    PREC_LESS,
    PREC_GREATER,
    PREC_EQUAL
};

struct PrecedenceTableEntry {
    Terminal key;
    Precedence value[TERMINAL_COUNT];
};

// Arrays of terminals used in expressions
vector<Terminal> operands = {
    T_PREC_ID,
    T_PREC_INT_LITERAL,
    T_PREC_FLOAT_LITERAL,
    T_PREC_STRING_LITERAL,
    T_PREC_NULL_LITERAL,
    T_PREC_IFJ
};

vector<Terminal> operators_multiplicative = {
    T_PREC_MULTIPLICATION,
    T_PREC_DIVISION
};

vector<Terminal> operators_additive = {
    T_PREC_PLUS,
    T_PREC_MINUS
};

vector<Terminal> operators_relational = {
    T_PREC_IDENTITY,
    T_PREC_NOT_EQUAL,
    T_PREC_LESS_THAN,
    T_PREC_GREATER_THAN,
    T_PREC_LESS_THAN_OR_EQUAL,
    T_PREC_GREATER_THAN_OR_EQUAL
};

// Operator precedence levels (higher number = higher precedence)
struct OperatorPrecedence {
    Terminal terminal;
    int precedenceLevel;
};

vector<OperatorPrecedence> operatorPrecedences = {
    {T_PREC_DOT, 4},
    {T_PREC_MULTIPLICATION, 3},
    {T_PREC_DIVISION, 3},
    {T_PREC_PLUS, 2},
    {T_PREC_MINUS, 2},
    {T_PREC_IDENTITY, 1},
    {T_PREC_NOT_EQUAL, 1},
    {T_PREC_LESS_THAN, 1},
    {T_PREC_GREATER_THAN, 1},
    {T_PREC_LESS_THAN_OR_EQUAL, 1},
    {T_PREC_GREATER_THAN_OR_EQUAL, 1}
};

PrecedenceTableEntry precedenceTable[TERMINAL_COUNT];

void initializePrecedenceTable() {
    // Initialize all entries to SYNTAX_ERROR
    for (int i = 0; i < TERMINAL_COUNT; ++i) {
        precedenceTable[i].key = static_cast<Terminal>(i);
        for (int j = 0; j < TERMINAL_COUNT; ++j) {
            precedenceTable[i].value[j] = SYNTAX_ERROR;
        }
    }

    // Set precedence relations for operands
    for (Terminal a : operands) {
        // After operand, operators can follow
        for (OperatorPrecedence op : operatorPrecedences) {
            precedenceTable[a].value[op.terminal] = PREC_LESS;
        }
        // After operand, right parenthesis or dollar may follow
        precedenceTable[a].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
        precedenceTable[a].value[T_PREC_CALL_LL] = PREC_GREATER;
        // For member access and function calls
        precedenceTable[a].value[T_PREC_DOT] = PREC_LESS;
        precedenceTable[a].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    }

    // Set precedence relations for operators
    for (OperatorPrecedence op1 : operatorPrecedences) {
        Terminal a = op1.terminal;
        for (OperatorPrecedence op2 : operatorPrecedences) {
            Terminal b = op2.terminal;
            if (op1.precedenceLevel > op2.precedenceLevel) {
                precedenceTable[a].value[b] = PREC_GREATER;
            } else if (op1.precedenceLevel < op2.precedenceLevel) {
                precedenceTable[a].value[b] = PREC_LESS;
            } else {
                // Same precedence level, left-associative operators reduce
                precedenceTable[a].value[b] = PREC_GREATER;
            }
        }
        // Operators before operands
        for (Terminal b : operands) {
            precedenceTable[a].value[b] = PREC_LESS;
        }
        // Operators before left parenthesis
        precedenceTable[a].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
        // Operators before right parenthesis and dollar
        precedenceTable[a].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
        precedenceTable[a].value[T_PREC_CALL_LL] = PREC_GREATER;
        // Operators before dot (higher precedence)
        precedenceTable[a].value[T_PREC_DOT] = PREC_LESS;
    }

    // Set precedence relations for T_DOT
    precedenceTable[T_PREC_DOT].value[T_PREC_ID] = PREC_EQUAL;
    precedenceTable[T_PREC_DOT].value[T_PREC_IFJ] = PREC_EQUAL;
    precedenceTable[T_PREC_DOT].value[T_PREC_DOT] = PREC_GREATER;
    // Dot before operators and right parenthesis
    for (OperatorPrecedence op : operatorPrecedences) {
        precedenceTable[T_PREC_DOT].value[op.terminal] = PREC_GREATER;
    }
    precedenceTable[T_PREC_DOT].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
    precedenceTable[T_PREC_DOT].value[T_PREC_CALL_LL] = PREC_GREATER;
    // Dot before left parenthesis (method call)
    precedenceTable[T_PREC_DOT].value[T_PREC_LEFT_BRACKET] = PREC_LESS;

    // Set precedence relations for T_LEFT_PARENTHESIS
    precedenceTable[T_PREC_LEFT_BRACKET].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    for (Terminal b : operands) {
        precedenceTable[T_PREC_LEFT_BRACKET].value[b] = PREC_LESS;
    }
    precedenceTable[T_PREC_LEFT_BRACKET].value[T_PREC_RIGHT_BRACKET] = PREC_EQUAL;
    for (OperatorPrecedence op : operatorPrecedences) {
        precedenceTable[T_PREC_LEFT_BRACKET].value[op.terminal] = PREC_LESS;
    }

    // Set precedence relations for T_RIGHT_PARENTHESIS
    for (int i = 0; i < TERMINAL_COUNT; ++i) {
        precedenceTable[i].value[T_PREC_RIGHT_BRACKET] = (precedenceTable[i].value[T_PREC_RIGHT_BRACKET] == SYNTAX_ERROR) ? SYNTAX_ERROR : precedenceTable[i].value[T_PREC_RIGHT_BRACKET];
    }
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_CALL_LL] = PREC_GREATER;

    // Set precedence relations for T_DOLLAR
    for (Terminal b : operands) {
        precedenceTable[T_PREC_CALL_LL].value[b] = PREC_LESS;
    }
    precedenceTable[T_PREC_CALL_LL].value[T_PREC_LEFT_BRACKET] = PREC_LESS;

    // Set precedence relations for T_IFJ
    precedenceTable[T_PREC_IFJ].value[T_PREC_DOT] = PREC_LESS;
    precedenceTable[T_PREC_IFJ].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    for (OperatorPrecedence op : operatorPrecedences) {
        precedenceTable[T_PREC_IFJ].value[op.terminal] = SYNTAX_ERROR;
    }
    precedenceTable[T_PREC_IFJ].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
    precedenceTable[T_PREC_IFJ].value[T_PREC_CALL_LL] = PREC_GREATER;
}

string terminalToString(Terminal t) {
    switch(t) {
        case T_PREC_ID: return "T_PREC_ID";
        case T_PREC_INT_LITERAL: return "T_PREC_INT_LITERAL";
        case T_PREC_FLOAT_LITERAL: return "T_PREC_FLOAT_LITERAL";
        case T_PREC_STRING_LITERAL: return "T_PREC_STRING_LITERAL";
        case T_PREC_NULL_LITERAL: return "T_PREC_NULL_LITERAL";
        case T_PREC_IFJ: return "T_PREC_IFJ";
        case T_PREC_DOT: return "T_PREC_DOT";
        case T_PREC_LEFT_BRACKET: return "T_PREC_LEFT_BRACKET";
        case T_PREC_RIGHT_BRACKET: return "T_PREC_RIGHT_BRACKET";
        case T_PREC_PLUS: return "T_PREC_PLUS";
        case T_PREC_MINUS: return "T_PREC_MINUS";
        case T_PREC_MULTIPLICATION: return "T_PREC_MULTIPLICATION";
        case T_PREC_DIVISION: return "T_PREC_DIVISION";
        case T_PREC_IDENTITY: return "T_PREC_IDENTITY";
        case T_PREC_NOT_EQUAL: return "T_PREC_NOT_EQUAL";
        case T_PREC_LESS_THAN: return "T_PREC_LESS_THAN";
        case T_PREC_GREATER_THAN: return "T_PREC_GREATER_THAN";
        case T_PREC_LESS_THAN_OR_EQUAL: return "T_PREC_LESS_THAN_OR_EQUAL";
        case T_PREC_GREATER_THAN_OR_EQUAL: return "T_PREC_GREATER_THAN_OR_EQUAL";
        case T_PREC_CALL_LL: return "T_PREC_CALL_LL";
        default: return "UNKNOWN_TERMINAL";
    }
}

string precedenceToString(Precedence p) {
    switch(p) {
        case SYNTAX_ERROR: return "P_SYNTAX_ERROR";
        case PREC_LESS: return "P_LESS";
        case PREC_GREATER: return "P_GREATER";
        case PREC_EQUAL: return "P_EQUAL";
        default: return "UNKNOWN_PREC";
    }
}

void printPrecedenceTable() {
    cout << "struct PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT] = {" << endl << "    ";
    for (int i = 0; i < TERMINAL_COUNT; ++i) {
        Terminal key = precedenceTable[i].key;
        cout << "{ " << terminalToString(key) << "," << setw(36 - (terminalToString(key).length() + 2)) << setfill(' ') << "{ ";
        for (int j = 0; j < TERMINAL_COUNT; ++j) {
            Precedence prec = precedenceTable[i].value[j];
            cout << setw(15) << setfill(' ') << precedenceToString(prec);
            if (j < TERMINAL_COUNT - 1) {
                cout << ", ";
            }
        }
        cout << " } }";
        if (i < TERMINAL_COUNT - 1) {
            cout << "," << endl << "    ";
        } else {
            cout << "," << endl << "};" << endl;
        }
    }
}

// Function to find precedence relation between two terminals using binary search
Precedence getPrecedence(Terminal a, Terminal b) {
    // Binary search on rows (precedenceTable is sorted by key)
    int left = 0;
    int right = TERMINAL_COUNT - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (precedenceTable[mid].key == a) {
            return precedenceTable[mid].value[b];
        }
        if (precedenceTable[mid].key < a) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    // Error handling if terminal not found
    cerr << "Internal error: Terminal not found in precedence table.\n";
    exit(1);
}

int main() {
    initializePrecedenceTable();
    printPrecedenceTable();
    return 0;
}
