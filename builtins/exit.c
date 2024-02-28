/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:45:10 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 10:48:46 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	built_in_exit(t_exec *exec, t_list **env_var)
{
	int	ret;

	ft_putstr_fd("exit\n", 2);
	ft_clearlst(env_var, free_var);
	if (exec->argv[1] && exec->argv[2])
		ft_putstr_fd("Minishell: exit: too many arguments\n", 2);
	else if (exec->argv[1] && ft_strisnum(exec->argv[1]) == 0)
	{
		cmd_error_write(exec->argv[1], "numeric argument required");
		free_cmds((t_cmd *)exec);
		exit(2);
	}
	else if (exec->argv[1])
	{
		ret = ft_atoi(exec->argv[1]) % 256;
		free_cmds((t_cmd *)exec);
		exit(ret);
	}
	else
	{
		free_cmds((t_cmd *)exec);
		exit(0);
	}
	return (0);
}
