#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
const string SEPARATOR = "::=";
const string EMPTYSYMBOL = "ε";

inline string trim(const string &s)
{
     size_t start = s.find_first_not_of(" \t\r\n");
     size_t end = s.find_last_not_of(" \t\r\n");
     return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

vector<string> readTerminals()
{
     vector<string> terminals = {};
     string fileName = "./terminals.txt";
     ifstream inFile(fileName);
     string line;
     while (getline(inFile, line))
     {
          if (line.empty())
               continue;
          terminals.push_back(trim(line));
     }
     return terminals;
}
vector<string> readNonTerminals()
{
     vector<string> nonTerminals = {};
     string fileName = "./grammar.txt";
     ifstream inFile(fileName);
     string line;
     while (getline(inFile, line))
     {
          if (line.empty())
               continue;
          size_t pos = line.find("::=");
          string nonTerminal = trim(line.substr(0, pos));
          nonTerminals.push_back(nonTerminal);
     }
     return nonTerminals;
}

vector<vector<string>> calculateRight(string line, size_t pos)
{
     vector<vector<string>> rightVector = {};
     vector<string> stringVector;
     string word = "";
     string right = trim(line.substr(pos));
     // cout << "right string : " << right << endl;
     for (int i = 0; i < right.length(); i++)
     {
          if (right[i] == '|')
          {
               rightVector.push_back(stringVector);
               stringVector = {};
               continue;
          }
          if (right[i] == ' ')
          {
               if (trim(word) != "")
               {
                    stringVector.push_back(word);
                    word = "";
               }
               continue;
          }
          word += right[i];
     }
     stringVector.push_back(word);
     rightVector.push_back(stringVector);
     return rightVector;
}

map<string, vector<vector<string>>> readGrammar()
{
     map<string, vector<vector<string>>> grammar;
     string fileName = "./grammar.txt";
     ifstream inFile(fileName);
     string line;
     while (getline(inFile, line))
     {
          size_t pos = line.find(SEPARATOR);
          string left = trim(line.substr(0, pos));
          vector<vector<string>> right = calculateRight(line, pos + SEPARATOR.length());
          grammar[left] = right;
     }
     return grammar;
}

const vector<string> TERMINALS = readTerminals();
const vector<string> NONTERMINALS = readNonTerminals();
const string STARTSYMBOL = NONTERMINALS[0];
map<string, vector<vector<string>>> GRAMMAR = readGrammar();

void printGrammar()
{
     for (auto &pair : GRAMMAR)
     {
          cout << pair.first << endl;
          for (auto &vec : pair.second)
          {
               for (auto &subvec : vec)
               {
                    cout << subvec << " , ";
               }
               cout << endl;
          }
     }
}

#endif // GRAMMAR_H