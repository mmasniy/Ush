#include "../../inc/ush.h"

static bool is_link_in_path(t_info *i, char *check_line, char **final_line) {
    struct stat line_stat;

    lstat(check_line, &line_stat);
    if ((line_stat.st_mode & S_IFMT) == S_IFLNK) {
        char *test;

        chdir(check_line);
        check_line = realloc(check_line, 1024);
        test = getcwd(check_line, 1024);
        mx_del_and_set(final_line, mx_strjoin(test, *final_line));
        chdir(i->pwd);
        mx_strdel(&test);
        return 1;
    }
    return 0;
}

static void save_link_path(char **res_line) {
    char *link_path = mx_strnew(1024);

    readlink(*res_line, link_path, 1024);
    mx_strdel(res_line);
    if (link_path && link_path[0] == '.'
        && (link_path[1] == '/' || !link_path[1])) {
        char *cwd_path = getcwd(NULL, 0);

        *res_line = mx_strjoin(cwd_path, &(link_path[1]));
        mx_strdel(&cwd_path);
    }
    else {
        char *tmp = mx_up_to_one(*res_line);

        *res_line = tmp == NULL || strcmp(tmp, "/") == 0
        ? mx_strjoin("/", link_path) : strdup(link_path);
        mx_strdel(&tmp);
    }
    mx_strdel(&link_path);
}

void mx_dots_for_path(t_info *info, char **arg, char flag, bool up) {
    if (up) {
        if (flag == 'P')
            mx_del_and_set(arg, mx_save_without_links(info, *arg));
        mx_find_last_slash(arg);
    }
    else
        if (strlen(*arg) > 1 && (*arg)[strlen(*arg) - 1] == '/')
            (*arg)[strlen(*arg) - 1] = '\0';
}

void mx_find_last_slash(char **str) {
    int pos = 0;
    char *craft = *str;
    char *new_str = NULL;

    for (int i = strlen(craft) - 1; i > 1 && craft[i] == '/'; )
        craft[i] = '\0';
    for (; mx_get_char_index(&(craft[pos]), '/') >= 0; pos++);
    if (pos > 0) {
        pos = pos == 1 ? pos + 1 : pos;
        new_str = strndup(*str, pos - 1);
        mx_strdel(str);
        if (new_str) {
            *str = strdup(new_str);
            mx_strdel(&new_str);
        }
    }
    else
        mx_strdel(str);
}

char *mx_save_without_links(t_info *info, char *path) {
    int size = 0;
    char *slash = path + strlen(path) - 1;
    char *check_line = strdup(path);
    char *final_line = NULL;

    if (is_link_in_path(info, check_line, &final_line) == 0)
        while (slash) {
            slash = mx_memrchr(path, '/', slash - path - 1);
            if (slash - path == 0)
                mx_del_and_set(&check_line, strndup(path, 1));
            else
                mx_del_and_set(&check_line, strndup(path, slash - path));
            if (is_link_in_path(info, check_line, &final_line))
                break;
            else {
                mx_del_and_set(&final_line, strdup(slash));
            }
            if (slash - path <= 0)
                break;
        }
    mx_strdel(&check_line);
    return final_line;
}
