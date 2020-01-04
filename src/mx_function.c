#include "ush.h"

int ush_pwd(char **args, t_info *info) {
    char *link = NULL;
    char *pwd = NULL;

    if (info) {}
    if (args[1] == NULL || strcmp(args[1], "-L") == 0) {
        pwd = strdup(getcwd(NULL,0));
        printf("%s\n", pwd);
    }
    else if (strcmp(args[1], "-P") == 0) {
        if ((readlink(getcwd(NULL,0), NULL, 0)) > 0) {
            link = strdup(getcwd(NULL,0));
            printf("%s\n", link);
        }
        else
        	link = strdup(getcwd(NULL,0));
        	printf("%s\n", link);
        }
        free(pwd);
        free(link);
    return 1;
}

int ush_cd(char **args, t_info *info) {
	if (info) {}
	if ((!args[1] || strcmp(args[1], "~") == 0))
    	chdir(getenv("HOME"));
	else if (strcmp(args[1], "-") == 0)
    	chdir(getenv("OLDPWD"));
    else if (chdir(args[1]) != 0)
        perror("ush");
	return 1;
}

int ush_help(char **args, t_info *info) {
	int i;

	if (args) {}
	for (i = 0; i < info->num_of_func; i++) {
	   printf("  %s\n", info->builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int ush_exit(char **args, t_info *info) {
    if (args && info) {}
	   return 0;
}
