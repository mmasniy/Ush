#include "../inc/ush.h"

void mx_print_error(t_info *i, char *error, char *arg) {
    if (error) {
        mx_printerr("u$h: ");
        mx_printerr(arg);
        mx_printerr(error);
    }
    mx_printerr("\n");
    i->status = 1;
    // exit(1);
}
