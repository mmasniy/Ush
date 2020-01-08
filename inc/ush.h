#ifndef USH_H
#define USH_H

#include "libmx/inc/libmx.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#define USH "u$h> "

#define USH_RL_BUFSIZE 	1024
#define USH_TOK_BUFSIZE 64
#define USH_TOK_DELIM 	" \t\r\n\a"
#define RED   			"\x1B[31m"
#define GRN				"\x1B[32m"
#define RESET 			"\x1B[0m"

#define MX_LNK(mode) (((mode) & S_IFMT) == S_IFLNK)

// Errors

#define EXIT_FAILURE 1
#define TERM_ENV_NOT_EXIST "TERM variable in env not exist"

// Structures

typedef struct	s_info {
	char 		**env_o;
	char 		**env_c;
	char		**args;
	char 		**builtin_str;
	int			(**builtin_func) (struct s_info *info);
	int			num_of_func;
} 				t_info;

// Functions

bool mx_str_head(const char *where, const char *what);
void mx_error_message(char *str);

void mx_info_start(t_info *info, char **environ);
int ush_pwd(t_info *info);
int ush_execute(t_info *info);
int ush_cd(t_info *info);
int ush_help(t_info *info);
int ush_exit(t_info *info);
char *ush_read_line(void);
void ush_loop(t_info *info_sh);
char **ush_split_line(char *line);
int ush_launch(t_info *info);

void mx_update_pwd(t_info *info);
int mx_element_search(char **env_c, char *pwd);

#endif
