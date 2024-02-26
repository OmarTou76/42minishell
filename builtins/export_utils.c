#include "../minishell.h"

static int key_len(char *str, int *error)
{
    int i;

    i = 0;
    while (str[i] && (str[i] == '_' || ft_isalnum(str[i])))
        i++;
    if (!i || (str[i] != '=' && ft_memcmp(str + i, "+=", 2) && str[i] != 0))
    {
        *error = 1;
        return (-1);
    }
    return (i);
}

char *parse_key(char **str, int *error, int *append)
{
    int i;
    char *dest;

    if (ft_isnum(**str))
    {
        *error = 1;
        return (NULL);
    }
    i = key_len(*str, error);
    if (i < 0)
        return (NULL);
    if (!ft_memcmp((*str) + i, "+=", 2))
        *append = 1;
    else
        *append = 0;
    dest = ft_substr(*str, 0, i);
    if (!dest)
        return (NULL);
    if ((*str)[i])
        (*str) += 1;
    (*str) += i + *append;
    return (dest);
}

static void sort_vars(t_var **var_tbl)
{
    int i;
    int j;
    t_var *temp;

    i = 0;
    while (var_tbl[i])
    {
        j = i + 1;
        while (var_tbl[j])
        {
            if (ft_strcmp(var_tbl[i]->key, var_tbl[j]->key) > 0)
            {
                temp = var_tbl[i];
                var_tbl[i] = var_tbl[j];
                var_tbl[j] = temp;
            }
            j++;
        }
        i++;
    }
}

static void print_sorted(t_var **var_tbl)
{
    while (*var_tbl)
    {
        ft_putstr("declare -x ");
        ft_putstr((*var_tbl)->key);
        if ((*var_tbl)->data)
        {
            ft_putstr("=\"");
            ft_putstr((*var_tbl)->data);
            ft_putstr("\"");
        }
        ft_putstr("\n");
        var_tbl++;
    }
}

int print_export(t_list *env_var)
{
    t_var **var_tbl;
    t_var *var;
    int i;

    var_tbl = ft_calloc(ft_listsize(env_var), sizeof(t_var *));
    if (!var_tbl)
    {
        ft_putstr_fd(strerror(errno), 2);
        return (127);
    }
    i = 0;
    while (env_var)
    {
        var = (t_var *)env_var->content;
        env_var = env_var->next;
        if (!ft_strcmp(var->key, "?") || !ft_strcmp(var->key, "_"))
            continue;
        var_tbl[i++] = var;
    }
    sort_vars(var_tbl);
    print_sorted(var_tbl);
    ft_safe_free(var_tbl);
    return (0);
}