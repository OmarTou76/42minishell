#include "../minishell.h"

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

void get_token_list(char *usr_cmd, t_tokens **tokens)
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
        printf("Error on parsing at %c\n", usr_cmd[i]);
}