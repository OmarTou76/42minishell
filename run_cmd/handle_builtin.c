/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:45 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 11:06:05 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

builtin_func	get_builtin_func(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (&built_in_echo);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (&built_in_pwd);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (&built_in_cd);
	else if (ft_strcmp(cmd, "env") == 0)
		return (&built_in_env);
	else if (ft_strcmp(cmd, "export") == 0)
		return (&built_in_export);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (&built_in_unset);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (&built_in_exit);
	return (NULL);
}

int	run_builtin(t_exec *cmd, t_list **envp)
{
	builtin_func	exec_builtin;

	exec_builtin = get_builtin_func(cmd->argv[0]);
	return (exec_builtin(cmd, envp));
}

int	can_exec(char *filepath)
{
	struct stat	sb;

	return (!stat(filepath, &sb));
}
