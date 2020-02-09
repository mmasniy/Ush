#include "../../inc/ush.h"

void mx_count_name_len(t_info *info) {
	info->name_len = 3;
	if (info->pwd) {
		int i = -1;

		info->name_len += 4;
		for (int j = 0; mx_get_char_index(&((info->pwd)[j]), '/') >= 0;
			i = j, j++);
		if (i > 0)
			info->name_len += strlen(&(info->pwd[i + 1]));
		else if (i == 0 && strlen(info->pwd) == 1) {
			info->name_len++;
		}
	}
}
