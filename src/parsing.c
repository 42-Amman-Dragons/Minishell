/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 10:56:26 by haya              #+#    #+#             */
/*   Updated: 2026/02/10 15:05:40 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *raise_parse_error(t_list *temp, int *parse_error)
{
    ft_putstr_fd("bash: syntax error near unexpected token ` ", 2);
    write(2, ((t_token *)temp->content)->content, ft_strlen(((t_token *)temp->content)->content));
    write(2, "'\n", 2* sizeof(char));
    *parse_error = 1;
    return(NULL);
}

t_tree *create_empty_node()
{
    t_tree *node;
    node = ft_calloc(1, sizeof(t_tree));
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

t_tree *parse_redir(t_list *tokens, char *type, int *parse_error)
{
    t_list *temp;
    t_list *prev;
    t_tree *node;

    prev = NULL;
    node = NULL;
    temp = find_type(tokens,type, &prev);

    temp = find_type(tokens,type,  &prev);
    if(temp){
        node = create_empty_node();
        node->type = type;
        node->right = NULL;
        // @TODO: if the next is non word then parse error. "DONE"
        if(!temp->next)
            raise_parse_error(temp, parse_error);
        if(ft_strncmp(((t_token *)temp->next->content)->type,"WORD", 4) != 0)
            raise_parse_error(temp, parse_error);
        node->filename = ((t_token *)temp->next->content)->content;
        ((t_token *)temp->content)->type = "finished";
        ((t_token *)temp->next->content)->type = "finished";
        node -> left = parse_tree(tokens, parse_error);
        return (node);
    }
    return (NULL);
}
t_tree * parse_operator(t_list *tokens, char *type, int *parse_error)
{
    t_list *temp;
    t_list *prev;
    t_tree *node;

    prev = NULL;
    node = NULL;
    temp = find_type(tokens,type, &prev);
    // If no privious if should raise an error in some cases. "DONE"
    if(temp)
    {
        node = create_empty_node();
        node->type = type;
        node->right = parse_tree(temp->next, parse_error);
        if(prev)
            prev->next = NULL;
        else
            raise_parse_error(temp, parse_error);
        node->left = parse_tree(tokens, parse_error);
        return (node);
    }
    return( NULL);
}
t_tree *parse_tree(t_list *tokens, int *parse_error)
{
    t_tree *node;

    if(!tokens)
        return (NULL);
    node = parse_operator(tokens, "AND", parse_error);
    if(node)
        return(node);
    node = parse_operator(tokens, "OR", parse_error);
    if(node)
        return(node);
    node = parse_operator(tokens, "PIPE", parse_error);
    if(node)
        return(node);
    node = parse_redir(tokens, "DIR_IN", parse_error);
    if(node)
        return(node);
    node = parse_redir(tokens, "DIR_OUT", parse_error);
    if(node)
        return(node);
    return(create_comand_node(&tokens));   
}

void free_tree(t_tree *tree)
{
    if(tree->left)
        free_tree(tree->left);
    if(tree->right)
        free_tree(tree->right);
    if(tree->argumnts)
        free(tree->argumnts);
    free(tree);
}

t_tree *parse(t_list *tokens)
{
    t_tree *node;
    int parse_error;
    
    if(!tokens)
        return (NULL);
    parse_error = 0;
    node = parse_tree(tokens, &parse_error);
    if(parse_error == 1)
    {
        free_tree(node);
        return (NULL);
    }
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
    return (node);
}
