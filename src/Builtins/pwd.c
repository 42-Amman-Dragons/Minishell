/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 02:57:13 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 03:41:59 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_minishell *shell)
{
	char	*pwd;

	pwd = get_env_value("PWD", shell->env);
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (1);
		ft_putstr_fd(pwd, 1);
		ft_putchar_fd('\n', 1);
		free(pwd);
		return (0);
	}
	ft_putstr_fd(pwd, 1);
	write(1, "\n", 1);
	return (0);
}
