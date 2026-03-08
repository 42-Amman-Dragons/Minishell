/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/08 17:55:50 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	word_has_quotes(char *word)
{
	while (*word)
	{
		if (*word == '\'' || *word == '"')
			return (1);
		word++;
	}
	return (0);
}

void	strip_empty_args(t_tree *node, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		if (node->data.cmd.args[i] != NULL)
			node->data.cmd.args[j++] = node->data.cmd.args[i];
		i++;
	}
	node->data.cmd.args[j] = NULL;
	if (j == 0)
	{
		free(node->data.cmd.args);
		node->data.cmd.args = NULL;
	}
}

void add_splitted(char **new_args, int *j, char *expanded)
{
	int i;
	char **splitted;
	
	i = 0;
	splitted = ft_split(expanded, ' ');
	while(splitted[i])
	{
		new_args[*j] = splitted[i];
		(*j)++;
		i++;
	}
	free(splitted);
}

char **generate_expanded_list(char **args, int i, char *expanded)
{
	int j;
	int k;
	int args_len;
	int expanded_len;
	char **new_args;

	args_len = calc_len_args(args);
	expanded_len = cal_len(expanded, ' '); 
	new_args = ft_calloc(args_len + expanded_len + 1, sizeof(char *));
	j = 0;
	k = 0;
	while(k < args_len)
	{
		if(k == i)
		{
			add_splitted(new_args, &j, expanded);
			free(args[k]);
		}
		else
		{
			new_args[j] = args[k];
			j++;
		}
		k++;
	}
	free(args);
	return(new_args);
}

 char **add_to_args(char **args, int i, char *expanded)
{
	if(expanded && contains(expanded, ' ') == 1)
		args = generate_expanded_list(args, i, expanded);
	else
	{
		free(args[i]);
		args[i] = expanded;
	}
	return(args);
}

char 	**expand_one_arg(char **args, int i, t_minishell *shell)
{
	char	*expanded;
	int		quoted;

	quoted = word_has_quotes(args[i]);
	expanded = expand_word(args[i], shell->env, shell->exit_status);
	if (!expanded)
		return (args);
	if (!quoted && expanded[0] == '\0')
	{
		if(contains(args[i], '*'))
		{
			ft_putstr_fd("minishell: ", 2); 
			ft_putstr_fd("no matches found: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("\n", 2);
			free(expanded);
			expanded = NULL;
			return(NULL);
		}
		free(expanded);
		expanded = NULL;
	}
	args = add_to_args(args, i, expanded);
	return(args);
}
