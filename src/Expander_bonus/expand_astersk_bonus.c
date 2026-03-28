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
	if (*pattern == '\x01')
	{
		if (*file_name == '*')
			return (is_matching(pattern + 1, file_name + 1));
		return (0);
	}
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

void	restore_astersks(char *str)
{
	while (*str)
	{
		if (*str == '\x01')
			*str = '*';
		str++;
	}
}

static void	add_to_result(int *i, char **result, char *file_name)
{
	if (*i != 0)
		*result = append_str(*result, ft_strdup(" "));
	*result = append_str(*result, ft_strdup(file_name));
	(*i)++;
}

char	*append_astersk(char *result, char *pattern)
{
	DIR				*current_dir;
	char			buff[PATH_MAX];
	struct dirent	*cf;
	int				i;

	free(result);
	result = NULL;
	if (!getcwd(buff, PATH_MAX))
		return (NULL);
	current_dir = opendir(buff);
	if (!current_dir)
		return (NULL);
	i = 0;
	while (1)
	{
		cf = readdir(current_dir);
		if (!cf)
			break ;
		if (cf->d_name[0] == '.' && (cf->d_name[1] == '\0'
				|| (cf->d_name[1] == '.' && cf->d_name[2] == '\0')))
			continue ;
		if (cf->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (is_matching(pattern, cf->d_name))
			add_to_result(&i, &result, cf->d_name);
	}
	closedir(current_dir);
	return (result);
}
