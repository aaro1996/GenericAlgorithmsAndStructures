#include "gen_red_black_tree.h"

int rbt_print_all_helper(unsigned long* printing, int level, struct rbt_node* node);

int rbt_print_all_helper(unsigned long* printing, int level, struct rbt_node* node) {
	if(node == NULL) {
		printf("\n");
		return 0;
	}
	if(node->color == 1) {
		printf("\033[1;31m");
	} else if (node->color < 0) {
		printf("\033[0;32m");
	} else {
		printf("\033[1;36m");
	}
	
	char parent_str[40];
	char node_str[40];

	if(node->parent) {
		GENTYPE_STRINGIFY(node->parent->data, parent_str, 40);
	}
	GENTYPE_STRINGIFY(node->data, node_str, 40);
	printf("[%s", (node->parent ? parent_str : "NULL"));
	if (!node->parent) {
		for(int i = 4; i < GENTYPE_PRINT_LENGTH; i++) {
			printf(" ");
		}
	}

	printf("][%s]", node_str);
	printf("\033[0m"); 
	if (node->left) {
		*printing |= (1L << level);
	}
	if (node->right) {
		printf("|->");
	}
	rbt_print_all_helper(printing, level+1, node->right);
	if(node->left != NULL) {
		for(int i = 0; i < level; i++) {
			for(int j = 0; j < GENTYPE_PRINT_LENGTH; j++) {
				printf("  ");
			}
			if(((*printing) >> i)&1L) {
				printf("%s", "    |  ");
			} else {
				printf("%s", "       ");
			}
		}
		for(int i = 0; i < GENTYPE_PRINT_LENGTH; i++) {
				printf("  ");
		}
		printf("    \\->");

		*printing &= (~(0x1L << level));
		rbt_print_all_helper(printing, level + 1, node->left);
	}
	return 0;
}

int rbt_print_all(struct rbt* tree) {
	unsigned long printing = 0;
	return rbt_print_all_helper(&printing, 0, tree->root);
}


int rbt_init(struct rbt** destination) {
	*destination = calloc(1, sizeof(struct rbt));
	if(!(*destination)) {return -1;} //fail on no alloc
	return 0;
}

int rbt_check_helper(struct rbt_node* node) {
	int right = 0;
	int left = 0;
	if ((node->right == node) || (node->left == node)) {
		return -4;
	}
	if (node->right != NULL) {
		if (node->color && node->right->color) {
			return -1;
		}
		if (node->right->parent != node) {
			return -5;
		}
		if (GENTYPE_LTE(node->right->data, node->data)) {
			return -6;
		}
		right = rbt_check_helper(node->right);
		if (right < 0) {
			return right;
		}
	}
	if (node->left != NULL) {
		if (node->color && node->left->color) {
			return -1;
		}
		if (node->left->parent != node) {
			return -5;
		}
		if (GENTYPE_GT(node->left->data, node->data)) {
			return -6;
		}
		left = rbt_check_helper(node->left);
		if (left < 0) {
			return left;
		}
	}
	if (left != right) {
		return -2;
	}
	if (node->color) {
		return left;
	} else {
		return (left + 1);
	}
}

int rbt_check(struct rbt* tree) {
	int result;
	if (tree->root == NULL) {
		return 0;
	}
	if (tree->root->color) {
		return -3;
	}
	result = rbt_check_helper(tree->root);
	if (result < 0) {
		return result;
	} else {
		return 0;
	}
}


int rbt_left_rotate(struct rbt* tree, struct rbt_node* node) {
	struct rbt_node* parent = node->parent;
	struct rbt_node* child = node->right;
	child->parent = parent;
	node->right = child->left;
	child->left = node;
	node->parent = child;
	if(node->right) {
		node->right->parent = node;
	}
	//fix with parent
	if(parent == NULL) {
		tree->root = child;
		return 0;
	}
	if(GENTYPE_GT(child->data, parent->data)) {
		parent->right = child;
	} else {
		parent->left = child;
	}
	return 0;
}

int rbt_right_rotate(struct rbt* tree, struct rbt_node* node) {
	struct rbt_node* parent = node->parent;
	struct rbt_node* child = node->left;
	child->parent = parent;
	node->left = child->right;
	child->right = node;
	node->parent = child;


	if(node->left) {
		node->left->parent = node;
	}
	//fix with parent
	if(parent == NULL) {
		tree->root = child;
		return 0;
	}
	if(GENTYPE_GT(child->data, parent->data)) {
		parent->right = child;
	} else {
		parent->left = child;
	}
	return 0;
}

