#include "../../inc/ush.h"

static void home_end_page(t_info *info, char **buf, int *position, char *c) {
    if (c[2] == 72)
        *position = 0;
    else if (c[2] == 70)
        *position = mx_strlen(*buf);
    else if (c[2] == 53 && info->history_pack->pos)
        while (info->history_pack->pos->next)
            mx_arrows_exec(info, buf, position, 65);
    else if (c[2] == 54 && info->history_pack->pos)
        while (info->history_pack->pos->prev)
            mx_arrows_exec(info, buf, position, 66);
}

static int input_work(t_info *info, char **buffer, int *pos, unsigned int ch) {
    bool result = 1;
    char *c = (char*)(&ch);

    if (ch > 127) {
        if (c[0] == 27) {
            if (c[2] >= 65 && c[2] <= 68)
                mx_arrows_exec(info, buffer, pos, c[2]);
            else if (c[2] == 72 || c[2] == 70 || c[2] == 53 || c[2] == 54)
                home_end_page(info, buffer, pos, c);
        }
    }
    else if (c[0] >= 32 && c[0] <= 127) {
        mx_str_edit(info, *buffer, pos, c);
    }
    else
        result = mx_line_hot_key(info, buffer, pos, c);
    return result;
}

static void print_and_realloc_line(t_info *info, char **buffer, int *bufsize,
                                   int position) {
    mx_print_line(info, *buffer, position);
    if ((size_t)position + 1 >= malloc_size(*buffer)
        || malloc_size(*buffer) <= (size_t)mx_strlen(*buffer) + 1)
        *buffer = realloc(*buffer, (*bufsize += MX_USH_RL_BUFSIZE));
}

char *mx_ush_read_line(t_info *info) {
    int bufsize = MX_USH_RL_BUFSIZE;
    int position = 0;
    char *buffer = mx_strnew(bufsize);
    unsigned int ch = 0;
    int fds[2];

    fds[0] = dup(1);
    fds[1] = open("/dev/tty", O_WRONLY);
    dup2(fds[1], 1);
    mx_push_history_front(&info->history_pack->history, buffer);
    info->history_pack->pos = info->history_pack->history;
    mx_print_ush(info);
    while (1) {
        ch = mx_getchar();
        if (!input_work(info, &buffer, &position, ch) && dup2(fds[0], 1) >= 0
            && close(fds[0]) >= 0 && close(fds[1]) >= 0)
            return buffer;
        print_and_realloc_line(info, &buffer, &bufsize, position);
    }
}
