#include "../../inc/ush.h"

static void print_env(char **env) {
    for (int i = 0; env[i]; i++) {
        mx_printstr(env[i]);
        write(1, "\n", 1);
    }
}

static void exec_program(t_info *info, int pos, char **env, char *path) {
    pid_t pid = fork();

    if (pid == 0) {
        if (execve(path, &(info->args[pos]), env) == -1)
            perror(USH);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        perror(USH);
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status)) {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
    }
}

static void step_to_exec(t_info *info, char **path, int *fgs, t_export *env) {
    int res = 0;
    char **env_massive = !fgs[0] ? mx_save_env_as_massive(env) : NULL;
    char *path_in_env = NULL;

    if ((res = mx_check_to_execute(info, path, fgs[3]))) {
        if (res == 1)
            exec_program(info, fgs[3], env_massive, *path);
        else if (res == 2) {
            mx_save_PATH(info, getenv("PATH"));
            path_in_env = mx_find_in_PATH(info->paths, info->args[fgs[3]], 1);
            exec_program(info, fgs[3], env_massive, path_in_env);
            mx_strdel(&path_in_env);
        }
    }
    else {
        mx_printerr("env: ");
        mx_printerr(info->args[fgs[3]]);
        mx_printerr(": No such file or directory\n");
    }
    mx_del_strarr(&env_massive);
}

int mx_ush_env(t_info *info) {
    extern char **environ;
    int flags[4] = {0, 0, 0, 0};
    char *path = NULL;
    t_export *env = mx_save_env_as_list(environ);

    if (info->args[1]) {
        if (mx_check_args(env, info->args, flags, &path))
            step_to_exec(info, &path, flags, env);
        else if (!flags[0] && !flags[1] && !flags[2]) {
            char **env_massive = mx_save_env_as_massive(env);

            print_env(env_massive);
            mx_del_strarr(&env_massive);
        }
    }
    else
        print_env(environ);
    while (env)
        mx_pop_export_front(&env);
    mx_save_PATH(info, getenv("PATH"));
    return 0;
}
