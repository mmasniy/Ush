#include "../../inc/ush.h"

static void mx_error_jobs(char **argv) {
	mx_printerr("jobs: job not found: ");
	mx_printerr(argv[1]);
	mx_printerr("\n");
}

int mx_jobs(t_info *i) {
	t_process *tmp = NULL;
	
	if (i->process && mx_arr_size(i->args) == 1) {
		tmp = i->process;
		while (tmp) {
			if (tmp->value < -1)
				printf("[%d]    suspended  %s\n", tmp->pos, tmp->cmd);
			else if (tmp->value == 1)
				printf("[%d]  + suspended  %s\n", tmp->pos, tmp->cmd);
			else if (tmp->value == -1)
				printf("[%d]  - suspended  %s\n", tmp->pos, tmp->cmd);
			tmp = tmp->next;
		}
	}
	return 0;
}
