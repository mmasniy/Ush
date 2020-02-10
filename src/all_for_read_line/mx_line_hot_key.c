#include "../../inc/ush.h"

static void up_lines(t_info *info, char *str, unsigned int add);
static void print_posible_history(t_info *info, t_history *result);
static t_history *search_for_ctrl_r(t_info *info, char *search_line);
static void ctrl_r(t_info *info, char **buffer, int *position);

bool mx_line_hot_key(t_info *info, char **buffer, int *position, char *c) {
    if (c[0] == TAB && *position > 0 && !mx_isspace((*buffer)[*position - 1]))
        mx_tab_work(info, buffer, position);
    else if (c[0] == CTRL_D || c[0] == CTRL_C || c[0] == 13 || c[0] == '\n') {
        write(1, "\n\r", 2);
        if (c[0] == CTRL_D)
            info->ctrl_d = 1;
        else if (c[0] == CTRL_C)
            info->ctrl_c = 1;
        return 0;
    }
    else if (c[0] == CTRL_R) {
        mx_print_char_loop('\b', strlen(*buffer));
        mx_print_char_loop(' ', strlen(*buffer));
        mx_print_char_loop('\b', strlen(*buffer));
        ctrl_r(info, buffer, position);
    }
    return 1;
}

static void up_lines(t_info *info, char *str, unsigned int add) {
    unsigned int lines;

    mx_winsize(info);
    lines = (strlen(str) + add) / (info->winsize);
    for (unsigned int i = 0; i < lines; i++) {
        mx_printchar(27);
        write(1,"[1A", 3);
    }
}

static void print_posible_history(t_info *info, t_history *result) {
    write(1, "\n\r", 2);
    mx_printstr("\033[1;34m");
    mx_printstr("Found: ");
    mx_printstr(RESET);
    if (result) {
        mx_printstr(result->data);
        up_lines(info, result->data, 5);
    }
    mx_printchar(27);
    write(1,"[1A", 3);
}

static t_history *search_for_ctrl_r(t_info *info, char *search_line) {
    for (t_history *tmp = info->history_pack->history->next;
        tmp; tmp = tmp->next) {
        int index;
        if ((index = mx_get_substr_index(tmp->data, search_line)) >= 0)
            return tmp;
    }
    return NULL;
}

static void ctrl_r(t_info *info, char **buffer, int *position) {
    if (info->history_pack->history) {
        char symbol[4];
        int pos_in_line = 0;
        char *search_line = mx_strnew(256);
        t_history *result = NULL;

        print_posible_history(info, result);
        mx_print_ush(info, USH);
        while (1) {
            read(STDIN_FILENO, symbol, 4);
            if (symbol[0] == ENTER || symbol[0] == ESC ||
                (symbol[0] == 27 && symbol[2] >= 65 && symbol[2] <= 68)) { ///// Exit
                up_lines(info, search_line, info->name_len);
                write(1, "\r", 1);
                mx_print_char_loop(' ', strlen(search_line) + info->name_len + 2);
                write(1, "\n\r", 2);
                mx_print_char_loop(' ', 7);
                if (result) {
                    mx_print_char_loop(' ', strlen(result->data));
                    up_lines(info, result->data, 5);
                }
                mx_printchar(27);
                write(1,"[1A", 3);
                up_lines(info, search_line, info->name_len);
                write(1, "\r", 1);
                break;
            }
            if (strlen(search_line) > 255 && symbol[0] != 127) ///// To big string
                continue;
            if (symbol[0] >= 32 && symbol[0] <= 127) { ////// User enter symbol
                write(1, "\n\r", 2);
                mx_print_char_loop(' ', 7);
                if (result) {
                    mx_print_char_loop(' ', strlen(result->data));
                    up_lines(info, result->data, 5);
                }
                mx_printchar(27);
                write(1,"[1A", 3);
                mx_str_edit(info, search_line, &pos_in_line, symbol);
                mx_print_line(info, search_line, pos_in_line);
                result = search_for_ctrl_r(info, search_line);
                print_posible_history(info, result);
                mx_print_line(info, search_line, pos_in_line);
            }
        } ///// Loop Exit
        if (result) ///// If found something in history
            mx_change_hry(info, position, buffer, result);
        if ((symbol[0] == 27 && symbol[2] >= 65 && symbol[2] <= 68))///// If user entered arrow key
            mx_arrows_exec(info, buffer, position, symbol[2]);
        free(search_line);
    }
}
