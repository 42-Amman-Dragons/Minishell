/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:42:48 by haya              #+#    #+#             */
/*   Updated: 2026/03/16 10:59:21 by haya             ###   ########.fr       */
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

char	*safe_join(char *str1, char *str2)
{
	char	*result;

	result = ft_strjoin(str1, str2);
	free(str1);
	return (result);
}

void	free_splitted(char **splitted)
{
	int	i;

	if (!splitted)
		return ;
	i = 0;
	while (splitted[i])
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}
