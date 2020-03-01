#include "../../inc/ush.h"

static bool check_arg(char *key, char *value) {
    for (int i = 0; key[i]; i++)
        if (!((key[i] >= 'a' && key[i] <= 'z')
            || (key[i] >= 'A' && key[i] <= 'Z')
            || key[i] == '_' || key[i] == '$'))
            return 1;
    if (value)
        for (int i = 0; value[i]; i++)
            if (!((value[i] >= 'a' && value[i] <= 'z')
                || (value[i] >= 'A' && value[i] <= 'Z')
                || mx_get_char_index(EXPORT_VALUE_ALLOW, value[i]) != -1))
                return 1;
    return 0;
}

static bool print_error(char *arg) {
    mx_printerr("u$h: export: `");
    mx_printerr(arg);
    mx_printerr("': not a valid identifier\n");
    return 1;
}

static bool check_arg_and_take_key_value(char *arg, char **key, char **value) {
    int j = -1;

    if ((j = mx_get_char_index(arg, '=')) > 0) {
        *key = mx_strndup(arg, j);
        *value = strdup(arg + j + 1);
    }
    else if (j == 0)
        return print_error(arg);
    else
        *key = mx_strdup(arg);
    if (check_arg(*key, *value) == 0)
        return 0;
    return 1;
}

static void go_export(t_info *info, char **key, char **value) {
    if (*value == NULL) {
        t_export *find = mx_search_key_in_list(info->variables, *key);

            if (find && find->value) {
                *value = strdup(find->value);
            }
        find = NULL;
    }
    if (*value == NULL) {
        mx_update_key_value(&(info->variables), key, value);
        mx_update_key_value(&(info->to_export), key, value);
    }
    else {
        mx_update_key_value(&(info->variables), key, value);
        mx_update_key_value(&(info->to_export), key, value);
        setenv(*key, *value, 1);
    }

}

bool mx_update_key_value(t_export **list, char **key, char **value) {
    if (list && *list) {
        t_export *find = mx_search_key_in_list(*list, *key);

        if (find) {
            mx_strdel(&(find->key));
            mx_strdel(&(find->value));
            find->key = mx_strdup(*key);
            find->value = *value ? mx_strdup(*value) : NULL;
        }
        else {
            mx_push_export_back(list, *key, *value);
        }
        return 0;
    }
    return 1;
}

int mx_ush_export(t_info *info) {
    if (info->args[1]) {
        char *key = NULL;
        char *value = NULL;

        for (int i = 1; info->args[i]; i++) {
            if (!check_arg_and_take_key_value(info->args[i], &key, &value)) {
                go_export(info, &key, &value);
            }
            mx_strdel(&key);
            mx_strdel(&value);
        }
    }
    else
        for (t_export *tmp = info->to_export; tmp; tmp = tmp->next)
            printf("%s=%s\n", tmp->key, tmp->value ? tmp->value : "");
    return 0;
}
// a-z
// A-Z
// 0-9
// _$
// 
// a-z
// A-Z
// 0-9
// _ $ + - # ( ) ? : < > | & = . , / ~
// EXPORT_VALUE_ALLOW