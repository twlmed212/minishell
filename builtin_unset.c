#include "minishell.h"

int builtin_unset(char **args, t_env **env)
{
    int i = 1;
    while (args[i]) {
        t_env *tmp = *env;
        t_env *prev = NULL;
        while (tmp) {
            if (ft_strcmp(tmp->key, args[i]) == 0) {
                if (prev)
                    prev->next = tmp->next;
                else
                    *env = tmp->next;
                free(tmp->key);
                free(tmp->value);
                free(tmp);
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        i++;
    }
    return 0;
}
