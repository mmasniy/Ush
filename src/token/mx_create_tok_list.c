#include "../../inc/ush.h"

int mx_check(char *token);

void mx_check_file_in_or_out(t_tok *prev, t_tok *next);

// char *mx_check_red(char *str, int *size) {
//     int i = 0;
//     char *ret;

//     if (size) {}
//     if (mx_isdigit(str[i])) {
//         while (mx_isdigit(str[i]))
//             i++;
//         if (!str)
//             return NULL;
//         (*size) = i - 1;
//         ret = mx_strndup(str, i - 1);
//         return ret;
//         printf("ret = %s\n", ret);
//         printf("number\n");
//     }
//     return NULL;
// }

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

// void mx_add_tok(t_tok **prev, char *cont, int size) {
//     t_tok *new_tok = (t_tok*)malloc(sizeof(t_tok));

//     if (!cont)
//         return ;
//     new_tok->token = mx_strndup(cont, size);
//     new_tok->type = mx_check_type(new_tok->token);
//     new_tok->prio = mx_check_priority(new_tok->token);
//     new_tok->prev = NULL;
//     new_tok->next = NULL;
//     if (*prev) {
//         t_tok *tmp = *prev;

//         while (tmp->next)
//             tmp = tmp->next;
//         tmp->next = new_tok;
//         new_tok->prev = tmp;
//     }
//     else
//         *prev = new_tok;
//     mx_check_file_in_or_out(new_tok->prev, new_tok);
// }
