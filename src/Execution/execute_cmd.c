/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/05 13:32:35 by haya             ###   ########.fr       */
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
		}
		i++;
	}
	path = ft_strtrim(path, "PATH=");
	return (path);
}

char	*absoulute_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;
	char	*sub;
	int		i;

	path =  get_path(env);
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

void execve_cmd(t_tree *node, t_minishell *shell)
{
	char *cmd_name;
	handle_redirections(node, shell);
	if (!node->data.cmd.args)
	{
		free_and_exit(node, shell, 0);
	}
	cmd_name = ft_strdup(node->data.cmd.args[0]);
	node->data.cmd.args[0] = absoulute_path(node->data.cmd.args[0], shell->env);
	if (!node->data.cmd.args[0])
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": command not found\n", 2);
		free(cmd_name);
		free_and_exit(node, shell, 127);
	}
	if (execve(node->data.cmd.args[0], node->data.cmd.args, shell->env) == -1)
	{
		perror("EXECVE ERROR: ");
		free(cmd_name);
		free_and_exit(node, shell, 1);
	}
	free(cmd_name);
	free_and_exit(node, shell, 0);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	id;
	int	status;
	int idx;

	idx = -1;
	if(node->data.cmd.args)
	{
		idx = is_builtin(node->data.cmd.args[0]);
		if ( idx >= 0)
			return (call_builtin(idx, node->data.cmd.args, shell));
	}
	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		return (errno);
	}
	if (id == 0)
		execve_cmd(node, shell);
	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (shell->exit_status);
}
