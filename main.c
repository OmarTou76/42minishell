#include "minishell.h"

int		g_status = 0;

void	exit_on_error(char *s)
{
	printf("%s\n", s);
	exit(0);
}

void	handle_sigint(int sig)
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
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		cmd = readline("➜  \033[1;32mminishell/>\033[0m  ");
		if (!ft_strlen(cmd))
		{
			free(cmd);
			continue ;
		}
		add_history(cmd);
		if (ft_strncmp(cmd, "exit", 4) == 0)
			break ;
		if (get_token_list(cmd, &tokens))
			continue ;
		update_tokens(&tokens, envp_list);
		
		if (tokens)
		{
			cmds = NULL;
			cmds = parse_tokens(&tokens);
			// print_cmd(cmds);
			g_status = 1;
			runcmd(cmds, &envp_list);
			free_cmds(cmds);
		}
		free(cmd);
		free_tokens(tokens);
	}
	free(cmd);
	return (0);
}
