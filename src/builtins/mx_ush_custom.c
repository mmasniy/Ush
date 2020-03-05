#include "../../inc/ush.h"

static bool parse_and_check_line(t_info *info, char **line, int *type) {
    if (!(info->args[1]) || strlen(info->args[1]) == 0
        || (info->args[2] && strlen(info->args[2]) > 0))
        return 1;
    for (int i = 0; info->args[1][i]; i++) {
        if (!mx_isdigit(info->args[1][i]))
            *line = strdup(info->args[1]);
    }
    if (!(*line)) {
        *type = atoi(info->args[1]);
        if (!(*type >= 0 && *type <= 5)) {
            fprintf(stderr, "Color type can be just from 0 to 5.\n");
            return 1;
        }
    }
    return 0;
}

static bool change_view(t_info *info, char *line) {
    if (line && strcmp(line, "default") == 0) {
        mx_strdel(&(info->color_name));
        mx_del_and_set(&(info->name), USH);
    }
    else if (line && strcmp(line, "help") == 0) {
        printf("You can set special color (0-5) or custom name.\n");
        printf("your entered line, or keyword like: default, unicorn).\n");
    }
    else if (line)
        mx_del_and_set(&(info->name), strdup(line));
    else
        return 1;
    return 0;
}

int mx_ush_custom(t_info *info) {
    int type = -1;
    char *line = NULL;

    if (parse_and_check_line(info, &line, &type))
        return 1;
    if (change_view(info, line)) {
        if (type >= 0) {
            type == 0 ? mx_strdel(&(info->color_name)) : 0;
            type == 1 ? mx_del_and_set(&(info->color_name), "\033[1;34m") : 0;
            type == 2 ? mx_del_and_set(&(info->color_name), "\033[1;33m") : 0;
            type == 3 ? mx_del_and_set(&(info->color_name), "\033[1;32m") : 0;
            type == 4 ? mx_del_and_set(&(info->color_name), "\033[1;31m") : 0;
            type == 5 ? mx_del_and_set(&(info->color_name), "\033[1;30m") : 0;
        }
        else
            return 1;
    }
    line ? info->def_name = 0 : 0;
    mx_strdel(&line);
    return 0;
}
