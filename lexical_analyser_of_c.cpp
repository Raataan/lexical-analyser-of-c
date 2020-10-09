#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void next();
string read_file_to_string();
string code = read_file_to_string();
string::iterator iter = code.begin();

char *src = (char *)code.c_str();
char token;
int token_val,line;
vector<string> keywords = {"auto", "break", "case", "char", "const", "continue",
                           "default", "do", "double", "else", "enum", "extern",
                           "float", "for", "goto", "if", "int", "long",
                           "register", "return", "short", "signed", "sizeof", "static",
                           "struct", "switch", "typedef", "union", "unsigned", "void",
                           "volatile", "while"};
ofstream fout;
typedef struct identifier
{
    int token;
    string name;
    int value;
} ID;
enum
{
    identifier,
    keyword,
    op
};
int main()
{
    line = count(code.begin(), code.end(), '\n')+1;
    fout.open("mark.txt", ios::out | ios::trunc);
    while ((token = *iter))
    {
        if (token == '#')
        {
            while (*iter != '\n')
                iter++;
            token = *iter;
        }
        if (token == '/' && *(iter + 1) == '/')
        {
            while (*iter != '\n')
                iter++;
            token = *iter;
        }

        next();
        iter++;
    }
    cout << line;
    fout.close();
    return 0;
}
string read_file_to_string()
{
    ifstream fs("program.c", ios::binary);
    stringstream ss;
    ss << fs.rdbuf();
    fs.close();
    string str = ss.str(); //read into string
    return str;
}
void next()
{
    if (token >= '0' && token <= '9') // parse number, three kinds: dec(123) hex(0x123) oct(017)

    {
        token_val = token - '0';
        iter++;
        if (token_val > 0)
        {
            // dec, starts with [1-9]
            while (*iter >= '0' && *iter <= '9')
            {
                token_val = token_val * 10 + *iter++ - '0';
            }
        }
        else
        {
            // starts with number 0
            if (*iter == 'x' || *iter == 'X')
            {
                //hex
                token = *++iter;
                while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' && token <= 'F'))
                {
                    token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
                    token = *++iter;
                }
            }
            else
            {
                // oct
                while (*iter >= '0' && *iter <= '7')
                {
                    token_val = token_val * 8 + *iter++ - '0';
                }
            }
        }
        fout << "< Number, Line=" << line << ", Value=" << token_val << " >" << endl;
        return;
    }
    else if (token == '"') //string
    {
        string now;
        while (*++iter != '"')
        {
            if (*iter == '\n'||*iter=='\\'||*iter=='\r')//f**k \r!!!! waste a lot of time here
                continue;
            now.push_back(*iter);
        }
        fout << "< String, Line=" << line << ", Value=" << now << " >" << endl;
    }
}
