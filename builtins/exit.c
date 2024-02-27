/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:45:10 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/27 12:23:41 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int built_in_exit(t_exec *exec, t_list **env_var)
{
    int ret;
    //(void)env_var;

    ft_putstr_fd("exit\n", 2);
    ft_clearlst(env_var, free_var);
    if (exec->argv[1] && exec->argv[2])
    {
        ret = 1;
        ft_putstr_fd("Minishell: exit: too many arguments\n", 2);
    }
    else if (exec->argv[1] && ft_strisnum(exec->argv[1]) == 0)
    {
        ret = 2;
        ft_putstr_fd("Minishell: exit: ", 2);
        ft_putstr_fd(exec->argv[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(2);
    }
    else if (exec->argv[1])
        exit(ft_atoi(exec->argv[1]) % 256);
    else
        exit(0);
    return (ret);
}