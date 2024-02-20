#include "minishell.h"

void exit_on_error(char *s)
{
    printf("%s\n", s);
    exit(0);
}

/* void signal_handler(int sig, struct __siginfo *info, void *additional)
{
    (void)additional;
    (void)info;
    if (sig == SIGINT)
        write(1, "(Signal ctrl-c reçu)", 22);
} */

/* void listen_signals(void)
{
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;

    sa.sa_sigaction = &signal_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaction(SIGINT, &sa, NULL);
} */

int main(int argc, char const *argv[], char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    t_tokens *tokens;
    char *cmd;
    t_cmd *cmds;

    while (1)
    {
        // listen_signals();
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
