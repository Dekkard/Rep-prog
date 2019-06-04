#include <string>
#include <stdlib.h>
#include <regex>

#include "libAux.hpp"

using namespace std;

node* createNode(token data){
	node* node = new node;
	node.data = data;
	node->left = NULL;
	node->right = NULL;
	return node;s
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
	if(node->left == NULL && node->right != NULL){
		node* temp = node->right;
		delete node;
		return temp;
	}
	if(node->left != NULL && node->right != NULL){
		//TO_DO
	}
}
node* setTree_helper(token* word, int i){
	node ite;
	if(token.type.compare("id")==0 || token.type.compare("num")==0){
		ite->right = createNode(line[++]);
		ite = ite->right;
		if(token.type.compare("math")==0){
			ite->right = createNode(line[++]);
			ite = turnLeft(ite);
			if(token.type.compare("id")==0 || token.type.compare("num")==0){
				ite->right = createNode(line[++]);
				ite = ite->right;
			}
		}
	}
	return ite;
}
node* setTree(token* line){
	int i=0;
	node* root;
	node* temp;
	if(line[i].type.compare("id")==0){
		node* ite = createNode(line[i++]);
		while(!line[i].value.compare(";"){
			if(line[i].type.compare("sym_att")==0){
				ite->right = createNode(line[i++]);
				ite = turnLeft(ite);
				if(line[i].value.compare("(")==0){
					temp = setTree_helper(line[i],&i);
					if(line[i].value.compare(")")==0){
						
				}
			}
		}
	}
	return root
}
