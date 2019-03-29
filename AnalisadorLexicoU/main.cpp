#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef string token;

token lex_keywords[] = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End", "End." };
token lex_symbols[] = {",", ";",":", "+", "-", "*", "/", "=", ":=","(",")"};

ifstream file;

token keywords[500];
int kw_it = 0;
token symbols[500];
int sb_it = 0;
token identifiers[500];
int id_it = 0;

typedef struct integer_Reg
{
    string name;
    int value;
} integer_Reg;
typedef struct float_Reg
{
    string name;
    float value;
} float_Reg;

integer_Reg * iR;
float_Reg * fR;

void read_file()
{
    ifstream file;
    string data;
    file.open ("prog.txt");
    if (!file.is_open())
    {
        cout << "File not found." << endl;
        return;
    }
    cout << "Reading from the file" << endl;
    //while(file != NULL)
    while(true)
    {
        file >> data;
        cout << data << endl;
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
    }
}

void string_lexing(token word)
{
    int i = 0;
    while(lex_keywords[i].compare(""))
    {
        if(word.compare(lex_keywords[i])==0)
        {
            cout << word <<" -- keyword found" << endl;
            keywords[kw_it] = word;
            kw_it++;
            return;
        }
        if(word.compare(lex_symbols[i])==0)
        {
            cout << word <<" -- Symbol found" << endl;
            symbols[sb_it] = word;
            sb_it++;
            return;
        }
        i++;
    }
    if(word.length() != 0)
    {
        for(int j=0; j<id_it; j++)
        {
            if(identifiers[j].compare(word)==0)
            {
                return;
            }
        }
        cout << word <<" -- Identifiers found" << endl;
        identifiers[id_it] = word;
        id_it++;
    }
}

string string_id(token word)
{
    int i = 0;
    while(lex_keywords[i].compare(""))
    {
        if(word.compare(lex_keywords[i])==0)
        {
            cout << word <<" -- keyword found" << endl;
            return "keyword";
        }
        if(word.compare(lex_symbols[i])==0)
        {
            cout << word <<" -- Symbol found" << endl;
            return "symbol";
        }
        i++;
    }
    if(word.length() != 0)
    {
        cout << word <<" -- Identifiers found" << endl;
        return "identifier";
    }
    return NULL;
}

string token_analyser(token word)
{
    int i=0;
    file >> word;
    if(word.compare("Program"))
    {
        file >> word;
    }
    if(word.compare("Var"))
    {
        string* reg;
        file >> word;
        while(!word.compare(";"))
        {
            file >> word;
            if(string_id(word).compare("indentifier"))
            {
                reg = new string[1];
                reg[i] = word;
                i++;
                file >> word;
                if(word.compare(","))
                {
                    continue;
                }
                else if(word.compare(":"))
                {
                break;
                }
                else
                {
                    return "erro";
                }
            }
            else
            {
                return "erro";
            }
        }
        file >> word;
        if(word.compare("int"))
        {
            for(int j=0;j<i;j++)
            {
                iR = new integer_Reg[1];
                reg[j] = iR[j].name;
            }
            delete reg;
        }
        if(word.compare("real"))
        {
            for(int j=0;j<i;j++)
            {
                fR = new float_Reg[1];
                reg[j] = fR[j].name;
            }
            delete reg;
        }
    }
    if(word.compare("Const"))
    {
    }
    if(word.compare("Begin"))
    {
    }
    if(word.compare("Read"))
    {
    }
    if(word.compare("Write"))
    {
    }
    if(word.compare("End."))
    {
    }
}

int main(int argc, char **argv)
{
    file.open (argv[1]);
    if (!file.is_open())
    {
        cout << "File not found." << endl;
        return 0;
    }
    string line;
    cout << "Begin Analysis..." << endl;

    string msg;
    token word;
    //while(file != NULL)
    while(true)
    {
        if(file.peek() == std::ifstream::traits_type::eof())
        {
            break;
        }
        msg = token_analyser(word);
    }
    cout << "...end Analysis." << endl;
    file.close();
    return 0;
}
