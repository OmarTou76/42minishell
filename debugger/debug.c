#include "../minishell.h"

void	print_redirs(t_redirs *cmd);

void	print_args(t_exec *exc)
{
	char	**argv;
	int		i;

	
	argv = exc->argv;
	i = 0;
	printf("builtin: %s\n", exc->is_builtin ? "TRUE" : "FALSE");
	while (argv[i])
	{
		printf("ARGV[%d]: %s\n", i, argv[i]);
		i++;
	}
}

void	print_pipe(t_pipe *p)
{
	t_exec		*ex;
	t_pipe		*sp;
	t_redirs	*r;

	if (p->left->type == EXEC)
	{
		ex = (t_exec *)p->left;
		print_args(ex);
	}
	else if (p->left->type == PIPE_CMD)
	{
		sp = (t_pipe *)p->left;
		print_pipe(sp);
	}
	else if (p->left->type == REDIR_CMD)
	{
		r = (t_redirs *)p->left;
		print_redirs(r);
	}
	printf("----\n");
	if (p->right->type == EXEC)
	{
		ex = (t_exec *)p->right;
		print_args(ex);
	}
	else if (p->right->type == PIPE_CMD)
	{
		sp = (t_pipe *)p->right;
		print_pipe(sp);
	}
	else if (p->right->type == REDIR_CMD)
	{
		r = (t_redirs *)p->right;
		print_redirs(r);
	}
}

void	print_redirs(t_redirs *cmd)
{
	if (cmd->is_here_doc)
		printf("Stop with: %s\n", cmd->filename);
	else
		printf("to file: %s\n", cmd->filename);
	if (cmd->cmd->type == REDIR_CMD)
		printf("(is sub redirs)\n");
	print_cmd(cmd->cmd);
}

void	print_cmd(t_cmd *cmd)
{
	t_pipe		*p;
	t_exec		*e;
	t_redirs	*r;

	print_type(cmd->type);
	if (cmd->type == EXEC)
	{
		e = (t_exec *)cmd;
		print_args(e);
	}
	else if (cmd->type == PIPE_CMD)
	{
		p = (t_pipe *)cmd;
		print_pipe(p);
	}
	else if (cmd->type == REDIR_CMD)
	{
		r = (t_redirs *)cmd;
		print_redirs(r);
	}
}

void	print_tokens(t_tokens *tokens)
{
	while (tokens)
	{
		printf("[%s] - [%d] - [%s]\n", tokens->cmd, tokens->type, tokens->env_var ? tokens->env_var : "");
		tokens = tokens->next;
	}
}

void	print_type(t_type type)
{
	switch (type)
	{
	case PIPE_CMD:
		printf("PIPE\n");
		break ;
	case REDIR_CMD:
		printf("REDIR\n");
		break ;
	case EXEC:
		printf("EXEC CMD\n");
		break ;

	default:
		printf("CMD NOT FOUND\n");
		break ;
	}
}