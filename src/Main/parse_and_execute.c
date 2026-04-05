/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:20:00 by haya              #+#    #+#             */
/*   Updated: 2026/04/05 16:40:30 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_err_heredoc(char *token)
{
	char	*buff;

	buff = ft_strdup("");
	buff = safe_join(buff, "minishell: ");
	buff = safe_join(buff, "syntax error near unexpected token `");
	buff = safe_join(buff, token);
	buff = safe_join(buff, "\'\n");
	ft_putstr_fd(buff, 2);
	free(buff);
	buff = NULL;
}

int	handle_heredoc(t_tree *tree, t_minishell *shell, int err)
{
	if (init_heredocs(tree, shell) == -1)
	{
		shell->exit_status = 130;
		close_tracked_fds(shell);
		free_tree(tree);
		return (-1);
	}
	if (err < 0)
	{
		shell->exit_status = 2;
		if (err == -1)
			syntax_err_heredoc("newline");
		close_tracked_fds(shell);
		free_tree(tree);
		return (-1);
	}
	return (0);
}

static t_tree	*build_tree(t_minishell *shell)
{
	t_list	*tokens;
	t_tree	*tree;
	int		err;

	err = 1;
	tokens = tokenizer(shell->line, &err);
	if (!tokens)
	{
		shell->exit_status = err;
		return (NULL);
	}
	err = 0;
	tree = build_ast(tokens, &err);
	ft_lstclear(&tokens, free_token);
	if (!tree)
	{
		shell->exit_status = err;
		return (NULL);
	}
	if (handle_heredoc(tree, shell, err) != 0)
		return (NULL);
	return (tree);
}

void	parse_and_execute(t_minishell *shell)
{
	t_tree	*tree;

	tree = build_tree(shell);
	if (!tree)
	{
		if (!shell->is_interactive && shell->exit_status == 2)
			exit(cleanup_shell(shell, 2));
		return ;
	}
	shell->current_tree = tree;
	shell->exit_status = exec_tree(tree, shell);
	shell->current_tree = NULL;
	close_tracked_fds(shell);
	rl_on_new_line();
	free_tree(tree);
}
