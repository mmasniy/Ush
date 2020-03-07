#include "../../inc/ush.h"

static bool file_check(t_info *info, char *arg, char **path) {
    char *binary_file = NULL;

    if (path[0]) {
        binary_file = mx_find_in_PATH(path, arg, 1);
        if (binary_file) {
            free(binary_file);
            return 1;
        }
    }
    else {
        if ((binary_file = mx_find_in_PATH(info->paths, arg, 1))) {
            free(binary_file);
            return 1;
        }
    }
    return 0;
}

int mx_check_to_execute(t_info *info, char **path, int position) {
    int res = 0;
    char *tmp = mx_strjoin(*path, info->args[position]);
    char **path_check = mx_create_strarr(1);
    char *find = getenv("PATH");

    if (*path) {
        path_check[0] = *path;
        if (file_check(info, info->args[position], path_check)) {
            free(*path);
            *path = mx_strdup(tmp);
            res = 1;
        }
    }
    else
        if (find && file_check(info, info->args[position], info->paths))
            res = 2;
    free(tmp);
    free(path_check);
    return res;
}

t_export *mx_save_env_as_list(char **environ) {
    char *key;
    char *value;
    t_export *new_env_list = NULL;
    int j = -1;

    for (int i = 0; environ[i]; i++, j = -1) {
        if ((j = mx_get_char_index(environ[i], '=')) >= 0)
            key = mx_strndup(environ[i], j);
        if (j >= 0 && environ[i][j + 1])
            value = mx_strdup(&(environ[i][j + 1]));
        else
            value = NULL;
        mx_push_export_back(&new_env_list, key, value);
        if (malloc_size(key))
            free(key);
        if (malloc_size(value))
            free(value);
    }
    return new_env_list;
}

char **mx_save_env_as_massive(t_export *env) {
    int size = 0;
    char **env_massive = NULL;
    char *tmp_line;
    int i = 0;

    for (t_export *tmp = env; tmp; tmp = tmp->next)
        size++;
    env_massive = (char**)malloc(sizeof(char *) * (size + 1));
    for (t_export *tmp = env; tmp; tmp = tmp->next, i++) {
        tmp_line = mx_strjoin(tmp->key, "=");
        env_massive[i] = mx_strjoin(tmp_line, tmp->value);
        if (malloc_size(tmp_line))
            free(tmp_line);
    }
    env_massive[size] = NULL;
    return env_massive;
}
