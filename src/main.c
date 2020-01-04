#include "ush.h"

void run_shell() {
	char *line;
	char **args;
	int status = 1;

	while (status) {
		printf("%su$h> %s", GRN, RESET);
		line = ush_read_line();
		args = ush_split_line(line);
		status = ush_execute(args);

		free(line);
		free(args);
	}
}

int main() {
	run_shell();
	return 0;
}
