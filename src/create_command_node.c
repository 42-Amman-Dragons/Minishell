/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:23:50 by haya              #+#    #+#             */
/*   Updated: 2026/02/10 14:15:39 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// @ @TODO: if more than one word is splitted by finished like ls <infile ls
// it raises this error ls: cannot access 'ls': No such file or directory.
// the same for ls ls 
// the plan is to put all the words inside the arguuments as a one cmand
// example : ls <file -l
// output : total 36
//  -rw-r--r-- 1 haya haya 1586 Feb  8 12:44 Makefile
// what I am currently doing is skippeng the leading skipped.

// t_tree *create_comand_node(t_list **tokens)
// {
//     t_tree *node;
//     t_list *current;
//     char **args;
//     int len;
    
//     current = *tokens;
//     len = 0;
//     node = ft_calloc(1, sizeof(t_tree));
//     // skip all the finished tokens.
//     while(ft_strncmp(((t_token *)current->content)->type, "finished", 8) == 0)
//         current = current->next;
//     while (current && ft_strncmp(((t_token *)current->content)->type, "WORD", 4) == 0)
//     {
//         current = current->next;
//         len++;   
//     }
//     if (len == 0)
//         return NULL;
//     args = generate_args(*tokens, len);
//     node->type = "CMD";
//     node->argumnts = args;
//     // to move the cursor after the last word.
//     *tokens = current;
//     return (node);
// }

static char **generate_args(t_list *tokens, int len)
{
    int i;
    char **args;

    args = ft_calloc(len + 1, sizeof(char *));
    i = 0;
    while(tokens)
    {
        if(ft_strncmp(((t_token *)tokens->content)->type, "finished", 8) == 0)
        {
            tokens = tokens->next;
            continue;
        }
        args[i] = ((t_token *)tokens->content)->content;
        tokens = tokens->next;
        i++;
    }
    return (args);
}

t_tree *create_comand_node(t_list **tokens)
{
    t_tree *node;
    t_list *current;
    char **args;
    int len;
    
    current = *tokens;
    len = 0;
    node = ft_calloc(1, sizeof(t_tree));
    while (current)
    {
        if(ft_strncmp(((t_token *)current->content)->type, "WORD", 4) == 0)
            len++;
        current = current->next;
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

