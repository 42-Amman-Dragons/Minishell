/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:49:47 by haya              #+#    #+#             */
/*   Updated: 2026/03/30 11:51:51 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_is_unset(t_minishell *shell)
{
	return (get_env_value("PATH", shell->env) == NULL);
}

int	exec_with_sh_fallback(char **args, char **env)
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
	/* If only the command name is present (e.g. "ls"), keep _ as argv[0]. */
	else if (node->data.cmd.args[0])
		set_env_value("_", node->data.cmd.args[0], shell);
}