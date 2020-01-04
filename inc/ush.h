#ifndef USH_H
#define USH_H

#include "libmx/inc/libmx.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define USH_RL_BUFSIZE 1024
#define USH_TOK_BUFSIZE 64
#define USH_TOK_DELIM " \t\r\n\a"
#define GRN	"\x1B[32m"
#define RESET "\x1B[0m"

typedef struct	s_info {
	char 		**env;
	char 		**builtin_str;
	int			(**builtin_func) (char **, struct s_info *);
	int			num_of_func;
} 				t_info;

t_info *mx_info_start(t_info *info, char **environ);
int ush_pwd(char **args, t_info *info);
int ush_execute(char **args, t_info *info);
int ush_cd(char **args, t_info *info);
int ush_help(char **args, t_info *info);
int ush_exit(char **args, t_info *info);
char *ush_read_line(void);
void ush_loop(t_info *info_sh);
char **ush_split_line(char *line);
int ush_launch(char **args, t_info *info);

#endif
