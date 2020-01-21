#include "ush.h"

int mx_work_w_toks(char *line, t_tok **tok) {
    int size = 1;
    int i = 0;
    char *tmp;

    if (!line)
        return 0;
    tmp = mx_strdup(line);
    while (size > 0 && *(tmp + i)) {
        if ((size = mx_get_size_tok(tmp + i)) == -1) {
            mx_strdel(&tmp);
            return 0;
        }
        if (*(tmp + i) != 32) {
            mx_add_tok(tok, tmp + i, size);
        }
        i += size;
    }
    // Вывод красивый, чтобы было понятнее
    printf("%slist: %s\n", GRN, RESET);
    printf("%s---------------------------------------------%s\n", MAG, RESET);
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s%s%s ", YEL, temp->token, RESET);
    }
    printf("\n%s---------------------------------------------%s\n", MAG, RESET);
    printf("\n");
    mx_strdel(&tmp);
    return 1;
}
