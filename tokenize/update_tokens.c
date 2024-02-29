/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:21 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 13:24:12 by otourabi         ###   ########.fr       */
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

void	update_str(t_tokens *token, t_list *envp_list)
{
	char	*start;
	char	*rest;
	char	*env;
	int		i;

	start = ft_strndup(token->cmd, (int)(token->env_var - token->cmd));
	rest = ft_strndup(token->cmd + (token->env_var - token->cmd),
			ft_strlen(token->cmd));
	i = 1;
	while (rest[i] && (rest[i] > 32 && rest[i] < 127) && rest[i] != '$')
	{
		if (i > 1 && ft_strncmp(rest, "$?", i) == 0)
			break ;
		i++;
	}
	if (ft_strncmp(rest, "$?", i) == 0)
		env = ft_strdup("$_LAST_EXIT_");
	else
		env = ft_strndup(rest, i);
	free(token->cmd);
	token->cmd = join_str(start, search_var(envp_list, env + 1), rest + i);
	token->env_var = strchrs(token->cmd, "$");
	free(env);
	free(rest);
}

void	replace_by_env(t_tokens *token, t_list *envp_list)
{
	int	c;

	while (token->env_var)
	{
		c = token->env_var[1];
		while (token->env_var && (!c || (c <= 32 && c <= 127)))
			token->env_var = strchrs(token->env_var + 1, "$");
		if (!token->env_var)
			break ;
		update_str(token, envp_list);
	}
}
