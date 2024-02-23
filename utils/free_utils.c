#include "../minishell.h"

void    ft_clearlst(t_list **lst, void (*del)(void *))
{
    t_list *temp;

    while (*lst)
    {
        temp = (*lst)->next;
        del((*lst)->content);
        free(*lst);
        *lst = temp;
    }
    *lst = NULL;
}

void    free_var(void *mem)
{
    t_var *var;

    var = (t_var *)mem;
    ft_safe_free(var->key);
    ft_safe_free(var->data);
    ft_safe_free(var);
}

void    free_table(char ***table)
{
    int i;

    i = 0;
    if (!*table)
        return ;
    while ((*table)[i])
        free((*table)[i++]);
    free(*table);
}

void ft_safe_free(void *mem)
{
    if (mem)
        free(mem);
}

static void free_elem(char **table, int max)
{
    int     i;
    i = 0;
    while (i <= max)
        ft_safe_free(table[i++]);
    free(table);
}

void *return_and_free(char **envp, int i)
{
    free_elem(envp, i);
    return (NULL);
}