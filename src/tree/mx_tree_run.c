#include "../../inc/ush.h"

static void trim(char **str) {
    if (str && *str) {
        int len = mx_strlen(*str);
        int start = 0;
        int end = 0;
        char *res;

        for (; (*str)[start] && mx_isspace((*str)[start]); start++);
        for (int j = len - 1; (*str)[j] && mx_isspace((*str)[j]); j--, end++);
        if (start + end >= len) {
            mx_del_and_set(str, mx_strnew(0));
            return;
        }
        res = mx_strnew(len - start - end);
        mx_strncpy(res, (*str) + start, len - start - end);
        mx_strdel(str);
        *str = strdup(res);
        mx_strdel(&res);
    }
}

static void exec_tilde(t_info *info, t_ast *tree) {
    int size = 0;

    for (int i = 0; tree->command[i]; i++)
        mx_insert_value(info, &(tree->command[i]), tree->command[i]);
    for (int i = 0; tree->command[i]; i++)
        trim(&(tree->command[i]));
    for (int i = 0; tree->command[i]; i++)
        if (tree->command[i][0])
            size++;
    if (size) {
        int pos = 0;
        char **massiv = (char **)malloc(sizeof(char *) * (size + 1));

        massiv[size] = NULL;
        for (int i = 0; tree->command[i]; i++)
            if (tree->command[i][0])
                massiv[pos++] = strdup(tree->command[i]);
        mx_del_strarr(&(tree->command));
        tree->command = massiv;
    }
}

static bool parse_before_exec(t_info *info, t_ast *tree) {
    bool not_valid = 0;
    char *line = mx_strarr_to_str(tree->command, 0);

    mx_add_alias(tree, info, 0);
    exec_tilde(info, tree);
    mx_del_slash_and_quotes_in_list(tree, &not_valid);
    if (mx_replace_als_to_cmd(info->alias, &(line), 0)) {
        mx_del_strarr(&(tree->command));
        tree->command = mx_strsplit(line, ' ');
    }
    if (!tree || !(tree->command[0])
        || strcmp(tree->command[0], "") == 0 || not_valid) {
        return 0;
    }
    return 1;
}

int mx_tree_run(t_ast *tree, t_info *info, int f) {
    parse_before_exec(info, tree);
    if (tree && (tree->type == 10 ||  mx_redirection(tree->type)))
        f = mx_start_function(tree, info, tree->command);
    else if (tree && tree->type == 3)
        mx_run_pipe(tree, info);
    else if (tree && tree->type == 1) {
        tree->left ? f = mx_tree_run(tree->left, info, 0) : 0;
        tree->right ? f = mx_tree_run(tree->right, info, 0) : 0;
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
    if (i->t) {
        mx_tree_run(i->t, i, 0);
    }
    if (i->t)
        mx_free_tree(&(i->t));
}
