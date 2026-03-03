/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:18 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 12:41:14 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env(char **env)
{
	int i;

	if (!env)
		return;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i++] = NULL;
	}
	free(env);
}


void free_all(t_minishell *shell)
{
	if (!shell)
		return;
	tcsetattr(STDIN_FILENO, TCSANOW, &(shell->original_termos));
	rl_clear_history();
	if (shell->history)
		ft_lstclear(&(shell->history), free);
	if (shell->prompt)
		free(shell->prompt);
	free_env(shell->env);
	shell->env = NULL;
	ft_bzero(shell, sizeof(*shell));
	free(shell);
}



// @TODO: check this leak 
// ==1778== 4,016 bytes in 1 blocks are still reachable in loss record 36 of 58
// ==1778==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
// ==1778==    by 0x489100C: xmalloc (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.0)
// ==1778==    by 0x488AA5B: add_history (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.0)
// ==1778==    by 0x109CBF: add_to_history (in /home/haya/minishell/minishell)
// ==1778==    by 0x109DF7: load_history (in /home/haya/minishell/minishell)
// ==1778==    by 0x1099D5: init_minishell (in /home/haya/minishell/minishell)
// ==1778==    by 0x109B1A: main (in /home/haya/minishell/minishell)