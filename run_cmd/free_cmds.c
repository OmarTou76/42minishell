/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:04 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 12:54:34 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_exec(t_exec *cmd)
{
	char	**argv;
	int		i;

	argv = cmd->argv;
	if (argv)
	{
		i = 0;
		while (argv[i])
		{
			free(argv[i]);
			i++;
		}
		free(argv);
	}
	free(cmd);
}

void	free_redirs(t_redirs *cmd)
{
	free_cmds(cmd->cmd);
	free(cmd->filename);
	free(cmd);
}

static void	free_pipe(t_pipe *p);

static void	free_side_pipe(t_cmd *cmd)
{
	t_exec		*ex;
	t_pipe		*sp;
	t_redirs	*red;

	if (cmd->type == EXEC)
	{
		ex = (t_exec *)cmd;
		free_exec(ex);
	}
	else if (cmd->type == PIPE_CMD)
	{
		sp = (t_pipe *)cmd;
		free_pipe(sp);
	}
	else if (cmd->type == REDIR_CMD)
	{
		red = (t_redirs *)cmd;
		free_redirs(red);
	}
}

static void	free_pipe(t_pipe *p)
{
	free_side_pipe(p->left);
	free_side_pipe(p->right);
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
