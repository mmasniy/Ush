#include "../inc/ush.h"

static void init_continue(t_info *info);

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
    // printf("================\n");
    // printf("pwd_path = %s\n", pwd_path);
    // printf("check = %s\n", check);
    // printf("================\n");
    if (strcmp(pwd_path, check))
        setenv("PWD", check, 1);
    mx_strdel(&pwd_path);
    mx_strdel(&check);
    mx_del_strarr(&path);
    info->pwd = strdup(getenv("PWD"));
    info->oldpwd = strdup(getenv("PWD"));
    // printf("pwd = %s\n", info->pwd);
}

void mx_info_start(t_info *info) {
    extern char **environ;
    info->num_of_func = 11;
    info->args = NULL;
    info->OLDPWD = NULL;
    info->history_pack = (t_history_pack *)malloc(sizeof(t_history_pack));
    mx_memset(info->history_pack, 0, sizeof(t_history_pack));
    info->to_export = mx_save_env_as_list(environ);
    info->variables = mx_save_env_as_list(environ);
    set_pwd(info);
    init_continue(info);
}

static void init_continue(t_info *info) {
    pid_t shell_pgid;
//  struct termios shell_tmodes;
    int shell_is_interactive = isatty(STDIN_FILENO);
    /* See if we are running interactively.  */
//    mx_terminal_init(m_s);
    if (shell_is_interactive) {
        /* Loop until we are in the foreground.  */
//      while (tcgetpgrp(STDIN_FILENO) != (shell_pgid = getpgrp()))
//          kill(-shell_pgid, SIGTTIN);
        /* Ignore interactive and job-control signals.  */
        signal(SIGINT, mx_sigio_handler);
        signal(SIGIO, mx_sigio_handler);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
        /* Put ourselves in our own process group.  */
        shell_pgid = getpid();
        // printf("parent shell_pgid %d\n", shell_pgid);
        // tcgetpgrp(int fd);
        /*The function tcgetpgrp() returns the process group ID of the foreground process
        * group on the terminal associated to fd */
        // printf("идентификатор группы процессов  %d\n", tcgetpgrp(shell_pgid));
        if (setpgid (shell_pgid, shell_pgid) < 0) {
            perror ("Couldn't put the shell in its own process group");
            exit (1);
        }
        // tcsetpgrp (shell_terminal, shell_pgid);
        info->shell_pgid = shell_pgid;
        mx_memset(info->jobs, 0, sizeof(t_job));
        info->max_number_job = 2;
    }
}
