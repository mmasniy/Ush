#include "../inc/ush.h"

static void mx_init_shell(t_info *info);

static void run_command(t_info *info, char **line) {
    t_tok *tok = NULL;

    info->status = 0;
    if (mx_parse_line(info, line) == 0) {
        if (mx_work_w_toks(*line, &tok))
            mx_tok_to_tree(tok, info);
        mx_free_toks(&tok);
    }
    mx_del_strarr(&(info->args));
    info->ctrl_d = 0;
    info->ctrl_c = 0;
    mx_strdel(line);
}

static bool check_open_type(t_info *info) {
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;

    if (isatty(STDIN_FILENO)) {
        return true;
    }
    while ((linelen = getline(&line, &linecap, stdin)) > 0) {
        line[linelen] = '\0';
        if (line[linelen - 1] == '\n')
            line[linelen - 1] = '\0';
        run_command(info, &line);
        // printf("i->status = %d\n", info->status);
    }
    return false;
}

void run_shell(t_info *info) {
    if (check_open_type(info)) {
        char *line = NULL;

        while (1) {
            line = mx_ush_read_line(info);
            info->name_len = 0;
            mx_check_history(info, line);
            if (info->ctrl_c) {
                mx_save_all_history(info);
                exit(0);
            }
            run_command(info, &line);
            // printf("i->status = %d\n", info->status);
        }
    }
}

int main(int argc, char **argv) {
    t_info *info = (t_info *)malloc(sizeof(t_info));

    (void)argc;
    (void)argv;
    // setvbuf(stdout, NULL, _IONBF, 0);
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
    if (getenv("PATH") == NULL) {
        char *path = mx_strjoin("/Users/vkmetyk/.brew/bin:/Users/vkmetyk/"
            , ".brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin");
        mx_del_and_set(&path, mx_strjoin(path, ":/sbin:/usr/local/munki"));
        setenv("PATH", path, 1);
        mx_push_export_back(&(info->variables), "PATH", path);
        mx_push_export_back(&(info->to_export), "PATH", path);
        mx_strdel(&path);
    }
    mx_save_PATH(info, getenv("PATH"));
    info->PWD = getenv("PWD");
}
