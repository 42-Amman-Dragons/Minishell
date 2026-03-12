/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 02:53:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/12 17:09:41 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	print_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	printf("[");
	while (arr[i])
	{
		printf("%s", arr[i]);
		if (arr[i + 1])
			printf(", ");
		i++;
	}
	printf("]");
}

char **add_to_end(char **args, char *path)
{
	int args_size;
	char **new_arg;
	int new_arg_i;

	args_size = 0;
	while(args[args_size])
		args_size++;
	new_arg = malloc((args_size + 2) * sizeof(char *));
	new_arg_i = 0;
	while(new_arg_i < args_size)
	{
		new_arg[new_arg_i] = args[new_arg_i];
		new_arg_i++;
	}
	new_arg[new_arg_i] = ft_strdup(path);
	new_arg[new_arg_i + 1] = NULL;
	return (new_arg);
}

static int	cd_get_path(char ***args, t_minishell *shell)
{
	char *home_path;
	
	home_path = NULL;
	if (!(*args)[1])
	{
		home_path = get_env_value("HOME", shell->env);
		*args = add_to_end(*args, home_path);
		if (!(*args)[1])
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if ((*args)[2])
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

	if (cd_get_path(&args, shell) != 0)
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
	update_prompt_path(shell);
	free(newpwd);
	return (0);
}
