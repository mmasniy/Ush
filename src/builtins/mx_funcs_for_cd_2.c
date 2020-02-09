#include "../../inc/ush.h"

static void save_link_path(char **res_line);

void mx_dots_for_path(char **arg, char flag, bool up) {
    if (flag == 'P') {
        
        for (int pos = strlen(*arg); pos > 0; pos--) {
            if ((*arg)[pos] == '/') {
                (*arg)[pos] = '\0';
                if (readlink(*arg, NULL, 0) >= 0) {
                    char *tmp = NULL;
                    char *link = mx_strnew(1024);
                    printf("*arg = %s\n", *arg);
                    readlink(*arg, link, 1024);
                    mx_find_last_slash(arg);
                    printf("*arg = %s\n", *arg);
                    tmp = mx_strjoin(*arg, "/");
                    printf("tmp = %s\n", tmp);
                    mx_del_and_set(arg, mx_strjoin(tmp
                        , link));
                    mx_strdel(&link);
                    mx_strdel(&tmp);
                    break;
                }
                else
                    (*arg)[pos] = '/';
            }
        }
    }
    if (up)
        mx_find_last_slash(arg);
}

char *mx_del_and_set(char **str, char *new_str) {
    mx_strdel(str);
    if (new_str)
        *str = strdup(new_str);
    mx_strdel(&new_str);
    return *str;
}

void mx_find_last_slash(char **str) {
    int pos = 0;
    char *craft = *str;
    char *new_str = NULL;

    for (int i = strlen(craft) - 1; i > 1 && craft[i] == '/'; ) {
        craft[i] = '\0';
    }
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

char *mx_save_without_links(char *path) {
    char *res_line = mx_strnew(strlen(path));
    char **steps = NULL;

    if ((steps = mx_strsplit(path, '/'))) {
        if (path[0] == '/')
            res_line[0] = '/';
        for (int i = 0; steps[i]; i++) {
            mx_del_and_set(&res_line, mx_strjoin(res_line, steps[i]));
            if (readlink(res_line, NULL, 0) >= 0) {
                save_link_path(&res_line);
            }
            mx_del_and_set(&res_line, mx_strjoin(res_line, "/"));
        }
        if (path[strlen(path) - 1] != '/')
            res_line[strlen(res_line) - 1] = '\0';
    }
    mx_del_strarr(&steps);
    return res_line;
}

static void save_link_path(char **res_line) {
    char *link_path = mx_strnew(1000);

    readlink(*res_line, link_path, 1000);
    mx_strdel(res_line);
    if (link_path && link_path[0] == '.'
        && (link_path[1] == '/' || !link_path[1])) {
        char *cwd_path = getcwd(NULL, 0);

        *res_line = mx_strjoin(cwd_path, &(link_path[1]));
        mx_strdel(&cwd_path);
    }
    else
        *res_line = strdup(link_path);
    mx_strdel(&link_path);
}
