#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <cctype>
using namespace std;
bool isNumber(string str)
{
     for (int i = 0; i < str.length(); i++)
     {
          if (isdigit(str[i]))
          {
               continue;
          }
          else
          {
               return false;
          }
     }
     return true;
}
vector<pair<string, string>> Scanner()
{
     vector<pair<string, string>> tokens = {};
     set<string> colors = {"red", "green", "blue"};
     string filename = "./input.txt";
     ifstream inFile(filename);
     map<char, string> delimiter = {{'(', "("}, {')', ")"}, {'-', "-"}, {' ', "space"}};
     string line;
     getline(inFile, line);
     string str = "";
     for (int i = 0; i < line.length(); i++)
     {
          if (delimiter.count(line[i]) || line[i] == ' ')
          {
               if (str != "")
               {
                    if ((isNumber(str)))
                    {
                         tokens.push_back({"int", str});
                    }
                    else if (colors.count(str))
                    {
                         tokens.push_back({"color", str});
                    }
                    else
                    {
                         cout << "Error !" << endl;
                         break;
                    }
               }
               tokens.push_back({delimiter[line[i]], delimiter[line[i]]});
               str = "";
          }
          else
          {
               str += line[i];
          }
     }
     if (str != "")
     {
          if ((isNumber(str)))
          {
               tokens.push_back({"int", str});
          }
          else if (colors.count(str))
          {
               tokens.push_back({"color", str});
          }
          else
          {
               cout << "Error !" << endl;
          }
     }
     return tokens;
}
