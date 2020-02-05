#include "ush.h"

int mx_print_error(char *error) {
    if (error) {
        mx_printerr("u$h: syntax error ");
        mx_printerr(error);
    }
    else
        mx_printerr("u$h: parse error");
    mx_printerr("\n");
    return -1;
}
