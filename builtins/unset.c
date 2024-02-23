#include "../minishell.h"

static int cmp_var(t_var *var, t_var *rm)
{
    return (ft_strcmp(var->key, rm->key));
}

int built_in_unset(int argc, char **argv, t_list **env_var)
{
    int i;
    t_var var;

    i = 0;
    while (i < argc)
    {
        var.key = argv[i++];
        ft_lstremoveif(env_var, &var, cmp_var, free_var);
    }
    return (0);
}