int rbt_rr(struct rbt* tree, struct rbt_node* node) {
	rbt_left_rotate(tree, node);
	char color = node->color;
	node->color = node->parent->color;
	node->parent->color = color;
	return 0;
}

int rbt_ll(struct rbt* tree, struct rbt_node* node) {
	rbt_right_rotate(tree, node);
	char color = node->color;
	node->color = node->parent->color;
	node->parent->color = color;
	return 0;
}

int rbt_insert(struct rbt* tree, GENTYPE data) {
	if(tree->root == NULL) {
		struct rbt_node* new_node = calloc(1, sizeof(struct rbt_node));
		if(!new_node) {return -1;} //fail on no alloc
		new_node->data = data;
		tree->root = new_node;
		new_node->color = 0;
		return 0;
	}
	struct rbt_node* curr_iter = tree->root;
	struct rbt_node* prev_iter = NULL;
	while(curr_iter != NULL) {
		if(GENTYPE_LT(data, curr_iter->data)) { //left if less than the node
			prev_iter = curr_iter;
			curr_iter = curr_iter->left;
		} else if (GENTYPE_GT(data, curr_iter->data)){
			prev_iter = curr_iter;
			curr_iter = curr_iter->right;
		} else {
			return -1;
		}
	}
	curr_iter = calloc(1, sizeof(struct rbt_node));
	if(!curr_iter) {return -1;} //fail on no alloc
	curr_iter->data = data;
	curr_iter->parent = prev_iter;
	if (GENTYPE_GT(curr_iter->data, prev_iter->data)) {
		prev_iter->right = curr_iter;
	} else {
		prev_iter->left = curr_iter;
	}
	curr_iter->color = 1; //standard insert, new is red
	while(1) {
		if (curr_iter == tree->root) {
			curr_iter->color = 0;  //if node is root make it black and bail
			return 0;
		}
		if (!(curr_iter->parent->color)) { //if parent is black, we're done here
			return 0;
		}
		if ((UNCLE(curr_iter)) && (UNCLE(curr_iter)->color)) { //if uncle exists and is red
			curr_iter->parent->color = 0; //make parent black
			UNCLE(curr_iter)->color = 0; //make uncle black
			curr_iter->parent->parent->color = 1; //make grandpa red
			curr_iter = curr_iter->parent->parent;  //repeat with node as grandparent
			continue;
		}
		//uncle must be black
		int dir1 = GENTYPE_GT(curr_iter->parent->data, curr_iter->parent->parent->data); //0 left, 1 right
		int dir2 = GENTYPE_GT(curr_iter->data, curr_iter->parent->data); //0 left, 1 right
		if (dir1) { //right cases
			if(dir2) { //right right
				rbt_rr(tree, curr_iter->parent->parent);
			} else { //right left
				rbt_right_rotate(tree, curr_iter->parent);
				rbt_rr(tree, curr_iter->parent);
			}
		} else { //left cases
			if(dir2) { //left right
				rbt_left_rotate(tree, curr_iter->parent);
				rbt_ll(tree, curr_iter->parent);
			} else { //left left
				rbt_ll(tree, curr_iter->parent->parent);
			}
		}
		break;
	}
	return 0;
}

