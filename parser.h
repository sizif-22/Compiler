#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "parsingTable.h"
#include "grammar.h"
#include "first&follow.h"
using namespace std;
void printTokens(vector<pair<string, string>> tokens)
{
     for (auto &pair : tokens)
     {
          cout << pair.first << " | " << pair.second << endl;
     }
}

void parse(vector<pair<string, string>> tokens)
{
     map<string, map<string, vector<string>>> parsingTable = CalculateParsingTable();
     tokens.push_back({"$", "$"});
     // printTokens(tokens);
     stack<string> parseStack;
     parseStack.push("$");
     parseStack.push(STARTSYMBOL);
     int index = 0;
     string currentToken = "";

     currentToken = tokens[index].first;
     while (!parseStack.empty())
     {
          string top = parseStack.top();
          parseStack.pop();

          if (top == "ε")
               continue;

          if (top == currentToken)
          {
               cout << "Matched: " << top << endl;
               index++;
               if (index == tokens.size())
               {
                    break;
               }
               currentToken = "";
               currentToken = tokens[index].first;
          }
          else if (parsingTable.count(top) && parsingTable[top].count(currentToken))
          {
               vector<string> production = parsingTable[top][currentToken];
               cout << top << " -> ";
               for (const string &sym : production)
                    cout << sym << " ";
               cout << endl;
               for (int i = production.size() - 1; i >= 0; i--)
               {
                    parseStack.push(production[i]);
               }
          }
          else
          {
               cout << "Syntax error at token: " << currentToken << endl;
               return;
          }
     }

     if (currentToken == "$")
     {
          cout << "Parsing successful!" << endl;
     }
     else
     {
          cout << "Unexpected tokens remaining." << endl;
     }
}