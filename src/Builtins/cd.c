/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 02:53:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/03 22:30:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_get_path(char **args, t_minishell *shell)
{
	if (!args[1])
	{
		args[1] = get_env_value("HOME", shell->env);
		if (!args[1])
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	ft_cd(char **args, t_minishell *shell)
{
	char	*oldpwd;
	char	*newpwd;

	if (cd_get_path(args, shell) != 0)
		return (1);
	oldpwd = get_env_value("PWD", shell->env);
	if (chdir(args[1]) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (1);
	if (oldpwd)
		set_env_value("OLDPWD", oldpwd, shell);
	set_env_value("PWD", newpwd, shell);
	free(newpwd);
	return (0);
}
