#include "../../inc/ush.h"

static bool check_final_flags(t_info *info, char flag, char **final_pwd) {
    bool return_value = 1;

    if (flag == 's') {
        char *tmp = mx_save_without_links(info, *final_pwd);

        if (strcmp(tmp, *final_pwd)) {
            mx_printerr("cd: not a directory: ");
            mx_printerr(*final_pwd);
            write(2, "\n", 1);
            return_value = 0;
        }
        mx_strdel(&tmp);
    }
    else if (flag == 'P') {
        mx_del_and_set(final_pwd, mx_save_without_links(info, *final_pwd));
    }
    return return_value;
}

static bool del_return(bool res, char **old_pwd_now
    , char **final_pwd, char **argument) {
    mx_strdel(old_pwd_now);
    mx_strdel(final_pwd);
    mx_strdel(argument);
    return res;
}

static bool change_all(char *argument, char **old_pwd_now, char **final_pwd) {
    if ((!argument || strcmp(argument, "--") == 0)) {
        if (!getenv("HOME"))
            return 1;
        mx_del_and_set(final_pwd, getenv("HOME"));
    }
    else if (strcmp(argument, "-") == 0) {
        char *tmp = strdup(argument);

        *final_pwd = strdup(argument);
        mx_del_and_set(final_pwd, strdup(*old_pwd_now));
        mx_del_and_set(old_pwd_now, strdup(tmp));
        mx_strdel(&tmp);
        printf("%s\n", *final_pwd);
    }
    else
        *final_pwd = strdup(argument);
    return 0;
}

int mx_ush_cd(t_info *i) {
    char flag = '\0';
    char *argument = NULL;
    char *old_pwd_now = strdup(i->oldpwd);
    char *final_pwd = NULL;
    int return_value;

    if (mx_check_cd_args(i, i->args, &flag, &argument)) {
        return_value = change_all(argument, &old_pwd_now, &final_pwd);
        if (check_final_flags(i, flag, &final_pwd) == 0)
            return del_return(1, &old_pwd_now, &final_pwd, &argument);
        if (chdir(final_pwd) >= 0) {
            mx_del_and_set(&(i->oldpwd), strdup(i->pwd));
            mx_del_and_set(&(i->pwd), strdup(final_pwd));
            setenv("PWD", i->pwd, 1);
            setenv("OLDPWD", i->oldpwd, 1);
        }
        return del_return(return_value, &old_pwd_now, &final_pwd, &argument);
    }
    else
        return del_return(1, &old_pwd_now, &final_pwd, &argument);
}
