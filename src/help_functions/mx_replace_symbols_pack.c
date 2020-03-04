#include "../../inc/ush.h"

void mx_replace_symbols_pack(char **str, int start, int size, char *new_pack) {
    if (*str && new_pack) {
        char *new_str = mx_strnew(strlen(*str) + strlen(new_pack));

        strncat(new_str, *str, start);
        strcat(new_str, new_pack);
        strcat(new_str, *str + start + size);
        mx_strdel(str);
        *str = strdup(new_str);
        mx_strdel(&new_str);
    }
}
