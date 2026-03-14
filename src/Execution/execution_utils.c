/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:42:48 by haya              #+#    #+#             */
/*   Updated: 2026/03/13 00:25:36 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	secure_close(int fd, t_tree *node, t_minishell *shell)
{
	if (close(fd) == -1)
	{
		perror("Close file error");
		free_and_exit(node, shell, 1);
	}
}

void	free_and_exit(t_tree *node, t_minishell *shell, int exit_code)
{
	free_tree(node);
	free_all(shell);
	exit(exit_code);
}
