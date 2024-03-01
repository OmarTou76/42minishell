/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:56 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 10:44:38 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_redirs(t_cmd *c, t_list **envp, int run_next);
void	exec_cmd_by_type(t_cmd *cmd, t_list **envp);
void	update_cmd(t_cmd **cmd);

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

void	run_executable(t_exec *e_cmd, t_list **envp)
{
	char	*filepath;

	filepath = get_file_path(e_cmd->argv[0], *envp);
	if (filepath)
	{
		free(e_cmd->argv[0]);
		e_cmd->argv[0] = filepath;
	}
	else if (!can_exec(e_cmd->argv[0]))
	{
		free(filepath);
		printf("minishell: command not found: %s\n", e_cmd->argv[0]);
		exit(127);
	}
	execve(e_cmd->argv[0], e_cmd->argv, build_env(*envp)); perror("minishell");
	exit(126);
}

void	run_exec(t_cmd *c, t_list **envp)
{
	t_exec	*e_cmd;

	e_cmd = (t_exec *)c;
	if (!ft_strlen(e_cmd->argv[0]))
		exit(0);
	if (!e_cmd->is_builtin)
		run_executable(e_cmd, envp);
	else
		exit(run_builtin(e_cmd, envp));
	exit(1);
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
