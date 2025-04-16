#if !defined(Parser)
#define Parser

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

vector<string> readTokens()
{
     string fileName = "./input.txt";
     vector<string> tokens;
     ifstream inFile(fileName);
     string line = "";
     getline(inFile, line);
     string word = "";
     line = trim(line);
     for (int i = 0; i < line.length(); i++)
     {
          if (line[i] == ' ')
          {
               tokens.push_back(word);
               word = "";
               continue;
          }
          word += line[i];
     }
     tokens.push_back(word);
     tokens.push_back("$");
     return tokens;
}
vector<string> tokens = readTokens();
void printTokens()
{
     cout << endl;
     for (auto &token : tokens)
     {
          cout << token << endl;
     }
}
map<string, map<string, vector<string>>> parsingTable = CalculateParsingTable();
void parse()
{
     stack<string> parseStack;
     parseStack.push("$");
     parseStack.push(STARTSYMBOL);

     int index = 0;
     string currentToken = tokens[index];

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
               currentToken = tokens[index];
          }
          else if (parsingTable.count(top) && parsingTable[top].count(currentToken))
          {
               vector<string> production = parsingTable[top][currentToken];
               cout << top << " -> ";
               for (const string &sym : production)
                    cout << sym << " ";
               cout << endl;

               // Push in reverse
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

#endif // Parser