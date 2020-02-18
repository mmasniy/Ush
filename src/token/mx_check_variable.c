#include "../../inc/ush.h"

int mx_check_type(char *cont) {
    if ((mx_count_substr(TYPE, cont)) == 0)
        return 0;
    else if ((mx_count_substr(TYPE, cont)) != 0)
        return 1;
    return -1;
}

int mx_check_priority(char *c) {
    int p = -1;

    mx_strcmp(c, ";") == 0 || mx_strcmp(c, "\n") == 0 ? p = 1:
    mx_strcmp(c, "&") == 0 ? p = 2 : mx_strcmp(c, "|") == 0 ? p = 3 :
    mx_strcmp(c, "<") == 0 ? p = 4 : mx_strcmp(c, ">") == 0 ? p = 5 :
    mx_strcmp(c, "||") == 0 ? p = 6 : mx_strcmp(c, "&&") == 0 ? p = 7 :
    mx_strcmp(c, "<<") == 0 ? p = 8 : mx_strcmp(c, ">>") == 0 ? p = 9 :
    mx_strcmp(c, "<>") == 0 ? p = 11 : mx_strcmp(c, "<&") == 0 ? p = 12 :
    mx_strcmp(c, ">&") == 0 ? p = 13 : mx_strcmp(c, ">>&") == 0 ? p = 16 :
    mx_strcmp(c, "&>") == 0 ? p = 17 : mx_strcmp(c, "&<") == 0 ? p = 18 :
    mx_strcmp(c, "&>>") == 0 ? p = 19 : mx_strcmp(c, "&<<") == 0 ? p = 20:  0;
    if (p == -1)
        return 10;
    return p;
}
