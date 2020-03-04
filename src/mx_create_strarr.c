#include "../inc/ush.h"

char **mx_create_strarr(int num_of_elements) {
    char **new_array = (char **)malloc(sizeof(char *) * (num_of_elements + 1));

    for (int i = 0; i <= num_of_elements; i++)
        new_array[i] = NULL;
    return new_array;
}
