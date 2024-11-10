#include <iostream>
#include <unordered_map>
#include <variant>
#include <set>
#include <algorithm>
#include <iomanip>
#include "rules.h"
#include "nonterminals.h" // Include NonTerminal definitions
#include "terminals.h"    // Include Terminal definitions

// Define a type for the set of terminals (characters or tokens)
using namespace std;
using std::vector;
using std::string;



#define START_SYMBOL NT_PROGRAM

// Output structure
struct OutputSets {
    std::unordered_map<NonTerminal, bool> Empty; // Nullable sets for non-terminals
    
    // First sets: NonTerminal -> set of terminals
    std::unordered_map<NonTerminal, std::unordered_set<Terminal>> First;
    
    // Follow sets: NonTerminal -> set of terminals
    std::unordered_map<NonTerminal, std::unordered_set<Terminal>> Follow;
    
    // Optional: Predict sets for parsing (indexed by rule or production)
    std::unordered_map<Rule, std::unordered_set<Terminal>> PredictSets; // Rule -> TerminalSet
};


struct HashPair {
    size_t hash_value;
    Terminal terminal;

    bool operator==(const HashPair& other) const {
        return hash_value == other.hash_value && terminal == other.terminal;
    }
};



bool IsEmpty(const OutputSets& output, std::variant<NonTerminal, Terminal> symbol) {
    if(holds_alternative<NonTerminal>(symbol)) {
        return output.Empty.at(get<NonTerminal>(symbol));
    }else {
        return get<Terminal>(symbol) == T_EPSILON;
    }
}

// Function to calculate Empty sets
void calculateEmptySets(OutputSets& output, const Rules& rules) {

    // Initialize Empty sets for non-terminals
    for (int nt = 0; nt < NON_TERMINAL_COUNT; ++nt) {
        output.Empty[static_cast<NonTerminal>(nt)] = false; // Initialize non-terminals as not nullable
    }

    bool changed;
    do {
        changed = false;

        for (const auto& rule : rules) {
            bool canDeriveEpsilon = true;

            //td::cout << "Processing rule: " << rule.lhs << " -> ";
            for (const auto& symbol : rule.rhs) {
                if (std::holds_alternative<Terminal>(symbol)) {
                    Terminal term = std::get<Terminal>(symbol);
                    if (term != T_EPSILON) {
                        canDeriveEpsilon = false; // Terminals can't derive ε unless it's explicitly T_EPSILON
                        //std::cout << term << " (Terminal)" << std::endl;
                        break;
                    } else {
                        //std::cout << "ε (Epsilon)" << std::endl;
                    }
                } else {
                    NonTerminal nt = std::get<NonTerminal>(symbol);
                    //std::cout << nt << " (Non-terminal)" << std::endl;
                    if (!output.Empty[nt]) {
                        canDeriveEpsilon = false; // Non-terminal can't derive ε
                        break;
                    }
                }
            }

            if (canDeriveEpsilon) {
                if (!output.Empty[rule.lhs]) {
                    //std::cout << "Non-terminal " << rule.lhs << " can derive ε" << std::endl;
                    output.Empty[rule.lhs] = true; // The left-hand side can derive ε
                    changed = true;
                }
            }
        }
    } while (changed); // Repeat until no changes occur
}


// Function to get the First set of a terminal or non-terminal
std::unordered_set<Terminal> GetFirst(OutputSets& output, const std::variant<NonTerminal, Terminal>& symbol) {
    if (std::holds_alternative<Terminal>(symbol)) {
        return { std::get<Terminal>(symbol) }; // First(terminal) is itself
    } else {
        NonTerminal nt = std::get<NonTerminal>(symbol);
        return output.First[nt]; // First(non-terminal) from the precomputed set
    }
}


