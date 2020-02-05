#include "ush.h"

static void mx_init_shell(t_info *info);

void run_shell(t_info *info) {
    char *line;
    int status = 1;
    t_tok *tok = NULL;
    t_ast *tree = NULL;

    while (status || 1) {
        mx_count_name_len(info);
        mx_custom_termios(info, STDIN_FILENO);
        line = mx_ush_read_line(info);
        mx_origin_termios(info, STDIN_FILENO);
        mx_check_history(info, line);
        if (mx_work_w_toks(line, &tok) < 0)
            printf("error\n");
        tree = NULL;
        /*
        ** Занесение в лист токенов
        */
        mx_parse_line(info, &line);

        info->args = mx_strsplit(line, ' ');
        if (mx_strlen(line) > 0) {
            t_job *new_job = (t_job *) malloc(sizeof(t_job)); //create new job
            new_job = mx_create_job(info, info->args);
            status = mx_ush_execute(info, new_job);
        }
        if (malloc_size(line))
            free(line);
        if (info->ctrl_c)
            exit(0);
        mx_del_strarr(&info->args);
        info->ctrl_d = 0;
        info->ctrl_c = 0;
        mx_free_tree(tree);
        mx_free_toks(&tok);
    }
}

int main(int argc, char **argv) {
    char *builtin_str[] = {/*"pwd", "cd",*/ "help", "exit", "history", "env", "unset", "export", "which", "echo", "jobs", "fg", "test", NULL};
    int (*builtin_func[]) (t_info *info, t_process *p) = {/*&mx_ush_pwd
        , &mx_ush_cd, */&mx_ush_help, &mx_ush_exit, &mx_ush_history, &mx_ush_env, &mx_ush_unset, &mx_ush_export, &mx_ush_which, &mx_ush_echo, &mx_jobs, &mx_fg, &mx_test};
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
    mx_init_shell(info);
    run_shell(info);
    return 0;
}

static void mx_init_shell(t_info *info) {
    if (getenv("TERM"))
        mx_info_start(info);
    else {
        mx_error_message(TERM_ENV_NOT_EXIST);
        exit(EXIT_FAILURE);
    }
    mx_save_PATH(info, getenv("PATH"));
    info->PWD = getenv("PWD");
}
