#include "../../inc/ush.h"

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

static bool find_start_finish(char *ln, int *srt, int *f, char c) {
    if (c == '`') {
        if ((*srt = mx_char_block(ln, '\\', c, '\0')) >= 0)
            return (*f = mx_char_block(&(ln[*srt + 1]), '\\',
                    c, '\0')) >= 0 ? 0 : 1;
    }
    else {
        int pos = 0;

        for (int i = -1; (i = mx_get_substr_index(&(ln[pos]), "$(")) >= 0; ) {
            pos += i;
            if (pos == 0 || ln[pos - 1] != '\\' && ((*srt = pos) || 1)) {
                mx_check_bracket(ln + pos + 2, f);
                *f >= 0 ? (*f)++ : 0;
                return *f >= 0 ? 0 : 1;
            }
            pos++;
        }
    }
    return 0;
}

/*
* printf("===================\n");
* printf("before = |%s|\n", before);
* printf("sub_line = |%s|\n", sub_line);
* printf("after = |%s|\n", after);
* printf("quotes = |%c|\n", info->quotes);
* printf("===================\n");
*/

static void recursion_substitutions(t_info *info, char **line,
                                    int start, int finish) {
    char *tmp = NULL;
    char *before = strndup(*line, start - 1);
    char *sub_line = strndup(&((*line)[start]), finish);
    char *after = strdup(&((*line)[start + finish + 1]));
    char quotes = info->quotes;

    del_slash_for_substitutions(&sub_line);
    mx_execute_substitutions(info, &sub_line);
    mx_exec_substitutions_command(info, &sub_line, quotes);
    tmp = mx_strjoin(before, sub_line);
    mx_del_and_set(line, mx_strjoin(tmp, after));
    mx_strdel(&tmp);
    mx_strdel(&before);
    mx_strdel(&sub_line);
    mx_strdel(&after);
    // for (int i = 0; (*line)[i]; i++)
    //     if ((*line)[i] >= 0 && (*line)[i] <= 127) {
    //         fprintf(stderr, "u$h> Not ASCII symbol in Substitution work!\n");
    //         break;
    //     }
}

static bool execute_substitutions_end(t_info *info, char **line,
                                      int start, int finish) {
    if ((info->quotes = mx_check_quotes(*line, start, finish)) == 1)
        return (info->status = 1);
    if (info->quotes != '\'')
        recursion_substitutions(info, line, start + 1, finish);
    return 0;
}

bool mx_execute_substitutions(t_info *info, char **line) {
    int num[4] = {-1, -1, 0, -1};

    while ((num[2]) >= 0 && ((num[0]) = (num[1]) = (num[3]) = -1) == -1) {
        if (find_start_finish(*line, &(num[2]), &(num[3]), '`')
            || find_start_finish(*line, &(num[0]), &(num[1]), '$')) {
            fprintf(stderr, "Substitution without pair\n");
            return (info->status = 1);
        }
        if ((num[0]) >= 0 && ((num[0]) < (num[2]) || (num[2]) == -1)) {
            mx_replace_symbols_pack(line, (num[0]) + (num[1]) + 1, 1, "`");
            mx_replace_symbols_pack(line, (num[0]), 2, "`");
            (num[2]) = (num[0]);
            (num[3]) = (num[1]) - 1;
        }
        if (num[2] == -1)
            break;
        if (execute_substitutions_end(info, line, (num[2]), (num[3])))
            return 1;
    }
    return 0;
}
