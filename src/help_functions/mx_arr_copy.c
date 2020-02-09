#include "../../inc/ush.h"

char **mx_arr_copy(char **array) {
    int i = 0;
    char **new_array = NULL;

    for (; array[i]; i++);
    new_array = (char **)malloc(sizeof(char *) * (i + 1));
    new_array[i] = NULL;
    for (int c = 0; c < i; c++)
        new_array[c] = mx_strdup(array[c]);
    return new_array;
}
