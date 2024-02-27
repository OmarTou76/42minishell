#include "../minishell.h"

static char *set_env_str(t_var var)
{
    char *env_str;

    env_str = ft_calloc(ft_strlen(var.key) + ft_strlen(var.data) + 2, sizeof(char));
    if (!env_str)
        return (NULL);
    ft_strcat(env_str, var.key);
    ft_strcat(env_str, "=");
    ft_strcat(env_str, var.data);
    return (env_str);
}

char **build_env(t_list *env_var)
{
    char **envp;
    int i;
    char *env_str;
    t_var var;

    envp = ft_calloc(ft_listsize(env_var), sizeof(char *));
    if (!envp)
        return (NULL);
    i = 0;
    while (env_var)
    {
        var = *(t_var *)env_var->content;
        if (!ft_strcmp(var.key, "?") || !ft_strcmp(var.key, "_LAST_EXIT_") || !var.data)
        {
            env_var = env_var->next;
            continue;
        }
        env_str = set_env_str(var);
        envp[i++] = env_str;
        env_var = env_var->next;
    }
    return (envp);
}