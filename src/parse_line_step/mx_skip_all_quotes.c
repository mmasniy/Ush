#include "../../inc/ush.h"

bool mx_skip_single_quotes(char *check, int *pos) {
    bool res = 0;
    int size = 0;
    char *sub = NULL;

    while (check[*pos + size] && check[*pos + size] == '\\')
        size++;
    if (check[*pos + size] == '\'') {
        sub = strndup(&(check[*pos]), size + 1);
        *pos += size + 1;
        if ((size = mx_get_substr_index(&check[*pos], sub)) >= 0)
            *pos += size + strlen(sub) - 1;
        else
            res = 1;
    }
    mx_strdel(&sub);
    return res;
}

bool mx_skip_double_quotes(char *check, int *pos) {
    bool res = 0;
    int size = 0;
    char *sub = NULL;

    while (check[*pos + size] && check[*pos + size] == '\\')
        size++;
    if (check[*pos + size] == '\"') {
        sub = strndup(&(check[*pos]), size + 1);
        *pos += size + 1;
        if ((size = mx_get_substr_index(&check[*pos], sub)) >= 0)
            *pos += size + strlen(sub) - 1;
        else
            res = 1;
    }
    mx_strdel(&sub);
    return res;
}

bool mx_skip_substitutions(char *check, int *pos) {
    bool res = 0;
    int size = 0;
    char *sub = NULL;

    while (check[*pos + size] && check[*pos + size] == '\\')
        size++;
    if (check[*pos + size] == '`') {
        sub = strndup(&(check[*pos]), size + 1);
        *pos += size + 1;
        if ((size = mx_get_substr_index(&check[*pos], sub)) >= 0)
            *pos += size + strlen(sub) - 1;
        else
            res = 1;
    }
    mx_strdel(&sub);
    return res;
}
