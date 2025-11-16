/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/16 04:30:54 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void execute_command(char *input, t_shell *shell)
{
    pid_t   pid;
    char    **args;
    char    *cmd_path;
    
    args = parse_command(input);
    if (!args || !args[0])
    {
        free_array(args);
        return;
    }
    if (is_builtin(args[0]))
    {
        execute_builtin(args, shell);
        free_array(args);
        return;
    }
    cmd_path = find_command_path(args[0], shell);
    if (!cmd_path)
    {
        printf("minishell: %s: command not found\n", args[0]);
        free_array(args);
        return;
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        free_array(args);
        return;
    }
    
    if (pid == 0)
    {
        if (execve(cmd_path, args, shell->env) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
        wait(NULL);
    free(cmd_path);
    free_array(args);
}