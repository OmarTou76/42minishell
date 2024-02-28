/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:28 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 11:15:50 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	file_error(char *file, char *error)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*ptr;
	size_t	i;

	ptr = malloc(count * size);
	if (!ptr)
		return (0);
	i = -1;
	while (++i < size * count)
		ptr[i] = 0;
	return (ptr);
}

void	*load_var_error(char *err_str, t_var *err_var, void *ret)
{
	if (err_var)
		free_var(err_var);
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(err_str, STDERR_FILENO);
	if (errno != 0)
	{
		ft_putstr_fd(" ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	return (ret);
}

void	cmd_error_write(char *cmd, char *error)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

static void	free_elem(char **table, int max)
{
	int	i;

	i = 0;
	while (i <= max)
		ft_safe_free(table[i++]);
	free(table);
}

void	*return_and_free(char **envp, int i)
{
	free_elem(envp, i);
	return (NULL);
}
