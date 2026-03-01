/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/01 13:28:22 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void exec_cmd(t_tree *node, char *env[])
{
	handle_redirections(node->data.cmd.redirections);
	if(!node->data.cmd.args)
		exit (0);
    node->data.cmd.args[0] = absoulute_path(node->data.cmd.args[0]);
    if(!node->data.cmd.args[0])
    {
        ft_putstr_fd("Command not found\n", 2);
        exit(1);
    }
    if(execve(node->data.cmd.args[0], node->data.cmd.args, env) == -1)
    {
        perror("EXECVE ERROR: ");
        exit(1);
    }
    exit(0);
}