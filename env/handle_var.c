#include "../minishell.h"

static t_bool add_var(t_list **var_list, char *key, char *value)
{
    t_var *var;
    t_list *elem;

    var = ft_calloc(1, sizeof(t_var));
    if (!var)
    {
        load_var_error(ENV_VAR_ERROR, NULL, NULL);
        return (false);
    }
    var->key = ft_strdup(key);
    if (var->key && value)
        var->data = ft_strdup(value);
    if (!var->key || (value && !var->data))
    {
        load_var_error(ENV_VAR_ERROR, var, NULL);
        return (false);
    }
    elem = ft_newlist(var);
    if (!elem)
    {
        load_var_error(ENV_VAR_ERROR, var, NULL);
        return (false);
    }
    ft_lstadd_back(var_list, elem);
    return (true);
}

t_bool edit_var(t_list **var_list, char *key, char *value)
{
    t_var *var;
    t_list *current;

    current = *var_list;
    while (current)
    {
        var = (t_var *)current->content;
        if (!ft_strcmp(var->key, key))
        {
            if (value)
            {
                ft_safe_free(var->data);
                var->data = ft_strdup(value);
                if (!var->data)
                {
                    load_var_error(ENV_VAR_ERROR, NULL, NULL);
                    return (false);
                }
            }
            return (true);
        }
        current = current->next;
    }
    return (add_var(var_list, key, value));
}