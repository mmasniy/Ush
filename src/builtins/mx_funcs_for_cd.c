#include "../../inc/ush.h"

static void parse_argument(t_info *info, char **arg, char flag) {
    char *new_arg = NULL;
    char **split = mx_strsplit(*arg, '/');

    if ((*arg)[0] == '/')
        mx_del_and_set(&new_arg, "/");
    for (int i = 0; split[i]; i++) {
        if (strcmp(split[i], ".") == 0)
            mx_dots_for_path(info, &new_arg, flag, 0);
        else if (strcmp(split[i], "..") == 0)
            mx_dots_for_path(info, &new_arg, flag, 1);
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
    char *tmp = mx_strjoin(info->pwd, "/");
    char *tmp2 = mx_strjoin(tmp, *arg);

    
    if ((f = opendir(tmp2))) {
        mx_del_and_set(&tmp, mx_strjoin(info->pwd, "/"));
        mx_del_and_set(arg, mx_strjoin(tmp, *arg));
        res = 1;
    }
    else if ((*arg)[0] == '/' && (f = opendir(*arg)))
        res = 1;
    mx_strdel(&tmp);
    mx_strdel(&tmp2);
    f ? closedir(f) : 0;
    if (res)
        parse_argument(info, arg, flag);
    return res;
}

static bool check_argument(t_info *info, char **arg, char *flag) {
    if (strcmp(*arg, "-") == 0 || strcmp(*arg, "--") == 0)
        return 1;
    else if (find_argument(info, arg, *flag)) {
        char *path_without_links = mx_save_without_links(info, *arg);

        if (*flag == 's') {
            if (strcmp(path_without_links, *arg)) {
                mx_strdel(&path_without_links);
                return 1;
            }
        }
        mx_strdel(&path_without_links);
        return 1;
    }
    return mx_cd_error(*arg);
}

bool mx_check_cd_args(t_info *info, char **args, char *flag, char **argument) {
    char find_flag = 0;
    bool flag_check = 1;
    int res;

    for (int i = 1; args[i]; i++)
        if (flag_check && args[i][0] == '-' && strcmp(args[i], "-")) {
            if (!(res = mx_check_cd_flags(info, &find_flag, i, argument)))
                return mx_cd_error(args[i]);
            flag_check = res == 2 ? 0 : 1;
            *flag = find_flag;
        }
        else {
            mx_strdel(argument);
            *argument = strdup(args[i]);
            if (args[i + 1])
                return mx_cd_error(args[i]);
        }
    if (!(*argument))
        return 1;
    return check_argument(info, argument, flag);
}
