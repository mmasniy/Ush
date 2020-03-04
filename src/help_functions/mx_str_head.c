#include "../../inc/ush.h"

bool mx_str_head(const char *where, const char *what) {
    int i = 0;

    for (; where[i] && what[i]; i++)
        if (where[i] != what[i])
            return 1;
    if (where[0] == what[0] && strlen(what) <= i)
        return 0;
    return 1;
}
