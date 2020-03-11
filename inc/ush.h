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
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/param.h>
#include <termios.h>
#include <signal.h>
#include <curses.h>
#include <malloc/malloc.h>
#include <wchar.h>
#include <termcap.h>
#include <utime.h>
#include "../libmx/inc/libmx.h"

/*
* Defines for tokens
*/

#define MX_Q "'\""
#define MX_TYPE "; | & &> <& &>> <<& < > << >> && ||"
#define MX_EXPORT_VALUE_ALLOW "_$+-#()?:<>|&=.,/~"

#define USH "u$h>"

#define MX_USH_RL_BUFSIZE  1024
#define MX_USH_TOK_BUFSIZE 64
#define MX_USH_TOK_DELIM   " \t\r\n\a"
#define RED             "\x1B[1;31m"
#define GRN             "\x1B[1;32m"
#define RESET           "\x1B[0m"
#define YEL             "\x1B[1;33m"
#define BLU             "\x1B[1;34m"
#define MAG             "\x1B[1;35m"

#define MX_LNK(mode) (((mode) & S_IFMT) == S_IFLNK)
#define MX_W_INT(w)         (*(int *)&(w))
#define MX_WST(x)           (x & 0177)
#define MX_WIFEXIT(x)       (MX_WST(x) == 0)
#define MX_WIFSIG(x)        (MX_WST(x) != _WSTOPPED && MX_WST(x) != 0)
#define MX_WTERMSIG(x)      (MX_WST(x))
#define MX_WEXITSTATUS(x)   ((MX_W_INT(x) >> 8) & 0x000000ff)

// Errors

#define EXIT_FAILURE 1
#define TERM_ENV_NOT_EXIST " TERM variable in env not exist\n"
#define ERROR_PARSE_TREE "u$h: syntax error near after token `"
#define MX_ER ": command not found"
#define MX_NOT_FOUND " no such file or directory: "
#define MX_DOT "u$h: parse error near `;;'\n"
#define MX_RED "u$h: parse error: input, please, one of redirections. \n"
#define MX_DOP_RED "If it's not like a case : \"functions << file1 > file\"\n"
// Structures

enum e_keys {
    CTRL_A = 1,
    CTRL_B = 2,
    CTRL_C = 3,
    CTRL_D = 4,
    CTRL_E = 5,
    CTRL_F = 6,
    CTRL_G = 7, // Sound
    CTRL_H = 8,
    TAB = 9, // CTRL+I
    CTRL_K = 11, // Vertical tab
    CTRL_L = 12, // New feed
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
    EXTRA_SYM = 10000,
};

/*
* List of tokens
*/

typedef struct  s_export {
    char *key;
    char *value;
    struct s_export *next;
}               t_export;

typedef struct  s_tok {
    int             type;
    int             prio;
    char            *token; // Content
    struct s_tok    *prev;
    struct s_tok    *next;
}               t_tok;

/*
* Tree of commands
*/

typedef struct  s_ast {
    char            **command;
    t_tok           token;
    int             type;
    struct s_ast    *father;
    struct s_ast    *left;
    struct s_ast    *right;
}               t_ast;

typedef struct  s_history {
    // int id;
    char *data;
    struct s_history *prev;
    struct s_history *next;
}               t_history;

typedef struct  s_history_pack {
    int total_num;
    struct s_history *pos;
    struct s_history *last;
    struct s_history *history;
}               t_history_pack;

typedef struct  s_process {
    int                 value;
    int                 last_value;
    int                 pos;
    char                *cmd;
    pid_t               pid;
    struct  s_process   *next;
}                       t_process;

typedef struct  s_alias {
    char                *name;     // mk
    char                *value;    // make && ./ush
    struct s_alias      *next;
}               t_alias;

typedef struct  s_info {
    char *name;
    char *color_name;
    bool def_name;
    unsigned int name_len;
    char **args;
    struct s_history_pack *history_pack;
    struct termios origin_termios;
    int history_pos;
    bool ctrl_d;
    bool ctrl_c;
    int winsize;

    // For working tab
    struct s_history *tab_list;
    struct s_history *tab_pos;
    int num_of_elems_in_tab;
    char *history_path;

    char **paths;
    struct s_export *to_export;
    struct s_export *variables;
    struct s_export *shell_funcs;

    char *pwd;
    char *oldpwd;
    int status;
    char quotes;

    //Fd
    int fd[3];
    int red;
    int fd_r;

    // For add out in file
    int file;
    int fd_f;
    char *path_f;

    int flag_for_valid;
    int file_not_f;
    int type_e;

    char *fname;

    // Alias
    struct s_alias          *alias;
    struct s_process        *process;
    struct s_ast            *t;

    // Variable for counting redirection in tree
    int num_of_red;

    // For ctrl+d
    int d;
}               t_info;

