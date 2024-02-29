/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:16:13 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 11:48:48 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "errno.h"
# include "string.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define SPACES " \n\t\r"
# define QUOTES "\"\'"
# define ENV_VAR_ERROR "Error while creating an environment variable"
# define EXIT_ERROR 2
# define BUFFER_SIZE 2048

typedef enum s_tok_type
{
	WORD,
	PIPE,
	REDIR,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	SPACE_SEPARATOR,
}					t_tok_type;

typedef enum s_type
{
	EXEC,
	PIPE_CMD,
	REDIR_CMD,
}					t_type;

typedef struct s_tokens
{
	char			*cmd;
	t_tok_type		type;
	char			*env_var;
	struct s_tokens	*next;
}					t_tokens;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_var
{
	char			*key;
	char			*data;
}					t_var;

typedef struct s_cmd
{
	t_type			type;
}					t_cmd;

typedef struct s_exec
{
	t_type			type;
	char			**argv;
	int				argc;
	int				is_builtin;
}					t_exec;

typedef struct s_pipe
{
	t_type			type;
	t_cmd			*left;
	t_cmd			*right;
}					t_pipe;

typedef struct s_redirs
{
	t_type			type;
	t_cmd			*cmd;
	int				fd;
	int				mode;
	char			*filename;
	int				is_here_doc;
}					t_redirs;

// DEBUGGER
void				print_tokens(t_tokens *tokens);
void				print_args(t_exec *exc);
void				print_type(t_type type);
void				print_cmd(t_cmd *cmd);

void				*ft_calloc(size_t count, size_t size);
void				file_error(char *file, char *error);
void				*load_var_error(char *err_str, t_var *err_var, void *ret);

// UTILS
int					is_alnum(char c);
int					is_charset(char c, char *cs);
char				*strchrs(char *s, char *cs);
char				*ft_strndup(char *s, int len);
int					ft_strlen(char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				**ft_split(char *s, char c);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
void				ft_putstr(char *s);
void				ft_putstr_fd(char *s, int fd);
char				*ft_strdup(char *s);
void				ft_putnl(char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strcat(char *str1, const char *str2);
int					print_export(t_list *env_var);
char				*parse_key(char **str, int *error, int *append);
void				cmd_error_write(char *cmd, char *error);
int					ft_isnum(int c);
int					ft_isalnum(int c);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
char				*ft_itoa(int n);
int					ft_atoi(const char *str);
int					ft_strisnum(const char *str);

// ENVP
int					edit_var(t_list **var_list, char *key, char *value);
int					build_var(char **envp, t_list **var_list);
t_var				*create_var(char *str);
char				*search_var(t_list *var_list, char *key);
char				**build_env(t_list *env_var);

// LIST UTILS
t_tokens			*create_token(char *token, t_tok_type TYPE);
void				append_token(t_tokens **list, t_tokens *new);
void				remove_tokens(t_tokens **tokens, size_t nb_to_remove,
						int rm_str);
void				free_tokens(t_tokens *tokens);
char				*ft_strjoin(char *s1, char const *s2, size_t len);
t_list				*ft_newlist(void *content);
int					ft_lstadd_back(t_list **lst, t_list *new);
void				ft_clearlst(t_list **lst, void (*del)(void *));
void				free_var(void *mem);
void				ft_safe_free(void *mem);
int					ft_listsize(t_list *lst);
int					ft_listsize(t_list *lst);
void				free_table(char ***table);
void				ft_lstremoveif(t_list **alst, void *data_rm, int (*cmp)(),
						void (*free_fct)(void *));

// LEXER
int					handle_pipe(t_tokens **tokens, char *cmd, int *i);
int					handle_parentheses(t_tokens **tokens, char *cmd, int *i);
int					handle_text(t_tokens **tokens, char *cmd, int *i);
int					handle_quotes(t_tokens **tokens, char *cmd, int *i);
int					handle_redir(t_tokens **tokens, char *cmd, int *i);

char				*trim_quotes(t_tokens **tokens);
int					get_quoted_len(char *str, char quote);
int					get_token_list(char *usr_cmd, t_tokens **tokens);

int					tokens_have_conflicts(t_tokens **tokens);
void				merge_tokens(t_tokens **tokens);
void				update_tokens(t_tokens **tokens, t_list *envp);

// PARSING
t_cmd				*create_exec(void);
t_cmd				*create_redirs(t_cmd *subcmd, char *filename, int mode,
						int fd);
t_cmd				*create_pipe(t_cmd *left, t_cmd *right);
t_cmd				*stdout_redirs(t_cmd *cmd, t_tokens **tokens);
t_cmd				*stdin_redirs(t_cmd *cmd, t_tokens **tokens);
t_cmd				*parse_redirs(t_cmd *cmd, t_tokens **tokens);

int					get_arg_count(t_tokens **tokens);
void				get_arg_value(t_cmd **cmd, t_tokens **tokens);
t_cmd				*parse_exec(t_tokens **tokens);

t_cmd				*parse_parentheses(t_tokens **tokens);
t_cmd				*parse_tokens(t_tokens **tokens);
void				replace_by_env(t_tokens *token, t_list *envp_list);

// BUILTINS
int					built_in_echo(t_exec *exec, t_list **env);
int					built_in_pwd(t_exec *e, t_list **envp);
int					built_in_cd(t_exec *exec, t_list **env_var);
int					built_in_env(t_exec *exec, t_list **env_var);
int					built_in_export(t_exec *exec, t_list **env_var);
int					built_in_unset(t_exec *exec, t_list **env_var);
int					built_in_exit(t_exec *exec, t_list **env_var);

// RUN COMMAND
int					cmd_is_builtin(char *cmd);
int					run_builtin(t_exec *cmd, t_list **envp);

void				run_redirs(t_cmd *c, t_list **envp, int run_next);
void				update_cmd(t_cmd **cmd);
void				run_multiple_heredoc(t_redirs *cmd, t_list **envp);
void				run_sub_redirs(t_cmd *c, t_list **envp, int prev_fd);
void				save_heredoc(t_redirs *cmd, char *tmp_file);
void				handle_heredoc(int sig);
int					handle_status(int pid);
void				run_left_cmd(t_pipe *p_cmd, int p[2], t_list **envp);
void				run_right_cmd(t_pipe *p_cmd, int p[2], t_list **envp);
void				run_exec(t_cmd *c, t_list **envp);

int					can_exec(char *filepath);
char				*get_file_path(char *execname, t_list *envp);

void				exec_cmd_by_type(t_cmd *cmd, t_list **envp);
void				runcmd(t_cmd *cmd, t_list **envp);
void				free_redirs(t_redirs *cmd);
void				free_cmds(t_cmd *cmd);

#endif