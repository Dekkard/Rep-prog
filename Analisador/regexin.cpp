#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>
using namespace std;

typedef string token;

regex reg_real("[0-9]+,[0-9]+([Ee][-+]?[0-9]+)|[0-9]+,[0-9]+|[0-9]+");
regex regex_keyword("(Program|Var|int|real|Const|Begin|Read|Write|(End.)|End)");
regex regex_symbols ("(?![0-9]),(?![0-9])|:|;|:=|=|[(]|[)]|[+]|[-]|[*]|[/]|[\"]");
//regex regex_math_sym("\+|-|\*|\/");

void printMatch(string var, regex reg){
    smatch match;
    while(regex_search(var,match,reg))
        {
            cout << "Match: " << match.str(1) << " -- Position: " << match.position() << endl;
            var = match.suffix().str();
        }
}
void printMatch2(string var, regex reg){
    sregex_iterator currentMatch(var.begin(),var.end(),reg);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch)
    {
        smatch match = *currentMatch;
        cout << "Match: " << match.str() << " -- Position: " << match.position() << endl;
        currentMatch++;
    }
}

int main()
{
    string var;
    /*while(true)
    {
        cout << "Digite um numero real" << endl;
        cin >> var;
        if(regex_match(var,reg_real))
        {
            cout << "Valido" << endl;
        }
        else if(var.compare("sair")==0)
        {
            break;
        }
        else
        {
            cout << "Invalido" << endl;
        }
    }*/
    while(true)
    {
        cout << "Digite uma frase: " << endl;
        getline(cin,var);
        if(var.compare("sair")==0 || var.compare("")==0 )
        {
            break;
        }
        printMatch2(var,regex_keyword);
        printMatch2(var,reg_real);
        //printMatch2(var,regex_math_sym);
        printMatch2(var,regex_symbols);
    }
}
