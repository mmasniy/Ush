#include "../../inc/ush.h"

static void exec_tilde(t_info *info, t_ast *tree) {
    for (int i = 0; tree->command[i]; i++)
        mx_insert_value(info, &(tree->command[i]), tree->command[i]);
}

int mx_tree_run(t_ast *tree, t_info *info, int f) {
    exec_tilde(info, tree);
    if (tree && (tree->type == 10 ||  mx_redirection(tree->type)))
        f = mx_start_function(tree, info, tree->command);
    else if (tree && tree->type == 3)
        mx_run_pipe(tree, info);
    else if (tree && tree->type == 1) {
        f = mx_tree_run(tree->left, info, 0);
        f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && tree->type == 6) {
        f = mx_tree_run(tree->left, info, 0);
        if (info->status == 0)
            f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && tree->type == 7) {
        f = mx_tree_run(tree->left, info, 0);
        if (info->status != 0)
            f = mx_tree_run(tree->right, info, 0);
    }
    return f;
}

void mx_tok_to_tree(t_tok *tok, t_info *i) {
    if (!tok)
        return;
    i->flag_for_valid = 0;
    i->type_e = 0;
    i->file_not_f = 0;
    i->fd_r = -2;
    i->fd_f = -1;
    i->t = mx_start_tree(tok, i);
    if (i->t)
        mx_tree_run(i->t, i, 0);
    if (i->t)
        mx_free_tree(&(i->t));
}
