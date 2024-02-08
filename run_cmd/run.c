#include "../minishell.h"

void exec_cmd_by_type(t_cmd *cmd, char **envp);

void run_exec(t_cmd *c, char **envp)
{
    t_exec *e_cmd;
    char *filepath;

    e_cmd = (t_exec *)c;

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
        exec_cmd_by_type(p_cmd->left, envp);
    }
    if (fork() == 0)
    {
        close(STDIN_FILENO);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        exec_cmd_by_type(p_cmd->right, envp);
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