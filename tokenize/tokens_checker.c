/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:14 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:36:18 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_parentheses(t_tokens *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->type == OPEN_BRACKET)
		{
			if (token->next && token->next->type == CLOSE_BRACKET)
				return (-1);
			count++;
		}
		else if (token->type == CLOSE_BRACKET)
			count--;
		if (count < 0)
			return (1);
		token = token->next;
	}
	if (count > 0)
		return (1);
	return (0);
}

static int	check_pipe_order(t_tokens *token)
{
	if (token && token->type == PIPE)
		return (-1);
	while (token)
	{
		if (token->type == PIPE)
		{
			if (!token->next)
				return (-1);
			else if (token->next->type == PIPE
				|| token->next->type == CLOSE_BRACKET)
				return (-1);
		}
		else if (token->type == OPEN_BRACKET && token->next
			&& token->next->type == PIPE)
			return (-1);
		token = token->next;
	}
	return (0);
}

static void	update_redirs(t_tokens **tokens)
{
	t_tokens	*token;
	t_tokens	*last;

	token = *tokens;
	last = NULL;
	while (token)
	{
		if (token->type == REDIR && token->next && token->next->type == REDIR)
		{
			if (last)
			{
				free_tokens(last->next);
				last->next = NULL;
			}
			else
			{
				free_tokens(token);
				token = NULL;
			}
			break ;
		}
		if (token->type == WORD)
			last = token;
		token = token->next;
	}
}

static int	check_redirs(t_tokens *token)
{
	while (token)
	{
		if (token->type == REDIR && !token->next)
			return (1);
		token = token->next;
	}
	return (0);
}

int	tokens_have_conflicts(t_tokens **tokens)
{
	if (check_parentheses(*tokens))
		return (-1);
	else if (check_pipe_order(*tokens))
		return (-1);
	else if (check_redirs(*tokens))
		return (-1);
	update_redirs(tokens);
	return (0);
}