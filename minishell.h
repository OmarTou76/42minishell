#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <signal.h>
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
    int is_here_doc;
} t_redirs;

// DEBUGGER
void print_tokens(t_tokens *tokens);
void print_args(t_exec *exc);
void print_type(t_type type);
void print_cmd(t_cmd *cmd);

// UTILS
void exit_on_error(char *s);
int is_charset(char c, char *cs);
char *strchrs(char *s, char *cs);
char *ft_strndup(char *s, int len);
int ft_strlen(char *s);
int ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_strcmp(const char *s1, const char *s2);
char **ft_split(char *s, char c);
size_t ft_strlcpy(char *dst, const char *src, size_t size);

// LIST UTILS
t_tokens *create_token(char *token, t_tok_type TYPE);
void append_token(t_tokens **list, t_tokens *new);
void remove_tokens(t_tokens **tokens, size_t nb_to_remove);
void free_tokens(t_tokens *tokens);

// LEXER
int handle_pipe(t_tokens **tokens, char *cmd, int *i);
int handle_parentheses(t_tokens **tokens, char *cmd, int *i);
int handle_text(t_tokens **tokens, char *cmd, int *i);
int handle_quotes(t_tokens **tokens, char *cmd, int *i);
int handle_redir(t_tokens **tokens, char *cmd, int *i);

char *trim_quotes(t_tokens **tokens);
int get_quoted_len(char *str, char quote);
int get_token_list(char *usr_cmd, t_tokens **tokens);

int tokens_have_conflicts(t_tokens **tokens);

// PARSING
t_cmd *create_exec(void);
t_cmd *create_redirs(t_cmd *subcmd, char *filename, int mode, int fd);
t_cmd *create_pipe(t_cmd *left, t_cmd *right);

t_cmd *stdout_redirs(t_cmd *cmd, t_tokens **tokens);
t_cmd *stdin_redirs(t_cmd *cmd, t_tokens **tokens);
t_cmd *parse_redirs(t_cmd *cmd, t_tokens **tokens);

int get_arg_count(t_tokens **tokens);
void get_arg_value(t_cmd **cmd, t_tokens **tokens);
t_cmd *parse_exec(t_tokens **tokens);

t_cmd *parse_parentheses(t_tokens **tokens);
t_cmd *parse_tokens(t_tokens **tokens);

// RUN COMMAND
int cmd_is_builtin(char *cmd);
void run_builtin(t_exec *cmd, char **envp);

int can_exec(char *filepath);
char *get_file_path(char *execname, char **envp);
void get_env_var_from_args(t_exec *e_cmd, char **envp);

void runcmd(t_cmd *cmd, char **envp);
void free_cmds(t_cmd *cmd);

#endif