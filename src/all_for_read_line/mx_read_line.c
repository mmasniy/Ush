#include "../../inc/ush.h"

static void home_end_page(t_info *info, char **buf, int *position, char *c);
static int input_work(t_info *info, char **buffer, int *pos, unsigned int ch);
static void save_all(t_info *info, char *buffer, int *position, char *c);

char *mx_ush_read_line(t_info *info) {
    int bufsize = USH_RL_BUFSIZE;
    int position = 0;
    char *buffer = mx_strnew(bufsize);
    unsigned int ch = 0;

    mx_push_history_front(&info->history_pack->history, buffer);
    info->history_pack->pos = info->history_pack->history;
    mx_print_ush(info);
    mx_custom_termios(info, STDIN_FILENO);
    while (1) {
        ch = mx_getchar();
        if (!input_work(info, &buffer, &position, ch)) {
            mx_origin_termios(info, STDIN_FILENO);
            return buffer;
        }
        mx_print_line(info, buffer, position);
        if ((size_t)position + 1 >= malloc_size(buffer)
            || malloc_size(buffer) <= (size_t)mx_strlen(buffer) + 1)
            buffer = realloc(buffer, (bufsize += USH_RL_BUFSIZE));
    }
}

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
//     printf("ch = %u\n", ch);
// printf("%d %d %d %d\n", c[0], c[1], c[2], c[3]);
    if (ch > 127) {
        if (c[0] == 27) {
            if (c[2] >= 65 && c[2] <= 68)
                mx_arrows_exec(info, buffer, pos, c[2]);
            else if (c[2] == 72 || c[2] == 70 || c[2] == 53 || c[2] == 54)
                home_end_page(info, buffer, pos, c);
        }
        else {
            save_all(info, *buffer, pos, c);
        }
    }
    else if (c[0] >= 32 && c[0] <= 127) {
        mx_str_edit(info, *buffer, pos, c);
    }
    else
        result = mx_line_hot_key(info, buffer, pos, c);
    return result;
}

static void save_all(t_info *info, char *buffer, int *position, char *c) {
    if (info && buffer && position && c) {}
    int len = mx_strlen(buffer);

    for (int i = 0; i < 4; i++) {
        if (buffer[0])
            for (int i = len; i > *position; i--)
                buffer[i] = buffer[i - 1];
        buffer[*position] = c[i];
        buffer[len + 1] = '\0';
        (*position)++;
        len++;
    }
}
// здвигати по 6 і перевіряти чи воно дорівнює == 2
