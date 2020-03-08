#include "../../inc/ush.h"

static bool print_results(t_history *paths, bool *flags) {
    if (!flags[1] && paths && paths->data) {
        for (t_history *tmp = paths; tmp; tmp = tmp->next) {
            printf("%s\n", tmp->data);
            if (!flags[0])
                break;
        }
    }
    if (paths && paths->data)
        return 0;
    return 1;
}

static void find_binary_files(t_info *info, char *word,
                              t_history **list, char *tmp) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    struct stat buff;

    for (int i = 0; info->paths[i]; i++)
        if ((f = opendir(info->paths[i]))) {
            while ((d = readdir(f)))
                if (strcmp(d->d_name, word) == 0) {
                    tmp = mx_strjoin(info->paths[i], d->d_name);
                    lstat(tmp, &buff);
                    if (buff.st_mode & S_IXOTH && !(buff.st_mode & S_ISTXT))
                        mx_push_history_back(list, tmp);
                    mx_strdel(&tmp);
                }
            closedir(f);
        }
}

static bool exec_which(t_info *info, int pos, bool *flags) {
    t_history *paths = NULL;
    bool result = 0;
    char *tmp;

    for (int i = pos; info->args[i]; i++) {
        if (mx_check_buildin(info, info->args[i], 0) >= 0) {
            tmp = mx_strjoin(info->args[i], ": shell built-in command");
            mx_push_history_back(&paths, tmp);
            mx_strdel(&tmp);
        }
        find_binary_files(info, info->args[i], &paths, tmp);
        if (print_results(paths, flags))
            result = 1;
        while (paths)
            mx_pop_history_front(&paths);
    }

    return result;
}

static int check_flags(char **args, bool *flags) {
    for (int i = 1; args[i]; i++) {
        if (args[i][0] == '-' && args[i][1]) {
            if (strcmp(args[i], "--") == 0)
                return args[i + 1] ? i + 1 : 0;
            for (int j = 1; args[i][j]; j++) {
                if ((args[i][j] == 'a' && (flags[0] = 1))
                    || (args[i][j] == 's' && (flags[1] = 1)));
                else {
                    fprintf(stderr, "which: illegal option -- %c\n"
                            , args[i][j]);
                    return 0;
                }
            }
            if (!(args[i + 1]))
                return 0;
        }
        else
            return i;
    }
    return 0;
}

int mx_ush_which(t_info *info) {
    if (info->args[1]) {
        int pos = 1;
        bool flags[2] = {0, 0};

        if ((pos = check_flags(info->args, flags)))
            return exec_which(info, pos, flags);
    }
    mx_printerr("usage: which [-as] program ...\n");
    return 1;
}
