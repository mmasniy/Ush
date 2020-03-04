#include "../../inc/ush.h"

t_tok *mx_search_first(t_tok *tok) {
    t_tok *max = tok;

    while (tok && tok->type != 2) {
        if (tok->prio <= max->prio)
            max = tok;
        if (!tok->next)
            break;
        tok = tok->next;
    }
    return max;
}

t_ast *mx_create_ast(t_tok *max) {
    t_ast *tree = NULL;

    if (max) {
        tree = (t_ast*)malloc(sizeof(t_ast));
        tree->command = NULL;
        tree->type = max->prio;
        tree->father = NULL;
        tree->right = NULL;
        tree->left = NULL;
    }
    return tree;
}

char **mx_merge_command(t_tok *t) {
    t_tok *new = t;
    int count = 0;
    int i = 0;
    char **cmd = NULL;

    while (new->prev && new->prev->type == 0) {
        count++;
        new = new->prev;
    }
    count++;
    cmd = (char **)malloc(sizeof(char *) * (count + 1));
    for (; count > i && new && new->type != 2; i++, new = new->next) {
        cmd[i] = mx_strdup(new->token);
        new->type = 2;
    }
    cmd[i] = NULL;
    return cmd;
}

char **mx_merge_op(t_tok *max) {
    char **cmd = NULL;

    if (max) {
        cmd = (char**)malloc(sizeof(char*) * 2);
        cmd[0] = mx_strdup(max->token);
        cmd[1] = NULL;
    }
    return cmd;
}

void mx_free_tree(t_ast **tree) {
    if (tree && *tree) {
        if ((*tree)->left)
            mx_free_tree(&((*tree)->left));
        if ((*tree)->right)
            mx_free_tree(&((*tree)->right));
        if ((*tree)->command)
            mx_del_strarr(&((*tree)->command));
        free((*tree));
        *tree = NULL;
    }
}
