#include "ush.h"

static bool check_link(char *pwd);

int ush_pwd(t_info *info) {
    char *link = NULL;
    char *pwd = NULL;
    if (check_link(info->args[1])) {}
    if (info->args[1] == NULL || strcmp(info->args[1], "-L") == 0) {
        // pwd = strdup(getcwd(NULL,0));
        printf("%s\n", info->env_c[mx_element_search(info->env_c, "PWD=")]);
    }
    else if (strcmp(info->args[1], "-P") == 0) {
        if ((readlink(getcwd(NULL,0), NULL, 0)) > 0) {
            // link = strdup(getcwd(NULL,0));
            // printf("%s\n", link);
            printf("%s\n", info->env_c[mx_element_search(info->env_c, "PWD=")]);
        }
        else {
        	// link = strdup(getcwd(NULL,0));
        	// printf("%s\n", link);
            printf("%s\n", info->env_c[mx_element_search(info->env_c, "PWD=")]);
            }
    }
        free(pwd);
        free(link);
    return 1;
}

int ush_cd(t_info *info) {
	int dir = 0;
    //char *temp_PWD = mx_strjoin(temp_PWD, 
        // info->env_c[mx_element_search(info->env_c, "PWD=")]);

    // if (check_link(info->args[1])) {
    	if ((!info->args[1] || strcmp(info->args[1], "~") == 0))
        	chdir(getenv("HOME"));
    	else if (strcmp(info->args[1], "-") == 0)
        	chdir(info->env_c[mx_element_search(info->env_c, "OLDPWD=")]);
        else if ((dir = chdir(info->args[1])) == 0)
        	mx_update_pwd(info);
        else
            perror("ush");
    // if (malloc_size(info->env_c[mx_element_search(info->env_c, "OLDPWD=")]))
    //     free(info->env_c[mx_element_search(info->env_c, "OLDPWD=")]);
    // info->env_c[mx_element_search(info->env_c, "OLDPWD=")] = strdup(temp_PWD);
    // printf("%s\n", info->env_c[mx_element_search(info->env_c, "PWD=")]);
    // free(temp_PWD);
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

static bool check_link(char *argv) {
    struct stat buff;

    lstat(argv, &buff);
    if (MX_LNK(buff.st_mode))
        return 1;
    return 0;
}
