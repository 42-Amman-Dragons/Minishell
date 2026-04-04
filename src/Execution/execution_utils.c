/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:42:48 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 18:11:40 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_tracked_fds(t_minishell *shell)
{
	t_list	*curr;
	int		**ref;

	if (!shell->openfiles)
		return ;
	curr = shell->openfiles;
	while (curr)
	{
		ref = (int **)curr->content;
		if (**ref >= 0)
		{
			close(**ref);
			**ref = -1;
		}
		curr = curr->next;
	}
	ft_lstclear(&(shell->openfiles), free);
}

void	secure_close(int fd, t_tree *node, t_minishell *shell)
{
	if (fd == -1)
		return ;
	if (close(fd) == -1)
	{
		perror("Close file error");
		free_and_exit(node, shell, 1);
	}
}

int	child_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

void	free_and_exit(t_tree *node, t_minishell *shell, int exit_code)
{
	close_tracked_fds(shell);
	if (node)
		free_tree(node);
	exit(cleanup_shell(shell, exit_code));
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
