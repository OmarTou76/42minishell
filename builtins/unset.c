/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:18:42 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 10:34:12 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	cmp_var(t_var *var, t_var *rm)
{
	return (ft_strcmp(var->key, rm->key));
}

int	built_in_unset(t_exec *exec, t_list **env_var)
{
	int		i;
	t_var	var;

	i = 0;
	while (i < exec->argc)
	{
		var.key = exec->argv[i++];
		ft_lstremoveif(env_var, &var, cmp_var, free_var);
	}
	return (0);
}
