#include "../inc/ush.h"

void mx_error_message(char *str) {
    mx_printerr(RED);
    mx_printerr(USH);
    mx_printerr(str);
    mx_printerr(RESET);
}
