#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "string.h"

#define SPACES " \n\t\r"
#define QUOTES "\"\'"

typedef enum s_tok_type
{
    WORD,
    PIPE,
    REDIR,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_tok_type;

typedef enum s_type
{
    EXEC,
    PIPE_CMD,
    REDIR_CMD,
} t_type;

typedef struct s_tokens
{
    char *cmd;
    t_tok_type type;
    struct s_tokens *next;
} t_tokens;

typedef struct s_cmd
{
    t_type type;
} t_cmd;

typedef struct s_exec
{
    t_type type;
    char **argv;
    int argc;
    int is_builtin;
} t_exec;

typedef struct s_pipe
{
    t_type type;
    t_cmd *left;
    t_cmd *right;
} t_pipe;

typedef struct s_redirs
{
    t_type type;
    t_cmd *cmd;
    int fd;
    int mode;
    char *filename;
} t_redirs;

// DEBUGGER
void print_tokens(t_tokens *tokens);
void print_args(t_exec *exc);
void print_type(t_type type);
void print_cmd(t_cmd *cmd);

void exit_on_error(char *s);

int is_charset(char c, char *cs);
char *strchrs(char *s, char *cs);
char *ft_strndup(char *s, int len);
int ft_strlen(char *s);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char **ft_split(char *s, char c);
size_t ft_strlcpy(char *dst, const char *src, size_t size);

t_tokens *create_token(char *token, t_tok_type TYPE);
void append_token(t_tokens **list, t_tokens *new);
void remove_tokens(t_tokens **tokens, size_t nb_to_remove);

int get_quoted_len(char *str, char quote);

int handle_pipe(t_tokens **tokens, char *cmd, int *i);
int handle_bracket(t_tokens **tokens, char *cmd, int *i);
int handle_text(t_tokens **tokens, char *cmd, int *i);
int handle_quotes(t_tokens **tokens, char *cmd, int *i);
int handle_redir(t_tokens **tokens, char *cmd, int *i);

void get_token_list(char *usr_cmd, t_tokens **tokens);
char *trim_quotes(t_tokens **tokens);

t_cmd *create_exec(void);
t_cmd *create_pipe(t_cmd *left, t_cmd *right);
t_cmd *create_redirs(t_cmd *subcmd, char *filename, int mode, int fd);
t_cmd *parse_tokens(t_tokens **tokens, char **envp);

int cmd_is_builtin(char *cmd);
void run_builtin(t_exec *cmd, char **envp);

int can_exec(char *filepath);
char *get_file_path(char *execname, char **envp);

void runcmd(t_cmd *cmd, char **envp);
void free_cmds(t_cmd *cmd);

#endif