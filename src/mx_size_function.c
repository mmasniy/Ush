#include "ush.h"

int mx_get_size_tok(char *s) {
    int size = 0;

    *s == ';' ? size = 1 : 0;
    (*s == '|' || *s == '&') ? size = mx_size_tok(s, 0, 0) : 0; // посмотреть как оно реагирует на &
    mx_isdigit(*s) ? size = mx_size_tok(s, 1, 0) : 0;
    (*s == '>' || *s == '<') ? size = mx_redirect_int(s, 0) : 0;
    (mx_isalpha(*s) || (mx_strchr(MX_CHECK2, *s)))
    ? size = mx_size_str(s, 1, 0) : 0;
    *s == '$' ? size = mx_size_str(s, 3, 0) : 0;
    *s == 32 ? size = 1 : 0;
    *s == '\n' ? size = 1 : 0;
    return size;
}

int mx_size_str(char *s, int f, int i) {
    if (f == 1)
        while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i])
            || (mx_strchr(MX_CHECK,s[i]))))
            i = i + (s[i] == '\\' || s[i] == '\"' ? 2 : 1); // внимательно проверь
    else if (f == 2) {
        while (s[i + 1] != s[0])
            i++;
        i = (s[i + 1] && s[i + 1] == s[0]) ? i + 2 : 0;
    }
    else
        while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i]) || s[i] == '_'
                || s[i] == '\\' || s[i] == '$' || s[i] == '\"'))
            i = i + (s[i] == '\\' || s[i] == '\"' ? 2 : 1);
        return i;
}

int mx_size_tok(char *s, bool f, int i) {
    if (f) {
        if (mx_isdigit(s[i]))
            i++;
        if (s[i] == '>' || s[i] == '<')
            return mx_redirect_int(s, 0);
        else
            while (s[i] && (mx_isdigit(s[i]) || mx_isalpha(s[i])
                    || (mx_strchr(MX_CHECK,s[i]))))
                i++;
        return i;
    }
    if (s[0] == '&' && s[1] && (s[1] == '<' || s[1] == '>'))
        return (mx_redirect_int(s, 1));
    else if (s[0] == s[1] && (s[0] == '|' || s[0] == '&'))
        return 2;
    else if (s[0] != s[1] && (s[0] == '|' || (s[0] == '&')))
        return 1;
    return 0;
}
