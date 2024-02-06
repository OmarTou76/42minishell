#include "../minishell.h"

t_tokens *create_token(char *token, t_tok_type TYPE)
{
    t_tokens *new;

    new = malloc(sizeof(t_tokens));
    new->cmd = token;
    new->type = TYPE;
    new->next = NULL;
    return (new);
}

void append_token(t_tokens **list, t_tokens *new)
{
    t_tokens *tmp;

    tmp = (*list);
    if (!*list)
        *list = new;
    else
    {
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void remove_tokens(t_tokens **tokens, size_t nb_to_remove)
{
    t_tokens *to_remove;
    size_t i;

    i = 0;
    while ((*tokens) && i < nb_to_remove)
    {
        to_remove = *tokens;
        *tokens = (*tokens)->next;
        free(to_remove);
        i++;
    }
}