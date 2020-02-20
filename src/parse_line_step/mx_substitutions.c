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
    // printf(")((((((((((((((((((((\n");
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
    // printf("========================================\n");
    // printf("\nSUB_LINE = %s\n", *sub_line);
    // mx_del_and_set(sub_line, mx_replace_substr(*sub_line, " ", "\\ "));
    // printf("SUB_LINE = %s\n", *sub_line);
    // printf("========================================\n");
    // mx_del_and_set(sub_line, mx_replace_substr(*sub_line, "\n", "QoIYem2"));
}

static char check_quotes_before_recursion(char *line, int start, int end) {
    int pair_pos = 0;
    bool error = 0;

    for (int pos = 0; !error && line[pos]; pos++) {
        if (pos == start) {
            // printf("%s\n", "pos = start + end");
            // printf("%s\n", line + start + end);
            pos = start + end + 1;
        }
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
    // printf("quotes = |%c|\n", info->quotes);
    // printf("===================\n");
    del_slash_for_substitutions(&sub_line);
    // printf("+++++++++++++++++\n");
    mx_execute_substitutions(info, &sub_line, sub_line);
    // printf("-------------------\n");
    exec_command(info, &sub_line);
    // printf("*******************\n");
    // printf("line1 = %s\n", *line);
    // printf("line2 = %s\n", *line);
    // execute sub_line;
    tmp = mx_strjoin(before, sub_line);
    // printf("^^^^^^^^^^^^^^^^^^\n");
    mx_strdel(line);
    *line = mx_strjoin(tmp, after);
    // printf("########################\n");
    mx_strdel(&tmp);
    mx_strdel(&before);
    mx_strdel(&sub_line);
    mx_strdel(&after);
}
// echo $(echo "`ls`")

// 0
// Makefile
// inc
// libmx
// ls
// src
// ush"
// list:
// ---------------------------------------------
// [echo] ["0

// 0
// Makefile
// inc
// libmx
// ls
// src
// ush"]

// [0] [0]

// [10] [10]
// ---------------------------------------------

// ****************************
// ================
// tmp->token = echo
// tmp->token = echo
// ush(56599,0x116dbf5c0) malloc: Incorrect checksum for freed object 0x7ff045c04290: probably modified after being freed.
// Corrupt value: 0xa786d62696c0a63
// ush(56599,0x116dbf5c0) malloc: *** set a breakpoint in malloc_error_break to debug
// [1]    56599 abort      ./ush

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
            printf("\nline[pos] = %s\n", &(line[pos]));
            if (pos == 0 || line[pos - 1] != '\\') {
                *start = pos;
                if ((*finish = mx_char_block(&(line[pos + 1]), '\\', ')', '\0')) >= 0)
                    return 0;
                else
                    return 1;
            }
            pos++;
        }
    }
    return 0;
}

bool mx_execute_substitutions(t_info *info, char **line, char *craft) {
    if (craft) {}
    printf("\n*line1 = %s\n", *line);

    int pos = 0;
    int tmp_start = -1;
    int tmp_finish = -1;
    int start = 0;
    int finish = -1;

    while ((start >= 0)) {
        // printf("\n*line2 = %s\n", *line);
        if (find_start_finish(*line, &start, &finish, '`'))
            return (info->status = 1);
        // printf("start = %d\n", start);
        // printf("finish = %d\n", finish);
        if (find_start_finish(*line, &tmp_start, &tmp_finish, '$'))
            return (info->status = 1);
        // printf("tmp_start = %d\n", tmp_start);
        // printf("tmp_finish = %d\n", tmp_finish);
        if (tmp_start >= 0 && (tmp_start < start || start == -1)) {
            // printf("Replace $\n");
            // printf("start[pos] = %s\n", *line + pos + start);
            // printf("finish[pos] = %s\n", *line + pos + start + finish);
            // printf("tmp_start[pos] = %s\n", *line + pos + tmp_start);
            // printf("tmp_finish[pos] = %s\n", *line + pos + tmp_start + tmp_finish);
            mx_replace_symbols_pack(line, tmp_start + tmp_finish + 1, 1, "`");
            // printf("*line_replace1 = %s\n", *line);
            mx_replace_symbols_pack(line, tmp_start, 2, "`");
            // printf("*line_replace2 = %s\n", *line);
            start = tmp_start;
            finish = tmp_finish - 1;
            // printf("*line_replace = %s\n", *line);
            // printf("***start[pos] = %s\n", *line + pos + start);
            // printf("***finish[pos] = %s\n", *line + pos + start + finish);
        }
        if (start == -1)
            break;
        // printf("\n|%d %d|\n", pos + start, finish);
        if ((info->quotes = check_quotes_before_recursion(*line, pos + start, finish)) == 1)
            return (info->status = 1);
        if (info->quotes != '\'') {
            // printf("\nline = %s | %d, %d\n", *line, start + 1, finish);
            mx_recursion_substitutions(info, line, start + 1, finish);
            // printf("&&&&&&&&&&&&&&&&&&\n");
        }
        tmp_start = tmp_finish = finish = -1;
    }
    // printf("\n*line3 = %s\n", *line);
    return 0;
}

