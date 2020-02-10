#include "../../inc/ush.h"

bool mx_cd_error(char *arg, short error_type) {
    if (error_type == 0)
        mx_printerr("cd: string not in pwd: ");
    else if (error_type == 1)
        mx_printerr("cd: not a directory: ");
    else if (error_type == 2)
        mx_printerr("cd: no such file or directory: ");
    mx_printerr(arg);
    write(2, "\n", 1);
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
            break;
        }
    }
    return 1;
}
