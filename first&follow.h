#ifndef FIRST_AND_FOLLOW_H
#define FIRST_AND_FOLLOW_H

#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <set>
#include "grammar.h"
#include <algorithm>
using namespace std;

bool contains(const vector<string> &array, const string &key)
{
     return find(array.begin(), array.end(), key) != array.end();
}

// set<string> calculateFirst(const string &symbol, map<string, set<string>> &memo);
map<string, vector<string>> calculateFirst(const string &symbol, map<string, map<string, vector<string>>> &memo);

// set<string> calculateFirstForRule(const vector<string> &rule, map<string, set<string>> &memo)
map<string, vector<string>> calculateFirstForRule(const vector<string> &rule, map<string, map<string, vector<string>>> &memo)
{
     // set<string> result;
     map<string, vector<string>> result;


          const string &symbol = rule[0];
          if (contains(TERMINALS, symbol))
          {
               result[symbol] = rule;
               return result;
          }
          else if (symbol == EMPTYSYMBOL)
          {
               result[EMPTYSYMBOL] = rule;
               return result;
          }
          else if (contains(NONTERMINALS, symbol))
          {
               // set<string> firstSet = calculateFirst(symbol, memo);
               map<string, vector<string>> firstSet = calculateFirst(symbol, memo);
               // result.insert(firstSet.begin(), firstSet.end());
               for(auto& fs : firstSet){
                    result[fs.first] = rule;
               }

               if (firstSet.find(EMPTYSYMBOL) == firstSet.end())
                    return result;
               else
                    result.erase(EMPTYSYMBOL);
          }
          else
          {
               result["Syntax Error"] = {};
               return result;
          }
     

     result[EMPTYSYMBOL] = {};
     return result;
}

// set<string> calculateFirst(const string &symbol, map<string, set<string>> &memo)
map<string, vector<string>> calculateFirst(const string &symbol, map<string, map<string, vector<string>>> &memo)
{
     // if (memo.find(symbol) != memo.end())
     //      return memo[symbol];

     // set<string> result;
     map<string, vector<string>> result;
     for (const auto &rule : GRAMMAR[symbol])
     {
          // set<string> ruleFirst = calculateFirstForRule(rule, memo);
          map<string, vector<string>> ruleFirst = calculateFirstForRule(rule, memo);
          result.insert(ruleFirst.begin(), ruleFirst.end());
     }

     memo[symbol] = result;
     return result;
}

// map<string, set<string>> FirstTable()
map<string, map<string, vector<string>>> FirstTable()
{
     // map<string, set<string>> firstTable;
     map<string, map<string, vector<string>>> firstTable;
     for (const auto &nt : NONTERMINALS)
     {
          firstTable[nt] = calculateFirst(nt, firstTable);
     }
     return firstTable;
}

map<string, set<string>> FollowTable()
{
     map<string, set<string>> follow;
     // map<string, set<string>> first = FirstTable();
     map<string, map<string, vector<string>>> first = FirstTable();

     for (const auto &nt : NONTERMINALS)
     {
          follow[nt] = {};
     }

     follow[STARTSYMBOL].insert("$");

     bool changed = true;
     while (changed)
     {
          changed = false;

          for (const auto &[lhs, rules] : GRAMMAR)
          {
               for (const auto &rule : rules)
               {
                    for (size_t i = 0; i < rule.size(); ++i)
                    {
                         const string &B = rule[i];
                         if (!contains(NONTERMINALS, B))
                              continue;

                         set<string> followB = follow[B];

                         if (i + 1 < rule.size())
                         {
                              const string &next = rule[i + 1];
                              set<string> firstNext;

                              if (contains(TERMINALS, next))
                              {
                                   firstNext.insert(next);
                              }
                              else
                              {
                                   set<string> vals = {};
                                   for (auto &val : first[next])
                                   {
                                        // firstNext = val.first;
                                        vals.insert(val.first);
                                   }
                                   firstNext = vals;
                              }

                              for (const auto &sym : firstNext)
                              {
                                   if (sym != EMPTYSYMBOL && follow[B].insert(sym).second)
                                        changed = true;
                              }

                              if (firstNext.find(EMPTYSYMBOL) != firstNext.end())
                              {
                                   for (const auto &sym : follow[lhs])
                                   {
                                        if (follow[B].insert(sym).second)
                                             changed = true;
                                   }
                              }
                         }
                         else
                         {
                              for (const auto &sym : follow[lhs])
                              {
                                   if (follow[B].insert(sym).second)
                                        changed = true;
                              }
                         }
                    }
               }
          }
     }

     return follow;
}

void printFollowTable()
{
     map<string, set<string>> followTable = FollowTable();
     cout << "\n=== FOLLOW Table ===\n";

     for (const auto &[nonTerminal, followSet] : followTable)
     {
          cout << nonTerminal << " -> { ";
          size_t count = 0;
          for (const auto &symbol : followSet)
          {
               cout << symbol;
               if (++count < followSet.size())
                    cout << ", ";
          }
          cout << " }\n";
     }
}

void printFirstTable()
{
     map<string, map<string, vector<string>>> first = FirstTable();
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

#endif // FIRST_AND_FOLLOW_H