// Functions --------------------------------------------------------------|

// All parse --------------------------------|

bool mx_is_allow(char c);

void mx_cntr_key(t_info *i);

// File: mx_check_open_close_symbols.c
bool mx_check_bracket(char *line, int *pos_in_line);
bool mx_check_open_close_symbols(t_info *info, char *ln, int symbol, int pos);

// File: mx_check_first_argument.c
bool mx_check_first_argument(t_info *info, char *head);

// File: mx_substitutions.c
bool mx_execute_substitutions(t_info *info, char **line);

// File: mx_substitutions_2.c
void mx_exec_substitutions_command(t_info *info
    , char **sub_line, char quotes);
char mx_check_quotes(char *line, int start, int end);

// File: mx_shell_functions.c
void mx_shell_functions(t_info *info, char **line);

// File: mx_work_with_tilde.c
int mx_tilde_work(t_info *info, char **line, char *craft);

// File: mx_find_key_and_insert_value.c
bool mx_insert_value(t_info *info, char **line, char *craft);

// File: mx_save_ush_key_value.c
void mx_save_ush_key_value(t_info *info, char **line, char *craft);

// File: mx_parse_line.c
void mx_search_slash(char **line);
char mx_is_quotes(char *line, int pos);
bool mx_parse_line(t_info *info, char **line);

// File: mx_del_slash_and_quotes_in_list.c
bool mx_del_slash_and_quotes_in_list(t_ast *tree, bool *not_valid);
bool mx_work_with_line(char **tmp_line, char **new, int *pos, char *line);

// End parse------------------------------------------|

// File: mx_multi_line_enter.c
void mx_multi_line_enter(t_info *info, char *key_word);

// mx_print_errors.c
void mx_check_error(t_info *i, char *error, char *arg);
void mx_print_error(t_info *i, char *error, char *arg);
void mx_print_red_err(int i, t_info *info);

// File: mx_buildin_funcs.c
int mx_run_buildin(t_info *info);
int mx_check_buildin(t_info *info, char *arg, bool exec);
char *mx_find_similar_buildin(char *what_check);

// File: mx_funcs_for_cd.c
bool mx_check_cd_args(t_info *info, char **args, char *flag, char **argument);

// File: mx_funcs_for_cd_2.c
void mx_dots_for_path(t_info *info, char **arg, char flag, bool up);
void mx_find_last_slash(char **str);
char *mx_save_without_links(t_info *info, char *path);

// File: mx_funcs_for_cd_3.c
void mx_change_link_for_dots_in_cd(char **arg);
bool mx_cd_error(char *arg);
int mx_check_cd_flags(t_info *info
    , char *find_flag, int i, char **argument);

// File: mx_funcs_for_env.c
t_export *mx_save_env_as_list(char **environ);
char **mx_save_env_as_massive(t_export *env);
int mx_check_to_execute(t_info *info, char **path, int position);

// File: mx_funcs_for_env_2.c
bool mx_check_args(t_export **env, char **args, int *flags, char **path);

// File: mx_funcs_for_env_3.c
void mx_del_env_elem(t_export **env);
void mx_sort_print(t_export **list_to_sort);
int mx_check_variable_create(t_export **env, char **args, int pos, int *fgs);

// File: mx_work_with_environ.c
t_export *mx_search_key_in_list(t_export *list, char *key);
t_export *mx_create_new_export(char *key, char *value);
void mx_push_export_back(t_export **list, char *key, char *value);
void mx_pop_export_front(t_export **head);
bool mx_check_is_continue(char *craft, int *pos, char **new_line, char **chk);

// File: mx_replace_pack.c
int mx_count_substr_without_symbol(const char *str, const char *sub,
                                   char symbol);
char *mx_replace_substr_without_symbol(const char *str,const char *sub,
                                       const char *replace, char symbol);
int mx_get_substr_index_without_symbol(const char *str, const char *sub,
                                       char symbol);

// File: mx_get_char_index_without_symbols.c
int mx_char_block(const char *str, char prev, char c, char next);

// File: mx_del_strarr_elem.c
void mx_del_strarr_elem(char **str, int index);

// File: mx_create_strarr.c
char **mx_create_strarr(int num_of_elements);

// File: mx_arrow_keys_for_read_line.c
void mx_arrows_exec(t_info *info, char **buffer, int *position, char c);
void mx_change_hry(t_info *info, int *pos, char **buffer, t_history *link);

