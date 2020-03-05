#include "../inc/ush.h"

static void mx_init_shell(t_info *info) {
    if (getenv("TERM"))
        mx_info_start(info);
    else {
        mx_error_message(TERM_ENV_NOT_EXIST);
        exit(EXIT_FAILURE);
    }
    if (getenv("PATH") == NULL) {
        mx_push_export_back(&(info->variables), "PATH"
            , "/usr/local/bin:/usr/bin:/bin:/usr/sbin");
    }
    mx_save_PATH(info, getenv("PATH"));
    info->PWD = getenv("PWD");
    info->d = 0;
}

static void run_command(t_info *info, char **line) {
    t_tok *tok = NULL;

    info->status = 0;
    if (mx_parse_line(info, line) == 0) {
        if (mx_work_w_toks(*line, &tok, info))
            mx_tok_to_tree(tok, info);
        if (tok)
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
    }
    return false;
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

void run_shell(t_info *info) {
    if (check_open_type(info)) {
        char *line = NULL;

        while (1) {
            mx_custom_termios(info, STDIN_FILENO);
            line = mx_ush_read_line(info);
            mx_origin_termios(info, STDIN_FILENO);
            info->name_len = 0;
            mx_check_history(info, line);
            mx_cntr_key(info);
            run_command(info, &line);
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
