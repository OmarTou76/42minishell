#include "../minishell.h"

void	run_redirs(t_cmd *c, char **envp, int run_next);
void	exec_cmd_by_type(t_cmd *cmd, char **envp);
void	update_cmd(t_cmd **cmd);

void	run_exec(t_cmd *c, char **envp)
{
	t_exec	*e_cmd;
	char	*filepath;

	e_cmd = (t_exec *)c;
	get_env_var_from_args(e_cmd, envp);
	if (!e_cmd->is_builtin)
	{
		if (!can_exec(e_cmd->argv[0]))
		{
			filepath = get_file_path(e_cmd->argv[0], envp);
			if (!filepath)
				printf("minishell: command not found: %s\n", e_cmd->argv[0]);
			free(e_cmd->argv[0]);
			e_cmd->argv[0] = filepath;
		}
		execve(e_cmd->argv[0], e_cmd->argv, envp);
	}
	else
		run_builtin(e_cmd, envp);
}

void	run_left_cmd(t_pipe *p_cmd, int p[2], char **envp)
{
	close(STDOUT_FILENO);
	dup(p[1]);
	close(p[0]);
	close(p[1]);
	exec_cmd_by_type(p_cmd->left, envp);
}

void	run_right_cmd(t_pipe *p_cmd, int p[2], char **envp)
{
	close(STDIN_FILENO);
	dup(p[0]);
	close(p[0]);
	close(p[1]);
	exec_cmd_by_type(p_cmd->right, envp);
}

void	run_pipe(t_cmd *cmd, char **envp)
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

void	save_heredoc(t_redirs *cmd, char **envp, char *tmp_file)
{
	int		fd;
	char	*line;

	(void)envp;
	fd = open(tmp_file, O_WRONLY | O_APPEND | O_CREAT, 0777);
	while (1)
	{
		// write(STDOUT_FILENO, "heredoc> ", 10);
		line = readline("heredoc> "); // get_next_line(0);
		/* if (ft_strlen(line) > 0)
			line[ft_strlen(line) - 1] = 0; */
		if (line && ft_strcmp(line, cmd->filename) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(cmd->filename);
	cmd->filename = ft_strndup(tmp_file, ft_strlen(tmp_file));
	close(fd);
}

void	run_sub_redirs(t_cmd *c, char **envp)
{
	t_redirs	*cmd;

	cmd = (t_redirs *)c;
	if (cmd->is_here_doc)
		save_heredoc(cmd, envp, "__fake__");
	if (open(cmd->filename, cmd->mode, 0777) < 0)
	{
		printf("%s, ", cmd->filename);
		exit_on_error("No such file");
	};
	if (cmd->is_here_doc)
		unlink(cmd->filename);
	if (cmd->cmd->type == REDIR_CMD)
		run_sub_redirs(cmd->cmd, envp);
	else
		exec_cmd_by_type(cmd->cmd, envp);
}

void	run_multiple_heredoc(t_redirs *cmd, char **envp)
{
	t_redirs	*next;

	if (cmd->cmd->type == REDIR_CMD)
	{
		next = (t_redirs *)cmd->cmd;
		if (next->is_here_doc)
			run_multiple_heredoc(next, envp);
	}
	save_heredoc(cmd, envp, "__fake__");
	unlink("__fake__");
}

void	update_cmd(t_cmd **cmd)
{
	t_redirs	*init;
	t_redirs	*next_cmd;
	t_redirs	*to_remove;

	init = (t_redirs *)*cmd;
	next_cmd = (t_redirs *)init->cmd;
	while (next_cmd->type == REDIR_CMD)
	{
		to_remove = next_cmd;
		next_cmd = (t_redirs *)next_cmd->cmd;
		free(to_remove->filename);
		free(to_remove);
	}
	init->cmd = (t_cmd *)next_cmd;
}

void	run_redirs(t_cmd *c, char **envp, int run_next)
{
	t_redirs	*cmd;
	t_redirs	*next;

	cmd = (t_redirs *)c;
	if (cmd->is_here_doc)
	{
		next = (t_redirs *)cmd->cmd;
		if (cmd->cmd->type == REDIR_CMD && next->is_here_doc)
		{
			run_multiple_heredoc(next, envp);
			update_cmd(&c);
		}
		save_heredoc(cmd, envp, "__tmp__");
	}
	close(cmd->fd);
	if (open(cmd->filename, cmd->mode, 0777) < 0)
	{
		printf("%s, ", cmd->filename);
		exit_on_error("No such file");
	};
	if (cmd->is_here_doc)
		unlink(cmd->filename);
	if (run_next)
	{
		if (cmd->cmd->type == REDIR_CMD)
			run_sub_redirs(cmd->cmd, envp);
		else
			exec_cmd_by_type(cmd->cmd, envp);
	}
}

void	exec_cmd_by_type(t_cmd *cmd, char **envp)
{
	if (cmd->type == EXEC)
		run_exec(cmd, envp);
	else if (cmd->type == PIPE_CMD)
		run_pipe(cmd, envp);
	else if (cmd->type == REDIR_CMD)
		run_redirs(cmd, envp, 1);
}

void	runcmd(t_cmd *cmd, char **envp)
{
	if (fork() == 0)
		exec_cmd_by_type(cmd, envp);
	wait(0);
}