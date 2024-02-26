#include "../minishell.h"

static void join_var(t_list **env_var, char *tmp_str, char *key)
{
    char *dest;
    char *var;
    int len_var;
    
    len_var = 0;
    var = search_var(*env_var, key);
    if (var)
        len_var = ft_strlen(var);
    dest = ft_calloc(len_var + ft_strlen(tmp_str) + 1, sizeof(char));
    if (!dest)
    {
        ft_putstr_fd(strerror(errno), 2);
        return ;
    }
    if (var)
        ft_strcat(dest, var);
    ft_strcat(dest, tmp_str);
    edit_var(env_var, key, dest);
    free(dest);
}

static void add_var(char *key, char *tmp_str, t_list **env_var, int append)
{
    if (!*tmp_str && *(tmp_str - 1) != '=')
        edit_var(env_var, key, NULL);
    else
    {
        if (append)
            join_var(env_var, tmp_str, key);
        else
            edit_var(env_var, key, tmp_str);
    }
}

int export_rules(char *str, t_list **env_var)
{
    char    *tmp_str;
    int     error;
    char    *key;
    int     append;

    error = 0;
    tmp_str = str;
    key = parse_key(&tmp_str, &error, &append);
    if (!key && !error)
    {
        cmd_error_write("export", strerror(errno));
        return (1);
    }
    if (error)
    {
        ft_putstr_fd("Minishell: export: '", 2);
        ft_putstr_fd(str, 2);
        ft_putstr_fd("' not a valid identifier\n", 2);
    }
    else
        add_var(key, tmp_str, env_var, append);
    ft_safe_free(key);
    return (error);
}

int built_in_export(t_exec *exec, t_list **env_var)
{
    int i;
    int error;

    if (exec->argc == 1)
        return (print_export(*env_var));
    i = 1;
    error = 0;
    while (i < exec->argc)
    {
        if (export_rules(exec->argv[i++], env_var))
            error = 1;
    }
    return (error);
}