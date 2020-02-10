#include "../../inc/ush.h"

int mx_getchar() {
	unsigned int ch = 0;

	if (read(1, &ch, 4) == 0)
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
    int lines;

    mx_winsize(info);
    lines = (strlen(buffer) + info->name_len) / (info->winsize);
    for (int i = 0; i < lines; i++) {
        mx_printchar(27);
        write(1,"[1A", 3);
    }
    mx_print_char_loop('\b', mx_strlen(buffer) - lines * info->winsize);
    mx_print_ush(info, USH);
    mx_printstr(buffer);
    mx_print_char_loop('\b', mx_strlen(buffer) - position);
}
