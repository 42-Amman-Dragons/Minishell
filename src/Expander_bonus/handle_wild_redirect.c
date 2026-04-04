/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wild_redirect.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 19:30:11 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 19:56:23 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	count_matches(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		i++;
	return (i);
}

static int	handle_no_match_array(char **expanded, char **matched_paths)
{
	free_args(matched_paths);
	restore_astersks(*expanded);
	return (0);
}

static int	handle_single_match(char **expanded, char **matched_paths)
{
	free(*expanded);
	*expanded = ft_strdup(matched_paths[0]);
	free_args(matched_paths);
	return (0);
}

static int	handle_ambiguous(char **expanded, t_redir_data *rd, t_list **redirs,
		char **matched_paths)
{
	free_args(matched_paths);
	free(*expanded);
	free(rd->filename);
	rd->filename = NULL;
	*redirs = (*redirs)->next;
	return (1);
}

int	handle_wild_redirect(char **expanded, t_redir_data *rd, t_list **redirs)
{
	char	**matched_paths;
	int		count;

	matched_paths = append_astersk(*expanded);
	if (!matched_paths)
	{
		restore_astersks(*expanded);
		free(*expanded);
		return (0);
	}
	count = count_matches(matched_paths);
	if (count > 1)
		return (handle_ambiguous(expanded, rd, redirs, matched_paths));
	if (count == 1)
		return (handle_single_match(expanded, matched_paths));
	return (handle_no_match_array(expanded, matched_paths));
}
