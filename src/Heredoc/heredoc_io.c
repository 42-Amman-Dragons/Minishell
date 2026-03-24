/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/23 22:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signum;

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
	if (!line)
		print_eof_warning(limiter);
	close(fd);
	exit(0);
}

static int	heredoc_parent(int fd, int pid, t_redir_data *rd, char *tmp)
{
	int	status;

	close(fd);
	waitpid(pid, &status, 0);
	set_signals_prompt();
	if (WIFSIGNALED(status))
	{
		g_signum = WTERMSIG(status);
		ft_putchar_fd('\n', 1);
		unlink(tmp);
		free(tmp);
		return (-1);
	}
	rd->heredoc_fd = open(tmp, O_RDONLY);
	unlink(tmp);
	free(tmp);
	if (rd->heredoc_fd < 0)
		return (-1);
	return (0);
}

static int	fork_heredoc(char *tmp, t_redir_data *rd,
		t_minishell *shell, t_list *redirs_head)
{
	pid_t	pid;
	int		fd;

	fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(tmp);
		return (-1);
	}
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

int	setup_heredoc_fd(t_redir_data *rd, t_minishell *shell, int idx,
		t_list *redirs_head)
{
	char	*num;
	char	*tmp;
	int		ret;

	num = ft_itoa(idx);
	if (!num)
		return (-1);
	tmp = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	if (!tmp)
		return (-1);
	if (!shell->is_interactive)
	{
		ret = read_heredoc_nointeractive(rd, shell, tmp);
		free(tmp);
		return (ret);
	}
	return (fork_heredoc(tmp, rd, shell, redirs_head));
}