// bool mx_execute_substitutions(t_info *info, char **line, char *craft) {
//     printf("*line1 = %s\n", *line);
//    // int pos = 0;

//    //  for (int i = 0; (i = mx_char_block(&((*line)[pos])
//    //      , '\\', '`', '\0')) >= 0; ) {
//    //      pos += i + 1;
//    //      if ((i = mx_char_block(&((*line)[pos]), '\\', '`', '\0')) >= 0) {
//    //          if ((info->quotes = check_quotes_before_recursion(*line, pos, i)) == 1)
//    //              return (info->status = 1);
//    //          // printf("quotes = |%c|\n", info->quotes);
//    //          if (info->quotes != '\'')
//    //              mx_recursion_substitutions(info, line, pos, i);
//    //          pos += i + 1;
//    //      }
//    //      else {
//    //          // printf("I don't have a pair(((\n");
//    //          return (info->status = 1);
//    //      }
//    //  }
//     int pos = 0;
//     int tmp_start = -1;
//     int tmp_finish = -1;
//     int start = -1;
//     int finish = -1;

//     while (craft[pos]) {
//         printf("*line2 = %s | craft[pos] = %s\n", *line, craft + pos);
//         if (find_start_finish(&(craft[pos]), &start, &finish, '`'))
//             return (info->status = 1);
//         printf("start = %d\n", start);
//         printf("finish = %d\n", finish);
//         if (find_start_finish(&(craft[pos]), &tmp_start, &tmp_finish, '$'))
//             return (info->status = 1);
//         printf("tmp_start = %d\n", tmp_start);
//         printf("tmp_finish = %d\n", tmp_finish);
//         if (tmp_start >= 0 && (tmp_start < start || start == -1)) {
//             printf("Replace $\n");
//             // printf("start[pos] = %s\n", *line + pos + start);
//             // printf("finish[pos] = %s\n", *line + pos + start + finish);
//             // printf("tmp_start[pos] = %s\n", *line + pos + tmp_start);
//             // printf("tmp_finish[pos] = %s\n", *line + pos + tmp_start + tmp_finish);
//             mx_replace_symbols_pack(line, pos + tmp_start + tmp_finish + 1, 1, "`");
//             // printf("*line_replace1 = %s\n", *line);
//             mx_replace_symbols_pack(line, pos + tmp_start, 2, "`");
//             // printf("*line_replace2 = %s\n", *line);
//             start = tmp_start;
//             finish = tmp_finish - 1;
//             // printf("*line_replace = %s\n", *line);
//             // printf("***start[pos] = %s\n", *line + pos + start);
//             // printf("***finish[pos] = %s\n", *line + pos + start + finish);
//         }
//         if (start == -1)
//             break;
//         printf("|%d %d|\n", pos + start, finish);
//         if ((info->quotes = check_quotes_before_recursion(*line, pos + start, finish)) == 1)
//             return (info->status = 1);
//         pos += start + 1;
//         if (info->quotes != '\'') {
//             mx_recursion_substitutions(info, line, pos, finish);
//             pos += finish + 1;
//         }
//     }
//     printf("*line3 = %s\n", *line);
//     return 0;
// }
