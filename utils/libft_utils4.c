/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:05:51 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 11:05:51 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strisnum(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[0] == '-')
		i++;
	if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	nb;
	int	sign;

	sign = 0;
	nb = 0;
	i = 0;
	while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\v'
		|| *str == '\r' || *str == '\f')
		str++;
	if (*str == '-')
		sign = 1;
	if (*str == '-' || *str == '+')
		str++;
	while (str[i] >= 48 && str[i] <= 57)
	{
		nb = (str[i] - 48) + (10 * nb);
		i++;
	}
	if (sign == 1)
		nb = nb * -1;
	return (nb);
}
