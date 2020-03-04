#include "../../inc/ush.h"

static void mx_no_new_line(t_info *info, int line_len) {
    int size = mx_winsize(info) - line_len - info->name_len - 1;
    char escape = 27;

    write(1, "%", 1);
    mx_print_char_loop(' ', size);
    write(1, &escape, 1);
    write(1, "[J", 2);
}

static void smart_print(char *str, int *line_len) {
    mx_search_slash(&str);
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\\' && str[i + 1] && str[i + 1] == 'f') {
            (*line_len)++;
            write(1, "\f", 1);
            i++;
            continue;
        }
        (*line_len)++;
        write(1, &(str[i]), 1);
    }
}

static bool check_flags(char *args, int *flags) {
    if (args[0] == '-') {
        for (int i = 1; args[i]; i++)
            if (args[i] != 'n' && args[i] != 'e' && args[i] != 'E')
                return 0;
        for (int i = 1; args[i]; i++) {
            if ((args[i] == 'n' && (flags[0] = 1))
                || (args[i] == 'e' && (flags[1] = 1))
                || (args[i] == 'E' && (flags[2] = 1)))
                continue;
        }
        return 1;
    }
    else
        return 0;
}

int mx_ush_echo(t_info *info) {
    int flags[3] = {0, 0, 0};
    int pos = 1;
    int line_len = 0;

    for (int i = 1; info->args[i]; i++)
        if (check_flags(info->args[i], flags))
            pos++;
    for (int i = pos; info->args[i]; i++) {
        if (flags[2] && !flags[1]) {
            mx_printstr(info->args[i]);
            line_len += strlen(info->args[i]);
        }
        else
            smart_print(info->args[i], &line_len);
        if (info->args[i + 1])
            write(1, " ", 1);
    }
    flags[0] ? mx_no_new_line(info, line_len) : write(1, "\n", 1);
    return 0;
}
