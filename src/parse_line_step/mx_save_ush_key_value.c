#include "../../inc/ush.h"

static bool check_and_add_arg(t_export **tmp_values, char *key, char *value) {
    for (int i = 0; key[i]; i++)
        if (!((key[i] >= 'a' && key[i] <= 'z')
            || (key[i] >= 'A' && key[i] <= 'Z')
            || key[i] == '_' || key[i] == '$'))
            return 1;
    if (value)
        for (int i = 0; value[i]; i++)
            if (!((value[i] >= 'a' && value[i] <= 'z')
                || (value[i] >= 'A' && value[i] <= 'Z')
                || mx_get_char_index(MX_EXPORT_VALUE_ALLOW, value[i]) != -1))
                return 1;
    mx_push_export_back(tmp_values, key, value);
    return 0;
}

static bool del_variables(bool res, char **key, char **value, char **arg) {
    mx_strdel(key);
    mx_strdel(value);
    mx_strdel(arg);
    return res;
}

static bool take_key_value(t_export **tmp_values, char *line, int *i) {
    int j = -1;
    char *key = NULL;
    char *value = NULL;
    char *argument = NULL;

    for (; line[*i] && mx_isspace(line[*i]) == 0; (*i)++) ;
    argument = strndup(line, *i + 1);
    if ((j = mx_get_char_index(argument, '=')) > 0) {
        key = mx_strndup(argument, j);
        value = strdup(argument + j + 1);
    }
    else if (j == -1 || j == 0) {
        return del_variables(1, &argument, &key, &value);
    }
    if (check_and_add_arg(tmp_values, key, value) == 0)
        return del_variables(0, &argument, &key, &value);
    return del_variables(1, &argument, &key, &value);
}

static void end_step(t_info *info, char **line,
                     t_export **tmp_values, int pos) {
    for (t_export *tmp = *tmp_values; tmp; tmp = tmp->next)
        if (tmp->key && tmp->value)
            mx_update_key_value(&(info->variables),
                                &(tmp->key), &(tmp->value));
        else
            fprintf(stderr, "We don't have key or value.\n");
    mx_del_and_set(line, strdup((*line) + pos));
}

void mx_save_ush_key_value(t_info *info, char **line, char *craft) {
    t_export *tmp_values = NULL;
    char *new_line = mx_strnew(strlen(*line));
    int valid = 1;
    int pos = 0;

    for (; craft[pos]; pos++) {
        if (mx_isspace(craft[pos]))
            while(craft[pos] && mx_isspace(craft[pos]))
                pos++;
        if (craft[pos] && mx_isspace(craft[pos]) == 0)
            if ((valid = take_key_value(&tmp_values, craft, &pos)) != 1)
                break;
    }
    valid == 1 ? end_step(info, line, &tmp_values, pos) : 0;
    if (valid == 2) {
        new_line = mx_strjoin(strdup((*line) + pos), strndup((*line), pos));
        mx_del_and_set(line, new_line);
    }
    while (tmp_values)
        mx_pop_export_front(&tmp_values);
}
