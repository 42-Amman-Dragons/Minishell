/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 10:56:26 by haya              #+#    #+#             */
/*   Updated: 2026/02/10 12:19:33 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **generate_args(t_list *tokens, int len)
{
    int i;
    char **args;

    args = ft_calloc(len + 1, sizeof(char *));
    i = 0;
    while(ft_strncmp(((t_token *)tokens->content)->type, "finished", 8) == 0)
        tokens = tokens->next;
    while(i < len)
    {
        args[i] = ((t_token *)tokens->content)->content;
        tokens = tokens->next;
        i++;
    }
    return (args);
}

t_tree *create_empty_node()
{
    t_tree *node;
    node = ft_calloc(1, sizeof(t_tree));
    return (node);
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

t_tree *create_comand_node(t_list **tokens)
{
    t_tree *node;
    t_list *current;
    char **args;
    int len;
    
    current = *tokens;
    len = 0;
    ft_lst_print(*tokens);
    node = ft_calloc(1, sizeof(t_tree));
    while(ft_strncmp(((t_token *)current->content)->type, "finished", 8) == 0)
        current = current->next;
    while (current && ft_strncmp(((t_token *)current->content)->type, "WORD", 4) == 0)
    {
        current = current->next;
        len++;   
    }
    if (len == 0)
        return NULL;
    args = generate_args(*tokens, len);
    node->type = "CMD";
    node->argumnts = args;
    // to move the cursor after the last word.
    *tokens = current;
    return (node);
}



t_list *find_type(t_list *tokens, char *type, t_list **prev)
{
    t_list *current;

    current = tokens;
    while(current)
    {
        if(ft_strncmp(((t_token *)current->content)->type, type, ft_strlen(type)) == 0)
        {
            return (current);
        }
        *prev = current; 
        current = current->next;
    }
    return (NULL);
}

void ft_lst_remove_front(t_list **lst)
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
        return ;
    node = ft_lstnew(head);
    ft_lstadd_back(&queue, node);
    while(queue)
    {
        printf("%i, %s", i, ((t_tree *)queue->content)->type);
        if(((t_tree *)queue->content)->argumnts)
            printf("%s", ((t_tree *)queue->content)->argumnts[0]);
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

t_tree *parse_tree(t_tree *node,t_list *tokens)
{
    t_list *temp;
    t_list *prev;
    
    prev = NULL;

    if(!tokens)
        return (NULL);
    temp = find_type(tokens,"AND", &prev);
    // If no privious if should raise an error in some cases.
    if(temp)
    {
        node->type = "AND";
        node->right = parse_tree(create_empty_node(),temp->next);
        if(prev)
            prev->next = NULL;
        node->left = parse_tree(create_empty_node(),tokens);
        return (node);
    }
    temp = find_type(tokens,"OR", &prev);
    if(temp)
    {
        node->type = "OR";
        node->right = parse_tree(create_empty_node(),temp->next);
        if(prev)
            prev->next = NULL;
        node->left = parse_tree(create_empty_node(),tokens);
        return (node);
    }
    temp = find_type(tokens,"PIPE", &prev);
    if(temp)
    {
        node->type = "PIPE";
        node->right = parse_tree(create_empty_node(),temp->next);
        if(prev)
            prev->next = NULL;
        node->left = parse_tree(create_empty_node(), tokens);
        return (node);
    }
    temp = find_type(tokens,"DIR_IN",  &prev);
    if(temp){
        node->type = "DIR_IN";
        node->right = NULL;
        // @TODO: if the next is non word then parse error.
        node->filename = ((t_token *)temp->next->content)->content;
        ((t_token *)temp->content)->type = "finished";
        ((t_token *)temp->next->content)->type = "finished";
        node -> left = parse_tree(create_empty_node(), tokens);
        return (node);
    }
    temp = find_type(tokens,"DIR_OUT", &prev);
    if(temp){
        node->type = "DIR_OUT";
        node->right = NULL;
        // @TODO: if the next is non word then parse error.
        node->filename = ((t_token *)temp->next->content)->content;
        ((t_token *)temp->content)->type = "finished";
        ((t_token *)temp->next->content)->type = "finished";
        node -> left = parse_tree(create_empty_node(), tokens);
        return (node);
    }
    return(create_comand_node(&tokens));   
}

void parse(t_list *tokens)
{
    t_tree *node;
    if(!tokens)
        return ;

    node = parse_tree(create_empty_node(), tokens);
    // _____The order rule_____
    // Look for && or ||
    
    // Then |

    // Then redirections

    // Then plain command
    // _________________________
    // if(((t_token *)current->content)->type == "WORD")
    // {
    //     create_comand_node(&current);
    // }
    // current = current->next;
    print_tree(node);
}