#include "../minishell.h"

void	handle_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		exit(0);
	}
	if (sig == SIGQUIT)
	{

	}
}

void	save_heredoc(t_redirs *cmd, t_list **envp, char *tmp_file)
{
	int		fd;
	char	*line;

	(void)envp;
	unlink(tmp_file);
	fd = open(tmp_file, O_WRONLY | O_CREAT, 0777);
	while (1)
	{
		signal(SIGINT, handle_heredoc);
		signal(SIGQUIT, handle_heredoc);
		line = readline("heredoc> ");
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

void	run_sub_redirs(t_cmd *c, t_list **envp, int prev_fd)
{
	t_redirs	*cmd;

	cmd = (t_redirs *)c;
	if (cmd->is_here_doc)
		save_heredoc(cmd, envp, "__fake__");
	if (prev_fd != cmd->fd)
		close(cmd->fd);
	if (open(cmd->filename, cmd->mode, 0777) < 0)
	{
		printf("%s, ", cmd->filename);
		exit_on_error("No such file");
	};
	if (cmd->is_here_doc)
		unlink(cmd->filename);
	if (cmd->cmd->type == REDIR_CMD)
		run_sub_redirs(cmd->cmd, envp, cmd->fd);
	else
		exec_cmd_by_type(cmd->cmd, envp);
}

void	run_multiple_heredoc(t_redirs *cmd, t_list **envp)
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

void	run_redirs(t_cmd *c, t_list **envp, int run_next)
{
	t_redirs *cmd;
	t_redirs *next;

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
			run_sub_redirs(cmd->cmd, envp, cmd->fd);
		else
			exec_cmd_by_type(cmd->cmd, envp);
	}
}