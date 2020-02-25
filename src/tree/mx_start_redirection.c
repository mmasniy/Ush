#include "../../inc/ush.h"

void dup_2(t_info *i, int flag) {
    if (flag == 0) {
        i->fd[0] = dup(0);
        i->fd[1] = dup(1);
        i->fd[2] = dup(2);
    }
    if (flag == 1) {
        if (i->fd[0] != STDIN_FILENO) {
            dup2(i->fd[0], 0);
            close(i->fd[0]);
        }
        if ((i->fd[1] != STDOUT_FILENO)) {
            dup2(i->fd[1], 1);
            close(i->fd[1]);
        }
        if ((i->fd[2] != STDERR_FILENO)) {
            dup2(i->fd[2], 2);
            close(i->fd[2]);
        }
    }
}

int mx_start_red(t_ast *t, t_info *info, pid_t pid) {
    if (mx_check_buildin(info, 1) == -1)
        mx_execute_red(t, info, pid);
    return 0;
}

void mx_execute_red(t_ast *t, t_info *info, pid_t pid) {
    char *path;

    if (pid == 0) {
        dup_2(info, 0);
        path = mx_find_in_PATH(info->paths, t->command[0], 1);
        if (info->fd_r < 0 && info->file != 1)
            mx_file_not_found(info->fname);
        else if (execv(path, t->command) == -1)
            mx_print_error(MX_ER, t->command[0]);
        dup_2(info, 1);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
            mx_print_error(MX_ER, t->command[0]);
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
    mx_strdel(&path);
}

void mx_exec_for_file(t_ast *t, t_info *i) {
    pid_t pid;

    // i->path_f = mx_strdup(".system_ush.txt");
    // printf("tut\n");
    pid = fork();
    mx_execute_file(t, i, pid);
    if (pid <= 0)
        exit(0);
}


void mx_execute_file(t_ast *t, t_info *info, pid_t pid) {
    char *path;

    if (pid == 0) {
        dup_2(info, 0);
        if (info->fd_f < 0)
            info->fd_f = open(info->path_f, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        // printf("i->fd_f = %d\n", info->fd_f);
        // printf("i->file = %d\n", info->file);
        if (info->fd_f != STDOUT_FILENO) {
            dup2(info->fd_f, 1);
            close(info->fd_f);
        }
        if (mx_check_buildin(info, 1) == -1) {
            path = mx_find_in_PATH(info->paths, t->command[0], 1);
            if (info->fd_r < 0 && info->file != 1)
                mx_file_not_found(info->fname);
            else if (execv(path, t->command) == -1)
                mx_print_error(MX_ER, t->command[0]);
        }
        dup_2(info, 1);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
            mx_print_error(MX_ER, t->command[0]);
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
    mx_strdel(&path);
}
