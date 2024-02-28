/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:17:47 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 15:01:31 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_pipe(t_tokens **tokens)
{
	t_cmd		*cmd;
	t_tokens	*tmp;

	cmd = parse_exec(tokens);
	if (cmd == NULL)
		return (cmd);
	if ((*tokens) && (*tokens)->type == PIPE)
	{
		tmp = (*tokens);
		(*tokens) = (*tokens)->next;
		free(tmp->cmd);
		free(tmp);
		cmd = create_pipe(cmd, parse_pipe(tokens));
	}
	return (cmd);
}

t_cmd	*parse_parentheses(t_tokens **tokens)
{
	t_cmd	*cmd;

	remove_tokens(tokens, 1);
	cmd = parse_tokens(tokens);
	if ((*tokens)->type != CLOSE_BRACKET)
		perror("Invalid syntax (close bracket)");
	remove_tokens(tokens, 1);
	cmd = parse_redirs(cmd, tokens);
	return (cmd);
}

t_cmd	*parse_tokens(t_tokens **tokens)
{
	t_cmd	*cmd;

	cmd = parse_pipe(tokens);
	return (cmd);
}
