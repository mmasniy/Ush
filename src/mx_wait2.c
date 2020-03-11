#include "../inc/ush.h"

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

void mx_del_procces_by_pid(t_process **p, pid_t pid) {
    t_process *pos = NULL;
    t_process *tmp = NULL;

    if (*p) {
        pos = *p;
        if (!(pos->next) || pos->pid == pid)
            mx_pop_front_process(p);
        else {
            while (pos->next && pos->next->next) {
                if (pos->next->pid == pid)
                    break;
                pos = pos->next;
            }
            tmp = pos->next->next;
            mx_strdel(&(pos->next->cmd));
            free(pos->next);
            pos->next = tmp;
        }
    }
}

void mx_pop_front_process(t_process **p) {
    if (p && *p) {
        t_process *temp = *p;
        
        *p = temp->next;
        mx_strdel(&temp->cmd);
        free(temp);
    }
}
