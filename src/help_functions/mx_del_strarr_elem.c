#include "../../inc/ush.h"

void mx_del_strarr_elem(char **str, int index) {
    if (index >= 0 && str[index]) {
        if (str[index + 1]) {
            char *tmp = str[index];
            int i = index;

            for (; str[i]; i++) {
                str[i] = str[i + 1];
            }
            free(tmp);
        }
        else {
            free(str[index]);
            str[index] = NULL;
        }
    }
}
