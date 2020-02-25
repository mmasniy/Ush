#include "../../inc/ush.h"

void mx_free_lst(t_tok *lst) {
    lst->next ? lst->next->prev = lst->prev : 0;
    lst->prev ? lst->prev->next = lst->next : 0;
    lst->next = NULL;
    lst->prev = NULL;
    if (lst->token)
        mx_strdel(&lst->token);
    free(lst);
}

void mx_add_num(t_tok **root, char *num, int i) {
    mx_strdel(&(*root)->token);
    if (num[i] == '>' && num[i + 1] == num[i] && num[i + 2] == '&') // >>&
        (*root)->prio = 16;
    else if (num[i] == num[i + 1] && (num[i] == '>' || num[i] == '<')) // >> || <<
        (*root)->prio = (num[i] == '<' ? 8 : 9);
    else if ((num[i] == '>' || num[i] == '<') && num[i + 1] == '&') // <& || >&
        (*root)->prio = (num[i] == '<' ? 12 : 13);
    else if ((num[i] == '>' || num[i] == '<')) // < || >
        (*root)->prio = (num[i] == '<' ? 4 : 5);
    (*root)->token = (i == 0 ? mx_strdup("1") : mx_strndup(num, i)); 
}

void mx_add_ampersand(t_tok **root, char *num) {
    if (num[1] == num[2] && (num[1] == '>' || num[1] == '<'))
        (*root)->prio = (num[1] == '>' ? 19 : 20); // &>> || &<<
    else if (num[1] == '>' || num[1] == '<')
        (*root)->prio = (num[1] == '<' ? 17 : 18); //&> || &<
    (*root)->token = mx_strdup("&");
}

void mx_check_tok(t_tok **tok, char *str, int i) {
    char *number = mx_strdup(str);

    while(mx_isdigit(str[i]))
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

void mx_valid_red(t_tok **tok) {
    t_tok *tmp = *tok;

    while (tmp) {
        mx_check_tok(&tmp, tmp->token, 0);
        tmp = tmp->next;
    }
}

int mx_work_w_toks(char *line, t_tok **tok) {
    int size = 1;
    int i = 0;
    char *tmp;

    if (!line)
        return 0;
    tmp = mx_strdup(line);
    while (size > 0 && *(tmp + i)) {
        if ((size = mx_get_size_tok(tmp + i)) == -1) {
            mx_strdel(&tmp);
            return 0;
        }
        if (*(tmp + i) != 32) {
            // mx_check_numbers_in_str(tmp + i, &size);
            mx_add_tok(tok, tmp + i, size);
        }
        i += size;
    }
    while (*tok && (*tok)->prev)
        *tok = (*tok)->prev;
    // printf("%slist: %s\n", GRN, RESET);
    // printf("%s---------------------------------------------%s\n", MAG, RESET);
    // for (t_tok *temp = *tok; temp; temp = temp->next) {
    //      printf("%s[%s%s%s%s%s]%s ",GRN , RESET, YEL, temp->token, RESET, GRN, RESET);
    // }
    // printf("\n\n");
    // for (t_tok *temp = *tok; temp; temp = temp->next) {
    //      printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->type, RESET, GRN, RESET);
    // }
    // printf("\n\n");
    // for (t_tok *temp = *tok; temp; temp = temp->next) {
    //      printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->prio, RESET, GRN, RESET);
    // }
    // printf("\n%s---------------------------------------------%s\n", MAG, RESET);
    // printf("\n");
    mx_del_slash_and_quotes_in_list(tok);
    printf("%slist: %s\n", GRN, RESET);
    printf("%s---------------------------------------------%s\n", MAG, RESET);
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s[%s%s%s%s%s]%s ",GRN , RESET, YEL, temp->token, RESET, GRN, RESET);
    }
    printf("\n\n");
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->type, RESET, GRN, RESET);
    }
    printf("\n\n");
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->prio, RESET, GRN, RESET);
    }
    printf("\n%s---------------------------------------------%s\n", MAG, RESET);
    printf("\n");
    mx_valid_red(tok);
    mx_strdel(&tmp);
    return 1;
}
