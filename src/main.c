#include "ush.h"

void run_shell(t_info *info) {
	char *line;
	int status = 1;

	while (status) {
		printf("%su$h> %s", GRN, RESET);
		line = ush_read_line();
		info->args = ush_split_line(line);
		status = ush_execute(info);

		free(line);
		free(info->args);
	}
}

int main(int argc, char **argv, char **environ) {
	if (argc && argv) {}
	char *builtin_str[] = {"pwd", "cd", "help", "exit", NULL};
	int (*builtin_func[]) (t_info *) = {&ush_pwd, &ush_cd, &ush_help, &ush_exit};
	t_info *info = (t_info *)malloc(sizeof(t_info));

	info->builtin_str = builtin_str;
	info->builtin_func = builtin_func;
	mx_info_start(info, environ);


	run_shell(info);
	return 0;
}
