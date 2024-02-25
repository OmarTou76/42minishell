/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:44:49 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/25 19:49:44 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int built_in_env(t_exec *exec, t_list **env_var)
{
    int i;
    char **envp;

    (void)exec;
    envp = build_env(*env_var);
    if (!envp)
    {
        ft_putstr_fd("Mninshell: env: ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        return (errno);
    }
    i = 0;
    while (envp[i])
    {
        ft_putstr_fd(envp[i], STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        i++;
    }
    free_table(&envp);
    return (0);
}