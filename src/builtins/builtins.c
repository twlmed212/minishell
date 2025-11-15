#include "../../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strncmp(cmd, "echo", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "cd", 2) == 0)
        return (1);
    if (ft_strncmp(cmd, "pwd", 3) == 0)
        return (1);
    if (ft_strncmp(cmd, "export", 6) == 0)
        return (1);
    if (ft_strncmp(cmd, "unset", 5) == 0)
        return (1);
    if (ft_strncmp(cmd, "env", 3) == 0)
        return (1);
    if (ft_strncmp(cmd, "exit", 4) == 0)
        return (1);

    return (0);
}

int execute_builtin(char **args, char **env)
{
    int len = ft_strlen(args[0]);

    if (len == 4 && ft_strncmp(args[0], "echo", 4) == 0)
        return (builtin_echo(args));
    if (len == 3 && ft_strncmp(args[0], "pwd", 3) == 0)
    {
        if (args[1]){
            printf("pwd: too many arguments\n");
            return (0);
        }
        return (builtin_pwd());
    }
    if (len == 4 && ft_strncmp(args[0], "exit", 4) == 0)
        return (builtin_exit(args));
    if (len == 3 && ft_strncmp(args[0], "env", 3) == 0)
        return (builtin_env(env));
    
    printf("minishell> %s: command not found\n", args[0]);
    return (0);
}
