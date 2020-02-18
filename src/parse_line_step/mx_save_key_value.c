#include "../../inc/ush.h"

static bool is_allow(char c);
static bool find_key_value(t_export **vars, char *arg);
static void save_all_variables(t_info *info, t_export **vars, bool is_ok);

void mx_find_and_add_key_value(t_info *info, char **line, char *craft) {
    bool all_ok = 1;
    t_export *vars = NULL;
    char **args = mx_strsplit(craft, ' ');
    char *new_line = mx_strnew(strlen(*line));

    for (int i = 0; args[i]; i++) {
        for (; *craft && mx_isspace(*craft); craft++)
            strncat(new_line, craft, 1);
        if (mx_get_char_index(args[i], '=') == -1)
            all_ok = 0;
        if (find_key_value(&vars, args[i]))
            while (*craft && !mx_isspace(*craft) && (craft++));
        else
            while (*craft && !mx_isspace(*craft))
                strncat(new_line, craft++, 1);
    }
    strcat(new_line, craft);
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
    mx_del_strarr(&args);
    save_all_variables(info, &vars, all_ok);
}

static void save_all_variables(t_info *info, t_export **vars, bool is_ok) {
    t_export *insert = NULL;

    if (is_ok) {
        for (t_export *tmp = *vars; tmp; tmp = tmp->next) {
            if ((insert = mx_search_key_in_list(info->variables, tmp->key))) {
                mx_strdel(&(insert->value));
                insert->value = strdup(tmp->value);
            }
            else
                mx_push_export_back(&(info->variables), tmp->key, tmp->value);
        }
    }
    while (*vars)
        mx_pop_export_front(vars);
}

static bool find_key_value(t_export **vars, char *arg) {
    int sign = -1;

    for (int i = 0; arg[i]; i++)
        if (arg[i] == '=') {
            sign = i;
            arg[i] = '\0';
            break;
        }
    for (int i = 0; arg[i]; i++)
        if (!is_allow(arg[i]))
            return 0;
    if (sign >= 0) {
        mx_push_export_back(vars, arg, &(arg[sign + 1]));
        return 1;
    }
    return 0;
}

static bool is_allow(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95
        || (c >= 48 && c <= 57))
        return 1;
    return 0;
}
