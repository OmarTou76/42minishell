#include "../minishell.h"

t_list *ft_newlist(void *content)
{
    t_list *lst;

    lst = (t_list *)malloc(sizeof(t_list));
    if (!lst)
        return (NULL);
    lst->next = NULL;
    lst->content = content;
    return (lst);
}
int ft_lstadd_back(t_list **lst, t_list *new)
{
    t_list *temp;

    if (!new)
        return (0);
    if (!lst)
    {
        *lst = new;
        return (1);
    }
    temp = *lst;
    while (temp->next)
        temp = temp->next;
    temp->next = new;
    return (1);
}

int ft_listsize(t_list *lst)
{
    int i;

    i = 0;
    while (lst)
    {
        lst = lst->next;
        i++;
    }
    return (i);
}

void ft_lstremoveif(t_list **alst, void *data_rm, int (*cmp)(), void (*free_fct)(void *))
{
    t_list *current;
    t_list *tmp_prev;

    current = *alst;
    tmp_prev = NULL;
    while (current != NULL)
    {
        if ((*cmp)(current->content, data_rm) == 0)
        {
            if (free_fct != NULL)
                (*free_fct)(current->content);
            if (tmp_prev == NULL)
                *alst = current->next;
            else
                tmp_prev->next = current->next;
            free(current);
            return ;
        }
        tmp_prev = current;
        current = current->next;
    }
}