// Function to calculate First sets
void calculateFirstSets(OutputSets& output, const Rules& rules) {
    for (int nt = 0; nt < NON_TERMINAL_COUNT; ++nt) {
        output.First[static_cast<NonTerminal>(nt)] = {};
    }

    bool changed;
    do {
        changed = false;

        // Iterate over all rules
        for (const auto& rule : rules) {
            NonTerminal lhs = rule.lhs;
            std::unordered_set<Terminal>& firstLHS = output.First[lhs]; // First(A)

            const RHS& rhs = rule.rhs; // Right-hand side symbols

            bool allNullable = true; // Tracks whether all symbols on RHS are nullable

            for (size_t i = 0; i < rhs.size(); ++i) {
                const auto& symbol = rhs[i];

                // Get First set of the current symbol (terminal or non-terminal)
                std::unordered_set<Terminal> firstSet = GetFirst(output, symbol);

                // Add First(symbol) to First(lhs), except epsilon (skip if empty)
                size_t beforeSize = firstLHS.size();
                firstLHS.insert(firstSet.begin(), firstSet.end());

                if (firstLHS.size() > beforeSize) {
                    changed = true; // Something changed, so we need another iteration
                }

                // Check if the current symbol is empty (nullable)
                if (!IsEmpty(output, symbol)) {
                    allNullable = false; // If one symbol is not nullable, break
                    break;
                }
            }

            // If all symbols on the right-hand side are nullable, mark lhs as nullable
            if (allNullable) {
                output.Empty[lhs] = true;
            }
        }
    } while (changed); // Repeat until no changes occur
}


void calculateFollowSets(OutputSets& output, const Rules& rules) {
    // Step 1: Initialize Follow sets for non-terminals
    for (int nt = 0; nt < NON_TERMINAL_COUNT; ++nt) {
        output.Follow[static_cast<NonTerminal>(nt)] = {}; // Start with empty Follow sets
    }

    output.Follow[START_SYMBOL].insert(T_EOF); // Follow(S) := {$}

    bool changed;
    do {
        changed = false;

        // Iterate over all rules
        for (const auto& rule : rules) {
            NonTerminal A = rule.lhs;
            const RHS& rhs = rule.rhs; // Right-hand side symbols

            for (size_t i = 0; i < rhs.size(); ++i) {
                const auto& symbol = rhs[i];

                if (std::holds_alternative<NonTerminal>(symbol)) {
                    NonTerminal B = std::get<NonTerminal>(symbol);

                    // Check if there is a symbol following B
                    if (i + 1 < rhs.size()) {
                        const auto& followingSymbol = rhs[i + 1];
                        if (std::holds_alternative<Terminal>(followingSymbol)) {
                            // If the next symbol is a terminal, it gets added to Follow(B)
                            Terminal term = std::get<Terminal>(followingSymbol);
                            if(term != T_EPSILON){
                                if (output.Follow[B].insert(term).second) {
                                    changed = true; // A change occurred
                                }
                            }
                        } else {
                            // The next symbol is a non-terminal
                            const auto& firstC = GetFirst(output, followingSymbol);
                            size_t beforeSize = output.Follow[B].size();
                            
                            // Add First(C) to Follow(B), excluding ε
                            output.Follow[B].insert(firstC.begin(), firstC.end());
                            output.Follow[B].erase(T_EPSILON); // Remove ε if present
                            
                            if (output.Follow[B].size() > beforeSize) {
                                changed = true; // A change occurred
                            }

                            // If C is nullable, add Follow(A) to Follow(B)
                            if (IsEmpty(output, followingSymbol)) {
                                size_t beforeSize = output.Follow[B].size();
                                output.Follow[B].insert(output.Follow[A].begin(), output.Follow[A].end());
                                if (output.Follow[B].size() > beforeSize) {
                                    changed = true; // A change occurred
                                }
                            }
                        }
                    } else {
                        // If there is no symbol following B, add Follow(A) to Follow(B)
                        size_t beforeSize = output.Follow[B].size();
                        output.Follow[B].insert(output.Follow[A].begin(), output.Follow[A].end());
                        if (output.Follow[B].size() > beforeSize) {
                            changed = true; // A change occurred
                        }
                    }
                }
            }
        }
    } while (changed); // Repeat until no changes occur
}


// Function to calculate Predict sets
void calculatePredictSets(OutputSets& output, const Rules& rules) {
    // Initialize Predict sets for each rule
    for (const auto& rule : rules) {
        output.PredictSets[rule] = {}; // Start with an empty set for each rule
    }

    for (const auto& rule : rules) {
        NonTerminal A = rule.lhs; // Left-hand side of the production (A → α)
        const RHS& rhs = rule.rhs; // Right-hand side of the production (α)

        bool rhsIsNullable = true; // Flag to track if RHS can derive ε

        for (const auto& symbol : rhs) {
            // Get First(symbol) and insert directly into Predict set
            std::unordered_set<Terminal> currentFirstSet = GetFirst(output, symbol);
            
            // Insert directly into Predict set, skipping ε
            output.PredictSets[rule].insert(currentFirstSet.begin(), currentFirstSet.end());
            output.PredictSets[rule].erase(T_EPSILON); // Remove ε (if present)

            // Check if this symbol is nullable
            if (!IsEmpty(output, symbol)) {
                rhsIsNullable = false; // No need to check further
                break;
            }
        }

        // If RHS is nullable, add Follow(A) to the Predict set
        if (rhsIsNullable) {
            output.PredictSets[rule].insert(output.Follow[A].begin(), output.Follow[A].end());
        }
    }
}





