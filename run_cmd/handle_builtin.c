#include "../minishell.h"

int cmd_is_builtin(char *cmd)
{
    if (ft_strncmp(cmd, "cd", ft_strlen(cmd)) == 0)
        return (1);
    else if (ft_strncmp(cmd, "echo", ft_strlen(cmd)) == 0)
        return (1);
    else if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
        return (1);
    else if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
        return (1);
    else if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
        return (1);
    else if (ft_strncmp(cmd, "env", ft_strlen(cmd)) == 0)
        return (1);
    else if (ft_strncmp(cmd, "exit", ft_strlen(cmd)) == 0)
        return (1);
    return (0);
}

typedef void (*builtin_func)(t_exec *, char **envp);

// Pour les tests
void ft_echo(t_exec *cmd, char **envp)
{
    int i;

    i = 1;
    (void)envp;
    while (cmd->argv[i])
    {
        printf("%s", cmd->argv[i]);
        i++;
        if (cmd->argv[i])
            printf(" ");
        else
            printf("\n");
    }
}
// Pour les tests
void ft_pwd(t_exec *cmd, char **envp)
{
    int i;

    i = 0;
    (void)cmd;
    while (ft_strncmp(envp[i], "PWD=", 4))
        i++;
    if (envp[i])
        printf("%s\n", envp[i] + 4);
}

builtin_func get_builtin_func(char *cmd)
{
    if (ft_strncmp(cmd, "echo", ft_strlen(cmd)) == 0)
        return &ft_echo;
    else if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
        return &ft_pwd;
    return NULL;
}

void run_builtin(t_exec *cmd, char **envp)
{
    builtin_func exec_builtin = get_builtin_func(cmd->argv[0]);
    if (exec_builtin != NULL)
    {
        exec_builtin(cmd, envp);
        exit(0);
    }
    else
        printf("%s doesn't exist yet\n", cmd->argv[0]);
}