#include "../minishell.h"

t_var *create_var(char *str)
{
    int i;
    t_var *var;

    errno = 0;
    var = ft_calloc(1, sizeof(t_var));
    if (!var)
        return (load_var_error(ENV_VAR_ERROR, NULL, NULL));
    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    var->key = ft_substr(str, 0, i);
    if (!var->key)
        return (load_var_error(ENV_VAR_ERROR, var, NULL));
    str += i + 1;
    var->data = ft_strdup(str);
    if (!var->data)
        return (load_var_error(ENV_VAR_ERROR, var, NULL));
    return (var);
}

char *search_var(t_list *var_list, char *key)
{
    t_var *var;

    while (var_list)
    {
        var = (t_var *)var_list->content;
        if (!ft_strcmp(var->key, key))
            return (var->data);
        var_list = var_list->next;
    }
    return (NULL);
}

/*static t_bool build_var(char **envp, t_list **var_list)
{
    t_var *var;
    t_list *elem;

    *var_list = NULL;
    while (*envp)
    {
        var = create_var(*envp);
        if (var)
            elem = ft_newlist(var);
        if (!var || !elem)
        {
            ft_clearlst(var_list, free_var);
            if (var)
                (load_var_error(ENV_VAR_ERROR, var, NULL));
            return (false);
        }
        ft_lstadd_back(var_list, elem);
        envp++;
    }
    if (!edit_var(var_list, "?", "0"))
    {
        ft_clearlst(var_list, free_var);
        return (false);
    }
    return (true);
}*/