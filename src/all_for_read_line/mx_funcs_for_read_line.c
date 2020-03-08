#include "../../inc/ush.h"

static void print_clean(char s) {
    char *symbol = &s;

    write(STDOUT_FILENO, "\b", 1);
    write(1, symbol, 1);
    write(1, "[J", 2);
}

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
            (*position)--;
            print_clean(27);
        }
    }
    else if (c[0] >= 32 && c[0] <= 126) {
        if (buffer[0])
            for (int i = len; i > *position; i--)
                buffer[i] = buffer[i - 1];
        buffer[*position] = c[0];
        buffer[len + 1] = '\0';
        (*position)++;
        write(STDOUT_FILENO, " ", 1);
    }
}

void mx_print_line(t_info *info, char *buffer, int position) {
    mx_print_char_loop('\b', strlen(buffer) - 1);
    print_clean(27);
    mx_print_ush(info);
    mx_printstr(buffer);
    mx_print_char_loop('\b', mx_strlen(buffer) - position);
}
