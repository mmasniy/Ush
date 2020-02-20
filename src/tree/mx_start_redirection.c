#include "../../inc/ush.h"

int mx_start_red(t_ast *t, t_info *info, pid_t pid) {
    if (mx_check_buildin(info, 1) == -1)
        mx_execute_red(t, info, pid);
    return 0;
}

void mx_execute_red(t_ast *t, t_info *info, pid_t pid) {
    char *path;

    if (pid == 0) {
            path = mx_find_in_PATH(info->paths, t->command[0], 1);
            if (info->fd_r < 0)
                mx_file_not_found(info->fname);
            else if (execv(path, t->command) == -1){
                // close(info->fd[2]);
                fprintf(stderr, "3\n");
                mx_print_error(MX_ER, t->command[0]);
            }
            exit(EXIT_FAILURE);
    }
    else if (pid < 0){
            // close(info->fd[2]);
            fprintf(stderr, "4\n");
            mx_print_error(MX_ER, t->command[0]);
    }
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
    mx_strdel(&path);
}

/*
#include "../../inc/ush.h"

int mx_start_function(t_ast *t, t_info *i, char **tree) {
    if (i->file_not_f != 1) {
        i->args = tree;
        if (i->file == 1) {
            mx_exec_for_file(t, i);
        }
        else {
            if (mx_check_buildin(i, 1) == -1){
                //search alias
                // dup2(i->fd[0], 0);
                // dup2(i->fd[1], 1);
                // dup2(i->fd[2], 2);
                mx_execute_binary_file(t, i);
                
            }
        }
    }
    
    return 0;
}

void mx_exec_for_file(t_ast *t, t_info *i) {
    pid_t pid;

    i->path_f = mx_strdup(".system_ush.txt");
    i->fd_f = open(i->path_f, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    pid = fork();
    if (i->fd_f != STDOUT_FILENO){
        dup2(i->fd_f, 1);
        close(i->fd_f);
    }
    if (mx_check_buildin(i, 1) == -1) {
        mx_execute_red(t, i, pid);
    }
    if (pid <= 0)
        exit(0);
}

static void execute_binary_file(t_ast *t, t_info *i, pid_t pid) {
    char *path;

    if (mx_redirection(t->type)) {
        i->fd_r = mx_create_file(t, i);
        mx_run_redirection(t, i, pid);
    }
    else {
        // mx_check_alias(t, i, 0);
        path = mx_find_in_PATH(i->paths, t->command[0], 1);
        if (execv(path, t->command) == -1)
            mx_print_error(MX_ER, t->command[0]);
        exit(EXIT_FAILURE);
    }
}

void dup_2(t_info *i) {
    if (i->fd[0] != STDIN_FILENO) {
        dup2(i->fd[0], 0);
        close(i->fd[0]);
    }
    if ((i->fd[1] != STDOUT_FILENO)) {
        dup2(i->fd[1], 1);
        close(i->fd[1])
    }
    if ((i->fd[2] != STDERR_FILENO)) {
        dup2(i->fd[2], 2);
        close(i->fd_f[2]);
    }
}

void mx_execute_binary_file(t_ast *t, t_info *i) {
    pid_t pid;

    pid = fork();
    if (pid == 0){
        dup_2(i);
        execute_binary_file(t, i, pid);
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
}

*/
