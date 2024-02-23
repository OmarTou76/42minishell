/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:44:44 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/23 10:21:22 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_bool is_valid_option(char *str)
{
    if (*str != '-')
        return (false);
    str++;
    while (*str == 'n')
        str++;
    if (*str == '\0')
        return (true);
    else
        return (false);
}

static int number_of_options(int argc, char **argv)
{
    int     i;

    i = 1;
    while (i < argc)
    {
        if (!is_valid_option(argv[i]))
            return (i);
        i++;
    }
    return (i);
}

int built_in_echo(int argc, char **argv)
{
    int i;
    t_bool n_option;

    n_option = false;
    i = number_of_options(argc, argv);
    if (i > 1)
        n_option = true;
    while (i < argc)
    {
        ft_putstr(argv[i]);
        if ((i + 1) < argc)
            ft_putstr(" ");
        i++;
    }
    if (n_option != false)
        ft_putstr("\n");
    return (0);
}