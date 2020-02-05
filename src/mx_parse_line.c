#include "ush.h"

static char findchar(char c);
static void search_just_slash(char **line);

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

void mx_parse_line(t_info *info, char **line) {
    search_just_slash(line);
    if (mx_get_char_index(*line, '$') >= 0)
        mx_insert_value(info, line, *line);
    mx_tilde_search(line, *line);// ~
    if (mx_get_char_index(*line, '=') >= 0)
        mx_find_and_add_key_value(info, line, *line);
    mx_shell_functions(info, line);
}

static void search_just_slash(char **line) {
    char *new_line = mx_strnew(mx_strlen(*line));
    int pos = 0;
    int i = 0;

    for (; (pos = mx_get_char_index(&((*line)[i]), '\\')) >= 0; i++) {
        if (pos >= 0) {
            strncat(new_line, &((*line)[i]), pos);
            i += pos;
            if ((*line)[i + 1] == '\\' && strcat(new_line, "\\"))
                i++;
        }
        else {
            strcat(new_line, &((*line)[i]));
            break;
        }
    }
    strcat(new_line, &((*line)[i]));
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
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
