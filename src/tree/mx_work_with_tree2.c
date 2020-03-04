#include "../../inc/ush.h"

static t_ast *mx_clean_free(t_ast **t) {
    if (malloc_size(*t)) {
        mx_free_tree(t);
        *t = NULL;
    }
    return *t;
}

static int valid2(t_ast **t, t_info* i, int type) {
    i->flag_for_valid++;
    i->file_not_f = 1;
    mx_error_mes_tree(type, i);
    return 0;
}

int mx_valid_tree(t_ast **t, t_tok *tok, t_info *i) {
    if ((*t)->type == 2)
        mx_error_mes_tree((*t)->type, i);
    if ((*t)->type == 1 && !(*t)->right && (*t)->left)
        (*t) = (*t)->left;
    if (i->type_e == 0)
        i->type_e = (*t)->type;
    if ((*t)) {
        if ((*t)->left && !((*t)->left->type == 1))
            mx_valid_tree(&(*t)->left, tok, i);
        if (i->flag_for_valid == 0) {
            if ((*t)->type > 0 && (*t)->type < 21 && (*t)->type != 10
                && (*t)->type != 15) {
                if (!(*t)->right || !(*t)->left)
                    return valid2(t, i, i->type_e);
            }
        }
        if ((*t)->right)
            mx_valid_tree(&(*t)->right, tok, i);
    }
    return 1;
}

t_ast *mx_valid_tree_and_input(t_ast **t, t_tok *tok, t_info *i) {
    t_ast *tmp = NULL;

    if (*t) {
        tmp = (*t); 
        if (!mx_valid_tree(&tmp, tok, i))
            *t = mx_clean_free(t);
    }
    return *t;
}
