#include "../minishell.h"

int can_exec(char *filepath)
{
    struct stat sb;
    return (!stat(filepath, &sb));
}

char *get_paths(char **envp, char *key)
{
    int i;
    char key_equal[ft_strlen(key) + 2];

    i = 0;
    ft_strlcpy(key_equal, key, ft_strlen(key) + 2);
    ft_strlcpy(key_equal + ft_strlen(key_equal), "=", ft_strlen(key) + 2);
    while (envp[i] && ft_strncmp(envp[i], key_equal, ft_strlen(key_equal)) != 0)
        i++;
    if (!envp[i])
        return (NULL);
    return (envp[i] + ft_strlen(key_equal));
}

int compute_and_check_path(char *execname, char *path, char target[], int bytes)
{
    ft_strlcpy(target, path, bytes);
    ft_strlcpy(target + ft_strlen(target), "/", bytes);
    ft_strlcpy(target + ft_strlen(target), execname, bytes);
    return (can_exec(target));
}

char *get_file_path(char *execname, char **envp)
{
    char filepath[1024];
    char *executable;
    char **paths;
    int i;

    i = 0;
    paths = ft_split(get_paths(envp, "PATH"), ':');
    while (paths[i] && !compute_and_check_path(execname, paths[i], filepath, 1024))
        i++;
    if (!paths[i])
        return (NULL);
    executable = ft_strndup(filepath, ft_strlen(filepath));
    i = -1;
    while (paths[++i])
        free(paths[i]);
    free(paths);
    return (executable);
}

void update_arg(char **arg, char **envp)
{
    char *value;

    value = get_paths(envp, (*arg) + 1);
    if (!value)
        *arg[0] = 0;
    else
    {
        free(*arg);
        *arg = ft_strndup(value, ft_strlen(value));
    }
}

void get_env_var_from_args(t_exec *c, char **envp)
{
    int i;

    i = 0;
    while (c->argv[i])
    {
        if (c->argv[i][0] == '$')
            update_arg(&c->argv[i], envp);
        i++;
    }
}