#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

regex regex_number("\\b[0-9]*,?[0-9]*(([Ee][-+])?[0-9]+)\\b");
//regex regex_keyword("(Program|Var|int|real|Const|Begin|Read|Write|(End.)|End)");
regex regex_keyword("\\b(if|else|while|return|float|char|void|prnt|int|and|or|not|proc|var|main)\\b");
regex regex_symbols ("(?![0-9]),(?![0-9])|!=|==|>=|<=|<|>|:=|:|;|=|[(]|[)]|[+]|[-]|<<|>>|\\*\\*|[*]|[/]|[\"]|[{]|[}]");
regex regex_id("[a-z]([a-zA-z0-9])*");
regex regex_string("['][a-zA-Z0-9]+[']");
//regex regex_math_sym("\+|-|\*|\/");
typedef struct token
{
	string value;
	string type;
	int len;
} token;
void printMatch_helper(token* lines, string line, regex reg, string type){
    sregex_iterator currentMatch(line.begin(),line.end(),reg);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch)
    {
        smatch match = *currentMatch;
		if(type.compare("id")==0){
			if(regex_match(match.str(),regex_keyword)){
				currentMatch++;
				continue;
			}
		}
		token aux;
		aux.value = match.str();
		aux.type = type;
		aux.len = match.length();
		lines[match.position()] = aux;
        //cout <<"-"<< match.str() <<"-(pos:"<< match.position() << "," << type << ") ";
		currentMatch++;
    }
}
token* printMatch(string line)
{
	token *lines = new token[sizeof(token)*line.length()];
	printMatch_helper(lines,line,regex_keyword,"key");
	printMatch_helper(lines,line,regex_number,"num");
	printMatch_helper(lines,line,regex_symbols,"sym");
	printMatch_helper(lines,line,regex_id,"id");
	printMatch_helper(lines,line,regex_string,"wrd");
	return lines;
}
int main(int argc, char **argv)
{
	ifstream file;
	file.open (argv[1]);
    if (!file.is_open())
    {
        cout << "Arquivo não encontrado." << endl;
        return 0;
    }
    string line;
	int rows = 0;
	while(getline(file, line))
    {
		rows++;
	}
	cout << rows << endl;
	file.clear();
	file.seekg(0,ios::beg);
    cout << "Começo da Analise...\n" << endl;
	token *Program[rows];
	int li = 0;
    while(getline(file, line))
    {
		token *lines = new token;
		token *temp, *aux;
		//cout << "Linha " << li+1 << ": ";
        temp = printMatch(line);
		//cout << "\n" << endl;
		for(int i=0,j=0;i<line.length();i++){
			if(temp[i].value.compare("")!=0){
				aux = new token[sizeof(token)*(j+1)];
				aux[j] = temp[i];
				int k=0;
				while(!lines[k].value.empty()){
                    aux[k] = lines[k];
                    k++;
                }
				lines = aux;
				//cout << " " << lines[j].value << " ";
				j++;
			}
		}
		Program[li] = lines;
		//cout << "" << endl;
		li++;
    }
	cout << "...fim da analise." << endl;
    file.close();
	for(int i=0;i<rows;i++){
		int j=0;
		cout << "Linha "<<i+1<<": ";
		while(!Program[i][j].value.empty()){
			cout << Program[i][j++].value << " ";
		}
		cout << "" << endl;
	}
	return 0;
}
