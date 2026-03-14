/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 09:12:58 by haya              #+#    #+#             */
/*   Updated: 2026/03/13 18:36:42 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

extern int	g_SIGNUM;

static void	sigint_home_dir(char *buff)
{
	char	*home;
	size_t	home_len;
	size_t	buff_len;

	home = getenv("HOME");
	if (!home)
		return ;
	buff_len = ft_strlen(buff);
	home_len = ft_strlen(home);
	if (!ft_strnstr(buff, home, buff_len))
		return ;
	buff[0] = '~';
	if (home_len == buff_len)
	{
		buff[1] = '/';
		buff[2] = '\0';
		return ;
	}
	ft_memmove(buff + 1, buff + home_len, buff_len - home_len + 1);
}

static char	*build_sigint_prompt(void)
{
	char	*prompt;
	char	*user;
	char	*host;

	user = getenv("USER");
	host = getenv("HOSTNAME");
	if (!user)
		user = "user";
	if (!host)
		host = "42Dragons";
	prompt = NULL;
	change_color(&prompt, DRAGON_GREEN);
	prompt = safe_join(prompt, ERROR_EMOJI " ");
	prompt = safe_join(prompt, user);
	prompt = safe_join(prompt, "@");
	prompt = safe_join(prompt, host);
	prompt = safe_join(prompt, ":");
	return (prompt);
}

static void	rebuild_prompt_sigint(void)
{
	char	buff[PATH_MAX];
	char	*prompt;

	if (getcwd(buff, PATH_MAX) == NULL)
		return ;
	sigint_home_dir(buff);
	prompt = build_sigint_prompt();
	change_color(&prompt, DRAGON_CYAN);
	prepare_prompt_path(&prompt, buff);
	change_color(&prompt, RESET);
	rl_set_prompt(prompt);
	free(prompt);
}

void	handle_sigint(int sig)
{
	g_SIGNUM = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rebuild_prompt_sigint();
	rl_redisplay();
}

void	set_signals_prompt(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signals_exec(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signals_child(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signals_heredoc(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
