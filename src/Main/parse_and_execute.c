/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:20:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 13:47:39 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree *build_tree(t_minishell *shell)
{
 	t_list *tokens;
	t_tree *tree; 
    tokens = tokenizer(shell->line);
    if(!tokens)
        return NULL;
    tree = build_ast(tokens);
    ft_lstclear(&tokens, free_token);
    if(!tree)
    {
        ft_putstr_fd("something went wrong", 1);
        shell->exit_status = 1;
        return (NULL);
    }
    init_heredocs(tree, shell);
	expander(tree, shell);
    return (tree);
}

void parse_and_execute(t_minishell *shell)
{
    t_tree *tree;
	int id;
	int status;

	tree = build_tree(shell);
	if(!tree)
		return;
	id = fork();
	if (id == -1)
	{
		perror("Fork Error: ");
		return ;
	}
	if (tree->type == NODE_CMD && tree->data.cmd.args && is_builtin_function(tree->data.cmd.args[0]))// execute build in functions
		return;
	else if (id == 0)
		exec_tree(tree, shell);
	else
    {
		waitpid(id, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		printf("err value = %i\n", shell->exit_status);
		rl_on_new_line();
    }
	free_tree(tree);
}
