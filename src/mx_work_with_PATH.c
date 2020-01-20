#include "ush.h"

void mx_save_PATH(t_info *info, char *all_paths) {
    char **splitted_paths = mx_strsplit(all_paths, ':');
    char *tmp;

    for (int i = 0; splitted_paths[i]; i++) {
        tmp = mx_strjoin(splitted_paths[i], "/");
        if (malloc_size(splitted_paths[i]))
            free(splitted_paths[i]);
        splitted_paths[i] = mx_strdup(tmp);
        free(tmp);
    }
    info->paths = mx_arr_copy(splitted_paths);
    mx_del_strarr(&splitted_paths);
}

char *mx_find_in_PATH(t_info *info, char *word, bool full) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    char *all_paths = NULL;
    char *tmp;

    for (int i = 0; info->paths[i]; i++) {
        f = opendir(info->paths[i]);

        if (f) {
            while ((d = readdir(f)))
                if (full) {
                    if (mx_strcmp(d->d_name, word) == 0)
                        return mx_strjoin(d->d_name, word);
                }
                else
                    if (mx_str_head(d->d_name, word) == 0) {
                        tmp = mx_strjoin(all_paths, ":");
                        if (malloc_size(all_paths))
                            free(all_paths);
                        all_paths = mx_strjoin(tmp, d->d_name);
                    }
            closedir(f);
        }
    }
    return all_paths;
}
