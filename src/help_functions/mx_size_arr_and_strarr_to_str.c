#include "../../inc/ush.h"

int mx_arr_size(char **str) {
	int size = 0;

	for (int i = 0; str[i]; i++)
		size++;
	return size;
}

int mx_allarr_size(char **str) {
	int size = 0;

	for (int i = 0; str[i]; i++){
		size += mx_strlen(str[i]);
	}
	return size;
}

char *mx_strarr_to_str(char **strarr, int size_strarr, int i) {
	char *str = mx_strnew(size_strarr);

	while (strarr[i]) {
		if (strarr[i] != 0) {
			str = mx_strcat(str, strarr[i]);
			str = mx_strcat(str, " ");
		}
		i++;
	}
	return str;
}
