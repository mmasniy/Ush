#ifndef USH_H
#define USH_H

#include "libmx/inc/libmx.h"
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <termios.h>
#include <dirent.h>

#define USH "\ru$h> "

#define USH_RL_BUFSIZE 	1024
#define USH_TOK_BUFSIZE 64
#define USH_TOK_DELIM 	" \t\r\n\a"
#define RED   			"\x1B[31m"
#define GRN				"\x1B[32m"
#define RESET 			"\x1B[0m"
#define YEL   		"\x1B[33m"
#define BLU   		"\x1B[34m"
#define MAG   		"\x1B[35m"

#define MX_LNK(mode) (((mode) & S_IFMT) == S_IFLNK)

// Errors

#define EXIT_FAILURE 1
#define TERM_ENV_NOT_EXIST "TERM variable in env not exist"

// Structures

enum e_keys{
	CTRL_A = 1,
	CTRL_B = 2,
	CTRL_C = 3,
	CTRL_D = 4,
	CTRL_E = 5,
	CTRL_F = 6,
	CTRL_G = 7, // sound
	CTRL_H = 8,
	TAB = 9, // CTRL+I
	CTRL_K = 11, // vertical tab
	CTRL_L = 12, // new feed
	ENTER = 13,
	CTRL_N = 14,
	CTRL_P = 16,
	CTRL_R = 18,
	CTRL_T = 20,
	CTRL_Y = 25,
	CTRL_U = 21,
	CTRL_W = 23,
	CTRL_X = 24,
	CTRL_Z = 26,
	ESC = 27,
	CTRL_CLOSE_BRACKET = 29,
	BACKSPACE = 127,
	UP = 1001,
	DOWN = 1002,
	LEFT = 1003,
	RIGHT = 1004,
	EXTRA_SYM = 10000,
};

typedef struct s_history {
	// int id;
	char *data;
	struct s_history *prev;
	struct s_history *next;
}				t_history;

typedef struct s_history_pack {
	int total_num;
	struct s_history *pos;
	struct s_history *last;
	struct s_history *history;
}				t_history_pack;

typedef struct	s_info {
	char 		**env_o;
	char 		**env_c;
	char		**args;
	char 		**builtin_str;
	int			(**builtin_func) (struct s_info *info);
	int			num_of_func;
	char		*PWD;
	char		*OLDPWD;
	struct s_history_pack *history_pack;
	struct termios origin_termios;
	int history_pos;
	bool ctrl_d;
	bool ctrl_c;
	int winsize;
} 				t_info;

// Functions

char **mx_arr_copy(char **array);

bool mx_str_head(const char *where, const char *what);
void mx_error_message(char *str);

// mx_print_history.c
int mx_history(t_info *info);

void mx_winsize(t_info *info);

// mx_print_char_loop.c
void mx_print_char_loop(char c, int len);

// mx_print_ush.c
void mx_print_ush();

// mx_work_with_termios.c
bool mx_custom_termios(t_info *info, int fd);
bool mx_origin_termios(t_info *info, int fd);

// mx_sigio_handler.c
void mx_sigio_handler(int sigio);

// mx_history.c
t_history *mx_create_new_history(char *data);
void mx_push_history_front(t_history **lst, void *data);
void mx_pop_history_front(t_history **head);
void mx_pop_history_back(t_history **head);
void mx_check_history(t_info *info, char *line);

void mx_info_start(t_info *info, char **environ);
bool check_link(char *argv);
int mx_ush_pwd(t_info *info);
int mx_ush_execute(t_info *info);
int mx_ush_cd(t_info *info);
int mx_ush_help(t_info *info);
int mx_ush_exit(t_info *info);
char *mx_ush_read_line(t_info *info);
void mx_ush_loop(t_info *info_sh);
char **mx_ush_split_line(char *line);
int mx_ush_launch(t_info *info);

char *mx_format_pwd(char *pwd);
void mx_update_pwd(t_info *info);
int mx_el(char **env_c, char *pwd);

#endif
