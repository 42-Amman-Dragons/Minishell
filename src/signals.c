/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 09:12:58 by haya              #+#    #+#             */
/*   Updated: 2026/03/01 09:12:59 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	SIGNUM;

void	ctrl_c_handler(int signalNumber)
{
	SIGNUM = signalNumber;
	rl_replace_line("", 1);
	printf("^C\n");
	rl_redisplay();
	rl_on_new_line();
	rl_redisplay();
}

void	ctrl_d_handler(int signalNumber)
{
	SIGNUM = signalNumber;
	exit(0);
}

void	ctrl_backslash(int signalNumber)
{
	SIGNUM = signalNumber;
	rl_redisplay();
}
