/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:20:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/04 23:00:00 by mabuqare         ###   ########.fr       */
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
	init_heredocs(tree, shell);
	expander(tree, shell);
	return (tree);
}

static void	exec_external(t_tree *tree, t_minishell *shell)
{
	int	id;
	int	status;

	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		return ;
	}
	if (id == 0)
		exec_tree(tree, shell);
	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	rl_on_new_line();
}

void	parse_and_execute(t_minishell *shell)
{
	t_tree	*tree;
	int		idx;

	tree = build_tree(shell);
	if (!tree)
		return ;
	if (tree->type == NODE_CMD && tree->data.cmd.args)
	{
		idx = is_builtin(tree->data.cmd.args[0]);
		if (idx >= 0)
		{
			shell->exit_status = call_builtin(idx,
					tree->data.cmd.args, shell);
			free_tree(tree);
			return ;
		}
	}
	exec_external(tree, shell);
	free_tree(tree);
}
