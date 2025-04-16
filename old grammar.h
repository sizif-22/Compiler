#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
using namespace std;

struct Production {
    vector<string> symbols; // e.g., {"a", "A"} or {"ε"}
    Production* next;
};

struct GrammarRule {
    string nonTerminal;
    Production* productions;
    GrammarRule* next;
};

// Function to trim whitespace
inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Load grammar into linked list
GrammarRule* loadGrammar(const string& filename) {
    ifstream infile(filename);
    string line;
    GrammarRule* head = nullptr;
    GrammarRule* lastRule = nullptr;

    while (getline(infile, line)) {
        if (line.empty()) continue;

        size_t pos = line.find("::=");
        string left = trim(line.substr(0, pos));
        string right = trim(line.substr(pos + 3));

        // Find or create the rule node
        GrammarRule* rule = head;
        while (rule && rule->nonTerminal != left)
            rule = rule->next;

        if (!rule) {
            rule = new GrammarRule{left, nullptr, nullptr};
            if (!head) head = rule;
            else lastRule->next = rule;
            lastRule = rule;
        }

        // Create a new production
        istringstream ss(right);
        string token;
        Production* prod = new Production;
        prod->next = nullptr;

        while (ss >> token) {
            prod->symbols.push_back(token);
        }

        // Append production to the rule
        if (!rule->productions) {
            rule->productions = prod;
        } else {
            Production* p = rule->productions;
            while (p->next) p = p->next;
            p->next = prod;
        }
    }

    return head;
}

