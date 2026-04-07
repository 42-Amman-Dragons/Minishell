/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wild_redirect_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 19:30:11 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 12:23:14 by hal-lawa         ###   ########.fr       */
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
	if (!*expanded)
		return (1);
	return (0);
}

static int	handle_ambiguous(char **expanded, t_redir_data *rd,
		char **matched_paths)
{
	if(matched_paths)
		free_args(matched_paths);
	free(*expanded);
	free(rd->filename);
	rd->filename = NULL;
	return (1);
}

int	handle_wild_redirect(char **expanded, t_redir_data *rd)
{
	char	**matched_paths;
	int		count;

	matched_paths = append_astersk(*expanded);
	if (!matched_paths)
	{
		restore_astersks(*expanded);
		if(*expanded)
		{
			free(*expanded);
			*expanded = NULL;
		}
		return (0);
	}
	count = count_matches(matched_paths);
	if (count > 1)
		return (handle_ambiguous(expanded, rd, matched_paths));
	if (count == 1)
		return (handle_single_match(expanded, matched_paths));
	return (handle_no_match_array(expanded, matched_paths));
}
