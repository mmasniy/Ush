#include "../../inc/ush.h"

int mx_ush_true(t_info *info) {
    return 0;
}

int mx_ush_false(t_info *info) {
    return 1;
}

int mx_ush_return(t_info *info) {
    if (info->args[1])
        return atoi(info->args[1]);
    else
        return 0;
}
