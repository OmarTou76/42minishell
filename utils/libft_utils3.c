/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:05:54 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 11:05:55 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strcat(char *str1, const char *str2)
{
	size_t	i;

	i = 0;
	while (str1[i])
		i++;
	while (*str2)
		str1[i++] = *str2++;
	str1[i] = '\0';
	return (str1);
}

void	ft_putnl(char *str)
{
	ft_putstr(str);
	ft_putstr("\n");
}

int	ft_isalnum(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0'
			&& c <= '9'));
}

int	ft_isnum(int c)
{
	return ((c >= '0' && c <= '9'));
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char unsigned	*cs1;
	char unsigned	*cs2;

	if (!n)
		return (0);
	cs1 = (unsigned char *)s1;
	cs2 = (unsigned char *)s2;
	while (--n && *cs1 == *cs2)
	{
		cs1++;
		cs2++;
	}
	return (*cs1 - *cs2);
}
