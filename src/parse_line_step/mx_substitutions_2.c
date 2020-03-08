#include "../../inc/ush.h"

static char find(char *line, int *pair_pos, int start, int end) {
    if ((*pair_pos = mx_char_block(&(line[1]),
        '\\', *line, '\0')) >= 0) {
        *pair_pos += 1;
        if (*pair_pos > start + end)
            return *line;
        else if (*pair_pos > start && *pair_pos < start + end) {
            if (mx_char_block(&(line[*pair_pos + 1]), '\\',
                *line, '\0') <= start + end
                && mx_char_block(&(line[start + end]),
                '\\', *line, '\0') >= 0)
                return *line;
            return 1;
        }
        else
            return 0;
    }
    return 1;
}

static void save_result(char **sub_line, char **line_from_file, char quotes) {
    if (*line_from_file) {
        if (!quotes) {
            char *file_elements = strtok(*line_from_file, "\n");

            *sub_line = mx_strnew(malloc_size(*line_from_file));
            while (file_elements) {
                strcat(*sub_line, file_elements);
                if ((file_elements = strtok(NULL, "\n")))
                    strcat(*sub_line, " ");
            }
            mx_strdel(&file_elements);
        }
        else if (quotes == '\"')
            *sub_line = strdup(*line_from_file);
    }
    mx_strdel(line_from_file);
}

void mx_exec_substitutions_command(t_info *info
    , char **sub_line, char quotes) {
    t_tok *tok = NULL;
    char *line_from_file = NULL;
    short fl = 0;
    int status = info->status;

    info->file = 1;
    if (mx_work_w_toks(*sub_line, &tok, info))
        mx_tok_to_tree(tok, info);
    info->status = status == 0 ? 0 : info->status;
    info->file = 0;
    mx_strdel(sub_line);
    mx_del_strarr(&info->args);
    mx_free_toks(&tok);
    line_from_file = mx_file_to_str("/tmp/.system_ush.txt");
    save_result(sub_line, &line_from_file, quotes);
    remove("/tmp/.system_ush.txt");
}

char mx_check_quotes(char *line, int start, int end) {
    int pair = 0;
    bool error = 0;
    char res = 0;

    for (int pos = 0; !error && line[pos]; pos++) {
        if (pos == start)
            pos = start + end + 1;
        if ((line[pos] == '\'' || line[pos] == '\"')
            && (pos == 0 || line[pos - 1] != '\\')) {
            if ((res = find(line + pos, &pair, start - pos, end - pos)) == 1)
                error = 1;
            else if (res)
                return res;
        }
    }
    if (error) {
        fprintf(stderr, "Quote without pair\n");
        return 1;
    }
    return '\0';
}
