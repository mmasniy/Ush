#include "../../inc/ush.h"

int mx_arr_size(char **str) {
	int size = 0;

	for (int i = 0; str[i]; i++)
		size++;
	return size;
}

char *mx_strarr_to_str(char **strarr, int i) {
	char *str = NULL;
	char *tmp = NULL;
	char *tmp1 = NULL;

	while (strarr[i]) {
			if (strarr[i + 1] != 0)
				tmp = mx_strjoin(strarr[i], " ");
			else
				tmp = mx_strjoin(strarr[i], "");
			str = mx_strjoin(tmp1, tmp);
			mx_strdel(&tmp1);
			tmp1 = mx_strdup(str);
			if (strarr[i + 1] != 0)
				mx_strdel(&str);
			mx_strdel(&tmp);
			i++;
	}
	mx_strdel(&tmp1);
	mx_strdel(&tmp);
	return str;
}
