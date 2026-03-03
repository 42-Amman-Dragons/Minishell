/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 13:14:46 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *safe_join(char *str1, char *str2)
{
	char *result;

	result = ft_strjoin(str1, str2);
	free(str1);
	return (result);
}

void free_splitted(char **splitted)
{
	int i;

	if (!splitted)
		return;
	i = 0;
	while (splitted[i])
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}

char *absoulute_path(char *cmd)
{
	char *path;
	char **paths;
	char *sub;
	int i;

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

void exec_cmd(t_tree *node,t_minishell *shell)
{
	handle_redirections(node, shell);
	if (!node->data.cmd.args)
	{
		free_and_exit(node, shell, 0);
	}
	node->data.cmd.args[0] = absoulute_path(node->data.cmd.args[0]);
	if (!node->data.cmd.args[0])
	{
		ft_putstr_fd("Command not found\n", 2);
		free_and_exit(node, shell, 127);
	}
	if (execve(node->data.cmd.args[0], node->data.cmd.args, shell->env) == -1)
	{
		perror("EXECVE ERROR: ");
		free_and_exit(node, shell, 1);
	}
	free_and_exit(node, shell, 0);
}
