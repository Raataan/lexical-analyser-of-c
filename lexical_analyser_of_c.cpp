#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
void next();
string read_file_to_string();
string code = read_file_to_string();
string::iterator iter = code.begin();
char *src = (char *)code.c_str();
char token;
int token_val, line = 1;
ofstream fout;
int main()
{
    fout.open("mark.txt", ios::out | ios::trunc);
    while ((token = *iter))
    {
        if (token == '#')
        {
            while (*iter != '\n')
                iter++;
            token = *iter;
        }
        if (token == '\n')
            line++;
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
    if (token >= '0' && token <= '9')
    {
        // parse number, three kinds: dec(123) hex(0x123) oct(017)
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
        fout << "< Number, Line=" << line << ", Value=" << token_val<<" >"<<endl;
        return;
    }
    else if (token == '"') //string
    {
        string now;
        while (*++iter != '"')
        {
            if (*iter == '\n')
                line++;
            now.push_back(*iter);
        }
        fout << "< String, Line=" << line << ", Value=" << now<<" >"<<endl;
    }
}
