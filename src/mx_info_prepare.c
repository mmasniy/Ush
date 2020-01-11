#include "ush.h"

void mx_info_start(t_info *info, char **environ) {
	int num = 0;
	int count = 0;
	int j = 0;

	for (int i = 0; info->builtin_str[i]; i++)
		num++;
	info->env_o = environ;
	for (int i = 0; info->env_o[i]; i++, count++);

	info->env_c = (char **)malloc(sizeof(char *) * (count + 1));
	for (; environ[j]; j++)
		info->env_c[j] = mx_strdup(environ[j]);
	info->env_c[j] = NULL;
	info->num_of_func = num;
	info->args = NULL;
	info->PWD = NULL;
	info->OLDPWD = NULL;
}
