/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_err.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:45:54 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 11:36:31 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_command_a_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) && contains(path, '/'))
		return (1);
	return (0);
}

static void	cmd_error_message(char *cmd_name, char *message)
{
	char	*buff;

	buff = ft_strdup("");
	buff = safe_join(buff, cmd_name);
	buff = safe_join(buff, ": ");
	buff = safe_join(buff, message);
	buff = safe_join(buff, "\n");
	ft_putstr_fd(buff, 2);
	if (buff)
		free(buff);
	buff = NULL;
}

void	handle_if_cmd_name(char *cmd_name, t_minishell *shell, int *exit_code)
{
	if (!ft_strchr(cmd_name, '/'))
	{
		if (!get_env_value("PATH", shell->env))
			cmd_error_message(cmd_name, "No such file or directory");
		else
			cmd_error_message(cmd_name, "command not found");
		*exit_code = 127;
	}
	else
	{
		cmd_error_message(cmd_name, strerror(errno));
		if (errno == ENOENT)
			*exit_code = 127;
		else
			*exit_code = 126;
	}
}

void	handle_cmd_error(char *cmd_name, t_tree *node, t_minishell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (is_command_a_directory(cmd_name))
	{
		cmd_error_message(cmd_name, "Is a directory");
		exit_code = 126;
	}
	else if (cmd_name)
		handle_if_cmd_name(cmd_name, shell, &exit_code);
	free(cmd_name);
	free_and_exit(node, shell, exit_code);
}
