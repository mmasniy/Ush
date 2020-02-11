#include "../../inc/ush.h"

static void count_name_len(t_info *info);

void mx_print_ush(t_info *info) {
    count_name_len(info);
    write(1, "\r", 1);
    mx_print_unicode(129412);
    mx_printstr(" \033[1;35m");
    mx_printstr(info->name);
    mx_printstr(RESET);
    if (info->PWD) {
    	mx_printstr("\033[1;29m");
    	write(1, "(", 1);
    	mx_printstr(&(info->pwd[strlen(info->pwd)
            - (info->name_len - (6 + strlen(info->name)))]));
    	write(1, "): ", 3);
    	mx_printstr(RESET);
	}
}

static void count_name_len(t_info *info) {
    info->name_len = 6 + strlen(info->name);
    if (info->pwd) {
        int i = strlen(info->pwd) - 1;

        for (; info->pwd[i] && info->pwd[i] != '/'; i--);
        info->name_len += strlen(&(info->pwd[i]));
    }
}

// mx_printstr("\033[1;35m");
// mx_printstr(name);
// mx_printstr(RESET);
// mx_print_unicode(129412);
