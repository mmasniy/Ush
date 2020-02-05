#include "ush.h"

void mx_count_name_len(t_info *info) {
	char *find = getenv("PWD");

	info->name_len = 3;
	if (find) {
		int i = 0;

		info->name_len += 4;
		for (int j = -1; mx_get_char_index(&(find[j]), '/') >= 0;
			i = j, j++);
		if (i >= 0)
			info->name_len += strlen(&(find[i + 1]));
	}
}
