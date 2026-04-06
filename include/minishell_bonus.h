/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:22:14 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/06 21:43:41 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include "minishell.h"

/* Bonus execution */
int		exec_and_or(t_tree *node, t_minishell *shell);
int		exec_subshell(t_tree *node, t_minishell *shell);

/* Bonus-only: wildcard expansion */
char	**generate_expanded_list(char **args, int i, char *expanded);
char	**append_astersk(char *pattern);
void	restore_astersks(char *str);
int		is_matching(char *pattern, char *file_name);
int		handle_wild_redirect(char **expanded, t_redir_data *rd);
char	**generate_expanded_list_asterisk(char **args, int i, char **expanded);
int		calc_arr_len(char **arr);
void	*free_args_expanded_asterisk(char **args, char **expanded);

#endif
