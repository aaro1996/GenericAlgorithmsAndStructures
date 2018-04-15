#ifndef _TYPENAME_RED_BLACK_TREE_H_
#define _TYPENAME_RED_BLACK_TREE_H_

#ifndef TYPEFILE
#define TYPEFILE "../generic_type.h"
#endif

#include TYPEFILE

#define UNCLE(curr_iter) (((curr_iter)->parent == (curr_iter)->parent->parent->left) ? (curr_iter)->parent->parent->right : (curr_iter)->parent->parent->left)
#define SIBLING(curr_iter) (((curr_iter) == (curr_iter)->parent->left) ? (curr_iter)->parent->right : (curr_iter)->parent->left)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


struct rbt_node {
	GENTYPE data;
	struct rbt_node* parent;
	struct rbt_node* right;
	struct rbt_node* left;
	char color; //0 black 1 red
};
struct rbt {
	struct rbt_node* root;
};

int rbt_check(struct rbt* tree);

int rbt_print_all(struct rbt* tree);

int rbt_init(struct rbt** destination);

int rbt_insert(struct rbt* tree, GENTYPE data);

int rbt_extract(struct rbt* tree, GENTYPE target, GENTYPE* destination);
#endif