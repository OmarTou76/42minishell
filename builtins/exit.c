/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:45:10 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/08 12:39:35 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define IN_PIPES    0
#define OUT_PIPES   2

static t_bool is_str_num(char *str)
{
    if (*str == '+' || *str == '-')
        if (!*++str)
            return (false);
    while (*str)
    {
        if (!ft_isnum(*str))
            return (false);
        str++;
    }
    return (true);
}

static int exit_not_num(char *av_1, int offset)
{
    ft_putstr_fd("Minishell: exit: ", 2);
    ft_putstr_fd(av_1, 2);
    ft_putstr_fd(": numeric argument required\n", 2);
    return (EXIT_ERROR + offset);
}

static int exit_value(int ac, char **av, t_list **env_var, int offset)
{
    int     ret;
    if (ac == 2)
        ret = ft_atoi(av[1]);
    else
        ret = ft_atoi(search_var(*env_var, "?"));
    return ((unsigned char)(ret + offset));
}

int built_in_exit(int ac, char **av, t_list **env_var, t_bool in_pipes)
{
    int     offset;

    if (in_pipes)
        offset = IN_PIPES;
    else
        offset = OUT_PIPES;
    if (isatty(STDIN_FILENO))
        ft_putstr("exit\n");
    if (ac >= 2 && !is_str_num(av[1]))
        return (exit_not_num(av[1], offset));
    if (ac > 2)
    {
        ft_putstr_fd("Minishell: exit: too many arguments\n", 2);
        edit_var(env_var, "?", "1");
        return (1);
    }
    else
        return (exit_value(ac, av, env_var, offset));
}