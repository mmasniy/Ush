#include "../../inc/ush.h"

static int check_command(t_info *i, char **cmd) {
    if (!mx_arr_size(cmd))
        return 1;
    if (mx_arr_size(cmd) > 1) {
        fprintf(stderr, "fg: too many arguments\n");
        i->status = 127;
        return 0;
    }
    if (cmd[0][0] != '%' || !cmd[0][1]) {
        fprintf(stderr, "fg: invalid argument: %s\n", cmd[0]);
        i->status = 127;
        return 0;
    }
    return 1;
}

static int output_error(t_info *i, char **args) {
    if (args && args[1] && atoi(&(args[1][1]))) {
        fprintf(stderr, "fg: %s: no such job\n", &(args[1][1]));
        i->status = 127;
    }
    else {
        i->status = 1;
        fprintf(stderr, "fg: no current jobs\n");
    }
    return i->status;
}

void return_value(t_process **p, int flag) {
    t_process *tmp = *p;

    if (flag == 1) {
        for (; tmp; tmp = tmp->next) {
            tmp->last_value = tmp->value;
            if (tmp->value == -1)
                tmp->value += 2;
            else if (tmp->value != 1)
                tmp->value++;
        }
    }
    else if (flag == -1) {
        for (; tmp; tmp = tmp->next) {
            tmp->last_value = tmp->value;
            if (tmp->value == -1)
                tmp->value += 2;
            else if (tmp->value < 1)
                tmp->value++;
        }
    }
}

int mx_fg(t_info *i, int status) {
    pid_t child;

    if (!check_command(i, &(i->args[1])))
        return i->status;
    if (i->process) {
        if (mx_continue_process(i, i->args, 1, 0) == 0) {
            child = waitpid(-1, &status, WUNTRACED);
            if (!MX_WIFEXIT(status))
                mx_wait_process(i, status, child);
            else {
                mx_del_procces_by_pid(&(i->process), child);
                i->status = MX_WEXITSTATUS(status);
            }
        }
    }
    else
        return output_error(i, i->args);
    return 0;
}

int mx_continue_process(t_info *i, char **argv, int fd, pid_t last) {
    t_process *p = i->process;

    if (argv[1] == 0) {
        p = get_last_process(i->process);
    }
    else if (!mx_atoi(&(argv[1][1])) || mx_atoi(&(argv[1][1]))) {
        p = mx_get_process(i, i->process, argv[1]);
    }
    if (p == 0)
        return 1;
    dprintf(fd, "[%d]    %d continued  %s\n", p->pos, p->pid, p->cmd);
    if (p->value == 1)
        return_value(&(i->process), 1);
    else if (p->value == -1)
        return_value(&(i->process), -1);
    last = p->pid;
    kill(last, SIGCONT);
    return 0;
}
