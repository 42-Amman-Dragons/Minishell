/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/26 13:07:16 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(void *ptr)
{
	t_redir_data	*redir;

	if (!ptr)
		return ;
	redir = (t_redir_data *)ptr;
	if (redir->heredoc_fd >= 0)
	{
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
	free(redir->filename);
	free(redir);
}


void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_tree(t_tree *tree)
{
	if (!tree)
		return ;
	if (tree->type == NODE_CMD)
	{
		free_args(tree->data.cmd.args);
		ft_lstclear(&tree->data.cmd.redirections, free_redir);
	}
	else if (tree->type == NODE_SUBSHELL)
	{
		free_tree(tree->data.subshell.child);
		ft_lstclear(&tree->data.subshell.redirections, free_redir);
	}
	else
	{
		free_tree(tree->data.oper.left);
		free_tree(tree->data.oper.right);
	}
	free(tree);
}

const char	*token_to_str(t_token *tok)
{
	if (tok->type == WORD)
		return (tok->data.word.value);
	if (tok->type == PIPE)
		return ("|");
	if (tok->type == AND)
		return ("&&");
	if (tok->type == OR)
		return ("||");
	if (tok->type == OPEN_PAREN)
		return ("(");
	if (tok->type == CLOSE_PAREN)
		return (")");
	if (tok->data.redir.mode == DIR_OUT_TRUNC)
		return (">");
	if (tok->data.redir.mode == DIR_OUT_APPEND)
		return (">>");
	if (tok->data.redir.mode == DIR_IN_FILE)
		return ("<");
	return ("<<");
}

void	syntax_error(t_list *cur, int *err)
{
	*err = 2;
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!cur)
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd((char *)token_to_str((t_token *)cur->content), 2);
	ft_putstr_fd("'\n", 2);
}
