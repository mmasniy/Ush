#include "../../inc/ush.h"

int mx_ush_set(t_info *info) {
    if (info->variables) {
        mx_sort_print(&(info->variables));
        for (t_export *tmp = info->variables; tmp; tmp = tmp->next)
            if (tmp->key)
                printf("%s=%s\n", tmp->key, tmp->value ? tmp->value : "");
    }
    return 0;
}
