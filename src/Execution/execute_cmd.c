/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/29 17:21:02 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	path_is_unset(t_minishell *shell)
{
	return (get_env_value("PATH", shell->env) == NULL);
}

static int	exec_with_sh_fallback(char **args, char **env)
{
	char	**sh_args;
	int		argc;
	int		i;

	argc = 0;
	while (args[argc])
		argc++;
	sh_args = ft_calloc(argc + 2, sizeof(char *));
	if (!sh_args)
		return (-1);
	sh_args[0] = ft_strdup("/bin/sh");
	if (!sh_args[0])
	{
		free(sh_args);
		return (-1);
	}
	sh_args[1] = args[0];
	i = 1;
	while (i < argc)
	{
		sh_args[i + 1] = args[i];
		i++;
	}
	execve("/bin/sh", sh_args, env);
	free(sh_args[0]);
	free(sh_args);
	return (-1);
}

static void	handle_cmd_error(char *cmd_name, t_tree *node, t_minishell *shell)
{
	int	exit_code;

	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	if (is_command_a_directory(cmd_name))
	{
		ft_putstr_fd("Is a directory\n", 2);
		exit_code = 126;
	}
	else if (!node->data.cmd.args[0])
	{

		if (!ft_strchr(cmd_name, '/') && path_is_unset(shell))
			ft_putstr_fd("No such file or directory\n", 2);
		else
			ft_putstr_fd("command not found\n", 2);
		exit_code = 127;
	}
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		if (errno == ENOENT)
			exit_code = 127;
		else
			exit_code = 126;
	}
	free(cmd_name);
	free_and_exit(node, shell, exit_code);
}

void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;

	if (handle_redirections(node) == -1)
		free_and_exit(node, shell, 1);
	close_tracked_fds(shell);
	if (!node->data.cmd.args)
		free_and_exit(node, shell, 0);
	cmd_name = ft_strdup(node->data.cmd.args[0]);
	if (!cmd_name)
		free_and_exit(node, shell, 1);
	if (!ft_strchr(node->data.cmd.args[0], '/'))
	{
		free(node->data.cmd.args[0]);
		node->data.cmd.args[0] = absoulute_path(cmd_name, shell->env);
	}
	if (!node->data.cmd.args[0])
		handle_cmd_error(cmd_name, node, shell);
	if (execve(node->data.cmd.args[0], node->data.cmd.args, shell->env) == -1)
	{
		if (errno == ENOEXEC)
			exec_with_sh_fallback(node->data.cmd.args, shell->env);
		handle_cmd_error(cmd_name, node, shell);
	}
}

int	handle_external_cmd(t_minishell *shell, t_tree *node)
{
	int	id;
	int	status;

	set_signals_exec();
	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		set_signals_prompt();
		return (1);
	}
	if (id == 0)
	{
		set_signals_child();
		execve_cmd(node, shell);
	}
	waitpid(id, &status, 0);
	set_signals_prompt();
	shell->exit_status = child_exit_status(status);
	return (shell->exit_status);
}

int	handle_builtin(int idx, t_tree *node, t_minishell *shell)
{
	if (temp_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout) ==
		-1)
		return (shell->exit_status = 1);
	track_fd(shell, &shell->builtin_temp_stdin);
	track_fd(shell, &shell->builtin_temp_stdout);
	if (handle_redirections(node) == -1)
	{
		restore_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout);
		return (shell->exit_status = 1);
	}
	shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
	if (restore_redir(&shell->builtin_temp_stdin,
			&shell->builtin_temp_stdout) == -1)
		shell->exit_status = 1;
	return (shell->exit_status);
}

static int	handle_redir_only_cmd(t_tree *node, t_minishell *shell)
{
	if (temp_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout) ==
		-1)
		return (shell->exit_status = 1);
	if (handle_redirections(node) == -1)
	{
		restore_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout);
		return (shell->exit_status = 1);
	}
	if (restore_redir(&shell->builtin_temp_stdin,
			&shell->builtin_temp_stdout) == -1)
		return (shell->exit_status = 1);
	return (shell->exit_status = 0);
}

static void	update_underscore_var(t_tree *node, t_minishell *shell)
{
	int		i;
	char	*last_arg;

	if (!node->data.cmd.args)
		return ;
	i = 0;
	while (node->data.cmd.args[i])
		i++;
	if (i > 0)
	{
		last_arg = node->data.cmd.args[i - 1];
		set_env_value("_", last_arg, shell);
	}
	/* If only the command name is present (e.g. "ls"), keep _ as argv[0]. */
	else if (node->data.cmd.args[0])
		set_env_value("_", node->data.cmd.args[0], shell);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	idx;

	if (!node->data.cmd.args)
		return (handle_redir_only_cmd(node, shell));
	if (node->data.cmd.args)
	{
		update_underscore_var(node, shell);
		idx = is_builtin(node->data.cmd.args[0]);
		if (idx >= 0)
			return (handle_builtin(idx, node, shell));
	}
	return (handle_external_cmd(shell, node));
}
