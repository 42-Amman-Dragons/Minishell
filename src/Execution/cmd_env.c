/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:49:47 by haya              #+#    #+#             */
/*   Updated: 2026/03/31 10:40:36 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_is_unset(t_minishell *shell)
{
	return (get_env_value("PATH", shell->env) == NULL);
}

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