// File: mx_line_hot_key.c
bool mx_line_hot_key(t_info *info, char **buffer, int *position, char *c);
void mx_up_lines(t_info *info, char *str, unsigned int add);
void mx_print_posible_history(t_info *info, t_history **result);
t_history *mx_search_for_ctrl_r(t_info *info, char *search_line);
void mx_ctrl_c(t_info *info, char **buffer, int *position);

// File: mx_del_and_set.c
char *mx_del_and_set(char **str, char *new_str);;

// File: mx_ctrl_r.c
void mx_ctrl_r(t_info *info, char **buffer, int *position);

// File: mx_str_char_in_str.c
bool mx_str_char_in_str(char *where, char *what);

// File: mx_read_line.c
char *mx_ush_read_line(t_info *info);

// File: mx_funcs_for_read_line.c
int mx_getchar();
void mx_str_edit(t_info *info, char *buffer, int *position, char *c);
void mx_print_line(t_info *info, char *buffer, int position);

// File: mx_up_to_one.c
char *mx_up_to_one(char *str);

// File: mx_print_tab_list.c
void mx_print_tab_list(t_info *info);

// File: mx_tab_work.c
void mx_tab_work(t_info *i, char **b, int *position);

// File: mx_funcs_for_tab.c
void mx_replace_special_symbols(t_info *info);

// File: mx_save_PATH.c
void mx_save_PATH(t_info *info);
char *mx_find_in_PATH(char **paths, char *word, bool full);

// File: mx_arr_copy.c
char **mx_arr_copy(char **array);

// File: mx_str_head.c
bool mx_str_head(const char *where, const char *what);

// File: mx_error_message.c
void mx_error_message(char *str);
void mx_file_not_found(char *filename, t_info *i);
void mx_error_mes_tree(int type, t_info *i);

// File: mx_winsize.c
int mx_winsize(t_info *info);

// File: mx_print_char_loop.c
void mx_print_char_loop(char c, int len);

// File: mx_replace_symbols_pack.c
void mx_replace_symbols_pack(char **str, int start, int size, char *new_pack);

// File: mx_print_ush.c
void mx_print_ush(t_info *info);

// File: mx_work_with_termios.c
bool mx_custom_termios(t_info *info, int fd);
bool mx_origin_termios(t_info *info, int fd);

// File: mx_signal.c
void mx_sigio_handler(int sigio);

// File: mx_work_with_history.c
t_history *mx_create_new_history(char *data);
void mx_push_history_front(t_history **lst, void *data);
void mx_push_history_back(t_history **lst, void *data);
void mx_pop_history_front(t_history **head);
void mx_pop_history_back(t_history **head);

// File: mx_work_with_history_2.c
void mx_save_all_history(t_info *info);
void mx_check_history(t_info *info, char *line);

// File: mx_info_prepare.c
void mx_info_start(t_info *info);

// File: mx_ush_[Name].c
int mx_ush_pwd(t_info *info);
int mx_ush_cd(t_info *info);
int mx_ush_history(t_info *info);
int mx_ush_help(t_info *info);
int mx_ush_exit(t_info *info);
int mx_ush_env(t_info *info);
int mx_ush_set(t_info *info);
int mx_ush_unset(t_info *info);

int mx_ush_export(t_info *info);
bool mx_update_key_value(t_export **list, char **key, char **value);

int mx_ush_which(t_info *info);
int mx_ush_echo(t_info *info);
int mx_ush_custom(t_info *info);
int mx_ush_true(t_info *info);
int mx_ush_false(t_info *info);
int mx_ush_return(t_info *info);

void mx_ush_loop(t_info *info_sh);

// File: mx_create_tok_list.c
void mx_add_tok(t_tok **prev, char *cont, int size);
char *mx_check_red(char *str, int *size);
void mx_free_toks(t_tok **tok);
void mx_check_file_in_or_out(t_tok *prev, t_tok *next);

//mx_check_variable.c
int mx_check_type(char *cont);
int mx_check_priority(char *c);
void mx_add_num(t_tok **root, char *num, int i);
void mx_add_ampersand(t_tok **root, char *num);
void mx_check_tok(t_tok **tok, char *str, int i);

// File: mx_work_with_tokens.c
int mx_work_w_toks(char *line, t_tok **tok, t_info *info);

// File: mx_size_function.c
int mx_get_size_tok(char *s);
int mx_size_str(char *s, int f, int i);
int mx_size_tok(char *s, bool f, int i);

// File: mx_redirect_[type].c
int mx_check(char *token);
int mx_redirect_int(char *s, int i);
int mx_redirect_str(char *s, int i);


// File: mx_work_with_tree.c
int mx_check_op(int p);
t_ast *mx_build_ast(t_tok *max);
t_ast *mx_start_tree(t_tok *tok, t_info *i);
t_ast *mx_create_leaf(t_tok *max, int side);

