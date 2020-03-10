#include "../../inc/ush.h"

static char findchar(char c) {
    char t = -1;

    t = c == 'n' ? 10 : t;
    t = c == 't' ? 9 : t;
    t = c == 'v' ? 11 : t;
    t = c == 'b' ? 8 : t;
    t = c == 'a' ? 7 : t;
    t = c == 'r' ? 013 : t;
    t = c == '\"' ? 34 : t;
    t = c == '\'' ? 39 : t;
    t = c == '\?' ? 63 : t;
    t = c == '\\' ? 92 : t;
    t = c == '\0' ? 0 : t;

    return t;
}

char mx_is_quotes(char *line, int pos) {
    int open = -1;
    int close = -1;

    for (int i = 0; i < pos; i++) {
        if ((line[i] == '\'' || line[i] == '"')
            && (i == 0 || line[i - 1] != '\\')) {
            close = line[i] == '\'' ? mx_get_char_index(line + i + 1, '\'')
            : mx_char_block(line + i + 1, '\\', '"', '\0');
            if (i + close + 1 > pos)
                return line[i];
            else
                i += close + 1;
        }
    }
    return '\0';
}

bool mx_parse_line(t_info *info, char **line) {
    int symbol = -1;

    if (mx_check_open_close_symbols(info, *line, symbol, 0) == 1)
        return 1;
    mx_tilde_work(info, line, *line);
    if (mx_execute_substitutions(info, line))
        return 1;
    if (mx_check_open_close_symbols(info, *line, symbol, 0) == 1)
        return 1;
    return 0;
}

bool mx_is_allowed_symbol_for_tilde(char c) {
    if ((c > 47 && c < 58) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)
        || c == '-' || c == '_')
        return 1;
    return 0;
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
