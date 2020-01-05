#include "ush.h"

int ush_pwd(t_info *info) {
    char *link = NULL;
    char *pwd = NULL;
    //struct stat buff;

    if (info->args[1] == NULL || strcmp(info->args[1], "-L") == 0) {
        pwd = strdup(getcwd(NULL,0));
        printf("%s\n", pwd);
    }
    else if (strcmp(info->args[1], "-P") == 0) {
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

int ush_cd(t_info *info) {
	int dir = 0;

	if ((!info->args[1] || strcmp(info->args[1], "~") == 0))
    	chdir(getenv("HOME"));
	else if (strcmp(info->args[1], "-") == 0)
    	chdir(getenv("OLDPWD"));
    else if ((dir = chdir(info->args[1])) == 0)
    	mx_update_pwd(info);
    else
        perror("ush");
	return 1;
}

int ush_help(t_info *info) {
	int i;

	for (i = 0; i < info->num_of_func; i++) {
	   printf("  %s\n", info->builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int ush_exit(t_info *info) {
    if (info) {}
	   return 0;
}
