#include "../minishell.h"

static int check_parentheses(t_tokens *tokens)
{
    int count;

    count = 0;
    while (tokens)
    {
        if (tokens->type == OPEN_BRACKET)
        {
            if (tokens->next && tokens->next->type == CLOSE_BRACKET)
                return (-1);
            count++;
        }
        else if (tokens->type == CLOSE_BRACKET)
            count--;
        if (count < 0)
            return (1);
        tokens = tokens->next;
    }
    if (count > 0)
        return (1);
    return (0);
}

int tokens_have_conflicts(t_tokens **tokens)
{
    if (check_parentheses(*tokens))
        return (-1);
    return (0);
}