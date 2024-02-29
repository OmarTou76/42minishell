/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_tokens_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:31:41 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 10:40:01 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if ((*tokens)->type == SPACE_SEPARATOR)
		remove_tokens(tokens, 1);
	remove_spaces(tokens);
}
