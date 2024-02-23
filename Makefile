CC = cc
CFLAGS = -Wall -Werror -Wextra -g
NAME = minishell
RM= rm -rf

SRCS = main.c utils/libft_utils.c utils/list_utils.c utils/error_utils.c tokenize/assign_types.c \
tokenize/tokens_utils.c parsing/parsing.c parsing/free_cmds.c parsing/type_init.c debugger/debug.c run_cmd/run.c \
env/handle_var.c env/var_env.c builtins/echo.c builtins/pwd.c builtins/env.c utils/libft_utils2.c utils/list_utils2.c \
utils/free_utils.c env/build_env.c builtins/cd.c builtins/unset.c builtins/exit.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME} ${SRCS} -lreadline

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all
