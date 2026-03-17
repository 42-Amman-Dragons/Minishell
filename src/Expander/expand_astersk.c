/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_astersk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 09:53:10 by haya              #+#    #+#             */
/*   Updated: 2026/03/17 05:28:10 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_matching(char *pattern, char *file_name)
{
	if (!*pattern && !*file_name)
		return (1);
	if (!*pattern || !*file_name)
		return (0);
	if (*pattern == '*')
		return (is_matching(pattern + 1, file_name) || is_matching(pattern,
				file_name + 1) || is_matching(pattern + 1, file_name + 1));
	if (*pattern == *file_name)
		return (is_matching(pattern + 1, file_name + 1));
	return (0);
}

void	add_to_result(int *i, char **result, char *file_name)
{
	if (*i != 0)
		*result = append_str(*result, ft_strdup(" "));
	*result = append_str(*result, ft_strdup(file_name));
	(*i)++;
}

char	*append_astersk(char *result, char *pattern)
{
	DIR				*current_dir;
	char			buff[1204];
	struct dirent	*current_file;
	int				i;

	getcwd(buff, 1204);
	current_dir = opendir(buff);
	i = 0;
	result = NULL;
	if (!current_dir)
		return (NULL);
	while (1)
	{
		current_file = readdir(current_dir);
		if (!current_file)
			break ;
		if (pattern[0] == '*' && current_file->d_name[0] == '.')
			continue ;
		if (is_matching(pattern, current_file->d_name))
			add_to_result(&i, &result, current_file->d_name);
	}
	closedir(current_dir);
	return (result);
}
