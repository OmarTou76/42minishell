/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:56 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:27:40 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_redirs(t_cmd *c, t_list **envp, int run_next);
void	exec_cmd_by_type(t_cmd *cmd, t_list **envp);
void	update_cmd(t_cmd **cmd);

void	refresh_argc(t_exec *e_cmd)
{
	int	i;

	i = 0;
	while (e_cmd->argv[i])
		i++;
	e_cmd->argc = i;
}

int	handle_status(int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status);
	return (status);
}

void	run_exec(t_cmd *c, t_list **envp)
{
	t_exec	*e_cmd;
	char	*filepath;

	e_cmd = (t_exec *)c;
	refresh_argc(e_cmd);
	if (!e_cmd->is_builtin)
	{
		if (!can_exec(e_cmd->argv[0]))
		{
			filepath = get_file_path(e_cmd->argv[0], *envp);
			if (!filepath)
			{
				printf("minishell: command not found: %s\n", e_cmd->argv[0]);
				exit(127);
			}
			free(e_cmd->argv[0]);
			e_cmd->argv[0] = filepath;
		}
		if (e_cmd->argc)
			execve(e_cmd->argv[0], e_cmd->argv, build_env(*envp));
	}
	else
		exit(run_builtin(e_cmd, envp));
	exit(0);
}

void	run_left_cmd(t_pipe *p_cmd, int p[2], t_list **envp)
{
	close(STDOUT_FILENO);
	dup(p[1]);
	close(p[0]);
	close(p[1]);
	exec_cmd_by_type(p_cmd->left, envp);
}

void	run_right_cmd(t_pipe *p_cmd, int p[2], t_list **envp)
{
	close(STDIN_FILENO);
	dup(p[0]);
	close(p[0]);
	close(p[1]);
	exec_cmd_by_type(p_cmd->right, envp);
}
