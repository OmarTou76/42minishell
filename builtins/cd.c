/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:18:26 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 11:07:13 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_cd_error(char *error, char *path)
{
	ft_putstr_fd("Minishell: cd ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

static int	cd_home(t_list **env_var)
{
	char	*home;

	home = search_var(*env_var, "HOME");
	if (!home)
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		return (1);
	}
	else if (chdir(home) == -1)
		return (print_cd_error(strerror(errno), "HOME"));
	return (0);
}

static int	get_dir(char **old_pwd, char *actual_dir, t_list **env_var,
		char *var_edited)
{
	if (old_pwd)
	{
		*old_pwd = ft_strdup(*old_pwd);
		if (!*old_pwd)
		{
			file_error("cannot load OLDPWD", strerror(errno));
			return (0);
		}
	}
	if (!getcwd(actual_dir, BUFFER_SIZE))
	{
		file_error("cannot get PWD", strerror(errno));
		if (old_pwd)
			free(*old_pwd);
		return (0);
	}
	if (!edit_var(env_var, var_edited, actual_dir))
	{
		if (old_pwd)
			free(*old_pwd);
		return (0);
	}
	return (1);
}

static int	cd_oldpwd(t_list **env_var)
{
	char	*old_pwd;
	char	actual_dir[BUFFER_SIZE];

	old_pwd = search_var(*env_var, "OLDPWD");
	if (!old_pwd)
	{
		ft_putstr_fd("Minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	if (!get_dir(&old_pwd, actual_dir, env_var, "OLDPWD"))
		return (1);
	if (chdir(old_pwd) == -1)
	{
		print_cd_error(strerror(errno), old_pwd);
		free(old_pwd);
		return (1);
	}
	ft_putnl(old_pwd);
	free(old_pwd);
	if (!get_dir(NULL, actual_dir, env_var, "PWD"))
		return (1);
	return (0);
}

int	built_in_cd(t_exec *exec, t_list **env_var)
{
	char	actual_dir[BUFFER_SIZE];

	if (exec->argc == 1)
		return (cd_home(env_var));
	else if (exec->argc > 2)
	{
		cmd_error_write("cd", "too many arguments");
		return(1);
	}
	else
	{
		if (ft_strcmp(exec->argv[1], "-") == 0)
			return (cd_oldpwd(env_var));
		if (!get_dir(NULL, actual_dir, env_var, "OLDPWD"))
			return (1);
		if (chdir(exec->argv[1]) == -1)
			return (print_cd_error(strerror(errno), exec->argv[1]));
		if (!get_dir(NULL, actual_dir, env_var, "PWD"))
			return (1);
	}
	return (0);
}
