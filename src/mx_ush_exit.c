#include "ush.h"

int mx_ush_exit(t_info *info, t_process *p) {
    (void)p;
    if (info->args[1]) {
        exit(atoi(info->args[1]));
    }
    exit(0);
}
