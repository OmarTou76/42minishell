/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:18 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:36:12 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*trim_quotes(t_tokens **tokens)
{
	if ((*tokens)->type != SINGLE_QUOTE && (*tokens)->type != DOUBLE_QUOTE)
		return ((*tokens)->cmd);
	ft_strlcpy((*tokens)->cmd, (*tokens)->cmd + 1, ft_strlen((*tokens)->cmd)
		- 1);
	return ((*tokens)->cmd);
}

int	get_quoted_len(char *str, char quote)
{
	int	len;

	len = 1;
	while (str[len] && str[len] != quote)
		len++;
	if (str[len] != quote)
		return (-1);
	len++;
	return (len);
}

int	save_spaces(t_tokens **tokens, char *cmd, int *i)
{
	int			len;
	char		*str;
	char		*tok;
	t_tokens	*token;

	str = cmd + (*i);
	len = 0;
	while (str[len] && is_charset(str[len], SPACES))
		len++;
	if (len)
	{
		tok = ft_strndup(cmd + (*i), len);
		if (!tok)
			perror("Error on malloc\n");
		token = create_token(tok, SPACE_SEPARATOR);
		if (!token)
			perror("Error on malloc\n");
		append_token(tokens, token);
		(*i) += len;
	}
	return (0);
}

int	get_token_list(char *usr_cmd, t_tokens **tokens)
{
	int	i;

	i = 0;
	*tokens = NULL;
	while (usr_cmd && usr_cmd[i])
	{
		if (handle_pipe(tokens, usr_cmd, &i))
			break ;
		else if (handle_parentheses(tokens, usr_cmd, &i))
			break ;
		else if (handle_redir(tokens, usr_cmd, &i))
			break ;
		else if (handle_text(tokens, usr_cmd, &i))
			break ;
		else if (handle_quotes(tokens, usr_cmd, &i))
			break ;
		while (save_spaces(tokens, usr_cmd, &i))
			break ;
	}
	if (usr_cmd[i])
	{
		printf("Error on parsing at %c (%d)\n", usr_cmd[i], i);
		free_tokens(*tokens);
		return (-1);
	}
	else if (tokens_have_conflicts(tokens))
	{
		printf("Syntax error\n");
		free_tokens(*tokens);
		return (-1);
	}
	return (0);
}

