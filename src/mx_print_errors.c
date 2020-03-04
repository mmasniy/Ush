#include "../inc/ush.h"

void mx_print_error(t_info *i, char *error, char *arg) {
    if (error) {
        mx_printerr("u$h: ");
        mx_printerr(arg);
        mx_printerr(error);
    }
    mx_printerr("\n");
    exit(1);
    i->status = 1;
}

void mx_print_red_err(int i, t_info *info) {
    if (i == 0) {
        mx_printerr(MX_DOT);
    }
    else {
        mx_printerr(MX_RED);
        mx_printerr(MX_DOP_RED);
    }
    info->status = 1;
}
