#include "../../inc/ush.h"

static void execute_binary_file(t_ast *t, t_info *i, pid_t pid) {
    char *path;

    if (mx_redirection(t->type)) {
        i->fd_r = mx_create_file(t, i);
        mx_run_redirection(t, i, pid);
    }
    else {
        path = i->paths ? mx_find_in_PATH(i->paths, t->command[0], 1) : NULL;
        if (execv(path, t->command) == -1) {
            mx_print_error(i, MX_ER, t->command[0]);
        }
        mx_dup_2(i, 1);
        exit(EXIT_FAILURE);
    }
}

int mx_start_function(t_ast *t, t_info *i, char **tree) {
    int pos = mx_arr_size(t->command) - 1;

    if (i->file_not_f != 1) {
        i->args = tree;
        if (i->file == 1) {
            mx_exec_for_file(t, i);
        }
        else {
            t->command ? setenv("_", t->command[pos], 1) : 0;
            if (mx_check_buildin(i, i->args[0], 1) == -1)
                mx_execute_binary_file(t, i);
        }
    }
    return 0;
}

void mx_execute_binary_file(t_ast *t, t_info *i) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        mx_dup_2(i, 0);
        execute_binary_file(t, i, pid);
    }
    else if (pid < 0)
        mx_print_error(i, MX_ER, t->command[0]);
    else {
        int status = 0;

        mx_waitpid(i, t, status, pid);
    }
}
