#include "../../inc/ush.h"

int mx_getchar() {
    unsigned int ch = 0;

    if (read(STDIN_FILENO, &ch, 4) == 0)
        return 0;
    return ch;
}

void mx_str_edit(t_info *info, char *buffer, int *position, char *c) {
    int len = mx_strlen(buffer);

    if (c[0] == BACKSPACE && buffer[0]) {
        if (*position > 0) {
            for (int i = *position; i < len; i++)
                buffer[i - 1] = buffer[i];
            buffer[len - 1] = '\0';
            mx_print_line(info, buffer, mx_strlen(buffer));
            write(STDOUT_FILENO, " ", 1);
            (*position)--;
        }
    }
    else if (c[0] >= 32 && c[0] <= 126) {
        if (buffer[0])
            for (int i = len; i > *position; i--)
                buffer[i] = buffer[i - 1];
        buffer[*position] = c[0];
        buffer[len + 1] = '\0';
        (*position)++;
    }
}

void mx_print_line(t_info *info, char *buffer, int position) {
    if (info && position) {}
    for (unsigned int i = 0; i < strlen(buffer) + info->name_len; i++) {
        mx_printstr("\b\x1b[2K");
    }
    mx_print_ush(info);
    mx_printstr(buffer);
    mx_print_char_loop('\b', mx_strlen(buffer) - position);
}
