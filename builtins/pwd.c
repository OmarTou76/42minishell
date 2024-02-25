/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:45:21 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/25 00:29:15 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define BUFFER_SIZE 2048

int built_in_pwd(t_exec *e, t_list **envp)
{
    char directory[BUFFER_SIZE];
    (void)e;
    (void)envp;

    if (!getcwd(directory, sizeof(directory)))
    {
        file_error("cannot get PWD", strerror(errno));
        return (1);
    }
    ft_putstr_fd(directory, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}