#include "../../inc/ush.h"

static int check_name_alias(t_alias *als, char *name, char *value) {
    t_alias *tmp = als;

    while (tmp && name && value) {
        if (mx_strcmp(tmp->name, name) == 0){
            mx_strdel(&(tmp->value));
            tmp->value = mx_strdup(value);
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}


t_alias *mx_create_als(t_alias **als, char *alias, t_info *i) {
    char *name = mx_get_name_als(&alias, i, 0);
    char *value = NULL;
    t_alias *start = NULL;

    mx_get_value_als(*als, &alias, 0);
    alias ? value = mx_strdup(alias) : 0;
    if (value && name && mx_strcmp(value, " ") != 0
        && check_name_alias(i->alias, name, value) == 0) {
        start = (t_alias *)malloc(sizeof(t_alias));
        start->name = mx_strdup(name);
        start->value = mx_strdup(value);
        start->next = NULL;
    }
    mx_strdel(&name);
    mx_strdel(&value);
    return start;
}

void mx_add_newnode_als(t_alias **als, char *alias, t_info *i) {
    t_alias *new = mx_create_als(als, alias, i);

    if (*als) {
        t_alias *tmp = *als;

        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    else
        *als = new;
}

int mx_replace_als_to_cmd(t_alias *als, char **line, int i) {
    t_alias *tmp = NULL;
    char **arr_line = NULL;
    int count = 0;

    if (*line && (arr_line = mx_strsplit(*line, ' ')) != NULL)
        for (i = 0; arr_line[i]; i++) {
            tmp = als;
            for (; tmp; tmp = tmp->next){
                if (mx_strcmp(arr_line[i], tmp->name) == 0) {
                    mx_strdel(&(arr_line[i]));
                    arr_line[i] = mx_strdup(tmp->value);
                    count++;
                }
            }
        }
    *line = mx_strarr_to_str(arr_line, 0);
    mx_del_strarr(&arr_line);
    if (count == 0)
        return 0;
    return 1;
}
