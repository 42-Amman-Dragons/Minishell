/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:23:50 by haya              #+#    #+#             */
/*   Updated: 2026/02/11 14:05:37 by haya             ###   ########.fr       */
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
static char	*safe_join(char *str1, char *str2)
{
	char	*result;

	result = ft_strjoin(str1, str2);
	free(str1);
	return (result);
}

void	free_splitted(char **splitted)
{
	int	i;
	
	if (!splitted)
		return ;
	i = 0;
	while (splitted[i])
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}

char	*absoulute_path(char *cmd)
{
	char	*path;
	char	**paths;
	char	*sub;
	int		i;

	path = getenv("PATH");
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		sub = ft_strjoin(paths[i], "/");
		sub = safe_join(sub, cmd);
		if (access(sub, F_OK) == 0)
		{
			errno = 0;
			free_splitted(paths);
			return (sub);
		}
		free(sub);
		i++;
	}
	free_splitted(paths);
	return (NULL);
}

int ft_strcmp(char *s1, char *s2)
{
    int len1;
    int len2;
    
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    if(len1 != len2)
        return (-1);
    return (ft_strncmp(s1, s2, len1));
}

int is_builtin_function(char *token)
{
    if (ft_strcmp(token, "exit") == 0)
        return (1);
    return (0);
}

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
        if(i == 0 && !is_builtin_function(((t_token *)tokens->content)->content))
        {
            args[i] = absoulute_path(((t_token *)tokens->content)->content);
            if(args[i] == NULL)
            {
                free(args);
                return (NULL);
            }
        }
        else 
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

