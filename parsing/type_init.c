/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:17:54 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 10:17:55 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*create_redirs(t_cmd *subcmd, char *filename, int mode, int fd)
{
	t_redirs	*redirs;

	redirs = malloc(sizeof(t_redirs));
	redirs->type = REDIR_CMD;
	redirs->cmd = subcmd;
	redirs->fd = fd;
	redirs->filename = filename;
	redirs->mode = mode;
	redirs->is_here_doc = 0;
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
