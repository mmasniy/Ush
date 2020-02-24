#include "../../inc/ush.h"

static bool before_tilde(char *line, int pos) {
    if (pos == 0
        || (mx_strchr(TYPE, line[pos - 1])
            && (pos == 1 || line[pos - 2] != '\\')))
        return 1;
    return 0;
}

static void find_tilde_content(char *search_line, char **tilde_change) {
    char *passwd_data = mx_file_to_str("/etc/passwd");

    if (passwd_data) {
        char **splitted_lines = mx_strsplit(passwd_data, '\n');

        for (int i = 0; splitted_lines[i]; i++) {
            if (mx_str_head(splitted_lines[i], search_line) == 0) {
                char **final_line = mx_strsplit(splitted_lines[i], ':');

                *tilde_change = strdup(final_line[5]);
                mx_del_strarr(&final_line);
                break;
            }
        }
        mx_del_strarr(&splitted_lines);
        mx_strdel(&passwd_data);
    }
}

static bool read_tilde_content(char *line, int *pos, char **tilde_change) {
    DIR *f = NULL;
    char *check_line = NULL;
    int size = 1;

    for (int i = *pos + 1; line[i + size]; size++)
        if (line[i + size] == '/' || mx_strchr(TYPE, line[i + size]))
            break;
    check_line = strndup(&(line[*pos + 1]), size);
    // printf("check_line = %s\n", check_line);
    find_tilde_content(check_line, tilde_change);
    mx_strdel(&check_line);
    // printf("size = %d\n",size);
    *pos += size;
    return 0;
}

int mx_tilde_work(t_info *info, char **line, char *craft) {
    int pos = 0;
    char *new_line = mx_strnew(strlen(*line));
    char *tilde_change = NULL;

    for (int i = 0; (i = mx_get_char_index(&(craft[pos]), '~')) >= 0; pos++) {
        strncat(new_line, &(craft[pos]), i);
        pos += i;
        if (before_tilde(craft, pos)) {
            if (!craft[pos + 1] || mx_strchr(TYPE, craft[pos + 1]) || craft[pos + 1] == '/')
                tilde_change = strdup(getenv("HOME"));
            else if (craft[pos + 1] == '-' && ((!craft[pos + 2]) || mx_isspace(craft[pos + 2])) && (++pos))
                tilde_change = strdup(info->oldpwd);
            else if (craft[pos + 1] == '+' && ((!craft[pos + 2]) || mx_isspace(craft[pos + 2])) && (++pos))
                tilde_change = strdup(info->pwd);
            else {
                read_tilde_content(craft, &pos, &tilde_change);
            }
            if (tilde_change) {
                new_line = realloc(new_line, malloc_size(new_line) + strlen(tilde_change));
                mx_del_and_set(&new_line, mx_strjoin(new_line, tilde_change));
            }
        }
    }
    strcat(new_line, &(craft[pos]));
    free(*line);
    *line = strdup(new_line);
    mx_strdel(&new_line);
    return 0;
}

// end = '/', ' ', '\0'

// can be : a-z, A-Z, -, _, 0-9 and just like all sub (+).

// ~   = /Users/vkmetyk
// ~/  = /Users/vkmetyk/
// ~-  = old_pwd
// ~+  = this_folder
// ~file_in_Users_folder
// if (~file_in_Users_folder == not exist)
// print = u$h: no such user or named directory: file_in_Users_folder
