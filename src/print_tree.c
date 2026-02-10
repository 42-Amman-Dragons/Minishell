/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:22:04 by haya              #+#    #+#             */
/*   Updated: 2026/02/10 14:36:56 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_lst_remove_front(t_list **lst)
{
    if(!lst)
        return;
    if(!*lst)
        return;
    t_list *temp;

    temp = *lst;
    *lst = (*lst)->next;
    temp->next = NULL;
    free(temp);
    temp = NULL;
}

void print_tree(t_tree *head)
{
    t_list *queue;
    t_list *node;
    int i;
    
    queue = NULL;
    i = 0;
    if(!head)
    {
        printf("no list\n");
        return ;
    }
    node = ft_lstnew(head);
    ft_lstadd_back(&queue, node);
    while(queue)
    {
        printf("%i, %s", i, ((t_tree *)queue->content)->type);
        if(((t_tree *)queue->content)->argumnts)
            print_arr(((t_tree *)queue->content)->argumnts);
        printf("\n");
        if(((t_tree *)queue->content)->left)
        {
            node = ft_lstnew(((t_tree *)queue->content)->left);
            ft_lstadd_back(&queue, node);
        }
        if(((t_tree *)queue->content)->right)
        {
            node = ft_lstnew(((t_tree *)queue->content)->right);
            ft_lstadd_back(&queue, node);
        }
        ft_lst_remove_front(&queue);
        i++;
    }
}


void ft_lst_print(t_list *lst)
{
    if(!lst)
        return;
    while (lst)
    {
        printf("%s, %s->",((t_token *)lst->content)->type,((t_token *)lst->content)->content);
        lst = lst->next;
    }
    printf("NULL\n");
}

void print_arr(char **arr)
{
    int i;
    if(!arr)
        return;
    i = 0;
    printf("[");
    while(arr[i])
    {
        printf("%s,", arr[i]);
        i++;
    }
    printf("]");
}