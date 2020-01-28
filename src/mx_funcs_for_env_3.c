#include "ush.h"

static int save_to_env(t_export *env, char **args, int sign, int *flags);

int mx_check_variable_create(t_export *env, char **args, int pos, int *flags) {
    flags[3] = pos;
    for (int i = pos; args[i]; i++) {
        if (args[i][0] == '=') {
            mx_printerr("env: setenv ");
            mx_printerr(args[i]);
            mx_printerr(": Invalid argument\n");
            flags[0] = 1;
            return 0;
        }
    }
    return save_to_env(env, args, pos, flags);
}

static int save_to_env(t_export *env, char **args, int sign, int *flags) {
    t_export *check;
    char *key;
    char *value;

    for (int i = sign; args[i]; i++)
        if ((sign = mx_get_char_index(args[i], '=')) > 0 && !(flags[3] = 0)) {
            key = mx_strndup(args[i], sign);
            value = mx_strdup(&(args[i][sign + 1]));
            if ((check = mx_search_key_in_list(env, key))) {
                mx_strdel(&(check->value));
                check->value = mx_strdup(value);
            }
            else
                mx_push_export_back(&env, key, value);
            mx_strdel(&key);
            mx_strdel(&value);
        }
        else
            return (flags[3] = i);
    return flags[3];
}
