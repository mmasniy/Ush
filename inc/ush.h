#ifndef USH_H
#define USH_H

#include <stdarg.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h> /* определения типов */ 
#include <sys/ioctl.h>
#include <sys/stat.h> /* структура, возвращаемая stat */ 
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/param.h>     //   const MAXPATHLEN     щк PATH_MAX
#include <termios.h>
#include <signal.h>
// #include <term.h>
#include <curses.h>
#include <malloc/malloc.h>
#include <wchar.h>

#include "libmx/inc/libmx.h"

//*****

#define BUF_SIZE 255
#define ERROR
#define FOUND
#define NOT_FOUND
#define SIXMONTHS ((365 / 2) * 86400)
#define FP_SPECIAL 1
#define STR_SIZE sizeof("rwxrwxrwx")
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
#define JOBS_NUMBER 500
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define FOREGROUND 1
#define BACKGROUND 0
#define PIPE 2
#define COMMAND_BUILTIN 1

#define STATUS_RUNNING 0
#define STATUS_DONE 1
#define STATUS_SUSPENDED 2
#define STATUS_CONTINUED 3
#define STATUS_TERMINATED 4

#define PROC_FILTER_ALL 0
#define PROC_FILTER_DONE 1
#define PROC_FILTER_REMAINING 2

//  JOB STATUS
#define STATUS_RUN "running"
#define STATUS_DON "done"
#define STATUS_SUS "suspended"
#define STATUS_CON "continued"
#define STATUS_TER "terminated"

//*****

/*
** Defines for tokens
*/

#define MX_CHECK "-_/~.:\"\\"
#define MX_CHECK2 "/-~'\".\\="
#define TYPE "; | & &> <& &>> <<& < > << >> && ||"

#define USH "\ru$h> "

#define USH_RL_BUFSIZE 	1024
#define USH_TOK_BUFSIZE 64
#define USH_TOK_DELIM 	" \t\r\n\a"
#define RED   			"\x1B[31m"
#define GRN				"\x1B[32m"
#define RESET 			"\x1B[0m"
#define YEL   			"\x1B[33m"
#define BLU   			"\x1B[34m"
#define MAG   			"\x1B[35m"

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

/*
** list of tokins
*/

typedef struct	s_tok {
	int				type;
	int				prio;
	char			*token; // content
	struct s_tok	*prev;
	struct s_tok	*next;
}					t_tok;

/*
** Tree of commands
*/

typedef struct	s_ast {
	char			*name;
	t_tok			token;
	struct s_ast	*parent;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct		s_process {
    char *fullpath;  //for execve
    char **argv;
   // char **envp;
    char *command;
    char *arg_command;
    pid_t pid;
    int exit_code;
    int status;  //status RUNNING DONE SUSPENDED CONTINUED TERMINATED
    int foreground;
    int pipe;
    int fd_in;
    int fd_out;
    int type;               // COMMAND_BUILTIN = 1;   defaultd = 0
    char completed;        // true if process has completed
    char stopped;          // true if process has stopped
    struct s_process *next;     // next process in pipeline
//    t_ast			*ast;  //ast tree
} t_process;

// A job is a pipeline of processes.
typedef struct s_job {
    int job_id;                 //number in jobs control
    int mark_job_id;            // " ", "-", "+"   "+" - last added job, "-" - prev added job;
    char *command;              // command line, used for messages
    t_process *first_process;     // list of processes in this job
    pid_t pgid;                 // process group ID
    int exit_code;
    int foreground;                  // foreground = 1 or background execution = 0
    char notified;              // true if user told about stopped job
    struct termios tmodes;      // saved terminal modes/
    int stdin;  // standard i/o channels
    int stdout;  // standard i/o channels
    int stderr;  // standard i/o channels
//    struct  s_job *next;           //next active job
} t_job;

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
	int			(**builtin_func) (struct s_info *info, struct s_process *p);
	int			num_of_func;
	char		*PWD;
	char		*OLDPWD;
	struct s_history_pack *history_pack;
	struct termios origin_termios;
	int history_pos;
	bool ctrl_d;
	bool ctrl_c;
	int winsize;

	t_job   *jobs[JOBS_NUMBER];     //arr jobs
	pid_t shell_pgid;

	// for working tab
	struct s_history *tab_list;
	struct s_history *tab_pos;
	int max_number_job;
	//

	char **paths;

} 				t_info;

