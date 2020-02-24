#include "../inc/ush.h"

static void init_continue(t_info *info) {
    int shell_is_interactive = isatty(STDIN_FILENO);

    if (shell_is_interactive) {
        signal(SIGINT, mx_sigio_handler);
        signal(SIGIO, mx_sigio_handler);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
    }
    info->name = strdup(USH);
}

static void set_pwd(t_info *info) {
    char *tmp = NULL;
    char *pwd_path = strdup("/");
    char **path = NULL;
    char *check = getcwd(NULL, 0);

    if ((path = mx_strsplit(getenv("PWD"), '/'))) {
        for (int i = 0; path[i]; i++) {
            tmp = mx_strjoin(pwd_path, path[i]);
            mx_strdel(&pwd_path);
            if (readlink(tmp, NULL, 0) >= 0) {
                pwd_path = mx_strnew(1000);
                readlink(tmp, pwd_path, 1000);
                if (strcmp(pwd_path, "..") == 0) {
                    mx_find_last_slash(&tmp);
                    mx_find_last_slash(&tmp);
                }
                else if (strcmp(pwd_path, ".") == 0)
                    mx_find_last_slash(&tmp);
                else {
                    mx_strdel(&tmp);
                    tmp = mx_strdup(pwd_path);
                }
                mx_strdel(&pwd_path);
            }
            pwd_path = mx_strjoin(tmp, "/");
            mx_strdel(&tmp);
        }
    }
    pwd_path[strlen(pwd_path) - 1] = '\0';
    if (strcmp(pwd_path, check))
        setenv("PWD", check, 1);
    mx_strdel(&pwd_path);
    mx_strdel(&check);
    mx_del_strarr(&path);
    info->pwd = strdup(getenv("PWD"));
    info->oldpwd = strdup(getenv("PWD"));
    // name for transport file
    info->path_f = mx_strdup(".system_ush.txt");
}

static void open_history_file(t_info *info) {
    char *history_file = mx_file_to_str(".history_ush.txt");

    if (history_file) {
        char **lines = mx_strsplit(history_file, '\n');

        for (int i = 0; lines[i]; i++)
            mx_push_history_front(&info->history_pack->history, lines[i]);
        mx_strdel(&history_file);
        mx_del_strarr(&lines);
    }
}

void mx_info_start(t_info *info) {
    extern char **environ;
    info->args = NULL;
    info->OLDPWD = NULL;
    info->history_pack = (t_history_pack *)malloc(sizeof(t_history_pack));
    mx_memset(info->history_pack, 0, sizeof(t_history_pack));
    info->to_export = mx_save_env_as_list(environ);
    info->variables = mx_save_env_as_list(environ);
    info->alias = NULL;
    info->num_of_al = 0;
    // info->alias = (t_alias *)malloc(sizeof(t_alias));
    // info->alias->name = NULL;
    // info->alias->value = NULL;
    // info->alias->next = NULL;
    set_pwd(info);
    open_history_file(info);
    init_continue(info);
}
