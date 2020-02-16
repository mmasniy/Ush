#include "../../inc/ush.h"

static bool before_tilde(char *line, int pos) {
    if (pos == 0 || (line[pos - 1] == ' ' && line[pos - 2] != '\\'))
        return 1;
    return 0;
}

static bool read_tilde_content(char *line, int *pos, char **tilde_change) {
    DIR *f = NULL;
    char *check_line = NULL;
    int size = 1;

    for (int i = *pos + 1; line[i + size]; size++)
        if (line[i + size] == ' ' || line[i + size] == '/')
            break;
    check_line = mx_strjoin("/Users/", strndup(&(line[*pos + 1]), size));
    printf("check_line = %s\n", check_line);
    if ((f = opendir(check_line))) {
        mx_del_and_set(tilde_change, check_line);
        closedir(f);
    }
    else {
        printf("I can't find = |%s|\n", check_line);
        return 1;
    }
    mx_strdel(&check_line);
    printf("size = %d\n",size);
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
            printf("+++++++++++++++\n");
            if (!craft[pos + 1] || craft[pos + 1] == ' ' || craft[pos + 1] == '/')
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
//     print = u$h: no such user or named directory: file_in_Users_folder


// static char *result_swap(char *search_line, int *len) {
//     char *result_line = "~";

//     if (!(search_line[1]) || mx_isspace(search_line[1])
//         || search_line[1] == '/') {
//         result_line = getenv("HOME");
//     }
//     else if (search_line[1] == '+' && (!(search_line[2])
//         || mx_isspace(search_line[2]))) {
//         result_line = getenv("PWD");
//         (*len)++;
//     }
//     return result_line;
// }

// void mx_tilde_search(char **line, char *craft) {
//     char *new_line = mx_strnew(strlen(*line));
//     char *tmp = NULL;
//     int len = 0;

//     for (int pos = -1; (pos = mx_get_char_index(craft, '~')) >= 0; len = 0) {
//         strncat(new_line, craft, pos);
//         if ((*line == &(craft[pos]) || mx_isspace(craft[pos - 1]))
//             && strcmp((tmp = result_swap(&(craft[pos]), &len)), "~")) {
//             new_line = realloc(new_line, malloc_size(new_line) + strlen(tmp));
//             strcat(new_line, tmp);
//             len++;
//         }
//         else
//             strncat(new_line, &(craft[pos]), (len = 1));
//         craft += pos + len;
//     }
//     strcat(new_line, craft);
//     free(*line);
//     *line = mx_strdup(new_line);
//     mx_strdel(&new_line);
// }
