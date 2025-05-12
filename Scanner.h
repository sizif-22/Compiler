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
     ofstream print("./tokens.txt");
     set<char> delimiter = {'(', ')', '-'};
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
               if (delimiter.count(line[i]))
               {

                    string delim = "";
                    delim += line[i];
                    tokens.push_back({delim, delim});
               }
               else
               {
                    tokens.push_back({"space", "space"});
               }
               str = "";
          }
          else
          {
               str += line[i];
          }
     }
     if (str != "")
          if (isNumber(str))
          {
               print << "int|" << str << endl;
          }
          else if (colors.count(str))
          {
               print << "color|" << str << endl;
          }
          else
          {
               print << str << "|" << str << endl;
          }
     return tokens;
}
