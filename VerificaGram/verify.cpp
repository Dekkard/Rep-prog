#include <iostream>
#include <string>
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
regex rules("S|X|Aa|Ab|AY|Ba|Bb|BY|Fa|Fb|FY");
int end_f = 0;
int counter_1 = 0;
int lst_lvl;
string path = ""; //FbBaAY
void verifyGrammarHelper(string str, string word, int lvl){
    sregex_iterator currentMatch(str.begin(),str.end(),rules);
    sregex_iterator lastMatch;
    //cout<<counter_1<<">"<<str<<" lvl="<<lvl<<endl;
    while(currentMatch != lastMatch){
        //cout<<" ->"<<str<<endl;
        smatch match = *currentMatch;
        string str_tmp = str;
        if(str.length() < word.length()+2){
            if(match.str().compare("S")==0){
                str_tmp.replace(match.position(),1,"XY");
                verifyGrammarHelper(str_tmp, word, lvl+1);
            }else if(match.str().compare("X")==0){
                str_tmp.replace(match.position(),1,"XaA");
                verifyGrammarHelper(str_tmp, word, lvl+1);
                if(end_f==1){
					if(lvl < lst_lvl){
						path.insert(0,str+"->");
						lst_lvl = lvl;
					}
                    return;
                }
                str_tmp = str;
                str_tmp.replace(match.position(),1,"XbB");
                verifyGrammarHelper(str_tmp, word,lvl+1);
            }
        }
        if(match.str().compare("X")==0){
            str_tmp.replace(match.position(),1,"F");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("Aa")==0){
            str_tmp.replace(match.position(),2,"aA");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("Ab")==0){
            str_tmp.replace(match.position(),2,"bA");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("AY")==0){
            str_tmp.replace(match.position(),2,"Ya");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("Ba")==0){
            str_tmp.replace(match.position(),2,"aB");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("Bb")==0){
            str_tmp.replace(match.position(),2,"bB");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("BY")==0){
            str_tmp.replace(match.position(),2,"Yb");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("Fa")==0){
            str_tmp.replace(match.position(),2,"aF");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("Fb")==0){
            str_tmp.replace(match.position(),2,"bF");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }else if(match.str().compare("FY")==0){
            str_tmp.replace(match.position(),2,"");
            verifyGrammarHelper(str_tmp, word, lvl+1);
        }
        str = str_tmp;
        //cout << str << " : " << counter_1 << endl;
        if(end_f==1){
			if(lvl < lst_lvl){
				path.insert(0,str+"->");
				lst_lvl = lvl;
			}
            return;
        }//else{cout<<"----------lvl="<<lvl<<endl;}
        currentMatch++;
    }
    if(str.compare(word)==0 && end_f != 1){
        cout << "String encontrada: "<< str << endl;
        end_f = 1;
		path += str;
		lst_lvl = lvl-1;
    }else{
		//cout<<"====="<<endl;
	}
}
int main(int argc, char **argv){
    string word;
    if(argv[1] == 0){
        cout << "Digite uma palavra: " << endl;
        cin >> word;
    }else{
        word = argv[1];
    }
    string start = "S";
    verifyGrammarHelper(start,word,0);
    if(end_f==0){
        cout << "Palavra não encontrada" << endl;
    }
    else{
        cout << path << endl;
    }
}
