#include "../../inc/ush.h"

int mx_ush_exit(t_info *info) {
    mx_save_all_history(info);
    if (info->args[1]) {
        exit(atoi(info->args[1]));
    }
    exit(0);
}
