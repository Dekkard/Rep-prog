#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>

//#include "libAux.hpp"

using namespace std;

regex regex_number("[0-9]*,?[0-9]*(([Ee][-+])?[0-9]+)");
//regex regex_int("[0-9]+");
regex regex_keyword("\\b(if|else|while|return|float|char|void|prnt|int|proc|var|main)\\b");
regex regex_symbols("(?![0-9]),(?![0-9])|\\*\\*|:=|=|<<|>>|:|;|\\(|\\)|[\"]|[{]|[}]");
regex regex_op_log("and|or|not");
regex regex_op_rel(">=|<=|<|>|<>");
regex regex_op_math("[+]|[-]|[*]|[/]|%");
regex regex_id("(?!')[a-z]([a-zA-Z_]|[0-9])*(?!')");
regex regex_string("'([0-9]|[a-zA-Z]|\n|\t| |:|\\(|\\)|,)'");

typedef struct posLine2{
	int pos;
	int line;
	struct posLine2* next;
} posLine2;
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
typedef struct token{
	string value;
	string type;
	int len;
	int pos;
} token;
typedef struct node{
    token word;
    struct node* left;
    struct node* right;
} node;
regist* createRegist(string value, string type){
	regist *r = new regist;
	r->value = value;
	r->type = type;
	r->next = NULL;
	return r;
}
posLine2* createPosLine2(int pos, int line){
	posLine2 *r = new posLine2;
	r->pos = pos;
	r->line = line;
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
posLine2* bracketFlag = new posLine2;
void startCountBrackets(){
	bracketFlag->pos = -1;
	bracketFlag->line = -1;
	bracketFlag->next = NULL;
}
void addPosLine2Entry(posLine2* pt,int pos, int line){
	if(pt->pos < 0){ 
		pt->pos = pos;
		pt->line = line;
		pt->next = NULL;
	}
	else{
		while(pt->next != NULL ){
			pt = pt->next;
		}
		pt->next = createPosLine2(pos,line);
	}
}
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
void lexMatch_helper(token* lines, string line, regex reg, string type, int li){
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
				if(m.size() == 1 && procFlag == 1 && m.str().compare("proc")==0){
					int ck = match.position()+1;
					while(true){
						if(line[ck]=='('){
							aux.type = "id_proc";
							addRegEntry(ids_ite,match.str(),"proc");
							break;
						} ck++;
						if(line[ck]=='-'){
							aux.type = "id_var";
							addRegEntry(ids_ite,match.str(),"var");
							break;
						} ck++;
					}
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
				if(match.str().compare(",")==0){
					if((line[match.position()-2]=='(' && line[match.position()-3]!='\'') || line[match.position()-2]==' '){
						currentMatch++;
						continue;}
				}else{
					currentMatch++;
					continue;
				}
			}
		}
		posLine2* brk_ite = bracketFlag;
		if(match.str().compare("{")==0 && type.compare("sym")==0){
			addPosLine2Entry(brk_ite,match.position(),li);
			currentMatch++;
			continue;
		}
		if(match.str().compare("}")==0 && type.compare("sym")==0){
			if(brk_ite->next == NULL){
				brk_ite->pos = -1;
				brk_ite->line = -1;
			}else{
				while(brk_ite->next->next != NULL ){
					brk_ite = brk_ite->next;
				}
				posLine2 *tmp = brk_ite->next;
				brk_ite->next = NULL;
				delete tmp;
			}
			currentMatch++;
			continue;
		}
		aux.value = match.str();
		aux.len = match.length();
		aux.pos = match.position();
		if(match.str().compare(":=")==0) aux.type += "_att";
		if(match.str().compare("=")==0) aux.type = "op_rel";
		
		lines[match.position()] = aux;
        //cout <<"-"<< match.str() <<"-(pos:"<< match.position() << "," << type << ") ";
		currentMatch++;
    }
}
token* lexMatch(string line,int li){
	token *lines = new token[sizeof(token)*line.length()];
	    
	lexMatch_helper(lines,line,regex_symbols,"sym", li);
	lexMatch_helper(lines,line,regex_number,"num", li);
	//lexMatch_helper(lines,line,regex_int,"int", li);
	lexMatch_helper(lines,line,regex_op_math,"sym_math", li);
	lexMatch_helper(lines,line,regex_op_rel,"op_rel", li);
	lexMatch_helper(lines,line,regex_op_log,"op_log", li);
	lexMatch_helper(lines,line,regex_id,"id", li);
	lexMatch_helper(lines,line,regex_keyword,"key", li);
	lexMatch_helper(lines,line,regex_string,"wrd", li);
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
	startCountBrackets();
	//token *Program[rows];
	token **Program = new token*[sizeof(token)*rows];
	int li = 0;
    while(getline(file, line)){
		token *lines = new token;
		token *temp, *aux;
		//cout << "Linha " << li+1 << ": ";
        temp = lexMatch(line,li+1);
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
	posLine2* brk_ite = bracketFlag;
	if(brk_ite->pos >= 0){
		while(brk_ite->next != NULL ){
			brk_ite = brk_ite->next;
		}
		cout << "Falta chaves a direita: linha" << brk_ite->line << ":"<< brk_ite->pos<<endl;
		exit(1);
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
node* createNode(token word){
	node* no = new node;
	no->word = word;
	no->left = NULL;
	no->right = NULL;
	return no;
}
node* turnLeft(node* node1){
	if(node1->right == NULL){
		return node1;
	}
	node* node2 = node1->right;
	node* temp = node2->left;
	node2->left = node1;
	node1->right = temp;
	return node2;
}
node* turnRight(node* node1){
	if(node1->left == NULL){
		return node1;
	}
	node* node2 = node1->left;
	node* temp = node2->right;
	node2->right = node1;
	node1->left = temp;
	return node2;
}
node* deleteNode(node* no){
	if(no->right == NULL && no->left != NULL){
		node* temp = no->left;
		delete no;
		return temp;
	}
	else if(no->left == NULL && no->right != NULL){
		node* temp = no->right;
		delete no;
		return temp;
	}
	else if(no->left != NULL && no->right != NULL){
		//TO_DO
	}
	else{
		delete no;
		return NULL;
	}
}
void setTree2_helper(node* no){
	node* ite = no;
	if((ite->word.type.compare("id_var")==0 || ite->word.type.compare("num")==0) || !ite->word.value.compare("return")==0){
		if(ite->right->word.type.compare("sym_math")==0){
			ite = turnLeft(ite);
			setTree2_helper(ite);
		}
	}
	if(ite->right->word.value.compare("(")==0){
		ite->right = deleteNode(ite->right);
		node* branch,*branch_ite;
		branch_ite = branch;
		while(true){
			if(ite->right->word.value.compare(")")==0){
				ite->right = deleteNode(ite->right);
				break;
			}else{
				branch_ite = createNode(ite->right->word);
				branch_ite = branch_ite->right;
				ite->right = deleteNode(ite->right);
			}
		}
		setTree2_helper(branch);
		ite->right->left = branch;
		setTree2_helper(ite);
	}
	if(ite->word.type.compare("sym_math")==0){
		if(ite->right->word.type.compare("id")==0 || ite->word.type.compare("num")==0){
			if(ite->right->right->word.type.compare("sym_math")==0){
				ite->right = turnLeft(ite->right);
				if(regex_match(ite->word.value,regex ("[/]|[*]|%")) && regex_match(ite->right->word.value,regex ("[-]|[+]"))){
					ite = turnLeft(ite);
				}else{
					ite = ite->right;
				}
			}
		}
		setTree2_helper(ite);
	}
	if(ite->right->word.value.compare(";")==0){
		ite = deleteNode(ite->right);
		return;
	}
}
void setTree2(node* root){
	node* ite = root;
	//node* simlist;
	if(ite->word.type.compare("id_var")==0 || ite->word.value.compare("return")==0){
		setTree2_helper(ite);
	}
}
node* tree_setup(token* line, int row){
	node* root = new node;
	node* ite = root;
	int i=0;
	node* counterPos;
	token opener = line[i];
	if(opener.type.compare("id_var")==0){
		while(!line[i].value.empty()){
			ite = createNode(line[i++]);
			if(ite->word.value.compare("(")==0){
				node* counter = createNode(ite->word);
				if(counterPos->word.value.empty()){
					counterPos = counter;
				}
				else if(counterPos->right == NULL){
					counterPos->right = counter;
					counter->left = counterPos;
					counterPos = counterPos->right;
				}
			}
			if(ite->word.value.compare(")")==0){
				if(counterPos->left == NULL){
					delete counterPos;
				}else{
					node* temp = counterPos;
					counterPos = counterPos->left;
					counterPos->right = NULL;
					delete temp;
				}
			}
			ite = ite->right;
		}
		if(!counterPos->word.value.empty()){
			cout << "Parenteses a esquerda sem fechamento, linha: "<< row << ", "<< counterPos->word.pos << endl; 
			exit(1);
		}
		delete counterPos;
		setTree2(root);
	}
	if(opener.value.compare("if")==0 || opener.value.compare("while")==0 ){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("if")==0|| line[i].value.compare("while")==0 || line[i].value.compare("(")==0 || line[i].value.compare(")")==0)
			{ oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0 || line[i].type.compare("num")==0 || line[i].type.compare("op_rel")==0 || line[i].type.compare("op_log")==0)
			{ oneliner += line[i++].type;}
		}
		regex regex_ifer("(not)?(id_var|num|ifexp)(op_rel|op_log)(not)?(id_var|num|ifexp)|if\\(ifexp\\)|while\\(ifexp\\)|\\(ifexp\\)");
		while(!oneliner.compare("ifexp")){
			smatch m;
			regex_search(oneliner,m,regex_ifer);
			if(m.size()>0){
				regex_replace(oneliner,regex_ifer,"ifexp");
			}
		}
		ite->word = opener;
		ite->left = NULL;
		ite->right = NULL;
	}
	if(opener.value.compare("var")==0){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("var")==0||line[i].value.compare("int")==0||line[i].value.compare("float")==0||line[i].value.compare("char")==0||line[i].value.compare("-")==0||line[i].value.compare(",")==0||line[i].value.compare(";")==0)
				{oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0)
				{oneliner += line[i++].type;}
		}
		regex regex_var("varid_var(,id_var)*-(int|float|char);");
		if(regex_match(oneliner,regex_var)){
			ite->word = opener;
			ite->left = NULL;
			ite->right = NULL;
		}
	}
	if(opener.value.compare("proc")==0){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("proc")==0||line[i].value.compare("int")==0||line[i].value.compare("float")==0||line[i].value.compare("char")==0||line[i].value.compare("-")==0||line[i].value.compare("(")==0||line[i].value.compare(")")==0||line[i].value.compare("main")==0)
				{oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0||line[i].type.compare("id_proc")==0)
				{oneliner += line[i++].type;}
		}
		regex regex_proc("procid_proc\\(id_var|main-(int|float|char)\\)(id_proc\\(id_var-(int|float|char)\\))+-(int|float|char)");
		if(regex_match(oneliner,regex_proc)){
			ite->word = opener;
			ite->left = NULL;
			ite->right = NULL;
		}
	}
	if(opener.value.compare("prnt")==0){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("prnt")==0||line[i].value.compare(";")==0||line[i].value.compare("(")==0||line[i].value.compare(")")==0||line[i].value.compare(",")==0)
				{oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0||line[i].type.compare("wrd")==0||line[i].type.compare("num")==0)
				{oneliner += line[i++].type;}
		}
		regex regex_prnt("prnt\\((wrd|id_var|num)(,(wrd|id_var|num))*\\);");
		if(regex_match(oneliner,regex_prnt)){
			ite->word = opener;
			ite->left = NULL;
			ite->right = NULL;
		}
	}
	return root;
}
void Parse(token **Program, int rows){
	token* tokens;
	node** root = new node*[sizeof(node)*rows];
	for(int li=0; li<rows;li++){
		tokens = Program[li];
		root[li] = tree_setup(tokens, li+1);
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
