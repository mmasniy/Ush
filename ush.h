#ifndef USH_H
#define USH_H

#include "libmx/inc/libmx.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
#define GRN	"\x1B[32m"
#define RESET "\x1B[0m"

int lsh_execute(char **args);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
char *lsh_read_line(void);
void lsh_loop(void);
char **lsh_split_line(char *line);
int lsh_launch(char **args);

#endif
