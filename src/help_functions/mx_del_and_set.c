#include "../../inc/ush.h"

char *mx_del_and_set(char **str, char *new_str) {
    char *new_line = NULL;

    if (new_str) {
        new_line = strdup(new_str);
        mx_strdel(&new_str);
        mx_strdel(str);
        *str = new_line;
    }
    else
        mx_strdel(str);
    return *str;
}
