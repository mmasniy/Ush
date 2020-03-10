#include "../inc/ush.h"

static void mx_init_shell(t_info *info) {
    int shlvl = getenv("SHLVL") ? atoi(getenv("SHLVL")) : -1;
    char *shlvl_char = shlvl == -1 ? "1\0" : mx_itoa(shlvl + 1);

    if (getenv("PATH") == NULL) {
        mx_push_export_back(&(info->variables), "PATH",
            "/usr/local/bin:/usr/bin:/bin:/usr/sbin");
    }
    setenv("SHLVL", shlvl_char, 1);
    mx_strdel(&shlvl_char);
    mx_info_start(info);
    mx_save_PATH(info);
    info->d = 0;
}

static void run_command(t_info *info, char **line) {
    t_tok *tok = NULL;

    if (line && *line && mx_parse_line(info, line) == 0) {
        if (mx_work_w_toks(*line, &tok, info)) {
            mx_tok_to_tree(tok, info);
        }
        if (tok)
            mx_free_toks(&tok);
    }
    mx_del_strarr(&(info->args));
    info->ctrl_d = 0;
    info->ctrl_c = 0;
    mx_strdel(line);
}

static bool check_open_type(t_info *info) {
    char *line = malloc(1);
    size_t linecap = 0;
    ssize_t linelen = 0;

    if (isatty(STDIN_FILENO)) {
        mx_strdel(&line);
        return true;
    }
    while ((linelen = getline(&line, &linecap, stdin)) > 0) {
        line[linelen] = '\0';
        if (line[linelen - 1] == '\n')
            line[linelen - 1] = '\0';
        run_command(info, &line);
        line = malloc(1);
        linecap = 0;
        linelen = 0;
    }
    mx_strdel(&line);
    return false;
}

static void run_shell(t_info *info) {
    if (check_open_type(info)) {
        char *line = NULL;

        while (1) {
            mx_custom_termios(info, STDIN_FILENO);
            line = mx_ush_read_line(info);
            mx_origin_termios(info, STDIN_FILENO);
            info->name_len = 0;
            mx_cntr_key(info);
            mx_check_history(info, line);
            run_command(info, &line);
        }
    }
}

int main(int argc, char **argv) {
    t_info *info = (t_info *)malloc(sizeof(t_info));

    (void)argc;
    (void)argv;
    memset(info, 0, sizeof(t_info));
    mx_init_shell(info);
    run_shell(info);
    return 0;
}
