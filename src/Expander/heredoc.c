/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/27 15:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	push_heredoc_line(int fd, char *line, t_redir_data *rd,
		t_minishell *shell)
{
	char	*expanded;

	if (rd->heredoc_expand)
	{
		expanded = expand_word(line, shell->env, shell->exit_status);
		if (expanded)
		{
			write(fd, expanded, ft_strlen(expanded));
			write(fd, "\n", 1);
			free(expanded);
		}
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

static void	heredoc_line_input(int fd, char *limiter, t_redir_data *rd,
		t_minishell *shell)
{
	char	*line;

	line = readline("> ");
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& ft_strlen(line) == ft_strlen(limiter))
		{
			free(line);
			break ;
		}
		push_heredoc_line(fd, line, rd, shell);
		free(line);
		line = readline("> ");
	}
}

static void	setup_heredoc_fd(t_redir_data *rd, t_minishell *shell, int idx)
{
	int		fd;
	char	*limiter;
	char	*num;
	char	*tmp;

	limiter = expand_word(rd->filename, NULL, 0);
	free(rd->filename);
	rd->filename = limiter;
	num = ft_itoa(idx);
	tmp = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(tmp));
	heredoc_line_input(fd, limiter, rd, shell);
	close(fd);
	rd->heredoc_fd = open(tmp, O_RDONLY);
	unlink(tmp);
	free(tmp);
}

static void	process_redir_list(t_list *redirs, t_minishell *shell, int *idx)
{
	t_redir_data	*rd;

	while (redirs)
	{
		rd = (t_redir_data *)redirs->content;
		if (rd->mode == DIR_IN_HEREDOC)
			setup_heredoc_fd(rd, shell, (*idx)++);
		redirs = redirs->next;
	}
}

void	init_heredocs(t_tree *tree, t_minishell *shell)
{
	static int	idx = 0;

	if (!tree)
		return ;
	if (tree->type == NODE_CMD)
		process_redir_list(tree->data.cmd.redirections, shell, &idx);
	else if (tree->type == NODE_SUBSHELL)
	{
		process_redir_list(tree->data.subshell.redirections, shell, &idx);
		init_heredocs(tree->data.subshell.child, shell);
	}
	else
	{
		init_heredocs(tree->data.oper.left, shell);
		init_heredocs(tree->data.oper.right, shell);
	}
}
