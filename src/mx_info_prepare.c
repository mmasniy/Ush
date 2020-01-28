#include "ush.h"

static void init_continue(t_info *info);

void mx_info_start(t_info *info) {
    extern char **environ;
    int num = 0;
    // int count = 0;
    // int j = 0;

    for (int i = 0; info->builtin_str[i]; i++)
        num++;
    // info->env_o = environ;
    // for (int i = 0; info->env_o[i]; i++, count++);

    // info->env_c = (char **)malloc(sizeof(char *) * (count + 1));
    // for (; environ[j]; j++)
    //     info->env_c[j] = mx_strdup(environ[j]);
    // info->env_c[j] = NULL;
    info->num_of_func = num;
    info->args = NULL;
    info->OLDPWD = NULL;
    info->history_pack = (t_history_pack *)malloc(sizeof(t_history_pack));
    mx_memset(info->history_pack, 0, sizeof(t_history_pack));
    info->to_export = mx_save_env_as_list(environ);
    info->variables = mx_save_env_as_list(environ);
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
