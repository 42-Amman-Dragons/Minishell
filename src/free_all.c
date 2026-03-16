/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:18 by haya              #+#    #+#             */
/*   Updated: 2026/03/16 10:57:54 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i++] = NULL;
	}
	free(env);
}

void	free_all(t_minishell *shell)
{
	if (!shell)
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, &(shell->original_termos));
	rl_clear_history();
	if (shell->history)
		ft_lstclear(&(shell->history), free);
	shell->history = NULL;
	if (shell->prompt)
		free(shell->prompt);
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if(shell->line) // haya has added this.
	{
		free(shell->line);
		shell->line = NULL;
	}
	ft_bzero(shell, sizeof(*shell));
	free(shell);
}

// ==10933== 4,016 bytes in 1 blocks are still reachable in loss record 37 of 59
// ==10933==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
// ==10933==    by 0x489300C: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.0)
// ==10933==    by 0x488CA5B: add_history (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.0)
// ==10933==    by 0x1095B0: add_to_history (in /home/haya/minishell/minishell)
// ==10933==    by 0x109722: load_history (in /home/haya/minishell/minishell)
// ==10933==    by 0x10DFDA: init_minishell (in /home/haya/minishell/minishell)
// ==10933==    by 0x10DEB9: main (in /home/haya/minishell/minishell)