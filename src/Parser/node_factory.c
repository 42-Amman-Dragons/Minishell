/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_factory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/07 09:50:59 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokenType	cur_type(t_list **cur)
{
	if (!*cur)
		return (0);
	return (((t_token *)(*cur)->content)->type);
}

t_token	*advance(t_list **cur)
{
	t_token	*tok;

	if (!*cur)
		return (NULL);
	tok = (t_token *)(*cur)->content;
	*cur = (*cur)->next;
	return (tok);
}

t_tree	*create_oper_node(t_node_type type, t_tree *l, t_tree *r, int *err)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
	{
		free_tree(l);
		free_tree(r);
		*err = 1;
		return (NULL);
	}
	node->type = type;
	node->data.oper.left = l;
	node->data.oper.right = r;
	return (node);
}

t_tree	*create_cmd_node(char **args, t_list *redirs, int *err)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
	{
		free_args(args);
		ft_lstclear(&redirs, free_redir);
		*err = 1;
		return (NULL);
	}
	node->type = NODE_CMD;
	node->data.cmd.args = args;
	node->data.cmd.redirections = redirs;
	return (node);
}

t_tree	*create_subshell_node(t_tree *child, t_list *redirs, int *err)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
	{
		free_tree(child);
		ft_lstclear(&redirs, free_redir);
		*err = 1;
		return (NULL);
	}
	node->type = NODE_SUBSHELL;
	node->data.subshell.child = child;
	node->data.subshell.redirections = redirs;
	return (node);
}
