NAME_O 				= minishell
NAME_B 				= minishell_bonus
NAME				= minishell

FLAGS 				= -Wall -Wextra -Werror -O2

R_FLAGS				= -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include		

SRC_O_DIR			= src/mandatory/
SRC_B_DIR			= src/bonus/
SRC_O 				= envp.c \
					free_struct.c \
					init.c \
					main.c \
					signals.c \
					utils_cut.c \
					utils_other.c \
					utils_print.c \
					shlvl.c \
					check_forbidden.c \
					parser.c \
					put_t_comm.c \
					finder.c \
					redirect.c \
					quotes.c \
					dollar.c \
					pre_open.c \
					

					


# SRC_B 				= envp.c \
# 					free_struct.c \
# 					init.c \
# 					main.c \
# 					signals.c \
# 					utils_cut.c \
# 					utils_other.c \
# 					utils_print.c \

HEAD_DIR 			= inc/
HEAD_O 				= minishell.h
# HEAD_B 				= minishell_bonus.h

OBJ_O				= $(patsubst %.c,%.o,$(SRC_O))
# OBJ_B				= $(patsubst %.c,%.o,$(SRC_B))
OBJ_DIR				= obj/
OBJ_O_DIR			= obj/mandatory/
# OBJ_B_DIR			= obj/bonus/

OBJS_1 				= $(addprefix $(OBJ_O_DIR), $(OBJ_O))
# OBJS_2 				= $(addprefix $(OBJ_B_DIR), $(OBJ_B))

CC 					= gcc
RM 					= rm -Rf

all: 				$(NAME)

bonus:				$(NAME)

$(OBJ_O_DIR)%.o: 	$(SRC_O_DIR)%.c $(HEAD_DIR)$(HEAD_O)
					@mkdir -p $(OBJ_O_DIR)
					@gcc -c $< -o $@ -I $(HEAD_DIR) -I ~/.brew/opt/readline/include

# $(OBJ_B_DIR)%.o: 	$(SRC_B_DIR)%.c $(HEAD_DIR)$(HEAD_B)
# 					@mkdir -p $(OBJ_B_DIR)
# 					@gcc -c $< -o $@ -I $(HEAD_DIR)

$(NAME):  			$(OBJS_1) $(OBJS_2)
					$(CC) $(FLAGS) $(OBJS_1) -o $(NAME_O) -lreadline $(R_FLAGS)
# 					$(CC) $(FLAGS) $(OBJS_2) -o $(NAME_B) $(R_FLAGS)

clean:
					$(RM) $(OBJ_DIR)


fclean: 			clean
					$(RM) $(NAME_O) $(NAME_B)

re: 				fclean all

.PHONY: 			all bonus clean fclean re