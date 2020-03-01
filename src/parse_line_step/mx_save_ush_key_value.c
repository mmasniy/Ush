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
                || mx_get_char_index(EXPORT_VALUE_ALLOW, value[i]) != -1))
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

    for (; line[*i] && mx_isspace(line[*i]) == 0; (*i)++);

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

void mx_save_ush_key_value(t_info *info, char **line, char *craft) {
    t_export *tmp_values = NULL;
    char *new_line = mx_strnew(strlen(*line));
    int valid = 1;
    int pos = 0;

    for (; craft[pos]; pos++) {
        if (mx_isspace(craft[pos]))
            while(craft[pos] && mx_isspace(craft[pos]))
                pos++;
        if (craft[pos] && mx_isspace(craft[pos]) == 0) {
            if ((valid = take_key_value(&tmp_values, craft, &pos)) != 1) { // take key/value and check arg
                break;
            }
        }
    }
    // end step
    if (valid == 1) {
        for (t_export *tmp = tmp_values; tmp; tmp = tmp->next) {
            if (tmp->key && tmp->value)
                mx_update_key_value(&(info->variables)
                    , &(tmp->key), &(tmp->value));
            else
                printf("We don't have key or value.\n");
        }
        // save
        mx_del_and_set(line, strdup(craft + pos));
    }
    else if (valid == 2) {
        new_line = mx_strjoin(strdup(craft + pos), strndup(craft, pos));
        mx_del_and_set(line, new_line);
    }
    mx_strdel(&new_line);
    while (tmp_values)
        mx_pop_export_front(&tmp_values);
}

// #include "../../inc/ush.h"

// static bool is_allow(char c) {
//     if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95
//         || (c >= 48 && c <= 57))
//         return 1;
//     return 0;
// }

// static void save_all_variables(t_info *info, t_export **vars, bool is_ok) {
//     t_export *insert = NULL;

//     if (is_ok) {
//         for (t_export *tmp = *vars; tmp; tmp = tmp->next) {
//             if ((insert = mx_search_key_in_list(info->variables, tmp->key))) {
//                 mx_strdel(&(insert->value));
//                 insert->value = strdup(tmp->value);
//             }
//             else
//                 mx_push_export_back(&(info->variables), tmp->key, tmp->value);
//         }
//     }
//     while (*vars)
//         mx_pop_export_front(vars);
// }

// static bool find_key_value(t_export **vars, char *arg) {
//     int sign = -1;

//     for (int i = 0; arg[i]; i++)
//         if (arg[i] == '=') {
//             sign = i;
//             arg[i] = '\0';
//             break;
//         }
//     for (int i = 0; arg[i]; i++)
//         if (!is_allow(arg[i]))
//             return 0;
//     if (sign >= 0) {
//         mx_push_export_back(vars, arg, &(arg[sign + 1]));
//         return 1;
//     }
//     return 0;
// }

// void mx_find_and_add_key_value(t_info *info, char **line, char *craft) {
//     bool all_ok = 1;
//     t_export *vars = NULL;
//     char **args = mx_strsplit(craft, ' ');
//     char *new_line = mx_strnew(strlen(*line));

//     for (int i = 0; args[i]; i++) {
//         for (; *craft && mx_isspace(*craft); craft++)
//             strncat(new_line, craft, 1);
//         if (mx_get_char_index(args[i], '=') == -1)
//             all_ok = 0;
//         if (find_key_value(&vars, args[i]))
//             while (*craft && !mx_isspace(*craft) && (craft++));
//         else
//             while (*craft && !mx_isspace(*craft))
//                 strncat(new_line, craft++, 1);
//     }
//     strcat(new_line, craft);
//     free(*line);
//     *line = mx_strdup(new_line);
//     mx_strdel(&new_line);
//     mx_del_strarr(&args);
//     save_all_variables(info, &vars, all_ok);
// }