void printEmptySets(const OutputSets& output) {
    std::cout << "Nullable (ε):\n";
    for (int nt = 0; nt < NON_TERMINAL_COUNT; ++nt) {
        if(IsEmpty(output, static_cast<NonTerminal>(nt))) {
            std::cout << "Non-terminal " << invNonTerminalMap[nt] << " can derive ε " << std::endl; // Use .at() instead of []
        }
    }
    
    std::cout << "Terminal " << invTerminalMap[T_EPSILON] << " can derive ε " << std::endl;

}


void printFirstSets(const OutputSets& output) {
    std::cout << "\nFirst Sets:\n";
    
    for (int nt = 0; nt < NON_TERMINAL_COUNT; ++nt) {
        NonTerminal nonTerminal = static_cast<NonTerminal>(nt);
        const auto& firstSet = output.First.at(nonTerminal); // Get the First set for the current non-terminal
        
        std::cout << "First(" << invNonTerminalMap[nt] << ") = { ";
        for (Terminal t : firstSet) {
            std::cout << invTerminalMap[t] << " ";  // Adjust to print terminal symbols meaningfully
        }
        std::cout << "}\n";
    }
}


void printFollowSets(const OutputSets& output) {
    std::cout << "\nFollow Sets:\n";
    
    for (int nt = 0; nt < NON_TERMINAL_COUNT; ++nt) {
        NonTerminal nonTerminal = static_cast<NonTerminal>(nt);
        const auto& followSet = output.Follow.at(nonTerminal);
        
        std::cout << "Follow(" << invNonTerminalMap[nt] << ") = { ";
        for (Terminal t : followSet) {
            std::cout << invTerminalMap[t] << " "; // Assuming you have the terminalToString function
        }
        std::cout << "}\n";
    }
}

// Helper function to print Predict sets
void printPredictSets(OutputSets& output, Rules& ruleSet) {
    int ruleIndex = 0;
    std::cout << "\nPredict Sets:\n";
    for (auto& rule : ruleSet) {
        std::cout << "Predict(" << invNonTerminalMap[rule.lhs] << " → ";
        for (const auto& symbol : rule.rhs) {
            if (std::holds_alternative<Terminal>(symbol)) {
                Terminal t = std::get<Terminal>(symbol);
                std::cout << invTerminalMap[t] << " ";
            } else {
                NonTerminal nt = std::get<NonTerminal>(symbol);
                std::cout << invNonTerminalMap[nt] << " ";
            }
        }
        std::cout << ") (rule " << ruleIndex << ") = { ";

        const auto& predictSet = output.PredictSets[rule];
        for (const auto& t : predictSet) {
            std::cout << invTerminalMap[t] << " "; // Terminal enum printing
        }
        std::cout << "}\n";
        ruleIndex++;
    }
}


// Function to check if the grammar is LL(1), meaning it has no left recursion
bool checkNoLeftRecursiong(const OutputSets& output, const Rules& rules) {
    int ruleIndex = 0;

    for (const auto& rule : rules) {
        NonTerminal lhs = rule.lhs; // Left-hand side of the rule

        // Check First(rhs) to see if it contains any terminal that matches lhs
        const RHS& rhs = rule.rhs;
        
        // If the first symbol of rhs is a non-terminal, check its First set
        if (!rhs.empty() && std::holds_alternative<NonTerminal>(rhs[0])) {
            NonTerminal firstSymbol = std::get<NonTerminal>(rhs[0]);
            const auto& firstSet = output.First.at(firstSymbol);

            // If lhs appears in First(firstSymbol), there's left recursion
            if (firstSet.find(static_cast<Terminal>(lhs)) != firstSet.end()) {
                std::cout << "\nLeft recursion detected in rule " << ruleIndex << ": " << invNonTerminalMap[lhs] << " -> ";

                for (const auto& symbol : rhs) {
                    if (std::holds_alternative<Terminal>(symbol)) {
                        std::cout << invTerminalMap[std::get<Terminal>(symbol)] << " ";
                    } else {
                        std::cout << invNonTerminalMap[std::get<NonTerminal>(symbol)] << " ";
                    }
                }
                std::cout << std::endl;

                // Print the First set of the first symbol in rhs
                std::cout << "\nFirst(" << invNonTerminalMap[firstSymbol] << ") = { ";
                for (const auto& t : firstSet) {
                    std::cout << invTerminalMap[t] << " "; // Print each terminal in the First set
                }
                std::cout << "}\n";

                // Suggestion for resolving left recursion
                std::cout << "To resolve left recursion, consider rewriting the rule." << std::endl;




                return false; // Left recursion detected
            }
        }
        ruleIndex++;
    }
    
    return true; // No left recursion found
}


