#include "../../inc/ush.h"

int mx_char_block(const char *str, char prev, char c, char next) {
    int pos = 0;

    if (!str)
        return -2;
    for (int i = 0; (pos = mx_get_char_index(&(str[i]), c)) >= 0; i++) {
        i += pos;
        if ((i == 0 || str[i - 1] != prev)
            && (!str[i + 1] || str[i + 1] != next))
            return i;
    }
    return -1;
}
