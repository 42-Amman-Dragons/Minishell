/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 10:56:26 by haya              #+#    #+#             */
/*   Updated: 2026/02/11 14:24:19 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *raise_parse_error(t_list *temp, int *parse_error)
{
    ft_putstr_fd("bash: syntax error near unexpected token ` ", 2);
    write(2, ((t_token *)temp->content)->content, ft_strlen(((t_token *)temp->content)->content));
    write(2, "'\n", 2* sizeof(char));
    *parse_error = 1;
    return(NULL);
}

t_tree *create_empty_node()
{
    t_tree *node;
    node = ft_calloc(1, sizeof(t_tree));
    return (node);
}

t_list *find_type(t_list *tokens, char *type, t_list **prev)
{
    t_list *current;

    current = tokens;
    while(current)
    {
        if(ft_strncmp(((t_token *)current->content)->type, type, ft_strlen(type)) == 0)
        {
            return (current);
        }
        *prev = current; 
        current = current->next;
    }
    return (NULL);
}

/*
@TODO: @mabuquare the << and >> redirections 🤷‍♀️.
*/

t_tree *parse_redir(t_list *tokens, char *type, int *parse_error)
{
    t_list *temp;
    t_list *prev;
    t_tree *node;

    prev = NULL;
    node = NULL;
    temp = find_type(tokens,type, &prev);

    temp = find_type(tokens,type,  &prev);
    if(temp){
        node = create_empty_node();
        node->type = type;
        node->right = NULL;
        // @TODO: if the next is non word then parse error. "DONE"
        if(!temp->next)
            raise_parse_error(temp, parse_error);
        if(ft_strncmp(((t_token *)temp->next->content)->type,"WORD", 4) != 0)
            raise_parse_error(temp, parse_error);
        node->filename = ((t_token *)temp->next->content)->content;
        ((t_token *)temp->content)->type = "finished";
        ((t_token *)temp->next->content)->type = "finished";
        node -> left = parse_tree(tokens, parse_error);
        return (node);
    }
    return (NULL);
}


/*
@TODO: @mabuquare here the bash is opening a new line with > expecting the completion of the pipe 
the same for the && and the ||, I am not sure if it should be handled, what do you think?. 🤔
eg: haya@haya:~$ ls |
>

do this follow the following subject rule ?
• Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).
*/

t_tree * parse_operator(t_list *tokens, char *type, int *parse_error)
{
    t_list *temp;
    t_list *prev;
    t_tree *node;

    prev = NULL;
    node = NULL;
    temp = find_type(tokens,type, &prev);
    // If no privious if should raise an error in some cases. "DONE"
    if(temp)
    {
        node = create_empty_node();
        node->type = type;
        node->right = parse_tree(temp->next, parse_error);
        if(prev)
            prev->next = NULL;
        else
            raise_parse_error(temp, parse_error);
        node->left = parse_tree(tokens, parse_error);
        return (node);
    }
    return( NULL);
}

// _____The order rule_____
// Look for && or ||
// Then |
// Then redirections
// Then plain command
t_tree *parse_tree(t_list *tokens, int *parse_error)
{
    t_tree *node;

    if(!tokens)
        return (NULL);
    node = parse_operator(tokens, "AND", parse_error);
    if(node)
        return(node);
    node = parse_operator(tokens, "OR", parse_error);
    if(node)
        return(node);
    node = parse_operator(tokens, "PIPE", parse_error);
    if(node)
        return(node);
    node = parse_redir(tokens, "DIR_IN", parse_error);
    if(node)
        return(node);
    node = parse_redir(tokens, "DIR_OUT", parse_error);
    if(node)
        return(node);
    return(create_comand_node(&tokens));   
}

void free_tree(t_tree *tree)
{
    if(tree->left)
        free_tree(tree->left);
    if(tree->right)
        free_tree(tree->right);
    if(tree->argumnts)
        free(tree->argumnts);
    free(tree);
}

t_tree *parse_and_execute(t_list *tokens, char *env[])
{
    t_tree *node;
    int err;
    int  id;
    int status;
    
    if(!tokens)
        return (NULL);
    err = 0;
    node = parse_tree(tokens, &err);
    if(err == 1)
    {
        free_tree(node);
        return (NULL);
    }
    print_tree(node);

    id = fork();
    if (id == -1)
    {
        perror("Fork Error: ");
        err = 1;
    }
    if(node->argumnts && is_builtin_function(node->argumnts[0]))
    {
        // execute build in functions
    }

    // @mabuquare [critical archetuctural point] this fork and else statment is needed because the execve inside the exec_tree will replace the
    // current process which means that readline will stop and the programm finished execution after the excve finish
    // so I decided to make a fork in the global level to handle each line seperatly in a child process without affecting the readline.
    // I have tried making the fork inside the exec_cmd but it unnessarly increases the complexity of the logic and open extra process 
    // when there is a parent pipe.
    // note if no wait happened the readline will run again mixing the outputs of the prompt with the result output from the cmd.
    // when having a pipe the readline execute before the pipe finishes. I think this because there is no inner wait inside the exec_tree
    else if(id == 0)
        exec_tree(node, env, &err);
    if(err == 1)
    {
        free_tree(node);
        return (NULL);
    }
    else{
        waitpid(id, &status, 0);
        if (WIFEXITED(status))
            err = WEXITSTATUS(status);
        // |
        // | this is a moc code it will make us loose the exit code for the $?, but needed now for development 
        // V
        // ------------------------------
        while(waitpid(-1,&status, 0) > 0);
        // ------------------------------
        rl_on_new_line();
    }
    return (node);
}
