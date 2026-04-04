/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_astersk_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 09:53:10 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 23:42:05 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	restore_astersks(char *str)
{
	while (*str)
	{
		if (*str == '\x01')
			*str = '*';
		str++;
	}
}

char	**append_string_to_list(char ***list, char *str)
{
	char	**new_list;
	int		i;
	int		size;

	i = 0;
	size = calc_arr_len(*list);
	new_list = malloc(sizeof(char *) * (size + 2));
	if (!new_list)
	{
		free(*list);
		*list = NULL;
		return (NULL);
	}
	while (i < size)
	{
		new_list[i] = (*list)[i];
		i++;
	}
	new_list[i] = str;
	new_list[i + 1] = NULL;
	free(*list);
	*list = NULL;
	*list = new_list;
	return (new_list);
}

static int	should_skip_file(char *name, char *pattern)
{
	if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.'
				&& name[2] == '\0')))
		return (1);
	if (name[0] == '.' && pattern[0] != '.')
		return (1);
	return (0);
}

static void	process_entry(struct dirent *cf, char *pattern, char ***result,
		int *i)
{
	if (should_skip_file(cf->d_name, pattern))
		return ;
	if (is_matching(pattern, cf->d_name))
	{
		*result = append_string_to_list(result, ft_strdup(cf->d_name));
		if (!result)
			return ;
		(*i)++;
	}
}

char	**append_astersk(char *pattern)
{
	DIR				*current_dir;
	char			**result;
	char			buff[PATH_MAX];
	struct dirent	*cf;
	int				i;

	result = NULL;
	if (!getcwd(buff, PATH_MAX))
		return (NULL);
	current_dir = opendir(buff);
	if (!current_dir)
		return (NULL);
	i = 0;
	cf = readdir(current_dir);
	while (cf)
	{
		process_entry(cf, pattern, &result, &i);
		cf = readdir(current_dir);
	}
	closedir(current_dir);
	return (result);
}
