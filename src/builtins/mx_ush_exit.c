#include "../../inc/ush.h"

int mx_ush_exit(t_info *info) {
    if (info->args[1]) {
        exit(atoi(info->args[1]));
    }
    exit(0);
}
