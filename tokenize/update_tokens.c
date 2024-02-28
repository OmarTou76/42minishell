/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:21 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 14:13:00 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*join_str(char *start, char *env, char *end)
{
	char	*res;

	if (env)
		res = ft_strjoin(start, env, ft_strlen(env));
	else
	{
		res = ft_strdup(start);
		free(start);
	}
	if (end)
		res = ft_strjoin(res, end, ft_strlen(end));
	return (res);
}

void	replace_by_env(t_tokens *token, t_list *envp_list)
{
	char	*start;
	char	*rest;
	int		i;
	char	*env;

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
		while (rest[i] && (rest[i] > 32 && rest[i] < 127))
			i++;
		env = ft_strndup(rest, i);
		if (ft_strcmp(env, "$?") == 0)
		{
			free(env);
			env = ft_strdup("$_LAST_EXIT_");
		}
		free(token->cmd);
		token->cmd = join_str(start, search_var(envp_list, env + 1), rest + i);
		token->env_var = strchrs(token->cmd, "$");
		free(env);
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
		return ;
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
