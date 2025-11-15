#include "../../include/minishell.h"

int builtin_pwd()
{
    char *path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd");
        return (1);
    }
    printf("%s\n", path);
    free(path);
    return (0);
}