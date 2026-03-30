/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:20:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/30 12:20:12 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*build_tree(t_minishell *shell)
{
	t_list	*tokens;
	t_tree	*tree;
	int		err;

	tokens = tokenizer(shell->line);
	if (!tokens)
		return (NULL);
	err = 0;
	tree = build_ast(tokens, &err);
	ft_lstclear(&tokens, free_token);
	if (!tree)
	{
		shell->exit_status = err;
		return (NULL);
	}
	if (init_heredocs(tree, shell) == -1)
	{
		shell->exit_status = 130;
		close_tracked_fds(shell);
		free_tree(tree);
		return (NULL);
	}
	if(err == 20)
	{
		shell->exit_status = 2;
		ft_putstr_fd("minishell: syntax error near unexpected token newline`'\n", 2);
		close_tracked_fds(shell);
		free_tree(tree);
		return (NULL);
	}
	expander(tree, shell);
	return (tree);
}

void	parse_and_execute(t_minishell *shell)
{
	t_tree	*tree;

	tree = build_tree(shell);
	if (!tree)
	{
		// In non-interactive a syntax error should exit
		if (!shell->is_interactive && shell->exit_status == 2)
			exit(cleanup_shell(shell, 2));
		return ;
	}
	shell->exit_status = exec_tree(tree, shell);
	close_tracked_fds(shell);
	rl_on_new_line();
	free_tree(tree);
}
