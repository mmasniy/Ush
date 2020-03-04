#include "../inc/ush.h"

static void check_number_proc(t_process **p, pid_t pid, char **cmd) {
    t_process *tmp = NULL;

    if (*p) {
        tmp = *p;
        if (tmp->pos != 1) {
            mx_push_front_proc(p, pid, cmd);
            return;
        }
        else {
            while (tmp && tmp->next) {
                if (!(tmp->pos + 1 == tmp->next->pos)) {
                    mx_add_inside_list(&tmp, p, pid, cmd);
                    return;
                }
                tmp = tmp->next;
            }
        }
    }
    mx_add_process(p, pid, cmd);
}

void mx_waitpid(t_info *i, t_ast *t, int status, pid_t pid) {
    pid_t wpid = waitpid(pid, &status, WUNTRACED);

    if (!MX_WIFEXIT(status)) {
        if (MX_WIFSIG(status) && MX_WTERMSIG(status) == SIGINT){
            mx_del_procces_by_pid(&(i->process), pid);
            i->status = 130;
        }
        else {
            check_number_proc(&(i->process), pid, t->command);
            mx_print_added_new_node(i->process, pid);
        }
    }
    else {
        i->status = MX_WEXITSTATUS(status);
    }
}
