#include "../../inc/ush.h"

int mx_start_function(t_ast *t, t_info *info, char **tree) {
    // mx_del_strarr(info->args);
    info->args = tree;
    if (mx_check_buildin(info, 1) == -1)
        mx_execute_binary_file(t, info);
    return 0;
}

int mx_execute_binary_file(t_ast *t, t_info *info) {
    char *path;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        if (mx_redirection(t->type)){
            int fd = -2;

            if (mx_redirection(t->type) && (fd = mx_create_file(t)) == -1) {
                if (t->right->command[0][0] != '-')
                    fd = mx_atoi(t->right->command[0]);
                else
                    fd = -1;
            }
            mx_run_redirection(t, info, fd, pid);
            if (fd != -1)
                close(fd);
        }
        else {
            path = mx_find_in_PATH(info->paths, t->command[0], 1);
            if (execv(path, t->command) == -1)
                perror(USH);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
        perror(USH);
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
    return 0;
}

int mx_start_red(t_ast *t, t_info *info, pid_t pid) {
    if (mx_check_buildin(info, 1) == -1)
        mx_execute_red(t, info, pid);
    return 0;
}

int mx_execute_red(t_ast *t, t_info *info, pid_t pid) {
    char *path;

    if (pid == 0) {
            path = mx_find_in_PATH(info->paths, t->command[0], 1);
            if (execv(path, t->command) == -1)
                perror(USH);
            exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        perror(USH);
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
    return 0;
}
