#include "../../inc/ush.h"

static bool is_allow(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95
        || (c >= 48 && c <= 57))
        return 1;
    return 0;
}

static int key_len(char *line) {
    int i = 0;

    while (line[i] && is_allow(line[i])) {
        i++;
    }
    return i;
}

void mx_insert_value(t_info *info, char **line, char *craft) {
    char *new_line = mx_strnew(strlen(*line));
    char *check;
    t_export *find_res = NULL;

    for (int pos = -1; (pos = mx_get_char_index(craft, '$')) >= 0;) {
        strncat(new_line, craft, pos);
        check = mx_strndup(&(craft[pos + 1]), key_len(&(craft[pos + 1])));
        if ((find_res = mx_search_key_in_list(info->variables, check))) { // find value
            new_line = realloc(new_line
                , malloc_size(new_line) + strlen(find_res->value));
            strcat(new_line, find_res->value);
        }
        craft += pos + 1 + (strlen(check));
        mx_strdel(&check);
    }
    strcat(new_line, craft);
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}
