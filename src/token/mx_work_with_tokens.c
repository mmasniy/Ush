#include "../../inc/ush.h"

static int check_rep(t_tok **tok, t_info *i) {
    t_tok *tmp1 = *tok;

    while (tmp1) {
        if (tmp1 && tmp1->next && tmp1->type == 1 && tmp1->prio
            == tmp1->next->prio && tmp1->prio != 10 && tmp1->prio != 15) {
            mx_error_mes_tree(tmp1->prio, i);
            mx_free_toks(tok);
            return 0;
        }
        tmp1 = tmp1->next;
    }
    return 1;
}

static int check_oper(t_tok **tok, t_info *i) {
    t_tok *tmp1 = *tok;
    int flag = 0;

    while (tmp1) {
        if (tmp1 && tmp1->next && mx_redirection(tmp1->prio)) {
            flag++;
        }
        tmp1 = tmp1->next;
    }
    if (flag > 1) {
        mx_printerr(MX_RED);
        mx_free_toks(tok);
        return 0;
    }
    return 1;
}

static int valid_red(t_tok **tok, t_info *i, int flag) {
    t_tok *tmp = NULL;

    tmp = *tok;
    while (tmp) {
        mx_check_tok(&tmp, tmp->token, 0);
        tmp = tmp->next;
    }
    if (!check_rep(tok, i))
        return 0;
    if (!check_oper(tok, i))
        return 0;
    return 1;
}

static int end_work(t_tok **tok, t_info *info, char **tmp) {
    while (*tok && (*tok)->prev)
        *tok = (*tok)->prev;
    mx_strdel(tmp);
    if (tok && !valid_red(tok, info, 0))
            return 0;
    return 1;
}

int mx_work_w_toks(char *line, t_tok **tok, t_info *info) {
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
        if (*(tmp + i) != 32)
            mx_add_tok(tok, tmp + i, size);
        i += size;
    }
    // fprintf(stderr, "%slist: %s\n", GRN, RESET);
    // fprintf(stderr, "%s---------------------------------------------%s\n", MAG, RESET);
    // for (t_tok *temp = *tok; temp; temp = temp->next) {
    //      fprintf(stderr, "%s[%s%s%s%s%s]%s ",GRN , RESET, YEL, temp->token, RESET, GRN, RESET);
    // }
    // fprintf(stderr, "\n\n");
    // for (t_tok *temp = *tok; temp; temp = temp->next) {
    //      fprintf(stderr, "%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->type, RESET, GRN, RESET);
    // }
    // fprintf(stderr, "\n\n");
    // for (t_tok *temp = *tok; temp; temp = temp->next) {
    //      fprintf(stderr, "%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->prio, RESET, GRN, RESET);
    // }
    // fprintf(stderr, "\n%s---------------------------------------------%s\n", MAG, RESET);
    // fprintf(stderr, "\n");
    if (!end_work(tok, info, &tmp))
        return 0;
    return 1;
}
