/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_types.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:12 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 10:19:13 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe(t_tokens **tokens, char *cmd, int *i)
{
	t_tokens	*token;
	char		*tok;

	token = NULL;
	if (cmd[*i] == '|')
	{
		if (cmd[*i + 1] == cmd[*i])
			return (-1);
		tok = ft_strndup(cmd + (*i), 1);
		if (!tok)
			return (-1);
		token = create_token(tok, PIPE);
		if (!token)
			return (-1);
		append_token(tokens, token);
		(*i)++;
	}
	return (0);
}

int	handle_parentheses(t_tokens **tokens, char *cmd, int *i)
{
	t_tokens	*token;
	char		*tok;

	token = NULL;
	if (cmd[*i] == '(' || cmd[*i] == ')')
	{
		tok = ft_strndup(cmd + (*i), 1);
		if (!tok)
			perror("Error on malloc\n");
		if (cmd[*i] == '(')
			token = create_token(tok, OPEN_BRACKET);
		else
			token = create_token(tok, CLOSE_BRACKET);
		if (!token)
			perror("Error on malloc\n");
		append_token(tokens, token);
		(*i)++;
	}
	return (0);
}

int	handle_text(t_tokens **tokens, char *cmd, int *i)
{
	int			len;
	char		*str;
	char		*tok;
	t_tokens	*token;

	str = cmd + (*i);
	len = 0;
	while (str[len] && !is_charset(str[len], SPACES) && !is_charset(str[len],
			QUOTES) && !is_charset(str[len], "<>|()"))
		len++;
	if (len)
	{
		tok = ft_strndup(cmd + (*i), len);
		if (!tok)
			perror("Error on malloc\n");
		token = create_token(tok, WORD);
		token->env_var = strchrs(token->cmd, "$");
		if (!token)
			perror("Error on malloc\n");
		append_token(tokens, token);
		(*i) += len;
	}
	return (0);
}

int	handle_quotes(t_tokens **tokens, char *cmd, int *i)
{
	int			len;
	char		*tok;
	t_tokens	*token;
	char		quote;

	if (!is_charset(cmd[*i], QUOTES))
		return (0);
	quote = cmd[*i];
	len = get_quoted_len(cmd + (*i), quote);
	if (len < 0)
		return (-1);
	if (len)
	{
		tok = ft_strndup(cmd + (*i) + 1, len - 2);
		if (quote == '\'')
			token = create_token(tok, SINGLE_QUOTE);
		else
		{
			token = create_token(tok, DOUBLE_QUOTE);
			token->env_var = strchrs(token->cmd, "$");
		}
		append_token(tokens, token);
		(*i) += len;
	}
	return (0);
}

int	handle_redir(t_tokens **tokens, char *cmd, int *i)
{
	int			len;
	char		*tok;
	t_tokens	*token;

	len = 0;
	while (cmd[(*i) + len] >= '0' && cmd[(*i) + len] <= '9')
		len++;
	if (cmd[(*i) + len] == '>' || cmd[(*i) + len] == '<')
	{
		if (cmd[(*i) + (len + 1)] == cmd[(*i) + len])
			len++;
		len++;
		tok = ft_strndup(cmd + (*i), len);
		token = create_token(tok, REDIR);
		append_token(tokens, token);
		(*i) += len;
	}
	return (0);
}
