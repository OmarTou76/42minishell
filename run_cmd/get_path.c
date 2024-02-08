#include "../minishell.h"

int can_exec(char *filepath)
{
    struct stat sb;
    return (!stat(filepath, &sb));
}

char *get_paths(char **envp)
{
    int i;

    i = 0;
    while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    if (!envp[i])
        perror("Impossible to find path in envp");
    return (envp[i] + 5);
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
    paths = ft_split(get_paths(envp), ':');
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