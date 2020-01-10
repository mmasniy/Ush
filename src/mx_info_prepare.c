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

// static void builtin_functions(t_info *info) {
// 	char *builtin_str[] = {"pwd", "cd", "help", "exit", NULL};
// 	int (*builtin_func[]) (t_info *) = {&ush_pwd, &ush_cd, &ush_help, &ush_exit, NULL};
// 	int i = 0;
// 	for (; builtin_func[i]; i++);

// 	info->builtin_str = mx_arr_copy(builtin_str);
// 	for (int i = 0; info->builtin_str[i]; i++)
// 		printf("Elem = %s\n", info->builtin_str[i]);
// 	// info->builtin_func = {&ush_pwd, &ush_cd, &ush_help, &ush_exit};
// }
