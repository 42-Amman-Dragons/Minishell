/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 02:45:56 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/19 06:22:06 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	rebuild_prompt_sigint(void)
{
	char buff[PATH_MAX];
	char *prompt;

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