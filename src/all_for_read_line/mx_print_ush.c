#include "../../inc/ush.h"

void mx_print_ush(t_info *info) {
    // mx_printstr("\033[1;29m");
    mx_printstr("\033[1;32m");
    mx_printstr(USH);
    mx_printstr(RESET);
    if (info->PWD) {
    	mx_printstr("\033[1;29m");
    	// mx_printstr("\033[1;31m");
    	write(1, "(", 1);
    	mx_printstr(&(info->pwd[strlen(info->pwd) - (info->name_len - 7)]));
    	write(1, "): ", 3);
    	mx_printstr(RESET);
	}
}
