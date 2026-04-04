/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/04 14:13:53 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	handle_wild_redirect(char **expanded, t_redir_data *rd,
		t_list **redirs)
{
	char	**matched_paths;
	int 	i;

	i =0;
	matched_paths = append_astersk(*expanded);
	if(!matched_paths)
	{
		restore_astersks(*expanded);
		free(*expanded);
		return (0);
	}	
	while (matched_paths[i])
	{
		rd = (t_redir_data *)(*redirs)->content;
		rd->filename = ft_strdup(matched_paths[i]);
		free(matched_paths[i]);
		matched_paths[i] = NULL;
		*redirs = (*redirs)->next;
		i++;
	}
	free(matched_paths);
	return (1);
}

static void	expand_redirs(t_list *redirs, t_minishell *shell)
{
	t_redir_data	*rd;
	char			*expanded;

	while (redirs)
	{
		rd = (t_redir_data *)redirs->content;
		if (rd->mode != DIR_IN_HEREDOC)
		{
			expanded = expand_word(rd->filename, shell->env,
					shell->exit_status);
			if (expanded)
			{
				if (ft_strchr(expanded, '*'))
				{
					if (handle_wild_redirect(&expanded, rd, &redirs) == 1)
						continue ;
				}
				free(rd->filename);
				rd->filename = expanded;
			}
		}
		redirs = redirs->next;
	}
}

static void	expand_node(t_tree *node, t_minishell *shell)
{
	int	i;
	int	count;
	int	prev_len;
	int	added;

	i = 0;
	count = 0;
	if (!node->data.cmd.args)
		return (expand_redirs(node->data.cmd.redirections, shell));
	while (node->data.cmd.args[count])
		count++;
	while (i < count)
	{
		prev_len = calc_len_args(node->data.cmd.args);
		node->data.cmd.args = expand_one_arg(node->data.cmd.args, i, shell);
		if (!node->data.cmd.args)
			return ;
		added = calc_len_args(node->data.cmd.args) - prev_len;
		count += added;
		i += 1 + added;
	}
	while (node->data.cmd.args[count])
		count++;
	strip_empty_args(node, count);
	expand_redirs(node->data.cmd.redirections, shell);
}

void	expander(t_tree *tree, t_minishell *shell)
{
	if (!tree)
		return ;
	if (tree->type == NODE_CMD)
		expand_node(tree, shell);
	else if (tree->type == NODE_SUBSHELL)
		expand_redirs(tree->data.subshell.redirections, shell);
}
