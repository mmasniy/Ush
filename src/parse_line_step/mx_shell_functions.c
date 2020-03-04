#include "../../inc/ush.h"

static bool allow(char c, bool end) {
    if (!end) {
        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95
            || (c >= 48 && c <= 57) || mx_isspace(c))
            return 1;
    }
    else {
        if (mx_isspace(c) || (c >= 33 && c <= 122) || c == 124 || c == 126)
            return 1;
    }
    return 0;
}

static void save_shell_func(t_info *info, char **line, char **args, int *pos) {
    t_export *insert = NULL;
    char *value = mx_strndup(&((*line)[pos[2] + 1]), pos[3] - pos[2] - 1);

    for (int i = 0; args[i]; i++)
        if ((insert = mx_search_key_in_list(info->shell_funcs, args[i]))) {
            mx_strdel(&(insert->value));
            insert->value = strdup(value);
        }
        else
            mx_push_export_back(&(info->shell_funcs), args[i], value);
    mx_strdel(&value);
    mx_del_strarr(&args);
    if (pos[4])
        value = mx_strdup(&((*line)[pos[4] + 1]));
    else
        value = mx_strdup(&((*line)[pos[3] + 1]));
    free(*line);
    *line = mx_strdup(value);
    mx_strdel(&value);
}

static void check_line(t_info *info, char **line, char **args, int *pos) {
    if (pos[0] < pos[1] && pos[1] < pos[2] && pos[2] < pos[3]) {
        for (int i = pos[0] + 1; i < pos[1]; i++)
            if (!mx_isspace((*line)[i]))
                return;
        for (int i = pos[1] + 1; i < pos[2]; i++)
            if (!mx_isspace((*line)[i]))
                return;
        for (int i = pos[2] + 1; i < pos[3]; i++)
            if (!allow((*line)[i], 1))
                return;
        for (int i = pos[3] + 1; (*line)[i]; i++) {
            if ((*line)[i] == ';' && (pos[4] = i))
                break;
            if (!mx_isspace((*line)[i]))
                return;
        }
        save_shell_func(info, line, args, pos);
        if (pos[4])
            mx_shell_functions(info, line);
    }
}

static void find_functions(t_info *info, char **line, char *ln, char **check) {
    char *new = mx_strnew(mx_strlen(*line));
    int sz = 0;
    t_export *find = NULL;

    for (int pos = 0; ln[pos]; pos++) {
        if (mx_isspace(ln[pos]) && allow(ln[pos], 1))
            strncat(new, &(ln[pos]), 1);
        else {
            while (!mx_isspace(ln[pos + sz]) && (allow(ln[pos + sz], 1)
                || ln[pos + sz] == '{' || ln[pos + sz] == '}'))
                sz++;
            *check = strndup(&(ln[pos]), sz);
            if ((find = mx_search_key_in_list(info->shell_funcs, *check))) {
                new = realloc(new, malloc_size(new) + strlen(find->value));
                strcat(new, find->value);
            }
            else
                strncat(new, &(ln[pos]), sz);
            sz - 1 ? pos += sz - 1 : 0;
            mx_strdel(check);
        }
    }
    mx_del_and_set(line, new);
}

void mx_shell_functions(t_info *info, char **line) {
    int pos[5] = {0, 0, 0, 0, 0};
    char **args = NULL;
    char *tmp = NULL;
    char *check = NULL;

    find_functions(info, line, *line, &check);
    if ((pos[0] = mx_get_char_index(*line, '(')) >= 0
        && (pos[1] = mx_get_char_index(*line, ')')) >= 0
        && (pos[2] = mx_get_char_index(*line, '{')) >= 0
        && (pos[3] = mx_get_char_index(*line, '}')) >= 0) {
        for (int i = 0; i < pos[0]; i++)
            if (!allow((*line)[i], 0))
                return;
        tmp = strndup(*line, pos[0]);
        args = mx_strsplit(tmp, ' ');
        mx_strdel(&tmp);
        check_line(info, line, args, pos);
        mx_del_strarr(&args);
    }
}
