#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef string token;
typedef token lexicon[100];

lexicon lex_keywords = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End" };
lexicon lex_symbols = {"+", "-", "*", "/", ":=" };

int main()
{
    int i = 0;
    ifstream file;
    file.open ("prog.txt");
    if (!file.is_open())
    {
        cout << "File not found.";
        return 0;
    }
    string line;
    cout << "Begin Analysis..." << endl;

    lexicon keywords;
    int kw_it = 0;
    lexicon symbols;
    int sb_it = 0;
    lexicon identifiers;
    int id_it = 0;

    char pos;
    token word;

    int* intReg;
    float* realReg;

    while(word.compare("end") == 0)
    {
        while(getline(file,line));
        {
            if(line[i] != ' ')
            {
                line[i] = pos;
                word.insert(word.end(),pos);
            }
            else
            {
                i++;
                for(int j=0; j<word.length(); j++)
                {
                    if(word.compare(lex_keywords[j])==1)
                    {
                        cout << "keyword found" << endl;
                        keywords[kw_it] = word;
                        kw_it++;
                        word.clear();
                        break;
                    }
                    if(word.compare(lex_symbols[j])==1)
                    {
                        cout << "Symbol found" << endl;
                        symbols[sb_it] = word;
                        sb_it++;
                        word.clear();
                        break;
                    }
                }
                if(word.length() != 0)
                {
                    cout << "Identifiers found" << endl;
                    identifiers[id_it] = word;
                    id_it++;
                    word.clear();
                    break;
                }
            }
            i++;
        }
    }
    cout << "...end Analysis." << endl;
    file.close();
    return 0;
}



