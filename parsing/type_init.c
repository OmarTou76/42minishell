#include "../minishell.h"

#define builtin ""

int	cmd_is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", ft_strlen(cmd)) == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo", ft_strlen(cmd)) == 0)
		return (1);
    else if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
		return (1);
    else if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
		return (1);
    else if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
		return (1);
    else if (ft_strncmp(cmd, "env", ft_strlen(cmd)) == 0)
		return (1);
    else if (ft_strncmp(cmd, "exit", ft_strlen(cmd)) == 0)
		return (1);
	return (0);
}

t_cmd	*create_redirs(t_cmd *subcmd, char *filename, int mode, int fd)
{
	t_redirs	*redirs;

	redirs = malloc(sizeof(t_redirs));
	redirs->type = REDIR_CMD;
	redirs->cmd = subcmd;
	redirs->fd = fd;
	redirs->filename = filename;
	redirs->mode = mode;
	return ((t_cmd *)redirs);
}

t_cmd	*create_exec(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->type = EXEC;
	cmd->argc = 0;
	cmd->argv = NULL;
	cmd->is_builtin = 0;
	return ((t_cmd *)cmd);
}

t_cmd	*create_pipe(t_cmd *left, t_cmd *right)
{
	t_pipe	*cmd;

	cmd = malloc(sizeof(t_pipe));
	cmd->type = PIPE_CMD;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}
