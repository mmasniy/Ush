#include "../../inc/ush.h"

int mx_count_substr_without_symbol(const char *str, const char *sub,
                                   char sym) {
    int num = 0;
    long len = strlen(sub);

    for (int i = 0; str[i]; i++) {
        if (str[i] == sub[0] && (i == 0 || str[i - 1] != sym)) {
            if (strncmp(str + i, sub, len) == 0) {
                num++;
                i += len - 1;
            }
        }
    }
    return num;
}

char *mx_replace_substr_without_symbol(const char *str, const char *sub,
                                       const char *replace, char symbol) {
    if (str && sub && replace) {
        int sub_len = mx_strlen(sub);
        int replace_len = mx_strlen(replace);
        int sub_num = mx_count_substr_without_symbol(str, sub, symbol);
        char *newstr = mx_strnew(mx_strlen(str) 
            - sub_len * sub_num + replace_len * sub_num);

        for (int i = 0; str[i]; i++)
            if (mx_strncmp(str + i, sub, sub_len) == 0
                && (i == 0 || str[i - 1] != symbol)){
                strcat(newstr, replace);
                i += sub_len - 1;
            }
        return newstr;
    }
    return NULL;
}

int mx_get_substr_index_without_symbol(const char *str, const char *sub,
                                       char symbol) {
    int i = 0;
    int len = 0;

    if (str && sub) {
        len = mx_strlen(sub);
        for (; str[i] != '\0'; i++) {
            if (str[i] == *sub && (i == 0 || str[i - 1] != symbol))
                if (mx_strncmp(str + i, sub, len) == 0)
                    return i;
        }
        if (!str[i])
            return -1;
    }
    return -2;
}
