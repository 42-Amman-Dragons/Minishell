/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/13 00:24:43 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*safe_join(char *str1, char *str2)
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
	if (!path)
		return (NULL);
	return (ft_strdup(path + 5));
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

void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;

	if (handle_redirections(node, shell) == -1)
		free_and_exit(node, shell, 1);
	if (!node->data.cmd.args)
		free_and_exit(node, shell, 0);
	cmd_name = ft_strdup(node->data.cmd.args[0]);
	if (!ft_strchr(node->data.cmd.args[0], '/'))
	{
		free(node->data.cmd.args[0]);
		node->data.cmd.args[0] = absoulute_path(cmd_name, shell->env);
	}
	if (!node->data.cmd.args[0])
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": command not found\n", 2);
		free(cmd_name);
		free_and_exit(node, shell, 127);
	}
	execve(node->data.cmd.args[0], node->data.cmd.args, shell->env);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(cmd_name);
	if (errno == ENOENT)
		free_and_exit(node, shell, 127);
	free_and_exit(node, shell, 126);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	id;
	int	status;
	int	idx;
	int	temp_stdin;
	int	temp_stdout;

	idx = -1;
	if (node->data.cmd.args)
	{
		idx = is_builtin(node->data.cmd.args[0]);
		if (idx >= 0)
		{
			temp_stdin = dup(STDIN_FILENO);
			temp_stdout = dup(STDOUT_FILENO);
			if (temp_stdin == -1 || temp_stdout == -1)
			{
				if (temp_stdin != -1)
					close(temp_stdin);
				if (temp_stdout != -1)
					close(temp_stdout);
				shell->exit_status = 1;
				return (1);
			}
			if (handle_redirections(node, shell) == -1)
			{
				dup2(temp_stdin, STDIN_FILENO);
				dup2(temp_stdout, STDOUT_FILENO);
				close(temp_stdin);
				close(temp_stdout);
				shell->exit_status = 1;
				return (1);
			}
			shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
			dup2(temp_stdin, STDIN_FILENO);
			dup2(temp_stdout, STDOUT_FILENO);
			close(temp_stdin);
			close(temp_stdout);
			return (shell->exit_status);
		}
	}
	set_signals_exec();
	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		set_signals_prompt();
		return (errno);
	}
	if (id == 0)
	{
		set_signals_child();
		execve_cmd(node, shell);
	}
	waitpid(id, &status, 0);
	set_signals_prompt();
	if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (shell->exit_status);
}
