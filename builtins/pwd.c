/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:45:21 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/08 13:22:00 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define BUFFER_SIZE 2048
int built_in_pwd(void)
{
    char    directory[BUFFER_SIZE];

    if (!getcwd(directory, sizeof(directory)))
    {
        file_error("cannot get PWD", strerror(errno));
        return (1);
    }
    ft_putstr_fd(directory, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}