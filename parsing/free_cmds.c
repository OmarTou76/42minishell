#include "../minishell.h"

static void	free_exec(t_exec *cmd)
{
	char	**argv;
	int		i;

	argv = cmd->argv;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	free(cmd);
}

static void	free_redirs(t_redirs *cmd)
{
	free_cmds(cmd->cmd);
	free(cmd->filename);
	free(cmd);
}

static void	free_pipe(t_pipe *p)
{
	t_exec		*ex;
	t_pipe		*sp;
	t_redirs	*red;

	ex = (t_exec *)p->left;
	free_exec(ex);
	if (p->right->type == EXEC)
	{
		ex = (t_exec *)p->right;
		free_exec(ex);
	}
	else if (p->right->type == PIPE_CMD)
	{
		sp = (t_pipe *)p->right;
		free_pipe(sp);
	} else if (p->right->type == REDIR_CMD)
	{
		red = (t_redirs *)p->right;
		free_redirs(red);
	}
	free(p);
}


void	free_cmds(t_cmd *cmd)
{
	t_pipe		*p;
	t_exec		*e;
	t_redirs	*r;

	if (cmd->type == EXEC)
	{
		e = (t_exec *)cmd;
		free_exec(e);
	}
	else if (cmd->type == PIPE_CMD)
	{
		p = (t_pipe *)cmd;
		free_pipe(p);
	}
	else if (cmd->type == REDIR_CMD)
	{
		r = (t_redirs *)cmd;
		free_redirs(r);
	}
}
