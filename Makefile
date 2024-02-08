CC = cc
CFLAGS = -Wall -Werror -Wextra -g
NAME = minishell
RM= rm -rf

SRCS = main.c utils/libft_utils.c utils/list_utils.c tokenize/assign_types.c \
	tokenize/tokens_utils.c parsing/parsing.c parsing/free_cmds.c parsing/type_init.c \
	debugger/debug.c run_cmd/run.c run_cmd/handle_builtin.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME} ${SRCS} -lreadline

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all