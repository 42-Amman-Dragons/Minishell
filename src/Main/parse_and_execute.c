/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:20:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/12 10:58:50 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*build_tree(t_minishell *shell)
{
	t_list	*tokens;
	t_tree	*tree;

	tokens = tokenizer(shell->line);
	if (!tokens)
		return (NULL);
	tree = build_ast(tokens);
	ft_lstclear(&tokens, free_token);
	if (!tree)
	{
		shell->exit_status = 2;
		return (NULL);
	}
	if (init_heredocs(tree, shell) == -1)
	{
		shell->exit_status = 130;
		free_tree(tree);
		return (NULL);
	}
	expander(tree, shell);
	return (tree);
}

static void	exec_external(t_tree *tree, t_minishell *shell)
{
	exec_tree(tree, shell);
	rl_on_new_line();
}

void	parse_and_execute(t_minishell *shell)
{
	t_tree	*tree;

	tree = build_tree(shell);
	if (!tree)
		return ;
	exec_external(tree, shell);
	free_tree(tree);
}