// Function to check for left recursion in a specific rule
bool checkLeftRecursion(const NonTerminal& lhs, const RHS& rhs, const Rules& rules, 
                        const OutputSets& output, std::set<NonTerminal>& visited, 
                        std::vector<NonTerminal>& recursionChain) {
    // If we've already visited this non-terminal, we have a recursion
    if (visited.count(lhs)) {
        return true; // Detected recursion
    }

    visited.insert(lhs);

    // Check each symbol in the right-hand side
    for (const auto& symbol : rhs) {
        if (std::holds_alternative<NonTerminal>(symbol)) {
            NonTerminal nextNt = std::get<NonTerminal>(symbol);

            // Find the rule for the next non-terminal
            auto nextRule = std::find_if(rules.begin(), rules.end(),
                                          [nextNt](const Rule& r) { return r.lhs == nextNt; });

            if (nextRule != rules.end()) {
                // Recur for the next non-terminal
                std::vector<NonTerminal> nextRecursionChain;
                if (checkLeftRecursion(nextNt, nextRule->rhs, rules, output, visited, nextRecursionChain)) {
                    recursionChain.push_back(nextNt); // Add to recursion chain
                    recursionChain.insert(recursionChain.end(), nextRecursionChain.begin(), nextRecursionChain.end());
                }
            }
        }
    }

    visited.erase(lhs); // Remove from visited to allow other paths
    return false; // No recursion detected for this branch
}


// Function to check if the grammar is LL(1), meaning it has no left recursion
bool checkNoLeftRecursion(const OutputSets& output, const Rules& rules, bool save_time) {
    if(save_time){
        cout << "\nNo left recursion found in the grammar.\n" << endl;
        return true;
    }
    int ruleIndex = 0;
    bool hasLeftRecursion = false; // Track if any left recursion is detected

    for (const auto& rule : rules) {
        NonTerminal lhs = rule.lhs; // Left-hand side of the rule

        // Set to track visited non-terminals to detect recursion
        std::set<NonTerminal> visited;

        // Vector to collect recursion chain
        std::vector<NonTerminal> recursionChain;

        // Check for left recursion in the current rule
        if (checkLeftRecursion(lhs, rule.rhs, rules, output, visited, recursionChain)) {
            // Print the left recursion details
            std::cout << "\nLeft recursion detected in rule " << ruleIndex << ": "
                      << invNonTerminalMap[lhs] << " -> ";

            // Print the right-hand side of the rule
            for (const auto& symbol : rule.rhs) {
                if (std::holds_alternative<Terminal>(symbol)) {
                    std::cout << invTerminalMap[std::get<Terminal>(symbol)] << " ";
                } else {
                    std::cout << invNonTerminalMap[std::get<NonTerminal>(symbol)] << " ";
                }
            }

            // Print the entire recursion chain
            std::cout << "\nRecursion chain: ";
            for (const auto& nt : recursionChain) {
                std::cout << invNonTerminalMap[nt] << " ";
            }
            std::cout << invNonTerminalMap[lhs] << " "; // Include the original lhs in the chain

            // Print the First set of the first symbol in rhs
            if (!rule.rhs.empty() && std::holds_alternative<NonTerminal>(rule.rhs[0])) {
                NonTerminal firstSymbol = std::get<NonTerminal>(rule.rhs[0]);
                const auto& firstSet = output.First.at(firstSymbol);
                std::cout << "\nFirst(" << invNonTerminalMap[firstSymbol] << ") = { ";
                for (const auto& t : firstSet) {
                    std::cout << invTerminalMap[t] << " "; // Print each terminal in the First set
                }
                std::cout << "}\n";
            }

            // Suggestion for resolving left recursion
            std::cout << "To resolve left recursion, consider rewriting the rule." << std::endl;

            hasLeftRecursion = true; // Set the flag indicating left recursion is detected
        }

        ruleIndex++;
    }

    // Print message if no left recursion is found
    if (!hasLeftRecursion) {
        std::cout << "\nNo left recursion found in the grammar." << std::endl;
    }
    
    return !hasLeftRecursion; // Return true if no left recursion found
}

