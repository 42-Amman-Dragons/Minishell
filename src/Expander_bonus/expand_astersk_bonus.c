/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_astersk_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 09:53:10 by haya              #+#    #+#             */
/*   Updated: 2026/04/01 11:09:33 by haya             ###   ########.fr       */
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

static void	add_to_result(int *i, char **result, char *file_name)
{
	if (*i != 0)
		*result = append_str(*result, ft_strdup(" "));
	*result = append_str(*result, ft_strdup(file_name));
	(*i)++;
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

static void	process_entry(struct dirent *cf, char *pattern, char **result,
		int *i)
{
	if (should_skip_file(cf->d_name, pattern))
		return ;
	if (is_matching(pattern, cf->d_name))
		add_to_result(i, result, cf->d_name);
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
	cf = readdir(current_dir);
	while (cf)
	{
		process_entry(cf, pattern, &result, &i);
		cf = readdir(current_dir);
	}
	closedir(current_dir);
	return (result);
}
