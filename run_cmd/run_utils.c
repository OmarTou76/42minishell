/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:27:19 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 11:03:29 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_pipe(t_cmd *cmd, t_list **envp)
{
	t_pipe		*p_cmd;
	t_redirs	*r_cmd;
	int			p[2];
	int			right_pid;

	p_cmd = (t_pipe *)cmd;
	r_cmd = (t_redirs *)p_cmd->left;
	if (p_cmd->left->type == REDIR_CMD && r_cmd->is_here_doc)
	{
		run_redirs((t_cmd *)r_cmd, envp, 0);
		update_cmd(&p_cmd->left);
		p_cmd->left = r_cmd->cmd;
	}
	if (pipe(p) < 0)
		printf("pipe error\n");
	if (fork() == 0)
		run_left_cmd(p_cmd, p, envp);
	right_pid = fork();
	if (right_pid == 0)
		run_right_cmd(p_cmd, p, envp);
	close(p[0]);
	close(p[1]);
	exit(handle_status(right_pid));
}

void	exec_cmd_by_type(t_cmd *cmd, t_list **envp)
{
	if (cmd->type == EXEC)
		run_exec(cmd, envp);
	else if (cmd->type == PIPE_CMD)
		run_pipe(cmd, envp);
	else if (cmd->type == REDIR_CMD)
		run_redirs(cmd, envp, 1);
}

int	cmd_can_be_exec_in_fork(t_exec *exec)
{
	char	*builtin;

	builtin = exec->argv[0];
	if (!exec->is_builtin)
		return (1);
	else if (ft_strncmp(builtin, "echo", ft_strlen(builtin)) == 0)
		return (1);
	else if (ft_strncmp(builtin, "pwd", ft_strlen(builtin)) == 0)
		return (1);
	else if (ft_strncmp(builtin, "env", ft_strlen(builtin)) == 0)
		return (1);
	return (0);
}

void	runcmd(t_cmd *cmd, t_list **envp)
{
	t_exec	*exec;
	int		status;
	int		pid;
	char	*err_code;

	exec = (t_exec *)cmd;
	if (cmd->type == EXEC && !cmd_can_be_exec_in_fork(exec))
		status = run_builtin(exec, envp);
	else
	{
		pid = fork();
		if (pid == 0)
			exec_cmd_by_type(cmd, envp);
		status = handle_status(pid);
	}
	err_code = ft_itoa(status);
	edit_var(envp, "_LAST_EXIT_", err_code);
	free(err_code);
}

void	handle_heredoc(int sig)
{
	if (sig == SIGINT)
		exit(0);
}
