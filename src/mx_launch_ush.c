#include "ush.h"

int ush_execute(char **args, t_info *info) {
	int i;

	if (args[0] == NULL)
	   return 1;
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
