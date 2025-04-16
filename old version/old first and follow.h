#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "grammar.h"
#include <algorithm>
using namespace std;
bool contains(vector<string> array, string key)
{
     return find(array.begin(), array.end(), key) != array.end();
}

vector<string> calcuateFirst(vector<vector<string>> right)
{
     vector<string> result = {};
     for (auto &vec : right)
     {
          // if (find(NONTERMINALS.begin(), NONTERMINALS.end(), vec[0]) != NONTERMINALS.end())
          if (contains(NONTERMINALS, vec[0]))
          {
               vector<string> resVector = calcuateFirst(GRAMMAR[vec[0]]);
               for (auto &res : resVector)
               {
                    result.push_back(res);
               }
          }
          // else if (find(TERMINALS.begin(), TERMINALS.end(), vec[0]) != TERMINALS.end() || vec[0] == EMPTYSYMBOL)
          else if (contains(TERMINALS, vec[0]) || vec[0] == EMPTYSYMBOL)
          {
               result.push_back(vec[0]);
          }
          else
          {
               return {" Syntax Error "};
          }
     }
     return result;
}
map<string, vector<string>> FirstTable()
{
     map<string, vector<string>> firstTable = {};
     for (auto &pair : GRAMMAR)
     {
          firstTable[pair.first] = calcuateFirst(pair.second);
     }
     return firstTable;
}

const map<string, vector<string>> FIRST = FirstTable();

vector<string> calculateFollow(string key)
{
     vector<string> follow;
     if (key == STARTSYMBOL)
          follow.push_back("$");
     for (auto &pair : GRAMMAR)
     {
          for (auto &vec : pair.second)
          {
               for (int i = 0; i < vec.size(); i++)
               {
                    if (key == vec[i])
                    {
                         if (i < vec.size() - 1)
                         {
                              if (contains(TERMINALS, vec[i + 1]))
                              {
                                   follow.push_back(vec[i + 1]);
                              }
                              else if (contains(NONTERMINALS, vec[i + 1]))
                              {
                                   vector<string> res = calcuateFirst(GRAMMAR[vec[i + 1]]);
                                   for (auto &val : res)
                                   {
                                        if (contains(TERMINALS, val))
                                        {
                                             follow.push_back(val);
                                        }
                                        else if (val == EMPTYSYMBOL)
                                        {
                                             vector<string> valFollow = calculateFollow(vec[i + 1]);
                                             for (auto &lol : valFollow)
                                             {
                                                  follow.push_back(lol);
                                             }
                                        }
                                   }
                              }
                         }
                         else
                         {
                              if (key == pair.first)
                                   continue;
                              else
                              {
                                   vector<string> res = calculateFollow(pair.first);
                                   for (auto &val : res)
                                   {
                                        follow.push_back(val);
                                   }
                              }
                         }
                    }
                    else
                         continue;
               }
          }
     }
     return follow;
}
map<string, vector<string>> FollowTable()
{
     map<string, vector<string>> followTable = {};
     for (auto &pair : GRAMMAR)
     {
          followTable[pair.first] = calculateFollow(pair.first);
     }
     return followTable;
}