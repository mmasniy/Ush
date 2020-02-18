#include "../inc/ush.h"

void mx_print_error(char *error, char *arg) {
    if (error) {
    	mx_printerr(RED);
    	mx_printerr("u$h: ");
    	mx_printerr(arg);
        mx_printerr(error);
        mx_printerr(RESET);
    }
    mx_printerr("\n");
    exit(1);
}
