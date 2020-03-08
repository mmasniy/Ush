#include "../../inc/ush.h"

static char *exit_cycle(t_info *info, char **search_line, t_history **result,
                        char *symbol) {
    mx_up_lines(info, *search_line, info->name_len);
    write(1, "\r", 1);
    mx_print_char_loop(' ', strlen(*search_line) + info->name_len + 2);
    write(1, "\n\r", 2);
    mx_print_char_loop(' ', 7);
    if (*result) {
        mx_print_char_loop(' ', strlen((*result)->data));
        mx_up_lines(info, (*result)->data, 5);
    }
    mx_printchar(27);
    write(1,"[1A", 3);
    mx_up_lines(info, *search_line, info->name_len);
    write(1, "\r", 1);
    return symbol;
}

static void print_cycle(t_info *info, t_history **result,
                        char **search_line) {
    char escape = 27;

    write(1, "\n\r", 2);
    mx_print_char_loop(' ', 7);
    if (*result) {
        mx_print_char_loop(' ', strlen((*result)->data));
        mx_up_lines(info, (*result)->data, 5);
    }
    mx_printchar(27);
    write(1,"[1A", 3);
    write(1, &escape, 1);
    write(1, "[J", 2);
}

static char *cycle_for_ctrl_r(t_info *info, char *search_line,
                              t_history **result) {
    int pos = 0;
    char symbol[4];

    while (1) {
        read(STDIN_FILENO, symbol, 4);
        if (symbol[0] == ENTER || symbol[0] == ESC ||
            (symbol[0] == 27 && symbol[2] >= 65 && symbol[2] <= 68)) { ///// Exit
            return exit_cycle(info, &search_line, result, symbol);
        }
        if (strlen(search_line) > 255 && symbol[0] != 127) ///// To big string
            continue;
        if (symbol[0] >= 32 && symbol[0] <= 127) { ////// User enter symbol
            print_cycle(info, result, &search_line);
            mx_str_edit(info, search_line, &pos, symbol);
            mx_print_line(info, search_line, pos);
            *result = mx_search_for_ctrl_r(info, search_line);
            mx_print_posible_history(info, result);
            mx_print_line(info, search_line, pos);
        }
    }
}

void mx_ctrl_r(t_info *info, char **buffer, int *position) {
    if (info->history_pack->history) {
        char *symbol;
        char *search_line = mx_strnew(256);
        t_history *result = NULL;

        mx_print_posible_history(info, &result);
        mx_print_ush(info);
        symbol = cycle_for_ctrl_r(info, search_line, &result);
        if (result) // If found something in history
            mx_change_hry(info, position, buffer, result);
        if ((symbol[0] == 27 && symbol[2] >= 65 && symbol[2] <= 68))
            mx_arrows_exec(info, buffer, position, symbol[2]);
        free(search_line);
    }
}

void mx_ctrl_c(t_info *info, char **buffer, int *position) {
    info->status = 130;
    *position = 0;
    info->ctrl_c = 1;
    mx_strdel(buffer);
}
