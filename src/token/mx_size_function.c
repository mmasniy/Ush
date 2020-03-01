#include "../../inc/ush.h"

int mx_get_size_tok(char *s) {
    int size = 0;

    *s == ';' ? size = 1 : 0;
    (*s == '|' || *s == '&') ? size = mx_size_tok(s, 0, 0) : 0; // посмотреть как оно реагирует на &
    mx_isdigit(*s) ? size = mx_size_tok(s, 1, 0) : 0;
    (*s == '>' || *s == '<') ? size = mx_redirect_int(s, 0) : 0;

    // *s == '"' || *s == '\'' ? size = mx_size_str(s, 2, 0) : 0; // echo" ls " - не правильно визначатиме
    (mx_isalpha(*s) || (mx_strchr(MX_CHECK2, *s)))
    ? size = mx_size_str(s, 1, 0) : 0;
    *s == 32 ? size = 1 : 0;
    *s == '\n' ? size = 1 : 0;
    return size;
}
// (i == 0 || *(tmp + i - 1) != '\\')
int mx_size_str(char *s, int f, int i) {
    if (f == 1)
        while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i])
            || (mx_strchr(MX_CHECK2,s[i]))
            || (s[i] == ' ' && i > 0 && s[i - 1] == '\\'))) {
            if (s[i] == '"' || s[i] == '\'') {
                if (mx_char_block(s + i + 1, '\\', s[i], '\0') >= 0)
                    i += mx_char_block(s + i + 1, '\\', s[i], '\0') + 2;
                else
                    i++;
            }
            else  {
                i += (s[i] == '\\' || s[i] == '\"') &&  s[i + 1] ? 2 : 1; // внимательно проверь
            // printf("2\n");
            }
            // printf("i = %d\n", i);
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
        else {
            i = mx_size_str(s, 1, 0);

            // while (s[i] && (mx_isdigit(s[i]) || mx_isalpha(s[i])
            //         || (mx_strchr(MX_CHECK2,s[i]))))
            //     i++;
        }
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

// int mx_get_size_tok(char *s) {
//     int size = 0;

//     *s == ';' ? size = 1 : 0;
//     (*s == '|' || *s == '&') ? size = mx_size_tok(s, 0, 0) : 0; // посмотреть как оно реагирует на &
//     mx_isdigit(*s) ? size = mx_size_tok(s, 1, 0) : 0;
//     (*s == '>' || *s == '<') ? size = mx_redirect_int(s, 0) : 0;
//     (mx_isalpha(*s) || (mx_strchr(MX_CHECK2, *s)))
//     ? size = mx_size_str(s, 1, 0) : 0;
//     *s == '$' ? size = mx_size_str(s, 3, 0) : 0;
//     *s == 32 ? size = 1 : 0;
//     *s == '\n' ? size = 1 : 0;
//     return size;
// }

// int mx_size_str(char *s, int f, int i) {
//     if (f == 1)
//         while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i])
//             || (mx_strchr(MX_CHECK2,s[i]))))
//             i = i + (s[i] == '\\' || s[i] == '\"' || s[i] == '\"' ? 2 : 1); // внимательно проверь
//     else if (f == 2) {
//         while (s[i + 1] != s[0])
//             i++;
//         i = (s[i + 1] && s[i + 1] == s[0]) ? i + 2 : 0;
//     }
//     else
//         while (s[i] && (mx_isalpha(s[i]) || mx_isdigit(s[i]) || s[i] == '_'
//                 || s[i] == '\\' || s[i] == '$' || s[i] == '\"'))
//             i = i + (s[i] == '\\' || s[i] == '\"' ? 2 : 1);
//         return i;
// }
// //mx_atoi(&s[i]) > -1 && mx_atoi(&s[i]) < 128)
// int mx_size_tok(char *s, bool f, int i) {
//     if (f) {
//         while (mx_isdigit(s[i]))
//             i++;
//         if (s[i] == '>' || s[i] == '<')
//             return mx_redirect_int(s, 0);
//         else
//             while (s[i] && (mx_isdigit(s[i]) || mx_isalpha(s[i])
//                     || (mx_strchr(MX_CHECK2,s[i]))))
//                 i++;
//         return i;
//     }
//     if (s[0] == '&' && s[1] && (s[1] == '<' || s[1] == '>'))
//         return (mx_redirect_int(s, 1));
//     else if (s[0] == s[1] && (s[0] == '|' || s[0] == '&'))
//         return 2;
//     else if (s[0] != s[1] && (s[0] == '|' || (s[0] == '&')))
//         return 1;
//     return 0;
// }
