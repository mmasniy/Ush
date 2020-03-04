#include "../../inc/ush.h"

void mx_add_tok(t_tok **prev, char *c, int s) {
    t_tok *new_tok = (t_tok*)malloc(sizeof(t_tok));

    new_tok->token = mx_strndup(c, s);
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
    mx_check_file_in_or_out(new_tok->prev, new_tok);
}

void mx_free_toks(t_tok **tok) {
    t_tok *tmp = NULL;
    
    while (*tok) {
        tmp = (*tok)->next;
        (*tok)->prev = NULL;
        mx_strdel(&((*tok)->token));
        free(*tok);
        *tok = tmp;
    }
}

void mx_check_file_in_or_out(t_tok *prev, t_tok *next) {
    if (next && prev && mx_check(prev->token) == -1 && prev->prio != 10) {
        if (prev->prio == 1 && next->prio != 1)
            next->prio = 10;
        else if (prev->prio == 1 && next->prio != 1)
            next->prio = 15;
    }
}

void mx_free_lst(t_tok *lst) {
    lst->next ? lst->next->prev = lst->prev : 0;
    lst->prev ? lst->prev->next = lst->next : 0;
    lst->next = NULL;
    lst->prev = NULL;
    if (lst->token)
        mx_strdel(&lst->token);
    free(lst);
}
