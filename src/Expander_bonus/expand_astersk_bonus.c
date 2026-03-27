/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_astersk_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 09:53:10 by haya              #+#    #+#             */
/*   Updated: 2026/03/27 23:50:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	is_matching(char *pattern, char *file_name)
{
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*file_name)
		{
			if (is_matching(pattern, file_name))
				return (1);
			file_name++;
		}
		return (0);
	}
	if (!*pattern && !*file_name)
		return (1);
	if (!*pattern || !*file_name)
		return (0);
	if (*pattern == *file_name)
		return (is_matching(pattern + 1, file_name + 1));
	return (0);
}

static void	sort_matches(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strncmp(arr[i], arr[j], INT_MAX) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static char	*build_result(char **matches, int count)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (i < count)
	{
		if (i != 0)
			result = append_str(result, ft_strdup(" "));
		result = append_str(result, ft_strdup(matches[i]));
		i++;
	}
	return (result);
}

char	*append_astersk(char *result, char *pattern)
{
	DIR				*current_dir;
	char			buff[1204];
	struct dirent	*cf;
	char			**matches;
	int				count;

	free(result);
	if (!getcwd(buff, 1204))
		return (NULL);
	current_dir = opendir(buff);
	if (!current_dir)
		return (NULL);
	matches = ft_calloc(1024, sizeof(char *));
	if (!matches)
		return (closedir(current_dir), NULL);
	count = 0;
	while (1)
	{
		cf = readdir(current_dir);
		if (!cf)
			break ;
		if (pattern[0] == '*' && cf->d_name[0] == '.')
			continue ;
		if (is_matching(pattern, cf->d_name) && count < 1023)
			matches[count++] = cf->d_name;
	}
	sort_matches(matches, count);
	result = build_result(matches, count);
	free(matches);
	closedir(current_dir);
	return (result);
}
