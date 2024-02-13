CC = cc
CFLAGS = -Wall -Werror -Wextra -g
NAME = minishell
RM= rm -rf

SRCS = main.c utils/libft_utils.c utils/list_utils.c debugger/debug.c \
	tokenize/assign_types.c tokenize/tokens_utils.c \
	parsing/parsing.c parsing/type_init.c parsing/parse_redirs.c parsing/parse_exec.c \
	run_cmd/run.c run_cmd/handle_builtin.c run_cmd/get_path.c run_cmd/free_cmds.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME} ${SRCS} -lreadline

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all