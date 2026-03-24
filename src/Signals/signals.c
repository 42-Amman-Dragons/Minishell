/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 09:12:58 by haya              #+#    #+#             */
/*   Updated: 2026/03/23 16:01:28 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

extern int	g_signum;

void	handle_sigint(int sig)
{
	g_signum = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	setup_sig(int signum, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	sa.sa_handler = handler;
	sigaction(signum, &sa, NULL);
}

void	set_signals_prompt(void)
{
	setup_sig(SIGINT, handle_sigint, SA_RESTART);
	setup_sig(SIGQUIT, SIG_IGN, 0);
}

void	set_signals_exec(void)
{
	setup_sig(SIGINT, SIG_IGN, 0);
	setup_sig(SIGQUIT, SIG_IGN, 0);
}

void	set_signals_child(void)
{
	setup_sig(SIGINT, SIG_DFL, 0);
	setup_sig(SIGQUIT, SIG_DFL, 0);
}
