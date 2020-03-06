#include "../../inc/ush.h"

static void work_with_two_args(t_info *i) {
    unsigned char status;

    if (atoi(i->args[1]))
        status = (char)atoi(i->args[1]);
    else {
        mx_printerr("u$h: exit: ");
        mx_printerr(i->args[1]);
        mx_printerr(": numeric argument required\n");
        status = 255;
    }
    exit(status);
}

static void work_with_more_args(t_info *i) {
    if (atoi(i->args[1]) && i->args[2]) {
        mx_printerr("u$h: exit: too many arguments\n");
        return ;
    }
    else if (!atoi(i->args[1])) {
        mx_printerr("u$h: exit: ");
        mx_printerr(i->args[1]);
        mx_printerr(": numeric argument required\n");
        exit(255);
    }
}

int mx_ush_exit(t_info *i) {
    mx_save_all_history(i);
    if (mx_arr_size(i->args) == 1)
        exit(i->status);
    if (mx_arr_size(i->args) == 2)
        work_with_two_args(i);
    if (mx_arr_size(i->args) > 2)
        work_with_more_args(i);
    return 0;
}
