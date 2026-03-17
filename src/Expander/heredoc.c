/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 14:22:34 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_SIGNUM;

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

static void	heredoc_child(int fd, char *limiter, t_redir_data *rd,
		t_minishell *shell)
{
	char	*line;

	set_signals_child();
	if (!isatty(STDIN_FILENO))
	{
		close(fd);
		exit(0);
	}
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
	close(fd);
	exit(0);
}

int	heredoc_parent(int fd, int pid, t_redir_data *rd, char *tmp)
{
	int	status;

	close(fd);
	waitpid(pid, &status, 0);
	set_signals_prompt();
	if (WIFSIGNALED(status))
	{
		g_SIGNUM = WTERMSIG(status);
		write(1, "\n", 1);
		return (unlink(tmp), free(tmp), -1);
	}
	rd->heredoc_fd = open(tmp, O_RDONLY);
	unlink(tmp);
	free(tmp);
	if (rd->heredoc_fd < 0)
		return (-1);
	return (0);
}

static void	close_inherited_heredoc_fds(t_list *redirs_head)
{
	t_redir_data	*r;

	while (redirs_head)
	{
		r = (t_redir_data *)redirs_head->content;
		if (r->heredoc_fd >= 0)
			close(r->heredoc_fd);
		redirs_head = redirs_head->next;
	}
}

static int	fork_heredoc(char *tmp, t_redir_data *rd,
		t_minishell *shell, t_list *redirs_head)
{
	pid_t	pid;
	int		fd;

	fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(tmp), -1);
	set_signals_exec();
	pid = fork();
	if (pid == -1)
	{
		close(fd);
		unlink(tmp);
		free(tmp);
		return (-1);
	}
	if (pid == 0)
	{
		close_inherited_heredoc_fds(redirs_head);
		heredoc_child(fd, rd->filename, rd, shell);
	}
	return (heredoc_parent(fd, pid, rd, tmp));
}

static int	setup_heredoc_fd(t_redir_data *rd, t_minishell *shell, int idx,
		t_list *redirs_head)
{
	char	*num;
	char	*tmp;

	num = ft_itoa(idx);
	if (!num)
		return (-1);
	tmp = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	if (!tmp)
		return (-1);
	return (fork_heredoc(tmp, rd, shell, redirs_head));
}

static void	fill_unquoted(char *result, char *str)
{
	int	i;
	int	j;
	int	sq;
	int	dq;

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
}

static char	*strip_quotes(char *str)
{
	char	*result;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	fill_unquoted(result, str);
	return (result);
}

static int	process_redir_list(t_list *redirs, t_minishell *shell, int *idx)
{
	t_redir_data	*rd;
	char			*stripped;
	t_list			*head;

	head = redirs;
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
			if (setup_heredoc_fd(rd, shell, (*idx)++, head) == -1)
				return (-1);
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
	else if (tree->type == NODE_SUBSHELL)
	{
		if (process_redir_list(tree->data.subshell.redirections, shell, &idx) ==
			-1)
			return (-1);
		return (init_heredocs(tree->data.subshell.child, shell));
	}
	if (init_heredocs(tree->data.oper.left, shell) == -1)
		return (-1);
	return (init_heredocs(tree->data.oper.right, shell));
}
