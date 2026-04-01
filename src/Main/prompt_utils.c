/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 15:00:25 by haya              #+#    #+#             */
/*   Updated: 2026/03/31 11:11:23 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_welcome_message(void)
{
	printf("\033[38;5;222m ===============================");
	printf("=================================================\033[0m\n");
	printf("\033[1;32m   ____  ____      _    ____  ___  _   _ ____ \033");
	printf("[0;36m /\\\033[1;32m____  _   _ _____ _     _		\033[0m \n");
	printf("\033[1;32m  |  _ \\|  _ \\    / \\  / ___|/ _ \\| \\ | / ___|\033");
	printf("[0;36m/ \033[1;32m/ ___|| | | | ____| |   | |		\033[0m \n");
	printf("\033[1;32m  | | | | |_) |  / _ \\| |  _| | | |  \\| \\___ \033");
	printf("[0;36m/  \033[1;32m\\___ \\| |_| |  _| | |   | |		");
	printf("\033[0m \n");
	printf("\033[1;32m  | |_| |  _ <  / ___ \\ |_| | |_| | |\\  |___)\033");
	printf("[0;36m\\   \033[1;32m_\033[0;36m/\033[1;32m_) ");
	printf("|  _  | |___| |___| |___	\033[0m \n");
	printf("\033[1;32m  |____/|_| \\_\\/_/   ");
	printf("\\_\\____|\\___/|_| \\_|____/\033");
	printf("[0;36m\\ \033[1;32m|____/|_| |_|_____|_____|_____| \033[0m \n");
	printf("                                               ");
	printf("\033[0;36m\\/\033[0m \n");
	printf("  Done by: Mohannad and Haya 🐉🔥\n");
	printf("\033[38;5;222m ===============================");
	printf("=================================================\033[0m\n");
	printf("\n");
}

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
			buff[1] = '\0';
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
	*prompt = safe_join(*prompt, PROMPT_EMOJI " ");
	*prompt = safe_join(*prompt, shell->username);
	*prompt = safe_join(*prompt, "@");
	*prompt = safe_join(*prompt, shell->servername);
	*prompt = safe_join(*prompt, ":");
}

void	prepare_prompt_path(char **prompt, char *buff)
{
	*prompt = safe_join(*prompt, buff);
	*prompt = safe_join(*prompt, "$ ");
}
