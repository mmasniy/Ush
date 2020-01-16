#include "ush.h"

int mx_ush_execute(t_info *info) {
	int i;

	if (info->args[0] == NULL)
	   return 1;
	for (i = 0; i < info->num_of_func; i++) {
	   if (strcmp(info->args[0], info->builtin_str[i]) == 0) {
  		    return (*info->builtin_func[i])(info);
	   }
	}
	//mx_del_strarr(&info->args);
	return mx_ush_launch(info);
}

int mx_ush_launch(t_info *info) {
	pid_t pid;
	pid_t wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(info->args[0], info->args) == -1) {
			mx_printerr(RED);
			perror(USH);
			mx_printerr(RESET);
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) {
		// Error forking
		mx_printerr(RED);
		perror(USH);
		mx_printerr(RESET);
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
