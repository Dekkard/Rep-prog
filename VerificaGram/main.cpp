#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>
/*
Desenvolver em C, C++ ou Java um programa que seja capaz de verificar se 
uma palavra qualquer pertence, ou não, à gramática a seguir. 
Gramática G=({S,X,Y,A,B,F},{a,b},P,S), onde: 
    P = {S→XY, X→XaA|XbB|F, Aa→aA, Ab→bA, AY→Ya, Ba→aB, Bb→bB, BY→Yb, Fa→aF, Fb→bF, FY→ε} 
    gera a linguagem {ww|w é palavra de {a,b}*}
*/
using namespace std;

regex alfa("a|b");
regex rules("S|X|Aa|Ab|AY|Ba|Bb|bB|BY|Fa|Fb|FY");
void verifyGrammarHelper(string str, string word){
    sregex_iterator currentMatch(str.begin(),str.end(),rules);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
        smatch match = *currentMatch;
        string str_tmp;
        if(match.str().compare("S")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"XY");
        }else if(match.str().compare("X")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"XaA");
            verifyGrammarHelper(str_tmp, word);
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"XbB");
            verifyGrammarHelper(str_tmp, word);
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"F");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("Aa")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"aA");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("Ab")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"bA");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("AY")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"Ya");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("Ba")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"aB");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("Bb")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"bB");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("BY")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"Yb");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("Fa")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"aF");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("Fb")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"bF");
            verifyGrammarHelper(str_tmp, word);
        }else if(match.str().compare("FY")==0){
            str_tmp = str;
            str_tmp.replace(match.length(),match.position(),"");
            verifyGrammarHelper(str_tmp, word);
        }
        
    }
}
int main(int argc, char **argv){
    string word;
    if(argv[1] == 0){
        cout << "Digite uma word: " << endl;
        cin >> word;
    }else{
        word = argv[1];
    }
    string start = "S";
    void verifyGrammarHelper(start, word){
}