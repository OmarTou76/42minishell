#include "minishell.h"

int		g_status = 0;

void	exit_on_error(char *s)
{
	printf("%s\n", s);
	exit(0);
}

void	handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		if (!g_status)
			rl_redisplay();
	}
}

int	main(int argc, char const *argv[], char **envp)
{
	t_tokens	*tokens;
	char		*cmd;
	t_cmd		*cmds;
	t_list		*envp_list;

	(void)argc;
	(void)argv;
	build_var(envp, &envp_list);
	while (1)
	{
		g_status = 0;
		signal(SIGINT, handle_sig);
		signal(SIGQUIT, SIG_IGN);
		cmd = readline("➜  \033[1;32mminishell/>\033[0m  ");
		if (!ft_strlen(cmd))
		{
			free(cmd);
			if (!cmd)
			{
				write(1, "\n", 1);
				break ;
			}
			continue ;
		}
		add_history(cmd);
		if (get_token_list(cmd, &tokens))
			continue ;
		free(cmd);
		update_tokens(&tokens, envp_list);
		if (tokens)
		{
			cmds = NULL;
			cmds = parse_tokens(&tokens);
			free_tokens(tokens);
			g_status = 1;
			runcmd(cmds, &envp_list);
			free_cmds(cmds);
		}
	}
	ft_clearlst(&envp_list, free_var);
	return (0);
}
