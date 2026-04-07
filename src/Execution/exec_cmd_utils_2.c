/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 10:32:15 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/04 16:01:21 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_underscore_var(t_tree *node, t_minishell *shell)
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
	else if (node->data.cmd.args[0])
		set_env_value("_", node->data.cmd.args[0], shell);
}

void	print_sigquit_if_needed(int status, t_minishell *shell)
{
	int	err;

	err = child_exit_status(status);
	if (shell->is_child == 0 && (err == 130 || (WIFSIGNALED(status)
				&& WTERMSIG(status) == SIGINT)))
		write(2, "\n", 1);
	if (shell->is_child == 0 && (err == 131 || (WIFSIGNALED(status)
				&& WTERMSIG(status) == SIGQUIT)))
		write(2, "Quit (core dumped)\n", 19);
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
