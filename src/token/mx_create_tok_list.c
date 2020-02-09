#include "../../inc/ush.h"

void mx_add_tok(t_tok **prev, char *cont, int size) {
    t_tok *new_tok = (t_tok*)malloc(sizeof(t_tok));
    // int size2 = size;
    // char *red = mx_check_red(&cont, &size);

    if (!cont)
        return ;
    // printf("red = %s\n", red);
    // printf("cont = %s\n", cont);
    // if (red) {
        // printf("size = %d\n", size);
        // new_tok->token = mx_strndup(red, size);
        // printf("new_tok->token = %s\n", new_tok->token);
    // }
    // else {
        // printf("size2 = %d\n", size2);
        new_tok->token = mx_strndup(cont, size);
        // printf("new_tok->token = %s\n", new_tok->token);
    // }
    new_tok->type = mx_check_type(new_tok->token);

    new_tok->prio = mx_check_priority(new_tok->token);
    new_tok->prev = NULL;
    new_tok->next = NULL;
    if (*prev) {
        t_tok *tmp = *prev;

        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_tok;
        new_tok->prev = tmp;
    }
    else
        *prev = new_tok;
    // if (new_tok->prev)
    mx_check_file_in_or_out(new_tok->prev, new_tok);
}

char *mx_check_red(char **content, int *size) {
    char *red;
    char *tmp = mx_strnew((*size));
    int i = 0;

    (*size) = 0;
    for (;mx_isdigit((*content)[i]); i++)
        tmp[i] = (*content)[i];
    if (((*content)[i] == '<' || (*content)[i] == '>') && mx_isdigit((*content)[i - 1])) {
        red = mx_strdup(&(*content)[i]);
        (*size) = mx_strlen(red);
        if (!tmp) {
            mx_strdel(&(*content));
            (*content) = mx_strdup(tmp);
            mx_strdel(&tmp);
        }
        return red;
    }
    return NULL;
}

void mx_free_toks(t_tok **tok) {
    t_tok *tmp;
    while (*tok) {
        tmp = (*tok)->next;
        (*tok)->prev = NULL;
        mx_strdel(&((*tok)->token));
        free(*tok);
        *tok = tmp;
    }
}

void mx_check_file_in_or_out(t_tok *prev, t_tok *next) {
    if (next && prev && mx_check(prev->token) != -1) {
        next->prio = 15;
    }
}
