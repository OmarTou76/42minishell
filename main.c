#include "minishell.h"

void free_tokens(t_tokens *tokens)
{
    t_tokens *tmp;
    t_tokens *token;

    token = tokens;
    while (token)
    {
        tmp = token;
        free(token->cmd);
        token = token->next;
        free(tmp);
    }
    tokens = NULL;
}

void exit_on_error(char *s)
{
    printf("%s\n", s);
    exit(1);
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
        cmd = readline("➜  \033[1;32mminishell/>\033[0m  ");
        if (!ft_strlen(cmd))
        {
            free(cmd);
            continue;
        }
        add_history(cmd);
        if (ft_strncmp(cmd, "exit", 4) == 0)
            break;
        get_token_list(cmd, &tokens);
        if (ft_strncmp(tokens->cmd, "cd", 2) == 0)
        {
            if (chdir(trim_quotes(&tokens->next)) < 0)
                perror("chdir");
        }
        else
        {
            cmds = NULL;
            cmds = parse_tokens(&tokens, envp);
            //print_cmd()
            if (fork() == 0)
                runcmd(cmds, envp);
            wait(0);
            free_cmds(cmds);
        }
        free(cmd);
        free_tokens(tokens);
    }
    free(cmd);
    return 0;
}
