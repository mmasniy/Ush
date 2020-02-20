#include "../inc/ush.h"

int mx_run_buildin(t_info *info) {
    int exit_code;

    !strcmp(info->args[0], "pwd") ? exit_code = mx_ush_pwd(info) : 0;
    !strcmp(info->args[0], "cd") ? exit_code = mx_ush_cd(info) : 0;
    !strcmp(info->args[0], "help") ? exit_code = mx_ush_help(info) : 0;
    !strcmp(info->args[0], "exit") ? exit_code = mx_ush_exit(info) : 0;
    !strcmp(info->args[0], "history") ? exit_code = mx_ush_history(info) : 0;
    !strcmp(info->args[0], "env") ? exit_code = mx_ush_env(info) : 0;
    !strcmp(info->args[0], "unset") ? exit_code = mx_ush_unset(info) : 0;
    !strcmp(info->args[0], "export") ? exit_code = mx_ush_export(info) : 0;
    !strcmp(info->args[0], "which") ? exit_code = mx_ush_which(info) : 0;
    !strcmp(info->args[0], "echo") ? exit_code = mx_ush_echo(info) : 0;
    !strcmp(info->args[0], "fg") ? mx_fg(info) : 0;
    return exit_code;
}

int mx_check_buildin(t_info *info, bool exec) {
  int return_value;

    if ((!strcmp(info->args[0], "pwd")) || (!strcmp(info->args[0], "cd"))
        || (!strcmp(info->args[0], "help"))
        || (!strcmp(info->args[0], "exit"))
        || (!strcmp(info->args[0], "history"))
        || (!strcmp(info->args[0], "env")) || (!strcmp(info->args[0], "unset"))
        || (!strcmp(info->args[0], "export"))
        || (!strcmp(info->args[0], "which"))|| (!strcmp(info->args[0], "echo"))
        || (!strcmp(info->args[0], "fg"))) {
        if (exec)
            return_value = mx_run_buildin(info);
        else
            return_value = 1;
    }
    else
        return (-1);
    return (return_value == 1 ? 0 : 1);
}
