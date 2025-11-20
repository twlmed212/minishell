/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:14 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/18 21:34:55 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int *save_std_fds(void)
{
    int *saved;
    
    saved = malloc(sizeof(int) * 2);
    if (!saved)
        return (NULL);
    
    saved[0] = dup(STDIN_FILENO);
    saved[1] = dup(STDOUT_FILENO);
    
    return (saved);
}

void restore_std_fds(int *saved)
{
    if (!saved)
        return;
    
    dup2(saved[0], STDIN_FILENO);
    dup2(saved[1], STDOUT_FILENO);
    
    close(saved[0]);
    close(saved[1]);
    free(saved);
}

int execute_output_redir(t_redir *redir)
{
    int fd;
    int flags;
    
    if (redir->type == REDIR_OUT)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else
        flags = O_WRONLY | O_CREAT | O_APPEND;
    fd = open(redir->file, flags, 0644);
    if (fd == -1)
    {
        perror(redir->file);
        return (-1);
    }
    
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return (-1);
    }
    
    close(fd);
    return (0);
}

int execute_input_redir(t_redir *redir)
{
    int fd;
    char *filename;
     if (redir->type == REDIR_HEREDOC)
    {
        // Read heredoc and get temp filename
        filename = read_heredoc(redir->file);  // redir->file = delimiter
        if (!filename)
            return (-1);
        
        // Open the temp file
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            perror(filename);
            unlink(filename);  // Delete temp file
            free(filename);
            return (-1);
        }
        
        // Redirect stdin to temp file
        dup2(fd, 0);
        close(fd);
        
        // Delete temp file (file stays accessible until process ends!)
        unlink(filename);
        free(filename);
    }else{
        if ((fd = open(redir->file, O_RDONLY)) == -1)
        {
            perror(redir->file);
            return (-1);
        }
        
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            return (-1);
        }
        
        close(fd);
    }
    
    return (0);
}

int execute_redirections(t_redir *redirs)
{
    t_redir *current;
    
    current = redirs;
    while (current)
    {
        if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
        {
            if (execute_output_redir(current) == -1)
                return (-1);
        }
        else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
        {
            if (execute_input_redir(current) == -1)
                return (-1);
        }
        current = current->next;
    }
    
    return (0);
}