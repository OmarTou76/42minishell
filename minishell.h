#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "string.h"

#define SPACES " \n\t\r"
#define QUOTES "\"\'"
#define true 1
#define false 0
#define ENV_VAR_ERROR	"Error while creating an environment variable"
#define EXIT_ERROR 2
#define BUFFER_SIZE 2048



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

typedef enum e_bool
{
	FALSE,
	TRUE
}	        t_bool;

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

typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}              t_list;

typedef struct s_var
{
    char *key;
    char *data;
}              t_var;       

// DEBUGGER
void print_tokens(t_tokens *tokens);
void print_args(t_exec *exc);
void print_type(t_type type);
void print_cmd(t_cmd *cmd);

/* LIST_UTILS */
t_list *ft_newlist(void *content);
int ft_lstadd_back(t_list **lst, t_list *new);
void    ft_clearlst(t_list **lst, void (*del)(void *));
void    free_var(void *mem);
void ft_safe_free(void *mem);
int ft_listsize(t_list *lst);int ft_listsize(t_list *lst);
void    free_table(char ***table);
void ft_lstremoveif(t_list **alst, void *data_rm, int (*cmp)(), void (*free_fct)(void *));

/* ERROR_UTILS*/
void exit_on_error(char *s);
void	file_error(char *file, char *error);
void	*load_var_error(char *err_str, t_var *err_var, void *ret);

/* LIBFT_UTILS */
int is_charset(char c, char *cs);
char *strchrs(char *s, char *cs);
char *ft_strndup(char *s, int len);
int	ft_strlen(const char *s);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char **ft_split(char *s, char c);
size_t ft_strlcpy(char *dst, const char *src, size_t size);
void	ft_putstr(char *s);
void	ft_putstr_fd(char *s, int fd);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s);
void	ft_putnl(char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strcat(char *str1, const char *str2);
 
 /* TOKENIZE */
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

/* EXEC */
t_cmd *create_exec(void);
t_cmd *create_pipe(t_cmd *left, t_cmd *right);
t_cmd *create_redirs(t_cmd *subcmd, char *filename, int mode, int fd);
t_cmd *parse_tokens(t_tokens **tokens, char **envp);
void runcmd(t_cmd *cmd, char **envp);
void free_cmds(t_cmd *cmd);

/* ENV */
// static t_bool add_var(t_list **var_list, char *key, char *value);
t_bool edit_var(t_list **var_list, char *key, char *value);
t_var *create_var(char *str);
char *search_var(t_list *var_list, char *key);
// static t_bool build_var(char **envp, t_list **var_list);
void *return_and_free(char **envp, int i);
char **build_env(t_list *env_var);
static t_bool build_var(char **envp, t_list **var_list);

/* BUILTINS */
int built_in_echo(int ac, char **av);
//int built_in_exit(int ac, char **av, t_list **env_var, t_bool in_pipes);
int built_in_pwd(void);
int built_in_env(t_list *env_var);
int built_in_cd(int argc, char **argv, t_list **env_var);
int built_in_unset(int argc, char **argv, t_list **env_var);

#endif