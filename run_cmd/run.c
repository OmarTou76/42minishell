#include "../minishell.h"

void	run_redirs(t_cmd *c, t_list **envp, int run_next);
void	exec_cmd_by_type(t_cmd *cmd, t_list **envp);
void	update_cmd(t_cmd **cmd);


void	refresh_argc(t_exec *e_cmd){
	int i;

	i = 0;
	while(e_cmd->argv[i])
		i++;
	e_cmd->argc = i;
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
		run_builtin(e_cmd, envp);
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

void	run_pipe(t_cmd *cmd, t_list **envp)
{
	t_pipe		*p_cmd;
	t_redirs	*r_cmd;
	int			p[2];

	p_cmd = (t_pipe *)cmd;
	r_cmd = (t_redirs *)p_cmd->left;
	if (p_cmd->left->type == REDIR_CMD && r_cmd->is_here_doc)
	{
		run_redirs((t_cmd *)r_cmd, envp, 0);
		update_cmd(&p_cmd->left);
		p_cmd->left = r_cmd->cmd;
	}
	if (pipe(p) < 0)
		exit_on_error("Pipe function");
	if (fork() == 0)
		run_left_cmd(p_cmd, p, envp);
	if (fork() == 0)
		run_right_cmd(p_cmd, p, envp);
	close(p[0]);
	close(p[1]);
	wait(NULL);
	wait(NULL);
	exit(0);
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
	return (0);
}

void	runcmd(t_cmd *cmd, t_list **envp)
{
	t_exec *exec;
	int status;
	int pid;

	exec = (t_exec *)cmd;
	if (cmd->type == EXEC && !cmd_can_be_exec_in_fork(exec))
		run_builtin(exec, envp);
	else
	{
		pid = fork();
		if (pid == 0)
			exec_cmd_by_type(cmd, envp);
		waitpid(pid, &status, 0);
		/* if (WIFEXITED(status))
			printf("OK: [%d]\n", WEXITSTATUS(status));
		if (WIFSIGNALED(status))
			printf("ERROR: [%d]\n", WTERMSIG(status)); */
	}
}