/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:21 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 11:13:04 by otourabi         ###   ########.fr       */
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
		while (rest[i] && (rest[i] > 40 && rest[i] < 127) && (rest[i - 1] != 63 && i == 1))
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
