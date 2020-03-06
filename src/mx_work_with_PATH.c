#include "../inc/ush.h"

static char *check_full_path(char **paths, char *word) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    char *path = NULL;

    if (mx_get_char_index(word, '/') >= 0)
        return strdup(word);
    for (int i = 0; paths[i]; i++) {
        if ((f = opendir(paths[i]))) {
            while ((d = readdir(f))) {
                if (mx_strcmp(d->d_name, word) == 0) {
                    path = mx_strjoin(paths[i], d->d_name);
                    closedir(f);
                    return path;
                }
            }
        }
    }
    return path;
}

static char *get_path(t_info *info) {
    if (info->variables)
        for (t_export *tmp = info->variables; tmp; tmp = tmp->next)
            if (tmp->key && strcmp(tmp->key, "PATH") == 0)
                return tmp->value;
    return NULL;
}

void mx_save_PATH(t_info *info) {
    if (info->paths)
        mx_del_strarr(&(info->paths));
    if (get_path(info)) {
        char **splitted_paths = mx_strsplit(get_path(info), ':');
        char *tmp;

        for (int i = 0; splitted_paths[i]; i++) {
            tmp = mx_strjoin(splitted_paths[i], "/");
            if (malloc_size(splitted_paths[i]))
                free(splitted_paths[i]);
            splitted_paths[i] = mx_strdup(tmp);
            free(tmp);
        }
        if (malloc_size(info->paths))
            mx_del_strarr(&(info->paths));
        info->paths = mx_arr_copy(splitted_paths);
        mx_del_strarr(&splitted_paths);
    }
}

char *mx_find_in_PATH(char **paths, char *word, bool full) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    char *all_paths = NULL;
    char *tmp;

    if (full)
        return check_full_path(paths, word);
    else
        for (int i = 0; paths[i]; i++)
            if ((f = opendir(paths[i]))) {
                while ((d = readdir(f)))
                    if (strcmp(d->d_name, ".") && strcmp(d->d_name, "..")
                        && mx_str_head(d->d_name, word) == 0) {
                        tmp = mx_strjoin(all_paths, ":");
                        mx_del_and_set(&all_paths, mx_strjoin(tmp, d->d_name));
                        mx_strdel(&tmp);
                    }
                closedir(f);
            }
    return all_paths;
}
