#include <string>
#include <stdlib.h>
#include <regex>

using namespace std;

typedef struct token{
	string value;
	string type;
	int len;
	int pos;
} token;

typedef struct node{
    token word;
    node* left;
    node* right;
} node;

node* createNode(token data);
node* turnLeft(node* node1);
node* turnRight(node* node1);
node* setTree(token* line);
