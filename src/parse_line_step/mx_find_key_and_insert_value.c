#include "../../inc/ush.h"

static int key_len(char *line) {
    int i = 0;

    if (line[0] >= '0' && line[0] <= '9')
        return 1;
    while (line[i] && mx_is_allow(line[i])) {
        i++;
    }
    return i;
}

static bool test_1(char *craft, int *pos, char **check) {
    bool result = 0;

    mx_strdel(check);
    if (craft[*pos + 1] == '{'
        && mx_get_char_index(craft + *pos + 1, '}') >= 0) {
        *check = strndup(craft + *pos + 2,
            mx_get_char_index(craft + *pos, '}') - 2);
        for (int c = 0; (*check)[c]; c++)
            if (mx_is_allow((*check)[c]) == 0) {
                fprintf(stderr, "${%s}: bad substitution\n", *check);
                result = 1;
            }
        *pos += 2;
    }
    if (result == 1)
        mx_strdel(check);
    return result;
}

bool mx_is_allow(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'
        || (c >= '0' && c <= '9') || c == '?' || c == '{')
        return 1;
    return 0;
}

void find_variable(t_info *info, char **check, char **new_line, int *pos) {
    t_export *find = NULL;
    char *tmp;

    if ((find = mx_search_key_in_list(info->variables, *check))) {
        if (find->value)
            mx_del_and_set(new_line, mx_strjoin(*new_line, find->value));
    }
    else if (strcmp(*check, "?") == 0) {
        tmp = mx_itoa(info->status);
        mx_del_and_set(new_line, mx_strjoin(*new_line, tmp));        
    }
    else if (getenv(*check))
        mx_del_and_set(new_line, mx_strjoin(*new_line, getenv(*check)));
    mx_strdel(&tmp);
    *pos += strlen(*check) + 1;
    mx_strdel(check);
}

bool mx_insert_value(t_info *info, char **line, char *craft) {
    char *new_line = mx_strnew(strlen(*line));
    char *check = NULL;
    int pos = 0;

    for (int i = 0; (i = mx_char_block(craft + pos, '\\', '$', '\0')) >= 0; ) {
        check = strndup(craft + pos, i);
        mx_del_and_set(&new_line, mx_strjoin(new_line, check));
        pos += i;
        if (mx_check_is_continue(craft, &pos, &new_line, &check))
            continue;
        if (test_1(craft, &pos, &check))
            return 1;
        if (check == NULL)
            check = mx_strndup(&(craft[pos + 1]), key_len(&(craft[pos + 1])));
        find_variable(info, &check, &new_line, &pos);
    }
    mx_del_and_set(&new_line, mx_strjoin(new_line, craft + pos));
    mx_del_and_set(line, new_line);
    mx_strdel(&new_line);
    return 0;
}
