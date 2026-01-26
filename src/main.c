#include "minishell.h"

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
    prompt = malloc(buff_len + user_len + server_len + 1);
    ft_memcpy(prompt, username, user_len);
    ft_memcpy(prompt + user_len, "@", 1);
    ft_memcpy(prompt + user_len + 1, servername, server_len);
    ft_memcpy(prompt + user_len + server_len + 1, ":", 1);
    ft_memcpy(prompt + user_len + server_len + 2, buff, buff_len);
    ft_memcpy(prompt + user_len + server_len + buff_len, "$ \0", 3);
    return(prompt);
}

void load_history()
{
    int fd;

    fd = open("./history", O_RDWR | O_CREAT, S_IRWXU);
    if (access(fd, F_OK) != 0)
        ft_putstr_fd("cannot find history file\n", 2);
    if (access(fd, R_OK) != 0)
        ft_putstr_fd("read permission denied in the history\n", 2);
    if (access(fd, W_OK) != 0)
        ft_putstr_fd("write permission denied in the history\n", 2);
    // @todo: use get next line to get history from history file and remove the \n and add it to readline history
}

// @todo: create a function to save the content of the history list to the history file 

int add_to_history(char *line, t_list **history)
{
    t_list *node;

    add_history(line);
    node = ft_lstnew(line);
    if(!node)
        return(0);
    ft_lstadd_back(history, node);
    return(1);
}

int main() {

    char *line;
    char *prompt;
    t_list *history;

    prompt = get_prompt("haya", "dragons");
    while(1)
    {
        line = readline(prompt);
        if(line[ft_strlen(line) ] == '\0' && ft_strlen(line) != 0)
        {
            if(!add_to_history(line, &history))
            {
                free(line);
                free(prompt);
                return(1);
            }
        }
        if(ft_strncmp("exit", line, 4) == 0)
            break;
        }
    free(prompt);
    free(line);
}
