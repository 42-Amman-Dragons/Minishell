/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/25 15:46:09 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_open_files(t_minishell *shell) 
{ 
	t_list *curr;
	int fd;
	if(!shell->openfiles)
		return;
	curr = shell->openfiles;
	while(curr)
	{
		fd = *((int *)curr->content);
		close(fd);
		curr = curr->next;
	}
	ft_lstclear(&(shell->openfiles), free); 
}


void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;

	if (handle_redirections(node) == -1)
		free_and_exit(node, shell, 1);
	close_open_files(shell);
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
		cmd_not_found(cmd_name, node, shell);
	if (execve(node->data.cmd.args[0], node->data.cmd.args, shell->env) == -1)
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": command not found\n", 2);
		free(cmd_name);
		free_and_exit(node, shell, 127);
	}
}

int	temp_dup_error(int temp_stdin, int temp_stdout, t_minishell *shell)
{
	if (temp_stdin != -1)
		close(temp_stdin);
	if (temp_stdout != -1)
		close(temp_stdout);
	ft_putstr_fd("minishell: dup error\n", 2);
	shell->exit_status = 1;
	return (1);
}

int	redirection_failure(int temp_stdin, int temp_stdout, t_minishell *shell)
{
	if (dup2(temp_stdin, STDIN_FILENO) == -1
		|| dup2(temp_stdout, STDOUT_FILENO) == -1)
		perror("minishell: dup2");
	close(temp_stdin);
	close(temp_stdout);
	shell->exit_status = 1;
	return (shell->exit_status);
}

int	run_cmd(t_minishell *shell, t_tree *node)
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
	close_heredoc_fds(node);
	waitpid(id, &status, 0);
	set_signals_prompt();
	shell->exit_status = child_exit_status(status);
	return (shell->exit_status);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	idx;
	int	temp_stdin;
	int	temp_stdout;
	struct stat path_stat;

	idx = -1;
	ft_memset(&path_stat, 0, sizeof(path_stat));
	if (node->data.cmd.args)
	{
		if(contains(node->data.cmd.args[0], '/'))
		{
			stat(node->data.cmd.args[0], &path_stat);
			if(S_ISDIR(path_stat.st_mode) != 0)
			{
				ft_putstr_fd("minishell: ",2);
				ft_putstr_fd(node->data.cmd.args[0], 2);
				ft_putstr_fd(": Is a directory\n", 2);
				shell->exit_status = 126;
				return (shell->exit_status);
			}
		}
		idx = is_builtin(node->data.cmd.args[0]);
		if (idx >= 0)
		{
			temp_stdin = dup(STDIN_FILENO);
			temp_stdout = dup(STDOUT_FILENO);
			if (temp_stdin == -1 || temp_stdout == -1)
				return (temp_dup_error(temp_stdin, temp_stdout, shell));
			if (handle_redirections(node) == -1)
				return (redirection_failure(temp_stdin, temp_stdout, shell));
			add_to_openfiles(shell, temp_stdin);
			add_to_openfiles(shell, temp_stdout);
			shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
			if (dup2(temp_stdin, STDIN_FILENO) == -1
				|| dup2(temp_stdout, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2");
				shell->exit_status = 1;
			}
			close(temp_stdin);
			close(temp_stdout);
			return (shell->exit_status);
		}
	}		
	return (run_cmd(shell, node));
}
