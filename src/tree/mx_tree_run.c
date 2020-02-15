#include "../../inc/ush.h"

int mx_tree_run(t_ast *tree, t_info *info, int f) {
    if (tree && (tree->type == 10 ||  mx_redirection(tree->type))) {
        f = mx_start_function(tree, info, tree->command);
    }
    else if (tree && tree->type == 3)
        mx_run_pipe(tree, info);
    else if (tree && tree->type == 1) {
        f = mx_tree_run(tree->left, info, 0);
        f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && tree->type == 6) {
        if ((f = mx_tree_run(tree->left, info, 0)))
            f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && tree->type == 7) {
        if (!(f = mx_tree_run(tree->left, info, 0)))
            f = mx_tree_run(tree->right, info, 0);
    }
    return f;
}

void mx_tok_to_tree(t_tok *tok, t_info *info) {
    t_ast *tree = NULL;
    int fd[3];

    if (!tok)
        return;
    fd[0] = dup(0);
    fd[1] = dup(1);
    fd[2] = dup(2);
    tree = mx_start_tree(tok);
    mx_tree_run(tree, info, 0);
    mx_free_tree(tree);
}

