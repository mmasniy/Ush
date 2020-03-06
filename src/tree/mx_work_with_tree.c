#include "../../inc/ush.h"

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
    return mx_valid_tree_and_input(&tree, tok, i);
}

t_ast *mx_create_leaf(t_tok *max, int side) {
    t_tok *next = NULL;
    
    if (side == 1) {
        if (!(next = mx_search_first(max)))
            return NULL;
        if (next->type == 1)
            next->type = 2;
    }
    else if (side == 0 && max) {
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
    t_tok *tmp = NULL;
    t_ast *tree = NULL;

    if (max) {
        tmp = max;
        tree = mx_create_ast(tmp);
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
    }
    return tree;
}

int mx_check_op(int p) {
    if (p > 0 && p < 10)
        return 1;
    return 0;
}
