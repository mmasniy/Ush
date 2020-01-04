#include "ush.h"

// List of builtin commands, followed by their corresponding functions.

int ush_pwd(char **args, t_info *info) {
    char *link;
    char *pwd;

    if (info) {}
    if (args[1] == NULL || strcmp(args[1], "-L") == 0) {
        pwd = strdup(getcwd(NULL,0));
        printf("%s\n", pwd);
    }
    else if (strcmp(args[1], "-P") == 0) {
        if ((readlink(getenv("PWD"), NULL, 0)) > 0) {
            link = mx_strnew(readlink(getenv("PWD"), NULL, 0));
            readlink(getenv("PWD"), link, 1000);
            //link = mx_strdup(readlink(getenv("PWD"), NULL, 0));
            printf("link = %s\n", link);
        }
        else
        printf("getenv = %s\n", getenv("PWD"));
        }
    return 1;
}


//Builtin function implementations.
int ush_cd(char **args, t_info *info) {
	if (info) {}
	if (args[1] == NULL) {
	   fprintf(stderr, "Ush: expected argument to \"cd\"\n");
	} 
	else {
        if (chdir(args[1]) != 0) {
            perror("Ush");
        }
	}
	return 1;
}

int ush_help(char **args, t_info *info) {
    if (args) {}
	   int i;
	   printf("Stephen Brennan's USH\n");
	   printf("Type program names and arguments, and hit enter.\n");
	   printf("The following are built in:\n");

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
//**********************************************

int ush_execute(char **args, t_info *info) {
	int i;

	if (args[0] == NULL) {
	// An empty command was entered.
	   return 1;
	}

	for (i = 0; i < info->num_of_func; i++) {
	   if (strcmp(args[0], info->builtin_str[i]) == 0) {
  		    return (*info->builtin_func[i])(args, info);
	   }
	}

	return ush_launch(args, info);
}

int ush_launch(char **args, t_info *info) {
	pid_t pid;
	pid_t wpid;
	int status;

	if (info) {}
	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1)
			perror("ush");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) {
		// Error forking
		perror("ush");
	} 
	else {
	// Parent process
		wpid = waitpid(pid, &status, /*Повертає управління батьківському процесу*/WUNTRACED);
		while (!/*Повертає нуль, доки процес триває*/WIFEXITED(status) 
		&& !/*Повертає не нульове значення, якщо процес завершився помилкою*/WIFSIGNALED(status)) {
			wpid = waitpid(pid, &status, WUNTRACED);
		}
	}
	return 1;
}
