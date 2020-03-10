#include "../../inc/ush.h"

static void count_name_len(t_info *info) {
    info->name_len = 5 + strlen(info->name);
    if (info->pwd) {
        int i = strlen(info->pwd) - 1;

        for (; i && info->pwd[i] && info->pwd[i] != '/'; i--);
        i = i < 0 ? 0 : i;
        info->name_len += strlen(&(info->pwd[i]));
    }
}

void mx_print_ush(t_info *info) {
    mx_print_char_loop('\b', info->name_len - 1);
    count_name_len(info);
    if (info->def_name) {
        (info->name_len)++;
        mx_print_unicode(129412);
        write(1, " ", 1);
    }
    if (info->color_name)
        mx_printstr(info->color_name);
    mx_printstr(info->name);
    info->color_name ? mx_printstr(RESET) : 0;
    write(1, " ", 1);
    if (info->pwd) {
        mx_printstr("\033[1;29m");
        write(1, "(", 1);
        mx_printstr(&(info->pwd[strlen(info->pwd)
            - (info->name_len - (6 + strlen(info->name)))]));
        write(1, "): ", 3);
        mx_printstr(RESET);
    }
}
