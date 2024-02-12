#include "../minishell.h"

t_cmd *stdout_redirs(t_cmd *cmd, t_tokens **tokens)
{
    t_tokens *token;

    token = *tokens;
    if (!ft_strcmp(token->cmd, ">>"))
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