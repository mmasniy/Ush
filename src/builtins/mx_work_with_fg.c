#include "../../inc/ush.h"

t_process *mx_search_by_char(char *cmd, t_process *processes) {
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

    while (tmp->next && tmp->value != 1)
        tmp = tmp->next;
    return tmp;
}
