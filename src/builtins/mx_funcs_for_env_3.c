#include "../../inc/ush.h"

void mx_del_env_elem(t_export **env) {
    mx_strdel(&((*env)->key));
    mx_strdel(&((*env)->value));
    free(*env);
    *env = (*env)->next;
}

static int save_to_env(t_export **env, char **args, int sign, int *flags) {
    t_export *check;
    char *key;
    char *value;

    for (int i = sign; args[i]; i++)
        if ((sign = mx_get_char_index(args[i], '=')) > 0 && !(flags[3] = 0)) {
            key = mx_strndup(args[i], sign);
            value = mx_strdup(&(args[i][sign + 1]));
            if ((check = mx_search_key_in_list(*env, key))) {
                mx_strdel(&(check->value));
                check->value = mx_strdup(value);
            }
            else
                mx_push_export_back(env, key, value);
            mx_strdel(&key);
            mx_strdel(&value);
        }
        else
            return (flags[3] = i);
    return flags[3];
}

int mx_check_variable_create(t_export **env, char **args, int pos, int *fgs) {
    fgs[3] = pos;
    for (int i = pos; args[i]; i++) {
        if (args[i][0] == '=') {
            mx_printerr("env: setenv ");
            mx_printerr(args[i]);
            mx_printerr(": Invalid argument\n");
            fgs[0] = 1;
            return 0;
        }
    }
    return save_to_env(env, args, pos, fgs);
}

void mx_sort_print(t_export **list_to_sort) {
    bool check = 1;
    t_export *tmp = NULL;
    char *tmp_link_to_line = NULL;

    if (list_to_sort && *list_to_sort) {
        while (check) {
            check = 0;
            for (tmp = *list_to_sort; tmp->next; tmp = tmp->next)
                if (tmp->next && strcmp(tmp->key, tmp->next->key) > 0) {
                    tmp_link_to_line = tmp->key;
                    tmp->key = tmp->next->key;
                    tmp->next->key = tmp_link_to_line;
                    tmp_link_to_line = tmp->value;
                    tmp->value = tmp->next->value;
                    tmp->next->value = tmp_link_to_line;
                    check = 1;
                }
        }
    }
}
