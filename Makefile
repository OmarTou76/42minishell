CC = cc
CFLAGS = -Wall -Werror -Wextra -g
NAME = minishell
RM= rm -rf
 

SRCS = main.c utils/libft_utils.c utils/libft_utils2.c utils/list_utils.c utils/list_utils2.c utils/free_utils.c utils/error_utils.c utils/ft_itoa.c \
	builtins/echo.c builtins/pwd.c builtins/cd.c  builtins/export.c builtins/export_utils.c builtins/unset.c debugger/debug.c \
	tokenize/assign_types.c tokenize/tokens_utils.c tokenize/tokens_checker.c tokenize/update_tokens.c \
	parsing/parsing.c parsing/type_init.c parsing/parse_redirs.c parsing/parse_exec.c \
	env/handle_env.c env/build_env.c builtins/env.c builtins/exit.c utils/libft_utils3.c utils/libft_utils4.c utils/libft_utils5.c utils/libft_utils6.c\
	run_cmd/run.c run_cmd/run_redirs.c run_cmd/handle_builtin.c run_cmd/get_path.c run_cmd/free_cmds.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME} ${SRCS} -lreadline

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all