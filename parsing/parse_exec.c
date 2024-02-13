#include "../minishell.h"

int get_arg_count(t_tokens **tokens)
{
    t_tokens *tmp;
    int c;

    tmp = *tokens;
    c = 0;
    while (tmp)
    {
        if (tmp->type == PIPE || tmp->type == REDIR || tmp->type == CLOSE_BRACKET)
            break;
        tmp = tmp->next;
        c++;
    }
    return (c);
}

void get_arg_value(t_cmd **cmd, t_tokens **tokens)
{
    t_exec *exec;
    int i;

    i = 0;
    exec = (t_exec *)*cmd;
    exec->argv = malloc(sizeof(char *) * (exec->argc + 1));
    while ((*tokens))
    {
        if ((*tokens)->type == PIPE || (*tokens)->type == CLOSE_BRACKET)
            break;
        else if ((*tokens)->type == REDIR)
            *cmd = parse_redirs(*cmd, tokens);
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

    if ((*tokens)->type == OPEN_BRACKET)
        return (parse_parentheses(tokens));
    cmd = create_exec();
    exec = (t_exec *)cmd;
    exec->argc = get_arg_count(tokens);
    if (exec->argc)
        get_arg_value(&cmd, tokens);
    return cmd;
}