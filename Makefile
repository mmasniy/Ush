NAME = ush

INC = ush.h

SRC = *.c

OBJ = *.o

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic# -g -fsanitize=address

all: install clean

install:
	@make install -sC libmx
	@cp $(addprefix src/, $(SRC)) .
	@cp $(addprefix inc/, $(INC)) .
	@clang $(CFLAGS) -c $(SRC) -I $(INC)
	@clang $(CFLAGS) libmx/libmx.a $(OBJ) -o $(NAME)
	@mkdir -p obj
	@mv $(OBJ) ./obj

uninstall: clean
	@make uninstall -sC libmx
	@rm -rf $(NAME)

clean:
	@make clean -sC libmx
	@rm -rf $(INC) $(SRC) ./obj

reinstall: uninstall install uninstall

# NAME = ush
# INC = inc/ush.h

# SRC = *.c

# #direct and lib
# LIB = libmx/libmx.a
# SRC_DIR = src
# OBJ_DIR = obj

# #colors
# RED = \x1B[31m
# GRN	= \x1B[32m"
# RESET = \x1B[0m"
# YEL = \x1B[33m"
# BLU = \x1B[34m"
# MAG = \x1B[35m"

# #compile
# CL = clang
# FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -g -fsanitize=address

# SRCS = $(addprefix $(SRC_DIR)/,$(SRC))

# OBJS = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

# all: install

# install: $(LIB) $(OBJS)
# 	$(CL) $(OBJS) $(LIB) -o $(NAME) -I $(INC)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(OBJ_DIR)
# 	@$(CL) $(FLAGS) -o $@ -c $< -I $(INC)

# $(LIB):
# 	@make install -sC libmx

# uninstall: clean
# 	@rm -rf $(NAME)
# 	@make uninstall -sC libmx

# clean:
# 	@rm -rf $(OBJ)
# 	@make clean -sC libmx

# reinstall: uninstall all

