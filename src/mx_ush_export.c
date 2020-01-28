#include "ush.h"

static void print_error(char *arg);
static bool check_arg(char *arg);
static bool change_in_list(t_info *info, char *key, char *value, int j);
static void go_export(t_info *info, char *key_value, char *key, char *value);

int mx_export(t_info *info, t_process *p) {
	if (p) {}
    if (info->args[1]) {
        char *key = NULL;
        char *value = NULL;

        for (int i = 1; info->args[i]; i++) {
            if (check_arg(info->args[i])) {
                go_export(info, info->args[i], key, value);
            }
        }
    }
    else
        for (t_export *tmp = info->to_export; tmp; tmp = tmp->next)
            printf("%s=%s\n", tmp->key, tmp->value);
	return 0;
}

static void print_error(char *arg) {
    mx_printerr("u$h: export: `");
    mx_printerr(arg);
    mx_printerr("': not a valid identifier\n");
}

static bool check_arg(char *arg) {
    if (!((arg[0] >= 65 && arg[0] <= 90) || (arg[0] >= 97 && arg[0] <= 122)
        || arg[0] == 95 || arg[0] == 61)) {
        print_error(arg);
        return 0;
    }
    for (int i = 0; arg[i]; i++) {
        if (!((arg[i] >= 48 && arg[i] <= 57) || (arg[i] >= 65 && arg[i] <= 90)
            || (arg[i] >= 97 && arg[i] <= 122)
            || arg[i] == 95 || arg[i] == 61)) {
            print_error(arg);
            return 0;
        }
    }
    return 1;
}

static bool change_in_list(t_info *info, char *key, char *value, int j) {
    t_export *find = NULL;

    if ((find = mx_search_key_in_list(info->to_export, key))) {
        if (j >= 0) {
            mx_strdel(&(find->key));
            mx_strdel(&(find->value));
            find->key = mx_strdup(key);
            find->value = mx_strdup(value);
        }
        find = mx_search_key_in_list(info->variables, key);
        if (j >= 0) {
            mx_strdel(&(find->key));
            mx_strdel(&(find->value));
            find->key = mx_strdup(key);
            find->value = mx_strdup(value);
        }
        return 1;
    }
    return 0;
}

static void go_export(t_info *info, char *key_value, char *key, char *value) {
    int j = -1;

    if ((j = mx_get_char_index(key_value, '=')) >= 0)
        key = mx_strndup(key_value, j);
    else
        key = mx_strdup(key_value);
    if (j >= 0 && key_value[j + 1])
        value = mx_strdup(&(key_value[j + 1]));
    else
        value = mx_strdup("''");
    if (change_in_list(info, key, value, j))
        setenv(key, value, 1);
    else {
        mx_push_export_back(&(info->variables), key, value);
        mx_push_export_back(&(info->to_export), key, value);
        if (strcmp(value, "''"))
            setenv(key, value, 1);
    }
    mx_strdel(&key);
    mx_strdel(&value);
}
