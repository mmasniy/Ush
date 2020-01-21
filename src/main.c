#include "ush.h"

static void mx_init_shell(char **environ, t_info *info);

void run_shell(t_info *info) {
    char *line;
    int status = 1;
    t_tok *tok = NULL;

    while (status || 1) {
        mx_custom_termios(info, STDIN_FILENO);
        line = mx_ush_read_line(info);
        mx_origin_termios(info, STDIN_FILENO);
        mx_check_history(info, line);
        
        /*
        ** Занесение в лист токенов
        */

        if (mx_work_w_toks(line, &tok) < 0)
            printf("error\n");
        
        info->args = mx_strsplit(line, ' ');
        if (mx_strlen(line) > 0) {
            t_job *new_job = (t_job *) malloc(sizeof(t_job));  //create new job
            new_job = mx_create_job(info, info->args);
            status = mx_ush_execute(info, new_job);
        }
        if (malloc_size(line))
            free(line);
        // for (t_history *tmp = info->history_pack->history; tmp; tmp = tmp->next) {
        //  printf("tmp->data: %s\n", tmp->data);
        // }
        if (info->ctrl_c)
            exit(0);
        mx_del_strarr(&info->args);
        info->ctrl_d = 0;
        info->ctrl_c = 0;
        // printf("status = %d\n", status);
        mx_free_toks(&tok);
    }
}

int main(int argc, char **argv, char **environ) {
    char *builtin_str[] = {"pwd", "cd", "help", "exit", "history", "jobs", "fg", "test", NULL};
    int (*builtin_func[]) (t_info *info, t_process *p) = {&mx_ush_pwd
        , &mx_ush_cd, &mx_ush_help, &mx_ush_exit, &mx_history, &mx_jobs, &mx_fg, &mx_test};
    t_info *info = (t_info *)malloc(sizeof(t_info));

    (void)argc;
    (void)argv;
    if (!isatty(STDIN_FILENO)) {
        mx_error_message("You must use it just in terminal");
        exit(EXIT_FAILURE);
    }
    memset(info, 0, sizeof(t_info));
    info->builtin_str = builtin_str;
    info->builtin_func = builtin_func;
    mx_init_shell(environ, info);
    run_shell(info);
    return 0;
}

static void mx_init_shell(char **environ, t_info *info) {
    bool exist = 0;

    for (int i = 0; environ[i]; i++) {
        if (mx_str_head(environ[i], "TERM=") == 0) {
            mx_info_start(info, environ);
            exist = 1;
        }
        else if (mx_str_head(environ[i], "PATH=") == 0) {
            mx_save_PATH(info, &(environ[i][5]));
        }
    }
    if (exist)
        return;
    mx_error_message(TERM_ENV_NOT_EXIST);
    exit(EXIT_FAILURE);
}
