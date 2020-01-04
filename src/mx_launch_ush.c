#include "ush.h"

// List of builtin commands, followed by their corresponding functions.
char *builtin_str[] = {"pwd", "cd", "help", "exit"};

int (*builtin_func[]) (char **) = {&ush_pwd, &ush_cd, &ush_help, &ush_exit};

int ush_num_builtins() {
  	return sizeof(builtin_str) / sizeof(char *);
}

int ush_pwd(char **args) {
    char *link;
    // mx_strnew(readlink(getenv("PWD"), NULL, 0));
    printf("%zd\n", readlink(getenv("PWD"), NULL, 0));
    printf("%s\n", args[1]);
    if (args[1] == NULL)
        printf("%s\n", getenv("PWD"));
    else if (strcmp(args[1], "-L") == 0)
        printf("%s\n", getenv("PWD"));
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
int ush_cd(char **args) {
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

int ush_help(char **args) {
	if (args) {}
  	int i;
  	printf("Stephen Brennan's USH\n");
  	printf("Type program names and arguments, and hit enter.\n");
  	printf("The following are built in:\n");

  	for (i = 0; i < ush_num_builtins(); i++) {
  	  	printf("  %s\n", builtin_str[i]);
  	}

  	printf("Use the man command for information on other programs.\n");
  	return 1;
}

int ush_exit(char **args) {
	if (args) {}
  	return 0;
}
//**********************************************

int ush_execute(char **args) {
  	int i;

  	if (args[0] == NULL) {
    	// An empty command was entered.
    	return 1;
  	}

  	for (i = 0; i < ush_num_builtins(); i++) {
    	if (strcmp(args[0], builtin_str[i]) == 0) {
      		return (*builtin_func[i])(args);
    	}
  	}

  	return ush_launch(args);
}

int ush_launch(char **args) {
	pid_t pid;
	pid_t wpid;
	int status;

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
