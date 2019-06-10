#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>

#include "libAux.hpp"

using namespace std;

regex regex_number("[0-9]*,?[0-9]*(([Ee][-+])?[0-9]+)");
//regex regex_int("[0-9]+");
regex regex_keyword("\\b(if|else|while|return|float|char|void|prnt|int|and|or|not|e|proc|var|main)\\b");
regex regex_symbols ("(?![0-9]),(?![0-9])|\\*\\*|:=|=|<<|>>|!=|==|:|;|\\(|\\)|[\"]|[{]|[}]");
regex regex_op_rel(">=|<=|<|>");
regex regex_op_math("[+]|[-]|[*]|[/]|%");
regex regex_id("(?!')[a-z]([a-zA-Z_]|[0-9])*(?!')");
regex regex_string("'([0-9]|[a-zA-Z]|\n|\t| |:|\\(|\\)|,)'");

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
typedef struct regist{
	string value;
	string type;
	struct regist* next;
} regist;
regist* createRegist(string value, string type){
	regist *r = new regist;
	r->value = value;
	r->type = type;
	r->next = NULL;
	return r;
}
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
regist* ids = new regist;
int procFlag=0;
void addRegEntry(regist* reg,string value, string type){
	if(reg->value.empty()){ 
		reg->value = value;
		reg->type = type;
		reg->next = NULL;
	}
	else{
		while(reg->next != NULL ){
			reg = reg->next;
		}
		reg->next = createRegist(value,type);
	}
}
void printMatch_helper(token* lines, string line, regex reg, string type, int li){
	regist *ids_ite = ids;
    sregex_iterator currentMatch(line.begin(),line.end(),reg);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
    	token aux;
    	aux.type = type;
        smatch match = *currentMatch;
		if(type.compare("id")==0){
			if(regex_match(match.str(),regex_keyword)){
				currentMatch++;
				continue;
			}
			smatch m;
			while(regex_search(line,m,regex ("proc|var"))){
				if(m.size() == 1 && procFlag == 0 && m.str().compare("proc")==0){
					aux.type = "id_"+m.str();
					if( m.str().compare("proc")==0) procFlag = 1;
					addRegEntry(ids_ite,match.str(),"proc");
					break;
				}
				else if(m.size() == 1 && (procFlag == 1 || m.str().compare("var")==0)){
					aux.type = "id_var";
					addRegEntry(ids_ite,match.str(),"var");
					break;
				}
			}
			if(m.empty()){
				regist* ite = ids;
				while(ite != NULL ){
					if(ite->value == match.str()){
						aux.type = type+"_"+ite->type;
						break;
					}
					ite = ite->next;
				}
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
			if(match.position() >= lineComment.pos){
				currentMatch++;
				continue;
			}
		}
		if(match.position() >= blockComment.start.pos && blockComment.status == 'S'){
			if(blockComment.end.line <= blockComment.start.line){
				currentMatch++;
				continue;
			} else {
				if(match.position() <= blockComment.end.pos+1){
					currentMatch++;
					continue;
		}}}
		if(regex_match(match.str(),regex ("[(]|[)]|:| |,")) && type.compare("sym")==0){
			if(line[match.position()-1]=='\'' && line[match.position()+1]=='\''){
				currentMatch++;
				continue;
			}
		}
		aux.value = match.str();
		aux.len = match.length();
		aux.pos = match.position();
		if(match.str().compare(":=")==0) aux.type += "_att";
		if(match.str().compare("=")==0) aux.type += "_rel";
		
		lines[match.position()] = aux;
        //cout <<"-"<< match.str() <<"-(pos:"<< match.position() << "," << type << ") ";
		currentMatch++;
    }
}
token* printMatch(string line,int li){
	token *lines = new token[sizeof(token)*line.length()];
	    
	printMatch_helper(lines,line,regex_symbols,"sym", li);
	printMatch_helper(lines,line,regex_number,"num", li);
	//printMatch_helper(lines,line,regex_int,"int", li);
	printMatch_helper(lines,line,regex_op_rel,"sym_rel", li);
	printMatch_helper(lines,line,regex_op_math,"sym_math", li);
	printMatch_helper(lines,line,regex_id,"id", li);
	printMatch_helper(lines,line,regex_keyword,"key", li);
	printMatch_helper(lines,line,regex_string,"wrd", li);
	if(blockComment.status == 'S'){
		lineComment.pos = -1;
		lineComment.status = 'N';
	}
	if(blockComment.end.pos > blockComment.start.pos && blockComment.end.line >= blockComment.start.line){
		reset_bc();
	}
	if(procFlag == 1){ procFlag = 0;}
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
			cout << Program[i][j].value <<":"<<Program[i][j++].type << " ";
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
	//Parse(Program,rows);
    printProg(Program,rows);
	return 0;
}
