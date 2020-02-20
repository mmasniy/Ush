#include "../../inc/ush.h"

int mx_redirection(int type) {
    if (type == 4 || type == 5 || type == 8 || type == 9 || type == 12
        || type == 13 || type == 16 || type == 17 || type == 18 || type == 19
        || type == 20)
        return 1;
    return 0;
}

int mx_create_file(t_ast *t, t_info *i) {
    if (t->type == 5 || t->type == 17)
        return (open(t->right->command[0],
            O_WRONLY | O_CREAT | O_TRUNC, 0600));
    if (t->type == 9 || t->type == 19)
        return (open(t->right->command[0],
            O_WRONLY | O_APPEND | O_CREAT, 0600));
    if (t->type == 4) {
        if (i->fname)
            mx_strdel(&i->fname);
        i->fname = mx_strdup(t->right->command[0]);
        return open (t->right->command[0], O_RDONLY, 0600);
    }
    if (t->type == 8) {
        mx_multi_line_enter(i, t->right->command[0]);
        return open(".system_ush.txt", O_RDONLY, 0600);
    }
    return -1;
}

/*
в ush_ было, обрати внимание
if (t->type == 4 && !path) {
        mx_strdel(&path);
        return (open (t->right->command[0],
            O_RDONLY | O_CREAT, 0600));
    }
    mx_strdel(&path);
*/

int mx_run_redirection(t_ast *t, t_info *i, pid_t pid) {
    if (i->fd_r == -1 && (t->type == 13 || t->type == 16)) {
        close(mx_atoi(t->command[0]));
    }
    else if (i->fd_r > 0 && (t->type == 5 || t->type == 9 || t->type == 13)) {
        dup2(i->fd_r, mx_atoi(t->command[0]));
        if (mx_atoi(t->command[0]) != 1)
            close(1);
    }
    else if (i->fd_r > 0 && (t->type == 4 || t->type == 8)) {
        if (i->fd_r != STDIN_FILENO){
            dup2(i->fd_r, 0);
            close(i->fd_r);
        }
    }
    return (mx_start_red(t->left, i, pid));
}

/*
else if (i->fd_r > 0 && (t->type == 17 || t->type == 19)){
        dup2(i->fd_r, 0);
        dup2(i->fd_r, 1);
        dup2(i->fd_r, 2);
    }
*/

void mx_multi_line_enter(t_info *info, char *key_word) {
    char *line = NULL;
    FILE *f = fopen(".system_ush.txt", "w+");

    mx_del_and_set(&(info->name), strdup(key_word));
    while (strcmp((line = mx_ush_read_line(info)), key_word)) {
        fprintf(f, "%s\n", line);
        mx_strdel(&line);
    }
    mx_del_and_set(&(info->name), USH);
    fflush(f);
    fclose(f);
}

