/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 00:00:00 by haya              #+#    #+#             */
/*   Updated: 2026/04/05 16:48:34 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_unclosed_quotes(char *line)
{
	int	sq;
	int	dq;
	int	i;

	sq = 0;
	dq = 0;
	i = -1;
	while (line[++i])
	{
		if (line[i] == '"' && !sq)
			dq = !dq;
		else if (line[i] == '\'' && !dq)
			sq = !sq;
	}
	return (sq || dq);
}

static void	join_next_line(t_minishell *shell)
{
	char	*next;
	char	*tmp;

	while (has_unclosed_quotes(shell->line))
	{
		next = get_next_line(STDIN_FILENO);
		if (!next)
			break ;
		tmp = ft_strjoin(shell->line, next);
		free(shell->line);
		free(next);
		shell->line = tmp;
	}
}

// ft_putendl_fd("minishell$ ", STDOUT_FILENO);
/* fixed prompt for tester */
int	process_line_non_interactive(t_minishell *shell)
{
	size_t	len;

	ft_putendl_fd("minishell$ ", STDOUT_FILENO);
	shell->line = get_next_line(STDIN_FILENO);
	if (!shell->line)
		return (1);
	join_next_line(shell);
	len = ft_strlen(shell->line);
	if (len > 0 && shell->line[len - 1] == '\n')
		shell->line[len - 1] = '\0';
	if (ft_strlen(shell->line) == 0)
	{
		free(shell->line);
		shell->line = NULL;
		return (0);
	}
	parse_and_execute(shell);
	free(shell->line);
	shell->line = NULL;
	return (0);
}
