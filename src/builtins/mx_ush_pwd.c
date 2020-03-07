#include "../../inc/ush.h"

static bool print_error(char *arg) {
    mx_printerr("pwd: bad option: -");
    write(2, arg, 1);
    write(2, "\n", 1);
    return 0;
}

static bool check_pwd_args(char **args, bool *flags) {
    bool flag_check = 1;

    for (int i = 1; args[i]; i++) {
        if (strcmp(args[i], "--") == 0)
            flag_check = 0;
        else if (flag_check && args[i][0] == '-')
            for (int j = 1; args[i][j]; j++) {
                if (args[i][j] == 'P')
                    flags[0] = 1;
                else if (args[i][j] == 'L')
                    flags[1] = 1;
                else
                    return print_error(&(args[i][j]));
            }
        else {
            mx_printerr("pwd: too many arguments\n");
            return 0;
        }
    }
    return 1;
}

int mx_ush_pwd(t_info *info) {
    bool flags[2] = {0, 0};

    if (check_pwd_args(info->args, flags)) {
        if ((!flags[0] && flags[1]) || (!flags[0] && !flags[1])) {
            mx_printstr(info->pwd);
        }
        else {
            char *cwd_path = getcwd(NULL, 0);

            mx_printstr(cwd_path);
            mx_strdel(&cwd_path);
        }
        write(1, "\n", 1);
    }
    else
        return 1;
    return 0;
}
