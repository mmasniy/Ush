#include "../../inc/ush.h"

int mx_start_function(t_ast *t, t_info *info, char **tree) {
    // mx_del_strarr(info->args);
    // printf("11\n");
    info->args = tree;
    if (mx_check_buildin(info, 1) == -1)
        mx_execute_binary_file(t, info, tree);
    // printf("12\n");
    return 0;
}

int mx_execute_binary_file(t_ast *t, t_info *info, char **args) {
    char *path;
    pid_t pid;
    int fd;
    if (args) {}
    // printf("mx_redirection(t->type) = %d\n", mx_redirection(t->type));
    fd = (mx_redirection(t->type) ? mx_create_file(t) : -1);
    // printf("fd = %d\n", fd);
    pid = fork();
    if (pid == 0) {
        if (mx_redirection(t->type) && t->type != 4) {
                dup2(fd, 1);
                t = t->left;
        }
        else {
            dup2(fd, 0);
            t = t->left;
        }
        path = mx_find_in_PATH(info->paths, t->command[0], 1);
        if (execv(path, t->command) == -1)
            perror(USH);
        if (fd != -1)
            close(fd);
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

