/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/31 11:24:53 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*parse_pipe_seq(t_list **cur, int *err)
{
	t_tree	*left;
	t_tree	*right;

	left = parse_cmd_or_sub(cur, err);
	if (*err && *err > 0)
		return (NULL);
	while (cur_type(cur) == PIPE)
	{
		advance(cur);
		right = parse_cmd_or_sub(cur, err);
		if (*err && *err > 0)
		{
			free_tree(left);
			return (NULL);
		}
		left = create_oper_node(NODE_PIPE, left, right, err);
		if (*err && *err > 0)
			return (NULL);
	}
	return (left);
}

t_tree	*parse_logic_expr(t_list **cur, int *err)
{
	t_tree		*left;
	t_tree		*right;
	t_node_type	type;

	left = parse_pipe_seq(cur, err);
	if (*err && *err > 0)
		return (NULL);
	while (cur_type(cur) == AND || cur_type(cur) == OR)
	{
		if (cur_type(cur) == AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		advance(cur);
		right = parse_pipe_seq(cur, err);
		if (*err && *err > 0)
		{
			free_tree(left);
			return (NULL);
		}
		left = create_oper_node(type, left, right, err);
		if (*err && *err > 0)
			return (NULL);
	}
	return (left);
}

t_tree	*build_ast(t_list *tokens, int *err)
{
	t_list	*cur;
	t_tree	*tree;

	if (!tokens)
		return (NULL);
	cur = tokens;
	tree = parse_logic_expr(&cur, err);
	if (*err && *err > 0)
	{
		free_tree(tree);
		return (NULL);
	}
	if (cur && *err > 0)
	{
		syntax_error(cur, err);
		free_tree(tree);
		return (NULL);
	}
	return (tree);
}
