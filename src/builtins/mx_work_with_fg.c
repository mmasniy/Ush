#include "../../inc/ush.h"

t_process *mx_search_by_char(char *cmd, t_process *processes) {
	t_process *tmp = NULL;

	if (processes) {
		tmp = processes;
		while (tmp) {
			if (mx_strcmp(tmp->cmd, cmd) == 0)
				return tmp;
			tmp = tmp->next;
		}
	}
	fprintf(stderr, "fg: %s: no such job\n", cmd);
	return NULL;
}

t_process *mx_search_by_id(int pos, t_process *processes) {
	t_process *tmp = NULL;

	if (processes) {
		tmp = processes;
		while (tmp && tmp->pos != pos)
			tmp = tmp->next;
		return tmp;
	}
	fprintf(stderr, "fg: %d: no such job\n", pos);
	return NULL;
}

t_process *mx_get_process(t_process *process, char *cmd) {
	bool num_or_char = true;
	unsigned int len = 0;
	t_process *p = process;

	if (!cmd)
		return p;
	cmd++;
	len = mx_strlen(cmd);
	for (unsigned int i = 0; i < len; i++) {
		if (!mx_isdigit(cmd[i])) {
			num_or_char = false;
			break;
		}
	}
	if (num_or_char)
		return mx_search_by_id(atoi(cmd), p);
	return mx_search_by_char(cmd, p);
}

t_process *get_last_process(t_process *p) {
	t_process *tmp = p;

	while (tmp->next)
		tmp = tmp->next;
	return tmp;
}

int mx_continue_process(t_info *i, char **argv, int fd) {
	t_process *p = i->process;

	if (argv[1] == 0) {
		t_process *tmp = get_last_process(i->process);

		kill(tmp->pid, SIGCONT);
		return 0;
	}
	p = mx_get_process(i->process, argv[1]);
	if (p == 0)
		return 1;
    dprintf(fd, "[%d]    %d continued  %s\n", p->pos, p->pid, p->cmd);
    kill(p->pid, SIGCONT);
    return 0;
}
