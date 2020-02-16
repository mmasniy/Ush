#include "../inc/ush.h"

static void mx_init_shell(t_info *info);

void run_shell(t_info *info) {
    char *line;
    t_tok *tok = NULL;

    while (1) {
        line = mx_ush_read_line(info);
        mx_check_history(info, line);
        if (info->ctrl_c)
            exit(0);
        mx_parse_line(info, &line);
        mx_work_w_toks(line, &tok);
        mx_tok_to_tree(tok, info);
        info->args = mx_strsplit(line, ' ');
        // if (mx_strlen(line) > 0) {
        //     t_job *new_job = (t_job *) malloc(sizeof(t_job)); //create new job
        //     new_job = mx_create_job(info, info->args);
        //     status = mx_ush_execute(info, new_job);
        // }
        if (malloc_size(line))
            free(line);
        mx_del_strarr(&info->args);
        info->ctrl_d = 0;
        info->ctrl_c = 0;
        mx_free_toks(&tok);
        info->status = 0;
    }
}

int main(int argc, char **argv) {
    t_info *info = (t_info *)malloc(sizeof(t_info));

    (void)argc;
    (void)argv;
    // setvbuf(stdout, NULL, _IONBF, 0);
    if (!isatty(STDIN_FILENO)) {
        mx_error_message("You must use it just in terminal");
        exit(EXIT_FAILURE);
    }
    memset(info, 0, sizeof(t_info));
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
