#include "../../inc/ush.h"

int mx_check_type(char *cont) {
    if ((mx_count_substr(MX_TYPE, cont)) == 0)
        return 0;
    else if ((mx_count_substr(MX_TYPE, cont)) != 0)
        return 1;
    return -1;
}

int mx_check_priority(char *c) {
    int p = -1;

    mx_strcmp(c, ";") == 0 || mx_strcmp(c, "\n") == 0 ? p = 1 : 0;
    mx_strcmp(c, "&") == 0 ? p = 2 : mx_strcmp(c, "|") == 0 ? p = 3 : 0;
    mx_strcmp(c, "<") == 0 ? p = 4 : mx_strcmp(c, ">") == 0 ? p = 5 : 0;
    mx_strcmp(c, "||") == 0 ? p = 7 : mx_strcmp(c, "&&") == 0 ? p = 6 : 0;
    mx_strcmp(c, "<<") == 0 ? p = 8 : mx_strcmp(c, ">>") == 0 ? p = 9 : 0;
    mx_strcmp(c, "<>") == 0 ? p = 11 : mx_strcmp(c, "<&") == 0 ? p = 12 : 0;
    mx_strcmp(c, ">&") == 0 ? p = 13 : mx_strcmp(c, ">>&") == 0 ? p = 16 : 0;
    mx_strcmp(c, "&>") == 0 ? p = 17 : mx_strcmp(c, "&<") == 0 ? p = 18 : 0;
    mx_strcmp(c, "&>>") == 0 ? p = 19 : mx_strcmp(c, "&<<") == 0 ? p = 20 : 0;
    if (p == -1)
        return 10;
    return p;
}

void mx_add_num(t_tok **root, char *num, int i) {
    mx_strdel(&(*root)->token);
    if (num[i] == '>' && num[i + 1] == num[i] && num[i + 2] == '&')
        (*root)->prio = 16;
    else if (num[i] == num[i + 1] && (num[i] == '>' || num[i] == '<'))
        (*root)->prio = (num[i] == '<' ? 8 : 9);
    else if ((num[i] == '>' || num[i] == '<') && num[i + 1] == '&')
        (*root)->prio = (num[i] == '<' ? 12 : 13);
    else if ((num[i] == '>' || num[i] == '<'))
        (*root)->prio = (num[i] == '<' ? 4 : 5);
    (*root)->type = 1;
    (*root)->token = (i == 0 ? mx_strdup("1") : mx_strndup(num, i));
}

void mx_add_ampersand(t_tok **root, char *num) {
    if (num[1] == num[2] && (num[1] == '>' || num[1] == '<'))
        (*root)->prio = (num[1] == '>' ? 19 : 20);
    else if (num[1] == '>' || num[1] == '<')
        (*root)->prio = (num[1] == '<' ? 17 : 18);
    (*root)->token = mx_strdup("&");
}

void mx_check_tok(t_tok **tok, char *str, int i) {
    char *number = mx_strdup(str);

    while (mx_isdigit(str[i]))
        i++;
    if (number[i] == '>' || number[i] == '<') {
        mx_add_num(tok, number, i);
        mx_strdel(&number);
        return;
    }
    else if (number[0] == '&' && number[1] != '&') {
        mx_add_ampersand(tok, number);
        mx_strdel(&number);
        return;
    }
    mx_strdel(&number);
}
