#include "../../inc/ush.h"

char *mx_up_to_one(char *str) {
    if (str) {
        int pos = mx_strlen(str) - 1;

        while (pos > 1 && str[pos] != '/')
            pos--;
        return mx_strndup(str, pos);
    }
    else
        return NULL;
}
