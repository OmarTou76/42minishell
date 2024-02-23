#include "../minishell.h"

int add_var(t_list **var_list, char *key, char *value)
{
    t_var *var;
    t_list *elem;

    var = ft_calloc(1, sizeof(t_var));
    if (!var)
    {
        load_var_error(ENV_VAR_ERROR, NULL, NULL);
        return (0);
    }
    var->key = ft_strdup(key);
    if (var->key && value)
        var->data = ft_strdup(value);
    if (!var->key || (value && !var->data))
    {
        load_var_error(ENV_VAR_ERROR, var, NULL);
        return (0);
    }
    elem = ft_newlist(var);
    if (!elem)
    {
        load_var_error(ENV_VAR_ERROR, var, NULL);
        return (0);
    }
    ft_lstadd_back(var_list, elem);
    return (1);
}

int edit_var(t_list **var_list, char *key, char *value)
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
                    return (0);
                }
            }
            return (1);
        }
        current = current->next;
    }
    return (add_var(var_list, key, value));
}

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

int build_var(char **envp, t_list **var_list)
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
            return (0);
        }
        ft_lstadd_back(var_list, elem);
        envp++;
    }
    if (!edit_var(var_list, "?", "0"))
    {
        ft_clearlst(var_list, free_var);
        return (0);
    }
    return (1);
}