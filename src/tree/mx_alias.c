#include "../../inc/ush.h"

void mx_get_value_als(t_alias *a, char **alias, int i) { 
    t_alias *als = NULL;
    char **temp_als = mx_strsplit(*alias, ' ');

    for (i = 0; temp_als[i]; i++) {
        als = a;
        while (als) {
            if (mx_strcmp(temp_als[i], als->name) == 0) {
                mx_strdel(&(temp_als[i]));
                temp_als[i] = mx_strdup(als->value);
                break;
            }
            als = als->next;
        }
    }
    mx_strdel(alias);
    *alias = mx_strarr_to_str(temp_als, 0);
    mx_del_strarr(&temp_als);
}

char *mx_get_name_als(char **alias, t_info *i, int count) {
    char *name;
    char *tmp;

    while ((*alias)[count] && (*alias)[count] != '=')
        count++;
    name = mx_strndup(*alias, count);
    tmp = mx_strndup(((*alias) + count + 2),
        mx_strlen((*alias)) - count - 1);
    mx_strdel(alias);
    *alias = mx_strndup(tmp, mx_strlen(tmp) - 1);
    mx_strdel(&tmp);
    return name;
}

void mx_add_alias(t_ast *t, t_info *i, int a) {
    int count = 0;
    char *alias = NULL;
    t_alias *als = i->alias;

    while (t->command[a]) {
        count += mx_count_substr(t->command[a], "alias");
        a++;
    }
    if (count == 1 && mx_arr_size(t->command) == 2) {
        alias = mx_strarr_to_str(t->command, 1);
        mx_add_newnode_als(&als, alias, i);
    }
    mx_strdel(&alias);
    i->alias = als;
}
