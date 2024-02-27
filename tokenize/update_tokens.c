#include "../minishell.h"

int	can_merge(t_tokens *next)
{
	if (!next || next->type == PIPE || next->type == REDIR
		|| next->type == OPEN_BRACKET || next->type == CLOSE_BRACKET
		|| next->type == SPACE_SEPARATOR)
		return (0);
	return (1);
}

void	merge_tokens(t_tokens **tokens)
{
	t_tokens	*token;
	t_tokens	*tmp;

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

int	is_possible_env(char *str)
{
	if (!str[1] || (str[1] <= 32 && str[1] <= 127))
		return (0);
	return (1);
}

void	replace_by_env(t_tokens *token, t_list *envp_list)
{
	char	*start;
	char	*rest;
	int		i;
	char	*env;
	char	*tmp;
	char	*env_value;

	while (token->env_var)
	{
		while (token->env_var && !is_possible_env(token->env_var))
			token->env_var = strchrs(token->env_var + 1, "$");
		if (!token->env_var)
			break ;
		start = ft_strndup(token->cmd, (int)(token->env_var - token->cmd));
		rest = ft_strndup(token->cmd + (token->env_var - token->cmd),
				ft_strlen(token->cmd));
		i = 1;
		while (rest[i] && (is_alnum(rest[i]) || rest[i] == '-'
				|| rest[i] == '_'))
			i++;
		env = ft_strndup(rest, i);
		tmp = rest;
		rest = ft_strndup(rest + i, ft_strlen(rest + i));
		free(tmp);
		free(token->cmd);
		env_value = search_var(envp_list, env + 1);
		if (env_value)
			token->cmd = ft_strjoin(start, env_value, ft_strlen(env_value));
		else
			token->cmd = ft_strdup(start);
		token->cmd = ft_strjoin(token->cmd, rest, ft_strlen(rest));
		token->env_var = strchrs(token->cmd, "$");
		free(rest);
	}
}

void	affect_env_var(t_tokens **tokens, t_list *envp_list)
{
	t_tokens	*token;

	token = *tokens;
	while (token)
	{
		if (token->env_var && token->env_var[1])
			replace_by_env(token, envp_list);
		token = token->next;
	}
}

void	remove_spaces(t_tokens **tokens)
{
	t_tokens	*token;
	t_tokens	*to_remove;

	token = *tokens;
	if (token->type == SPACE_SEPARATOR && !token->next)
	{
		free((*tokens)->cmd);
		free((*tokens));
		*tokens = NULL;
	}
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

void	update_tokens(t_tokens **tokens, t_list *envp_list)
{
	affect_env_var(tokens, envp_list);
	merge_tokens(tokens);
	remove_spaces(tokens);
}