// Functions --------------------------------------------------------------|

// mx_arrow_keys_for_read_line.c
void mx_arrows_exec(t_info *info, char **buffer, int *position, char c);

// mx_str_char_in_str.c
bool mx_str_char_in_str(char *where, char *what);

// mx_ush_fg.c
int mx_fg(t_info *info, t_process *p);

// mx_read_line.c
char *mx_ush_read_line(t_info *info);
void mx_str_edit(t_info *info, char *buffer, int *position, char *c);
void mx_print_line(t_info *info, char *buffer, int position);

// mx_up_to_one.c
char *mx_up_to_one(char *str);

// mx_tab_work.c
void mx_tab_work(t_info *info, char **buffer, int *position);

// mx_jobs.c
int mx_jobs(t_info *info, t_process *p);

// mx_launch_ush.c
int mx_ush_execute(t_info *info, t_job *job);
int mx_launch_process(t_info *info, int job_id
	, t_process *p, int infile, int outfile, int errfile);

// mx_work_with_process.c
int mx_get_pgid_by_job_id(t_info *info, int job_id);
int mx_get_next_job_id(t_info *info);
int mx_insert_job(t_info *info, t_job *job);
void mx_remove_job(t_info *info, int id);
void mx_destroy_jobs(t_info *info, int id);
void mx_print_process_in_job(t_info *info, int id);
int mx_get_proc_count(t_info *info, int job_id, int filter);
int mx_wait_job(t_info *info, int id);
int mx_wait_pid(t_info *info, int pid);
void mx_set_process_status(t_info *info, int pid, int status);
int mx_print_job_status(t_info *info, int id);
int mx_is_job_completed(t_info *info, int id);
void mx_check_jobs(t_info *info);
int mx_get_job_id_by_pid(t_info *info, int pid);
int mx_set_job_status(t_info *info, int job_id, int status);

// mx_create_job.c
t_job *mx_create_job(t_info *info, char **args);

// mx_save_PATH.c
void mx_save_PATH(t_info *info, char *paths);
char *mx_find_in_PATH(t_info *info, char *word, bool full);

// mx_test.c
int mx_test(t_info *info, t_process *p);

// mx_arr_copy.c
char **mx_arr_copy(char **array);

// mx_str_head.c
bool mx_str_head(const char *where, const char *what);

// mx_error_message.c
void mx_error_message(char *str);

// mx_print_history.c
int mx_history(t_info *info, t_process *p);

// mx_winsize.c
void mx_winsize(t_info *info);

// mx_print_char_loop.c
void mx_print_char_loop(char c, int len);

// mx_print_ush.c
void mx_print_ush();

// mx_work_with_termios.c
bool mx_custom_termios(t_info *info, int fd);
bool mx_origin_termios(t_info *info, int fd);

// mx_signal.c
void mx_sigio_handler(int sigio);

// mx_history.c
t_history *mx_create_new_history(char *data);
void mx_push_history_front(t_history **lst, void *data);
void mx_push_history_back(t_history **lst, void *data);
void mx_pop_history_front(t_history **head);
void mx_pop_history_back(t_history **head);
void mx_check_history(t_info *info, char *line);

// mx_info_prepare.c
void mx_info_start(t_info *info, char **environ);

// mx_ush_[Name].c
int mx_ush_pwd(t_info *info, t_process *p);
int mx_ush_cd(t_info *info, t_process *p);
int mx_ush_help(t_info *info, t_process *p);
int mx_ush_exit(t_info *info, t_process *p);

void mx_ush_loop(t_info *info_sh);
char **mx_ush_split_line(char *line);

bool check_link(char *argv);

char *mx_format_pwd(char *pwd);
void mx_update_pwd(t_info *info);
int mx_el(char **env_c, char *pwd);

//mx_create_tok_list
void mx_add_tok(t_tok **prev, char *cont, int size);
void mx_free_toks(t_tok **tok);

//mx_check_variable
int mx_check_type(char *cont);
int mx_check_priority(char *cont);

//mx_work_with_tokens
int mx_work_w_toks(char *line, t_tok **tok);

//mx_size_function
int mx_size_str(char *s, int f, int i);
int mx_get_size_tok(char *s);
int mx_size_tok(char *s, bool f, int i);

//mx_redirect_[type].c
int mx_redirect_int(char *s, int i);
int mx_redirect_str(char *s, int i);


#endif
