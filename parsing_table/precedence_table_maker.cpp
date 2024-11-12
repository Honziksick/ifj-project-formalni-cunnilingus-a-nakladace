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
    T_PREC_COMMA,       // Symbol čárky ","
    T_PREC_DOLLAR,      // Symbol konce vstupu "$"
    TERMINAL_COUNT       // Počet terminálů použité v precedenční syntaktické analýze
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
    {T_PREC_GREATER_THAN_OR_EQUAL, 1},
    {T_PREC_COMMA, 0}           // Přidání čárky s nejnižší precedencí
};


PrecedenceTableEntry precedenceTable[TERMINAL_COUNT];

void initializePrecedenceTable() {
    // Inicializace všech položek na SYNTAX_ERROR
    for (int i = 0; i < TERMINAL_COUNT; ++i) {
        precedenceTable[i].key = (Terminal)i;
        for (int j = 0; j < TERMINAL_COUNT; ++j) {
            precedenceTable[i].value[j] = SYNTAX_ERROR;
        }
    }

    // Definice operátorů a jejich precedencí
    // Vyšší číslo = vyšší precedence
    struct OperatorPrecedence {
        Terminal terminal;
        int precedenceLevel;
    };

    vector<OperatorPrecedence> operatorPrecedences = {
        {T_PREC_DOT, 4},               // Operátor tečka má vysokou precedenci
        {T_PREC_LEFT_BRACKET, 4},      // Operátor volání funkce má stejnou precedenci jako tečka
        {T_PREC_MULTIPLICATION, 3},
        {T_PREC_DIVISION, 3},
        {T_PREC_PLUS, 2},
        {T_PREC_MINUS, 2},
        {T_PREC_IDENTITY, 1},
        {T_PREC_NOT_EQUAL, 1},
        {T_PREC_LESS_THAN, 1},
        {T_PREC_GREATER_THAN, 1},
        {T_PREC_LESS_THAN_OR_EQUAL, 1},
        {T_PREC_GREATER_THAN_OR_EQUAL, 1},
        {T_PREC_COMMA, 0}              // Čárka má nejnižší precedenci
    };

    // Nastavení precedenčních vztahů pro operátory
    for (OperatorPrecedence op1 : operatorPrecedences) {
        Terminal a = op1.terminal;
        for (OperatorPrecedence op2 : operatorPrecedences) {
            Terminal b = op2.terminal;
            if (op1.precedenceLevel > op2.precedenceLevel) {
                precedenceTable[a].value[b] = PREC_GREATER;
            } else if (op1.precedenceLevel < op2.precedenceLevel) {
                precedenceTable[a].value[b] = PREC_LESS;
            } else {
                // Stejná precedence, levostranná asociativita
                precedenceTable[a].value[b] = PREC_GREATER;
            }
        }
        // Operátory před operandy
        for (Terminal b : operands) {
            precedenceTable[a].value[b] = PREC_LESS;
        }
        // Operátory před levou závorkou
        precedenceTable[a].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
        // Operátory před pravou závorkou a koncem vstupu
        precedenceTable[a].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
        precedenceTable[a].value[T_PREC_DOLLAR] = PREC_GREATER;
        // Operátory před čárkou
        precedenceTable[a].value[T_PREC_COMMA] = PREC_GREATER;
        // Zakázání tečky a 'ifj' po operátorech
        precedenceTable[a].value[T_PREC_DOT] = SYNTAX_ERROR;
        precedenceTable[a].value[T_PREC_IFJ] = SYNTAX_ERROR;
    }

    // Nastavení precedenčních vztahů pro operandy
    for (Terminal a : operands) {
        // Po operandu mohou následovat operátory
        for (OperatorPrecedence op : operatorPrecedences) {
            precedenceTable[a].value[op.terminal] = PREC_LESS;
        }
        // Po operandu může následovat pravá závorka nebo konec vstupu
        precedenceTable[a].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
        precedenceTable[a].value[T_PREC_DOLLAR] = PREC_GREATER;
        // Po operandu může následovat čárka (oddělení argumentů)
        precedenceTable[a].value[T_PREC_COMMA] = PREC_GREATER;
        // Zakázání tečky a levé závorky po operandu
        precedenceTable[a].value[T_PREC_DOT] = SYNTAX_ERROR;
        precedenceTable[a].value[T_PREC_LEFT_BRACKET] = SYNTAX_ERROR;
        // Zakázání 'ifj' po operandu
        precedenceTable[a].value[T_PREC_IFJ] = SYNTAX_ERROR;
    }

    // Nastavení precedenčních vztahů pro 'ifj'
    // Po 'ifj' může následovat pouze tečka
    precedenceTable[T_PREC_IFJ].value[T_PREC_DOT] = PREC_EQUAL;
    // Zakázání ostatních tokenů po 'ifj'
    for (int j = 0; j < TERMINAL_COUNT; ++j) {
        if (j != T_PREC_DOT) {
            precedenceTable[T_PREC_IFJ].value[j] = SYNTAX_ERROR;
        }
    }

    // Nastavení precedenčních vztahů pro tečku '.'
    // Po '.' musí následovat 'id'
    precedenceTable[T_PREC_DOT].value[T_PREC_ID] = PREC_EQUAL;
    // Zakázání ostatních tokenů po '.'
    for (int j = 0; j < TERMINAL_COUNT; ++j) {
        if (j != T_PREC_ID) {
            precedenceTable[T_PREC_DOT].value[j] = SYNTAX_ERROR;
        }
    }

    // Nastavení precedenčních vztahů pro 'id'
    // Po 'id' může následovat levá závorka (volání funkce) nebo operátor
    precedenceTable[T_PREC_ID].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    // Po 'id' mohou následovat operátory
    for (OperatorPrecedence op : operatorPrecedences) {
        precedenceTable[T_PREC_ID].value[op.terminal] = PREC_LESS;
    }
    // Po 'id' může následovat pravá závorka nebo konec vstupu
    precedenceTable[T_PREC_ID].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
    precedenceTable[T_PREC_ID].value[T_PREC_DOLLAR] = PREC_GREATER;
    // Po 'id' může následovat čárka
    precedenceTable[T_PREC_ID].value[T_PREC_COMMA] = PREC_GREATER;
    // Zakázání 'ifj' a '.' po 'id'
    precedenceTable[T_PREC_ID].value[T_PREC_IFJ] = SYNTAX_ERROR;
    precedenceTable[T_PREC_ID].value[T_PREC_DOT] = SYNTAX_ERROR;

    // Nastavení precedenčních vztahů pro levou závorku '('
    // Po '(' mohou následovat operandy, 'ifj' a další '('
    for (Terminal b : operands) {
        precedenceTable[T_PREC_LEFT_BRACKET].value[b] = PREC_LESS;
    }
    precedenceTable[T_PREC_LEFT_BRACKET].value[T_PREC_IFJ] = PREC_LESS;
    precedenceTable[T_PREC_LEFT_BRACKET].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    // Zakázání ostatních tokenů po '('
    for (int j = 0; j < TERMINAL_COUNT; ++j) {
        if (!(std::find(operands.begin(), operands.end(), (Terminal)j) != operands.end() ||
              j == T_PREC_IFJ ||
              j == T_PREC_LEFT_BRACKET)) {
            precedenceTable[T_PREC_LEFT_BRACKET].value[j] = SYNTAX_ERROR;
        }
    }

    // Nastavení precedenčních vztahů pro pravou závorku ')'
    // Po ')' mohou následovat operátory, pravá závorka, čárka nebo konec vstupu
    for (OperatorPrecedence op : operatorPrecedences) {
        precedenceTable[T_PREC_RIGHT_BRACKET].value[op.terminal] = PREC_LESS;
    }
    // Pravá závorka může být následována pravou závorkou, čárkou nebo koncem vstupu
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_COMMA] = PREC_GREATER;
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_DOLLAR] = PREC_GREATER;
    // Zakázání 'ifj' a '.' po ')'
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_IFJ] = SYNTAX_ERROR;
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_DOT] = SYNTAX_ERROR;
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_LEFT_BRACKET] = SYNTAX_ERROR;
    precedenceTable[T_PREC_RIGHT_BRACKET].value[T_PREC_ID] = SYNTAX_ERROR;

    // Nastavení precedenčních vztahů pro čárku ','
    // Čárka má nízkou precedenci, umožňuje oddělení argumentů
    // Po čárce očekáváme výraz (operand), 'ifj', nebo '('
    precedenceTable[T_PREC_COMMA].value[T_PREC_ID] = PREC_LESS;
    precedenceTable[T_PREC_COMMA].value[T_PREC_IFJ] = PREC_LESS;
    precedenceTable[T_PREC_COMMA].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    precedenceTable[T_PREC_COMMA].value[T_PREC_COMMA] = PREC_LESS;
    // Po čárce může následovat pravá závorka (volitelná čárka)
    precedenceTable[T_PREC_COMMA].value[T_PREC_RIGHT_BRACKET] = PREC_GREATER;
    precedenceTable[T_PREC_COMMA].value[T_PREC_DOLLAR] = PREC_GREATER;
    // Zakázání '.' a dalších operátorů po čárce
    precedenceTable[T_PREC_COMMA].value[T_PREC_DOT] = SYNTAX_ERROR;
    precedenceTable[T_PREC_COMMA].value[T_PREC_IFJ] = SYNTAX_ERROR;
    // Další zakázané tokeny po čárce
    for (int j = 0; j < TERMINAL_COUNT; ++j) {
        if (j != T_PREC_ID && j != T_PREC_IFJ && j != T_PREC_LEFT_BRACKET && j != T_PREC_COMMA && j != T_PREC_RIGHT_BRACKET && j != T_PREC_DOLLAR) {
            precedenceTable[T_PREC_COMMA].value[j] = SYNTAX_ERROR;
        }
    }

    // Nastavení precedenčních vztahů pro konec vstupu '$'
    // Počáteční tokeny mohou být operandy, 'ifj', '(' a čárka (pro prázdné argumenty)
    for (Terminal b : operands) {
        precedenceTable[T_PREC_DOLLAR].value[b] = PREC_LESS;
    }
    precedenceTable[T_PREC_DOLLAR].value[T_PREC_IFJ] = PREC_LESS;
    precedenceTable[T_PREC_DOLLAR].value[T_PREC_LEFT_BRACKET] = PREC_LESS;
    precedenceTable[T_PREC_DOLLAR].value[T_PREC_COMMA] = PREC_LESS;
    // Konec vstupu může být následován pouze koncem vstupu
    precedenceTable[T_PREC_DOLLAR].value[T_PREC_DOLLAR] = PREC_EQUAL;
    // Zakázání ostatních tokenů po '$'
    for (int j = 0; j < TERMINAL_COUNT; ++j) {
        if (!(std::find(operands.begin(), operands.end(), (Terminal)j) != operands.end() ||
              j == T_PREC_IFJ ||
              j == T_PREC_LEFT_BRACKET ||
              j == T_PREC_COMMA)) {
            precedenceTable[T_PREC_DOLLAR].value[j] = SYNTAX_ERROR;
        }
    }

    // Zakázání použití 'ifj' a '.' mimo povolenou kombinaci
    for (int i = 0; i < TERMINAL_COUNT; ++i) {
        if ((Terminal)i != T_PREC_IFJ) {
            precedenceTable[i].value[T_PREC_IFJ] = SYNTAX_ERROR;
        }
        if ((Terminal)i != T_PREC_DOT) {
            precedenceTable[i].value[T_PREC_DOT] = SYNTAX_ERROR;
        }
    }
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
        case T_PREC_COMMA: return "T_PREC_COMMA";
        case T_PREC_DOLLAR: return "T_PREC_DOLLAR";
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
    cout << "const struct PrecedenceTable precedenceTable[PREC_TERMINAL_COUNT] = {" << endl << "    ";
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
