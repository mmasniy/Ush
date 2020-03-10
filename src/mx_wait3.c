#include "../inc/ush.h"

static void plus_minus(t_process **tmp) {
    t_process *tmp1 = NULL;

    if (*tmp) {
        tmp1 = *tmp;
        while (tmp1) {
            if (tmp1->value <= -1) {
                tmp1->value -= 1;
                tmp1->last_value = tmp1->value + 1;
            }
            if (tmp1->value == 1) {
                tmp1->value = -1;
                tmp1->last_value = 1;
            }
            tmp1 = tmp1->next;
        }
    }
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

void mx_add_inside_list(t_process **p, t_process **a, pid_t pid, char **cmd) {
    t_process *new = NULL;

    if (*p) {
        plus_minus(a);
        new = (t_process*)malloc(sizeof(t_process));
        new->pos = (*p)->pos + 1;
        new->cmd = mx_strarr_to_str(cmd, 0);
        new->pid = pid;
        new->value = 1;
        new->last_value = 0;
        new->next = (*p)->next;
        (*p)->next = new;
    }
}

void mx_push_front_proc(t_process **p, pid_t pid, char **cmd) {
    t_process *new = (t_process*)malloc(sizeof(t_process));
    
    new->pos = 1;
    new->cmd = mx_strarr_to_str(cmd, 0);
    new->pid = pid;
    new->value = 1;
    new->last_value = 0;
    if (*p){
        new->next = *p;
        *p = new;
    }
    else {
        new->next = NULL;
        *p = new;
    }
    plus_minus(p);
}

void mx_add_process(t_process **p, pid_t pid, char **com) {
    t_process *new_node = (t_process*)malloc(sizeof(t_process));
    t_process *tmp = NULL;

    if (com && *com) {
        new_node->cmd = mx_strarr_to_str(com, 0);
        new_node->pos = search_number_procces(*p);
        new_node->pid = pid;
        new_node->value = 1;
        new_node->last_value = 0;
        new_node->next = NULL;
    }
    if (*p) {
        tmp = *p;
        plus_minus(&tmp);
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    else
        *p = new_node;
}
