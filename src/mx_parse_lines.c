#include "ush.h"

static int str_filter(char *str, int pos, char *c);

static void print_line(t_info *info, char *buffer) {
	if (info) { }
	// printf("info->winsize = %d\n", info->winsize);
	// if (info->winsize > mx_strlen(buffer) + 5) {
		write(STDOUT_FILENO, buffer, mx_strlen(buffer));
	// }
	// else {
		// write(STDOUT_FILENO, buffer, mx_strlen(buffer));
		// int position = (mx_strlen(buffer) + 5) / info->winsize * info->winsize;

		// for (int i = (mx_strlen(buffer) + 5) / info->winsize; i > 0; i--) {
			// write(STDOUT_FILENO, &buffer[position]
			// 	, info->winsize < mx_strlen(&buffer[position]) 
			// 	? info->winsize : mx_strlen(&buffer[position]));
			// write(STDOUT_FILENO, "\n", 1);
			// position += info->winsize;
		// }
	// 	write(STDOUT_FILENO, &buffer[info->winsize], mx_strlen(&buffer[info->winsize]));
	// }
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
			if (str_filter(buffer, position, c)) {
				if (c[0] == CTRL_D)
					info->ctrl_d = 1;
				else if (c[0] == CTRL_C) {
					info->ctrl_c = 1;
					return buffer;
				}
				else if (c[0] == BACKSPACE) {
					mx_print_ush();
					print_line(info, buffer);
					if (position > 0) {
						position--;
						write(STDOUT_FILENO, " ", 1);
						mx_print_ush();
						print_line(info, buffer);
					}
				}
				else {
					mx_print_ush();
					print_line(info, buffer);
				}
			}
			else {
				position++;
				mx_print_ush();
				print_line(info, buffer);
			}
		}
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

static int str_filter(char *str, int pos, char *c) {
	if (c[0] == 27) {
		if (c[2] == 65) {
			printf("UP\n");
		}
		if (c[2] == 66) {
			printf("DOWN\n");
		}
		if (c[2] == 67) {
			printf("RIGHT\n");
		}
		if (c[2] == 68) {
			printf("LEFT\n");
		}
		return 1;
	}
	else if (c[0] >= 32 && c[0] <= 126)
		str[pos] = c[0];
	else {
		if (c[0] == BACKSPACE && str[0])
			str[pos - 1] = '\0';
		// else if (mx_strlen(str) == 0 && (c >= 1 && c <= 31))
			// return c;
		return c[0];
	}
	return 0;
}
