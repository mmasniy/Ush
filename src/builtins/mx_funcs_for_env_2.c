#include "../../inc/ush.h"

static bool del_el(t_export **env, char **args, int i, int pos) {
    if (!pos)
        return 0;
    if (pos == -1 && (++i))
        pos = 0;
    if (mx_strcmp(&(args[i][pos]), (*env)->key) == 0)
        mx_del_env_elem(env);
    else
        for (t_export *tmp = (*env); tmp->next; tmp = tmp->next)
            if (mx_strcmp(&(args[i][pos]), tmp->next->key) == 0) {
                t_export *tmp2 = tmp->next;

                tmp->next = tmp->next->next;
                mx_strdel(&(tmp2->key));
                mx_strdel(&(tmp2->value));
                tmp2->next = NULL;
                free(tmp2);
                break;
            }
    return 1;
}

static bool print_error_flag(int *flags, char symbol, int flag_id, char *arg) {
    if (flag_id < 4) {
        mx_printerr("env: option requires an argument -- ");
        write(2, &symbol, 1);
    }
    else {
        mx_printerr("env: illegal option -- ");
        mx_printerr(arg); 
    }
    mx_printerr("\n");
    mx_printerr("usage: env [-iv] [-P utilpath] [-u name]\n");
    mx_printerr("[name=value ...] [utility [argument ...]]\n");
    flags[flag_id] = 1;
    return 0;
}

static int check_flag_u(int *index, int i, char **args, int *flags) {
    if (args[*index][i + 1]) {
        return i;
    }
    else if (args[*index + 1]) {
        (*index)++;
        return -1;
    }
    else {
        return print_error_flag(flags, 'u', 2, NULL);
    }
}

static int check_flags(int *index, char **args, int *flags, char **path) {
    for (int i = 1; args[*index][i]; i++) {
        if (args[*index][i] == '-' || args[*index][i] == 'i')
            flags[0] = 1;
        else if (args[*index][i] == 'P') {
            mx_strdel(path);
            if (args[*index][i + 1])
                *path = mx_strjoin(&(args[*index][i]), "/");
            else if (args[*index + 1] && ((*index)++))
                *path = mx_strjoin(args[*index], "/");
            else
                return print_error_flag(flags, 'P', 1, NULL);
            if (*path)
                break;
        }
        else if (args[*index][i] == 'u')
            return check_flag_u(index, i, args, flags);
        else
            return print_error_flag(flags, 0, 4, &(args[*index][i]));
    }
    return -10;
}

bool mx_check_args(t_export **env, char **args, int *flags, char **path) {
    bool flag_check = 1;

    for (int i = 1; args[i]; i++)
        if (args[i][0] == '-' && flag_check) {
            if (!strcmp(args[i], "--") && !(flag_check = 0))
                continue;
            if (!strcmp(args[i], "-") && (flags[0] = 1))
                while (*env)
                    mx_pop_export_front(env);
            else {
                if (!del_el(env, args, i, check_flags(&i, args, flags, path)))
                    return 0;
                else if (flags[0])
                    while (*env)
                        mx_pop_export_front(env);
            }
        }
        else
            return mx_check_variable_create(env, args, i, flags);
    return 0;
}
