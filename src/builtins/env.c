#include "../../include/minishell.h"

int builtin_env(char **args)
{
    int i;
    
    i = 0;
    while (args[i])
    {
        printf("%s\n", args[i]);
        i++;
    }
    
    return (0);
}