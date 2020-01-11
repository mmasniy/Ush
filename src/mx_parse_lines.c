#include "ush.h"

static int str_filter(t_info *info, char *str, int *position, char *c);

char **ush_split_line(char *line) {
	int bufsize = USH_TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;
	
	if (!tokens) {
		fprintf(stderr, "Ush: allocation error\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, USH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += USH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "Ush: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, USH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

static void extra_sym_execute(t_info *info, char *buffer, char *c, int *position) {
	// if (type[2] == 65) {
	// 	// up, down, right, left
	// }
	if (info) {}
	mx_print_ush();
	mx_printstr(buffer);
	if (c[2] == 67) {
		if (*position < mx_strlen(buffer))
			(*position)++;
	}
	else if (c[2] == 68) {
		if (*position > 0)
			(*position)--;
	}
}

char *ush_read_line(t_info *info) {
	int bufsize = USH_RL_BUFSIZE;
	int position = 0;
	char *buffer = mx_strnew(bufsize);
	char c[3];

	if (!buffer) {
		fprintf(stderr, "Ush: allocation error\n");
		exit(EXIT_FAILURE);
	}
	mx_print_ush();
	while (1) {
		read(STDIN_FILENO, &c, 3);
		mx_winsize(info);
		if (c[0] == ENTER || c[0] == '\n') {
			write(1, "\n\r", 2);
			buffer[position] = '\0';
			return buffer;
		} 
		else {
			if (str_filter(info, buffer, &position, c) == CTRL_C) {
				write(1, "\n\r", 2);
				buffer[position] = '\0';
				return buffer;
			}
			mx_print_ush();
			mx_printstr(buffer);
		}
		mx_print_char_loop('\b', mx_strlen(buffer) - position);
		if (position >= bufsize) {
			bufsize += USH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "Ush: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

static void str_edit(char *buffer, int *position, char *c) {
	int len = mx_strlen(buffer);
	if (c[0] == BACKSPACE && buffer[0]) {
		if (*position > 0) {
			for (int i = *position; i < len; i++)
				buffer[i - 1] = buffer[i];
			buffer[len - 1] = '\0';
			mx_print_ush();
			mx_printstr(buffer);
			write(STDOUT_FILENO, " ", 1);
			(*position)--;
		}
	}
	else if (c[0] >= 32 && c[0] <= 126) {
		if (buffer[0])
			for (int i = len; i >= *position; i--)
				buffer[i] = buffer[i - 1];
		buffer[*position] = c[0];
		(*position)++;
	}
}

static int str_filter(t_info *info, char *buffer, int *position, char *c) {
	if (c[0] == 27) {
		if (c[2] >= 65 && c[2] <= 68)
			extra_sym_execute(info, buffer, c, position);
	}
	else if (c[0] >= 32 && c[0] <= 127)
		str_edit(buffer, position, c);
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
