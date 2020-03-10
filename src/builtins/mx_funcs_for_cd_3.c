#include "../../inc/ush.h"

void mx_change_link_for_dots_in_cd(char **arg) {
    char *tmp = NULL;
    char *link = mx_strnew(1024);

    readlink(*arg, link, 1024);
    mx_find_last_slash(arg);
    tmp = mx_strjoin(*arg, "/");
    mx_del_and_set(arg, mx_strjoin(tmp, link));
    mx_strdel(&link);
    mx_strdel(&tmp);
}

bool mx_cd_error(char *arg) {
    char *error = mx_strjoin("cd: ", arg);

    perror(error);
    mx_strdel(&error);
    return 0;
}

int mx_check_cd_flags(t_info *info
    , char *find_flag, int i, char **argument) {
    for (int j = 1; info->args[i][j]; j++) {
        if (info->args[i][j] == 'P')
            *find_flag = 'P';
        else if (info->args[i][j] == 's') {
            if (*find_flag != 'P')
                *find_flag = 's';
        }
        else {
            mx_strdel(argument);
            *argument = strdup(info->args[i]);
            if (info->args[i + 1] && strcmp(*argument, "--"))
                return 0;
            return 2;
        }
    }
    return 1;
}
