#include "../../inc/ush.h"

int mx_redirect_int(char *s, int i) {
    if (mx_isdigit(s[i])) {
        while (mx_isdigit(s[i]))
            i++;
        if ((s[i] == '>' || s[i] == '>') && s[i + 1] == '&')
            return i + 2;
        if (s[i] == '>' && s[i + 1] == '>' && s[i + 2] == '&')
            return i + 3;
        if ((s[i] == '>' || s[i] == '<') && ((s[i + 1] != '<')
            || s[i + 1] != '>')){
            return ++i;
    }
        if (s[i] == '>' && s[i + 1] == '>')
            return i + 2;
    }
    else
        return mx_redirect_str(s, i);
    return 0;
}

int mx_redirect_str(char *s, int i) {
    if (s[i] == s[i + 1] && (s[i] == '<' || s[i] == '>'))
        return i + 2;
    if (s[i] == '<' || s[i] == '>')
        return ++i;
    return 0;
}

int mx_check(char *token) {
    int p = mx_check_priority(token);

    if ((p > 1 && p < 10) || (p > 10 && p < 14) || (p > 15 && p < 21))
        return 1;
    return -1;
}
