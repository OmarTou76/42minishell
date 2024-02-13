#include "../minishell.h"

t_cmd *parse_pipe(t_tokens **tokens)
{
    t_cmd *cmd;
    t_tokens *tmp;

    cmd = parse_exec(tokens);
    if ((*tokens) && (*tokens)->type == PIPE)
    {
        tmp = (*tokens);
        (*tokens) = (*tokens)->next;
        free(tmp->cmd);
        free(tmp);
        cmd = create_pipe(cmd, parse_pipe(tokens));
    }
    return (cmd);
}

t_cmd *parse_parentheses(t_tokens **tokens)
{
    t_cmd *cmd;

    remove_tokens(tokens, 1);
    cmd = parse_tokens(tokens);
    if ((*tokens)->type != CLOSE_BRACKET)
        perror("Invalid syntax (close bracket)");
    remove_tokens(tokens, 1);
    cmd = parse_redirs(cmd, tokens);
    return cmd;
}

t_cmd *parse_tokens(t_tokens **tokens)
{
    t_cmd *cmd;
    cmd = parse_pipe(tokens);
    return cmd;
}
