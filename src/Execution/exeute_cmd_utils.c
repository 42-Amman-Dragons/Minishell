/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeute_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 10:32:15 by haya              #+#    #+#             */
/*   Updated: 2026/03/24 03:46:22 by mabuqare         ###   ########.fr       */
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
		return (ft_strdup("/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin"));
	return (ft_strdup(path + 5));
}

int	is_command_a_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

static char	*asssemple_path(char *one_path, char *cmd)
{
	char	*sub;

	sub = ft_strjoin(one_path, "/");
	sub = safe_join(sub, cmd);
	return (sub);
}

char	*absoulute_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;
	char	*sub;
	int		i;

	path = get_path(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		sub = asssemple_path(paths[i], cmd);
		if (access(sub, F_OK) == 0 && !is_command_a_directory(sub))
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
