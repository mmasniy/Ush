#include "../inc/ush.h"

static void init_continue(t_info *info) {
    char *key = "PWD";
    char *value = getenv("PWD");

    signal(SIGINT, mx_sigio_handler);
    signal(SIGIO, mx_sigio_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, mx_sigio_handler);
    info->name = strdup(USH);
    mx_update_key_value(&(info->to_export), &key, &value);
    mx_update_key_value(&(info->variables), &key, &value);
}

static void set_pwd(t_info *info) {
    char *pwd_path = getenv("PWD");
    char *cwd = getcwd(NULL, 0);
    char *path = NULL;

    if (pwd_path == NULL || chdir(pwd_path) < 0)
        setenv("PWD", cwd, 1);
    else {
        path = mx_save_without_links(info, pwd_path);
        if (strcmp(path, cwd))
            setenv("PWD", cwd, 1);
    }
    mx_strdel(&pwd_path);
    mx_strdel(&cwd);
    mx_strdel(&path);
    info->pwd = strdup(getenv("PWD"));
    info->oldpwd = strdup(getenv("PWD"));
    unsetenv("OLDPWD");
    // name for transport file
    info->path_f = "/tmp/.system_ush.txt";
    info->history_path = "/tmp/.history_ush.txt";
}

static void open_history_file(t_info *info) {
    struct stat time;
    char *history_file = mx_file_to_str("/tmp/.history_ush.txt");

    if (history_file) {
        stat("/tmp/.history_ush.txt", &time);
        if (time.st_mtime == 1576800125) {
            char **lines = mx_strsplit(history_file, '\n');

            for (int i = 0; lines[i]; i++) {
                if (info->history_pack->total_num >= 250)
                    break;
                mx_push_history_front(&info->history_pack->history, lines[i]);
                (info->history_pack->total_num)++;
            }
            mx_strdel(&history_file);
            mx_del_strarr(&lines);
        }
    }
    remove("/tmp/.history_ush.txt");
}

void mx_info_start(t_info *info) {
    extern char **environ;

    info->args = NULL;
    info->history_pack = (t_history_pack *)malloc(sizeof(t_history_pack));
    mx_memset(info->history_pack, 0, sizeof(t_history_pack));
    set_pwd(info);
    info->to_export = mx_save_env_as_list(environ);
    info->variables = mx_save_env_as_list(environ);
    info->alias = NULL;
    info->num_of_red = 0;
    open_history_file(info);
    init_continue(info);
}
