#include "../inc/ush.h"

static int cont_run_built(t_info *info, int *exit_code) {
    !strcmp(info->args[0], "fg") ? *exit_code = mx_fg(info, 0) : 0;
    !strcmp(info->args[0], "jobs") ? *exit_code = mx_jobs(info) : 0;
    !strcmp(info->args[0], "kill") ? *exit_code = mx_kill(info) : 0;
    return (info->status = *exit_code);
}

int mx_run_buildin(t_info *info) {
    int exit_code;

    !strcmp(info->args[0], "pwd") ? exit_code = mx_ush_pwd(info) : 0;
    !strcmp(info->args[0], "cd") ? exit_code = mx_ush_cd(info) : 0;
    !strcmp(info->args[0], "help") ? exit_code = mx_ush_help(info) : 0;
    !strcmp(info->args[0], "exit") ? exit_code = mx_ush_exit(info) : 0;
    !strcmp(info->args[0], "history") ? exit_code = mx_ush_history(info) : 0;
    !strcmp(info->args[0], "env") ? exit_code = mx_ush_env(info) : 0;
    !strcmp(info->args[0], "set") ? exit_code = mx_ush_set(info) : 0;
    !strcmp(info->args[0], "unset") ? exit_code = mx_ush_unset(info) : 0;
    !strcmp(info->args[0], "export") ? exit_code = mx_ush_export(info) : 0;
    !strcmp(info->args[0], "which") ? exit_code = mx_ush_which(info) : 0;
    !strcmp(info->args[0], "echo") ? exit_code = mx_ush_echo(info) : 0;
    !strcmp(info->args[0], "custom") ? exit_code = mx_ush_custom(info) : 0;
    !strcmp(info->args[0], "true") ? exit_code = mx_ush_true(info) : 0;
    !strcmp(info->args[0], "false") ? exit_code = mx_ush_false(info) : 0;
    !strcmp(info->args[0], "return") ? exit_code = mx_ush_return(info) : 0;
    cont_run_built(info, &exit_code);
    return (info->status = exit_code);
}

int mx_check_buildin(t_info *info, char *arg, bool exec) {
    int return_value;

    if ((!strcmp(arg, "pwd")) || (!strcmp(arg, "cd"))
        || (!strcmp(arg, "help")) || (!strcmp(arg, "exit"))
        || (!strcmp(arg, "history"))
        || (!strcmp(arg, "env")) || (!strcmp(arg, "set"))
        || (!strcmp(arg, "unset"))
        || (!strcmp(arg, "export"))
        || (!strcmp(arg, "which")) || (!strcmp(arg, "fg"))
        || (!strcmp(arg, "echo")) || (!strcmp(arg, "true"))
        || (!strcmp(arg, "false")) || (!strcmp(arg, "kill"))
        || (!strcmp(arg, "return"))
        || (!strcmp(arg, "jobs"))
        || (!strcmp(arg, "custom")))  {
        return exec ? mx_run_buildin(info) : 1;
    }
    return -1;
}

char *mx_find_similar_buildin(char *what_check) {
    char *res = NULL;

    if ((!mx_str_head(what_check, "pwd") && (res = strdup("pwd")))
        || (!mx_str_head(what_check, "cd") && (res = strdup("cd")))
        || (!mx_str_head(what_check, "help") && (res = strdup("help")))
        || (!mx_str_head(what_check, "exit") && (res = strdup("exit")))
        || (!mx_str_head(what_check, "history") && (res = strdup("history")))
        || (!mx_str_head(what_check, "env") && (res = strdup("env")))
        || (!mx_str_head(what_check, "set") && (res = strdup("set")))
        || (!mx_str_head(what_check, "unset") && (res = strdup("unset")))
        || (!mx_str_head(what_check, "export") && (res = strdup("export")))
        || (!mx_str_head(what_check, "which") && (res = strdup("which")))
        || (!mx_str_head(what_check, "echo") && (res = strdup("echo")))
        || (!mx_str_head(what_check, "custom") && (res = strdup("custom")))
        || (!mx_str_head(what_check, "true") && (res = strdup("true")))
        || (!mx_str_head(what_check, "false") && (res = strdup("false")))
        || (!mx_str_head(what_check, "return") && (res = strdup("return")))
        || (!mx_str_head(what_check, "jobs") && (res = strdup("jobs")))
        || (!mx_str_head(what_check, "fg") && (res = strdup("fg"))) || 1)
        return res;
}

void mx_cntr_key(t_info *i) {
    if (i->ctrl_d) {
        if (i->process && i->d == 0) {
            fprintf(stderr, "%s\n", "u$h: you have suspended jobs.");
            i->d++;
        }
        else {
            mx_save_all_history(i);
            for (t_process *p = i->process; p; p = p->next)
                kill(p->pid, SIGHUP);
            exit(0);
        }
    }
}
