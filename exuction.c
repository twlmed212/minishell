#include "minishell.h"

// ----------------- export -----------------

void exec_builtin(t_command *cmd, t_env **env)
{
    if (!cmd || !cmd->args[0])
        return ;

    if (ft_strcmp(cmd->args[0], "echo") == 0)
         builtin_echo(cmd->args);
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
         builtin_cd(cmd->args);
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
         builtin_pwd();
    else if (ft_strcmp(cmd->args[0], "export") == 0)
         builtin_export(cmd->args, env);
    else if (ft_strcmp(cmd->args[0], "unset") == 0)
         builtin_unset(cmd->args, env);
    else if (ft_strcmp(cmd->args[0], "env") == 0)
         builtin_env(*env);
    else if (ft_strcmp(cmd->args[0], "exit") == 0)
         builtin_exit(cmd->args);

    return ; // Not a builtin
}
