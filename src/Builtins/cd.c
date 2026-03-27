/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 02:53:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/26 09:55:02 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int len1;
	int len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if(len1 != len2)
		return(1);
	return (strncmp(s1,s2,len1));
}

static int	cd_get_path(char **args, char **path_out, t_minishell *shell)
{
	// cd ~
	if (!args[1] ||  ft_strcmp(args[1], "~") == 0)
	{
		*path_out = get_env_value("HOME", shell->env);
		if (!*path_out)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
	}
	//  cd -
	// else if(ft_strcmp(args[1], "-")==0)
	// {
	// 	*path_out = get_env_value("OLDPWD", shell->env);
	// 	if (!*path_out)
	// 	{
	// 		ft_putstr_fd("cd: OLDPWD not set\n", 2);
	// 		return (1);
	// 	}
	// }
	// else if(ft_strcmp(args[1], "+")==0)
	// {
	// 	*path_out = get_env_value("PWD", shell->env);
	// 	if (!*path_out)
	// 	{
	// 		ft_putstr_fd("cd: PWD not set\n", 2);
	// 		return (1);
	// 	}
	// }
	else if (args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	else
		*path_out = args[1];
	return (0);
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
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (1);
	if (oldpwd)
		set_env_value("OLDPWD", oldpwd, shell);
	// if(ft_strnstr(newpwd))
	set_env_value("PWD", newpwd, shell);
	update_prompt_path(shell);
	free(newpwd);
	return (0);
}
