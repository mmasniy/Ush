NAME = ush
SRCS = src/mx_work_with_termios.c \
	src/mx_winsize.c \
	src/mx_work_with_PATH.c \
	src/mx_signal.c \
	src/mx_split_line.c \
	src/mx_info_prepare.c \
	src/mx_print_errors.c \
	src/mx_error_message.c \
	src/mx_create_strarr.c \
	src/mx_buildin_funcs.c \
	src/main.c \
	\
	src/parse_line_step/mx_shell_functions.c \
	src/parse_line_step/mx_work_with_environ_and_variables.c \
	src/parse_line_step/mx_save_key_value.c \
	src/parse_line_step/mx_parse_line.c \
	src/parse_line_step/mx_find_key_and_insert_value.c \
	src/parse_line_step/mx_search_and_change_tilde.c \
	src/parse_line_step/mx_skip_all_quotes.c \
	src/parse_line_step/mx_substitutions.c \
	\
	src/help_functions/mx_str_head.c \
	src/help_functions/mx_up_to_one.c \
	src/help_functions/mx_str_char_in_str.c \
	src/help_functions/mx_print_char_loop.c \
	src/help_functions/mx_del_strarr_elem.c \
	src/help_functions/mx_arr_copy.c \
	src/help_functions/mx_replace_pack.c \
	src/help_functions/mx_get_char_index_without_symbols.c \
	src/help_functions/mx_replace_symbols_pack.c \
	src/help_functions/mx_size_arr_and_strarr_to_str.c \
	\
	src/builtins/mx_ush_which.c \
	src/builtins/mx_ush_unset.c \
	src/builtins/mx_ush_pwd.c \
	src/builtins/mx_ush_help.c \
	src/builtins/mx_ush_history.c \
	src/builtins/mx_ush_fg.c \
	src/builtins/mx_ush_export.c \
	src/builtins/mx_ush_exit.c \
	src/builtins/mx_ush_env.c \
	src/builtins/mx_ush_echo.c \
	src/builtins/mx_ush_cd.c \
	src/builtins/mx_funcs_for_env.c \
	src/builtins/mx_funcs_for_env_3.c \
	src/builtins/mx_funcs_for_env_2.c \
	src/builtins/mx_funcs_for_cd.c \
	src/builtins/mx_funcs_for_cd_2.c \
	src/builtins/mx_funcs_for_cd_3.c \
	\
	src/all_for_read_line/mx_work_with_history.c \
	src/all_for_read_line/mx_tab_work.c \
	src/all_for_read_line/mx_funcs_for_tab.c \
	src/all_for_read_line/mx_read_line.c \
	src/all_for_read_line/mx_print_ush.c \
	src/all_for_read_line/mx_line_hot_key.c \
	src/all_for_read_line/mx_funcs_for_read_line.c \
	src/all_for_read_line/mx_arrow_keys_for_read_line.c \
	\
	src/token/mx_work_with_tokens.c \
	src/token/mx_size_function.c \
	src/token/mx_create_tok_list.c \
	src/token/mx_redirect_[type].c \
	src/token/mx_check_variable.c \
	\
	src/tree/mx_work_with_tree.c \
	src/tree/mx_trees_help_func.c \
	src/tree/mx_tree_run.c \
	src/tree/mx_tree_pipe.c \
	src/tree/mx_tree_redirection.c \
	src/tree/mx_tree_start_function.c \
	src/tree/mx_start_redirection.c \
	src/tree/mx_alias.c \

OBJS = $(SRCS:.c=.o)
FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic# -g -fsanitize=address
HEADER = inc/ush.h
LIB = libmx/libmx.a

all: install

install: $(LIB) $(OBJS)
	@clang -o $(NAME) $(OBJS) $(LIB)

%.o: %.c $(HEADER)
	@clang -c $(FLAGS) -o $@ -c $<

$(LIB):
	@make -C libmx
	@rm -rf $(OBJS)
uninstall: clean
	@make uninstall -sC libmx
	@rm -rf $(NAME)

clean:
	@make clean -C libmx
	@rm -rf $(OBJS)

reinstall: uninstall install uninstall
