#include "../minishell.h"

t_cmd *stdout_redirs(t_cmd *cmd, t_tokens **tokens)
{
    t_tokens *token;

    token = *tokens;
    if (!ft_strncmp(token->cmd, ">>", ft_strlen(token->cmd)))
        cmd = create_redirs(cmd, token->next->cmd, O_WRONLY | O_APPEND | O_CREAT, STDOUT_FILENO);
    else
        cmd = create_redirs(cmd, token->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
    free(token->cmd);
    remove_tokens(tokens, 2);
    return cmd;
}

t_cmd *stdin_redirs(t_cmd *cmd, t_tokens **tokens)
{
    t_tokens *token;
    t_redirs *r;

    token = *tokens;
    cmd = create_redirs(cmd, token->next->cmd, O_RDONLY, STDIN_FILENO);
    if (!ft_strcmp(token->cmd, "<<"))
    {
        r = (t_redirs *)cmd;
        r->is_here_doc++;
    }
    free(token->cmd);
    remove_tokens(tokens, 2);
    return cmd;
}

t_cmd *parse_redirs(t_cmd *cmd, t_tokens **tokens)
{
    t_tokens *token;

    token = (*tokens);
    if (!token->next)
        exit_on_error("Missing file name after redirs");
    if (!ft_strcmp(token->cmd, ">") || !ft_strcmp(token->cmd, ">>"))
        cmd = stdout_redirs(cmd, tokens);
    else if (!ft_strcmp(token->cmd, "<") || !ft_strcmp(token->cmd, "<<"))
        cmd = stdin_redirs(cmd, tokens);
    return cmd;
}

char *trim_quotes(t_tokens **tokens)
{
    if ((*tokens)->type != SINGLE_QUOTE && (*tokens)->type != DOUBLE_QUOTE)
        return (*tokens)->cmd;
    ft_strlcpy((*tokens)->cmd, (*tokens)->cmd + 1, ft_strlen((*tokens)->cmd) - 1);
    return (*tokens)->cmd;
}

int get_arg_count(t_tokens **tokens)
{
    t_tokens *tmp;
    int c;

    tmp = *tokens;
    c = 0;
    while (tmp)
    {
        if (tmp->type == PIPE || tmp->type == REDIR)
            break;
        tmp = tmp->next;
        c++;
    }
    return (c);
}

void get_arg_value(t_cmd *cmd, t_tokens **tokens)
{
    t_exec *exec;
    int i;

    i = 0;
    exec = (t_exec *)cmd;
    exec->argv = malloc(sizeof(char *) * (exec->argc + 1));
    while ((*tokens))
    {
        if ((*tokens)->type == PIPE_CMD)
            break;
        else if ((*tokens)->type == REDIR)
            cmd = parse_redirs(cmd, tokens);
        else
        {
            exec->argv[i] = trim_quotes(tokens);
            i++;
            remove_tokens(tokens, 1);
        }
    }
    exec->argv[i] = NULL;
    exec->is_builtin = cmd_is_builtin(exec->argv[0]);
}

t_cmd *parse_exec(t_tokens **tokens)
{
    t_exec *exec;
    t_cmd *cmd;

    cmd = create_exec();
    exec = (t_exec *)cmd;
    exec->argc = get_arg_count(tokens);
    if (exec->argc)
        get_arg_value(cmd, tokens);
    return cmd;
}

t_cmd *parse_pipe(t_tokens **tokens)
{
    t_cmd *cmd;
    t_tokens *tmp;

    cmd = parse_exec(tokens);
    if ((*tokens) && (*tokens)->cmd[0] == '|')
    {
        tmp = (*tokens);
        (*tokens) = (*tokens)->next;
        free(tmp->cmd);
        free(tmp);
        cmd = create_pipe(cmd, parse_pipe(tokens));
    }
    return (cmd);
}

t_cmd *parse_tokens(t_tokens **tokens)
{
    t_cmd *cmd;
    cmd = parse_pipe(tokens);
    return cmd;
}
