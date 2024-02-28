/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncourtoi <ncourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:06:09 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:07:49 by ncourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_type(t_type type)
{
	switch (type)
	{
		case PIPE_CMD:
			printf("PIPE\n");
		break ;
		case REDIR_CMD:
			printf("REDIR\n");
		break ;
		case EXEC:
			printf("EXEC CMD\n");
		break ;
		default :
			printf("CMD NOT FOUND\n");
		break ;
	}
}
