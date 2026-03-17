/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 15:00:25 by haya              #+#    #+#             */
/*   Updated: 2026/03/17 05:28:55 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	consider_home_dir(char *buff, char **env)
{
	char	*home_path;
	char	*home_location;
	size_t	buff_size;

	home_path = get_env_value("HOME", env);
	if (!home_path)
		return ;
	home_location = ft_strnstr(buff, home_path, ft_strlen(buff));
	buff_size = ft_strlen(buff);
	if (home_location)
	{
		buff[0] = '~';
		if (ft_strlen(home_path) == buff_size)
		{
			buff[1] = '/';
			buff[2] = '\0';
			return ;
		}
		ft_memmove(buff + 1, buff + ft_strlen(home_path), buff_size
			- ft_strlen(home_path) + 1);
		ft_bzero(buff + buff_size - ft_strlen(home_path) + 1, 1);
	}
}

void	change_color(char **prompt, char *color)
{
	*prompt = safe_join(*prompt, color);
}

void	prepare_prompt_beggining(char **prompt, t_minishell *shell)
{
	if (shell->exit_status == 0)
		*prompt = safe_join(*prompt, SUCESS_EMOJI);
	else
		*prompt = safe_join(*prompt, ERROR_EMOJI);
	*prompt = safe_join(*prompt, " ");
	*prompt = safe_join(*prompt, shell->username);
	*prompt = safe_join(*prompt, "@");
	*prompt = safe_join(*prompt, shell->servername);
	*prompt = safe_join(*prompt, ":");
}

void	prepare_prompt_path(char **prompt, char *buff)
{
	*prompt = safe_join(*prompt, buff);
	*prompt = safe_join(*prompt, "$ \0");
}
