/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 00:15:40 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/07 17:16:58 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	track_fd(t_minishell *shell, int *heredoc_fd_ptr)
{
	int		**ref;
	t_list	*node;

	ref = malloc(sizeof(int *));
	if (!ref)
		free_and_exit(NULL, shell, 1);
	*ref = heredoc_fd_ptr;
	node = ft_lstnew(ref);
	if (!node)
	{
		free(ref);
		free_and_exit(NULL, shell, 1);
	}
	ft_lstadd_back(&shell->openfiles, node);
}

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
