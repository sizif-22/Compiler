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

// vector<string> readTokens()
// {
//      string fileName = "./input.txt";
//      vector<string> tokens;
//      ifstream inFile(fileName);
//      string line = "";
//      getline(inFile, line);
//      string word = "";
//      line = trim(line);
//      for (int i = 0; i < line.length(); i++)
//      {
//           if (line[i] == ' ')
//           {
//                tokens.push_back(word);
//                word = "";
//                continue;
//           }
//           word += line[i];
//      }
//      tokens.push_back(word);
//      tokens.push_back("$");
//      return tokens;
// }

vector<map<string, string>> getTokens()
{
     // get tokens...
     vector<map<string, string>> tokens;
     string filename = "./tokens.txt";
     ifstream inFile(filename);
     string line;
     while (getline(inFile, line))
     {
          map<string, string> token = {};
          if (line == "")
          {
               continue;
          }
          else
          {
               size_t pos = line.find('|');
               string left = line.substr(0, pos);
               string right = line.substr(pos + 1);
               token[left] = right;
               tokens.push_back(token);
          }
     }
     map<string, string> d;
     d["$"] = "$";
     tokens.push_back(d);
     for (auto &vec : tokens)
     {
          for (auto &pair : vec)
          {
               cout << pair.first << " | " << pair.second << endl;
          }
     }

     return tokens;
}

vector<map<string, string>> tokens = getTokens();
// vector<string> tokens = readTokens();
// void printTokens()
// {
//      cout << endl;
//      for (auto &token : tokens)
//      {
//           cout << token << endl;
//      }
// }
map<string, map<string, vector<string>>> parsingTable = CalculateParsingTable();
void parse()
{
     stack<string> parseStack;
     parseStack.push("$");
     parseStack.push(STARTSYMBOL);

     int index = 0;
     // string currentToken = tokens[index];
     string currentToken = "";
     for (auto &pair : tokens[index])
     {
          currentToken = pair.first;
     }

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
               for (auto &pair : tokens[index])
               {
                    currentToken = pair.first;
               }
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