#include "../../inc/ush.h"

static void count_name_len(t_info *info);

void mx_print_ush(t_info *info, char *name) {
    count_name_len(info);
    mx_printstr("\033[1;32m");
    mx_printstr(name);
    mx_printstr(RESET);
    if (info->PWD) {
    	mx_printstr("\033[1;29m");
    	write(1, "(", 1);
    	mx_printstr(&(info->pwd[strlen(info->pwd) - (info->name_len - 7)]));
    	write(1, "): ", 3);
    	mx_printstr(RESET);
	}
}

static void count_name_len(t_info *info) {
    info->name_len = 3;
    if (info->pwd) {
        int i = strlen(info->pwd) - 1;

        info->name_len += 4;
        for (; info->pwd[i] && info->pwd[i] != '/'; i--);
        info->name_len += strlen(&(info->pwd[i]));
    }
}
