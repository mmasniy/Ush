#include "../../inc/ush.h"

t_process *mx_search_by_char(t_info *i, char *cmd, t_process *processes) {
    t_process *tmp = NULL;

    if (processes) {
        tmp = processes;
        while (tmp) {
            if (mx_strncmp(tmp->cmd, cmd, mx_strlen(cmd)) == 0)
                return tmp;
            tmp = tmp->next;
        }
    }
    fprintf(stderr, "fg: %s: no such job\n", cmd);
    i->status = 127;
    return NULL;
}

t_process *mx_search_by_id(t_info *i, int pos, t_process *processes) {
    t_process *tmp = NULL;

    if (processes) {
        tmp = processes;
        while (tmp) {
            if (tmp->pos == pos)
                return tmp;
            tmp = tmp->next;
        }
    }
    fprintf(stderr, "fg: %d: no such job\n", pos);
    i->status = 127;
    return NULL;
}

t_process *mx_get_process(t_info *i, t_process *process, char *cmd) {
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
        return mx_search_by_id(i, atoi(cmd), p);
    return mx_search_by_char(i, cmd, p);
}

t_process *get_last_process(t_process *p) {
    t_process *tmp = p;

    while (tmp->next && tmp->value != 1)
        tmp = tmp->next;
    return tmp;
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

