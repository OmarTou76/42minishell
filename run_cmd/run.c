#include "../minishell.h"

char *get_file_path(char *execname, char **envp)
{
    char file[1024];
    char *filepath;
    int i;
    int found;

    i = 0;
    found = 0;
    while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
        envp++;
    char **paths = ft_split(*envp + 5, ':');
    while (paths[i] && !found)
    {
        ft_strlcpy(file, paths[i], 1024);
        ft_strlcpy(file + ft_strlen(file), "/", 1024);
        ft_strlcpy(file + ft_strlen(file), execname, 1024);
        struct stat sb;
        if (stat(file, &sb) == 0)
            found++;
        i++;
    }
    if (!found)
        return (NULL);
    filepath = ft_strndup(file, ft_strlen(file));
    i = -1;
    while (paths[++i])
        free(paths[i]);
    free(paths);
    return (filepath);
}

void run_exec(t_cmd *c, char **envp)
{
    t_exec *e_cmd;
    char *filepath;

    e_cmd = (t_exec *)c;
    filepath = get_file_path(e_cmd->argv[0], envp);
    if (!filepath)
        printf("minishell: command not found: %s\n", e_cmd->argv[0]);
    else
    {
        free(e_cmd->argv[0]);
        e_cmd->argv[0] = filepath;
        execve(e_cmd->argv[0], e_cmd->argv, envp);
    }
}

void run_pipe(t_cmd *cmd, char **envp)
{
    t_pipe *p_cmd;
    int p[2];

    p_cmd = (t_pipe *)cmd;
    if (pipe(p) < 0)
        exit_on_error("Pipe function");
    if (fork() == 0)
    {
        close(STDOUT_FILENO);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        runcmd(p_cmd->left, envp);
    }
    if (fork() == 0)
    {
        close(STDIN_FILENO);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        runcmd(p_cmd->right, envp);
    }
    close(p[0]);
    close(p[1]);
    wait(0);
    wait(0);
}

void run_redirs(t_cmd *c, char **envp)
{
    t_redirs *cmd;

    cmd = (t_redirs *)c;
    close(cmd->fd);
    if (open(cmd->filename, cmd->mode) < 0)
    {
        printf("%s, ", cmd->filename);
        exit_on_error("No such file");
    };
    runcmd(cmd->cmd, envp);
}

void runcmd(t_cmd *cmd, char **envp)
{
    if (cmd->type == EXEC)
        run_exec(cmd, envp);
    else if (cmd->type == PIPE_CMD)
        run_pipe(cmd, envp);
    else if (cmd->type == REDIR_CMD)
        run_redirs(cmd, envp);
    exit(0);
}