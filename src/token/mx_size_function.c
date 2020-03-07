#include "../../inc/ush.h"

int mx_get_size_tok(char *s) {
    int size = 0;

    *s == ';' ? size = 1 : 0;
    (*s == '|' || *s == '&') ? size = mx_size_tok(s, 0, 0) : 0;
    mx_isdigit(*s) ? size = mx_size_tok(s, 1, 0) : 0;
    (*s == '>' || *s == '<') ? size = mx_redirect_int(s, 0) : 0;
    *s == 32 ? size = 1 : 0;
    *s == '\n' ? size = 1 : 0;
    (size == 0 && *s >= 33 && *s <= 126 && mx_get_char_index(MX_TYPE, *s)
        == -1) ? size = mx_size_str(s, 1, 0) : 0;
    return size;
}

int mx_size_str(char *s, int f, int i) {
    if (f == 1)
        while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i])
               || (s[i] >= 33 && s[i] <= 126
               && mx_get_char_index(MX_TYPE, s[i]) == -1)
               || (s[i] == ' ' && i > 0 && s[i - 1] == '\\'))) {
            if (s[i] == '"' || s[i] == '\'') {
                if (mx_char_block(s + i + 1, '\\', s[i], '\0') >= 0)
                    i += mx_char_block(s + i + 1, '\\', s[i], '\0') + 2;
                else
                    i++;
            }
            else
                i += (s[i] == '\\' || s[i] == '\"') &&  s[i + 1] ? 2 : 1;
        }
    else
        while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i]) || s[i] == '_'
               || s[i] == '\\' || s[i] == '$' || s[i] == '\"'
               || (s[i] == ' ' && i > 0 && s[i - 1] == '\\')))
            i += (s[i] == '\\' || s[i] == '\"') &&  s[i + 1] ? 2 : 1;
    return i;
}

int mx_size_tok(char *s, bool f, int i) {
    if (f) {
        while (mx_isdigit(s[i]))
            i++;
        if (s[i] == '>' || s[i] == '<')
            return mx_redirect_int(s, 0);
        else
            i = mx_size_str(s, 1, 0);
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
