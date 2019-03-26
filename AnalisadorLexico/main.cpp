#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef string token;

token lex_keywords[] = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End.", "End" };
token lex_symbols[] = {",", ";", "+", "-", "*", "/", "=", ":=" };
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
    while(file != NULL)
    {
        file >> data;
        cout << data << endl;
    }
}
int main(int argc, char **argv)
{
    ifstream file;

    file.open (argv[1]);
    if (!file.is_open())
    {
        cout << "File not found." << endl;
        return 0;
    }
    string line;
    cout << "Begin Analysis..." << endl;

    token keywords[500];
    int kw_it = 0;
    token symbols[500];
    int sb_it = 0;
    token identifiers[500];
    int id_it = 0;

    char pos;
    token word;
    int* intReg;
    float* realReg;

    while(file != NULL)
    {
        file >> word;
        int i = 0;
        while(lex_keywords[i].compare(""))
        {
            if(word.compare(lex_keywords[i])==0)
            {
                cout << word <<": keyword found" << endl;
                keywords[kw_it] = word;
                kw_it++;
                word.clear();
                break;
            }
            if(word.compare(lex_symbols[i])==0)
            {
                cout << word <<": Symbol found" << endl;
                symbols[sb_it] = word;
                sb_it++;
                word.clear();
                break;
            }
            i++;
        }
        if(word.length() != 0)
        {
            cout << word <<": Identifiers found" << endl;
            identifiers[id_it] = word;
            id_it++;
        }
    }
    cout << "...end Analysis." << endl;
    file.close();
    return 0;
}



