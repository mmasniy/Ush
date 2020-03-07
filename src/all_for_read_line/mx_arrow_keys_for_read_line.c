#include "../../inc/ush.h"

static void key_up(t_info *i, char **buffer, int *position) {
    if (i->history_pack->pos->next) {
        int pos = *position - 1;
        bool exist = 0;
        int len = 1;
        char *what_check = NULL;

        if (pos >= 0 && mx_strcmp(*buffer, i->history_pack->pos->data)) {
            for (; pos > 0 && !mx_isspace((*buffer)[pos - 1]); pos--, len++);
            what_check = strndup(&(*buffer)[pos], len);
            for (t_history *search = i->history_pack->pos->next;
                search; search = search->next)
                if (mx_str_head(search->data, what_check) == 0) {
                    exist = 1;
                    mx_change_hry(i, position, buffer, search);
                    break;
                }
        }
        if (!exist)
            mx_change_hry(i, position, buffer, i->history_pack->pos->next);
    }
}

static void key_down(t_info *i, char **buffer, int *position) {
    if (i->history_pack->pos->prev) {
        int pos = *position - 1;
        bool exist = 0;
        int len = 1;
        char *what_check = NULL;

        if (pos >= 0 && mx_strcmp(*buffer, i->history_pack->pos->data)) {
            for (; pos > 0 && !mx_isspace((*buffer)[pos - 1]); pos--, len++);
            what_check = strndup(&(*buffer)[pos], len);
            for (t_history *search = i->history_pack->pos->prev;
                 search; search = search->prev)
                if (mx_str_head(search->data, what_check) == 0) {
                    exist = 1;
                    mx_change_hry(i, position, buffer, search);
                    break;
                }
        }
        if (!exist)
            mx_change_hry(i, position, buffer, i->history_pack->pos->prev);
    }
}

void mx_arrows_exec(t_info *info, char **buffer, int *position, char c) {
    if (c == 65) { // UP
        key_up(info, buffer, position);
    }
    else if (c == 66) { // DOWN
        key_down(info, buffer, position);
    }
    else if (c == 67) {
        if (*position < mx_strlen(*buffer))
            (*position)++;
    }
    else if (c == 68) {
        if (*position > 0)
            (*position)--;
    }
}

void mx_change_hry(t_info *info, int *pos, char **buffer, t_history *link) {
    free(info->history_pack->pos->data);
    info->history_pack->pos->data = mx_strdup(*buffer);
    info->history_pack->pos = link;
    mx_print_char_loop('\b', mx_strlen(*buffer));
    mx_print_char_loop(' ', mx_strlen(*buffer));
    mx_print_char_loop('\b', mx_strlen(*buffer));
    free(*buffer);
    *buffer = mx_strdup(info->history_pack->pos->data);
    *pos = mx_strlen(*buffer);
    mx_print_ush(info);
    mx_printstr(*buffer);
}
