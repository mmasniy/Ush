#include "ush.h"

int mx_check_type(char *cont) {
    if ((mx_count_substr(TYPE, cont)) == 0)
        return 0;
    else if ((mx_count_substr(TYPE, cont)) != 0)
        return 1;
    else
        return 2;
}

int mx_check_priority(char *cont) {
    if (mx_strcmp(cont, ";") == 0)
        return 1;
    else if (mx_strcmp(cont, "|") == 0 || (mx_strcmp(cont, "&")) == 0)
        return 2;
    else if ((mx_strcmp(cont, "<&") == 0 || (mx_strcmp(cont, "&>")) == 0)
        || (mx_strcmp(cont, "<<&") == 0 || (mx_strcmp(cont, "&>>")) == 0))
        return 3;
    else if (mx_strcmp(cont, "||") == 0 || (mx_strcmp(cont, "&&")) == 0)
        return 4;
    else if (mx_strcmp(cont, "<") == 0 || (mx_strcmp(cont, ">")) == 0)
        return 5;
    else if (mx_strcmp(cont, "<<") == 0 || (mx_strcmp(cont, ">>")) == 0)
        return 6;
    return 7;
}