//returns 1 on successful extract, 0 if not there
int rbt_extract(struct rbt* tree, GENTYPE data, GENTYPE* destination) {
	struct rbt_node* curr_iter = tree->root;
	if (curr_iter == NULL) {
		return 0;
	}
	while((!GENTYPE_EQ(curr_iter->data, data))) {
		if(GENTYPE_LTE(data, curr_iter->data)) { //go left if lte
			curr_iter = curr_iter->left;
		} else { //go right
			curr_iter = curr_iter->right;
		}
		if(!curr_iter) {  //if we can't find it return 0
			return 0;
		}
	}
	*destination = curr_iter->data;  //extraction complete, now to fix the tree
	struct rbt_node* extraction_node = curr_iter;
	struct rbt_node* deletion_node;
	if(curr_iter->left) {
		curr_iter = curr_iter->left;
		while (curr_iter->right) {
			curr_iter = curr_iter->right;
		}
		deletion_node = curr_iter;
		extraction_node->data = deletion_node->data;
	} else {
		deletion_node = curr_iter;
	}
	struct rbt_node* child_node;
	if (deletion_node->right) {
		child_node = deletion_node->right;
	} else if (deletion_node->left) {
		child_node = deletion_node->left;
	} else {
		child_node = NULL;
	}
	if(child_node) {
		if(child_node->color || deletion_node->color) { //if one is red and one is black things are EZ PZ
			deletion_node->color = 0;
			deletion_node->data = child_node->data;
			deletion_node->left = child_node->left;
			deletion_node->right = child_node->right;
			if(deletion_node->right) deletion_node->right->parent = deletion_node;
			if(deletion_node->left) deletion_node->left->parent = deletion_node;
			free(child_node);
			return 1;
		} else { //well we copy the child in then we deal with DOUBLE BLACK
			deletion_node->color = -1;
			deletion_node->data = child_node->data;
			deletion_node->left = child_node->left;
			deletion_node->right = child_node->right;
			if(deletion_node->right) deletion_node->right->parent = deletion_node;
			if(deletion_node->left) deletion_node->left->parent = deletion_node;
			free(child_node);
		}
	} else {
		if(deletion_node == tree->root) {
			tree->root = NULL;
			return 1;
		}
		if(deletion_node->color) {
			if (deletion_node == deletion_node->parent->left) {
				deletion_node->parent->left = NULL;
			} else {
				deletion_node->parent->right = NULL;
			}
			free(deletion_node);
			return 1;
		}
		deletion_node->color = -2; //double black NULL
	}
	//
	curr_iter = deletion_node;
	while(curr_iter->color < 0) {
		if (curr_iter == tree->root) {  //if we hit root, c
			if(curr_iter->color == -2) {
				tree->root = NULL;
				free(curr_iter);
			} else {
				curr_iter->color = 0;
			}
			return 1;
		}
		struct rbt_node* sibling = SIBLING(curr_iter);
		if (!sibling) {
			rbt_print_all(tree);
		}
		//if sibling is black and at has at least one red child
		if(!sibling->color) { //if sibling black
			if ((sibling->left && sibling->left->color)||(sibling->right && sibling->right->color)) {
				if(sibling == sibling->parent->left) { //sibling is on left
					if (sibling->left && sibling->left->color) { //left child red i.e. left left
						sibling->color = curr_iter->parent->color;
						curr_iter->parent->color = 0;
						sibling->left->color = 0;
						rbt_right_rotate(tree, curr_iter->parent);
					} else { //left right
						rbt_left_rotate(tree, sibling);
						sibling->color = sibling->parent->color;
						sibling->parent->color = 0;
						sibling = SIBLING(curr_iter);
						sibling->color = curr_iter->parent->color;
						curr_iter->parent->color = 0;
						sibling->left->color = 0;
						rbt_right_rotate(tree, curr_iter->parent);
					}
				} else { //sibling is on right
					if (sibling->right && sibling->right->color) { //right child red i.e. right right
						sibling->color = curr_iter->parent->color;
						curr_iter->parent->color = 0;
						sibling->right->color = 0;
						rbt_left_rotate(tree, curr_iter->parent);
					} else { //right left
						rbt_right_rotate(tree, sibling);
						sibling->color = sibling->parent->color;
						sibling->parent->color = 0;
						sibling = SIBLING(curr_iter);
						sibling->color = curr_iter->parent->color;
						curr_iter->parent->color = 0;
						sibling->right->color = 0;
						rbt_left_rotate(tree, curr_iter->parent);					}
				}
				if(curr_iter->color == -2) {
					if (curr_iter == curr_iter->parent->left) {
						curr_iter->parent->left = NULL;
					} else {
						curr_iter->parent->right = NULL;
					}
					free(curr_iter);
				} else {
					curr_iter->color = 0;
				}
				return 1;
			} else { //all children are black, recolor and make the parent blacker, potentially double black
				struct rbt_node* parent = curr_iter->parent;
				sibling->color = 1;
				parent->color--;
				if(curr_iter->color == -2) {
					if (curr_iter == parent->left) {
						parent->left = NULL;
					} else {
						parent->right = NULL;
					}
					free(curr_iter);
				} else {
					curr_iter->color = 0;
				}
				curr_iter = parent;
			}
		} else { //if sibling is red
			if (sibling == sibling->parent->left) { //sibling is left child
				sibling->color = sibling->parent->color;
				sibling->parent->color = 1;
				rbt_right_rotate(tree, sibling->parent);
			} else { //sibling is right child
				sibling->color = sibling->parent->color;
				sibling->parent->color = 1;
				rbt_left_rotate(tree, sibling->parent);
			}
		}
	}
	return 1;
}