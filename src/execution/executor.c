/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/17 16:56:38 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
void execute_command(char *input, t_shell *shell)
{
    pid_t   pid;
    char    **args;
    char    *cmd_path;
    t_cmd   *cmd;
    int     *saved_fds;
    
    args = parse_command(input);
    if (!args || !args[0])
    {
        free_array(args);
        return;
    }
    
    cmd = parse_cmd_with_redir(args);
    if (!cmd)
    {
        free_array(args);
        return;
    }

    if (is_builtin(cmd->args[0]))
    {
        saved_fds = save_std_fds();
        
        if (cmd->redirs)
        {
            if (execute_redirections(cmd->redirs) == -1)
            {
                restore_std_fds(saved_fds);
                free_cmd(cmd);
                return;
            }
        }
        
        execute_builtin(cmd->args, shell);
        restore_std_fds(saved_fds);
        
        free_cmd(cmd);
        return;
    }
    
    cmd_path = find_command_path(cmd->args[0], shell);
    if (!cmd_path)
    {
        printf("minishell: %s: command not found\n", cmd->args[0]);
        free_cmd(cmd);
        return;
    }
    
    pid = fork();
    
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        free_cmd(cmd);
        return;
    }
    
    if (pid == 0)
    {
        if (cmd->redirs)
        {
            if (execute_redirections(cmd->redirs) == -1)
                exit(1);
        }
        
        if (execve(cmd_path, cmd->args, shell->env) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
    
    free(cmd_path);
    free_cmd(cmd);
}