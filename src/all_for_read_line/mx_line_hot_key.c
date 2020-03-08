#include "../../inc/ush.h"

static void ctrl_d(t_info *info, char **buffer, int *position, char *c) {
    c[0] = BACKSPACE;
    if (strlen(*buffer) > *position + 1) {
        (*position) += 2;
        mx_str_edit(info, *buffer, position, c);
        (*position)--;
        mx_print_line(info, *buffer, *position);
    }
}

bool mx_line_hot_key(t_info *info, char **buffer, int *position, char *c) {
    if (c[0] == TAB && *position > 0 && !mx_isspace((*buffer)[*position - 1]))
        mx_tab_work(info, buffer, position);
    else if (c[0] == CTRL_D && strlen(*buffer))
        ctrl_d(info, buffer, position, c);
    else if (c[0] == CTRL_D || c[0] == CTRL_C || c[0] == 13 || c[0] == '\n') {
        write(1, "\n\r", 2);
        if (c[0] == CTRL_D)
            info->ctrl_d = 1;
        else if (c[0] == CTRL_C)
            mx_ctrl_c(info, buffer, position);
        return 0;
    }
    else if (c[0] == CTRL_R) {
        mx_print_char_loop('\b', strlen(*buffer));
        mx_print_char_loop(' ', strlen(*buffer));
        mx_print_char_loop('\b', strlen(*buffer));
        mx_ctrl_r(info, buffer, position);
    }
    return 1;
}

void mx_up_lines(t_info *info, char *str, unsigned int add) {
    unsigned int lines;

    mx_winsize(info);
    lines = (strlen(str) + add) / (info->winsize);
    for (unsigned int i = 0; i < lines; i++) {
        mx_printchar(27);
        write(1,"[1A", 3);
    }
}

void mx_print_posible_history(t_info *info, t_history **result) {
    write(1, "\n\r", 2);
    mx_printstr("\033[1;34m");
    mx_printstr("Found: ");
    mx_printstr(RESET);
    if (result && *result) {
        mx_printstr((*result)->data);
        mx_up_lines(info, (*result)->data, 5);
    }
    mx_printchar(27);
    write(1,"[1A", 3);
    write(1, "\r", 1);
}

t_history *mx_search_for_ctrl_r(t_info *info, char *search_line) {
    for (t_history *tmp = info->history_pack->history->next;
         tmp; tmp = tmp->next) {
        int index;

        if ((index = mx_get_substr_index(tmp->data, search_line)) >= 0)
            return tmp;
    }
    return NULL;
}
