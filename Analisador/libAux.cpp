#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <regex>

#include "libAux.hpp"

using namespace std;

node* createNode(token word){
	node* node = new node;
	node.word = data;
	node->left = NULL;
	node->right = NULL;
	return node;
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
node* deleteNode(node* node){
	if(node->right == NULL && node->left != NULL){
		node* temp = node->left;
		delete node;
		return temp;
	}
	else if(node->left == NULL && node->right != NULL){
		node* temp = node->right;
		delete node;
		return temp;
	}
	else if(node->left != NULL && node->right != NULL){
		//TO_DO
	}
	else{
		delete node;
		return NULL;
	}
}
void setTree2_helper(node* node){
	ite = node;
	if((ite.word.type.compare("id_var")==0 || ite.word.type.compare("num")==0) && || !ite.word.value.compare("return")==0){
		if(ite->right.type.compare("sym_math")==0){
			ite = turnleft(ite);
			setTree2_helper(ite);
		}
	}
	if(ite->right.word.value.compare("(")==0){
		ite->right = deleteNode(ite->right);
		node* branch,branch_ite;
		branch_ite = branch;
		while(true){
			if(ite->right.word.value.comapre(")")==0){
				ite->right = deleteNode(ite->right);
				break;
			}else{
				branch_ite = createNode(ite->right.word);
				branch_ite = branch_ite->right;
				ite->right = deleteNode(ite->right);
			}
		}
		setTree2_helper(branch);
		ite->right->left = branch;
		setTree2_helper(ite);
	}
	if(ite.word.type.compare("sym_math")==0){
		if(ite->right.word.type.compare("id")==0 || ite.word.type.compare("num")==0){
			if(ite->right->right.word.type.compare("sym_math")==0){
				ite->right = turnleft(ite->right);
				if(regex_match(ite.word.value,regex ("[/]|[*]|%")) && regex_match(ite->right.word.value,regex ("[-]|[+]"))){
					ite = turnleft(ite);
				}else{
					ite = ite->right;
				}
			}
		}
		setTree2_helper(ite);
	}
	if(ite->right.word.compare)
	if(ite->right.word.compare(";")==0){
		ite = deleteNode(ite->right);
		return;
	}
}
void setTree2(node* root){
	node* ite = root;
	//node* simlist;
	if(ite.word.type.compare("id_var")==0 || ite.word.value.compare("return")==0){
		setTree2_helper(ite);
	}
}
node* tree_setup(token* line, int row){
	node* root;
	node* ite = root;
	int i=0;
	node* counterPos;
	string opener = line[i].value;
	if(opener.compare("id_var")==0){
		while(!line[i].value.empty()){
			ite = createNode(line[i++]);
			if(ite.word.value.compare("(")==0){
				node* counter = createNode(ite.token);
				if(counterPos.token.value.empty()){
					counterPos = counter;
				}
				else if(counterPos->right == NULL){
					counterPos->right = counter;
					counter->left = counterPos;
					counterPos = counterPos->right;
				}
			}
			if(ite.word.value.compare(")")==0){
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
		if(!counterPos.token.value.empty()){
			cout << "Parenteses a esquerda sem fechamento, linha: "<< row << ", "<< counterPos.word.pos << endl; 
			exit(1);
		}
		delete counterPos;
		setTree2(root);
	}
	if(opener.compare("if")==0 || opener.compare("while")==0 ){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("if")==0|| line[i].value.compare("while")==0 || line[i].value.compare("(")==0 || line[i].value.compare(")")==0)
			{ oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0 || line[i].type.compare("num")==0 || line[i].type.compare("op_rel")==0 || line[i].type.compare("op_log")==0)
			{ oneliner += line[i++].type;}
		}
		regex regex_ifer("(not)?(id_var|num|ifexp)(op_rel|op_log)(not)?(id_var|num|ifexp)|if\\(ifexp\\)|while\\(ifexp\\)|\\(ifexp\\)")
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
	if(opener.compare("var")){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("var")==0||line[i].value.compare("int")==0||line[i].value.compare("float")==0||line[i].value.compare("char")==0||line[i].value.compare("-")==0||line[i].value.compare(",")==0||line[i].value.compare(";")==0)
				{oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0)
				{oneliner += line[i++].type;}
		}
		regex regex_var("varid_var(,id_var)*-(int|float|char);");
		if(regex_match(oneliner,regex_var)){
			ite->word = "var";
			ite->left = NULL;
			ite->right = NULL;
		}
	}
	if(opener.compare("proc")){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("proc")==0||line[i].value.compare("int")==0||line[i].value.compare("float")==0||line[i].value.compare("char")==0||line[i].value.compare("-")==0||line[i].value.compare("(")==0||line[i].value.compare(")")==0)
				{oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0||line[i].type.compare("id_proc")==0)
				{oneliner += line[i++].type;}
		}
		regex regex_proc("procid_proc\\(id_var-(int|float|char)\\)(id_proc\\(id_var-(int|float|char)\\))+-(int|float|char)");
		if(regex_match(oneliner,regex_proc)){
			ite->word = "proc";
			ite->left = NULL;
			ite->right = NULL;
		}
	}
	if(opener.compare("prnt")){
		string oneliner = "";
		while(!line[i].value.empty()){
			if(line[i].value.compare("prnt")==0||line[i].value.compare(";")==0||line[i].value.compare("(")==0||line[i].value.compare(")")==0)
				{oneliner += line[i++].value;}
			if(line[i].type.compare("id_var")==0||line[i].type.compare("wrd")==0||line[i].type.compare("num")==0)
				{oneliner += line[i++].type;}
		}
		regex regex_prnt("prnt\\((wrd|id_var|num)(,(wrd|id_var|num))*\\);");
		if(regex_match(oneliner,regex_prnt)){
			ite->word = "prnt";
			ite->left = NULL;
			ite->right = NULL;
		}
	}
	return root;
}