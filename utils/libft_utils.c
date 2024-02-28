/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:41 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 11:08:22 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_alnum(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0'
			&& c <= '9'))
		return (1);
	return (0);
}

int	is_charset(char c, char *cs)
{
	int	i;

	i = 0;
	while (cs[i])
	{
		if (cs[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*strchrs(char *s, char *cs)
{
	while (*s)
	{
		if (is_charset(*s, cs))
			return (s);
		s++;
	}
	return (NULL);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char	*ft_strndup(char *s, int len)
{
	int		i;
	char	*new;

	i = 0;
	if (!s)
		return (NULL);
	if (ft_strlen(s) < len)
		len = ft_strlen(s);
	new = malloc(len + 1);
	while (s[i] && i < len)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = 0;
	return (new);
}
