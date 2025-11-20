/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:19 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/20 02:38:12 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static int is_executable(char *path)
{
    return (access(path, X_OK) == 0);
}
char *find_command_path(char *cmd, t_shell *shell)
{
    char    *path_env;
    char    **dirs;
    char    *full_path;
    char    *temp;
    int     i;
    
    if (ft_strchr(cmd, '/'))
    {
        if (is_executable(cmd))
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    path_env = get_env_value("PATH", shell);

    if (!path_env)
        return (NULL);
    
    dirs = ft_split(path_env, ':');
    if (!dirs)
        return (NULL);
    
    i = 0;
    while (dirs[i])
    {
        temp = ft_strjoin(dirs[i], "/");
        if (!temp)
        {
            free_array(dirs);
            return (NULL);
        }
        
        full_path = ft_strjoin(temp, cmd);
        free(temp);
        
        if (!full_path)
        {
            free_array(dirs);
            return (NULL);
        }
        
        if (is_executable(full_path))
        {
            free_array(dirs);
            return (full_path);
        }
        
        free(full_path);
        i++;
    }
    
    free_array(dirs);
    return (NULL);
}