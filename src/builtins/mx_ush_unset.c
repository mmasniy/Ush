#include "../../inc/ush.h"

static void print_error(char *arg) {
    mx_printerr("u$h: unset: `");
    mx_printerr(arg);
    mx_printerr("': not a valid identifier\n");
}

static bool check_arg(char *arg) {
    if (!((arg[0] >= 65 && arg[0] <= 90) || (arg[0] >= 97 && arg[0] <= 122)
        || arg[0] == '_' || arg[0] == '$')) {
        print_error(arg);
        return 1;
    }
    for (int i = 0; arg[i]; i++) {
        if (!((arg[i] >= 48 && arg[i] <= 57) || (arg[i] >= 65 && arg[i] <= 90)
            || (arg[i] >= 97 && arg[i] <= 122)
            || arg[i] == '_' || arg[i] == '$')) {
            print_error(arg);
            return 1;
        }
    }
    return 0;
}

static void del_elem_form_list(t_export **list, char *key) {
    if ((*list)->next)
        for (t_export *tmp = *list; tmp->next; tmp = tmp->next) {
            if (strcmp(tmp->next->key, key) == 0) {
                t_export *tmp2 = tmp->next;

                tmp->next = tmp->next->next;
                mx_strdel(&(tmp2->key));
                mx_strdel(&(tmp2->value));
                tmp2->next = NULL;
                free(tmp2);
                break;
            }
        }
    else
        if (strcmp((*list)->key, key) == 0) {
            mx_strdel(&((*list)->key));
            mx_strdel(&((*list)->value));
            free(*list);
            *list = NULL;     
        }
}

int mx_ush_unset(t_info *info) {
    bool exit_code = 0;

    if (info->args[1]) {
        for (int i = 1; info->args[i]; i++) {
            if (check_arg(info->args[i]) == 0) {
                del_elem_form_list(&(info->to_export), info->args[i]);
                del_elem_form_list(&(info->variables), info->args[i]);
                unsetenv(info->args[i]);
            }
            else
                exit_code = 1;
        }
    }
    mx_save_PATH(info);
    return exit_code;
}
