/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 14:58:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/22 22:23:56 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	resolve_user_host(t_minishell *shell)
{
	if (!get_env_value("USER", shell->env))
		set_env_value("USER", "user", shell);
	shell->username = get_env_value("USER", shell->env);
	if (!get_env_value("HOSTNAME", shell->env))
		set_env_value("HOSTNAME", "42Dragons", shell);
	shell->servername = get_env_value("HOSTNAME", shell->env);
}

char	*get_prompt(t_minishell *shell)
{
	char	buff[PATH_MAX];
	char	*prompt;

	resolve_user_host(shell);
	ft_bzero(buff, PATH_MAX);
	if (getcwd(buff, PATH_MAX) == NULL)
	{
		if (get_env_value("PWD", shell->env))
			ft_strlcpy(buff, get_env_value("PWD", shell->env), PATH_MAX);
		else
			ft_strlcpy(buff, ".", PATH_MAX);
	}
	consider_home_dir(buff, shell->env);
	prompt = NULL;
	change_color(&prompt, DRAGON_GREEN);
	prepare_prompt_beggining(&prompt, shell);
	change_color(&prompt, DRAGON_CYAN);
	prepare_prompt_path(&prompt, buff);
	change_color(&prompt, RESET);
	return (prompt);
}

void	update_prompt_path(t_minishell *shell)
{
	char	*new_prompt;

	new_prompt = get_prompt(shell);
	free(shell->prompt);
	shell->prompt = new_prompt;
}
