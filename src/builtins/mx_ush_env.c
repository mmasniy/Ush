#include "../../inc/ush.h"

static void print_env(char **env) {
    for (int i = 0; env[i]; i++) {
        mx_printstr(env[i]);
        write(1, "\n", 1);
    }
}

static int exec_program(t_info *info, int pos, char **env, char *path) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        if (execve(path, &(info->args[pos]), env) == -1)
            perror(USH);
        exit(info->status);
    }
    else {
        int status = 0;

        mx_waitpid(info, info->t, status, pid);
    }
    return info->status;
}

static int step_to_exec(t_info *i, char **path, int *f, t_export *env) {
    int res = 0;
    char **env_massive = mx_save_env_as_massive(env);
    char *path_in_env = NULL;

    if ((res = mx_check_to_execute(i, path, f[3]))) {
        if (res == 1)
            res = exec_program(i, f[3], env_massive, *path);
        else if (res == 2) {
            mx_save_PATH(i);
            path_in_env = mx_find_in_PATH(i->paths, i->args[f[3]], 1);
            res = exec_program(i, f[3], env_massive, path_in_env);
            mx_strdel(&path_in_env);
        }
    }
    else {
        fprintf(stderr, "env: %s: No such file or directory\n", i->args[f[3]]);
        return 1;
    }
    mx_del_strarr(&env_massive);
    return res;
}

static bool result(t_info *info, bool res, t_export **env) {
    while (*env)
        mx_pop_export_front(env);
    mx_save_PATH(info);
    return res;
}

int mx_ush_env(t_info *info) {
    extern char **environ;
    int flags[5] = {0, 0, 0, 0, 0};
    char *path = NULL;
    t_export *env = mx_save_env_as_list(environ);

    if (info->args[1]) {
        if (mx_check_args(&env, info->args, flags, &path))
            return result(info, step_to_exec(info, &path, flags, env), &env);
        else if (flags[1] == 0 && flags[2] == 0 && flags[4] == 0) {
            char **env_massive = mx_save_env_as_massive(env);

            print_env(env_massive);
            mx_del_strarr(&env_massive);
        }
        else
            return result(info, 1, &env);
    }
    else
        print_env(environ);
    return result(info, 0, &env);
}
