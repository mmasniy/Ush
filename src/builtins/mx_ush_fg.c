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

static void return_value(t_process **p, int flag) {
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

void mx_wait_process(t_info *i, int status, pid_t child) {
    if (MX_WIFSIG(status)) {
        if (MX_WTERMSIG(status) == SIGINT) {
            i->status = 130;
        }
        else
            mx_print_added_new_node(i->process, child);
    }
}

int mx_fg(t_info *i, int status) {
    pid_t child;

    if (!check_command(&(i->args[1])))
        return 1;
    if (i->process) {
        if (mx_continue_process(i, i->args, 1) == 0) {
            child = waitpid(-1, &status, WUNTRACED);
            if (!MX_WIFEXIT(status))
                mx_wait_process(i, status, child);
            else
                i->status = MX_WEXITSTATUS(status);
        }
    }
    else {
        fprintf(stderr, "%s", "fg: no current jobs\n");
        return 1;
    }
    return 0;
}

int mx_continue_process(t_info *i, char **argv, int fd) {
    t_process *p = i->process;

    if (argv[1] == 0 || !mx_atoi(argv[1]))
        p = get_last_process(i->process);
    else
        p = mx_get_process(i->process, argv[1]);
    if (p == 0)
        return 1;
    dprintf(fd, "[%d]    %d continued  %s\n", p->pos, p->pid, p->cmd);
    kill(p->pid, SIGCONT);
    if (p->value == 1)
        return_value(&(i->process), 1);
    else if (p->value == -1)
        return_value(&(i->process), -1);
    mx_del_procces_by_pid(&(i->process), p->pid);
    return 0;
}
