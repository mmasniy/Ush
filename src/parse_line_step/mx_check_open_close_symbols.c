#include "../../inc/ush.h"

static bool error_return(t_info *info, char c) {
    info->status = 1;
    fprintf(stderr, "u$h: parse error near `%c'\n", c);
    return 1;
}

bool mx_check_open_close_symbols(t_info *info, char *line) {
    char *close = ")'\"";
    int symbol = -1;
    int pos = -1;

    for (int i = 0; line[i]; i++) {
        if ((symbol = mx_get_char_index(OPEN_CHECK, line[i])) >= 0
            && (i == 0 || line[i - 1] != '\\')) {
            if ((pos = mx_char_block(line + i + 1, '\\', close[symbol], '\0'))
                == -1 || !(i += pos + 1))
                return error_return(info, close[symbol]);
                // fprintf(stderr, "u$h: parse error near `%c'\n", close[symbol]);
                // return (info->status = 1);
        }
        else if (i > 0 && line[i] == '{' && line[i - 1] == '$') {
            if ((pos = mx_char_block(line + i + 1, '\\', '}', '\0')) == -1
                || !(i += pos + 1))
                return error_return(info, '}');
                // printf("%d\n", fprintf(stderr, "u$h: parse error near `%c'\n", '}'));
                // return (info->status = 1);
        }
        else if (line[i] == ')' && (i == 0 || line[i - 1] != '\\')) {
            return error_return(info, line[i]);
            // fprintf(stderr, "u$h: parse error near `%c'\n", line[i]);
            // return (info->status = 1);
        }
    }
    return 0;
}
