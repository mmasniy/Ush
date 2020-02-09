#include "../../inc/ush.h"

static bool cd_error(char *arg, short error_type);
static void parse_argument(char **arg, char flag);
static bool find_argument(t_info *info, char **arg, char flag);
static bool check_argument(t_info *info, char **arg, char *flag);

bool mx_check_cd_args(t_info *info, char **args, char *flag, char **argument) {
    char find_flag = 0;
    bool flag_check = 1;

    for (int i = 1; args[i]; i++) {
        if (flag_check && args[i][0] == '-' && strcmp(args[i], "-")) {
            for (int j = 1; args[i][j]; j++) {
                if (args[i][j] == 'P')
                    find_flag = 'P';
                else if (args[i][j] == 's') {
                    if (find_flag != 'P')
                        find_flag = 's';
                }
                else {
                    mx_strdel(argument);
                    *argument = strdup(args[i]);
                    if (args[i + 1] && strcmp(*argument, "--"))
                        return cd_error(args[i], 0);
                    flag_check = 0;
                    break;
                }
            }
            *flag = find_flag;
        }
        else {
            mx_strdel(argument);
            *argument = strdup(args[i]);
            if (args[i + 1])
                return cd_error(args[i], 0);
        }
    }
    if (!(*argument))
        return 1;
    return check_argument(info, argument, flag);
}

static bool cd_error(char *arg, short error_type) {
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

// mx_find_last_slash
static void parse_argument(char **arg, char flag) {
    char *new_arg = NULL;
    char **split = mx_strsplit(*arg, '/');

    if ((*arg)[0] == '/')
        mx_del_and_set(&new_arg, "/");
    for (int i = 0; split[i]; i++) {
        if (strcmp(split[i], ".") == 0)
            mx_dots_for_path(&new_arg, flag, 0);
        else if (strcmp(split[i], "..") == 0)
            mx_dots_for_path(&new_arg, flag, 1);
        else
            mx_del_and_set(&new_arg, mx_strjoin(new_arg, split[i]));
        mx_del_and_set(&new_arg, mx_strjoin(new_arg, "/"));
    }
    if (strlen(*arg) > 1 && (*arg)[strlen(*arg)] != '/')
        new_arg[strlen(new_arg) - 1] = '\0';
    mx_strdel(arg);
    *arg = strdup(new_arg);
    mx_strdel(&new_arg);
    mx_del_strarr(&split);
}

static bool find_argument(t_info *info, char **arg, char flag) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    bool res = 0;
    char *tmp = NULL;

    if ((*arg)[0] == '/' && (f = opendir(*arg))) {
        closedir(f);
        res = 1;
    }
    else if ((f = opendir(info->pwd))) {
        while ((d = readdir(f))) {
            if (mx_str_head(d->d_name, *arg) == 0) {
                if ((info->pwd)[strlen(info->pwd) - 1] != '/')
                    tmp = mx_strjoin("/", *arg);
                else
                    tmp = strdup(*arg);
                mx_del_and_set(arg, mx_strjoin(info->pwd, tmp));
                mx_strdel(&tmp);
                res = 1;
            }
        }
        closedir(f);
    }
    if (res)
        parse_argument(arg, flag);
    return res;
}

static bool check_argument(t_info *info, char **arg, char *flag) {
    if (strcmp(*arg, "-") == 0 || strcmp(*arg, "--") == 0) {
        return 1;
    }
    else if (find_argument(info, arg, *flag)) {
        char *path_without_links = mx_save_without_links(*arg);

        if (*flag == 's') {
            if (strcmp(path_without_links, *arg)) {
                mx_strdel(&path_without_links);
                return cd_error(*arg, 1);
            }
        }
        mx_strdel(&path_without_links);
        return 1;
    }
    return cd_error(*arg, 2);
}
