#include "../../inc/ush.h"

bool mx_str_char_in_str(char *where, char *what) {
    if (where && what) {
        int len = mx_strlen(where);

        for (int i = 0; what[i]; i++)
            for (int j = 0; j < len; j++)
                if (where[j] == what[i])
                    return 1;
    }
    return 0;
}
