#include "../../inc/ush.h"

static int check_command(char **cmd) {
	if (!mx_arr_size(cmd))
		return 1;
	if (mx_arr_size(cmd) > 1) {
		fprintf(stderr, "fg: too many arguments\n");
		return 0;
	}
	if (cmd[0][0] != '%' || !cmd[0][1]) {
		fprintf(stderr, "fg: invalid argument: %s\n", cmd[0]);
		return 0;
	}
	return 1;
}

void mx_wait_process(t_info *i, int status, pid_t child) {
	if (MX_WIFSIG(status)) {
		if (MX_WTERMSIG(status) == SIGINT) {
			mx_del_procces_by_pid(&(i->process), child);
			i->status = 130;
		}
		else
			mx_print_added_new_node(i->process, child);
	}
}

int mx_fg(t_info *i) {
	int status;
	pid_t child;
	printf("1\n");

	if (!check_command(&(i->args[1])))
		return 1;
	printf("2\n");
	if (i->process) {
			printf("3\n");

		if (mx_continue_process(i, i->args, 1) == 0) {
				printf("4\n");

			child = waitpid(-1, &status, WUNTRACED);
				printf("5\n");

			if (!MX_WIFEXIT(status)) {
					printf("6\n");
				mx_wait_process(i, status, child);
			}
			else {
					printf("7\n");

				mx_del_procces_by_pid(&(i->process), child);
				i->status = MX_WEXITSTATUS(status);
			}
				printf("8\n");

		}
			printf("9\n");

	}
	else {
			printf("10\n");

    	fprintf(stderr, "%s", "fg: no current jobs\n");
        return 1;
    }
    	printf("11\n");

    return 0;
}