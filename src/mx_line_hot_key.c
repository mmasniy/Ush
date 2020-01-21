#include "ush.h"

static void print_posible_history(t_history *result) {
    // printf("strlen(buffer) = %lu\n", strlen(buffer));
    // printf("++++++++++++\n");
    // char *data_without_head = strdup(&(result->data)[strlen(buffer)]);
    printf("\n\r%sFound:%s %s\r\n", MAG, RESET, result->data);
    // mx_printstr(YEL);
    // mx_printstr(result->data);
    // mx_printstr(RESET);
    mx_printchar(27);
    write(1,"[1A", 3);
    mx_printchar(27);
    write(1,"[1A", 3);
    // printf("------------\n");
}

static t_history *search_for_ctrl_r(t_info *info, char *search_line) {
    for (t_history *tmp = info->history_pack->history->next; tmp; tmp = tmp->next) {
        if (mx_get_substr_index(tmp->data, search_line) >= 0)
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

        printf("\n\r%sFound:%s\n\r", MAG, RESET);
        mx_printchar(27);
        write(1,"[1A", 3);
        mx_printchar(27);
        write(1,"[1A", 3);
        mx_print_ush();
        while (1) {
            read(STDIN_FILENO, symbol, 4);
            if (symbol[0] == ENTER || symbol[0] == ESC ||
                (symbol[0] == 27 && symbol[2] >= 65 && symbol[2] <= 68)) {
                mx_print_char_loop('\b', strlen(search_line));
                mx_print_char_loop(' ', strlen(search_line));
                write(1, "\n", 1);
                write(1, "\r", 1);
                if (result)
                    mx_print_char_loop(' ', strlen(result->data) + 7);
                else
                    mx_print_char_loop(' ', 7);
                mx_printchar(27);
                write(1,"[1A", 3);
                break;
            }
            if (mx_strlen(search_line) > 255)
                continue;
            if (symbol[0] >= 32 && symbol[0] <= 127) {
                mx_print_char_loop('\b', strlen(search_line));
                mx_print_char_loop(' ', strlen(search_line));
                if (result) {
                    printf("\n\r%sFound:%s ", MAG, RESET);
                    mx_print_char_loop(' ', strlen(result->data));
                    mx_printchar(27);
                    write(1,"[1A", 3);
                }
                mx_str_edit(info, search_line, &pos_in_line, symbol);
                if ((result = search_for_ctrl_r(info, search_line))) {
                    print_posible_history(result);
                }
                mx_print_line(info, search_line, pos_in_line);
            }
        }
        if (result) {
            mx_change_hry(info, position, buffer, result);
        }
        if ((symbol[0] == 27 && symbol[2] >= 65 && symbol[2] <= 68)) {
            mx_arrows_exec(info, buffer, position, symbol[2]);
        }
        free(search_line);
    }
}

void mx_line_hot_key(t_info *info, char **buffer, int *position, char *c) {
    if (c[0] == TAB && *position > 0 && !mx_isspace((*buffer)[*position - 1]))
        mx_tab_work(info, buffer, position);
    else if (c[0] == CTRL_D)
        info->ctrl_d = 1;
    else if (c[0] == CTRL_C)
        info->ctrl_c = 1;
    else if (c[0] == CTRL_R) {
        mx_print_char_loop('\b', strlen(*buffer));
        mx_print_char_loop(' ', strlen(*buffer));
        mx_print_char_loop('\b', strlen(*buffer));
        ctrl_r(info, buffer, position);
    }
}
