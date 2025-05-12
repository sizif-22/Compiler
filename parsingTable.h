#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <set>
#include "grammar.h"
#include "first&follow.h"
#include <algorithm>
using namespace std;

map<string, map<string, vector<string>>> CalculateParsingTable()

{
     map<string, map<string, vector<string>>> parsingTable = {};
     map<string, map<string, vector<string>>> firstTable = FirstTable();
     map<string, set<string>> followTable = FollowTable(firstTable);
     for (auto &pair : firstTable)
     {
          for (auto &subMap : pair.second)
          {
               if (subMap.first == EMPTYSYMBOL)
               {
                    for (auto &fr : followTable[pair.first])
                    {
                         parsingTable[pair.first][fr] = firstTable[pair.first][subMap.first];
                    }
               }
               else
               {
                    parsingTable[pair.first][subMap.first] = firstTable[pair.first][subMap.first];
               }
          }
     }

     return parsingTable;
}
void printParsingTable()
{
     map<string, map<string, vector<string>>> first = CalculateParsingTable();
     cout << "\n=== FIRST Table ===\n";

     for (const auto &[nonTerminal, productions] : first)
     {
          cout << nonTerminal << " -> {\n";
          for (const auto &[terminal, rules] : productions)
          {
               cout << "   " << terminal << " : [";
               for (size_t i = 0; i < rules.size(); ++i)
               {
                    cout << rules[i];
                    if (i < rules.size() - 1)
                         cout << ", ";
               }
               cout << "]\n";
          }
          cout << "}\n";
     }
}