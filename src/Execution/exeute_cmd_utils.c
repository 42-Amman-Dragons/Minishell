/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeute_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 10:32:15 by haya              #+#    #+#             */
/*   Updated: 2026/03/17 05:28:05 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path(char **env)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			path = env[i];
			break ;
		}
		i++;
	}
	if (!path)
		return (NULL);
	return (ft_strdup(path + 5));
}

static char	*asssemple_path(char *one_path, char *cmd)
{
	char	*sub;

	sub = ft_strjoin(one_path, "/");
	sub = safe_join(sub, cmd);
	return (sub);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*sub;
	int		i;

	i = 0;
	while (paths[i])
	{
		sub = asssemple_path(paths[i], cmd);
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

char	*absoulute_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;

	path = get_path(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}

void	cmd_not_found(char *cmd_name, t_tree *node, t_minishell *shell)
{
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
	free(cmd_name);
	free_and_exit(node, shell, 127);
}
