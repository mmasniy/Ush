#include "ush.h"

static void mx_del_sub(t_info *info, int count, int pos);

void mx_update_pwd(t_info *info) {
	int pos = 0;
	char *tmp = NULL;

	pos = (mx_element_search(info->env_c, "PWD=") == 0) ? 0 :
	mx_element_search(info->env_c, "PWD=");
	if (mx_count_substr(info->args[1], "../") != 0)
		mx_del_sub(info, mx_count_substr(info->args[1], "../"), pos);
	else if (strcmp(info->args[1], "..") == 0)
		mx_del_sub(info, mx_count_substr(info->args[1], ".."), pos);
	if (strcmp(info->args[1], "..") != 0) {
		if (strstr(info->args[1], getenv("HOME")) == 0)
			tmp = mx_strjoin(info->env_c[pos], "/");
		else
			tmp = mx_strjoin(tmp, "PWD=");
		info->env_c[pos] = mx_strjoin(tmp, info->args[1]);
	}
	free(tmp);
}

static void mx_del_sub(t_info *info, int count, int pos) {
	int i = mx_strlen(info->env_c[pos]);
	char *tmp = NULL;

	info->args[1] = mx_replace_substr(info->args[1], "../", "");
	for (; count != 0; i--)
		if (info->env_c[pos][i] == '/')
			count--;
	++i;
	tmp = mx_strnew(i + 1 + mx_strlen(info->args[1]));
	tmp = strncpy(tmp, info->env_c[pos], i);
	if (malloc_size(info->env_c[pos]))
		free(info->env_c[pos]);
	info->env_c[pos] = strdup(tmp);
	free(tmp);
}

int mx_element_search(char **env_c, char *pwd) {
	for (int i = 0; env_c[i]; i++)
		if (mx_strncmp(env_c[i], pwd, mx_strlen(pwd)) == 0)
			return i;
	return 0;
}
