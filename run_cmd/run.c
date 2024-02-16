#include "../minishell.h"

void exec_cmd_by_type(t_cmd *cmd, char **envp);

void run_exec(t_cmd *c, char **envp)
{
    t_exec *e_cmd;
    char *filepath;

    e_cmd = (t_exec *)c;
    get_env_var_from_args(e_cmd, envp);
    if (!e_cmd->is_builtin)
    {
        if (!can_exec(e_cmd->argv[0]))
        {
            filepath = get_file_path(e_cmd->argv[0], envp);
            if (!filepath)
                printf("minishell: command not found: %s\n", e_cmd->argv[0]);
            free(e_cmd->argv[0]);
            e_cmd->argv[0] = filepath;
        }
        execve(e_cmd->argv[0], e_cmd->argv, envp);
    }
    else
        run_builtin(e_cmd, envp);
    exit(0);
}

void run_left_cmd(t_pipe *p_cmd, int p[2], char **envp)
{
    close(STDOUT_FILENO);
    dup(p[1]);
    close(p[0]);
    close(p[1]);
    exec_cmd_by_type(p_cmd->left, envp);
}

void run_right_cmd(t_pipe *p_cmd, int p[2], char **envp)
{
    close(STDIN_FILENO);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    exec_cmd_by_type(p_cmd->right, envp);
}

void run_pipe(t_cmd *cmd, char **envp)
{
    t_pipe *p_cmd;
    int p[2];

    p_cmd = (t_pipe *)cmd;
    if (pipe(p) < 0)
        exit_on_error("Pipe function");
    if (fork() == 0)
        run_left_cmd(p_cmd, p, envp);
    if (fork() == 0)
        run_right_cmd(p_cmd, p, envp);
    close(p[0]);
    close(p[1]);
    wait(NULL);
    wait(NULL);
    exit(0);
}

void save_heredoc(t_redirs *cmd, char **envp)
{
    int fd;
    char *line;
    (void)envp;

    fd = open("__tmp_file__", O_WRONLY | O_APPEND | O_CREAT, 0777);
    while (1)
    {
        line = readline("heredoc> ");
        if (ft_strcmp(line, cmd->filename) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    free(cmd->filename);
    cmd->filename = ft_strndup("__tmp_file__", ft_strlen("__tmp_file__"));
    close(fd);
}

void run_redirs(t_cmd *c, char **envp)
{
    t_redirs *cmd;

    cmd = (t_redirs *)c;
    if (cmd->is_here_doc)
        save_heredoc(cmd, envp);
    close(cmd->fd);
    if (open(cmd->filename, cmd->mode, 0777) < 0)
    {
        printf("%s, ", cmd->filename);
        exit_on_error("No such file");
    };
    if (cmd->is_here_doc)
        unlink(cmd->filename);
    exec_cmd_by_type(cmd->cmd, envp);
}

void exec_cmd_by_type(t_cmd *cmd, char **envp)
{
    if (cmd->type == EXEC)
        run_exec(cmd, envp);
    else if (cmd->type == PIPE_CMD)
        run_pipe(cmd, envp);
    else if (cmd->type == REDIR_CMD)
        run_redirs(cmd, envp);
}

void runcmd(t_cmd *cmd, char **envp)
{
    if (fork() == 0)
        exec_cmd_by_type(cmd, envp);
    wait(0);
}