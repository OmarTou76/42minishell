/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:54:51 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 10:04:23 by otourabi         ###   ########.fr       */
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
