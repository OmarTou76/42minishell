/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:44:44 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/23 18:36:03 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_option(char *str)
{
	if (*str != '-')
		return (0);
	str++;
	while (*str == 'n')
		str++;
	if (*str == '\0')
		return (1);
	else
		return (0);
}

static int	number_of_options(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_option(argv[i]))
			return (i);
		i++;
	}
	return (i);
}

int	built_in_echo(t_cmd *cmd)
{
	t_exec *exec;
	int i;
	int n_option;

	exec = (t_exec *)cmd;
	n_option = 0;
	i = number_of_options(exec->argc, exec->argv);
	if (i > 1)
		n_option = 1;
	while (i < exec->argc)
	{
		ft_putstr(exec->argv[i]);
		if ((i + 1) < exec->argc)
			ft_putstr(" ");
		i++;
	}
	if (n_option != 0)
		ft_putstr("\n");
	return (0);
}