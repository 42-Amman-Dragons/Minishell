/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 02:53:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/06 13:20:27 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_get_path(char **args, char **path_out, t_minishell *shell)
{
	if (!args[1] || ft_strncmp(args[1], "~", 1) == 0)
	{
		*path_out = get_env_value("HOME", shell->env);
		if (!*path_out)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
		if (args[1])
			*path_out = ft_strjoin(*path_out, args[1] + 1);
		else
			*path_out = ft_strdup(*path_out);
	}
	else if (args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	else
		*path_out = ft_strdup(args[1]);
	return (0);
}

void	safe_free(char **ptr)
{
	if (!*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

int	cd_error(char **path)
{
	char	*buff;

	buff = ft_strdup("");
	buff = safe_join(buff, "minishell: ");
	buff = safe_join(buff, "cd: ");
	buff = safe_join(buff, *path);
	buff = safe_join(buff, ": ");
	buff = safe_join(buff, strerror(errno));
	buff = safe_join(buff, "\n");
	ft_putstr_fd(buff, 2);
	if (buff)
		free(buff);
	buff = NULL;
	safe_free(path);
	return (1);
}

int	ft_cd(char **args, t_minishell *shell)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	if (cd_get_path(args, &path, shell) != 0)
		return (1);
	oldpwd = get_env_value("PWD", shell->env);
	if (chdir(path) != 0)
		return (cd_error(&path));
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		safe_free(&path);
		return (1);
	}
	if (oldpwd)
		set_env_value("OLDPWD", oldpwd, shell);
	set_env_value("PWD", newpwd, shell);
	update_prompt_path(shell);
	free(newpwd);
	safe_free(&path);
	return (0);
}
