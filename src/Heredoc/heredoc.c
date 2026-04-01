/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/31 10:45:17 by haya             ###   ########.fr       */
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

static char	*strip_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		sq;
	int		dq;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !dq)
			sq = !sq;
		else if (str[i] == '"' && !sq)
			dq = !dq;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	push_heredoc_line(int fd, char *line, t_redir_data *rd,
		t_minishell *shell)
{
	char	*expanded;

	if (rd->heredoc_expand)
	{
		expanded = expand_word_heredoc(line, shell->env, shell->exit_status);
		if (expanded)
		{
			ft_putstr_fd(expanded, fd);
			ft_putchar_fd('\n', fd);
			free(expanded);
		}
	}
	else
	{
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
	}
}

static int	process_redir_list(t_list *redirs, t_minishell *shell, int *idx)
{
	t_redir_data	*rd;
	char			*stripped;

	while (redirs)
	{
		rd = (t_redir_data *)redirs->content;
		if (rd->mode == DIR_IN_HEREDOC)
		{
			stripped = strip_quotes(rd->filename);
			if (!stripped)
				return (-1);
			free(rd->filename);
			rd->filename = stripped;
			if (setup_heredoc_fd(rd, shell, (*idx)++) == -1)
				return (-1);
			track_fd(shell, &rd->heredoc_fd);
		}
		redirs = redirs->next;
	}
	return (0);
}

int	init_heredocs(t_tree *tree, t_minishell *shell)
{
	static int	idx = 0;

	if (!tree)
		return (0);
	if (tree->type == NODE_CMD)
		return (process_redir_list(tree->data.cmd.redirections, shell, &idx));
	if (tree->type == NODE_SUBSHELL)
	{
		if (process_redir_list(tree->data.subshell.redirections, shell, &idx))
			return (-1);
		return (init_heredocs(tree->data.subshell.child, shell));
	}
	if (init_heredocs(tree->data.oper.left, shell) == -1)
		return (-1);
	return (init_heredocs(tree->data.oper.right, shell));
}
