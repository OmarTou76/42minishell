#include "../minishell.h"

char *trim_quotes(t_tokens **tokens)
{
    if ((*tokens)->type != SINGLE_QUOTE && (*tokens)->type != DOUBLE_QUOTE)
        return (*tokens)->cmd;
    ft_strlcpy((*tokens)->cmd, (*tokens)->cmd + 1, ft_strlen((*tokens)->cmd) - 1);
    return (*tokens)->cmd;
}

int get_quoted_len(char *str, char quote)
{
    int len;

    len = 1;
    while (str[len] && str[len] != quote)
        len++;
    if (str[len] != quote)
        return (-1);
    len++;
    return (len);
}

int get_token_list(char *usr_cmd, t_tokens **tokens)
{
    int i;

    i = 0;
    *tokens = NULL;
    while (usr_cmd && usr_cmd[i])
    {
        if (handle_pipe(tokens, usr_cmd, &i))
            break;
        else if (handle_bracket(tokens, usr_cmd, &i))
            break;
        else if (handle_redir(tokens, usr_cmd, &i))
            break;
        else if (handle_text(tokens, usr_cmd, &i))
            break;
        else if (handle_quotes(tokens, usr_cmd, &i))
            break;
        while (usr_cmd[i] && is_charset(usr_cmd[i], SPACES))
            i++;
    }
    if (usr_cmd[i])
    {
        printf("Error on parsing at %c (%d)\n", usr_cmd[i], i);
        free_tokens(*tokens);
        return (-1);
    }
    return (0);
}