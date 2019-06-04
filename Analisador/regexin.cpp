#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>

#include "libAux.hpp"

using namespace std;

regex regex_number("\\b[0-9]*,?[0-9]*(([Ee][-+])?[0-9]+)\\b");
regex regex_keyword("\\b(if|else|while|return|float|char|void|prnt|int|and|or|not|proc|var|main)\\b");
regex regex_symbols ("(?![0-9]),(?![0-9])|\\*\\*|<<|>>|!=|==|>=|<=|<|>|:=|:|;|=|[(]|[)]|[\"]|[{]|[}]");
regex regex_math("[+]|[-]|[*]|[/]");
regex regex_id("[a-z]([a-zA-z0-9])*");
regex regex_string("['][a-zA-Z0-9]+[']");

typedef struct posLine{
	int pos;
	int line;
} posLine;
typedef struct indBlock{
	posLine start;
	posLine end;
	char status;
} indBlock;
typedef struct indLine{
	int pos;
	char status;
} indLine;

indLine lineComment;
indBlock blockComment;

void reset_bc(){
	blockComment.start.pos = -1;
	blockComment.start.line = -1;
	blockComment.end.pos = -1;
	blockComment.end.line = -1;
	blockComment.status = 'N';
	lineComment.pos = -1;
	lineComment.status = 'N';
}
void printMatch_helper(token* lines, string line, regex reg, string type, int li){
    sregex_iterator currentMatch(line.begin(),line.end(),reg);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
        smatch match = *currentMatch;
		if(type.compare("id")==0){
			if(regex_match(match.str(),regex_keyword)){
				currentMatch++;
				continue;
			}
		}
		if(type.compare("sym")==0){
			if(match.str().compare(">>")==0){
				blockComment.start.pos = match.position();
				blockComment.start.line = li;
				blockComment.status = 'S';
				currentMatch++;
				continue;
			}
			if(match.str().compare("<<")==0){
				blockComment.end.pos = match.position();
				blockComment.end.line = li;
				currentMatch++;
				continue;
			}
			if(match.str().compare("**")==0){
				lineComment.pos = match.position();
				lineComment.status = 'S';
				currentMatch++;
				continue;
			}
		}
		if(lineComment.status == 'S'){
			if(match.position() > lineComment.pos){
				currentMatch++;
				continue;
			}
		}
		if(match.position() > blockComment.start.pos && blockComment.status == 'S'){
			if(blockComment.end.line < blockComment.start.line){
				currentMatch++;
				continue;
			} else {
				if(match.position() < blockComment.end.pos){
					currentMatch++;
					continue;
		}}}
		token aux;
		aux.value = match.str();
		aux.type = type;
		aux.len = match.length();
		aux.pos = match.position();
		if(match.str().compare(":=")==0){ aux.type+="_att";}
		lines[match.position()] = aux;
        //cout <<"-"<< match.str() <<"-(pos:"<< match.position() << "," << type << ") ";
		currentMatch++;
    }
}
token* printMatch(string line,int li){
	token *lines = new token[sizeof(token)*line.length()];
	printMatch_helper(lines,line,regex_symbols,"sym", li);
	printMatch_helper(lines,line,regex_math,"math", li);
	printMatch_helper(lines,line,regex_keyword,"key", li);
	printMatch_helper(lines,line,regex_number,"num", li);
	printMatch_helper(lines,line,regex_id,"id", li);
	printMatch_helper(lines,line,regex_string,"wrd", li);
	if(blockComment.status == 'S'){
		lineComment.pos = -1;
		lineComment.status = 'N';
	}
	if(blockComment.end.pos > blockComment.start.pos && blockComment.end.line >= blockComment.start.line){
		reset_bc();
	}
	return lines;
}
token** Lex(string filename, int &rows){
	ifstream file;
	file.open (filename);
    if (!file.is_open()){
        return NULL;
    }
	string line;
	/*int*/ rows = 0;
	while(getline(file, line)){
		rows++;
	}
	//cout << rows << endl;
	file.clear();
	file.seekg(0,ios::beg);
    //cout << "Começo da Analise... ";
	//token *Program[rows];
	token **Program = new token*[sizeof(token)*rows];
	int li = 0;
    while(getline(file, line)){
		token *lines = new token;
		token *temp, *aux;
		//cout << "Linha " << li+1 << ": ";
        temp = printMatch(line,li+1);
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
	//cout << "...fim da analise." << endl;
    file.close();
	return Program;
}
void printProg(token** Program, int rows){
	for(int i=0;i<rows;i++){
		int j=0;
		cout << "Linha "<<i+1<<":\t";
		while(!Program[i][j].value.empty()){
			cout << Program[i][j++].value << " ";
		}
		cout << "" << endl;
	}
}
//regex parser_
void Parse(token **Program, int rows){
	token *tokens;
	string line[rows];
	for(int li=0; li<rows;li++){
		tokens = Program[li];
		int i = 0;
		while(!tokens[i].value.empty()){
			line[li] += tokens[i++].type;
			line[li] += " ";
		}
		cout <<li+1<<": "<< line[li] << endl;
	}
}
int main(int argc, char **argv)
{
	if(argv[1] == 0){
		cout << "Arquivo não encontrado." << endl;
		return 0;}
	reset_bc();
	int rows;
	token **Program = Lex(argv[1],rows);
	Parse(Program,rows);
    //printProg(Program,rows);
	return 0;
}
