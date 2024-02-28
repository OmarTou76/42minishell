/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:45 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 12:54:46 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_is_builtin(char *cmd)
{
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

typedef int		(*builtin_func)(t_exec *, t_list **envp);

// Pour les tests
void	ft_echo(t_exec *cmd, char **envp)
{
	int	i;

	i = 1;
	(void)envp;
	while (cmd->argv[i])
	{
		printf("%s", cmd->argv[i]);
		i++;
		if (cmd->argv[i])
			printf(" ");
		else
			printf("\n");
	}
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
