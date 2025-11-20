/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:49 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/18 21:38:35 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);

    return (0);
}

int execute_builtin(char **args, t_shell *shell)
{
    if (ft_strcmp(args[0], "echo") == 0)
        return (builtin_echo(args));
    if (ft_strcmp(args[0], "pwd") == 0)
    {
        if (args[1]){
            printf("pwd: too many arguments\n");
            return (0);
        }
        return (builtin_pwd());
    }
    if (ft_strcmp(args[0], "exit") == 0)
        return (builtin_exit(args));
    if (ft_strcmp(args[0], "env") == 0)
        return (builtin_env(args, shell));
    if (ft_strcmp(args[0], "cd") == 0)
        return (builtin_cd(args));
    if (ft_strcmp(args[0], "export") == 0)
        return (builtin_export(args, shell));
    if (ft_strcmp(args[0], "unset") == 0)
            return (builtin_unset(args, shell));
    
    printf("minishell> %s: command not found\n", args[0]);
    return (0);
}
