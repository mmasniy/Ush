#include "ush.h"

int mx_ush_help(t_info *info) {
	int i;

	for (i = 0; i < info->num_of_func; i++) {
	   printf("  %s\n", info->builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}
