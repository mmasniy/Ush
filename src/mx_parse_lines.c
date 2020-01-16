#include "ush.h"

static void str_edit(t_info *info, char *buffer, int *position, char *c);
static int str_filter(t_info *info, char **str, int *position, char *c);
static void arrows_exec(t_info *info, char **buffer, char *c, int *position);
static void print_line(t_info *info, char *buffer, int position);

// static int custom_str_len(char *str) {

// }

char *mx_ush_read_line(t_info *info) {
	int bufsize = USH_RL_BUFSIZE;
	int position = 0;
	char *buffer = mx_strnew(bufsize);
	char c[3];

	if (!buffer) {
		fprintf(stderr, "Ush: allocation error\n");
		exit(EXIT_FAILURE);
	}
	mx_push_history_front(&info->history_pack->history, buffer);
	info->history_pack->pos = info->history_pack->history;
	mx_print_ush();
	while (1) {
		read(STDIN_FILENO, c, 3);
		if (c[0] == ENTER || c[0] == '\n') {
			// print_line(info, buffer, mx_strlen(buffer));
			write(1, "\n\r", 2);
			return buffer;
		}
		else {
			if (str_filter(info, &buffer, &position, c) == CTRL_C) {
				write(1, "\n\r", 2);
				info->ctrl_c = 1;
				return buffer;
			}
			print_line(info, buffer, position);
		}
		if ((size_t)position + 1 >= malloc_size(buffer)
			|| malloc_size(buffer) <= (size_t)mx_strlen(buffer) + 1) {
			bufsize += USH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "Ush: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

static void str_edit(t_info *info, char *buffer, int *position, char *c) {
	int len = mx_strlen(buffer);
	if (c[0] == BACKSPACE && buffer[0]) {
		if (*position > 0) {
			for (int i = *position; i < len; i++)
				buffer[i - 1] = buffer[i];
			buffer[len - 1] = '\0';
			print_line(info, buffer, mx_strlen(buffer));
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

static int str_filter(t_info *info, char **buffer, int *position, char *c) {
	if (c[0] == 27) {
		if (c[2] >= 65 && c[2] <= 68)
			arrows_exec(info, buffer, c, position);
	}
	else if (c[0] >= 32 && c[0] <= 127) {
		str_edit(info, *buffer, position, c);
	}
	else {
		if (c[0] == CTRL_D)
			info->ctrl_d = 1;
		else if (c[0] == CTRL_C) {
			info->ctrl_c = 1;
			return CTRL_C;
		}
		return c[0];
	}
	return c[0] >= 32 && c[0] <= 126 ? 0 : c[0];
}

static void arrows_exec(t_info *info, char **buffer, char *c, int *position) {
	mx_print_char_loop('\b', mx_strlen(*buffer));
	mx_print_char_loop(' ', mx_strlen(*buffer) + 1);
	if (c[2] == 65) {
		if (info->history_pack->pos->next) {
			free(info->history_pack->pos->data);
			info->history_pack->pos->data = mx_strdup(*buffer);
			info->history_pack->pos = info->history_pack->pos->next;
			mx_print_char_loop('\b', mx_strlen(*buffer));
			mx_print_char_loop(' ', mx_strlen(*buffer));
			free(*buffer);
			*buffer = mx_strdup(info->history_pack->pos->data);
			*position = mx_strlen(*buffer);
		}
	}
	else if (c[2] == 66) {
		if (info->history_pack->pos->prev) {
			free(info->history_pack->pos->data);
			info->history_pack->pos->data = mx_strdup(*buffer);
			info->history_pack->pos = info->history_pack->pos->prev;
			mx_print_char_loop('\b', mx_strlen(*buffer));
			mx_print_char_loop(' ', mx_strlen(*buffer));
			free(*buffer);
			*buffer = mx_strdup(info->history_pack->pos->data);
			*position = mx_strlen(*buffer);
		}
	}
	else if (c[2] == 67) {
		if (*position < mx_strlen(*buffer))
			(*position)++;
	}
	else if (c[2] == 68) {
		if (*position > 0)
			(*position)--;
	}
	print_line(info, *buffer, *position);
}

static void print_line(t_info *info, char *buffer, int position) {
	int lines;

	mx_winsize(info);
	lines = (mx_strlen(buffer) + 5) / (info->winsize);
	for (int i = 0; i < lines; i++) {
		mx_printchar(27);
		write(1,"[1A", 3);
	}
	mx_print_char_loop('\b', mx_strlen(buffer) - position - lines * info->winsize);
	mx_print_ush();
	mx_printstr(buffer);
	mx_print_char_loop('\b', mx_strlen(buffer) - position);
}
