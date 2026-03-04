/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 03:03:59 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/03 03:17:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int		i;
	char	*builtins[8];

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (ft_strlen(cmd) == ft_strlen(builtins[i])
			&& ft_strncmp(cmd, builtins[i], ft_strlen(cmd)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_exit(char **args, t_minishell *shell)
{
	(void)args;
	ft_putstr_fd("exit\n", 1);
	free_all(shell);
	exit(0);
	return (0);
}

int	call_builtin(int idx, char **args, t_minishell *shell)
{
	if (idx == 0)
		return (ft_echo(args));
	if (idx == 1)
		return (ft_cd(args, shell));
	if (idx == 2)
		return (ft_pwd(shell));
	if (idx == 3)
		return (ft_export(args, shell));
	if (idx == 4)
		return (ft_unset(args, shell));
	if (idx == 5)
		return (ft_env(shell));
	if (idx == 6)
		return (ft_exit(args, shell));
	return (1);
}
