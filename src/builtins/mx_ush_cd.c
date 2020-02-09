#include "../../inc/ush.h"

static bool check_final_flags(char flag, char **final_pwd);
static bool del_return(bool res, char **old_pwd_now
    , char **final_pwd, char **argument);

int mx_ush_cd(t_info *info) {
    char flag = '\0';
    char *argument = NULL;
    char *old_pwd_now = strdup(info->oldpwd);
    char *final_pwd = NULL;
    char *tmp = NULL;

    if (mx_check_cd_args(info, info->args, &flag, &argument)) {
        if ((!argument || strcmp(argument, "--") == 0)) {
            if (!getenv("HOME"))
                return del_return(1, &old_pwd_now, &final_pwd, &argument);
            mx_del_and_set(&final_pwd, getenv("HOME"));
        }
        else if (strcmp(argument, "-") == 0) {
            tmp = strdup(final_pwd);
            mx_del_and_set(&final_pwd, strdup(old_pwd_now));
            mx_del_and_set(&old_pwd_now, strdup(tmp));
            mx_strdel(&tmp);
        }
        else {
            mx_del_and_set(&final_pwd, strdup(argument)); // just argument
        }
        if (!check_final_flags(flag, &final_pwd))
            return del_return(1, &old_pwd_now, &final_pwd, &argument);
        mx_del_and_set(&(info->oldpwd), strdup(info->pwd));
        mx_del_and_set(&(info->pwd), strdup(final_pwd));
        setenv("PWD", info->pwd, 1);
        setenv("OLDPWD", info->oldpwd, 1);
        printf("getenv(PWD) = %s\n", getenv("PWD"));
        printf("getenv(OLDPWD) = %s\n", getenv("OLDPWD"));
        return del_return(0, &old_pwd_now, &final_pwd, &argument);
    }
    else
        return del_return(1, &old_pwd_now, &final_pwd, &argument);
}

static bool check_final_flags(char flag, char **final_pwd) {
    bool return_value = 1;
    if (flag == 's') {
        char *tmp = mx_save_without_links(*final_pwd);

        if (strcmp(tmp, *final_pwd)) {
            mx_printerr("cd: not a directory: ");
            mx_printerr(*final_pwd);
            write(2, "\n", 1);
            return_value = 0;
        }
        mx_strdel(&tmp);
    }
    else if (flag == 'P')
        mx_del_and_set(final_pwd, mx_save_without_links(*final_pwd));
    return return_value;
}

static bool del_return(bool res, char **old_pwd_now
    , char **final_pwd, char **argument) {
    mx_strdel(old_pwd_now);
    mx_strdel(final_pwd);
    mx_strdel(argument);
    return res;
}
