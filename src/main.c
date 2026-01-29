/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by haya              #+#    #+#             */
/*   Updated: 2026/01/29 10:03:51 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

char *get_prompt(char *username, char *servername)
{
    char buff[PATH_MAX];
    char *prompt;
    int user_len;
    int server_len;
    int buff_len;

    ft_bzero(buff, PATH_MAX);
    if(getcwd(buff, PATH_MAX) == NULL)
        return (NULL);
    user_len = ft_strlen(username);
    server_len = ft_strlen(servername);
    buff_len = ft_strlen(buff);
    prompt = malloc(buff_len + user_len + server_len + 6);
    if(!prompt)
        return (NULL);
    ft_memcpy(prompt, username, user_len);
    ft_memcpy(prompt + user_len, "@", 1);
    ft_memcpy(prompt + user_len + 1, servername, server_len);
    ft_memcpy(prompt + user_len + server_len + 1, ":", 1);
    ft_memcpy(prompt + user_len + server_len + 2, buff, buff_len);
    ft_memcpy(prompt + user_len + server_len + buff_len, "$ \0", 3);
    return(prompt);
}

t_minishell *init_minishell()
{
    t_minishell *shell;

    shell = malloc(sizeof(t_minishell));
    if(!shell)
        return (NULL);
    shell->history = NULL;
    shell->line = NULL;
    shell->exit_status = 0;
    load_history(&(shell->history));
    shell->prompt = get_prompt("haya", "dragons");
    if(!shell->prompt)
    {
        free_all(shell);
        return (NULL);
    }
    return (shell);
}

int main() 
{
    t_minishell *shell;
    
    shell = init_minishell();
    if(!shell)
        return (1);
    signal(SIGINT, ctrl_c_handler);
    signal(SIGQUIT, ctrl_backslash);
    while(1)
    {
        shell->line = readline(shell->prompt);
        if(!shell->line)
            break;
        if(add_to_history(shell->line, &(shell->history)) == -1)
        {
                free_all(shell);
                return(1);
        }
        if(ft_strncmp("exit", shell->line, 4) == 0)
            break;
    }
    custom_save_history(&(shell->history));
    free_all(shell);
    return (0);
}