size_t hashFunction(string key) {
    size_t hash = 5381;
    for(auto i : key) {
        hash = ((hash << 5) + hash) + i;
    }
    return hash;
}

void createHashPairs(vector<HashPair>& hashPairs) {
    for (const auto &pair : terminalMap) {
        size_t hash_value = hashFunction(pair.first);
        hashPairs.push_back({hash_value, pair.second});
    }

    std::sort(hashPairs.begin(), hashPairs.end(), [](const HashPair &a, const HashPair &b) {
        return a.hash_value < b.hash_value;
    });

}

void printHashPairs(vector<HashPair>& hashPairs) {
    for (const auto &pair : hashPairs) {
        std::cout << "Terminal: " << pair.terminal << " Hash: " << pair.hash_value << std::endl << std::endl;
    }
}

void printLLtable(vector<HashPair>& hashPairs, OutputSets& output, bool readable) {
    // Seřazení hashPairs podle terminálů
    std::sort(hashPairs.begin(), hashPairs.end(), [](const HashPair& a, const HashPair& b) {
        return terminalOrder[a.terminal] < terminalOrder[b.terminal];
    });

    // print header
    cout << "LLtable table[LL_TERMINAL_COUNT] = {" << endl << "    ";

    // for each terminal
    for(auto& pair : hashPairs) {
        if(readable) {
            cout << "{ " << invTerminalMap[pair.terminal] << "," << setw(28 - (invTerminalMap[pair.terminal].length() + 2)) << setfill(' ') << "{"; 
        } else {
            cout << "{ " << pair.hash_value << "ULL, {";
        }

        // for each non-terminal
        for(int i = 0; i < NON_TERMINAL_COUNT; i++) {
            int added = 0;
            int rulecouter = 0;
            vector<int> added_rule_num;
            vector<int> added_rule_lhs;
            for(auto& rule : ruleSet) {
                if(rule.lhs == i) {
                    if(output.PredictSets[rule].find(pair.terminal) != output.PredictSets[rule].end()) {
                        added_rule_num.push_back(rulecouter);
                        added_rule_lhs.push_back(rule.lhs);
                        added++;
                    }
                }
                rulecouter++;
            }
            if(added == 0) {
                cout << setfill(' ') << setw(17) << (readable ? valueMap[-1] : "-1");
            } else if(added == 1) {
                cout << setfill(' ') << setw(17) << (readable ? valueMap[added_rule_num[0]] : to_string(added_rule_num[0]));
            } else {
                cout << " ERROR( ";
                for(auto& num : added_rule_num) {
                    cout <<"rule " << num << " ";
                }
                cout << "from nonterminals: ";
                for(auto& num : added_rule_lhs) {
                    cout << invNonTerminalMap[num] << " ";
                }
                cout << ") ";
            }
            if(i != NON_TERMINAL_COUNT - 1) {
                cout << ", ";
            }
        }
        cout << " } }";
        if(pair == hashPairs.back()) {
            cout << "," << endl << "};" << endl;
        } else {
            cout << "," << endl << "    ";
        }
    }
}



int main() {
    OutputSets output; // Initialize output sets
    Rules grammarRules = ruleSet; // Use your defined rules
    vector<HashPair> hashPairs;

    // Calculate Empty sets
    calculateEmptySets(output, grammarRules);

    // Print results
    //printEmptySets(output);

    // Calculate First sets
    calculateFirstSets(output, grammarRules);

    // Print results
    //printFirstSets(output);

    // Calculate Follow sets
    calculateFollowSets(output, grammarRules);

    // Print Follow sets
    //printFollowSets(output);

    // Calculate Predict sets
    calculatePredictSets(output, grammarRules);

    // Print Predict sets
    //printPredictSets(output, grammarRules);

    // Check for left recursion
    //checkNoLeftRecursion(output, grammarRules, true);
    


    createHashPairs(hashPairs);

    //printHashPairs(hashPairs);

    printLLtable(hashPairs, output, true);

    return 0;
}

