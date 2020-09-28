#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;
bool is_keyword(char buffer[])
{
    char keywords[32][10] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long",
        "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while"};
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(keywords[i], buffer) == 0)
            return true;
    }
    return false;
}
bool is_operator(char buffer[])
{
    char keywords[17][3] = {"+", "-", "*", "/", "<<", ">>", "%",">","<","=","==","+=","-=","/=","*=","++","--"};
    for (int i = 0; i < 15; i++)
    {
        if (strcmp(keywords[i], buffer) == 0)
            return true;
    }
    return false;
}
int main()
{
    ifstream fin("program.txt");
    if(!fin.is_open())
        cout << "Error: Can\'t open the file. ";
    return 0;
}
