#include "../inc/ush.h"

static void mx_print_pr(t_process *p) {
	t_process *t = p;
	if (p) {
		while (t) {
			if (p->pid && p->cmd && p->pos) {
				printf("\n[%d] + %d ush: suspended ", t->pos, t->pid);
				printf("%s\n", t->cmd);
			}
			t = t->next;
		}
	}
}

void mx_waitpid(t_info *i, t_ast *t, int status, pid_t pid) {
	pid_t wpid = waitpid(pid, &status, WUNTRACED);
	if (!MX_WIFEXIT(status)) {
		if (MX_WIFSIG(status) && MX_WTERMSIG(status) == SIGINT){
			mx_del_procces_by_pid(&(i->process), pid);
			i->status = 130;
		}
		else {
			mx_add_process(&(i->process), pid, t->command); //+
			mx_print_added_new_node(i->process, pid); // +
		}
	}
	else {
		mx_del_procces_by_pid(&(i->process), pid);
		i->status = MX_WEXITSTATUS(status);
	}
	// mx_print_pr(i->process);
}

void mx_pop_front_process(t_process **p) {
	if (p && *p) {
		t_process *temp = *p;
		
		*p = temp->next;
		free(temp);
	}
}

void mx_del_procces_by_pid(t_process **p, pid_t pid) {
	t_process *pos = NULL;
	t_process *tmp = NULL;

	if (*p) {
		pos = *p;
		if (!pos->next){
			mx_pop_front_process(p);
			return;
		}
		while (pos->next && pos->next->pid != pid)
			pos = pos->next;
		tmp = pos->next->next;
		pos->next->next = NULL;
		mx_strdel(&pos->next->cmd);
		free(pos->next);
		pos = tmp;
	}
}

void mx_print_added_new_node(t_process *p, pid_t pid) {
	t_process *tmp = NULL;

	if (p) {
		tmp = mx_get_name_procces(p, pid); //+
		if (tmp && tmp->cmd) {
			printf("[%d] + %d ush: suspended ", tmp->pos, tmp->pid);
			printf("%s\n", tmp->cmd);
		}
	}
}

t_process *mx_get_name_procces(t_process *process, pid_t pid) {
	t_process *tmp = NULL;

	if (process) {
		tmp = process;
		while (tmp && tmp->pid != pid)
			tmp = tmp->next;
		if (tmp && tmp->cmd)
			return tmp;
	}
	return NULL;
}
static int search_number_procces(t_process *p) {
	t_process *tmp = p;

	if (p && p->pos) {
		while (tmp->next) {
			if ((tmp->pos + 1) != tmp->next->pos)
				break;
			tmp = tmp->next;
		}
		return tmp->pos + 1;
	}
	return 1;
}
void mx_add_process(t_process **p, pid_t pid, char **com) {
	t_process *new_node = (t_process*)malloc(sizeof(t_process));
	t_process *tmp = NULL;

	if (com && *com) {
		new_node->cmd = mx_strarr_to_str(com, 0);
		new_node->pos = search_number_procces(*p);
		new_node->pid = pid;
		new_node->next = NULL;
	}
	if (*p) {
		tmp = *p;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	else
		*p = new_node;
}
