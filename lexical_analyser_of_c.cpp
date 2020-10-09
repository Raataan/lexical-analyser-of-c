#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void next();
void print_overview();
string read_file_to_string();
string code = read_file_to_string();
string::iterator iter = code.begin();
vector<string>::iterator keyword_finder;
char *src = (char *)code.c_str();
char token;
int token_val, line, delimiter_amount = 0, op_amount = 0, keyword_amount = 0, string_amount = 0, number_amount = 0, comment_amount = 0;
vector<string> keywords = {"auto", "break", "case", "char", "const", "continue",
                           "default", "do", "double", "else", "enum", "extern",
                           "float", "for", "goto", "if", "int", "long",
                           "register", "return", "short", "signed", "sizeof", "static",
                           "struct", "switch", "typedef", "union", "unsigned", "void",
                           "volatile", "while"};
vector<string> one_op_and_delimiter = {
    "+", "-", "*", "/",
    "<", ">", "=",
    "\?", ":", "!", "&",
    "|", "%", "~", "^",
    ";", "(", ")", ",", "\"", "\'", "[", "]",
    "{", "}", "\\", "."};
vector<string> two_op_and_delimiter = {
    "<=", ">=", "==", "+=", "!="
                            "&&",
    "||", "<<", ">>",
    "&=", "^=", "|="};
vector<string> three_op_and_delimiter = {
    "<<=", ">>="};
vector<string> identifier;
ofstream fout;
int main()
{

    fout.open("mark.txt", ios::out | ios::trunc);
    while ((token = *iter))
    {
        if (token == '#') //jump marco
        {
            while (*iter != '\n')
                iter++;
            token = *iter;
        }
        else if (token == '/' && *(iter + 1) == '/')
        {
            while (*iter != '\n')
                iter++;
            token = *iter;
        }
        else if (token == '/' && *(iter + 1) == '*')
        {
            for (;; iter++)
            {
                if (*iter == '*' && *(iter + 1) == '/')
                {
                    iter += 2;
                    token = *iter;
                    break;
                }
            }
        }
        next();
        iter++;
    }
    print_overview();
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
        iter--; //set back iterator
        line = count(code.begin(), iter, '\n') + 1;
        fout << "< Number, Line=" << line << ", Value=" << token_val << " >" << endl;
        number_amount++;
        return;
    }
    else if (token == '"') //string
    {
        line = count(code.begin(), iter, '\n') + 1;
        string now;
        while (*++iter != '"')
        {
            if (*iter == '\n' || *iter == '\\' || *iter == '\r') //f**k \r!!!! waste a lot of time here
                continue;
            now.push_back(*iter);
        }
        fout << "< String, Line=" << line << ", Value=\"" << now << "\" >" << endl;
        string_amount++;
        return;
    }
    string token_s(1, token);

    auto temp_iter = find(one_op_and_delimiter.begin(), one_op_and_delimiter.end(), token_s);
    if (temp_iter != one_op_and_delimiter.end())
    {
        int index = distance(one_op_and_delimiter.begin(), temp_iter);
        token_s += *(iter + 1);
        if (find(two_op_and_delimiter.begin(), two_op_and_delimiter.end(), token_s) != two_op_and_delimiter.end())
        {
            token_s += *(iter + 2);
            if (find(three_op_and_delimiter.begin(), three_op_and_delimiter.end(), token_s) != three_op_and_delimiter.end()) //three-digit
            {
                line = count(code.begin(), iter, '\n') + 1;
                fout << "< Op, Line=" << line << ", Value=\"" << token_s << "\" >" << endl; //three-digit are all Op.
                op_amount++;
                iter += 2;
                return;
            }
            else //two-digit
            {
                line = count(code.begin(), iter, '\n') + 1;
                fout << "< Op, Line=" << line << ", Value=\"" << token_s << "\" >" << endl; //two-digit are all Op.
                op_amount++;
                iter++;
                return;
            }
        }
        else //one-digit
        {
            line = count(code.begin(), iter, '\n') + 1;
            if (index <= 14)
            {
                fout << "< Op, Line=" << line << ", Value=\"" << token << "\" >" << endl;
                op_amount++;
            }
            else
            {
                fout << "< Delimiter, Line=" << line << ", Value=\"" << token << "\" >" << endl;
                delimiter_amount++;
            }
        }
        return;
    }
    else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
    {
        token = *++iter;
        while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
        {
            token_s.push_back(token);
            token = *++iter;
        }
        iter--; //set back iterator
        keyword_finder = find(keywords.begin(), keywords.end(), token_s);
        if (keyword_finder != keywords.end())
        {
            line = count(code.begin(), iter, '\n') + 1;
            fout << "< Keyword, Line=" << line << ", Value=\"" << token_s << "\" >" << endl;
            keyword_amount++;
            return;
        }
        else
        {
            keyword_finder = find(identifier.begin(), identifier.end(), token_s);
            if (keyword_finder == identifier.end())
                identifier.push_back(token_s);
            line = count(code.begin(), iter, '\n') + 1;
            fout << "< Identifier, Line=" << line << ", Value=\"" << token_s << "\" >" << endl;
        }
    }
}
void print_overview()
{
    line = count(code.begin(), code.end(), '\n') + 1;
    fout << endl;
    fout << "Total Lines: " << line << endl;
    fout << "Delimiter Amount: " << delimiter_amount << endl;
    fout << "Operator Amount: " << op_amount << endl;
    fout << "Keywords Amount: " << keyword_amount << endl;
    fout << "Identifier Amount: " << identifier.size() << endl;
    fout << "String Amount: " << string_amount << endl;
    fout << "Number Amount: " << number_amount << endl;
    fout << "Character Amount: " << code.length();
}