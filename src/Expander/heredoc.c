/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/10 18:25:01 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	SIGNUM;

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

static int	setup_heredoc_fd(t_redir_data *rd, t_minishell *shell, int idx)
{
	int		fd;
	pid_t	pid;
	int		status;
	char	*num;
	char	*tmp;

	num = ft_itoa(idx);
	tmp = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(tmp), -1);
	set_signals_exec();
	pid = fork();
	if (pid == 0)
		heredoc_child(fd, rd->filename, rd, shell);
	close(fd);
	waitpid(pid, &status, 0);
	set_signals_prompt();
	if (WIFSIGNALED(status))
	{
		SIGNUM = WTERMSIG(status);
		write(1, "\n", 1);
		return (unlink(tmp), free(tmp), -1);
	}
	rd->heredoc_fd = open(tmp, O_RDONLY);
	unlink(tmp);
	free(tmp);
	return (0);
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
			free(rd->filename);
			rd->filename = stripped;
			if (setup_heredoc_fd(rd, shell, (*idx)++) == -1)
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
