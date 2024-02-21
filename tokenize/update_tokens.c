#include "../minishell.h"

int can_merge(t_tokens *next)
{
	if (!next || next->type == PIPE || next->type == REDIR || next->type == OPEN_BRACKET || next->type == CLOSE_BRACKET || next->type == SPACE_SEPARATOR)
		return (0);
	return (1);
}

void merge_tokens(t_tokens **tokens)
{
	t_tokens *token;
	t_tokens *tmp;

	token = *tokens;
	while (token)
	{
		if (can_merge(token) && can_merge(token->next))
		{
			token->type = WORD;
			token->cmd = ft_strjoin(token->cmd, token->next->cmd,
									ft_strlen(token->next->cmd));
			tmp = token->next;
			token->next = token->next->next;
			free(tmp->cmd);
			free(tmp);
		}
		else
			token = token->next;
	}
}

char *get_env(char *s)
{
	char *result;

	(void)s;
	if (s[1] <= 97)
		result = ft_strndup("__ENV__VAR__", ft_strlen("__ENV__VAR__"));
	else
		result = ft_strndup("", ft_strlen(""));
	return (result);
}

void replace_by_env(t_tokens *token)
{
	char *start;
	char *rest;
	int i;
	char *env;
	char *tmp;
	char *element;

	while (token->env_var)
	{
		start = ft_strndup(token->cmd, (int)(token->env_var - token->cmd));
		rest = ft_strndup(token->cmd + (token->env_var - token->cmd),
						  ft_strlen(token->cmd));
		i = 1;
		while (rest[i] && is_alnum(rest[i]))
			i++;
		env = ft_strndup(rest, i);
		tmp = rest;
		rest = ft_strndup(rest + i, ft_strlen(rest + i));
		free(tmp);
		element = get_env(env);
		free(token->cmd);
		token->cmd = ft_strjoin(start, element, ft_strlen(element));
		token->cmd = ft_strjoin(token->cmd, rest, ft_strlen(rest));
		token->env_var = strchrs(token->cmd, "$");
		free(element);
		free(rest);
	}
}

void affect_env_var(t_tokens **tokens, char **envp)
{
	t_tokens *token;

	(void)envp;
	token = *tokens;
	while (token)
	{
		if (token->env_var && token->env_var[1])
			replace_by_env(token);
		token = token->next;
	}
}

void remove_spaces(t_tokens **tokens)
{
	t_tokens *token;
	t_tokens *to_remove;

	token = *tokens;
	while (token && token->next)
	{
		if (token->next->type == SPACE_SEPARATOR)
		{
			to_remove = token->next;
			token->next = token->next->next;
			free(to_remove->cmd);
			free(to_remove);
		}
		else
			token = token->next;
	}
}

void update_tokens(t_tokens **tokens, char **envp)
{
	affect_env_var(tokens, envp);
	merge_tokens(tokens);
	remove_spaces(tokens);
}