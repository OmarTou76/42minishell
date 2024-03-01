/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:51 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 10:27:18 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	compute_and_check_path(char *execname, char *path, char target[], int bytes)
{
	ft_strlcpy(target, path, bytes);
	ft_strlcpy(target + ft_strlen(target), "/", bytes);
	ft_strlcpy(target + ft_strlen(target), execname, bytes);
	return (can_exec(target));
}

void	free_split(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}

char	*get_file_path(char *execname, t_list *envp)
{
	char	filepath[1024];
	char	*executable;
	char	**paths;
	int		i;

	i = 0;
	if (!search_var(envp, "PATH"))
		return (NULL);
	paths = ft_split(search_var(envp, "PATH"), ':');
	while (paths[i] && !compute_and_check_path(execname, paths[i], filepath,
			1024))
		i++;
	if (!paths[i])
	{
		free_split(paths);
		return (NULL);
	}
	executable = ft_strndup(filepath, ft_strlen(filepath));
	free_split(paths);
	return (executable);
}

void	save_heredoc(t_redirs *cmd, char *tmp_file)
{
	int		fd;
	char	*line;

	unlink(tmp_file);
	fd = open(tmp_file, O_WRONLY | O_CREAT, 0777);
	while (1)
	{
		signal(SIGINT, handle_heredoc);
		line = readline("heredoc> ");
		if (!line)
		{
			write(1, "\n", 1);
			break ;
		}
		if (line && ft_strcmp(line, cmd->filename) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	free(cmd->filename);
	cmd->filename = ft_strndup(tmp_file, ft_strlen(tmp_file));
	close(fd);
}
