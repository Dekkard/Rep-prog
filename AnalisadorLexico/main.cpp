#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef string token;
typedef token lexicon[100];

lexicon keywords = {"Program", "Var", "int", "real", "Const", "Begin", "Read", "Write", "End" };
lexicon symbols = {"+", "-", "*", "/", ":=" };

int main()
{
    ifstream file;
    file.open ("prog.txt");
    if (!file.is_open())
    {
        cout << "File not found.";
        return 0;
    }
    string line;
    cout << "Begin Analysis..." << endl;

    lexicon identifiers;
    char pos;
    token word;

    int* intReg;
    float* realReg;

    while(word.compare("end") == 1)
    {
        while(getline(file,line));
        {

        }
    }
    cout << "...end Analysis." << endl;
    file.close();
    return 0;
}
/*
void token_analyser(string word)
{
}
*/
