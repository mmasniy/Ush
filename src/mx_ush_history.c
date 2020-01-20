#include "ush.h"

int mx_history(t_info *info, t_process *p) {
    if (p) {}
    short num_of_history = 1;
    t_history *tmp = info->history_pack->history;

    while (tmp->next)
        tmp = tmp->next;
    for (; tmp; tmp = tmp->prev, num_of_history++) {
        mx_printstr(YEL);
        mx_printint(num_of_history);
        write(1, ": \t", 3);
        mx_printstr(RESET);
        mx_printstr(tmp->data);
        write(1, "\n", 1);
    }
    return 1;
}
