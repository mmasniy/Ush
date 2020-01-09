#include "ush.h"

static void mx_init_shell(char **environ, t_info *info);

void run_shell(t_info *info) {
	char *line;
	int status = 1;

	while (status) {
		mx_print_ush();
		mx_custom_termios(info, STDIN_FILENO);
		line = ush_read_line(info);
		mx_origin_termios(info, STDIN_FILENO);
		info->args = ush_split_line(line);
		status = ush_execute(info);
		free(line);
		free(info->args);
		info->ctrl_d = 0;
		info->ctrl_c = 0;
	}
}

int main(int argc, char **argv, char **environ) {
	char *builtin_str[] = {"pwd", "cd", "help", "exit", NULL};
	int (*builtin_func[]) (t_info *) = {&ush_pwd
		, &ush_cd, &ush_help, &ush_exit};
	t_info *info = (t_info *)malloc(sizeof(t_info));

	(void)argc;
	(void)argv;
	signal(SIGINT, mx_sigio_handler);
	signal(SIGIO, mx_sigio_handler);
	if (!isatty(STDIN_FILENO)) {
		mx_error_message("You must use it just in terminal");
		exit(EXIT_FAILURE);
	}
	memset(info, 0, sizeof(t_info));
	info->builtin_str = builtin_str;
	info->builtin_func = builtin_func;
	mx_init_shell(environ, info);
	run_shell(info);
	return 0;
}

static void mx_init_shell(char **environ, t_info *info) {
	for (int i = 0; environ[i]; i++) {
		if (mx_str_head(environ[i], "TERM=")) {
			mx_info_start(info, environ);
			return;
		}
	}
	mx_error_message(TERM_ENV_NOT_EXIST);
	exit(EXIT_FAILURE);
}
