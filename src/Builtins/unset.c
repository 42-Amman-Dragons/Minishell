/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 04:19:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/13 00:26:51 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(char **args, t_minishell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		del_env_value(args[i], shell);
		i++;
	}
	return (0);
}
