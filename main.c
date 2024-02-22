#include "minishell.h"
int g_status = 0;

void exit_on_error(char *s)
{
    printf("%s\n", s);
    exit(0);
}
// REGLER POUR LE HEREDOC
void handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        g_status = 130;
        ioctl(STDIN_FILENO, TIOCSTI, "\n");
        // rl_replace_line("", 0);
        /* rl_redisplay();
        rl_on_new_line(); */
    }
}

int main(int argc, char const *argv[], char **envp)
{
    (void)argc;
    (void)argv;
    t_tokens *tokens;
    char *cmd;
    t_cmd *cmds;

    while (1)
    {
        // signal(SIGINT, handle_sigint);
        // signal(SIGQUIT, SIG_IGN);
        cmd = readline("➜  \033[1;32mminishell/>\033[0m  ");
        if (!ft_strlen(cmd))
        {
            free(cmd);
            continue;
        }
        add_history(cmd);
        if (ft_strncmp(cmd, "exit", 4) == 0)
            break;
        if (get_token_list(cmd, &tokens))
            continue;
        update_tokens(&tokens, envp);
        // print_tokens(tokens);
        if (tokens && ft_strncmp(tokens->cmd, "cd", 2) == 0)
        {
            if (chdir(trim_quotes(&tokens->next)) < 0)
                perror("chdir");
        }
        else if (tokens)
        {
            cmds = NULL;
            cmds = parse_tokens(&tokens);
            // print_cmd(cmds);
            runcmd(cmds, envp);
            free_cmds(cmds);
        }
        free(cmd);
        free_tokens(tokens);
    }
    free(cmd);
    return 0;
}
