#include "../inc/ush.h"

static char *result_swap(char *search_line, int *len) {
    char *result_line = "~";

    if (!(search_line[1]) || mx_isspace(search_line[1])
        || search_line[1] == '/') {
        result_line = getenv("HOME");
    }
    else if (search_line[1] == '+' && (!(search_line[2])
        || mx_isspace(search_line[2]))) {
        result_line = getenv("PWD");
        (*len)++;
    }
    return result_line;
}

void mx_tilde_search(char **line, char *craft) {
    char *new_line = mx_strnew(strlen(*line));
    char *tmp = NULL;
    int len = 0;

    for (int pos = -1; (pos = mx_get_char_index(craft, '~')) >= 0; len = 0) {
        strncat(new_line, craft, pos);
        if ((*line == &(craft[pos]) || mx_isspace(craft[pos - 1]))
            && strcmp((tmp = result_swap(&(craft[pos]), &len)), "~")) {
            new_line = realloc(new_line, malloc_size(new_line) + strlen(tmp));
            strcat(new_line, tmp);
            len++;
        }
        else
            strncat(new_line, &(craft[pos]), (len = 1));
        craft += pos + len;
    }
    strcat(new_line, craft);
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}
