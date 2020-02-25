#include "../../inc/ush.h"

static void exec_command(t_info *info, char **sub_line, char quotes) {
    t_tok *tok = NULL;
    char *line_from_file = NULL;
    short fl = 0;

    info->file = 1;
    mx_work_w_toks(*sub_line, &tok);
    mx_tok_to_tree(tok, info);
    info->file = 0;
    mx_strdel(sub_line);
    mx_del_strarr(&info->args);
    mx_free_toks(&tok);
    line_from_file = mx_file_to_str(".system_ush.txt");
    if (line_from_file) {
        if (!quotes) {
            char *file_elements = strtok(line_from_file, "\n");

            *sub_line = mx_strnew(malloc_size(line_from_file));
            while (file_elements) {
                strcat(*sub_line, file_elements);
                if ((file_elements = strtok(NULL, "\n")))
                    strcat(*sub_line, " ");
            }
            mx_strdel(&file_elements);
        }
        else if (quotes == '\"')
            *sub_line = strndup(line_from_file, strlen(line_from_file) - 1);
    }
    remove(".system_ush.txt");
    mx_strdel(&line_from_file);
    // printf("\n\nSUB_LINE = %s\n\n", *sub_line);
}

static char check_quotes_before_recursion(char *line, int start, int end) {
    int pair_pos = 0;
    bool error = 0;

    for (int pos = 0; !error && line[pos]; pos++) {
        if (pos == start) {
            pos = start + end + 1;
        }
        if ((line[pos] == '\'' || line[pos] == '\"') && (pos == 0 || line[pos - 1] != '\\')) {
            if ((pair_pos = mx_char_block(&(line[pos + 1]), '\\', line[pos], '\0')) >= 0) {
                pair_pos += pos + 1;
                if (pair_pos > start + end)
                    return line[pos];
                else if (pair_pos > start && pair_pos < start + end) {
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
        fprintf(stderr, "Quote without pair\n");
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
    char quotes = info->quotes;

    // printf("===================\n");
    // printf("before = |%s|\n", before);
    // printf("sub_line = |%s|\n", sub_line);
    // printf("after = |%s|\n", after);
    // printf("quotes = |%c|\n", info->quotes);
    // printf("===================\n");
    del_slash_for_substitutions(&sub_line);
    mx_execute_substitutions(info, &sub_line);
    exec_command(info, &sub_line, quotes);
    tmp = mx_strjoin(before, sub_line);
    mx_strdel(line);
    *line = mx_strjoin(tmp, after);
    mx_strdel(&tmp);
    mx_strdel(&before);
    mx_strdel(&sub_line);
    mx_strdel(&after);
}

static bool find_start_finish(char *line, int *start, int *finish, char c) {
    if (c == '`') {
        if ((*start = mx_char_block(line, '\\', c, '\0')) >= 0) {
            if ((*finish = mx_char_block(&(line[*start + 1]), '\\', c, '\0')) >= 0)
                return 0;
            else
                return 1;
        }
    }
    else {
        int pos = 0;

        for (int i = -1; (i = mx_get_substr_index(&(line[pos]), "$(")) >= 0; ) {
            pos += i;
            if (pos == 0 || line[pos - 1] != '\\') {
                *start = pos;
                if ((*finish = mx_char_block(&(line[pos + 1]), '\\', ')', '\0')) >= 0) {
                    i = mx_get_substr_index(&(line[pos + 1]), "$(");
                    if (i >= 0 && i <= *start + *finish) {

                    }
                    return 0;
                }
                else
                    return 1;
            }
            pos++;
        }
    }
    return 0;
}

bool mx_execute_substitutions(t_info *info, char **line) {
   // printf("\n*line1 = %s\n", *line);
    int pos = 0;
    int tmp_start = -1;
    int tmp_finish = -1;
    int start = 0;
    int finish = -1;

    while ((start >= 0)) {
       // printf("\n\n*line2 = %s\n\n", *line);
        if (find_start_finish(*line, &start, &finish, '`')
            || find_start_finish(*line, &tmp_start, &tmp_finish, '$')) {
            fprintf(stderr, "Substitution without pair\n");
            return (info->status = 1);
        }
        // printf("start = %d\n", start);
        // printf("finish = %d\n", finish);
        // printf("tmp_start = %d\n", tmp_start);
        // printf("tmp_finish = %d\n", tmp_finish);
        if (tmp_start >= 0 && (tmp_start < start || start == -1)) {
            mx_replace_symbols_pack(line, tmp_start + tmp_finish + 1, 1, "`");
            mx_replace_symbols_pack(line, tmp_start, 2, "`");
            start = tmp_start;
            finish = tmp_finish - 1;
        }
        if (start == -1)
            break;
        if ((info->quotes = check_quotes_before_recursion(*line, pos + start, finish)) == 1)
            return (info->status = 1);
        if (info->quotes != '\'') {
            // printf("\nline = %s | %d, %d\n", *line, start + 1, finish);
            mx_recursion_substitutions(info, line, start + 1, finish);
        }
        tmp_start = tmp_finish = finish = -1;
    }
    // printf("\n\n*line3 = %s\n\n", *line);
    return 0;
}
