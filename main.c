/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otourabi <otourabi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:16:25 by ncourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 10:21:07 by otourabi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <string.h>

int		g_status = 0;

void	handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		rl_line_buffer[0] = '\0';
		printf("\n");
		rl_on_new_line();
		if (!g_status)
			rl_redisplay();
	}
	if (sig == SIGQUIT && g_status)
	{
		rl_line_buffer[0] = '\0';
		printf("Quit (core dumped)\n");
		rl_on_new_line();
	}
}

void	init_signal(void)
{
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
}

int	get_cmd(char **cmd)
{
	*cmd = readline("➜  \033[1;32mminishell/>\033[0m  ");
	if (!*cmd)
	{
		free(*cmd);
		write(1, "\n", 1);
		return (0);
	}
	else
		add_history(*cmd);
	return (1);
}

void	parse_and_run_cmds(t_tokens **tokens, t_list **envp_list)
{
	t_cmd	*cmds;

	if (!*tokens)
		return ;
	cmds = NULL;
	cmds = parse_tokens(tokens);
	free_tokens(*tokens);
	g_status = 1;
	signal(SIGQUIT, handle_sig);
	runcmd(cmds, envp_list);
	free_cmds(cmds);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_tokens	*tokens;
	char		*cmd;
	t_list		*envp_list;

	(void)argc;
	(void)argv;
	build_var(envp, &envp_list);
	while (1)
	{
		g_status = 0;
		init_signal();
		if (!get_cmd(&cmd))
			break ;
		if (!ft_strlen(cmd) || get_token_list(cmd, &tokens, &envp_list))
		{
			free(cmd);
			continue ;
		}
		free(cmd);
		update_tokens(&tokens, envp_list);
		parse_and_run_cmds(&tokens, &envp_list);
	}
	ft_clearlst(&envp_list, free_var);
	return (0);
}
