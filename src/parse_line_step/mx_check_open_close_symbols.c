#include "../../inc/ush.h"

static bool error_return(t_info *info, char c) {
    info->status = 1;
    fprintf(stderr, "u$h: parse error near `%c'\n", c);
    return 1;
}

static int num_of_brackets_inside(char *line, int max_pos) {
    int num = 0;
    int pos = 0;

    for (int i = 0; (i = mx_char_block(line + pos, '\\', '(', '\0'))
                     >= 0 && i < max_pos; num++) {
        pos += i + 1;
    }
    return num;
}

bool mx_check_bracket(char *line, int *pos_in_line) {
    int pos = 0;
    int inside_num = -1;
    int close_num = 0;
    int close_pos = mx_char_block(line, '\\', ')', '\0');

    if (close_pos >= 0) {
        if ((inside_num = num_of_brackets_inside(line, close_pos))) {
            for (int i = 0; (i = mx_char_block(line + close_pos,
                 '\\', ')', '\0')) >= 0; close_num++)
                close_pos += i + 1;
            close_pos--;
            if (inside_num != close_num - 1) {
                return 1;
            }
        }
        *pos_in_line += close_pos + 1;
    }
    else
        return 1;
    return 0;
}

bool mx_check_open_close_symbols(t_info *info, char *ln, int symbol, int pos) {
    char *check_ln = mx_strnew(strlen(ln));
    char *tmp = NULL;

    if (mx_work_with_line(&tmp, &check_ln, &pos, ln))
        return 1;
    for (int i = 0; check_ln[i]; i++) {
        if (check_ln[i] == '(' && (i == 0 || check_ln[i - 1] != '\\')) {
            if (mx_check_bracket(check_ln + i + 1, &i))
                return error_return(info, '(');
        }
        else if (i > 0 && check_ln[i] == '{' && check_ln[i - 1] == '$') {
            if ((pos = mx_char_block(check_ln + i + 1, '\\', '}', '\0')) == -1
                || !(i += pos + 1))
                return error_return(info, '}');
        }
        else if (check_ln[i] == ')' && (i == 0 || check_ln[i - 1] != '\\'))
            return error_return(info, check_ln[i]);
    }
    mx_strdel(&check_ln);
    return 0;
}
