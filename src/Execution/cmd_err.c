/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:45:54 by haya              #+#    #+#             */
/*   Updated: 2026/03/30 20:50:27 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmd_error_message(char *cmd_name, char *message)
{
	char	*buff;

	buff = ft_strdup("");
	buff = safe_join(buff, cmd_name);
	buff = safe_join(buff, ": ");
	buff = safe_join(buff, message);
	buff = safe_join(buff, "\n");
	ft_putstr_fd(buff, 2);
	free(buff);
	buff = NULL;
}

void	handle_cmd_error(char *cmd_name, t_tree *node, t_minishell *shell)
{
	int	exit_code;

	if (is_command_a_directory(cmd_name))
	{
		cmd_error_message(cmd_name, "Is a directory");
		exit_code = 126;
	}
	else if (!node->data.cmd.args[0])
	{
		if (!ft_strchr(cmd_name, '/') && path_is_unset(shell))
			cmd_error_message(cmd_name, "No such file or directory");
		else
			cmd_error_message(cmd_name, "command not found");
		exit_code = 127;
	}
	else
	{
		if (!ft_strchr(cmd_name, '/') && errno == ENOENT)
		{
			cmd_error_message(cmd_name, "command not found");
			exit_code = 127;
		}
		else
		{
			cmd_error_message(cmd_name, strerror(errno));
			if (errno == ENOENT)
				exit_code = 127;
			else
				exit_code = 126;
		}
	}
	free(cmd_name);
	free_and_exit(node, shell, exit_code);
}
