#include "../../inc/ush.h"

static int check_command(t_info *i, char **args) {
    if (!mx_arr_size(args)) {
        fprintf(stderr, "kill: not enough arguments\n");
        i->status = 1;
        return 0;
    }
    if (mx_arr_size(args) > 1) {
        fprintf(stderr, "kill: too many arguments\n");
        i->status = 1;
        return 0;
    }
    if (!atoi(args[0])) {
        fprintf(stderr, "kill: illegal pid: %s\n", args[0]);
        i->status = 1;
        return 0;
    }
    return 1;
}

t_process *mx_get_process_kill(t_info *i, t_process *p, char *cmd) {
    t_process *tmp = NULL;

    if (p) {
        tmp = p;
        while (tmp) {
            if (cmd && tmp->pid == atoi(cmd))
                return tmp;
            tmp = tmp->next;
        }
    }
    fprintf(stderr, "kill: %s: No such process \n", cmd);
    i->status = 1;
    return NULL;
}

static int mx_term_process(t_info *i, char **argv, int fd) {
    t_process *p = i->process;

    if (mx_atoi(argv[1])) {
        p = mx_get_process_kill(i, i->process, argv[1]);
    }
    if (p == 0)
        return 1;
    dprintf(fd, "[%d]    %d terminated  %s\n", p->pos, p->pid, p->cmd);
    if (p->value == 1)
        return_value(&(i->process), 1);
    else if (p->value == -1)
        return_value(&(i->process), -1);
    kill(p->pid, SIGTERM);
    return 0;
}

int mx_kill(t_info *i) {
    t_process *p = i->process;
    pid_t pid;

    if (!check_command(i, &(i->args[1])))
        return 1;
    pid = atoi(i->args[1]);
    while (p) {
        if (p->pid == pid) {
            mx_term_process(i, i->args, 1);
            mx_del_procces_by_pid(&(i->process), pid);
            return 0;
        }
        p = p->next;
    }
    fprintf(stderr, "kill: %s: No such process \n", i->args[1]);
    return 1;
}
