#include "../../include/minishell.h"

int builtin_exit(char **args)
{
    (void) args;

    free_array(args);
    exit(0);
    return (0);
}