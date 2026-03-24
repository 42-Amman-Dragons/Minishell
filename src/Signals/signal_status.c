/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 00:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

extern int	g_signum;

void	handle_signal_status(t_minishell *shell)
{
	if (g_signum != 0)
	{
		shell->exit_status = 128 + g_signum;
		g_signum = 0;
	}
}
