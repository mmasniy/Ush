#include "../../inc/ush.h"

static void exec_command(t_info *info, char **sub_line) {
    t_tok *tok = NULL;
    char *line_from_file = NULL;
    short fl = 0;

    info->file = 1;
    mx_work_w_toks(*sub_line, &tok);
    mx_tok_to_tree(tok, info);
    info->file = 0;
    mx_strdel(sub_line);
    line_from_file = mx_file_to_str(".system_ush.txt");
    if (line_from_file) {
        if (!info->quotes) {
            char *tok = strtok(line_from_file, "\n");

            *sub_line = mx_strnew(malloc_size(line_from_file));
            while (tok) {
                strcat(*sub_line, tok);
                if ((tok = strtok(NULL, "\n")))
                    strcat(*sub_line, " ");
            }
            mx_strdel(&tok);
        }
        else if (info->quotes == '\"')
            *sub_line = strndup(line_from_file, strlen(line_from_file) - 1);
    }
    if ((fl = open(".system_ush.txt", O_RDONLY))) {
        close(fl);
        remove(".system_ush.txt");
    }
    mx_del_and_set(sub_line, mx_replace_substr(*sub_line, " ", "Zu5Qmpnr"));
    mx_del_and_set(sub_line, mx_replace_substr(*sub_line, "\n", "QoIYem2"));
}

static char check_quotes_before_recursion(char *line, int start, int end) {
    int pair_pos = 0;
    bool error = 0;

    for (int pos = 0; !error && line[pos]; pos++) {
        if (pos == start)
            pos = start + end;
        if ((line[pos] == '\'' || line[pos] == '\"') && (pos == 0 || line[pos - 1] != '\\')) {
            if ((pair_pos = mx_char_block(&(line[pos + 1]), '\\', line[pos], '\0')) >= 0) {
                pair_pos += pos + 1;
                if (pair_pos > start + end)
                    return line[pos];
                else if (pair_pos > start && pair_pos < start + end) {
                    // printf("line[pos] = %s\n", &(line[pos]));
                    if (mx_char_block(&(line[pair_pos + 1]), '\\', line[pos], '\0') <= start + end
                        && mx_char_block(&(line[start + end]), '\\', line[pos], '\0') >= 0)
                        return line[pos];
                    error = 1;
                }
                else
                    continue;
            }
            error = 1;
        }
    }
    if (error) {
        // printf("OMG, WTF ???\n");
        return 1;
    }
    return '\0';
}

static void del_slash_for_substitutions(char **line) {
    char *new_line = mx_strnew(mx_strlen(*line));
    int pos = 0;
    int i = 0;

    for (; (pos = mx_get_char_index(&((*line)[i]), '\\')) >= 0; i++) {
        strncat(new_line, &((*line)[i]), pos);
        i += pos;
        if ((*line)[i + 1] == '\\' && strcat(new_line, "\\"))
            i++;
        else if ((*line)[i + 1] == '`' && strcat(new_line, "`"))
            i++;
        else
            strcat(new_line, "\\");
    }
    strcat(new_line, &((*line)[i]));
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}

void mx_recursion_substitutions(t_info *info, char **line
    , int start, int finish) {
    char *tmp = NULL;
    char *before = strndup(*line, start - 1);
    char *sub_line = strndup(&((*line)[start]), finish);
    char *after = strdup(&((*line)[start + finish + 1]));

    // printf("===================\n");
    // printf("before = |%s|\n", before);
    // printf("sub_line = |%s|\n", sub_line);
    // printf("after = |%s|\n", after);
    // printf("===================\n");
    del_slash_for_substitutions(&sub_line);
    mx_execute_substitutions(info, &sub_line);
    exec_command(info, &sub_line);
    // printf("line1 = %s\n", *line);
    // printf("line2 = %s\n", *line);
    // execute sub_line;
    tmp = mx_strjoin(before, sub_line);
    mx_strdel(line);
    *line = mx_strjoin(tmp, after);
    mx_strdel(&tmp);
    mx_strdel(&before);
    mx_strdel(&sub_line);
    mx_strdel(&after);
}

bool mx_execute_substitutions(t_info *info, char **line) {
   int pos = 0;

    for (int i = 0; (i = mx_char_block(&((*line)[pos])
        , '\\', '`', '\0')) >= 0; ) {
        pos += i + 1;
        if ((i = mx_char_block(&((*line)[pos])
        , '\\', '`', '\0')) >= 0) {
            if ((info->quotes = check_quotes_before_recursion(*line, pos, i)) == 1)
                return (info->status = 1);
            // printf("quotes = |%c|\n", info->quotes);
            if (info->quotes != '\'')
                mx_recursion_substitutions(info, line, pos, i);
            pos += i + 1;
        }
        else {
            // printf("I don't have a pair(((\n");
            return (info->status = 1);
        }
    }
    return 0;
}
