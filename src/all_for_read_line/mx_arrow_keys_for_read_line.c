#include "../../inc/ush.h"

static void key_up(t_info *info, char **buffer, int *position);
static void key_down(t_info *info, char **buffer, int *position);

void mx_arrows_exec(t_info *info, char **buffer, int *position, char c) {
    mx_print_char_loop('\b', mx_strlen(*buffer));
    mx_print_char_loop(' ', mx_strlen(*buffer) + 1);
    if (c == 65) { // UP
        key_up(info, buffer, position);
    }
    else if (c == 66) { // DOWN
        key_down(info, buffer, position);
    }
    else if (c == 67) {
        if (*position < mx_strlen(*buffer))
            (*position)++;
        // mx_print_line(info, *buffer, *position);
    }
    else if (c == 68) {
        if (*position > 0)
            (*position)--;
        // mx_print_line(info, *buffer, *position);
    }
}

void mx_change_hry(t_info *info, int *pos, char **buffer,
    t_history *link) {
    free(info->history_pack->pos->data);
    info->history_pack->pos->data = mx_strdup(*buffer);
    info->history_pack->pos = link;
    mx_print_char_loop('\b', mx_strlen(*buffer));
    mx_print_char_loop(' ', mx_strlen(*buffer));
    mx_print_char_loop('\b', mx_strlen(*buffer));
    free(*buffer);
    *buffer = mx_strdup(info->history_pack->pos->data);
    *pos = mx_strlen(*buffer);
    mx_print_ush(info, USH);
    mx_printstr(*buffer);
}

static void key_up(t_info *info, char **buffer, int *position) {
    if (info->history_pack->pos->next) {
        int pos = *position - 1;
        bool exist = 0;

        if (pos >= 0 && mx_strcmp(*buffer, info->history_pack->pos->data)) {
            int len = 1;
            char *what_check = NULL;

            for (; pos > 0 && !mx_isspace((*buffer)[pos - 1]); pos--, len++);
            what_check = strndup(&(*buffer)[pos], len);
            for (t_history *search = info->history_pack->pos->next;
                search; search = search->next)
                if (mx_str_head(search->data, what_check) == 0) {
                    exist = 1;
                    mx_change_hry(info, position, buffer, search);
                    break;
                }
        }
        if (!exist)
            mx_change_hry(info, position, buffer, info->history_pack->pos->next);
    }
}

static void key_down(t_info *info, char **buffer, int *position) {
    if (info->history_pack->pos->prev) {
        int pos = *position - 1;
        bool exist = 0;

        if (pos >= 0 && mx_strcmp(*buffer, info->history_pack->pos->data)) {
            int len = 1;
            char *what_check = NULL;

            for (; pos > 0 && !mx_isspace((*buffer)[pos - 1]); pos--, len++);
            what_check = strndup(&(*buffer)[pos], len);

            for (t_history *search = info->history_pack->pos->prev;
                search; search = search->prev) {
                if (mx_str_head(search->data, what_check) == 0) {
                    exist = 1;
                    mx_change_hry(info, position, buffer, search);
                    break;
                }
            }
        }
        if (!exist)
            mx_change_hry(info, position, buffer, info->history_pack->pos->prev);
    }
}
