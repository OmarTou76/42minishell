/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:35 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 10:19:35 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_get_nbr_dec(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	ft_insert_nb(char *res, long nb, int len)
{
	if (nb == 0)
	{
		res[0] = '0';
		res[len] = 0;
		return ;
	}
	if (nb < 0)
	{
		nb = -nb;
		res[0] = '-';
	}
	res[len--] = 0;
	while (nb)
	{
		res[len--] = '0' + (nb % 10);
		nb /= 10;
	}
}

char	*ft_itoa(int n)
{
	long	nb;
	char	*res;
	int		len;

	nb = n;
	len = ft_get_nbr_dec(nb);
	if (nb < 0)
		len++;
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	ft_insert_nb(res, nb, len);
	return (res);
}
