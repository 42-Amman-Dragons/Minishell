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

int main() {

    char *line;
    char *prompt;
    t_list *history;
    int exit_status;
    
    history = NULL;
    exit_status = 0;
    load_history(&history);
    prompt = get_prompt("haya", "dragons");
    signal(SIGINT, ctrl_c_handler);
    signal(SIGQUIT, ctrl_backslash);
    if(!prompt)
        return (1);
    while(1)
    {
        line = readline(prompt);
        if(!line)
            break;
        if(line[ft_strlen(line) ] == '\0' && ft_strlen(line) != 0)
            if(!add_to_history(line, &history))
            {
                free(prompt);
                return(1);
            }
        if(ft_strncmp("exit", line, 4) == 0)
            break;
    }
    custom_save_history(&history);
    free(prompt);
}
