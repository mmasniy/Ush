#include "../../inc/ush.h"

void mx_parse_line(t_info *info, char **line) {
    mx_tilde_work(info, line, *line);
    // printf("line = %s\n", *line);
    mx_execute_substitutions(info, line);
    // mx_shell_functions(info, line);
    // del_quotes(line);
    // mx_replace_spaces_and_slash(line);
    if (info->status > 0) {
        printf("Error !\n");
        return;
    }
    // printf("-------------------------------------\n%s\n-------------------------------------\n", *line);
}

// void mx_parse_line(t_info *info, char **line) {
//     search_just_slash(line);
//     if (mx_get_char_index(*line, '$') >= 0)
//         mx_insert_value(info, line, *line);
//     mx_tilde_search(line, *line);// ~
//     if (mx_get_char_index(*line, '=') >= 0)
//         mx_find_and_add_key_value(info, line, *line);
//     mx_shell_functions(info, line);
// }

bool mx_is_allowed_symbol_for_tilde(char c) {
    if ((c > 47 && c < 58) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)
        || c == '-' || c == '_')
        return 1;
    return 0;
}

bool mx_tilde_search(t_info *info, char **line) {
    for (int pos = 0; (*line)[pos]; pos++) {
        if (mx_skip_single_quotes(*line, &pos)
            || mx_skip_double_quotes(*line, &pos)
            || mx_skip_substitutions(*line, &pos)) {
            return (info->status = 1);
        }
        printf("+%s+\n", &((*line)[pos]));
        if ((*line)[pos] == '~') {
        }
    }
    return 0;
}

static char findchar(char c) {
    char t = -1;

    t = c == 'n' ? 10 :
    c == 't' ? 9 :
    c == 'v' ? 11 :
    c == 'b' ? 8 :
    c == 'a' ? 7 :
    c == 'r' ? 013 :
    c == '\"' ? 34 :
    c == '\'' ? 39 :
    c == '\?' ? 63 :
    c == '\\' ? 92 :
    c == '\0' ? 0 : -1;
    return t;
}

void mx_search_slash(char **line) { 
    char type = -1;
    char *craft = *line;
    char *new_line = mx_strnew(mx_strlen(*line));

    for (int i = 0; (i = mx_get_char_index(craft, '\\')) >= 0;) {
        if (i >= 0 && craft[i + 1] && ((type = findchar(craft[i + 1])) >= 0)) {
            strncat(new_line, craft, i);
            new_line[mx_strlen(new_line)] = type;
        }
        else
            strncat(new_line, craft, i + 2);
        if (craft[i + 1])
            craft += i + 2;
        else
            craft += i + 1;
    }
    strcat(new_line, craft);
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}
