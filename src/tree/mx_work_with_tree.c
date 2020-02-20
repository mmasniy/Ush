#include "../../inc/ush.h"

// void PrintLKP(t_ast *Tree);
void printKLP(t_ast* root);

static t_ast *valid2(t_ast **t, t_info* i, int type) {
	i->flag_for_valid++;
	i->file_not_f = 1;
	mx_error_mes_tree(type);
	if (malloc_size(*t)) {
		mx_free_tree(*t);
		*t = NULL;
	}
	return *t;
}

t_ast *mx_valid_tree(t_ast **t, t_tok *tok, t_info *i) {
	if ((*t)->type == 1 && !(*t)->right)
		(*t) = (*t)->left;
	if (i->type_e == 0)
		i->type_e = (*t)->type;
	if ((*t)) {
		if ((*t)->left)
			mx_valid_tree(&(*t)->left, tok, i);
		if (i->flag_for_valid == 0) {
			if ((*t)->type > 0 && (*t)->type < 21 && (*t)->type != 10
				&& (*t)->type != 15) {
				if (!(*t)->right || !(*t)->left)
					return valid2(t, i, i->type_e);
			}
		}
		if ((*t)->right)
			mx_valid_tree(&(*t)->right, tok, i);
	}
	return (*t);
}

t_ast *mx_start_tree(t_tok *tok, t_info *i) {
	t_tok *tmp = mx_search_first(tok);
	t_ast *tree = mx_create_ast(tmp);
	if (tmp->prio == 10 || tmp->prio == 15) {
		tree->command = mx_merge_command(tmp);
	}
	else if (mx_check_op(tmp->prio) == 1) {
		tmp->type = 2;
		tree->command = mx_merge_op(tmp);
		tree->right = mx_create_leaf(tmp->next, 1);
		if (tree->right)
			tree->right->father = tree;
		tree->left = mx_create_leaf(tmp->prev, 0);
		if (tree->left)
			tree->left->father = tree;
	}
	if (i) {}
	print_all(tree, tok);
	return mx_valid_tree(&tree, tok, i);
}

t_ast *mx_create_leaf(t_tok *max, int side) {
	t_tok *next = NULL;

	if (side == 1) {
		if (!(next = mx_search_first(max)))
			return NULL;
		if (next->type == 1)
			next->type = 2;
	}
	else if (side == 0) {
		while (max->prev && (max->type == 0 || max->type == 1))
			max = max->prev;
		if (max->type == 2)
			max = max->next;
		if (!(next = mx_search_first(max)))
			return NULL;
		if (next->type == 1)
			next->type = 2;
	}
	return mx_build_ast(next);
}

t_ast *mx_build_ast(t_tok *max) {
	t_tok *tmp = max;
	t_ast *tree = mx_create_ast(tmp);
	if (tmp->prio == 10 || tmp->prio == 15)
		tree->command = mx_merge_command(tmp);
	if (mx_check_op(tmp->prio) == 1)
		tree->command = mx_merge_op(tmp);
	if (tmp->next && tmp->next->type != 2)
		tree->right = mx_create_leaf(tmp->next, 1);
	if (tree->right)
		tree->right->father = tree;
	if (tmp->prev && tmp->prev->type != 2)
		tree->left = mx_create_leaf(tmp->prev, 0);
	if (tree->left)
		tree->left->father = tree;
	return tree;
}

int mx_check_op(int p) {
	if (p > 0 && p < 10)
		return 1;
	return 0;
}

void print_all(t_ast *tree, t_tok *tok) {
if (tree && tok) {}
	// printf("%sTree: %s\n", GRN, RESET);
 //    printf("%s---------------------------------------------%s\n", MAG, RESET);
	// printKLP(tree);
	// printf("\n%s---------------------------------------------%s\n", MAG, RESET);
 //    printf("\n");

	// printf("%slist: %s\n", GRN, RESET);
 //    printf("%s---------------------------------------------%s\n", MAG, RESET);
 //    for (t_tok *temp = tok; temp; temp = temp->next) {
 //         printf("%s[%s%s%s%s%s]%s ",GRN , RESET, YEL, temp->token, RESET, GRN, RESET);
 //    }
 //    printf("\n\n");
 //    for (t_tok *temp = tok; temp; temp = temp->next) {
 //         printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->type, RESET, GRN, RESET);
 //    }
 //    printf("\n\n");
 //    for (t_tok *temp = tok; temp; temp = temp->next) {
 //         printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->prio, RESET, GRN, RESET);
 //    }
 //    printf("\n%s---------------------------------------------%s\n", MAG, RESET);
 //    printf("\n");
}

void mx_printf_strarr(char **str) {
    if (str) {}
	// printf("%s[%s", GRN, RESET);
	// for (int i = 0; str[i]; i++) {
	// 		if (str[i + 1])
	// 			printf("%s%s%s \n",YEL, str[i], RESET);
	// 		else
	// 			printf("%s%s%s",YEL, str[i], RESET);
	// }
	// printf("%s]%s ",GRN, RESET);
}

void printKLP(t_ast* root) {
    // int i = 0;

    if (root) {
    // 	if (i == 0)
    // 		printf("%sFATHER:%s\n", MAG, RESET);
    // 	else
    // 		printf("%sCHILD:%s\n", MAG, RESET);
    // 	i++;
    // 	printf("%scommand:%s\n", RED, RESET);
    //     mx_printf_strarr(root->command);
    //     printf("\n");
    //     printf("%stype:%s %s%d%s\n", RED, RESET, YEL, root->type, RESET);
    //     if (root->father)
    //     	printf("%sfather->%s%s[%s%s%s%s%s]%s\n\n", MAG, RESET, GRN, RESET, YEL, root->father->command[0], RESET, GRN, RESET);
    //     if (root->left)
    //     	printf("%sleft->%s%s[%s%s%s%s%s]%s\n\n", MAG, RESET, GRN, RESET, YEL, root->left->command[0], RESET, GRN, RESET);
    // 	if (root->right)
    //     printf("%sright->%s%s[%s%s%s%s%s]%s\n\n", MAG, RESET, GRN, RESET, YEL, root->right->command[0], RESET, GRN, RESET);
    //     printKLP(root->left);
    //     printKLP(root->right);
    }
}