// File: mx_work_with_tree2.c
int mx_valid_tree(t_ast **t, t_tok *tok, t_info *i);
t_ast *mx_valid_tree_and_input(t_ast **t, t_tok *tok, t_info *i);

// File: mx_trees_help_func.c
t_tok *mx_search_first(t_tok *tok);
t_ast *mx_create_ast(t_tok *max);
char **mx_merge_command(t_tok *t);
char **mx_merge_op(t_tok *max);
void mx_free_tree(t_ast **tree);

// File: tree_run.c
int mx_tree_run(t_ast *tree, t_info *info, int f);
int mx_run_pipe(t_ast *tree, t_info *info);
void mx_tok_to_tree(t_tok *tok, t_info *info);

// File: mx_tree_start_func.c
void mx_dup_2(t_info *i, int flag);
int mx_start_function(t_ast *t, t_info *info, char **tree);
void mx_execute_binary_file(t_ast *t, t_info *info);
void mx_exec_for_file(t_ast *t, t_info *i);

// File: mx_start_redirection.c
int mx_start_red(t_ast *t, t_info *info, pid_t pid);
void mx_execute_red(t_ast *t, t_info *info, pid_t pid);
void mx_execute_file(t_ast *t, t_info *info, pid_t pid);

// File: mx_tree_redirection.c
int mx_create_file(t_ast *t, t_info *i);
int mx_redirection(int type);
int mx_run_redirection(t_ast *t, t_info *i, int flag);
void mx_multi_line_enter(t_info *info, char *key_word);

// File: mx_alias.c
void mx_add_alias(t_ast *t, t_info *i, int a);
char *mx_get_name_als(char **alias, t_info *i, int count);
void mx_get_value_als(t_alias *a, char **alias, int i);

// File: mx_work_with_alias_list.c
t_alias *mx_create_als(t_alias **als, char *alias, t_info *i);
void mx_add_newnode_als(t_alias **als, char *alias, t_info *i);
int mx_replace_als_to_cmd(t_alias *als, char **line, int i);

// File: mx_size_arr_and_strarr_to_str.c
int mx_arr_size(char **str);
char *mx_strarr_to_str(char **strarr, int i);

// File: mx_wait.c
void mx_waitpid(t_info *i, t_ast *t, int status, pid_t pid);

// File: mx_wait2.c
void mx_pop_front_process(t_process **p);
void mx_del_procces_by_pid(t_process **p, pid_t pid);
void mx_print_added_new_node(t_process *p, pid_t pid);
t_process *mx_get_name_procces(t_process *process, pid_t pid);

// File: mx_wait3.c
void mx_add_inside_list(t_process **p, t_process **a, pid_t pid, char **cmd);
void mx_push_front_proc(t_process **p, pid_t pid, char **cmd);
void mx_add_process(t_process **p, pid_t pid, char **com);

// File: mx_ush_fg.c
int mx_fg(t_info *i, int status);
void mx_wait_process(t_info *i, int status, pid_t child);
void return_value(t_process **p, int flag);

//File: mx_work_with_fg.c
t_process *mx_search_by_char(t_info *i, char *cmd, t_process *processes);
t_process *mx_search_by_id(t_info *i, int pos, t_process *processes);
t_process *mx_get_process(t_info *i, t_process *process, char *cmd);
t_process *get_last_process(t_process *p);
int mx_continue_process(t_info *i, char **argv, int fd, pid_t last);

// File: mx_ush_jobs.c
int mx_jobs(t_info *i);

//File: mx_ush_kill.c
int mx_kill(t_info *i);

// Delete
void printKLP(t_ast* root);
void mx_printf_strarr(char **str);
void print_all(t_ast *tree, t_tok *tok);
void mx_print_pr(t_process *p);

#endif

/*
* ╔═══════════════╗
* ║     TOKEN     ║
* ╠═══════════════╣
* ║ 10  Command   ║
* ║ 1   ";"       ║
* ║ 2   "&"       ║
* ║ 3   "|"       ║
* ║ 4   "<"       ║
* ║ 5   ">"       ║
* ║ 6   "||"      ║
* ║ 7   "&&"      ║
* ║ 8   "<<"      ║
* ║ 9   ">>"      ║
* ║ 11  "<>"      ║
* ║ 12  "<&"      ║
* ║ 13  ">&"      ║
* ║ 16  ">>&"     ║
* ║ 17  "&>"      ║
* ║ 18  "&<"      ║
* ║ 19  "&>>"     ║
* ║ 20  "&<<"     ║
* ║ 15  filein    ║
* ║               ║
* ║               ║
* ║               ║
* ╚═══════════════╝
*/
