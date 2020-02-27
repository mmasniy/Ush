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

int mx_valid_red(t_tok **tok) {
    t_tok *tmp = *tok;
    // int start = 0;
    // t_tok *tmp1 = *tok;
    // int prio = 0;

    while (tmp) {
        mx_check_tok(&tmp, tmp->token, 0);
        tmp = tmp->next;
    }
    // printf("1\n");
    // while (tmp1) {
    //     // printf("2\n");
    //     // printf("tmp1->token = %s\n", tmp1->token);
    //     if (tmp1 && tmp1->type == 1 && start == 0) {
    //         prio = tmp1->prio;
    //         start++;
    //     }
    //     if (prio == 8 && tmp1->prio == 5)
    //         continue;
    //     if (tmp1 && tmp1->type == 1 && tmp1->prio == prio && (tmp1->prio == 4
    //         || tmp1->prio == 5 || tmp1->prio == 8 || tmp1->prio == 9
    //         || (tmp1->prio > 10 && tmp1->prio < 15) || (tmp1->prio > 15
    //         && tmp1->prio < 21))) {
    //         mx_free_toks(tok);
    //         mx_print_red_err(1);
    //         return 0;
    //     }
    //     // printf("4\n");
    //     if (tmp1 && tmp1->type == 1 && tmp1->prio != prio)
    //         start--;
    //     // printf("5\n");
    //     tmp1 = tmp1->next; 
    // }
    // if (tok) {
    //     printf("yeap!!\n");
    // }
    return 1;
}

int mx_work_w_toks(char *line, t_tok **tok) {
    int size = 1;
    int i = 0;
    char *tmp;
    bool not_valid = 0;

    if (!line)
        return 0;
    tmp = mx_strdup(line);
    while (size > 0 && *(tmp + i)) {
        if ((size = mx_get_size_tok(tmp + i)) == -1) {
            mx_strdel(&tmp);
            return 0;
        }
        if (*(tmp + i) != 32)
            mx_add_tok(tok, tmp + i, size);
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
    // system("leaks ush");
    mx_del_slash_and_quotes_in_list(tok, &not_valid);
    // system("leaks ush");
    for (t_tok *tmp_tok = *tok; tmp_tok; tmp_tok = tmp_tok->next)
        if (strcmp(tmp_tok->token, "" ) == 0 || not_valid) {
            mx_strdel(&tmp);
            return 0;
        }
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
    mx_strdel(&tmp);
    if (!mx_valid_red(tok))
        return 0;
    return 1;
}
